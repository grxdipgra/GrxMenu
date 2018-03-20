#include "form_usuarios.h"
#include "ui_botonera.h"
#include "botonera/botonera.h"
#include "configuracion/configuracion.h"
#include <QFileInfo>
//#include <QThread>
#include <QProgressDialog>

//falta comprobar cuando los usuarios se desbloquean por tiempo

 LDAP *ldap;
 QSqlDatabase bd;
 int id_usuario;

//convierte QString a char *
char* form_usuarios::convierte(QString dato){
    char* cstr;
    std::string fname = dato.toStdString();
    cstr = new char [fname.size()+1];
    strcpy( cstr, fname.c_str() );
    return cstr;
}

bool fileExite(QString path) {
    QFileInfo check_file(path);
    return (check_file.exists() && check_file.isFile());
}

form_usuarios::form_usuarios(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::form_usuarios)
{
    ui->setupUi(this);

//------------------SQLITE

bool existe;

    Configuracion *configuracion = new Configuracion;
    //QString home = configuracion->cual_es_home();
    QString rutaDB = configuracion->cual_es_home() + "/.grx/grx.sqlite";

    //comprobamos si existe la BD
    if (!fileExite(rutaDB)){
        QMessageBox::critical(this, "Configurar", "Es la primera vez que ejecuta GrxMenu\no se ha borrado la base de datos\nSe va a crear la base de datos de usuarios, esto llevara unos segundos...espere",QMessageBox::Ok);
        //Creamos la base de datos
        existe=false;
    }
    else{
        existe=true;
    }

    //qDebug()<<"antes de addDatabase";

    //bd1 = QSqlDatabase::addDatabase("QSQLITE","ldapdb");
    //No es necesario hacer addDatabase puesto que ya se ha creado la conexión en Botonera::cargaVariables()
    //solo es necesario definir
    bd = QSqlDatabase::database();
    //qDebug()<<"despues de addDatabase";
    bd.setDatabaseName(rutaDB);

    if (!bd.open()) {
        QMessageBox::critical(nullptr, QObject::tr("No he podido crear la DB"),
        QObject::tr("No ha sido posible crear la base de datos temporal de SQLITE.\n"
                    "Comprueba que tengas instaladas las librerias de SQLITE"), QMessageBox::Cancel);
        return;
    }
    //qDebug()<<"OPEN";

//    QString pwdLastSet;
//    QString temp, basedn1;
//    QDateTime fecha;
//    int userAccountControl;

    //QSqlQuery query;
    QSqlQuery* consulta = new QSqlQuery(bd);


// COMENTADO           /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
if (!existe){
    //qDebug()<<"crea";
    int pb_cont = 0;

    //Dialogo de espera...
    QApplication::processEvents();
    QProgressDialog pb("Creando la base de datos de usuarios . . .", "", 0, 100, this);
    pb.setWindowModality(Qt::WindowModal);
    //d.setLabelText("HOLAAAAA");
    //d.setMaximum(50);
    pb.setCancelButton(0);
    pb.setValue(pb_cont);
    pb.show();

    /*QDialog *newdialog = new QDialog(this);
    QThread *mipolla = new QThread;
    newdialog->moveToThread(mipolla);
    newdialog->setModal(false);
    newdialog->exec();

    qDebug()<<"paso";*/
        consulta->exec("drop table if exists ldap");
        consulta->exec("create table ldap (id int primary key,"
               "usuario varchar(100),"
               "nombre varchar (100),"
               "cuenta_caduca varchar(50),"
               "cambio_clave varchar(50),"
               "clave_caducada varchar(50),"
               "correo varchar(100),"
               "creada varchar(50),"
               "estado varchar(25),"
               "fecha_correo varchar(50),"
               "intentos int,"
               "logon int,"
               "modificacion_cuenta varchar(50),"
               "telefono varchar(50),"
               "ultimo_login varchar(50),"
               "descripcion varchar(250),"
               "dn varchar(250))");
        consulta->exec("create table grupos (id_grupo int,"
               "id_usuario int,"
               "grupo varchar (100),"
               "usuario varchar(100))");

//-------------------
   // rellenamos el combobox con todos los usuarios del dominio de las OU que se usan

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


    if (conecta_oldap()){

        // lo usamos para ir guardando los resultados de las consultas, ordenarlo y luego se lo pasamos al combo
//        QStringList a;
//        QStringList b;
        int id_tmp=0;

        if (carga_OU()){ //Si hay unidades organizativas realizamos las búsquedas
        int i=0;
        pb.setMaximum(OU.count()+1);

        //recorremos todo OU
        foreach (const QString &qstr, OU) {

            pb_cont=pb_cont+1;
            pb.setLabelText("Cargando base de datos de usuarios . . .\n"+ qstr);
            pb.setValue(pb_cont);
            //QCoreApplication::processEvents();
            QApplication::processEvents();
            //qDebug()<<qstr;

            //Botonera::ui->progressBar->setFormat("Buscando lo que sea..." + QString::number(10) + "%");

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

                   rellena_entrada(entry);

                    //insertamos los usuarios en la base de datos
                    consulta->exec( "insert into ldap values(" + QString::number(id_tmp) + ", "
                               "'"+entrada.usuario+"', "
                               "'"+entrada.nombre+ "', "
                               "'"+entrada.caduca_cuenta+"', "
                               "'"+entrada.cambio_clave+"', "
                               "'"+entrada.caduca_clave+"', "
                               "'"+entrada.correo+"', "
                               "'"+entrada.creada+"', "
                               "'"+entrada.estado+"', "
                               "'"+entrada.fecha_correo+"', "
                               ""+ QString::number(entrada.intentos) +", "
                               ""+ QString::number(entrada.logon) +", "
                               "'"+entrada.modificacion_cuenta+"', "
                               "'"+entrada.telefono+"', "
                               "'"+entrada.ultimo_login+"', "
                               "'"+entrada.descripcion+"' ,"
                               "'"+entrada.dn+"')");

                    for (int j = 0; j < vec_grupos.size(); ++j) {
                        //vec_grupos[j].usuario=entrada.usuario;

                        //insertamos los grupos del usuario en la base de datos
                        consulta->exec( "insert into grupos values(" + QString::number(j) + ", "
                                   "" + QString::number(id_tmp) + ", "
                                   "'"+vec_grupos[j].nombre+ "', "
                                   "'"+vec_grupos[j].usuario+"')");
                    }

                    id_tmp=id_tmp+1;
                    //qDebug()<<entrada.nombre;

               }//no

               ldap_msgfree(resul_consul);//no

            }//no

        }

    }

    //ldap_unbind_s(ldap);

    }


    //COMENTADO  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
    QString sql;
    //QSqlQuery* consulta = new QSqlQuery(bd);

    QSqlQueryModel *model = new QSqlQueryModel();
    sql = "select usuario from ldap order by usuario";
    consulta->prepare(sql);
    if(!consulta->exec()){
        qDebug() <<"Error en la consulta: "<< consulta->lastError();
    }else{
        qDebug() <<"Consulta realizada con exito: "<<consulta->lastQuery();
        model->setQuery(*consulta);
        ui->comboBox_usuarios->setModel(model);
        on_comboBox_usuarios_activated(ui->comboBox_usuarios->itemText(0));
    }

    QSqlQueryModel *model1 = new QSqlQueryModel();
    sql = "select nombre from ldap order by usuario";
    consulta->prepare(sql);
    if(!consulta->exec()){
        qDebug() <<"Error en la consulta: "<< consulta->lastError();
    }else{
        qDebug() <<"Consulta realizada con exito: "<<consulta->lastQuery();
        model1->setQuery(*consulta);
        ui->comboBox_nombres->setModel(model1);
        on_comboBox_nombres_activated(ui->comboBox_nombres->itemText(0));
    }

}

