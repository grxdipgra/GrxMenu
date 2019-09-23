#include "basedatos.h"
#include "ui_basedatos.h"
#include <QStandardItemModel>
#include <QTreeView>
#include <QFile>
#include <QFileDialog>
#include <QNetworkInterface>
#include <QDesktopServices>
#include <QSqlRecord>
#include <QDebug>
#include "tunel.h"
#include "soporte/nmap_xml.h"
#include "configuracion/configuracion.h"
#include "lib/lib.h"
#include "lib/globals.h"
#include <QDateTime>
#include <QFileInfo>
#include <QSqlField>
#include <QProgressDialog>
#include <QString>
#include <QAction>
#include <QDir>


BaseDatos::~BaseDatos() {
    delete ui;
}

/*!
 * \brief Manejo de las bases de datos de la aplicación.<br>
 * Tenemos dos DB, una local con sqlite y otra remota con mysql.<br>
 * Por defecto trabajamos en modo local y solo hacemos uso de la remota para actualizar la DB local.
 * \param completo
 * \param parent
 */

BaseDatos::BaseDatos(bool completo, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BaseDatos)
{
    ui->setupUi(this);
    if (completo){
        inicia();
        cabeceras();
        cargaVariables();
    }
}

BaseDatos::BaseDatos( QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BaseDatos)
{
    ui->setupUi(this);
    inicia();
    cabeceras();
    cargaVariables();

}
/*!
 * \brief Crea la conexion a la base de datos mysql
  * \return Devuelve el estado de la conexion de la DB; true si la DB está abierta, false en caso contrario
 */
bool BaseDatos::inicia_mysql(){
    db_mysql = QSqlDatabase::database("QMYSQL");
    if (!db_mysql.open())
        if (!db_mysql.isValid())
        {
            db_mysql = QSqlDatabase::addDatabase("QMYSQL","mysql");
        }

    if (db_mysql.isValid())
        return true;

return false;

}

/*!
 * \brief Crea la conexion a la base de datos local sqlite
  * \return Devuelve el estado de la conexion de la DB; true si la DB está abierta, false en caso contrario
 */
bool BaseDatos::inicia_sqlite(){

    db_sqlite = QSqlDatabase::database("sqlite");
    if (!db_sqlite.open())
        if (!db_sqlite.isValid())
        {
            db_sqlite = QSqlDatabase::addDatabase("QSQLITE","sqlite");
        }

    if (db_sqlite.isValid())
        return true;
return false;
}

/*!
 * \brief Carga las variables de la conexion a la DB.<br>
 * En el caso de conexiones a la DB de mysql remota, lo hacemos a través de ssh con la libreria libssh.<br>
 * Tambien cargamos las variables necesarias para hacer una conexion ssh en caso de estar activa la opcion ssh en la configuracion.<br>
 * Antes de hacer la conexion con la DB mysql externa comprobamos que el puerto remoto esté abierto en el servidor<br>
 * \return Devuelve true en caso de no detectar errores en la conexiones con las DB, false en caso contrario
 */
bool BaseDatos::cargaVariables(){

    Configuracion *configuracion = new Configuracion;
    //Cargamos en el struct (datos) los datos que le pasaremos a las librerias libssh
    datos.keyfile1=configuracion->cual_es_keyfile_publica();
    datos.keyfile2=configuracion->cual_es_keyfile_privada();
    datos.username_ssh=configuracion->cual_es_usuarioSSH();
    datos.password_ssh=configuracion->cual_es_password_ssh();
    datos.username_DB=configuracion->cual_es_usernameDB();
    datos.password_DB=configuracion->cual_es_passwordDB();
    datos.hostname_DB=configuracion->cual_es_hostnameDB();
    datos.local_listenip="127.0.0.1";

    if (configuracion->usarSSH()){
        datos.remote_port=configuracion->cual_es_puerto_remoto_ssh();
        datos.server_ip=configuracion->cual_es_servidorSSH().toLatin1().data();
    }
    else{
        datos.remote_port=configuracion->cual_es_PuertoDB();
        datos.server_ip="127.0.0.1";
        conectado=true;
    }
   datos.remote_desthost="127.0.0.1";
   datos.databasename=configuracion->cual_es_DataBaseName();

   NMap* nmap = new NMap(); //Comprobamos que el puerto de ssh este abierto en el servidor
   if (datos.remote_port!=0){
     nmap->nmap_run_scan(QString::number(datos.remote_port),datos.server_ip); //Escaneamos puertos
     if (nmap->nmap_is_open_port(datos.server_ip, QString::number(datos.remote_port))){
       if (configuracion->usarSSH()){ //Tenemos seleccionado usar tunel ssh
               datos.local_listenport=puerto_libre();
               datos.usar_ssh=true;
               db_mysql.setPort(datos.local_listenport);
               if (!creaConexion()){
                   QMessageBox::critical(this, "Error SSH", "No hemos podido crear una conexion SSH con el servidor, compruebe la configuracion ",QMessageBox::Ok);
                   return false;
               }else {
                    qDebug()<<"ssh perfect";
               }
       }
       else{
               datos.usar_ssh=false;
               db_mysql.setDatabaseName(datos.databasename);
               db_mysql.setHostName("127.0.0.1");
               db_mysql.setUserName(datos.username_DB);
               db_mysql.setPassword(datos.password_DB);
               db_mysql.setPort(configuracion->cual_es_PuertoDB());
               if (!db_mysql.open()){
                   qDebug() <<"Mysql sin conexión";
                   return false;
               }
               else  {
                   qDebug() <<"Mysql conectado";
                   conectado=true;
                   emit conectadoDB();
                   }
               }
   }
       else{
            qDebug() << "Puerto Cerrado";
       }
   }

    //Configuramos sqlite y la abrimos
    QString rutaDB_sqlite = configuracion->cual_es_ruta_sqlite();
    db_sqlite.setDatabaseName(rutaDB_sqlite);
    if (!db_sqlite.open()){
        QMessageBox::critical(this, "Error SQLite", "No hemos podido abrir sqlite, compruebe la configuracion ",QMessageBox::Ok);
        return false;
     }
     else{
       qDebug() << "Conectado SQLITE";
     }

delete configuracion;
return true;
}

/*!
 * \brief Crea una conexion ssh metiendolo en un hilo para evitar dejar colgada la aplicacion en caso de demora en la conexion.<br>
 * Cuando se crea un hilo nuevo creamos tunel(ssh). Cuando el tunel emite la señal de conectado, indica que se ha conectado al servidor ssh remoto.<br>
 * Una vez conectado por ssh, configuramos la DB y emitimos otra señal de conectado en caso de exito.
 * \return Devuelve true en caso de exito
 */
bool BaseDatos::creaConexion()
{
    Tunel *tunel = new Tunel;
    QThread *hilo= new QThread;
    tunel->keyfile1=convierte(datos.keyfile1);
    tunel->keyfile2=convierte(datos.keyfile2);
    tunel->username_ssh=convierte(datos.username_ssh);
    tunel->remote_destport=datos.remote_destport;
    tunel->local_listenport=datos.local_listenport;
    tunel->local_listenip=("127.0.0.1");
    tunel->remote_port=datos.remote_port;
    tunel->remote_desthost=convierte(datos.remote_desthost);
    tunel->password_ssh=convierte(datos.password_ssh);
    tunel->server_ip=convierte(datos.server_ip);
    tunel->remote_destport=3306;//mysql remoto
    tunel->moveToThread(hilo);
    QObject::connect(hilo,&QThread::started, tunel, &Tunel::crea_conexion); /** No se si esto funciona */
    QObject::connect(tunel,&Tunel::destroyed, hilo, &QThread::quit);
    QObject::connect(tunel,&Tunel::sshConectado, this, &BaseDatos::configuraBasedatos);
    QObject::connect(tunel,&Tunel::sshDesconectado,tunel,&Tunel::cierra_conexion);

    hilo->start();
    return true;
}

