#include "clase_ldap.h"
#include <QtDebug>
#include "lib/lib.h"
#include <QDateTime>
#include <QMessageBox>


clase_ldap::clase_ldap( QString host, int  puerto, QString usuario, QString dominio, QString password,QString *info)

{
    QString qstr;

    ldap_host     = convierte(host);
    ldap_port     = puerto;
    ldap_dn       = convierte(usuario);
    ldap_pw       = convierte(password);
    Dominio = dominio;

    rellena_dominio(&qstr);

}

//Devuelve un vector con todas las unidades organizativas en formato DN
//que existen en el dominio
QStringList clase_ldap::Consultar_OU(QString *info){

    QString basedn;//, infor;
    QStringList lista_OU;


    // variables para CONSULTA LDAP ------------------------------------------------------------------------------------------------
    LDAPMessage *resul_consul, *entry;
  //BerElement *ber;
    // Guardamos lo que devuelve entries_found que es el numero entradas encontradas para una consulta LDAP
    //int  num_entradas  = 0;
    // dn guarda el DN name string the los objetos devueltos por la consulta
  char *dn           = "";
    // atributo guarda el nombre de los atributos de los objetos devueltos
  //char *atributo     = "";
    // values is un array para guardar los valores de cada atributo, de los atributos de los objetos devueltos
  //char **values;
    char *attrs[]= {NULL};
    // variables para CONSULTA LDAP ------------------------------------------------------------------------------------------------

    if (conecta_oldap(info)){

        QString Qstr="(objectClass=organizationalUnit)";

        //basedn="DC=cantoso,DC=local";
        QString str = Dominio;

        //sacamos las partes de la cadena del dominio separadas por puntos
        QStringList strList = str.split(".");
        //lo recorremos y lo guardamos en el formato basedn
        for (int i = 0; i < strList.size(); ++i) {
            basedn = basedn + "DC=" + strList.at(i);
            //añadimos una coma si no es la ultima parte
            if (i+1 < strList.size())
                basedn=basedn+",";
        }

        resul_consul=clase_ldap::consulta_oldap(convierte(Qstr), attrs, 0, basedn.toLocal8Bit(),LDAP_SCOPE_SUBTREE,info);

        for (entry = ldap_first_entry(ldap, resul_consul); entry != NULL; entry = ldap_next_entry(ldap, entry)) {

            dn = ldap_get_dn(ldap, entry);
            //printf("Encontrado objeto con DN: %s\n", dn);

            //descartamos la UO "Domain Controllers" que contiene los DC del dominio
            if (!QString::fromStdString(dn).contains("Domain Controllers")){

                //lo convertimos a lista de cadenas
                lista_OU<<QString::fromStdString(dn);

            }

            ldap_memfree(dn);

        }

    }

    return lista_OU;
}

//introduce los datos en la estructura dominio
//con los datos del dominio del servidor ldap
bool clase_ldap::rellena_dominio(QString *info){

    QString temp, basedn;//, info;
    QDateTime fecha;
    int i;

    // variables para CONSULTA LDAP ------------------------------------------------------------------------------------------------
    LDAPMessage *resul_consul, *entry;
    BerElement *ber;
    // Guardamos lo que devuelve entries_found que es el numero entradas encontradas para una consulta LDAP
    int  num_entradas  = 0;
    // dn guarda el DN name string the los objetos devueltos por la consulta
    char *dn           = "";
    // atributo guarda el nombre de los atributos de los objetos devueltos
    char *atributo     = "";
    // values is un array para guardar los valores de cada atributo, de los atributos de los objetos devueltos
    char **values;
    char *attrs[]= {NULL};
    // variables para CONSULTA LDAP ------------------------------------------------------------------------------------------------

     if (conecta_oldap(info)){//, &info){

        //ui->label_estado->setText(info);

        QString Qstr="(&(objectClass=domain))";

        //basedn="DC=cantoso,DC=local";
        //Configuracion *configuracion = new Configuracion;
        QString str = Dominio;

        //sacamos las partes de la cadena del dominio separadas por puntos
        QStringList strList = str.split(".");
        //lo recorremos y lo guardamos en el formato basedn
        for (int i = 0; i < strList.size(); ++i) {
            basedn = basedn + "DC=" + strList.at(i);
            //añadimos una coma si no es la ultima parte
            if (i+1 < strList.size())
                basedn=basedn+",";
        }

        //qDebug()<<"AQUI 1";
        resul_consul=consulta_oldap(convierte(Qstr), attrs, 0, basedn.toLocal8Bit(),LDAP_SCOPE_BASE,info);

        // para mostrar todos los atributos del dominio
         // poner antes --->  char *attrs[]= {NULL};
        entry = ldap_first_entry(ldap, resul_consul);
        for ( atributo = ldap_first_attribute(ldap, entry, &ber);atributo != NULL;
             atributo = ldap_next_attribute(ldap, entry, ber))
        {


            //usuario
            if (QString::fromStdString(atributo)=="lockoutThreshold"){
                if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                  for (i = 0; values[i] != NULL; i++) {
                         dominio.intentos_fallidos=QString::fromStdString(values[i]).toInt();
                  }
                  ldap_value_free(values);
                }
            }

            //usuario
            if (QString::fromStdString(atributo)=="maxPwdAge"){
                if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                  for (i = 0; values[i] != NULL; i++) {

                       dominio.vigencia_clave=QString::fromStdString(values[i]).toUpper();
                  }
                  ldap_value_free(values);
                }
            }

            //usuario
            if (QString::fromStdString(atributo)=="lockoutDuration"){
                if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                  for (i = 0; values[i] != NULL; i++) {

                      dominio.tiempo_bloqueo=QString::fromStdString(values[i]).toUpper();
                  }
                  ldap_value_free(values);
                }
            }

        }

        return true;
    }
    else {
         return false;
    }


}

