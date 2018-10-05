#include "botonera.h"
#include "ui_botonera.h"
#include <QNetworkInterface>
#include <QDesktopServices>
#include <QSqlRecord>
#include <QDebug>
#include "soporte/soporte.h"
#include "sedes/sedes.h"
#include "soporte/equipos.h"
#include "tunel.h"
#include "soporte/nmap_xml.h"
#include "configuracion/configuracion.h"
#include "mame/mame.h"
#include "usuarios/form_usuarios.h"
#include "basedatos/basedatos.h"
#include "lib/lib.h"
#include <QDateTime>
#include <QFileInfo>
#include <QProgressDialog>
// En este struct vamos a guardar los datos de conexion ssh y DB

struct variables{
    QString keyfile1;
    QString keyfile2;
    QString username_ssh;
    QString password_ssh;
    QString username_DB;
    QString password_DB;
    QString server_ip;
    QString local_listenip;
    unsigned int remote_port;
    unsigned int local_listenport;
    QString remote_desthost;
    QString databasename;
    QString hostname_DB;
    unsigned int remote_destport;
    bool usar_ssh;
}datos;

//Constructor
Botonera::Botonera(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Botonera)
{
    ui->setupUi(this);
    cargaVariables();
    muestraBotones();
    barraEstado();

// popup en construccion
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(ctxMenu(const QPoint &)));

}

Botonera::~Botonera()
{
    delete ui;
}

void Botonera::test_slot(){ //popup
    qDebug()<<"Prueba";
}

void Botonera::ctxMenu(const QPoint &pos) { //popup
    QMenu *menu = new QMenu;
    menu->addAction(tr("Test Item"), this, SLOT(test_slot()));
    menu->exec(this->mapToGlobal(pos));
}

void Botonera::on_actionUsuarios_triggered()
{
    form_usuarios *usuarios = new form_usuarios();
    usuarios->show();
}

void Botonera::on_actionSalir_triggered()
{
    QMainWindow::close();
}

void Botonera::on_actionCronos_triggered()
{
   Configuracion *configuracion = new Configuracion();
   QDesktopServices::openUrl(QUrl(configuracion->cual_es_cronos()));
   delete configuracion;
}

void Botonera::on_actionWebmail_triggered()
{
    Configuracion *configuracion = new Configuracion();
    QDesktopServices::openUrl(QUrl(configuracion->cual_es_correo()));
    delete configuracion;
}

void Botonera::on_actionBeiro_triggered()
{
    Configuracion *configuracion = new Configuracion();
    QDesktopServices::openUrl(QUrl(configuracion->cual_es_beiro()));
    delete configuracion;
}

void Botonera::on_actionGlpi_triggered()
{
    Configuracion *configuracion = new Configuracion();
    QDesktopServices::openUrl(QUrl(configuracion->cual_es_glpi()));
    delete configuracion;
}

void Botonera::on_actionAtalaya_triggered()
{
    Configuracion *configuracion = new Configuracion();
    QDesktopServices::openUrl(QUrl(configuracion->cual_es_atalaya()));
    delete configuracion;
}

void Botonera::on_actionActiveDirectory_triggered()
{
    Configuracion *configuracion = new Configuracion;
    QProcess process;
    if (configuracion->es_rdesktop())
       process.startDetached("rdesktop", QStringList() << "-u"+configuracion->cual_es_usuarioAD() <<"-p"+configuracion->cual_es_claveAD() << "-g"+configuracion->cual_es_resolucion() << configuracion->cual_es_servidorAD());
    else
       process.startDetached("xfreerdp", QStringList() << "/v:"+configuracion->cual_es_servidorAD() << "/u:"+configuracion->cual_es_usuarioAD() << "/p:"+configuracion->cual_es_claveAD() << "/size:"+configuracion->cual_es_resolucion());
    delete configuracion;
}

void Botonera::on_actionISL_triggered()
{
    Configuracion *configuracion = new Configuracion;
    QProcess process;
    if (configuracion->usarproxy_chains())
        process.startDetached(configuracion->cual_es_proxychains(), QStringList() << configuracion->cual_es_isl());
    else
        process.startDetached(configuracion->cual_es_isl());
    delete configuracion;
}

void Botonera::on_actionConfigurar_triggered()
{
    Configuracion *configuracion= new Configuracion;
    configuracion->setFixedSize(1024,800);
    configuracion->show();
}

void Botonera::on_actionNuevo_Men_triggered()
{
    Botonera *menu = new Botonera;
    menu->show();
}

void Botonera::on_actionSedes_triggered()
{
    Sedes *sedes = new Sedes;
    sedes->show();
}

void Botonera::on_actionSoporte_triggered()
{
    Soporte *soporte = new Soporte;
    soporte->show();
}

void Botonera::basedatos(){
    db_mysql.setDatabaseName(datos.databasename);
    db_mysql.setHostName(datos.hostname_DB);
    db_mysql.setUserName(datos.username_DB);
    db_mysql.setPassword(datos.password_DB);
    db_mysql.setPort(datos.local_listenport);
    if (!db_mysql.open()){
        ui->label_mysql_DB->setText("Cerrado");
    }else{
        ui->label_mysql_DB->setText("Conectada");
    }
}