void form_usuarios::limpia_entrada(){
    entrada.id_entry=0;
    entrada.usuario="";
    entrada.nombre="";
    entrada.cambio_clave="";
    entrada.clave="";
    entrada.correo="";
    entrada.creada="";
    entrada.estado="";
    entrada.fecha_correo="";
    entrada.intentos=0;
    entrada.logon=0;
    entrada.modificacion_cuenta="";
    entrada.telefono="";
    entrada.ultimo_login="";
    entrada.descripcion="";
    entrada.caduca_clave="";
    entrada.caduca_cuenta="";
    entrada.dn="";

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

//introduce los datos en la estructura entrada desde ldap
void form_usuarios::rellena_entrada(LDAPMessage *entry){

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

    QString pwdLastSet;
    QString temp, basedn1;
    QDateTime fecha;
    int userAccountControl,i;


            // Capturamos la cadena DN del objeto
            dn = ldap_get_dn(ldap, entry);
            //limpiamos el struct entrada y vector grupos
            limpia_entrada();
            vec_grupos.clear();

            // recorremos todos los atributos de cada entry
            for ( atributo = ldap_first_attribute(ldap, entry, &ber);atributo != NULL;
                 atributo = ldap_next_attribute(ldap, entry, ber))
            {

                //entrada.dn=qstr;
                entrada.dn=QString::fromStdString(dn);



                //usuario
                if (QString::fromStdString(atributo)=="sAMAccountName"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                        //ldap_sort_values(ldap,values,LDAP_SORT_AV_CMP_PROC("a","x"));
                      // recorremos todos los valores devueltos por este atributo
                      for (i = 0; values[i] != NULL; i++) {
                           entrada.usuario = QString::fromStdString(values[i]).toUpper();
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
                           entrada.nombre = QString::fromStdString(values[i]).toUpper();
                      }
                      ldap_value_free(values);
                    }
                }

                // Descripcion
                if (QString::fromStdString(atributo)=="description"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                           entrada.descripcion = QString::fromStdString(values[i]);
                      }
                      ldap_value_free(values);
                    }
                }

                // Telefono
                if (QString::fromStdString(atributo)=="telephoneNumber"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                           entrada.telefono = QString::fromStdString(values[i]);
                      }
                      ldap_value_free(values);
                    }
                }

                // Correo
                if (QString::fromStdString(atributo)=="mail"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                           entrada.correo = QString::fromStdString(values[i]);
                      }
                      ldap_value_free(values);
                    }
                }

                // Ultimo logon
                if (QString::fromStdString(atributo)=="lastLogon"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                          if (QString::fromStdString(values[i]).trimmed()!="0"){
                              fecha.setMSecsSinceEpoch((QString::fromStdString(values[i]).toLongLong()/10000)-11644473600000);
                              entrada.ultimo_login = fecha.toString("dd-MM-yyyy hh:mm");
                          }
                          else
                              entrada.ultimo_login = "NUNCA";
                      }
                      ldap_value_free(values);
                    }
                }

                // la cuenta caduca
                if (QString::fromStdString(atributo)=="accountExpires"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                          if (QString::fromStdString(values[i]).toLongLong()==9223372036854775807 || QString::fromStdString(values[i]).trimmed()=="0")
                              entrada.caduca_cuenta = "No Caduca";
                              //entrada.caduca_cuenta = "";
                          else {
                              fecha.setMSecsSinceEpoch((QString::fromStdString(values[i]).toLongLong()/10000)-11644473600000);
                              entrada.caduca_cuenta = fecha.toString("dd-MM-yyyy hh:mm");
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
                              entrada.estado = "Activa";
                              //ui->text_estado->setStyleSheet("color: rgb(11, 97, 29)");
                          }
                          else{
                              entrada.estado = "Bloqueada";
                              //ui->text_estado->setStyleSheet("color: rgb(164, 0, 0)");
                          }
                      }
                      ldap_value_free(values);
                    }
                }

                // Numero de logon
                if (QString::fromStdString(atributo)=="logonCount"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                           entrada.logon = QString::fromStdString(values[i]).toInt();
                      }
                      ldap_value_free(values);
                    }
                }

                // cuando se creo la cuenta
                if (QString::fromStdString(atributo)=="whenCreated"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                          entrada.creada = QString::fromStdString(values[i]).mid(6,2)+"-"+QString::fromStdString(values[i]).mid(4,2)+"-"+QString::fromStdString(values[i]).mid(0,4);
                      }
                      ldap_value_free(values);
                    }
                }

                //cuando se modifico por ultima vez la cuenta
                if (QString::fromStdString(atributo)=="whenChanged"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                          entrada.modificacion_cuenta = QString::fromStdString(values[i]).mid(6,2)+"-"+QString::fromStdString(values[i]).mid(4,2)+"-"+QString::fromStdString(values[i]).mid(0,4);
                      }
                      ldap_value_free(values);
                    }
                }

                // cuando se creo la cuenta de correo
                if (QString::fromStdString(atributo)=="msExchWhenMailboxCreated"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                          entrada.fecha_correo = QString::fromStdString(values[i]).mid(6,2)+"-"+QString::fromStdString(values[i]).mid(4,2)+"-"+QString::fromStdString(values[i]).mid(0,4);
                      }
                      ldap_value_free(values);
                    }
                }

                // Ultimo cambio de contraseña
                if (QString::fromStdString(atributo)=="pwdLastSet"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                          pwdLastSet=QString::fromStdString(values[i]);//para usarlo luego
                          fecha.setMSecsSinceEpoch((QString::fromStdString(values[i]).toLongLong()/10000)-11644473600000);
                          entrada.cambio_clave = fecha.toString("dd-MM-yyyy  hh:mm");
                      }
                      ldap_value_free(values);
                    }
                }

                //Intentos fallidos de contraseña
                if (QString::fromStdString(atributo)=="badPwdCount"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                          entrada.intentos = QString::fromStdString(values[i]).toInt();
                      }
                      ldap_value_free(values);
                    }
                }


                if (QString::fromStdString(atributo)=="userAccountControl"){
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {
                          if (QString::fromStdString(values[i])=="66048" || QString::fromStdString(values[i])=="1114624") {
                              entrada.caduca_clave = "No caduca";
                              userAccountControl=1;//para usarlo luego
                          }
                          else{
                              userAccountControl=0;//para usarlo luego
                          }

                      }
                      ldap_value_free(values);
                    }
                }

                //grupos
                if (QString::fromStdString(atributo)=="memberOf"){
                    vec_grupos.clear();
                    if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                      for (i = 0; values[i] != NULL; i++) {

                          //qDebug()<< usuario;
                          //qDebug()<< QString::fromStdString(values[i]).toUpper();
                          //grupos.usuario=entrada.usuario;
                          grupos.nombre=QString::fromStdString(values[i]).toUpper();
                          vec_grupos.push_back(grupos);
                      }
                      //qDebug()<<vec_grupos.count();
                      ldap_value_free(values);
                    }
                }

            }

            ldap_memfree(dn);

            //rellenamos aquí el Samaccountname del usuario en cada grupo porque se resuelve despues del Memberof
            for (int j = 0; j < vec_grupos.size(); ++j) {
                vec_grupos[j].usuario=entrada.usuario;
                //qDebug()<<vec_grupos[j].usuario;
            }


            //INICIO_______ si la clave caduca rellenamos el campo CLAVE CADUCA***********************************************************************************
            // no se puede poner dentro del for que recorre los atributos porque pwdLastSet
            // se resuelve despues de userAccountControl y no funciona
            if (userAccountControl==0) {

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
                    fecha.setMSecsSinceEpoch((temp.toLongLong()/10000)-11644473600000);
                    entrada.caduca_clave = fecha.toString("dd-MM-yyyy  hh:mm");
                }

                ldap_value_free(values1);
                ldap_memfree(dn1);
                ldap_msgfree(resul_consul1);
                //FIN____ si la clave caduca rellenamos el campo CLAVE CADUCA*************************************************************************************
            }
       //}

       //ldap_msgfree(resul_consul);
    //}

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

    /*QString Qstr=("(&(objectClass=user)(sAMAccountName=" + ui->comboBox_usuarios->currentText() + "))");

    //obtenemos la base_dn del usuario:
    QString basedn = usuario_basedn[ui->comboBox_usuarios->currentIndex()];

    rellena(Qstr, basedn);*/
    carga_datos_usuario(0, arg1);
    //qDebug() << DN.remove(0,DN.indexOf(",OU=")+1);