//función que devuelve una clase usuario
//con los datos de un usuario de ldap
usuario clase_ldap::rellena_entrada(LDAPMessage *entry){

    // variables para CONSULTA LDAP --------------------------------------------------------------------------
    //LDAPMessage *entry;//*resul_consul
    BerElement *ber;
    // Guardamos lo que devuelve entries_found que es el numero entradas encontradas para una consulta LDAP
//    int  num_entradas  = 0;
//    // dn guarda el DN name string de los objetos devueltos por la consulta
    char *dn            = "";
    // atributo guarda el nombre de los atributos de los objetos devueltos
    char *atributo     = "";
    // values es un array para guardar los valores de cada atributo, de los atributos de los objetos devueltos
    char **values;
    //char *attrs[]       = {NULL};
    // variables para CONSULTA LDAP --------------------------------------------------------------------------

    QString pwdLastSet, info;
    QString temp, basedn1;
    QDateTime fecha, ahora;
    int userAccountControl,i;
    usuario * user=new usuario;

            //Rellenamos la estructura dominio si no está rellena ya
            if (dominio.intentos_fallidos==-1){
                bool a=rellena_dominio(&info);
            }


            // Capturamos la cadena DN del objeto
            dn = ldap_get_dn(ldap, entry);
            //limpiamos el struct entrada y vector grupos
            //limpia_entrada();
            //entrada.vec_grupos.clear();

            // Estado
            //OJO!!! si no hay fijada ninguna politica de bloqueo de cuentas
            //en el dominio, el campo lockoutTime no existirá en el usuario
            //por lo tanto le asignamos el valor por defecto "Activa"
            user->carga_estado("Activa");

            // recorremos todos los atributos de cada entry
            for ( atributo = ldap_first_attribute(ldap, entry, &ber);atributo != NULL;
                 atributo = ldap_next_attribute(ldap, entry, ber))
            {

                //qDebug()<<QString::fromStdString(atributo);

                //entrada.dn=qstr;
                user->carga_dn(QString::fromStdString(dn));

                //usuario
                if (QString::fromStdString(atributo)=="sAMAccountName"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                        //ldap_sort_values(ldap,values,LDAP_SORT_AV_CMP_PROC("a","x"));
                      // recorremos todos los valores devueltos por este atributo
                      for (i = 0; values[i] != NULL; i++) {
                           user->carga_usuario(QString::fromStdString(values[i]).toUpper());
                      }
                      ldap_value_free(values);
                    }
                }

                // Nombre y apellidos
                if (QString::fromStdString(atributo)=="cn"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                        //ldap_sort_values(ldap,values,LDAP_SORT_AV_CMP_PROC("a","x"));
                      // recorremos todos los valores devueltos por este atributo
                      for (i = 0; values[i] != NULL; i++) {
                           user->carga_nombre(QString::fromStdString(values[i]).toUpper());
                      }
                      ldap_value_free(values);
                    }
                }

                // Descripcion
                if (QString::fromStdString(atributo)=="description"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                           user->carga_descripcion(QString::fromStdString(values[i]));
                      }
                      ldap_value_free(values);
                    }
                }

                // Telefono
                if (QString::fromStdString(atributo)=="telephoneNumber"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                           user->carga_telefono(QString::fromStdString(values[i]));
                      }
                      ldap_value_free(values);
                    }
                }

                // Correo
                if (QString::fromStdString(atributo)=="mail"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                           user->carga_correo(QString::fromStdString(values[i]));
                      }
                      ldap_value_free(values);
                    }
                }


//****************

                // userPrincipalName
                if (QString::fromStdString(atributo)=="userPrincipalName"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                           user->carga_user(QString::fromStdString(values[i]));
                      }
                      ldap_value_free(values);
                    }
                }
                // sAMAccountName
                if (QString::fromStdString(atributo)=="sAMAccountName"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                           user->carga_user_preW2000(QString::fromStdString(values[i]));
                      }
                      ldap_value_free(values);
                    }
                }
                // nombre
                if (QString::fromStdString(atributo)=="givenName"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                           user->carga_nombre_solo(QString::fromStdString(values[i]));
                      }
                      ldap_value_free(values);
                    }
                }
                // apellidos
                if (QString::fromStdString(atributo)=="sn"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                           user->carga_apellidos(QString::fromStdString(values[i]));
                      }
                      ldap_value_free(values);
                    }
                }
                // nombre para mostrar
                if (QString::fromStdString(atributo)=="displayName"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                           user->carga_nombre_mostrar(QString::fromStdString(values[i]));
                      }
                      ldap_value_free(values);
                    }
                }
                // movil
                if (QString::fromStdString(atributo)=="mobile"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                           user->carga_movil(QString::fromStdString(values[i]));
                      }
                      ldap_value_free(values);
                    }
                }
                // fax
                if (QString::fromStdString(atributo)=="facsimileTelephoneNumber"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                           user->carga_fax(QString::fromStdString(values[i]));
                      }
                      ldap_value_free(values);
                    }
                }
                // department
                if (QString::fromStdString(atributo)=="department"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                           user->carga_departamento(QString::fromStdString(values[i]));
                      }
                      ldap_value_free(values);
                    }
                }
                // oficina
                if (QString::fromStdString(atributo)=="physicalDeliveryOfficeName"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                           user->carga_oficina(QString::fromStdString(values[i]));
                      }
                      ldap_value_free(values);
                    }
                }
                // organizacion
                if (QString::fromStdString(atributo)=="company"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                           user->carga_organizacion(QString::fromStdString(values[i]));
                      }
                      ldap_value_free(values);
                    }
                }
                // puesto
                if (QString::fromStdString(atributo)=="title"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                           user->carga_puesto(QString::fromStdString(values[i]));
                      }
                      ldap_value_free(values);
                    }
                }
                // provincia
                if (QString::fromStdString(atributo)=="st"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                           user->carga_provincia(QString::fromStdString(values[i]));
                      }
                      ldap_value_free(values);
                    }
                }
                // localidad
                if (QString::fromStdString(atributo)=="l"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                           user->carga_localidad(QString::fromStdString(values[i]));
                      }
                      ldap_value_free(values);
                    }
                }
                // direccion
                if (QString::fromStdString(atributo)=="streetAddress"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                           user->carga_direccion(QString::fromStdString(values[i]));
                      }
                      ldap_value_free(values);
                    }
                }
                // cp
                if (QString::fromStdString(atributo)=="postalCode"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                           user->carga_cp(QString::fromStdString(values[i]));
                      }
                      ldap_value_free(values);
                    }
                }
                // notas
                if (QString::fromStdString(atributo)=="info"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                           user->carga_notas(QString::fromStdString(values[i]));
                      }
                      ldap_value_free(values);
                    }
                }




