#include "form_usuarios.h"

 LDAP *ldap;

//convierte QString a char *
char* form_usuarios::convierte(QString dato){
    char* cstr;
    std::string fname = dato.toStdString();
    cstr = new char [fname.size()+1];
    strcpy( cstr, fname.c_str() );
    return cstr;
}

form_usuarios::form_usuarios(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::form_usuarios)
{
    ui->setupUi(this);

//------------------SQLITE

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    if (!db.open()) {
        QMessageBox::critical(nullptr, QObject::tr("No he podido crear la DB"),
        QObject::tr("No ha sido posible crear la base de datos temporal de SQLITE.\n"
                    "Comprueba que tengas instaladas las librerias de SQLITE"), QMessageBox::Cancel);
        return;
    }


        QSqlQuery query;
        query.exec("create table ldap (id int primary key,"
               "usuario varchar(20),"
               "nombre varchar (100), "
               "cambio_clave varchar(10),"
               "clave_caducada varchar(10),"
               "correo varchar(100),"
               "creada varchar(10),"
               "estado varchar(10),"
               "fecha_correo varchar(10),"
               "intentos varchar(2),"
               "logon varchar(6),"
               "modificacion_cuenta varchar(10),"
               "telefono varchar(10),"
               "ultimo_login varchar(10),"
               "descripcion varchar(100) )");




//-------------------
   // rellenamos el combobox con todos los usuarios del dominio de las OU que se usan

    // variables para CONSULTA LDAP --------------------------------------------------------------------------
    LDAPMessage *resul_consul, *entry;
    BerElement *ber;
    // Guardamos lo que devuelve entries_found que es el numero entradas encontradas para una consulta LDAP
    int  num_entradas  = 0;
    // dn guarda el DN name string de los objetos devueltos por la consulta
    char *dn            = "";
    // atributo guarda el nombre de los atributos de los objetos devueltos
    char *atributo     = "";
    // values es un array para guardar los valores de cada atributo, de los atributos de los objetos devueltos
    char **values;
    char *attrs[]       = {NULL};
    // variables para CONSULTA LDAP --------------------------------------------------------------------------


    if (conecta_oldap()){

        // lo usamos para ir guardando los resultados de las consultas, ordenarlo y luego se lo pasamos al combo
        QStringList a;
        QStringList b;
        int id_tmp=0;

        if (carga_OU()){ //Si hay unidades organizativas realizamos las búsquedas
        int i=0;
        //recorremos todo OU
        foreach (const QString &qstr, OU) {

            resul_consul=consulta_oldap("(&(!(objectclass=computer))(objectClass=user))", attrs, 0, qstr.toLocal8Bit(),LDAP_SCOPE_SUBTREE);

            // Devuelve el numero de objetos encontrados durante la busqueda
            num_entradas = ldap_count_entries(ldap, resul_consul);
            if ( num_entradas == 0 ) {
               QMessageBox::critical(this, "Ldap Error", "LDAP no ha devuelto ningun resultado\nRevise la configuracion de ldap o la consulta", QMessageBox::Ok);
            }
            else {
               printf("La búsqueda LDAP ha devuelto %d objectos.\n", num_entradas);

               // recorremos todos los objetos (entry) devueltos por la consulta
               // cada entry tiene atributos
               for ( entry = ldap_first_entry(ldap, resul_consul); entry != NULL; entry = ldap_next_entry(ldap, entry))
               {

                    // Capturamos la cadena DN del objeto
                    dn = ldap_get_dn(ldap, entry);
                    //limpiamos el struct entrada
                    limpia_entrada();

                    // recorremos todos los atributos de cada entry
                    for ( atributo = ldap_first_attribute(ldap, entry, &ber);atributo != NULL;
                         atributo = ldap_next_attribute(ldap, entry, ber))
                    {



                        if (QString::fromStdString(atributo)=="sAMAccountName"){
                            if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                                // recorremos todos los valores devueltos por este atributo
                                for (i = 0; values[i] != NULL; i++) {
                                   entrada.usuario=values[i];
                                   //a<<QString::fromStdString(values[i]).toUpper();
                                   // printf("a: %s\n", values[i]);
                                   //en usuario_basedn guardamos usuario$base_dn para consultar el dn (UO y dominio) del usuario en memoria
                                   //usuario_basedn<<QString::fromStdString(values[i]).toUpper() + "$" + qstr;

                                }
                                ldap_value_free(values);
                            }
                        }
                        if (QString::fromStdString(atributo)=="cn"){
                            if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                                // recorremos todos los valores devueltos por este atributo
                                for (i = 0; values[i] != NULL; i++) {
                                    entrada.nombre=values[i];


                                    //b<<QString::fromStdString(values[i]).toUpper();
                                    //usuario_basedn2<<QString::fromStdString(values[i]).toUpper() + "$" + qstr;
                                    // print each value of a attribute here
                                    //printf("%s: %s\n", atributo, values[i] );
                                }
                                ldap_value_free(values);
                            }
                        }
                        if (QString::fromStdString(atributo)=="lastLogonTimestamp"){
                            if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                                // recorremos todos los valores devueltos por este atributo
                                for (i = 0; values[i] != NULL; i++) {
                                    entrada.ultimo_login = values[i];


                                    //b<<QString::fromStdString(values[i]).toUpper();
                                    //usuario_basedn2<<QString::fromStdString(values[i]).toUpper() + "$" + qstr;
                                    // print each value of a attribute here
                                    //printf("%s: %s\n", atributo, values[i] );
                                }
                                ldap_value_free(values);
                            }
                        }
                        if (QString::fromStdString(atributo)=="logonCount"){
                            if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                                // recorremos todos los valores devueltos por este atributo
                                for (i = 0; values[i] != NULL; i++) {
                                    entrada.ultimo_login =values[i];


                                    //b<<QString::fromStdString(values[i]).toUpper();
                                    //usuario_basedn2<<QString::fromStdString(values[i]).toUpper() + "$" + qstr;
                                    // print each value of a attribute here
                                    //printf("%s: %s\n", atributo, values[i] );
                                }
                                ldap_value_free(values);
                            }
                        }
                        if (QString::fromStdString(atributo)=="lockoutTime"){
                            if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                                // recorremos todos los valores devueltos por este atributo
                                for (i = 0; values[i] != NULL; i++) {
                                    entrada.estado =values[i];


                                    //b<<QString::fromStdString(values[i]).toUpper();
                                    //usuario_basedn2<<QString::fromStdString(values[i]).toUpper() + "$" + qstr;
                                    // print each value of a attribute here
                                    //printf("%s: %s\n", atributo, values[i] );
                                }
                                ldap_value_free(values);
                            }
                        }
                        if (QString::fromStdString(atributo)=="badPasswordTime"){
                            if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                                // recorremos todos los valores devueltos por este atributo
                                for (i = 0; values[i] != NULL; i++) {
                                    entrada.caduca_clave = values[i];


                                    //b<<QString::fromStdString(values[i]).toUpper();
                                    //usuario_basedn2<<QString::fromStdString(values[i]).toUpper() + "$" + qstr;
                                    // print each value of a attribute here
                                    //printf("%s: %s\n", atributo, values[i] );
                                }
                                ldap_value_free(values);
                            }
                        }
                        if (QString::fromStdString(atributo)=="accountExpires"){
                            if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                                // recorremos todos los valores devueltos por este atributo
                                for (i = 0; values[i] != NULL; i++) {
                                    entrada.caduca_cuenta = values[i];


                                    //b<<QString::fromStdString(values[i]).toUpper();
                                    //usuario_basedn2<<QString::fromStdString(values[i]).toUpper() + "$" + qstr;
                                    // print each value of a attribute here
                                    //printf("%s: %s\n", atributo, values[i] );
                                }
                                ldap_value_free(values);
                            }
                        }
                        if (QString::fromStdString(atributo)=="pwdLastSet"){
                            if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                                // recorremos todos los valores devueltos por este atributo
                                for (i = 0; values[i] != NULL; i++) {
                                    entrada.cambio_clave = values[i];


                                    //b<<QString::fromStdString(values[i]).toUpper();
                                    //usuario_basedn2<<QString::fromStdString(values[i]).toUpper() + "$" + qstr;
                                    // print each value of a attribute here
                                    //printf("%s: %s\n", atributo, values[i] );
                                }
                                ldap_value_free(values);
                            }
                        }
                        if (QString::fromStdString(atributo)=="whenCreated"){
                            if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                                // recorremos todos los valores devueltos por este atributo
                                for (i = 0; values[i] != NULL; i++) {
                                    entrada.creada = values[i];


                                    //b<<QString::fromStdString(values[i]).toUpper();
                                    //usuario_basedn2<<QString::fromStdString(values[i]).toUpper() + "$" + qstr;
                                    // print each value of a attribute here
                                    //printf("%s: %s\n", atributo, values[i] );
                                }
                                ldap_value_free(values);
                            }
                        }
                        if (QString::fromStdString(atributo)=="msExchWhenMailboxCreated"){
                            if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                                // recorremos todos los valores devueltos por este atributo
                                for (i = 0; values[i] != NULL; i++) {
                                    entrada.fecha_correo = values[i];


                                    //b<<QString::fromStdString(values[i]).toUpper();
                                    //usuario_basedn2<<QString::fromStdString(values[i]).toUpper() + "$" + qstr;
                                    // print each value of a attribute here
                                    //printf("%s: %s\n", atributo, values[i] );
                                }
                                ldap_value_free(values);
                            }
                        }





                    }
                    ldap_memfree(dn);
                    qDebug()<<entrada.usuario;
                    qDebug()<<entrada.nombre;
                    qDebug()<<entrada.creada;
                    qDebug()<<entrada.caduca_clave;
                    qDebug()<<entrada.correo;
                    qDebug()<<entrada.estado;
                    qDebug()<<entrada.intentos;

                    //query.exec("insert into ldap values(\""+id_tmp.+"\", 'Danny', 'Young')");
               }
               ldap_msgfree(resul_consul);
            }
        }


        //ordenamos la lista
        qSort(usuario_basedn);//ordenamos el QStringList
        qSort(a);//ordenamos el QStringList
        ui->comboBox_usuarios->addItems(a);//insertamos los datos

        qSort(usuario_basedn2);//ordenamos el QStringList
        qSort(b);//ordenamos el QStringList
        ui->comboBox_nombres->addItems(b);//insertamos los datos

    }
    }
}