/*!
 * \brief Configura la conexion mysql y emite una señal en caso de estar conectado con exito a la DB<br>
 * Esta funcion se llama cuando sshConectado ha emitido la señal de estar conectados por ssh
 */
void BaseDatos::configuraBasedatos(){
    db_mysql.setDatabaseName(datos.databasename);
    db_mysql.setHostName(datos.hostname_DB);
    db_mysql.setUserName(datos.username_DB);
    db_mysql.setPassword(datos.password_DB);
    db_mysql.setPort(datos.local_listenport);
    if (!db_mysql.open()){
        conectado = false;
    }else{
        conectado = true;
        emit conectadoDB();
    }
}

/*!
 * \brief Crea las cabeceras de las tablas (nombres del campo)
 */
void BaseDatos::cabeceras() {
    model_municipio->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    model_municipio->setHeaderData(1, Qt::Horizontal, QObject::tr("idComarca"));
    model_municipio->setHeaderData(2, Qt::Horizontal, QObject::tr("idMancomunidad"));
    model_municipio->setHeaderData(3, Qt::Horizontal, QObject::tr("nombre"));
    model_municipio->setHeaderData(4, Qt::Horizontal, QObject::tr("nombreId"));
    model_municipio->setHeaderData(5, Qt::Horizontal, QObject::tr("CIF"));
    model_municipio->setHeaderData(6, Qt::Horizontal, QObject::tr("codigoDIR3"));
    model_municipio->setHeaderData(7, Qt::Horizontal, QObject::tr("codigoINE"));
    model_municipio->setHeaderData(8, Qt::Horizontal, QObject::tr("numeroHabitantes"));
    model_municipio->setHeaderData(9, Qt::Horizontal, QObject::tr("superficie"));
    model_municipio->setHeaderData(10, Qt::Horizontal, QObject::tr("altitud"));
    model_municipio->setHeaderData(11, Qt::Horizontal, QObject::tr("latitud"));
    model_municipio->setHeaderData(12, Qt::Horizontal, QObject::tr("longitud"));
    model_municipio->setHeaderData(13, Qt::Horizontal, QObject::tr("urlBandera"));
    model_municipio->setHeaderData(14, Qt::Horizontal, QObject::tr("urlEscudo"));
    model_municipio->setHeaderData(15, Qt::Horizontal, QObject::tr("web"));
    model_municipio->setHeaderData(16, Qt::Horizontal, QObject::tr("siglasPartidoPolitico"));
    model_municipio->setHeaderData(17, Qt::Horizontal, QObject::tr("nombrePartidoPolitico"));
    model_municipio->setHeaderData(18, Qt::Horizontal, QObject::tr("nombreAlcalde"));
    model_municipio->setHeaderData(19, Qt::Horizontal, QObject::tr("fechaElecciones"));
    model_municipio->setHeaderData(20, Qt::Horizontal, QObject::tr("urlTablon"));
    model_municipio->setHeaderData(21, Qt::Horizontal, QObject::tr("urlPortalTransparencia"));

    model_poblacion->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    model_poblacion->setHeaderData(1, Qt::Horizontal, QObject::tr("idMunicipio"));
    model_poblacion->setHeaderData(2, Qt::Horizontal, QObject::tr("idEla"));
    model_poblacion->setHeaderData(3, Qt::Horizontal, QObject::tr("nombre"));
    model_poblacion->setHeaderData(4, Qt::Horizontal, QObject::tr("nombreId"));

    model_nodo->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    model_nodo->setHeaderData(1, Qt::Horizontal, QObject::tr("idPoblacion"));
    model_nodo->setHeaderData(2, Qt::Horizontal, QObject::tr("idCentro"));
    model_nodo->setHeaderData(3, Qt::Horizontal, QObject::tr("nombre"));
    model_nodo->setHeaderData(4, Qt::Horizontal, QObject::tr("tipoVia"));
    model_nodo->setHeaderData(5, Qt::Horizontal, QObject::tr("nombreVia"));
    model_nodo->setHeaderData(6, Qt::Horizontal, QObject::tr("numeroDireccion"));
    model_nodo->setHeaderData(7, Qt::Horizontal, QObject::tr("letraDireccion"));
    model_nodo->setHeaderData(8, Qt::Horizontal, QObject::tr("escaleraDireccion"));
    model_nodo->setHeaderData(9, Qt::Horizontal, QObject::tr("pisoDireccion"));
    model_nodo->setHeaderData(10, Qt::Horizontal, QObject::tr("puertaDireccion"));
    model_nodo->setHeaderData(11, Qt::Horizontal, QObject::tr("codigoPostal"));
    model_nodo->setHeaderData(12, Qt::Horizontal, QObject::tr("latitud"));
    model_nodo->setHeaderData(13, Qt::Horizontal, QObject::tr("longitud"));
    model_nodo->setHeaderData(14, Qt::Horizontal, QObject::tr("contacto"));
    model_nodo->setHeaderData(15, Qt::Horizontal, QObject::tr("extension"));
    model_nodo->setHeaderData(16, Qt::Horizontal, QObject::tr("fax"));
    model_nodo->setHeaderData(17, Qt::Horizontal, QObject::tr("web"));
    model_nodo->setHeaderData(18, Qt::Horizontal, QObject::tr("sede"));
    model_nodo->setHeaderData(19, Qt::Horizontal, QObject::tr("adslLinea"));
    model_nodo->setHeaderData(20, Qt::Horizontal, QObject::tr("numAdministrativo"));
    model_nodo->setHeaderData(21, Qt::Horizontal, QObject::tr("ipLinea"));
    model_nodo->setHeaderData(22, Qt::Horizontal, QObject::tr("ipCifrado"));
    model_nodo->setHeaderData(23, Qt::Horizontal, QObject::tr("servicioLinea"));
    model_nodo->setHeaderData(24, Qt::Horizontal, QObject::tr("caudalLinea"));
    model_nodo->setHeaderData(25, Qt::Horizontal, QObject::tr("equipamientoLinea"));
    model_nodo->setHeaderData(26, Qt::Horizontal, QObject::tr("numeroSerieRouter"));
    model_nodo->setHeaderData(27, Qt::Horizontal, QObject::tr("dns1"));
    model_nodo->setHeaderData(28, Qt::Horizontal, QObject::tr("dns2"));
    model_nodo->setHeaderData(29, Qt::Horizontal, QObject::tr("proxy"));

    model_programa->setHeaderData(0, Qt::Horizontal, QObject::tr("idNodo"));
    model_programa->setHeaderData(1, Qt::Horizontal, QObject::tr("anio"));
    model_programa->setHeaderData(2, Qt::Horizontal, QObject::tr("portalWeb"));
    model_programa->setHeaderData(3, Qt::Horizontal, QObject::tr("email"));
    model_programa->setHeaderData(4, Qt::Horizontal, QObject::tr("baseDatosJuridica"));
    model_programa->setHeaderData(5, Qt::Horizontal, QObject::tr("suscripcionDominio"));
    model_programa->setHeaderData(6, Qt::Horizontal, QObject::tr("perfilContratante"));
    model_programa->setHeaderData(7, Qt::Horizontal, QObject::tr("gestionMunicipal"));
    model_programa->setHeaderData(8, Qt::Horizontal, QObject::tr("gestionEconomica"));
    model_programa->setHeaderData(9, Qt::Horizontal, QObject::tr("soporte"));
    model_programa->setHeaderData(10, Qt::Horizontal, QObject::tr("sedeElectronica"));
    model_programa->setHeaderData(11, Qt::Horizontal, QObject::tr("epol"));
    model_programa->setHeaderData(12, Qt::Horizontal, QObject::tr("epolMovil"));
    model_programa->setHeaderData(13, Qt::Horizontal, QObject::tr("siapol"));

    model_emailnodo->setHeaderData(0, Qt::Horizontal, QObject::tr("idNodo"));
    model_emailnodo->setHeaderData(1, Qt::Horizontal, QObject::tr("email"));

    model_telefononodo->setHeaderData(0, Qt::Horizontal, QObject::tr("idNodo"));
    model_telefononodo->setHeaderData(1, Qt::Horizontal, QObject::tr("telefono"));

    model_comarca->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    model_comarca->setHeaderData(1, Qt::Horizontal, QObject::tr("nombre"));

    model_mancomunidad->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    model_mancomunidad->setHeaderData(1, Qt::Horizontal, QObject::tr("nombre"));

    model_centro->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    model_centro->setHeaderData(1, Qt::Horizontal, QObject::tr("nombre"));
    model_centro->setHeaderData(2, Qt::Horizontal, QObject::tr("direccion"));
    model_centro->setHeaderData(3, Qt::Horizontal, QObject::tr("telefono"));

    model_diafestivopoblacion->setHeaderData(0, Qt::Horizontal, QObject::tr("idPoblacion"));
    model_diafestivopoblacion->setHeaderData(1, Qt::Horizontal, QObject::tr("diaFestivo"));
    model_diafestivopoblacion->setHeaderData(2, Qt::Horizontal, QObject::tr("descripcion"));

    model_aplicacion->setHeaderData(0, Qt::Horizontal, QObject::tr("idNodo"));
    model_aplicacion->setHeaderData(1, Qt::Horizontal, QObject::tr("atalaya"));
    model_aplicacion->setHeaderData(2, Qt::Horizontal, QObject::tr("glpi"));
    model_aplicacion->setHeaderData(3, Qt::Horizontal, QObject::tr("osc"));

    model_ldap->setHeaderData(0, Qt::Horizontal, QObject::tr("idLdap"));
    model_ldap->setHeaderData(1, Qt::Horizontal, QObject::tr("usuario"));
    model_ldap->setHeaderData(2, Qt::Horizontal, QObject::tr("nombre"));
    model_ldap->setHeaderData(3, Qt::Horizontal, QObject::tr("cuenta_caduca"));
    model_ldap->setHeaderData(4, Qt::Horizontal, QObject::tr("correo"));
    model_ldap->setHeaderData(5, Qt::Horizontal, QObject::tr("creada"));
    model_ldap->setHeaderData(6, Qt::Horizontal, QObject::tr("estado"));
    model_ldap->setHeaderData(7, Qt::Horizontal, QObject::tr("fecha_correo"));
    model_ldap->setHeaderData(8, Qt::Horizontal, QObject::tr("intentos"));
    model_ldap->setHeaderData(9, Qt::Horizontal, QObject::tr("logon"));
    model_ldap->setHeaderData(10, Qt::Horizontal, QObject::tr("modificacion_cuenta"));
    model_ldap->setHeaderData(11, Qt::Horizontal, QObject::tr("telefono"));
    model_ldap->setHeaderData(12, Qt::Horizontal, QObject::tr("ultimo_login"));
    model_ldap->setHeaderData(13, Qt::Horizontal, QObject::tr("descripcion"));
    model_ldap->setHeaderData(14, Qt::Horizontal, QObject::tr("dn"));
    model_ldap->setHeaderData(15, Qt::Horizontal, QObject::tr("useraccountcontrol"));
    model_ldap->setHeaderData(16, Qt::Horizontal, QObject::tr("ultimo_cambio_clave"));
    model_ldap->setHeaderData(17, Qt::Horizontal, QObject::tr("user"));
    model_ldap->setHeaderData(18, Qt::Horizontal, QObject::tr("user_preW2000"));
    model_ldap->setHeaderData(19, Qt::Horizontal, QObject::tr("nombre_solo"));
    model_ldap->setHeaderData(20, Qt::Horizontal, QObject::tr("apellidos"));
    model_ldap->setHeaderData(21, Qt::Horizontal, QObject::tr("nombre_mostrar"));
    model_ldap->setHeaderData(22, Qt::Horizontal, QObject::tr("movil"));
    model_ldap->setHeaderData(23, Qt::Horizontal, QObject::tr("fax"));
    model_ldap->setHeaderData(24, Qt::Horizontal, QObject::tr("departamento"));
    model_ldap->setHeaderData(25, Qt::Horizontal, QObject::tr("oficina"));
    model_ldap->setHeaderData(26, Qt::Horizontal, QObject::tr("organizacion"));
    model_ldap->setHeaderData(27, Qt::Horizontal, QObject::tr("puesto"));
    model_ldap->setHeaderData(28, Qt::Horizontal, QObject::tr("provincia"));
    model_ldap->setHeaderData(29, Qt::Horizontal, QObject::tr("localidad"));
    model_ldap->setHeaderData(31, Qt::Horizontal, QObject::tr("direccion"));
    model_ldap->setHeaderData(32, Qt::Horizontal, QObject::tr("cp"));
    model_ldap->setHeaderData(33, Qt::Horizontal, QObject::tr("notas"));
    model_ldap->setHeaderData(34, Qt::Horizontal, QObject::tr("ultima_sincro"));

}