//*****************

                // Ultimo logon
                if (QString::fromStdString(atributo)=="lastLogon"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                          if (QString::fromStdString(values[i]).trimmed()!="0"){
                              fecha.setMSecsSinceEpoch((QString::fromStdString(values[i]).toLongLong()/10000)-11644473600000);
                              user->carga_ultimo_login(fecha.toString("dd-MM-yyyy  hh:mm"));
                          }
                          else
                              user->carga_ultimo_login("NUNCA");
                      }
                      ldap_value_free(values);
                    }
                }

                // la cuenta caduca
                if (QString::fromStdString(atributo)=="accountExpires"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                          if (QString::fromStdString(values[i]).toLongLong()==9223372036854775807 || QString::fromStdString(values[i]).trimmed()=="0")
                              user->carga_caduca_cuenta("No Caduca");
                              //entrada.caduca_cuenta = "";
                          else {
                              fecha.setMSecsSinceEpoch((QString::fromStdString(values[i]).toLongLong()/10000)-11644473600000);
                              user->carga_caduca_cuenta(fecha.toString("dd-MM-yyyy  hh:mm"));
                          }
                      }
                      ldap_value_free(values);
                    }
                }

                // Estado
                //OJO!!! si no hay fijada ninguna politica de bloqueo de cuentas
                //en el dominio, este campo no existirá
                if (QString::fromStdString(atributo)=="lockoutTime"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                          if (QString::fromStdString(values[i])=="0"){
                              user->carga_estado("Activa");
                              //ui->text_estado->setStyleSheet("color: rgb(11, 97, 29)");
                          }
                          else{
                              // if (lockoutTime(usuario) + lockoutDuration(dominio) < fecha actual and (badPwdCount=3))
                              // la contraseña está desbloqueada por tiempo
                              temp=QString::number(QString::fromStdString(values[i]).toLongLong()-dominio.tiempo_bloqueo.toLongLong());
                              fecha.setMSecsSinceEpoch((temp.toLongLong()/10000)-11644473600000);
                              QDateTime ahora = QDateTime::currentDateTime();
                              //qDebug()<<fecha.toString("dd-MM-yyyy  hh:mm");
                              //pwdLastSet.toLongLong()-dominio.vigencia_clave.toLongLong()
                              if (fecha<ahora){
                                  //qDebug()<<"*************la contraseña está desbloqueada por tiempo***************";
                                  user->carga_estado("Desbloqueado automático");
                              }
                              else{
                                  user->carga_estado("Bloqueada hasta " + fecha.toString("hh:mm")); //+ " del " + fecha.toString("dd-MM-yyyy");
                              }
                          }
                      }
                      ldap_value_free(values);
                    }
                }

                // Numero de logon
                if (QString::fromStdString(atributo)=="logonCount"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                           user->carga_logon(QString::fromStdString(values[i]).toInt());
                      }
                      ldap_value_free(values);
                    }
                }

                // cuando se creo la cuenta
                if (QString::fromStdString(atributo)=="whenCreated"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                          //user->carga_creada(QString::fromStdString(values[i]).mid(6,2)+"-"+QString::fromStdString(values[i]).mid(4,2)+"-"+QString::fromStdString(values[i]).mid(0,4));
                          user->carga_creada(gtime_to_string(QString::fromStdString(values[i])));
                      }
                      ldap_value_free(values);
                    }
                }

                //cuando se modifico por ultima vez la cuenta
                if (QString::fromStdString(atributo)=="whenChanged"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                          //user->carga_modificacion_cuenta(QString::fromStdString(values[i]).mid(6,2)+"-"+QString::fromStdString(values[i]).mid(4,2)+"-"+QString::fromStdString(values[i]).mid(0,4));
                          user->carga_modificacion_cuenta(gtime_to_string(QString::fromStdString(values[i])));
                      }
                      ldap_value_free(values);
                    }
                }

                // cuando se creo la cuenta de correo
                if (QString::fromStdString(atributo)=="msExchWhenMailboxCreated"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                          //user->carga_fecha_correo(QString::fromStdString(values[i]).mid(6,2)+"-"+QString::fromStdString(values[i]).mid(4,2)+"-"+QString::fromStdString(values[i]).mid(0,4));
                          user->carga_fecha_correo(gtime_to_string(QString::fromStdString(values[i])));
                      }
                      ldap_value_free(values);
                    }
                }

                // Ultimo cambio de contraseña
                // si pwdLastSet tiene valor 0 significa que el usuario debe cambiar la contraseña en el siguiente inicio de sesión
                // este valor lo vamos a guardar en otro campo y lo vamos a actualizar en la base de datos si no se ha
                // marcado la opción "cambiar la contraseña" porque sino perderiamos el valor de la fecha
                if (QString::fromStdString(atributo)=="pwdLastSet"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                          pwdLastSet=QString::fromStdString(values[i]);//para usarlo luego
                          //qDebug()<<pwdLastSet;
                          fecha.setMSecsSinceEpoch((QString::fromStdString(values[i]).toLongLong()/10000)-11644473600000);
                          user->carga_cambio_clave(fecha.toString("dd-MM-yyyy  hh:mm"));
                          if (pwdLastSet!="0"){
                                user->carga_ultimo_cambio_clave(fecha.toString("dd-MM-yyyy  hh:mm"));
                          }
                      }
                      ldap_value_free(values);
                    }
                }

                //Intentos fallidos de contraseña
                if (QString::fromStdString(atributo)=="badPwdCount"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                          user->carga_intentos(QString::fromStdString(values[i]).toInt());
                      }
                      ldap_value_free(values);
                    }
                }


                if (QString::fromStdString(atributo)=="userAccountControl"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                          if (QString::fromStdString(values[i])=="66048" || QString::fromStdString(values[i])=="66050" || QString::fromStdString(values[i])=="66082" || QString::fromStdString(values[i])=="1114624") {
                              user->carga_caduca_clave("No Caduca");
                              userAccountControl=1;//para usarlo luego
                          }
                          else{
                              userAccountControl=0;//para usarlo luego
                          }

                          //Para el userAccountControl
                          user->carga_useraccountcontrol(QString::fromStdString(values[i]).toInt());
                       }
                      ldap_value_free(values);
                    }
                }


                //grupos
                if (QString::fromStdString(atributo)=="memberOf"){
                    //entrada.vec_grupos.clear();
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {

                          //qDebug()<< usuario;
                          //qDebug()<< QString::fromStdString(values[i]).toUpper();
                          //grupos.usuario=entrada.usuario;
                          //grupos.nombre=QString::fromStdString(values[i]).toUpper();
                          //entrada.vec_grupos.push_back(grupos);
                          user->carga_grupos(QString::fromStdString(values[i]).toUpper());
                      }
                      //qDebug()<<vec_grupos.count();
                      ldap_value_free(values);
                    }
                }

            }

            ldap_memfree(dn);

            //INICIO_______ si la clave caduca rellenamos el campo CLAVE CADUCA***********************************************************************************
            // no se puede poner dentro del for que recorre los atributos porque pwdLastSet
            // se resuelve despues de userAccountControl y no funciona
            if (userAccountControl==0) {
                temp=QString::number(pwdLastSet.toLongLong()-dominio.vigencia_clave.toLongLong());
                fecha.setMSecsSinceEpoch((temp.toLongLong()/10000)-11644473600000);
                user->carga_caduca_clave(fecha.toString("dd-MM-yyyy  hh:mm"));
            }

            //fecha de sincronizacion
            user->carga_ultima_sincro(QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss"));
            //user->carga_ultima_sincro(QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss:zzz"));//con precisión de milisegundos

            return (*user);

}