void form_usuarios::limpia_entrada(){
    entrada.cambio_clave="";
    entrada.clave="";
    entrada.correo="";
    entrada.creada="";
    entrada.descripcion="";
    entrada.estado="";
    entrada.fecha_correo="";
    entrada.id_entry=0;
    entrada.intentos=0;
    entrada.logon=0;
    entrada.modificacion_cuenta="";
    entrada.nombre="";
    entrada.telefono="";
    entrada.ultimo_login="";
    entrada.usuario="";

}


form_usuarios::~form_usuarios()
{
    delete ui;
    desconecta_oldap();
}

bool form_usuarios::carga_OU(){

     Configuracion * configuracion=new Configuracion;
     if (configuracion->usar_ou_externos())
         OU<<"OU=Recursos Centros Externos,DC=grx";
     if (configuracion->usar_ou_perrera())
        OU<<"OU=Recursos Perrera,DC=grx";
     if (configuracion->usar_ou_cie())
        OU<<"OU=Recursos CIE,DC=grx";
     if (configuracion->usar_ou_cpd())
        OU<<"OU=Recursos Drogas San Juan De Dios,DC=grx";
     if (configuracion->usar_ou_ayuntamientos())
        OU<<"OU=Ayuntamientos,DC=grx";
     if (!configuracion->lineEdit_OU_vacio())
        OU << convierte(configuracion->lineEdit_OU_datos());
     delete configuracion;

     if (OU.isEmpty())
         return false;
return true;
}

