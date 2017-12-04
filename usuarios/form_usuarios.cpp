#include "form_usuarios.h"
#include "ui_form_usuarios.h"
//para los msgbox:
#include <QMessageBox>
//para operar con comando de consola
#include <QProcess>
#include <QDebug>
#include <QTextStream>
#include <QFile>
#include <QDateTime>

#include "configuracion.h"
#include <ldif.h>
#include <stdio.h>
#include <stdlib.h>

LDAP *ldap;

// aqui guardamos el par usuario basedn separado por $ para que podamos saber en que UO está el usuario
// y no tengamos que buscar en el dominio grx entero
// para acelerar la busqueda a la hora de consultar los datos del usuario
QStringList usuario_basedn;
QStringList usuario_basedn2;


//convierte QString a char *
char* convierte(QString dato){
    char* cstr;
    std::string fname = dato.toStdString();
    cstr = new char [fname.size()+1];
    strcpy( cstr, fname.c_str() );
    return cstr;
}

void conecta_oldap() {
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

     /* First, we print out an informational message. */
     //printf( "Connecting to host %s at port %d...\n\n", ldap_host, ldap_port );

     // STEP 1: Establecer la conexión LDAP y fijar preferencias de la sesión ***************************************
     Qstr="ldap://" + QString::fromStdString(ldap_host) + ":" + QString::number(ldap_port);
     if(ldap_initialize(&ldap, Qstr.toLocal8Bit()))
     {
        fprintf(stderr, "ldap_initialize...error: %s\n", ldap_err2string(result));
        exit( EXIT_FAILURE );
     }
        printf("LDAP initialized\n");

     //FIN ldap_initialize ******************************************************************************************

     result = ldap_set_option(ldap, LDAP_OPT_PROTOCOL_VERSION, &ldap_version);

     if ( result != LDAP_OPT_SUCCESS ) {
         ldap_perror(ldap, "ldap_set_option failed!");
         exit(EXIT_FAILURE);
     } else {
       printf("Set LDAPv3 client version.\n");
     }

     //fijamos maximo numero de resultados devueltos
     int max_result=999;
     result = ldap_set_option(ldap, LDAP_OPT_SIZELIMIT, &max_result );

     /* STEP 2: Autentificamos al usuario en el servidor *************************************************************/
     result = ldap_simple_bind_s(ldap, ldap_dn, ldap_pw );
     result = ldap_simple_bind_s(ldap, ldap_dn, ldap_pw );
     //FIN ldap_simple_bind_s ****************************************************************************************

     if ( result != LDAP_SUCCESS ) {
       fprintf(stderr, "ldap_simple_bind_s: %s\n", ldap_err2string(result));
       //exit(EXIT_FAILURE);
     } else {
       printf("LDAP connection successful.\n");
     }
    delete configuracion;
}

void desconecta_oldap(){

    ldap_unbind(ldap);
}

int num_entradas_oldap(LDAPMessage *resul_consul){

    // Devuelve el numero de objetos encontrados durante la busqueda
    int num_entradas = ldap_count_entries(ldap, resul_consul);
    if ( num_entradas == 0 ) {
      fprintf(stderr, "LDAP search did not return any data.\n");
      //exit(EXIT_FAILURE);
      BerElement *ber;
    } else {
      printf("LDAP search returned %d objects.\n", num_entradas);
    }

    return(num_entradas);

}



LDAPMessage * consulta_oldap(char *filtro, char *attrs[], int attrsonly, const char * base_dn, int scope){

    //const char *base_dn       = "OU=SI - NUEVAS TECNOLOGIAS (SISTEMAS DE INFORMACION),OU=Recursos Perrera,DC=grx";
    //const char *base_dn       = "OU=Recursos Centros Externos,DC=grx";
    //const char *base_dn       = "DC=grx";

    int  result;

    LDAPMessage *answer;

    // The search scope must be either LDAP_SCOPE_BASE, LDAP_SCOPE_SUBTREE or LDAP_SCOPE_ONELEVEL
    //int  scope          = LDAP_SCOPE_SUBTREE;

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

    //*******************************************************************************************************************
    /* STEP 3: Hacemos la busqueda LDAP*/
    result = ldap_search_s(ldap, base_dn, scope, filtro,
                           attrs, attrsonly, &answer);
    if ( result != LDAP_SUCCESS ) {
      fprintf(stderr, "ldap_search_s: %s\n", ldap_err2string(result));
      //exit(EXIT_FAILURE);
    } else {
      printf("LDAP search successful.\n");
    }
    //*******************************************************************************************************************
    return (answer);
}