/*!
 * \brief Inicia y configura los tableView con las tablas de las DB.<br>
 * Tambien configura las consultas sql por defecto
 */
void BaseDatos::inicia() {

    if (!inicia_mysql()){
            qDebug()<< "No hemos podido inicar mysql";
    }
    if (!inicia_sqlite()){
            qDebug()<< "No hemos podido inicar sqlite";
    }

    model_municipio->setTable("municipio");
    model_municipio->select();
    ui->tableView_municipio->setModel(model_municipio);
    ui->tableView_municipio->resizeColumnsToContents();
    ui->tableView_municipio->resizeRowsToContents();
    ui->tableView_municipio->setSortingEnabled(true);
    ui->tableView_municipio->sortByColumn(0, Qt::AscendingOrder);

    model_nodo->setTable("nodo");
    model_nodo->select();
    ui->tableView_nodo->setModel(model_nodo);
    ui->tableView_nodo->resizeColumnsToContents();
    ui->tableView_nodo->resizeRowsToContents();
    ui->tableView_nodo->setSortingEnabled(true);
    ui->tableView_nodo->sortByColumn(0, Qt::AscendingOrder);

    model_poblacion->setTable("poblacion");
    model_poblacion->select();
    ui->tableView_poblacion->setModel(model_poblacion);
    ui->tableView_poblacion->resizeColumnsToContents();
    ui->tableView_poblacion->resizeRowsToContents();
    ui->tableView_poblacion->setSortingEnabled(true);
    ui->tableView_poblacion->sortByColumn(0, Qt::AscendingOrder);

    model_centro->setTable("centro");
    model_centro->select();
    ui->tableView_centro->setModel(model_centro);
    ui->tableView_centro->resizeColumnsToContents();
    ui->tableView_centro->resizeRowsToContents();
    ui->tableView_centro->setSortingEnabled(true);
    ui->tableView_centro->sortByColumn(0, Qt::AscendingOrder);

    model_aplicacion->setTable("aplicacion");
    model_aplicacion->select();
    ui->tableView_aplicaciones->setModel(model_aplicacion);
    ui->tableView_aplicaciones->resizeColumnsToContents();
    ui->tableView_aplicaciones->resizeRowsToContents();
    ui->tableView_aplicaciones->setSortingEnabled(true);
    ui->tableView_aplicaciones->sortByColumn(0, Qt::AscendingOrder);

    model_programa->setTable("programa");
    model_programa->select();
    ui->tableView_programa->setModel(model_programa);
    ui->tableView_programa->resizeColumnsToContents();
    ui->tableView_programa->resizeRowsToContents();
    ui->tableView_programa->setSortingEnabled(true);
    ui->tableView_programa->sortByColumn(0, Qt::AscendingOrder);

    model_emailnodo->setTable("emailnodo");
    model_emailnodo->select();
    ui->tableView_emailnodo->setModel(model_emailnodo);
    ui->tableView_emailnodo->resizeColumnsToContents();
    ui->tableView_emailnodo->resizeRowsToContents();
    ui->tableView_emailnodo->setSortingEnabled(true);
    ui->tableView_emailnodo->sortByColumn(0, Qt::AscendingOrder);

    model_telefononodo->setTable("telefononodo");
    model_telefononodo->select();
    ui->tableView_telefononodo->setModel(model_telefononodo);
    ui->tableView_telefononodo->resizeColumnsToContents();
    ui->tableView_telefononodo->resizeRowsToContents();
    ui->tableView_telefononodo->setSortingEnabled(true);
    ui->tableView_telefononodo->sortByColumn(0, Qt::AscendingOrder);

    model_comarca->setTable("comarca");
    model_comarca->select();
    ui->tableView_comarca->setModel(model_comarca);
    ui->tableView_comarca->resizeColumnsToContents();
    ui->tableView_comarca->resizeRowsToContents();
    ui->tableView_comarca->setSortingEnabled(true);
    ui->tableView_comarca->sortByColumn(0, Qt::AscendingOrder);

    model_mancomunidad->setTable("mancomunidad");
    model_mancomunidad->select();
    ui->tableView_mancomunidad->setModel(model_mancomunidad);
    ui->tableView_mancomunidad->resizeColumnsToContents();
    ui->tableView_mancomunidad->resizeRowsToContents();
    ui->tableView_mancomunidad->setSortingEnabled(true);
    ui->tableView_mancomunidad->sortByColumn(0, Qt::AscendingOrder);

    model_ldap->setTable("ldap");
    model_ldap->select();
    ui->tableView_usuarios->setModel(model_ldap);
    ui->tableView_usuarios->resizeColumnsToContents();
    ui->tableView_usuarios->resizeRowsToContents();
    ui->tableView_usuarios->setSortingEnabled(true);
    ui->tableView_usuarios->sortByColumn(0, Qt::AscendingOrder);

    sql[0] = "";

    sql[1] =    "SELECT * from nodo";

    sql[2] =    "SELECT n.nombre, "
                    "pro.anio, "
                    "pro.portalWeb as 165_A, "
                    "pro.email as B, "
                    "pro.baseDatosJuridica as C, "
                    "pro.suscripcionDominio as D, "
                    "pro.perfilContratante as E, "
                    "pro.gestionMunicipal as 166_A, "
                    "pro.gestionEconomica as B, "
                    "pro.soporte as C, "
                    "pro.sedeElectronica as 167_A, "
                    "pro.epol as 168_A, "
                    "pro.epolMovil as B, "
                    "pro.siapol as C "
                "FROM asismun.programa pro INNER JOIN nodo n ON pro.idNodo = n.id "
                "ORDER BY n.nombre ASC, pro.anio ASC;";

    sql[3] =    "SELECT n.id as idNodo, "
                    "n.nombre as Nodo, "
                    "p.nombre as Población, "
                    "GROUP_CONCAT(DISTINCT tn.telefono ORDER BY tn.telefono SEPARATOR '\n') as Teléfono, "
                    "n.fax as Fax, "
                    "GROUP_CONCAT(DISTINCT en.email ORDER BY en.email SEPARATOR '\n') as Email, "
                    "n.web as Web, "
                    "n.tipoVia as TipoVia, "
                    "n.nombreVia as NombreVia, "
                    "n.numeroDireccion as NumeroVia, "
                    "n.codigoPostal as CódigoPostal "
                "FROM asismun.nodo n "
                "INNER JOIN asismun.poblacion p ON n.idPoblacion = p.id "
                "LEFT JOIN asismun.telefononodo tn ON n.id = tn.idNodo "
                "LEFT JOIN asismun.emailnodo en ON n.id = en.idNodo "
                "WHERE n.tipoVia is null and tn.idNodo is null "
                "GROUP BY n.id "
                "ORDER BY n.nombre;";

    sql[4] =    "SELECT c.nombre as Centro, "
                    "GROUP_CONCAT(DISTINCT n.nombre ORDER BY n.nombre SEPARATOR '\n') as Nodo "
                "FROM asismun.centro c "
                "LEFT JOIN nodo n ON c.id = n.idCentro "
                "GROUP BY c.id "
                "ORDER BY c.nombre;";

    sql[5] =    "SELECT n.id as idNodo, "
                    "case when ela.id is null "
                        "then n.nombre else CONCAT(n.nombre, ' (ELA)') "
                    "end as Nodo, "
                    "case when ela.id is null "
                        "then m.nombre else ela.nombre "
                        "end as Municipio, "
                    "p.nombre as Población, "
                    "GROUP_CONCAT(DISTINCT tn.telefono ORDER BY tn.telefono SEPARATOR '\n') as Teléfono, "
                    "n.fax as Fax, "
                    "GROUP_CONCAT(DISTINCT en.email ORDER BY en.email SEPARATOR '\n') as Email, "
                    "n.web as Web, "
                    "n.tipoVia as TipoVia, "
                    "n.nombreVia as NombreVia, "
                    "n.numeroDireccion as NumeroVia, "
                    "n.codigoPostal as CódigoPostal, "
                    "case when (ela.id is null and m.urlEscudo is not null) or (ela.id is not null and ela.urlEscudo is not null) "
                        "then 'SI' else null "
                        "end as escudo "
                "FROM asismun.nodo n "
                "INNER JOIN asismun.poblacion p ON n.idPoblacion = p.id "
                "INNER JOIN asismun.municipio m ON p.idMunicipio = m.id "
                "LEFT JOIN asismun.municipio ela ON p.idEla = ela.id "
                "LEFT JOIN asismun.telefononodo tn ON n.id = tn.idNodo "
                "LEFT JOIN asismun.emailnodo en ON n.id = en.idNodo "
                "WHERE esAyuntamiento = 1 "
                "GROUP BY n.id "
                "ORDER BY n.nombre;";

    sql[6] =    "SELECT n.nombre as Nodo, "
                    "p.* "
                "FROM asismun.programa p "
                "INNER JOIN nodo n ON p.idNodo = n.id "
                "ORDER BY n.nombre;";

    sql[7] =    "SELECT m.nombre as Municipio, "
                    "GROUP_CONCAT(DISTINCT p.nombre ORDER BY p.nombre SEPARATOR '\n') as Población "
                "FROM municipio m "
                "INNER JOIN poblacion p ON m.id = p.idMunicipio "
                "GROUP BY m.id "
                "ORDER BY m.nombre;";

    sql[8] =    "SELECT p.nombre as Población, "
                    "GROUP_CONCAT(DISTINCT n.nombre ORDER BY n.nombre SEPARATOR '\n') as Nodo "
                "FROM asismun.poblacion p "
                "INNER JOIN nodo n ON p.id = n.idPoblacion "
                "GROUP BY p.id "
                "ORDER BY p.nombre;";

    sql[9] =    "SELECT n.equipamientoLinea as Router, "
                    "count(*) as Total, "
                    "GROUP_CONCAT(DISTINCT n.caudalLinea ORDER BY n.caudalLinea SEPARATOR '\n') as Caudales "
                "FROM nodo n "
                "GROUP BY n.equipamientoLinea "
                "ORDER BY n.equipamientoLinea;";

}