bool form_usuarios::conecta_oldap() {
    Configuracion *configuracion = new Configuracion;
    int  result;
    QString Qstr;

    int  auth_method    = LDAP_AUTH_SIMPLE;
    int  ldap_version   = LDAP_VERSION3;
    result = ldap_set_option(ldap, LDAP_OPT_PROTOCOL_VERSION, &ldap_version);
    const char *ldap_host     = convierte(configuracion->cual_es_servidor_ldap());
    int   ldap_port     = configuracion->cual_es_puerto_ldap();
    const char *ldap_dn       = convierte(configuracion->cual_es_usuario_ldap());
    const char *ldap_pw       = convierte(configuracion->cual_es_clave_ldap());

    delete configuracion;

    // STEP 1: Establecer la conexión LDAP y fijar preferencias de la sesión ***************************************
     Qstr="ldap://" + QString::fromStdString(ldap_host) + ":" + QString::number(ldap_port);
     if(ldap_initialize(&ldap, Qstr.toLocal8Bit())) {
        ui->label_estado->setText(QString ("No ha sido posible conectarse al servidor %1 en el puerto %2...revise la configuración").arg(ldap_host).arg(ldap_port));
        return false;
     }
     ui->label_estado->setText(QString ("Conectado al servidor %1 en el puerto %2...").arg(ldap_host).arg(ldap_port));

     //FIN ldap_initialize ******************************************************************************************

     result = ldap_set_option(ldap, LDAP_OPT_PROTOCOL_VERSION, &ldap_version);

     if ( result != LDAP_OPT_SUCCESS ) {
         ui->label_estado->setText(QString ("No ha sido posible conectarse al servidor %1 en el puerto %2...revise la configuración").arg(ldap_host).arg(ldap_port));
         return false;
     }
     else {
         ui->label_estado->setText(QString ("Conectado al servidor %1 en el puerto %2 version del cliente LDAPv3 ").arg(ldap_host).arg(ldap_port));
     }

     result = ldap_set_option(ldap, LDAP_OPT_SIZELIMIT, &max_result );

     /* STEP 2: Autentificamos al usuario en el servidor *************************************************************/
     result = ldap_simple_bind_s(ldap, ldap_dn, ldap_pw );

     if ( result != LDAP_SUCCESS ) {
         ui->label_estado->setStyleSheet("background-color:yellow");
         ui->label_estado->setText(QString ("No ha sido posible conectarse al servidor %1 en el puerto %2  Error:%3").arg(ldap_host).arg(ldap_port).arg(ldap_err2string(result)));
         return false;
     }
     else {
         ui->label_estado->setText(QString ("Conectado al servidor %1 en el puerto %2 version del cliente LDAPv3 ").arg(ldap_host).arg(ldap_port));
     }
return true;
}