//Creamos una conexion ssh con el servidor remoto.
//Cuando conectamos llamamos a basedatos.

bool Botonera::creaConexion()
{
    Tunel *tunel = new Tunel;
    QThread *hilo= new QThread;
    tunel->keyfile1=convierte(datos.keyfile1);
    tunel->keyfile2=convierte(datos.keyfile2);
    tunel->username_ssh=convierte(datos.username_ssh);
    tunel->remote_destport=datos.remote_destport;
    tunel->local_listenport=datos.local_listenport;
    tunel->local_listenip="127.0.0.1";
    tunel->remote_port=datos.remote_port;
    tunel->remote_desthost=convierte(datos.remote_desthost);
    tunel->password_ssh=convierte(datos.password_ssh);
    tunel->server_ip=convierte(datos.server_ip);
    tunel->remote_destport=3306;//mysql remoto
    tunel->moveToThread(hilo);
    QObject::connect(hilo,&QThread::started, tunel, &Tunel::crea_conexion);
    QObject::connect(tunel,&Tunel::destroyed, hilo, &QThread::quit);
    QObject::connect(tunel,&Tunel::sshConectado, this, &Botonera::basedatos);
    QObject::connect(tunel,&Tunel::sshDesconectado,tunel,&Tunel::cierra_conexion);
    hilo->start();
    return true;
}

//Muestra los botones y ajusta el menú al número de botones visibles
void Botonera::muestraBotones(){
    int tamano = 2;
    Configuracion *configuracion = new Configuracion;

    if (configuracion->usuarios_up()){
        ui->mainToolBar->actions().at(0)->setVisible(true);
        tamano++;
    } else
        ui->mainToolBar->actions().at(0)->setVisible(false);


    if (configuracion->soporte_up()){
        ui->mainToolBar->actions().at(1)->setVisible(true);
        tamano++;
    } else
        ui->mainToolBar->actions().at(1)->setVisible(false);

    if (configuracion->sedes_up()){
        ui->mainToolBar->actions().at(2)->setVisible(true);
        tamano++;
    } else
        ui->mainToolBar->actions().at(2)->setVisible(false);

    if (configuracion->cronos_up()){
        ui->mainToolBar->actions().at(3)->setVisible(true);
        tamano++;
    } else
        ui->mainToolBar->actions().at(3)->setVisible(false);

    if (configuracion->webmail_up()){
        ui->mainToolBar->actions().at(4)->setVisible(true);
        tamano++;
    } else
        ui->mainToolBar->actions().at(4)->setVisible(false);

    if (configuracion->beiro_up()){
        ui->mainToolBar->actions().at(5)->setVisible(true);
        tamano++;
    } else
        ui->mainToolBar->actions().at(5)->setVisible(false);
    if (configuracion->glpi_up()){
        ui->mainToolBar->actions().at(6)->setVisible(true);
        tamano++;
    } else
        ui->mainToolBar->actions().at(6)->setVisible(false);
    if (configuracion->ocs_up()){
        ui->mainToolBar->actions().at(7)->setVisible(true);
        tamano++;
    } else
        ui->mainToolBar->actions().at(7)->setVisible(false);
    if (configuracion->ts_up()){
        ui->mainToolBar->actions().at(8)->setVisible(true);
        tamano++;
    } else
        ui->mainToolBar->actions().at(8)->setVisible(false);

    if (configuracion->isl_up()){
        ui->mainToolBar->actions().at(9)->setVisible(true);
        tamano++;
    } else
        ui->mainToolBar->actions().at(9)->setVisible(false);

    if (configuracion->atalaya_up()){
        ui->mainToolBar->actions().at(10)->setVisible(true);
        tamano++;
    } else
        ui->mainToolBar->actions().at(10)->setVisible(false);

    ui->mainToolBar->show();
    this->setFixedSize(10+84*tamano,135);

}

bool Botonera::crearDB(QString rutaDB){

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

    db_sqlite = QSqlDatabase::database("sqlite");
    if (!db_sqlite.open()){
            ui->label_sqlite_DB->setText("Cerrado");
            return false;
        }else{
            ui->label_sqlite_DB->setText("Conectada");
    }

    QSqlQuery query(db_sqlite);
    QList<QString> crea_tablas;

    QString aplicacion = "CREATE TABLE aplicacion "
                           "(idNodo	mediumint(6) NOT NULL,"
                           "atalaya	smallint(4) DEFAULT NULL,"
                           "glpi smallint(4) DEFAULT NULL,"
                           "ocs	smallint(4) DEFAULT NULL,"
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

    QString ldap = "CREATE TABLE ldap (id int primary key,"
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
           "dn varchar(250))";

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
                          "programa, telefononodo, ldap, grupos";

    crea_tablas << aplicacion << centro << comarca << diafestivopoblacion << emailnodo << mancomunidad << mancomunidadmunicipio << municipio << nodo << poblacion << programa << telefononodo << extensiontelefononodo << horarionodo << ldap << grupos << sicalwin ;

    for (int i=0;i<crea_tablas.size() ;i++ ){
        if (!query.exec(QString(crea_tablas.at(i)))){
            QMessageBox::critical(this, "DATABASE ERROR", "No hemos podido crear la tabla "+crea_tablas.at(i),QMessageBox::Ok);
            return false;
        }
     }

return true;

}