/*!
 * \brief Realiza la consulta sql contenida en ui->comboBox_consulta y las muestra en en tableView_consulta
 */
void BaseDatos::on_pB_sql_clicked() {
    Configuracion *configuracion= new Configuracion;
    QString rutaDB_sqlite = configuracion->cual_es_ruta_sqlite();
    if (!db_sqlite.open()){
        QMessageBox::critical(this, "Sql Error", "No puedo abrir sqlite", QMessageBox::Ok);
    } else{
            QSqlQuery* query_consulta = new QSqlQuery(db_sqlite);
            QString sql_consulta = ui->comboBox_consulta->currentText();
            query_consulta->prepare(sql_consulta);

            if (!query_consulta->exec()) {
                    QMessageBox::critical(this, "Sql Error", "Error en la consulta: \n" + query_consulta->lastError().text(), QMessageBox::Ok);
            }
            else {
                model_consulta->setQuery(*query_consulta);
                ui->tableView_consulta->setModel(model_consulta);
                ui->tableView_consulta->resizeColumnsToContents();
                ui->tableView_consulta->resizeRowsToContents();
            }
    }
}

/*!
 * \brief Realiza la consulta sql contenida en ui->comboBox_consulta
 * \param arg1
 */
void BaseDatos::on_comboBox_consulta_activated(const QString &arg1) {
    QSqlDatabase db = QSqlDatabase::database("sqlite");
    QSqlQuery* query_consulta = new QSqlQuery(db);
    QString sql_consulta;
    int indice = ui->comboBox_consulta->currentIndex();

    if ((indice > 0) && (indice < 10)) {
        sql_consulta = sql[indice];
    }
    else {
        sql_consulta = arg1;
    }

    if (!sql_consulta.isNull()) {
        query_consulta->prepare(sql_consulta);

        if (!query_consulta->exec()) {
            QMessageBox::critical(this, "Sql Error", "Error en la consulta: \n" + query_consulta->lastError().text(), QMessageBox::Ok);
        }
        else {
            model_consulta->setQuery(*query_consulta);
            ui->tableView_consulta->setModel(model_consulta);
            ui->tableView_consulta->resizeColumnsToContents();
            ui->tableView_consulta->resizeRowsToContents();
        }
     }
}