void form_usuarios::desconecta_oldap(){

    ldap_unbind(ldap);
}

int form_usuarios::num_entradas_oldap(LDAPMessage *resul_consul){

    // Devuelve el numero de objetos encontrados durante la busqueda
    int num_entradas = ldap_count_entries(ldap, resul_consul);
    if ( num_entradas == 0 ) {
        ui->label_estado->setText(QString ("La consulta ldap no ha devuelto ningun resultado"));
    }
    else {
        ui->label_estado->setText(QString ("La consulta ldap ha devuelto %1 resultados").arg(num_entradas));
    }

    return(num_entradas);

}

LDAPMessage * form_usuarios::consulta_oldap(char *filtro, char *attrs[], int attrsonly, const char * base_dn, int scope){

    /*
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
    */
    int  result;
    LDAPMessage *answer;

    result = ldap_search_s(ldap, base_dn, scope, filtro, attrs, attrsonly, &answer);
    if ( result != LDAP_SUCCESS ) {
        ui->label_estado->setText(QString ("No ha sido posible realizar la consulta ldap Error:%1").arg(ldap_err2string(result)));
    }

    return (answer);
}

void form_usuarios::on_comboBox_usuarios_activated(const QString &arg1)
{

    QString Qstr=("(&(objectClass=user)(sAMAccountName=" + ui->comboBox_usuarios->currentText() + "))");

    //obtenemos la base_dn del usuario:
    QString basedn = usuario_basedn[ui->comboBox_usuarios->currentIndex()];

    rellena(Qstr, basedn);
}