//conecta con el servidor LDAP y devuelve si se ha conseguido conectar o no
//en el parametro "info" devuelve una cadena de informacion sobre el resultado de la conexión
bool clase_ldap::conecta_oldap(QString *info) {

    int  result;
    QString Qstr;

    result = ldap_set_option(ldap, LDAP_OPT_PROTOCOL_VERSION, &ldap_version);
    result = ldap_set_option(ldap, LDAP_OPT_SIZELIMIT, &max_result );

    //para permitir ldap sobre ssl
    int reqcert = LDAP_OPT_X_TLS_ALLOW;
    //result = ldap_set_option(NULL, LDAP_OPT_X_TLS_REQUIRE_CERT, &reqcert);
    result = ldap_set_option(ldap, LDAP_OPT_X_TLS_REQUIRE_CERT, &reqcert);

    if (result != LDAP_OPT_SUCCESS)
    {
        fprintf(stderr, "unable to set require cert option (LDAP_OPT_X_TLS_REQUIRE_CERT): %s\n", ldap_err2string(result));
    }

    // STEP 1: Establecer la conexión LDAP y fijar preferencias de la sesión ***************************************
     Qstr="ldap://" + QString::fromStdString(ldap_host) + ":" + QString::number(ldap_port);
     if(ldap_initialize(&ldap, Qstr.toLocal8Bit())) {
         *info=QString ("ERROR: No ha sido posible conectarse al servidor %1 en el puerto %2  Error:%3").arg(ldap_host).arg(ldap_port).arg(ldap_err2string(result));
         qDebug()<<"error";
         return false;
     }
     *info=QString ("Conectado al servidor %1 en el puerto %2...").arg(ldap_host).arg(ldap_port);

     //FIN ldap_initialize ******************************************************************************************

     //result = ldap_set_option(ldap, LDAP_OPT_PROTOCOL_VERSION, &ldap_version);

     if ( result != LDAP_OPT_SUCCESS ) {
         *info=QString ("ERROR: No ha sido posible conectarse al servidor %1 en el puerto %2...revise la configuración").arg(ldap_host).arg(ldap_port);
         return false;
     }
     else {
         *info=QString ("Conectado al servidor %1 en el puerto %2 version del cliente LDAPv3 ").arg(ldap_host).arg(ldap_port);
     }

     /* STEP 2: Autentificamos al usuario en el servidor *************************************************************/
     result = ldap_simple_bind_s(ldap, ldap_dn, ldap_pw );

     if ( result != LDAP_SUCCESS ) {
         *info=QString ("ERROR: No ha sido posible iniciar sesion con el usuario %1 Error:%2").arg(ldap_dn).arg(ldap_err2string(result));
         return false;
     }
     else {
         *info=QString ("Conectado al servidor %1 en el puerto %2 version del cliente LDAPv3 ").arg(ldap_host).arg(ldap_port);
     }

    return true;

}

//realiza una consulta ldap y devuelve el resultado (LDAPMessage)
//en el parametro "info" devuelve una cadena de informacion sobre el resultado de la consulta
LDAPMessage * clase_ldap::consulta_oldap(char *filtro, char *attrs[], int attrsonly, const char * base_dn, int scope, QString *info){


    //*******************************************************************************************************************
    // El filtro de busqueda, "(objectClass=*)" devuelve todos los objetos. /
    // Windows puede devolver 1000 objetos en una busqueda. Si se sebrepasa devuelve "Size limit exceeded"
    //char *filtro        = "(&(objectClass=user)(sAMAccountName=si_*))";
    //*******************************************************************************************************************

    //*******************************************************************************************************************
    // Array de atributos que se devolverán, usar {NULL} para devolver todos los atributos del objeto *******************
    // Siempre se debe poner NULL como ultimo parametro
    //char *attrs[]       = {"objectSid","userPassword", NULL};
    //char *attrs[]       = {NULL};
    //*******************************************************************************************************************

    // Devolver solo nombres de atributos (1), o devolver nombre y valores (0)
    //int  attrsonly      = 0;

    int  result;
    LDAPMessage *answer;

    result = ldap_search_s(ldap, base_dn, scope, filtro, attrs, attrsonly, &answer);
    if ( result != LDAP_SUCCESS ) {
        *info=QString ("ERROR: No ha sido posible realizar la consulta ldap Error:%1").arg(ldap_err2string(result));
    }

    return (answer);
}


//devuelve el numero de entradas que devuelve resul_consul
int clase_ldap::num_entradas_oldap(LDAPMessage *resul_consul){

    // Devuelve el numero de objetos encontrados durante la busqueda
    int num_entradas = ldap_count_entries(ldap, resul_consul);

    return(num_entradas);

}

//devuelve un objeto usuario pasandole el nombre de la cuenta y el DN del usuario
//devuelve mensaje de información en el parametro info
usuario clase_ldap::Consulta_Usuario(QString cuenta_usuario, QString qstr_dn, QString *info){

    //    // variables para CONSULTA LDAP --------------------------------------------------------------------------
        LDAPMessage *resul_consul, *entry;
    //    BerElement *ber;
        // Guardamos lo que devuelve entries_found que es el numero entradas encontradas para una consulta LDAP
        int  num_entradas  = 0;
        // dn guarda el DN name string de los objetos devueltos por la consulta
    //    char *dn            = "";
        // atributo guarda el nombre de los atributos de los objetos devueltos
    //    char *atributo     = "";
        // values es un array para guardar los valores de cada atributo, de los atributos de los objetos devueltos
    //    char **values;
        char *attrs[]       = {NULL};
    //usuario clase_ldap::rellena_entrada(LDAPMessage *entry);

     if (conecta_oldap(info)){

       qstr_dn.remove(0,qstr_dn.indexOf(",OU=")+1);
       QString qstr_consul="(&(objectClass=user)(sAMAccountName=" + cuenta_usuario + "))";

       resul_consul=consulta_oldap(convierte(qstr_consul), attrs, 0, qstr_dn.toLocal8Bit(),LDAP_SCOPE_SUBTREE,info);

       // Devuelve el numero de objetos encontrados durante la busqueda
       num_entradas = num_entradas_oldap(resul_consul);
       if ( num_entradas == 0 ) {
           *info="ERROR: La consulta ldap no ha devuelto ningun resultado";
           usuario * user=new usuario;
           return(*user);
       }
       else {
           *info=QString("La consulta ldap ha devuelto %1 resultados").arg(num_entradas);
       }

      // recorremos todos los objetos (entry) devueltos por la consulta
      // cada entry tiene atributos
      for ( entry = ldap_first_entry(ldap, resul_consul); entry != NULL; entry = ldap_next_entry(ldap, entry))
      {

          return(rellena_entrada(entry));
          //ldap_msgfree(resul_consul);
      }

    }

}