void BaseDatos::on_pB_csv_clicked()
{

    QString textData;
    model_consulta=ui->tableView_consulta->model();
    int rows = model_consulta->rowCount();
    int columns = model_consulta->columnCount();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
                textData += model_consulta->data(model_consulta->index(i,j)).toString();
                textData += ", " ;     // para .csv
        }
        textData += "\n";             // (opcional)
    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("Exportar fichero a csv"),
                               "./select.csv",
                               tr("CSV (*.csv)"));

    QFile csvFile(fileName);
    if(csvFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream out(&csvFile);
        out << textData;

        csvFile.close();
    }
}

bool BaseDatos::crearDB(QString rutaDB){


//Borra el archivo de bak si existe y si existe la DB le cambia el nombre a .bak
    QString rutaDB_bak;
    rutaDB_bak=rutaDB+".bak";
    if (fileExists(rutaDB)){
        if (fileExists(rutaDB_bak)) {
            QFile file_bak(rutaDB_bak);
            file_bak.remove();
        }
        QFile file(rutaDB);
        file.rename(rutaDB,rutaDB_bak);
    }

    QSqlQuery query(db_sqlite);
    QList<QString> crea_tablas;

    QString aplicacion = "CREATE TABLE aplicacion "
                           "(idNodo mediumint(6) NOT NULL,"
                           "atalaya smallint(4) DEFAULT NULL,"
                           "glpi smallint(4) DEFAULT NULL,"
                           "ocs smallint(4) DEFAULT NULL,"
                           "PRIMARY KEY(idNodo),"
                           "FOREIGN KEY(idNodo) REFERENCES nodo ( id ))";

    QString centro =    "CREATE TABLE centro "
                        "(id smallint(3) NOT NULL,"
                        "nombre	varchar(32) NOT NULL,"
                        "direccion	varchar(64) DEFAULT NULL,"
                        "telefono	varchar(16) DEFAULT NULL,"
                        "PRIMARY KEY(id))";


    QString comarca = "CREATE TABLE comarca "
                      "(id smallint(2) NOT NULL,"
                      "nombre	varchar(64) NOT NULL,"
                      "nombreId	varchar(64) NOT NULL,"
                      "PRIMARY KEY(id))";


    QString diafestivopoblacion = "CREATE TABLE diafestivopoblacion "
                                    "(idPoblacion	smallint(3) NOT NULL,"
                                    "diaFestivo	date NOT NULL,"
                                    "descripcion	varchar(32) NOT NULL,"
                                    "PRIMARY KEY(idPoblacion,diaFestivo),"
                                    "FOREIGN KEY(idPoblacion) REFERENCES poblacion (id))";

    QString emailnodo = "CREATE TABLE emailnodo"
                        "(idNodo	mediumint(6) NOT NULL,"
                        "email	varchar(128) NOT NULL,"
                        "tipo   varchar (64) NOT NULL,"
                        "prioridad tinyint(1) NOT NULL,"
                        "notas varchar (64),"
                        "PRIMARY KEY(idNodo,email),"
                        "FOREIGN KEY(idNodo) REFERENCES nodo (id))";

    QString extensiontelefononodo = "CREATE TABLE extensiontelefononodo "
                                    "(telefono varchar(16) NOT NULL,"
                                    "numeroExtension tinyint(2) NOT NULL,"
                                    "nombreExtension varchar(32) NOT NULL,"
                                    "PRIMARY KEY (telefono,numeroExtension))";

    QString horarionodo = "CREATE TABLE horarionodo "
                           "(idNodo mediumint(6) NOT NULL,"
                           "diaSemana tinyint(1) NOT NULL,"
                           "horaApertura time NOT NULL,"
                           "horaCierre time NOT NULL,"
                           "PRIMARY KEY (idNodo,diaSemana,horaApertura))";






    QString mancomunidad =  "CREATE TABLE mancomunidad"
                            "(id	smallint(2) NOT NULL,"
                            "nombre	varchar(64) NOT NULL,"
                            "nombreId	varchar(64) NOT NULL,"
                            "PRIMARY KEY(id))";

    QString mancomunidadmunicipio = "CREATE TABLE mancomunidadmunicipio "
                                    "(idMancomunidad	smallint(2) NOT NULL,"
                                    "idMunicipio	smallint(3) NOT NULL,"
                                    "PRIMARY KEY(idMancomunidad,idMunicipio),"
                                    "FOREIGN KEY(idMancomunidad) REFERENCES mancomunidad (id),"
                                    "FOREIGN KEY(idMunicipio) REFERENCES municipio (id))";


    QString municipio = "CREATE TABLE municipio "
                        "(id	smallint(3) NOT NULL,"
                "idComarca	smallint(2) DEFAULT NULL,"
                "nombre	varchar(64) NOT NULL,"
                "nombreId	varchar(64) NOT NULL,"
                "codigoINE	mediumint(6) DEFAULT NULL,"
                "numeroHabitantes	mediumint(8) DEFAULT NULL,"
                "superficie	decimal(6,3) DEFAULT NULL,"
                "altitud	smallint(4) DEFAULT NULL,"
                "latitud	varchar(12) DEFAULT NULL,"
                "longitud	varchar(12) DEFAULT NULL,"
                "urlBandera	varchar(256) DEFAULT NULL,"
                "urlEscudo	varchar(256) DEFAULT NULL,"
                "web	varchar(128) DEFAULT NULL,"
                "siglasPartidoPolitico	varchar(16) DEFAULT NULL,"
                "nombrePartidoPolitico	varchar(64) DEFAULT NULL,"
                "nombreAlcalde	varchar(128) DEFAULT NULL,"
                "fechaPosesion	date DEFAULT NULL,"
                "PRIMARY KEY(id),"
                "FOREIGN KEY(idComarca) REFERENCES comarca (id))";

    QString nodo = "CREATE TABLE nodo "
                "(id	mediumint(6) NOT NULL,"
                "idPoblacion	smallint(3) NOT NULL,"
                "idCentro	smallint(3) DEFAULT NULL,"
                "nombre	varchar(128) NOT NULL,"
                "CIF	char(9) DEFAULT NULL,"
                "codigoDIR3	char(9) DEFAULT NULL,"
                "tipoVia	varchar(16) DEFAULT NULL,"
                "nombreVia	varchar(64) DEFAULT NULL,"
                "numeroDireccion	varchar(3) DEFAULT NULL,"
                "letraDireccion	char(1) DEFAULT NULL,"
                "escaleraDireccion	varchar(3) DEFAULT NULL,"
                "pisoDireccion	varchar(3) DEFAULT NULL,"
                "puertaDireccion	varchar(3) DEFAULT NULL,"
                "codigoPostal	char(5) DEFAULT NULL,"
                "latitud	varchar(12) DEFAULT NULL,"
                "longitud	varchar(12) DEFAULT NULL,"
                "contacto	varchar(128) DEFAULT NULL,"
                "extension	varchar(5) DEFAULT NULL,"
                "fax	varchar(16) DEFAULT NULL,"
                "web	varchar(128) DEFAULT NULL,"
                "sede	varchar(128) DEFAULT NULL,"
                "tablon	varchar(128) DEFAULT NULL,"
                "portalTransparencia	varchar(128) DEFAULT NULL,"
                "adslLinea	varchar(16) DEFAULT NULL,"
                "numAdministrativoLinea	varchar(16) DEFAULT NULL,"
                "ipLinea	varchar(16) DEFAULT NULL,"
                "ipCifradoLinea	varchar(16) DEFAULT NULL,"
                "servicioLinea	varchar(64) DEFAULT NULL,"
                "caudalLinea	varchar(64) DEFAULT NULL,"
                "equipamientoLinea	varchar(128) DEFAULT NULL,"
                "numeroSerieRouter	varchar(16) DEFAULT NULL,"
                "dns1 varchar(16) DEFAULT NULL,"
                "dns2 varchar(16) DEFAULT NULL,"
                "proxy smallint(1) DEFAULT 0,"
                "esAyuntamiento	smallint(1) DEFAULT 0,"
                "PRIMARY KEY(id),"
                "FOREIGN KEY(idPoblacion) REFERENCES poblacion (id),"
                "FOREIGN KEY(idCentro) REFERENCES centro (id))";


    QString poblacion = "CREATE TABLE poblacion "
                "(id	smallint(3) NOT NULL,"
                "idMunicipio	smallint(3) NOT NULL,"
                "idEla	smallint(3) DEFAULT NULL,"
                "nombre	varchar(64) NOT NULL,"
                "nombreId	varchar(64) NOT NULL,"
                "PRIMARY KEY(id),"
                "FOREIGN KEY(idMunicipio) REFERENCES municipio (id),"
                "FOREIGN KEY(idEla) REFERENCES municipio (id))";


    QString programa = "CREATE TABLE programa"
                "(idNodo	mediumint(6) NOT NULL,"
                "anio	varchar(5) NOT NULL DEFAULT 'xx/yy',"
                "portalWeb	tinyint(1) NOT NULL DEFAULT '0',"
                "email	tinyint(1) NOT NULL DEFAULT '0',"
                "baseDatosJuridica	tinyint(1) NOT NULL DEFAULT '0',"
                "suscripcionDominio	tinyint(1) NOT NULL DEFAULT '0',"
                "perfilContratante	tinyint(1) NOT NULL DEFAULT '0',"
                "gestionMunicipal	tinyint(1) NOT NULL DEFAULT '0',"
                "gestionEconomica	tinyint(1) NOT NULL DEFAULT '0',"
                "soporte	tinyint(1) NOT NULL DEFAULT '0',"
                "sedeElectronica	tinyint(1) NOT NULL DEFAULT '0',"
                "epol	tinyint(1) NOT NULL DEFAULT '0',"
                "epolMovil	tinyint(1) NOT NULL DEFAULT '0',"
                "siapol	tinyint(1) NOT NULL DEFAULT '0',"
                "PRIMARY KEY(idNodo,anio),"
                "FOREIGN KEY(idNodo) REFERENCES nodo (id))";

    QString telefononodo = "CREATE TABLE telefononodo "
                        "(idNodo	mediumint(6) NOT NULL,"
                        "telefono	varchar(16) NOT NULL,"
                        "PRIMARY KEY(idNodo,telefono),"
                        "FOREIGN KEY(idNodo) REFERENCES nodo (id))";

    QString ldap = "CREATE TABLE ldap (idLdap int primary key,"
                   "usuario varchar(100),"
                   "nombre varchar (100),"
                   "cuenta_caduca varchar(50),"
                   "cambio_clave varchar(50),"
                   "clave_caducada varchar(50),"
                   "correo varchar(100),"
                   "creada varchar(50),"
                   "estado varchar(50),"
                   "fecha_correo varchar(50),"
                   "intentos int,"
                   "logon int,"
                   "modificacion_cuenta varchar(50),"
                   "telefono varchar(50),"
                   "ultimo_login varchar(50),"
                   "descripcion varchar(250),"
                   "dn varchar(250),"
                   "useraccountcontrol int,"
                   "ultimo_cambio_clave varchar(50),"//guardamos el ultimo cambio de la clave indep. del campo pwlastset
                   "user varchar(100),"
                   "user_preW2000 varchar(50),"
                   "nombre_solo varchar(50),"
                   "apellidos varchar(100),"
                   "nombre_mostrar varchar(100),"
                   "movil varchar(50),"
                   "fax varchar(50),"
                   "departamento varchar(50),"
                   "oficina varchar(50),"
                   "organizacion varchar(50),"
                   "puesto varchar(50),"
                   "provincia varchar(50),"
                   "localidad varchar(50),"
                   "direccion varchar(50),"
                   "cp varchar(50),"
                   "notas varchar(250),"
                   "ultima_sincro varchar(50))";


    QString grupos = "CREATE TABLE grupos (id_grupo int,"
           "id_usuario int,"
           "grupo varchar (100),"
           "usuario varchar(100))";

    QString sicalwin = "CREATE TABLE sicalwin (idMunicipio smallint(3) NOT NULL,"
                       "numeroServidores smallint(3) NOT NULL DEFAULT '0',"
                       "numeroClientes smallint(3) NOT NULL DEFAULT '0')";

    QString drop_tablas = "DROP TABLE IF EXISTS aplicacion,centro,comarca,"
                          "diafestivopoblacion, emailnodo, mancomunidad,"
                          "mancomunidadmunicipio, municipio, nodo, poblacion,"
                          "programa, telefononodo, grupos";

    crea_tablas << aplicacion << centro << comarca << diafestivopoblacion << emailnodo << mancomunidad << mancomunidadmunicipio << municipio << nodo << poblacion << programa << telefononodo << extensiontelefononodo << horarionodo << ldap << grupos << sicalwin ;
    if (db_sqlite.open()){
        for (int i=0;i<crea_tablas.size() ;i++ ){
            if (!query.exec(QString(crea_tablas.at(i)))){
                QMessageBox::critical(this, "DATABASE ERROR", "No hemos podido crear la tabla "+crea_tablas.at(i),QMessageBox::Ok);
                return false;
            }
         }
    }else{
        QMessageBox::critical(this, "DATABASE ERROR", "No hemos podido abrir la base de datos sqlite",QMessageBox::Ok);
    }

return true;

}