void form_usuarios::on_comboBox_nombres_activated(const QString &arg1)
{

    QString Qstr=("(&(objectClass=user)(cn=" + ui->comboBox_nombres->currentText() + "))");

    //obtenemos la base_dn del usuario:
    QString basedn = usuario_basedn2[ui->comboBox_nombres->currentIndex()];

    rellena(Qstr, basedn);

}

void form_usuarios::clear_text(){

    ui->text_cambio_clave->clear();
    ui->text_clave_caduca->clear();
    ui->text_correo->clear();
    ui->text_creada->clear();
    ui->text_cuenta_caduca->clear();
    ui->text_estado->clear();
    ui->text_fecha_correo->clear();
    ui->text_intentos->clear();
    ui->text_logon->clear();
    ui->text_modif_cuenta->clear();
    ui->text_telefono->clear();
    ui->text_ulti_login->clear();
    ui->label_descripcion->clear();


}

// Procedimiento para rellenar los campos del formulario
void form_usuarios::rellena(QString consulta, QString base_DN) {

    QDateTime fecha;
    QString temp, basedn1;
    QString pwdLastSet,userAccountControl;


    // variables para CONSULTA LDAP ------------------------------------------------------------------------------------------------
    LDAPMessage *resul_consul, *entry;
    BerElement *ber;
    // Guardamos lo que devuelve entries_found que es el numero entradas encontradas para una consulta LDAP
    int  num_entradas  = 0;
    // dn guarda el DN name string the los objetos devueltos por la consulta
    char *dn            = "";
    // atributo guarda el nombre de los atributos de los objetos devueltos
    char *atributo     = "";
    // values is un array para guardar los valores de cada atributo, de los atributos de los objetos devueltos
    char **values;
    char *attrs[]       = {NULL};
    // variables para CONSULTA LDAP ------------------------------------------------------------------------------------------------

    int  i = 0;

    clear_text();//Limpiamos los lineEdit


    //************************************************************************************************************************

    QString Qstr=consulta;

    //obtenemos la base_dn del usuario:
    QString basedn = base_DN;

    //************************************************************************************************************************

    //eliminamos la parte del usuario y nos quedamos con la cadena (OU=xxx, DC=xxx)
    basedn=basedn.remove(0,basedn.indexOf("$")+1);
    ui->groupBox_descripcion->setTitle(basedn);

    resul_consul=consulta_oldap(convierte(Qstr), attrs, 0, basedn.toLocal8Bit(),LDAP_SCOPE_SUBTREE);//attrs LDAP_SCOPE_CHILDREN

     // recorremos todos los objetos (entry) devueltos por la consulta
     // cada entry tiene atributos
     for ( entry = ldap_first_entry(ldap, resul_consul); entry != NULL; entry = ldap_next_entry(ldap, entry))
     {

       // Imprimimos la cadena DN del objeto
       dn = ldap_get_dn(ldap, entry);
       DN=QString::fromStdString(dn);//para usarlo luego
       //printf("Found Object: %s\n", dn);

       // recorremos todos los atributos de cada entry
       for ( atributo = ldap_first_attribute(ldap, entry, &ber);atributo != NULL;
             atributo = ldap_next_attribute(ldap, entry, ber))
       {

         //usuario
         if (QString::fromStdString(atributo)=="sAMAccountName"){
             if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                 //ldap_sort_values(ldap,values,LDAP_SORT_AV_CMP_PROC("a","x"));
               // recorremos todos los valores devueltos por este atributo
               for (i = 0; values[i] != NULL; i++) {
                    ui->comboBox_usuarios->setCurrentText(QString::fromStdString(values[i]).toUpper());
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
                    ui->comboBox_nombres->setCurrentText(QString::fromStdString(values[i]).toUpper());
               }
               ldap_value_free(values);
             }
         }

         // Descripcion
         if (QString::fromStdString(atributo)=="description"){
             if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
               for (i = 0; values[i] != NULL; i++) {
                    ui->label_descripcion->setText(QString::fromStdString(values[i]));
               }
               ldap_value_free(values);
             }
             else {
                ui->label_descripcion->clear();
             }
         }

         // Telefono
         if (QString::fromStdString(atributo)=="telephoneNumber"){
             if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
               for (i = 0; values[i] != NULL; i++) {
                    ui->text_telefono->setText(QString::fromStdString(values[i]));
               }
               ldap_value_free(values);
             }
         }

         // Correo
         if (QString::fromStdString(atributo)=="mail"){
             if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
               for (i = 0; values[i] != NULL; i++) {
                    ui->text_correo->setText(QString::fromStdString(values[i]));
               }
               ldap_value_free(values);
             }
         }

         // Ultimo logon
         if (QString::fromStdString(atributo)=="lastLogon"){
             if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
               for (i = 0; values[i] != NULL; i++) {
                   if (QString::fromStdString(values[i]).trimmed()!="0"){
       //--------------  fecha.setSecsSinceEpoch((QString::fromStdString(values[i]).toLongLong()/10000000)-11644473600);
                       ui->text_ulti_login->setText(fecha.toString("dd-MM-yyyy hh:mm"));
                   }
                   else
                       ui->text_ulti_login->setText("NUNCA");
               }
               ldap_value_free(values);
             }
         }

         // la cuenta caduca
         if (QString::fromStdString(atributo)=="accountExpires"){
             if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
               for (i = 0; values[i] != NULL; i++) {
                   if (QString::fromStdString(values[i]).toLongLong()==9223372036854775807 || QString::fromStdString(values[i]).trimmed()=="0")
                        ui->text_cuenta_caduca->setText("No Caduca");
                   else {
    //------------------ fecha.setSecsSinceEpoch((QString::fromStdString(values[i]).toLongLong()/10000000)-11644473600);
                       ui->text_cuenta_caduca->setText(fecha.toString("dd-MM-yyyy hh:mm"));
                   }
               }
               ldap_value_free(values);
             }
         }

         // Estado
         if (QString::fromStdString(atributo)=="lockoutTime"){
             if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
               for (i = 0; values[i] != NULL; i++) {
                   if (QString::fromStdString(values[i])=="0"){
                       ui->text_estado->setText("Activa");
                       ui->text_estado->setStyleSheet("color: rgb(11, 97, 29)");
                   }
                   else{
                       ui->text_estado->setText("Bloqueada");
                       ui->text_estado->setStyleSheet("color: rgb(164, 0, 0)");
                   }
               }
               ldap_value_free(values);
             }
         }

         // Numero de logon
         if (QString::fromStdString(atributo)=="logonCount"){
             if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
               for (i = 0; values[i] != NULL; i++) {
                    ui->text_logon->setText(QString::fromStdString(values[i]));
               }
               ldap_value_free(values);
             }
         }

         // cuando se creo la cuenta
         if (QString::fromStdString(atributo)=="whenCreated"){
             if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
               for (i = 0; values[i] != NULL; i++) {
                   ui->text_creada->setText(QString::fromStdString(values[i]).mid(6,2)+"-"+QString::fromStdString(values[i]).mid(4,2)+"-"+QString::fromStdString(values[i]).mid(0,4));
               }
               ldap_value_free(values);
             }
         }

         //cuando se modifico por ultima vez la cuenta
         if (QString::fromStdString(atributo)=="whenChanged"){
             if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
               for (i = 0; values[i] != NULL; i++) {
                   ui->text_modif_cuenta->setText(QString::fromStdString(values[i]).mid(6,2)+"-"+QString::fromStdString(values[i]).mid(4,2)+"-"+QString::fromStdString(values[i]).mid(0,4));
               }
               ldap_value_free(values);
             }
         }

         // cuando se creo la cuenta de correo
         if (QString::fromStdString(atributo)=="msExchWhenMailboxCreated"){
             if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
               for (i = 0; values[i] != NULL; i++) {
                   ui->text_fecha_correo->setText(QString::fromStdString(values[i]).mid(6,2)+"-"+QString::fromStdString(values[i]).mid(4,2)+"-"+QString::fromStdString(values[i]).mid(0,4));
               }
               ldap_value_free(values);
             }
         }

         // Ultimo cambio de contraseña
         if (QString::fromStdString(atributo)=="pwdLastSet"){
             if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
               for (i = 0; values[i] != NULL; i++) {
                   pwdLastSet=QString::fromStdString(values[i]);//para usarlo luego
   //----------  fecha.setSecsSinceEpoch((QString::fromStdString(values[i]).toLongLong()/10000000)-11644473600);
                   ui->text_cambio_clave->setText(fecha.toString("dd-MM-yyyy  hh:mm"));
               }
               ldap_value_free(values);
             }
         }

         //Intentos fallidos de contraseña
         if (QString::fromStdString(atributo)=="badPwdCount"){
             if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
               for (i = 0; values[i] != NULL; i++) {
                   ui->text_intentos->setText(QString::fromStdString(values[i]));
               }
               ldap_value_free(values);
             }
         }


         if (QString::fromStdString(atributo)=="userAccountControl"){
             if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
               for (i = 0; values[i] != NULL; i++) {

                   if (QString::fromStdString(values[i])=="66048" || QString::fromStdString(values[i])=="1114624") {
                       ui->text_clave_caduca->setText("No caduca");
                   }
                   else{
                       userAccountControl="0";//para usarlo luego
                   }

               }
               ldap_value_free(values);
             }
         }

       }

       ldap_memfree(dn);
     }

     // si la clave caduca rellenamos el campo CLAVE CADUCA
     // no se puede poner dentro del for que recorre los atributos porque pwdLastSet
     // se resuelve despues de userAccountControl y no funciona
     if (userAccountControl=="0") {

         // variables para CONSULTA LDAP ------------------------------------------------------------------------------------------------
         LDAPMessage *resul_consul1, *entry1;
         BerElement *ber1;
         // Guardamos lo que devuelve entries_found que es el numero entradas encontradas para una consulta LDAP
         int  num_entradas1  = 0;
         // dn guarda el DN name string the los objetos devueltos por la consulta
         char *dn1            = "";
         // atributo guarda el nombre de los atributos de los objetos devueltos
         char *atributo1     = "";
         // values is un array para guardar los valores de cada atributo, de los atributos de los objetos devueltos
         char **values1;
         char *attrs1[]= {"maxPwdAge", NULL};
         // variables para CONSULTA LDAP ------------------------------------------------------------------------------------------------

         QString Qstr="(&(objectClass=domain))";
         basedn1="DC=grx";
         resul_consul1=consulta_oldap(convierte(Qstr), attrs1, 0, basedn1.toLocal8Bit(),LDAP_SCOPE_BASE);
         entry1 = ldap_first_entry(ldap, resul_consul1);
         dn1 = ldap_get_dn(ldap, entry1);
         atributo1 = ldap_first_attribute(ldap, entry1, &ber1);
         if ((values1 = ldap_get_values(ldap, entry1, atributo1)) != NULL) {
             temp=QString::fromStdString(values1[0]);
             temp=QString::number(pwdLastSet.toLongLong()-temp.toLongLong());
  //------------           fecha.setSecsSinceEpoch((temp.toLongLong()/10000000)-11644473600);
             ui->text_clave_caduca->setText(fecha.toString("dd-MM-yyyy  hh:mm"));
         }

         ldap_value_free(values1);
         ldap_memfree(dn1);
         ldap_msgfree(resul_consul1);
    }

    ldap_msgfree(resul_consul);

}

void form_usuarios::on_boton_desbloquear_clicked()
{
    LDAPMod attribute1;

    char *modificaciones_values[] = { "0", NULL };
    attribute1.mod_type = "lockoutTime";
    attribute1.mod_op = LDAP_MOD_REPLACE;
    attribute1.mod_values = modificaciones_values;

    LDAPMod *mods[2];
    mods[0]=&attribute1;
    mods[1]=NULL;

    int result=  ldap_modify_ext_s( ldap, convierte(DN), mods,NULL,NULL);

    if ( result == LDAP_SUCCESS )
        printf("\n\tModified %s's attributes.\n", convierte(DN));
    else {
        printf("\n\tFailed to modify %s's attributes. ldap_modify_ext_s: %s.\n",
                                                  convierte(DN), ldap_err2string(result));
        //return ();
    }
}