//devuelve un vector de usuarios pasandole el DN de la Unidad Organizativa
//devuelve mensaje de información en el parametro info
QVector<usuario> clase_ldap::Consulta_Usuarios(QString qstr_dn_OU, QString *info){

    QVector <usuario> usuarios;

    //    // variables para CONSULTA LDAP --------------------------------------------------------------------------
        LDAPMessage *resul_consul, *entry;
    //    BerElement *ber;
        // Guardamos lo que devuelve entries_found que es el numero entradas encontradas para una consulta LDAP
        int  num_entradas  = 0;
        // dn guarda el DN name string de los objetos devueltos por la consulta
    //    char *dn            = "";
        // atributo guarda el nombre de los atributos de los objetos devueltos
    //    char *atributo     = "";
        // values es un array para guardar los valores de cada atributo, de los atributos de los objetos devueltos
    //    char **values;
        char *attrs[]       = {NULL};
    //    // variables para CONSULTA LDAP --------------------------------------------------------------------------

    if (conecta_oldap(info)){

        //realizamos la consulta con LDAP_SCOPE_ONELEVEL en vez de LDAP_SCOPE_SUBTREE
        //puesto que seleccionamos explicitamente todas las OU deseadas en configuracion
        resul_consul=consulta_oldap("(&(!(objectclass=computer))(objectClass=user))", attrs, 0, qstr_dn_OU.toLocal8Bit(),LDAP_SCOPE_ONELEVEL ,info);

        // Devuelve el numero de objetos encontrados durante la busqueda
        num_entradas = num_entradas_oldap(resul_consul);
        if ( num_entradas == 0 ) {
            *info=QString ("ERROR: La consulta ldap no ha devuelto ningun resultado");
            //QMessageBox::critical(this, "Ldap Error", "LDAP no ha devuelto ningun resultado\nRevise la configuracion de ldap o la consulta", QMessageBox::Ok);

        }
        else {
            *info=QString ("La consulta ldap ha devuelto %1 resultados").arg(num_entradas);

           // recorremos todos los objetos (entry) devueltos por la consulta
           // cada entry tiene atributos
           for ( entry = ldap_first_entry(ldap, resul_consul); entry != NULL; entry = ldap_next_entry(ldap, entry))
           {
               usuarios.push_back(rellena_entrada(entry));
            }
        }

    }

    return(usuarios);
   // ldap_msgfree(resul_consul);

}


void clase_ldap::desbloquear_usuario(QString DN_usuario, QString *info)
{

     if (conecta_oldap(info)){

         LDAPMod attribute1;

        char *modificaciones_values[] = { "0", NULL };
        attribute1.mod_type = "lockoutTime";
        attribute1.mod_op = LDAP_MOD_REPLACE;
        attribute1.mod_values = modificaciones_values;

        LDAPMod *mods[2];
        mods[0]=&attribute1;
        mods[1]=NULL;

        int result=  ldap_modify_ext_s( ldap, convierte(DN_usuario), mods,NULL,NULL);

        if ( result == LDAP_SUCCESS )
            *info="Modificación satisfactoria del atributo del usuario: " + DN_usuario;
        else {
            *info=QString("ERROR: Modificando atributo del usuario: "
                          + DN_usuario + (" ldap Error:%1")).arg(ldap_err2string(result));
        }

    }

}


//renovar caducidad de la contraseña
void clase_ldap::renovar_clave_usuario(QString DN_usuario, QString *info)
{
    //consiste en modificar el campo pwdLastSet=0 y luego pwdLastSet=-1

    if (conecta_oldap(info)){

        LDAPMod attribute1;

        char *modificaciones_values[] = { "0", NULL };
        attribute1.mod_type = "pwdLastSet";
        attribute1.mod_op = LDAP_MOD_REPLACE;
        attribute1.mod_values = modificaciones_values;

        LDAPMod *mods[2];
        mods[0]=&attribute1;
        mods[1]=NULL;

        int result=  ldap_modify_ext_s( ldap, convierte(DN_usuario), mods,NULL,NULL);

        if ( result == LDAP_SUCCESS )
            *info="Modificación satisfactoria del atributo del usuario: " + DN_usuario;
        else {
            *info=QString("ERROR: Modificando atributo del usuario: " + DN_usuario + (" ldap Error:%1")).arg(ldap_err2string(result));
        }

        char *modificaciones_values2[] = { "-1", NULL };
        attribute1.mod_type = "pwdLastSet";
        attribute1.mod_op = LDAP_MOD_REPLACE;
        attribute1.mod_values = modificaciones_values2;

        mods[0]=&attribute1;
        mods[1]=NULL;

        result=  ldap_modify_ext_s( ldap, convierte(DN_usuario), mods,NULL,NULL);

        if ( result == LDAP_SUCCESS )
            *info="Modificación satisfactoria del atributo del usuario: " + DN_usuario;
        else {
            *info=QString("ERROR: Modificando atributo del usuario: "
                          + DN_usuario + (" ldap Error:%1")).arg(ldap_err2string(result));
        }

    }

}


//habilita o deshabilita el usuario actual, dependiendo de su estado actual
void clase_ldap::habilitar_usuario(QString cuenta_usuario, QString DN_usuario, QString *info)
{

    int valor,uac;
    usuario *usu=new usuario;

    *usu=Consulta_Usuario(cuenta_usuario,DN_usuario,info);
    uac=usu->cual_es_useraccountcontrol();

    delete usu;

    if (estado_habilitado(uac)!=2){
        //qDebug()<<uac;

        if (estado_habilitado(uac)==0)
            valor=uac-2;
        else
            valor=uac+2;

        if (conecta_oldap(info)){
            LDAPMod attribute1;

            char *modificaciones_values[] = {convierte(QString::number(valor)), NULL };
            attribute1.mod_type = "UserAccountControl";
            attribute1.mod_op = LDAP_MOD_REPLACE;
            attribute1.mod_values = modificaciones_values;

            LDAPMod *mods[2];
            mods[0]=&attribute1;
            mods[1]=NULL;

            int result=  ldap_modify_ext_s( ldap, convierte(DN_usuario), mods,NULL,NULL);

            if ( result == LDAP_SUCCESS )
                *info="Modificación satisfactoria del atributo del usuario: " + DN_usuario;
            else {
                *info=QString("ERROR: Modificando atributo del usuario: "
                              + DN_usuario + (" ldap Error:%1")).arg(ldap_err2string(result));
            }

      }

    }

}