;
}

void form_usuarios::on_comboBox_nombres_activated(const QString &arg1)
{

    /*QString Qstr=("(&(objectClass=user)(cn=" + ui->comboBox_nombres->currentText() + "))");

    //obtenemos la base_dn del usuario:
    QString basedn = usuario_basedn2[ui->comboBox_nombres->currentIndex()];

    rellena(Qstr, basedn);*/
    carga_datos_usuario(1, arg1);

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
// tipo=0->filtro por usuario, tipo=1->filtro por nombre
void form_usuarios::carga_datos_usuario(int tipo, QString filtro){

    //QSqlQuery consultar;

    QSqlQuery* consultar = new QSqlQuery(bd);

    //qDebug()<<"entra";
    //QString resultado;
    if (tipo==0)
        consultar->prepare(QString("select * from ldap where usuario = :usuario"));
    else
        consultar->prepare(QString("select * from ldap where nombre = :usuario"));

    //consultar.bindValue(":usuario", ui->comboBox_usuarios->currentText());
    consultar->bindValue(":usuario", filtro);
    if (consultar->exec() and consultar->first()){
            id_usuario=consultar->value(0).toInt();
            ui->comboBox_usuarios->setCurrentText(consultar->value(1).toString());//usuario
            ui->comboBox_nombres->setCurrentText(consultar->value(2).toString());//nombre
            ui->text_cuenta_caduca->setText(consultar->value(3).toString());
            ui->text_cambio_clave->setText(consultar->value(4).toString());
            ui->text_clave_caduca->setText(consultar->value(5).toString());
            ui->text_correo->setText(consultar->value(6).toString());
            ui->text_creada->setText(consultar->value(7).toString());
            ui->text_estado->setText(consultar->value(8).toString());
            ui->text_fecha_correo->setText(consultar->value(9).toString());
            ui->text_intentos->setText(consultar->value(10).toString());
            ui->text_logon->setText(consultar->value(11).toString());
            ui->text_modif_cuenta->setText(consultar->value(12).toString());
            ui->text_telefono->setText(consultar->value(13).toString());
            ui->text_ulti_login->setText(consultar->value(14).toString());
            ui->label_descripcion->setText(consultar->value(15).toString());
            DN=consultar->value(16).toString();
    }


    //QSqlQuery* consulta1 = new QSqlQuery(bd);
    QString sql;
    QSqlQueryModel *model = new QSqlQueryModel();
    //instr(X,Y)

    //consultamos los grupos del usuario y enseñamos solo la parte del nombre del grupo de la cadena dn
    sql = "select substr(grupo,4,instr(grupo,',')-4) from grupos where id_usuario=" + QString::number(id_usuario);
    consultar->prepare(sql);
    if(!consultar->exec()){
        qDebug() <<"Error en la consulta: "<< consultar->lastError();
    }else{
        //qDebug() <<"Consulta realizada con exito: "<<consultar->lastQuery();
        model->setQuery(*consultar);
        ui->listView_grupos->setModel(model);
        //on_comboBox_usuarios_activated(ui->comboBox_usuarios->itemText(0));
    }



    if (ui->text_estado->text()=="Activa"){
        ui->text_estado->setStyleSheet("color: rgb(11, 97, 29)");
    }
    else{
        ui->text_estado->setStyleSheet("color: rgb(164, 0, 0)");
    }

    QDateTime caduca = QDateTime::fromString(ui->text_clave_caduca->text(),"dd-MM-yyyy  hh:mm");
    //fecha actual:
    QDateTime ahora = QDateTime::currentDateTime();

    //qDebug()<<ahora;

    if (ui->text_clave_caduca->text()=="No caduca")
        ui->text_clave_caduca->setStyleSheet("color: rgb(5, 31, 137)");
    else{
        if (ahora>caduca){
            ui->text_clave_caduca->setStyleSheet("color: rgb(164, 0, 0)");
        }
        else{
            ui->text_clave_caduca->setStyleSheet("color: rgb(11, 97, 29)");
        }
    }
}

// Procedimiento para actualizar la base de datos y los campos del formulario
void form_usuarios::actualizar_usuarios(){

//    QString pwdLastSet;
//    QString temp, basedn1;
//    QDateTime fecha;
//    int userAccountControl;

    //QSqlQuery query;
    QSqlQuery* consulta = new QSqlQuery(bd);

    consulta->exec("delete from ldap");
    consulta->exec("delete from grupos");


   // rellenamos el combobox con todos los usuarios del dominio de las OU que se usan

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


        int pb_cont = 0;

        //Dialogo de espera...
        QCoreApplication::processEvents();
        QProgressDialog pb("Creando la base de datos de usuarios . . .", "", 0, 10, this);
        pb.setWindowModality(Qt::WindowModal);
        //d.setLabelText("HOLAAAAA");
        //d.setMaximum(50);
        pb.setCancelButton(0);
        pb.setValue(pb_cont);
        pb.show();
        QApplication::processEvents();


    if (conecta_oldap()){

        // lo usamos para ir guardando los resultados de las consultas, ordenarlo y luego se lo pasamos al combo
        int id_tmp=0;

        if (carga_OU()){ //Si hay unidades organizativas realizamos las búsquedas
        int i=0;

        pb.setMaximum(OU.count());

        //recorremos todo OU
        foreach (const QString &qstr, OU) {

            pb_cont=pb_cont+1;
            pb.setLabelText("Cargando base de datos de usuarios . . .\n"+ qstr);
            pb.setValue(pb_cont);
            //QCoreApplication::processEvents();
            QApplication::processEvents();

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

                   rellena_entrada(entry);


                    //insertamos los usuarios en la base de datos
                    consulta->exec( "insert into ldap values(" + QString::number(id_tmp) + ", "
                               "'"+entrada.usuario+"', "
                               "'"+entrada.nombre+ "', "
                               "'"+entrada.caduca_cuenta+"', "
                               "'"+entrada.cambio_clave+"', "
                               "'"+entrada.caduca_clave+"', "
                               "'"+entrada.correo+"', "
                               "'"+entrada.creada+"', "
                               "'"+entrada.estado+"', "
                               "'"+entrada.fecha_correo+"', "
                               ""+ QString::number(entrada.intentos) +", "
                               ""+ QString::number(entrada.logon) +", "
                               "'"+entrada.modificacion_cuenta+"', "
                               "'"+entrada.telefono+"', "
                               "'"+entrada.ultimo_login+"', "
                               "'"+entrada.descripcion+"' ,"
                               "'"+entrada.dn+"')");

                    for (int j = 0; j < vec_grupos.size(); ++j) {
                        //vec_grupos[j].usuario=entrada.usuario;

                        //insertamos los grupos del usuario en la base de datos
                        consulta->exec( "insert into grupos values(" + QString::number(j) + ", "
                                   "" + QString::number(id_tmp) + ", "
                                   "'"+vec_grupos[j].nombre+ "', "
                                   "'"+vec_grupos[j].usuario+"')");
                    }

                    id_tmp=id_tmp+1;

                    //qDebug()<<entrada.nombre;
               }
               ldap_msgfree(resul_consul);
            }
        }

    }

    }

    carga_datos_usuario(0, ui->comboBox_usuarios->currentText());

}