void BaseDatos::borrar_tablas(QSqlQuery* consulta){

    //qDebug()<<"base de datos aquí";

    consulta->exec("drop table if exists ldap");

    consulta->exec("drop table if exists grupos");

}

void BaseDatos::borrar_datos(QSqlQuery* consulta){

    consulta->exec("delete from ldap");
    consulta->exec("delete from grupos");

}

bool BaseDatos::insertar_usuario(QSqlQuery* consulta, int id_usuario, usuario usu){

    //insertamos los usuarios en la base de datos
    if (!consulta->exec("insert into ldap values(" + QString::number(id_usuario) + ", "
               "'"+usu.cual_es_usuario()+"', "
               "'"+usu.cual_es_nombre()+ "', "
               "'"+usu.cual_es_caduca_cuenta()+"', "
               "'"+usu.cual_es_cambio_clave()+"', "
               "'"+usu.cual_es_caduca_clave()+"', "
               "'"+usu.cual_es_correo()+"', "
               "'"+usu.cual_es_creada()+"', "
               "'"+usu.cual_es_estado()+"', "
               "'"+usu.cual_es_fecha_correo()+"', "
               ""+ QString::number(usu.cual_es_intentos()) +", "
               ""+ QString::number(usu.cual_es_logon()) +", "
               "'"+usu.cual_es_modificacion_cuenta()+"', "
               "'"+usu.cual_es_telefono()+"', "
               "'"+usu.cual_es_ultimo_login()+"', "
               "'"+usu.cual_es_descripcion()+"' ,"
               "'"+usu.cual_es_dn()+"' ,"
               "'"+ QString::number(usu.cual_es_useraccountcontrol()) +"' ,"
               "'"+usu.cual_es_ultimo_cambio_clave()+"' ,"
               "'"+usu.cual_es_user()+"' ,"
               "'"+usu.cual_es_usuario_preW2000()+"' ,"
               "'"+usu.cual_es_nombre_solo()+"' ,"
               "'"+usu.cual_es_apellidos()+"' ,"
               "'"+usu.cual_es_nombre_mostrar()+"' ,"
               "'"+usu.cual_es_movil()+"' ,"
               "'"+usu.cual_es_fax()+"' ,"
               "'"+usu.cual_es_departamento()+"' ,"
               "'"+usu.cual_es_oficina()+"' ,"
               "'"+usu.cual_es_organizacion()+"' ,"
               "'"+usu.cual_es_puesto()+"' ,"
               "'"+usu.cual_es_provincia()+"' ,"
               "'"+usu.cual_es_localidad()+"' ,"
               "'"+usu.cual_es_direccion()+"' ,"
               "'"+usu.cual_es_cp()+"' ,"
               "'"+usu.cual_es_notas()+"' ,"
               "'"+usu.cual_es_ultima_sincro()+"')"));

        return false;

    return true;
}