//función para guardar la fecha de caducidad de la cuenta del usuario
void clase_ldap::guardar_fecha_caduca(QString fecha, QString DN_usuario, QString *info)
{

    LDAPMod attribute1;
    QDateTime fecha1;
    QString sfecha;

    //si no se encuentra coincidencia
    if (!es_fecha(fecha)){
        sfecha="9223372036854775807";// indicates that the account never expires
    }
    //guardamos la fecha de caducidad
    else {
        fecha1=QDateTime::fromString(fecha,"dd-MM-yyyy  hh:mm");
        sfecha=QString::number((fecha1.toMSecsSinceEpoch()+11644473600000)*10000);
    }

    if (conecta_oldap(info)){

       char *modificaciones_values[] = {convierte(sfecha), NULL };
       attribute1.mod_type = "accountExpires";
       attribute1.mod_op = LDAP_MOD_REPLACE;
       attribute1.mod_values = modificaciones_values;

       LDAPMod *mods[2];
       mods[0]=&attribute1;
       mods[1]=NULL;

       int result=  ldap_modify_ext_s( ldap, convierte(DN_usuario), mods,NULL,NULL);

       if ( result == LDAP_SUCCESS )
           *info="Modificación satisfactoria del atributo del usuario: " + DN_usuario;
       else {
           *info=QString("ERROR: Modificando atributo del usuario: " + DN_usuario
                         + (" ldap Error:%1")).arg(ldap_err2string(result));
       }
    }

}

//inserta el usuario DN_usuario en el grupo DN_grupo
void clase_ldap::miembro_grupo(QString DN_grupo, QString DN_usuario, QString *info)
{

     if (conecta_oldap(info)){

         LDAPMod attribute1;

        char *modificaciones_values[] = { convierte(DN_usuario), NULL };
        attribute1.mod_type = "member";
        attribute1.mod_op = LDAP_MOD_ADD;
        attribute1.mod_values = modificaciones_values;

        LDAPMod *mods[2];
        mods[0]=&attribute1;
        mods[1]=NULL;

        int result=  ldap_modify_ext_s( ldap, convierte(DN_grupo), mods, NULL, NULL);

        if ( result == LDAP_SUCCESS )
            *info="Modificación satisfactoria del atributo del grupo: " + DN_grupo;
        else {
            *info=QString("ERROR: Modificando atributo del grupo: "
                          + DN_grupo + (" ldap Error:%1")).arg(ldap_err2string(result));
        }

    }

}


//quita al usuario DN_usuario como miembro del grupo DN_grupo
void clase_ldap::quitar_grupo(QString DN_grupo, QString DN_usuario, QString *info)
{

     if (conecta_oldap(info)){

         LDAPMod attribute1;

        char *modificaciones_values[] = { convierte(DN_usuario), NULL };
        attribute1.mod_type = "member";
        attribute1.mod_op = LDAP_MOD_DELETE;
        attribute1.mod_values = modificaciones_values;

        LDAPMod *mods[2];
        mods[0]=&attribute1;
        mods[1]=NULL;

        int result=  ldap_modify_ext_s( ldap, convierte(DN_grupo), mods, NULL, NULL);

        if ( result == LDAP_SUCCESS )
            *info="Modificación satisfactoria del atributo del grupo: " + DN_grupo;
        else {
            *info=QString("ERROR: Modificando atributo del grupo: "
                          + DN_grupo + (" ldap Error:%1")).arg(ldap_err2string(result));
        }

    }

}



//guarda los datos del correo del usuario en el directorio LDAP
void clase_ldap::guarda_correo(QString correo, QString DN_usuario, QString *info){

    if (conecta_oldap(info)){

       LDAPMod attribute1;

       char *modificaciones_values[] = {convierte(correo), NULL };
       attribute1.mod_type = "mail";
       attribute1.mod_op = LDAP_MOD_REPLACE;
       attribute1.mod_values = modificaciones_values;

       LDAPMod *mods[2];
       mods[0]=&attribute1;
       mods[1]=NULL;

       int result=  ldap_modify_ext_s( ldap, convierte(DN_usuario), mods,NULL,NULL);

       if ( result == LDAP_SUCCESS )
           *info="Modificación satisfactoria del atributo del usuario: " + DN_usuario;
       else {
           *info=QString("ERROR: Modificando atributo del usuario: " + DN_usuario
                         + (" ldap Error:%1")).arg(ldap_err2string(result));
       }
    }

}


//guarda los datos del telefono del usuario en el directorio LDAP
void clase_ldap::guarda_telefono(QString telefono, QString DN_usuario, QString *info){

    if (conecta_oldap(info)){

       LDAPMod attribute1;

       char *modificaciones_values[] = {convierte(telefono), NULL };
       attribute1.mod_type = "telephoneNumber";
       attribute1.mod_op = LDAP_MOD_REPLACE;
       attribute1.mod_values = modificaciones_values;

       LDAPMod *mods[2];
       mods[0]=&attribute1;
       mods[1]=NULL;

       int result=  ldap_modify_ext_s( ldap, convierte(DN_usuario), mods,NULL,NULL);

       if ( result == LDAP_SUCCESS )
           *info="Modificación satisfactoria del atributo del usuario: " + DN_usuario;
       else {
           *info=QString("ERROR: Modificando atributo del usuario: " + DN_usuario
                         + (" ldap Error:%1")).arg(ldap_err2string(result));
       }
    }
}

//guarda los datos de la descripcion del usuario en el directorio LDAP
void clase_ldap::guarda_descripcion(QString descripcion, QString DN_usuario, QString *info){

    if (conecta_oldap(info)){

       LDAPMod attribute1;

       char *modificaciones_values[2];

       attribute1.mod_type = "description";
       if (!descripcion.isEmpty()){
           modificaciones_values[0]=convierte(descripcion);
           modificaciones_values[1]=NULL;
           attribute1.mod_op = LDAP_MOD_REPLACE;
       }
       else{
           modificaciones_values[0]=NULL;
           attribute1.mod_op = LDAP_MOD_DELETE;
       }

       attribute1.mod_values = modificaciones_values;

       LDAPMod *mods[2];
       mods[0]=&attribute1;
       mods[1]=NULL;

       int result=  ldap_modify_ext_s( ldap, convierte(DN_usuario), mods,NULL,NULL);

       if ( result == LDAP_SUCCESS )
           *info="Modificación satisfactoria del atributo del usuario: " + DN_usuario;
       else {
           *info=QString("ERROR descripcion: Modificando atributo del usuario: " + DN_usuario
                         + (" ldap Error:%1")).arg(ldap_err2string(result));
       }
    }
}