// Procedimiento para actualizar los campos del formulario
void form_usuarios::actualiza_usuario(){

//    QString pwdLastSet;
//    QString temp, basedn1;
//    QDateTime fecha;
//    int userAccountControl;

    //QSqlQuery query;
    QSqlQuery* consulta = new QSqlQuery(bd);

   // rellenamos el combobox con todos los usuarios del dominio de las OU que se usan

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


    if (conecta_oldap()){

        //int id_tmp=0;

        //if (carga_OU()){ //Si hay unidades organizativas realizamos las búsquedas
        int i=0;
        //recorremos todo OU
        //foreach (const QString &qstr, OU) {
        QString qstr_dn=DN.remove(0,DN.indexOf(",OU=")+1);
        QString qstr_consul="(&(objectClass=user)(sAMAccountName=" + ui->comboBox_usuarios->currentText() + "))";

            resul_consul=consulta_oldap(convierte(qstr_consul), attrs, 0, qstr_dn.toLocal8Bit(),LDAP_SCOPE_SUBTREE);

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

                   rellena_entrada(entry);

                    //actualizamos el usuario en la base de datos
                    consulta->exec( "update ldap set "
                               "nombre='"+entrada.nombre+ "', "
                               "cuenta_caduca='"+entrada.caduca_cuenta+"', "
                               "cambio_clave='"+entrada.cambio_clave+"', "
                               "clave_caducada='"+entrada.caduca_clave+"', "
                               "correo='"+entrada.correo+"', "
                               "creada='"+entrada.creada+"', "
                               "estado='"+entrada.estado+"', "
                               "fecha_correo='"+entrada.fecha_correo+"', "
                               "intentos="+ QString::number(entrada.intentos) +", "
                               "logon="+ QString::number(entrada.logon) +", "
                               "modificacion_cuenta='"+entrada.modificacion_cuenta+"', "
                               "telefono='"+entrada.telefono+"', "
                               "ultimo_login='"+entrada.ultimo_login+"', "
                               "descripcion='"+entrada.descripcion+"' ,"
                               "dn='"+entrada.dn+"' where id = " + QString::number(id_usuario) + "" );
                               //"dn='"+entrada.dn+"' where usuario like '"+ui->comboBox_usuarios->currentText()+"'");

                    //eliminar los grupos del usuario
                    consulta->exec("delete from grupos where id_usuario=" + QString::number(id_usuario));

                    //actualizar los grupos del usuario
                    for (int j = 0; j < vec_grupos.size(); ++j) {
                        //vec_grupos[j].usuario=entrada.usuario;

                        //insertamos los grupos del usuario en la base de datos
                        consulta->exec( "insert into grupos values(" + QString::number(j) + ", "
                                   "" + QString::number(id_usuario) + ", "
                                   "'"+vec_grupos[j].nombre+ "', "
                                   "'"+vec_grupos[j].usuario+"')");
                    }

                    //id_tmp=id_tmp+1;

                    //qDebug()<<entrada.estado;
               }
               ldap_msgfree(resul_consul);
            }

    }

    carga_datos_usuario(0, ui->comboBox_usuarios->currentText());

}