form_usuarios::form_usuarios(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::form_usuarios)
{
    ui->setupUi(this);

    // rellenamos el combobox con todos los usuarios del dominio de las OU que se usan

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

    conecta_oldap();

    /*//recorremos las unidades organizativas que vamos a cargar
    resul_consul=consulta_oldap("(&(objectClass=organizationalUnit)(name=recursos*))", attrs, 0, "DC=grx");
    //entry = ldap_first_entry(ldap, resul_consul);
    QMessageBox msgBox;
    //msgBox.setText(QString::fromStdString(ldap_get_dn(ldap, entry)));
    msgBox.setText(QString::number(ldap_count_entries(ldap, resul_consul)));
    msgBox.exec();*/

    //QString Qstr;
    //Qstr=("(&(objectClass=user)(sAMAccountName=" + ui->comboBox_usuarios->currentText() + "*))");
    //resul_consul=consulta_oldap(convierte(Qstr), attrs, 0);
    //resul_consul=consulta_oldap("(&(!(objectclass=computer))(objectClass=user)(|(sAMAccountName=ab_*)(|(sAMAccountName=ab_*)(|(sAMAccountName=si_*)(sAMAccountName=ss_*)))))", attrs, 0);

    // lo usamos para ir guardando los resultados de las consultas, ordenarlo y luego se lo pasamos al combo
    QStringList a;
    QStringList b;
    // lista de Unidades Organizativas que se van a ir consultando
    QStringList OU;

     int  i = 0;
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
     //recorremos todo OU
     foreach (const QString &qstr, OU) {

         resul_consul=consulta_oldap("(&(!(objectclass=computer))(objectClass=user))", attrs, 0, qstr.toLocal8Bit(),LDAP_SCOPE_SUBTREE);

         // Devuelve el numero de objetos encontrados durante la busqueda
         num_entradas = ldap_count_entries(ldap, resul_consul);
         if ( num_entradas == 0 ) {
           fprintf(stderr, "LDAP search did not return any data.\n");
           //exit(EXIT_FAILURE);
         } else {
           printf("LDAP search returned %d objects.\n", num_entradas);
         }

         // recorremos todos los objetos (entry) devueltos por la consulta
         // cada entry tiene atributos
         for ( entry = ldap_first_entry(ldap, resul_consul); entry != NULL; entry = ldap_next_entry(ldap, entry))
         {

           // Imprimimos la cadena DN del objeto
           dn = ldap_get_dn(ldap, entry);
           //printf("Found Object: %s\n", dn);

           // recorremos todos los atributos de cada entry
           for ( atributo = ldap_first_attribute(ldap, entry, &ber);atributo != NULL;
                 atributo = ldap_next_attribute(ldap, entry, ber))
           {

             // Imprimimos el nombre del atributo
             //printf("Found Attribute: %s\n", atributo);
             if (QString::fromStdString(atributo)=="sAMAccountName"){
                 if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                     //ldap_sort_values(ldap,values,LDAP_SORT_AV_CMP_PROC("a","x"));
                   // recorreomos todos los valores devueltos por este atributo
                   for (i = 0; values[i] != NULL; i++) {
                        a<<QString::fromStdString(values[i]).toUpper();
                        //en usuario_basedn guardamos usuario$base_dn para consultar el dn (UO y dominio) del usuario en memoria
                        usuario_basedn<<QString::fromStdString(values[i]).toUpper() + "$" + qstr;
                     // print each value of a attribute here
                     //printf("%s: %s\n", atributo, values[i] );
                   }
                   ldap_value_free(values);
                 }
              }
             if (QString::fromStdString(atributo)=="cn"){
                 if ((values = ldap_get_values(ldap, entry, atributo)) != NULL) {
                     //ldap_sort_values(ldap,values,LDAP_SORT_AV_CMP_PROC("a","x"));
                   // recorreomos todos los valores devueltos por este atributo
                   for (i = 0; values[i] != NULL; i++) {
                        b<<QString::fromStdString(values[i]).toUpper();
                        usuario_basedn2<<QString::fromStdString(values[i]).toUpper() + "$" + qstr;
                     // print each value of a attribute here
                     //printf("%s: %s\n", atributo, values[i] );
                   }
                   ldap_value_free(values);
                 }
              }
           }
           ldap_memfree(dn);
         }

         ldap_msgfree(resul_consul);
    }

    //ordenamos la lista
     qSort(usuario_basedn);//ordenamos el QStringList
     qSort(a);//ordenamos el QStringList
     ui->comboBox_usuarios->addItems(a);//insertamos los datos

     qSort(usuario_basedn2);//ordenamos el QStringList
     qSort(b);//ordenamos el QStringList
     ui->comboBox_nombres->addItems(b);//insertamos los datos
    //desconecta_oldap();


}


form_usuarios::~form_usuarios()
{
    delete ui;
}

void form_usuarios::on_form_usuarios_destroyed()
{

    desconecta_oldap();

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

// Procedimiento para rellenar los campos del formulario
void form_usuarios::rellena(QString consulta, QString base_DN) {

    QDateTime fecha;
    QString temp, basedn1;
    QString pwdLastSet,userAccountControl;

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

    //conecta_oldap();

    int  i = 0;

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
       //printf("Found Object: %s\n", dn);

       // recorremos todos los atributos de cada entry
       for ( atributo = ldap_first_attribute(ldap, entry, &ber);atributo != NULL;
             atributo = ldap_next_attribute(ldap, entry, ber))
       {

         // Nombre y apellidos
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