//el usuario debe cambiar la contraseña en el siguiente inicio de sesión
void clase_ldap::guarda_debe_cambiar_pass(bool valor, QString DN_usuario, QString *info)
{
    //consiste en modificar el campo
    //pwdLastSet=0 -> activa "User Must Change Password at Next Logon"
    //pwdLastSet=-1 -> desactiva "User Must Change Password at Next Logon"

    QString v;

    if (valor==true)
        v="0";
    else
        v="-1";

    if (conecta_oldap(info)){

        LDAPMod attribute1;

        char *modificaciones_values2[] = { convierte(v), NULL };
        attribute1.mod_type = "pwdLastSet";
        attribute1.mod_op = LDAP_MOD_REPLACE;
        attribute1.mod_values = modificaciones_values2;

        LDAPMod *mods[2];
        mods[0]=&attribute1;
        mods[1]=NULL;

        int result=  ldap_modify_ext_s( ldap, convierte(DN_usuario), mods,NULL,NULL);

        if ( result == LDAP_SUCCESS )
            *info="Modificación satisfactoria del atributo del usuario: " + DN_usuario;
        else {
            *info=QString("ERROR cambiar pass: Modificando atributo del usuario: "
                          + DN_usuario + (" ldap Error:%1")).arg(ldap_err2string(result));
        }

    }

}


//cambia el valor de UAC la contraseña no expira
void clase_ldap::guarda_pass_nunca_expira(bool no_expira, QString cuenta_usuario, QString DN_usuario, QString *info)
{

    int valor,uac;
    bool modificar=false;
    usuario *usu=new usuario;

    *usu=Consulta_Usuario(cuenta_usuario,DN_usuario,info);
    uac=usu->cual_es_useraccountcontrol();

    //qDebug()<<uac;

    delete usu;

    //si actualmente la contraseña expira y se ha marcado que no expire
    if (uac>=512 && uac<=640 && no_expira){
        valor=uac+65536;
        modificar=true;
     }
     else
        //si actualmente la contraseña no expira y no se ha marcado que no expire
        if (uac>=66048 && uac<=66176 && !no_expira){
            valor=uac-65536;
            modificar=true;
        }

    if (conecta_oldap(info) && modificar){
        LDAPMod attribute1;

        char *modificaciones_values[] = {convierte(QString::number(valor)), NULL };
        attribute1.mod_type = "UserAccountControl";
        attribute1.mod_op = LDAP_MOD_REPLACE;
        attribute1.mod_values = modificaciones_values;

        LDAPMod *mods[2];
        mods[0]=&attribute1;
        mods[1]=NULL;

        int result=  ldap_modify_ext_s( ldap, convierte(DN_usuario), mods,NULL,NULL);

        if ( result == LDAP_SUCCESS )
            *info="Modificación satisfactoria del atributo del usuario: " + DN_usuario;
        else {
            *info=QString("ERROR pass nunca expira: Modificando atributo del usuario: "
                          + DN_usuario + (" ldap Error:%1")).arg(ldap_err2string(result));
        }
  }

}


//Devuelve un vector con todos los grupos en formato DN
//que existen en el dominio
QStringList clase_ldap::Consultar_grupos_dominio(QString *info){

    QString basedn;//, infor;
    QStringList lista_grupos;


    // variables para CONSULTA LDAP ------------------------------------------------------------------------------------------------
    LDAPMessage *resul_consul, *entry;
  //BerElement *ber;
    // Guardamos lo que devuelve entries_found que es el numero entradas encontradas para una consulta LDAP
    //int  num_entradas  = 0;
    // dn guarda el DN name string the los objetos devueltos por la consulta
  char *dn           = "";
    // atributo guarda el nombre de los atributos de los objetos devueltos
  //char *atributo     = "";
    // values is un array para guardar los valores de cada atributo, de los atributos de los objetos devueltos
  //char **values;
    char *attrs[]= {NULL};
    // variables para CONSULTA LDAP ------------------------------------------------------------------------------------------------

    if (conecta_oldap(info)){

        QString Qstr="(objectClass=group)";

        QString str = Dominio;

       //sacamos las partes de la cadena del dominio separadas por puntos
        QStringList strList = str.split(".");
        //lo recorremos y lo guardamos en el formato basedn
        for (int i = 0; i < strList.size(); ++i) {
            basedn = basedn + "DC=" + strList.at(i);
            //añadimos una coma si no es la ultima parte
            if (i+1 < strList.size())
                basedn=basedn+",";
        }

        resul_consul=clase_ldap::consulta_oldap(convierte(Qstr), attrs, 0, basedn.toLocal8Bit(),LDAP_SCOPE_SUBTREE,info);

        for (entry = ldap_first_entry(ldap, resul_consul); entry != NULL; entry = ldap_next_entry(ldap, entry)) {

            dn = ldap_get_dn(ldap, entry);
            //printf("Encontrado objeto con DN: %s\n", dn);

            //descartamos la UO "Domain Controllers" que contiene los DC del dominio
            //if (!QString::fromStdString(dn).contains("Domain Controllers")){

                //lo convertimos a lista de cadenas
                lista_grupos<<QString::fromStdString(dn);

            //}

            ldap_memfree(dn);

        }

    }

    return lista_grupos;
}


//Devuelve un vector con todos los grupos en formato DN
//que existen en las Unidades Organizativas seleccionadas
QStringList clase_ldap::Consultar_grupos_OU(QString qstr_dn_OU, QString *info){

    QStringList lista_grupos;

    //    // variables para CONSULTA LDAP --------------------------------------------------------------------------
        LDAPMessage *resul_consul, *entry;
    //    BerElement *ber;
        // Guardamos lo que devuelve entries_found que es el numero entradas encontradas para una consulta LDAP
        int  num_entradas  = 0;
        // dn guarda el DN name string de los objetos devueltos por la consulta
        char *dn            = "";
        // atributo guarda el nombre de los atributos de los objetos devueltos
    //    char *atributo     = "";
        // values es un array para guardar los valores de cada atributo, de los atributos de los objetos devueltos
    //    char **values;
        char *attrs[]       = {NULL};
    //    // variables para CONSULTA LDAP --------------------------------------------------------------------------

    if (conecta_oldap(info)){

        //realizamos la consulta con LDAP_SCOPE_ONELEVEL en vez de LDAP_SCOPE_SUBTREE
        //puesto que seleccionamos explicitamente todas las OU deseadas en configuracion
        resul_consul=consulta_oldap("(&(!(objectclass=computer))(objectClass=group))", attrs, 0, qstr_dn_OU.toLocal8Bit(),LDAP_SCOPE_ONELEVEL ,info);

        // Devuelve el numero de objetos encontrados durante la busqueda
        num_entradas = num_entradas_oldap(resul_consul);
        if ( num_entradas == 0 ) {
            *info=QString ("ERROR: La consulta ldap no ha devuelto ningun resultado");
            //QMessageBox::critical(this, "Ldap Error", "LDAP no ha devuelto ningun resultado\nRevise la configuracion de ldap o la consulta", QMessageBox::Ok);

        }
        else {
            *info=QString ("La consulta ldap ha devuelto %1 resultados").arg(num_entradas);

           // recorremos todos los objetos (entry) devueltos por la consulta
           // cada entry tiene atributos
           for ( entry = ldap_first_entry(ldap, resul_consul); entry != NULL; entry = ldap_next_entry(ldap, entry))
           {
               dn = ldap_get_dn(ldap, entry);
               lista_grupos<<QString::fromStdString(dn);
               ldap_memfree(dn);

            }
        }

    }


    return lista_grupos;
}