//devuelve los DN de los grupos del usuario
QStringList BaseDatos::consulta_grupos(QSqlQuery* consulta, int id_usuario){

    QStringList grupos;

    QString sql = "select grupo from grupos where id_usuario=" + QString::number(id_usuario);
    consulta->prepare(sql);
    if (consulta->exec()){
        while (consulta->next()) {
            grupos<<consulta->value(0).toString();
        }
    }

    return grupos;

}

void BaseDatos::insertar_grupo(QSqlQuery* consulta, int id_usuario, usuario usu, int id){

    consulta->exec( "insert into grupos values(" + QString::number(id) + ", "
           "" + QString::number(id_usuario) + ", "
           "'"+usu.cual_es_grupos()[id]+ "', "
           "'"+usu.cual_es_usuario()+"')");
}

void BaseDatos::actualiza_usuario(QSqlQuery* consulta, int id_usuario, usuario usu){


    QString str;
    str="update ldap set "
             "nombre='"+usu.cual_es_nombre()+ "', "
             "cuenta_caduca='"+usu.cual_es_caduca_cuenta()+"', "
             "cambio_clave='"+usu.cual_es_cambio_clave()+"', "
             "clave_caducada='"+usu.cual_es_caduca_clave()+"', "
             "correo='"+usu.cual_es_correo()+"', "
             "creada='"+usu.cual_es_creada()+"', "
             "estado='"+usu.cual_es_estado()+"', "
             "fecha_correo='"+usu.cual_es_fecha_correo()+"', "
             "intentos="+ QString::number(usu.cual_es_intentos()) +", "
             "logon="+ QString::number(usu.cual_es_logon()) +", "
             "modificacion_cuenta='"+usu.cual_es_modificacion_cuenta()+"', "
             "telefono='"+usu.cual_es_telefono()+"', "
             "ultimo_login='"+usu.cual_es_ultimo_login()+"', "
             "descripcion='"+usu.cual_es_descripcion()+"' ,"
             "useraccountcontrol="+ QString::number(usu.cual_es_useraccountcontrol()) +", "
               "user='"+usu.cual_es_user()+"' ,"
               "user_preW2000='"+usu.cual_es_usuario_preW2000()+"' ,"
               "nombre_solo='"+usu.cual_es_nombre_solo()+"' ,"
               "apellidos='"+usu.cual_es_apellidos()+"' ,"
               "nombre_mostrar='"+usu.cual_es_nombre_mostrar()+"' ,"
               "movil='"+usu.cual_es_movil()+"' ,"
               "fax='"+usu.cual_es_fax()+"' ,"
               "departamento='"+usu.cual_es_departamento()+"' ,"
               "oficina='"+usu.cual_es_oficina()+"' ,"
               "organizacion='"+usu.cual_es_organizacion()+"' ,"
               "puesto='"+usu.cual_es_puesto()+"' ,"
               "provincia='"+usu.cual_es_provincia()+"' ,"
               "localidad='"+usu.cual_es_localidad()+"' ,"
               "direccion='"+usu.cual_es_direccion()+"' ,"
               "cp='"+usu.cual_es_cp()+"' ,"
               "notas='"+usu.cual_es_notas()+"' ,"
               "ultima_sincro='"+usu.cual_es_ultima_sincro()+"' ,";


    //si no se ha marcado la opción "cambiar contraseña" entonces actualizamos el campo ultimo_cambio_clave
    if (!usu.cual_es_cambio_clave().contains("01-01-1601")) {
        str=str + "ultimo_cambio_clave='"+usu.cual_es_cambio_clave()+"', ";
    }

    str=str + "dn='"+usu.cual_es_dn()+"' where id = " +  QString::number(id_usuario) + "";


    consulta->exec(str);
}

void BaseDatos::elimina_grupos_usuario(QSqlQuery* consulta, int id_usuario){

    consulta->exec("delete from grupos where id_usuario=" + QString::number(id_usuario));

}

// devuelve un una clase usuario con los datos del usuario de la base de datos
// tipo_consulta=0->filtro por usuario, tipo_consulta=1->filtro por nombre
usuario BaseDatos::consulta_usuario(QSqlQuery* consulta, QString filtro_usuario, int tipo_consulta){

    usuario* usu=new usuario;

    if (tipo_consulta==0)
        consulta->prepare(QString("select * from ldap where usuario = :usuario"));
    else
        consulta->prepare(QString("select * from ldap where nombre = :usuario"));

    consulta->bindValue(":usuario", filtro_usuario);
    if (consulta->exec() and consulta->first()){

        usu->carga_id(consulta->value(0).toInt());
        usu->carga_usuario(consulta->value(1).toString());//usuario
        usu->carga_nombre(consulta->value(2).toString());//nombre
        usu->carga_caduca_cuenta(consulta->value(3).toString());
        usu->carga_cambio_clave(consulta->value(4).toString());
        usu->carga_caduca_clave(consulta->value(5).toString());
        usu->carga_correo(consulta->value(6).toString());
        usu->carga_creada(consulta->value(7).toString());
        usu->carga_estado(consulta->value(8).toString());
        usu->carga_fecha_correo(consulta->value(9).toString());
        usu->carga_intentos(consulta->value(10).toInt());
        usu->carga_logon(consulta->value(11).toInt());
        usu->carga_modificacion_cuenta(consulta->value(12).toString());
        usu->carga_telefono(consulta->value(13).toString());
        usu->carga_ultimo_login(consulta->value(14).toString());
        usu->carga_descripcion(consulta->value(15).toString());
        usu->carga_dn(consulta->value(16).toString());
        usu->carga_useraccountcontrol(consulta->value(17).toInt());
        usu->carga_ultimo_cambio_clave(consulta->value(18).toString());
        usu->carga_user(consulta->value(19).toString());
        usu->carga_user_preW2000(consulta->value(20).toString());
        usu->carga_nombre_solo(consulta->value(21).toString());
        usu->carga_apellidos(consulta->value(22).toString());
        usu->carga_nombre_mostrar(consulta->value(23).toString());
        usu->carga_movil(consulta->value(24).toString());
        usu->carga_fax(consulta->value(25).toString());
        usu->carga_departamento(consulta->value(26).toString());
        usu->carga_oficina(consulta->value(27).toString());
        usu->carga_organizacion(consulta->value(28).toString());
        usu->carga_puesto(consulta->value(29).toString());
        usu->carga_provincia(consulta->value(30).toString());
        usu->carga_localidad(consulta->value(31).toString());
        usu->carga_direccion(consulta->value(32).toString());
        usu->carga_cp(consulta->value(33).toString());
        usu->carga_notas(consulta->value(34).toString());
        usu->carga_ultima_sincro(consulta->value(35).toString());
    }

    return *usu;

}