void form_usuarios::on_boton_desbloquear_clicked()
{
    LDAPMod attribute1;

     if (conecta_oldap()){


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
        //ldap_unbind_s(ldap);

        actualiza_usuario();

   }
}


//renovar caducidad de la contraseña
void form_usuarios::on_boton_renovar_clicked()
{
    //consiste en modificar el campo pwdLastSet=0 y luego pwdLastSet=-1

    LDAPMod attribute1;

    if (conecta_oldap()){

        char *modificaciones_values[] = { "0", NULL };
        attribute1.mod_type = "pwdLastSet";
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

        char *modificaciones_values2[] = { "-1", NULL };
        attribute1.mod_type = "pwdLastSet";
        attribute1.mod_op = LDAP_MOD_REPLACE;
        attribute1.mod_values = modificaciones_values2;

        mods[0]=&attribute1;
        mods[1]=NULL;

        result=  ldap_modify_ext_s( ldap, convierte(DN), mods,NULL,NULL);

        if ( result == LDAP_SUCCESS )
            printf("\n\tModified %s's attributes.\n", convierte(DN));
        else {
            printf("\n\tFailed to modify %s's attributes. ldap_modify_ext_s: %s.\n",
                                                      convierte(DN), ldap_err2string(result));
            //return ();
        }

        actualiza_usuario();

    }
}


void form_usuarios::on_boton_actualiza_usuarios_clicked()
{
    actualizar_usuarios();
}


void form_usuarios::on_boton_actualiza_usuario_clicked()
{
    actualiza_usuario();
}