//guarda "otros datos" del usuario en el directorio LDAP
void clase_ldap::guarda_otrosdatos(QStringList otrosdatos_valor, QString cuenta_usuario, QString DN_usuario, QString *info){

    int cuantos_modificar=0;

    //se gener un error cuando se intenta eliminar un atributo que no existe en el usuario
    //pero si se puede insertar datos en ellos
    //para evitar esto controlamos cuales son los datos validos para modificar
    //que serán aquellos que no cumplan:
    //que no existen en el usuario y que además no llevan datos para modificar
    //con lo cual estariamos intentando eliminar un atributo que no existe

    if (conecta_oldap(info)){

        QStringList otrosdatos_nombre;
        /*otrosdatos_nombre<<"userPrincipalName";
        otrosdatos_nombre<<"sAMAccountName";*/
        otrosdatos_nombre<<"givenName";
        otrosdatos_nombre<<"sn";
        otrosdatos_nombre<<"displayName";
        otrosdatos_nombre<<"mobile";
        otrosdatos_nombre<<"facsimileTelephoneNumber";
        otrosdatos_nombre<<"department";
        otrosdatos_nombre<<"physicalDeliveryOfficeName";
        otrosdatos_nombre<<"company";
        otrosdatos_nombre<<"title";
        otrosdatos_nombre<<"st";
        otrosdatos_nombre<<"l";
        otrosdatos_nombre<<"streetAddress";
        otrosdatos_nombre<<"postalCode";
        otrosdatos_nombre<<"info";

        QVector <bool> existe;

        for (int j=0; j<otrosdatos_nombre.size(); j++) {

            //que no existen en el usuario y que además no llevan datos para modificar
            //se marca para luego saltarnoslo
            if (!existe_atributo(cuenta_usuario ,DN_usuario, otrosdatos_nombre[j]) && otrosdatos_valor[j].isEmpty()){
                existe.append(false);
            }
            else{
                existe.append(true);
                cuantos_modificar++;
            }
        }

       LDAPMod attribute1[cuantos_modificar];

       struct m
       {
          char * modificaciones_values[2];
       };

       struct m vec[cuantos_modificar];

       LDAPMod *mods[cuantos_modificar+1];
       int a=0;

       for (int i=0; i<otrosdatos_nombre.size(); i++){
          if (existe[i]){
              attribute1[a].mod_type=convierte(otrosdatos_nombre[i]);
              if (!otrosdatos_valor[i].isEmpty()){
                   vec[a].modificaciones_values[0]=convierte(otrosdatos_valor[i]);
                   vec[a].modificaciones_values[1]=NULL;
                   attribute1[a].mod_op = LDAP_MOD_REPLACE;
               }
               else{
                   vec[a].modificaciones_values[0]=NULL;
                   attribute1[a].mod_op = LDAP_MOD_DELETE;
               }

               attribute1[a].mod_values =vec[a].modificaciones_values;
               mods[a]=&(attribute1[a]);
               a++;
          }
       }


       mods[cuantos_modificar]=NULL;

       int result=  ldap_modify_ext_s( ldap, convierte(DN_usuario), mods,NULL,NULL);

       if ( result == LDAP_SUCCESS )
           *info="Modificación satisfactoria del atributo del usuario: " + DN_usuario;
       else {
           *info=QString("ERROR %1: Modificando atributo del usuario: " + DN_usuario
                         + (" ldap Error:")).arg(ldap_err2string(result));
       }
    }
}


//función que devuelve si existe el atributo en un usuario
bool clase_ldap::existe_atributo(QString cuenta_usuario, QString qstr_dn, QString atrib){

    QString info;
    //    // variables para CONSULTA LDAP --------------------------------------------------------------------------
        LDAPMessage *resul_consul, *entry;
    //    BerElement *ber;
        // Guardamos lo que devuelve entries_found que es el numero entradas encontradas para una consulta LDAP
        int  num_entradas  = 0;
        char *attrs[]       = {NULL};
    //usuario clase_ldap::rellena_entrada(LDAPMessage *entry);
        BerElement *ber;
        char *dn            = "";
        // atributo guarda el nombre de los atributos de los objetos devueltos
        char *atributo     = "";
        // values es un array para guardar los valores de cada atributo, de los atributos de los objetos devueltos

     if (conecta_oldap(&info)){

       qstr_dn.remove(0,qstr_dn.indexOf(",OU=")+1);
       QString qstr_consul="(&(objectClass=user)(sAMAccountName=" + cuenta_usuario + "))";

       resul_consul=consulta_oldap(convierte(qstr_consul), attrs, 0, qstr_dn.toLocal8Bit(),LDAP_SCOPE_SUBTREE,&info);

       // Devuelve el numero de objetos encontrados durante la busqueda
       num_entradas = num_entradas_oldap(resul_consul);
       if ( num_entradas == 0 ) {
           info="ERROR: La consulta ldap no ha devuelto ningun resultado";
           usuario * user=new usuario;
           //return(*user);
       }
       else {
           info=QString("La consulta ldap ha devuelto %1 resultados").arg(num_entradas);
       }

      // recorremos todos los objetos (entry) devueltos por la consulta
      // cada entry tiene atributos
      for ( entry = ldap_first_entry(ldap, resul_consul); entry != NULL; entry = ldap_next_entry(ldap, entry))
      {

          // recorremos todos los atributos de cada entry
          for ( atributo = ldap_first_attribute(ldap, entry, &ber);atributo != NULL;
               atributo = ldap_next_attribute(ldap, entry, ber))
          {

              if (QString::fromStdString(atributo)==atrib){
                return(true);

              }

         }
       }
     }


     return(false);

}



clase_ldap::~clase_ldap() {

    ldap_unbind(ldap);

}