bool Botonera::actualizaDB(QString rutaDB) {
        int pb_cont = 1;
        QString nombre_tabla;
        Configuracion *configuracion = new Configuracion;
        QProcess process;
        NMap* nmap = new NMap();
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
            //Dialogo de espera...
            nombre_tabla = tablas.at(i);
            pb.setLabelText("Creando la tabla "+tablas.at(i));
           //QApplication::processEvents();
           // Copiamos todas las entradas
            if (!srcQuery.exec(QString("SELECT * FROM %1").arg(nombre_tabla)))
              QMessageBox::critical(this, "Select", "No hemos podido consultar "+nombre_tabla,QMessageBox::Ok);

            while (srcQuery.next()) {
                pb_cont++;
                pb.setValue(pb_cont);

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
    delete configuracion;
    return true;

}

bool Botonera::cargaVariables(){
    //Creamos los conectores de las base de datos
    db_mysql = QSqlDatabase::addDatabase("QMYSQL","mysql");
    db_sqlite = QSqlDatabase::addDatabase("QSQLITE","sqlite");

    Configuracion *configuracion = new Configuracion;
    home = configuracion->cual_es_home();
    grxconf_ini = home + ".grx/.grxconf.ini";

    if (!dirExists(home+".grx"))    //Si no existe el directorio .grx lo creamos
       QDir().mkdir(home+".grx");

    if (!fileExists(grxconf_ini)){
        QMessageBox::critical(this, "Configurar", "Es la primera vez que ejecuta GrxMenu\no se ha borrado el archivo de configuración\nSe han puesto los datos por defecto, revíselos\nDebe configurar la aplicación y guardar los cambios",QMessageBox::Ok);
        on_actionConfigurar_triggered();
        return false;
    }

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
                   ui->label_mysql_DB->setText("Mysql sin conexion");
                   return false;
               }
               else  {
                   ui->label_mysql_DB->setText("Mysql conectado");
                   }
               }
   }
       else{
       ui->statusBar->messageChanged("Puerto Cerrado");

   }
   }

    //Configuramos sqlite y la abrimos
    QString rutaDB_sqlite = configuracion->cual_es_ruta_sqlite();
    db_sqlite.setDatabaseName(rutaDB_sqlite);
    if (!db_sqlite.open()){
        QMessageBox::critical(this, "Error SQLite", "No hemos podido abrir sqlite, compruebe la configuracion ",QMessageBox::Ok);
                ui->label_sqlite_DB->setText("Cerrado SQLITE");
                ui->actionSedes->setDisabled(true);
                ui->actionSoporte->setDisabled(true);
                return false;
            }
            else
                ui->label_sqlite_DB->setText("Conectado SQLITE");


    //Muestra la ip

    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
            ui->label_ip->setText(address.toString());
    }

delete configuracion;
return true;

}

void Botonera::barraEstado(){
    Configuracion *configuracion = new Configuracion;
    QLabel *bienvenido = new QLabel("Bienvenido ");
    QLabel *nombre = new QLabel(configuracion->cual_es_usuario_logado());
    QLabel *KB = new QLabel("Ticket Kerberos");
    QLabel *DB = new QLabel("Base de Datos");
    QLabel *ipdir = new QLabel("Direccion IP");
    bienvenido->font().setBold(false);
    nombre->font().setBold(true);
    KB->font().setBold(false);
    DB->font().setBold(false);

    ui->statusBar->addWidget(bienvenido);
    ui->statusBar->addWidget(nombre);
    ui->statusBar->addWidget(KB);
    ui->statusBar->addWidget(ui->kerberos);
    ui->statusBar->addWidget(ui->pb_kerberos);
    ui->statusBar->addWidget(DB);
    ui->statusBar->addWidget(ui->label_sqlite_DB);
    ui->statusBar->addWidget(ui->label_mysql_DB);
    ui->statusBar->addWidget(ui->pb_reconectaDB);
    ui->statusBar->addWidget(ipdir);
    ui->statusBar->addWidget(ui->label_ip);
    delete configuracion;
}

void Botonera::on_actionAcerca_de_triggered()
{
    AcerdaDe *acerca = new AcerdaDe;
    acerca->show();
}

void Botonera::on_actionMame_triggered()
{
    Mame *juego = new Mame;
    juego->show();
}

void Botonera::on_pb_reconectaDB_clicked()
{
   Configuracion *configuracion = new Configuracion;
   actualizaDB(configuracion->cual_es_ruta_sqlite());
   delete configuracion;
}