bool BaseDatos::actualizaDB() {
Configuracion *config = new Configuracion;
    if (conectado == true){
        int pb_cont = 1;
        QString nombre_tabla;
        QString rutaDB= config->cual_es_ruta_sqlite();
        QProcess process;

        //inicia_mysql();

        db_mysql = QSqlDatabase::database("mysql");

        //Comprobamos si tenemos mysql conectada, si no es así no podemos hacer la copia.
        if (!db_mysql.isOpen()){
            QMessageBox::critical(this, "Error NO podemos conectar con el servidor mysql", "NO podemos conectar con el servidor mysql en el servidor remoto",QMessageBox::Ok);

            return false;
        }

        if (!crearDB(rutaDB)){
            QMessageBox::critical(this, "Error SQLITE", "No hemos podido crear una DB SQLITE, compruebe la configuracion",QMessageBox::Ok);
            return false;
        }

        QStringList tablas =  db_mysql.tables(); //Listado de las tablas de la DB
        QSqlQuery srcQuery(db_mysql); //DB source
        QSqlQuery destQuery(db_sqlite); //DB destino

        //Ventana de progreso
        QProgressDialog pb("", "", 1, 5000, this);
        pb.setWindowModality(Qt::WindowModal);
        pb.setCancelButton(0);
        pb.setValue(0);
        pb.show();

        for (int i=0;i<tablas.size();i++){
            int p;
            //Dialogo de espera...
            nombre_tabla = tablas.at(i);
            pb.setLabelText("Creando la tabla "+tablas.at(i));

            //Obtenemos los campos y los tipos
            QSqlRecord registro;
            registro = db_mysql.record(tablas.at(i));
            p = registro.count();
            registro.fieldName(1);
            QSqlField campo = registro.field(2);
            int g = campo.typeID();
            QVariant tipo = campo.type();
            QString nombre = campo.name();
            int tamano = campo.length();
            QVariant tipo7 =campo.value();


           QApplication::processEvents();
           // Copiamos todas las entradas
            if (!srcQuery.exec(QString("SELECT * FROM %1").arg(nombre_tabla)))
              QMessageBox::critical(this, "Select", "No hemos podido consultar "+nombre_tabla,QMessageBox::Ok);

            while (srcQuery.next()) {
                pb_cont++;
                pb.setValue(pb_cont);

                //Construimos las cabeceras de las tablas
                QSqlRecord record=srcQuery.record();
                QStringList names;
                QStringList placeholders;
                QList<QVariant > values;
                for (int j = 0; j < record.count(); ++j) {
                    names << record.fieldName(j);
                    placeholders << ":" + record.fieldName(j);
                    QVariant value=srcQuery.value(j);
                    values << value;
                }

                // Construimos una consulta
                QString queryStr;
                queryStr.append("INSERT INTO " + nombre_tabla);
                queryStr.append(" (" + names.join(", ") + ") ");
                queryStr.append(" VALUES (" + placeholders.join(", ") + ");");
                destQuery.prepare(queryStr);
                foreach(QVariant value, values)
                    destQuery.addBindValue(value);
                QSqlError error;
                if (!destQuery.exec()){
                    error = destQuery.lastError();
                    QMessageBox::critical(this, "Insert", "No hemos podido consultar "+destQuery.lastQuery()+error.text()
                                          ,QMessageBox::Ok);
                }
        }

        for (int x=pb_cont;x<5000;x++){
             pb.setValue(x);
        }
        }

        config->actualizaFechaDB();
        actualizar_usuarios();
    }

delete config;
return true;

}

bool BaseDatos::compruebaDB(){
    Configuracion *configuracion = new Configuracion;
    db_sqlite = QSqlDatabase::database("sqlite");
    QSqlQuery* consulta = new QSqlQuery(db_sqlite);
    consulta->exec("select id from nodo");
    QDate fechaUltimaActualizacion, fechaActual;
    int dias;
    fechaUltimaActualizacion=configuracion->ultimaActualizacionDB();
    fechaActual=QDate::currentDate();
    dias=fechaUltimaActualizacion.daysTo(fechaActual);
    if ((dias > 7)||(!consulta->first())){
       return true;
    }
    delete configuracion;
return false;
}

bool BaseDatos::carga_OU(){

    Configuracion *config = new Configuracion();

    if (!config->listaOU_vacio()){
         OU = config->listaOU_datos();
     }

    if (!config->lineEdit_OU_vacio()){
        OU << config->lineEdit_OU_datos();
     }

    if (OU.isEmpty())
         return false;

     delete config;
return true;
}


// Procedimiento para actualizar la base de datos y los campos del formulario
void BaseDatos::actualizar_usuarios(){
    Configuracion *config = new Configuracion;
    db_sqlite = QSqlDatabase::database("sqlite");
    QString host = config->cual_es_servidor_ldap();
    int puerto = config->cual_es_puerto_ldap();
    QString usuarioLdap = config->cual_es_usuario_ldap();
    QString dominio=config->cual_es_dominio_ldap();
    QString password=config->cual_es_clave_ldap();
    QSqlQuery* consulta = new QSqlQuery(db_sqlite);
    QString info;
    QVector <usuario> usuarios;
    clase_ldap *ldap1 = new clase_ldap(host,puerto,usuarioLdap,dominio,password,&info);

    if (!info.contains("ERROR")) {
        int pb_cont = 0;
        //Dialogo de espera...
        QCoreApplication::processEvents();
        QProgressDialog pb("Creando la base de datos de usuarios . . .", "", 0, 10, this);
        pb.setWindowModality(Qt::WindowModal);
        pb.setCancelButton(0);
        pb.setValue(pb_cont);
        pb.show();
        QApplication::processEvents();
        int id_tmp=0;
        if (carga_OU()){ //Si hay unidades organizativas realizamos las búsquedas
            int i=0;
            pb.setMaximum(OU.count());
            //recorremos todo OU
            foreach (const QString &qstr, OU) {
                pb_cont=pb_cont+1;
                pb.setLabelText("Cargando base de datos de usuarios . . .\n"+ qstr);
                pb.setValue(pb_cont);
                QApplication::processEvents();
                usuarios=ldap1->Consulta_Usuarios(qstr, &info);
                if (!info.contains("ERROR")) {
                       // recorremos todos los objetos (entry) devueltos por la consulta
                       // cada entry tiene atributos
                       foreach (const usuario &usu, usuarios)
                       {
                            //insertamos los usuarios en la base de datos

                            insertar_usuario(consulta, id_tmp, usu);

                            for (int j = 0; j < usu.cual_es_grupos().size(); ++j) {

                                //insertamos los grupos del usuario en la base de datos
                                insertar_grupo(consulta, id_tmp, usu,j);
                            }

                            id_tmp=id_tmp+1;

                   }
                }
            }
        }
    }

    delete ldap1;
    delete config;


}


void BaseDatos::on_pushButton_clicked()
{
    actualizaDB();
}

