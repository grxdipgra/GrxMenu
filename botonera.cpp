#include "botonera.h"
#include "ui_botonera.h"
#include "usuario.h"
#include "soporte.h"
#include "sedes.h"
#include "configuracion.h"
#include "equipos.h"
#include "tunel.h"
#include <QNetworkInterface>
#include "qdebug.h"
#include "nmap_xml.h"
#include <QDesktopServices>
#include "configuracion.h"
#include <QFileInfo>


bool fileExists(QString path) {
    QFileInfo check_file(path);
    // comprueba que exista y no sea un directorio
    if (check_file.exists() && check_file.isFile()) {
        return true;
    } else {
        return false;
    }
}

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

Botonera::Botonera(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Botonera)
{
    QSqlDatabase db = QSqlDatabase::database();
    ui->setupUi(this);
    muestraBotones();
    cargaVariables();
    barraEstado();
}

Botonera::~Botonera()
{
    delete ui;
}

void Botonera::on_actionUsuarios_triggered()
{
  //  form_usuarios *usuarios = new form_usuarios();
  //  usuarios->show();
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
    if (configuracion->es_usarproxy_chains())
        process.startDetached(configuracion->cual_es_proxychains(), QStringList() << configuracion->cual_es_isl());
    else
        process.startDetached(configuracion->cual_es_isl());
    delete configuracion;
}

void Botonera::on_actionConfigurar_triggered()
{
    Configuracion *configuracion= new Configuracion;
    configuracion->setFixedSize(690,690);
    configuracion->show();
}

void Botonera::on_actionNuevo_Men_triggered()
{
    Botonera *menu = new Botonera;
    menu->setFixedSize(1100,135);
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

unsigned int puerto_libre(){
    QTcpServer server;
    if(server.listen(QHostAddress::Any, 0))
           return server.serverPort();
return -1;
}

bool Botonera::basedatos(){
    db.setDatabaseName(datos.databasename);
    db.setHostName("127.0.0.1");
    db.setUserName(datos.username_DB);
    db.setPassword(datos.password_DB);
    db.setPort(datos.local_listenport);

    if (!db.open()){
        ui->label_DB->setText("Cerrado");
        ui->actionSedes->setDisabled(true);
        ui->actionSoporte->setDisabled(true);
        return false;
    }else
        ui->label_DB->setText("Conectada");
        ui->actionSedes->setEnabled(true);
        ui->actionSoporte->setEnabled(true);

    return true;
}

char* Botonera::convierte(QString dato){
    char* cstr;
    std::string fname = dato.toStdString();
    cstr = new char [fname.size()+1];
    strcpy( cstr, fname.c_str() );
    return cstr;
}

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

bool Botonera::muestraBotones(){

    Configuracion *configuracion = new Configuracion;


    if (!configuracion->usuarios_up())
               ui->mainToolBar->actions().at(0)->setVisible(false);
    else
               ui->mainToolBar->actions().at(0)->setVisible(true);

    if (!configuracion->soporte_up())
               ui->mainToolBar->actions().at(1)->setVisible(false);
    else
               ui->mainToolBar->actions().at(1)->setVisible(true);


    if (!configuracion->sedes_up())
               ui->mainToolBar->actions().at(2)->setVisible(false);
    else
               ui->mainToolBar->actions().at(2)->setVisible(true);

    if (!configuracion->cronos_up())
               ui->mainToolBar->actions().at(3)->setVisible(false);
    else
               ui->mainToolBar->actions().at(3)->setVisible(true);

    if (!configuracion->webmail_up())
               ui->mainToolBar->actions().at(4)->setVisible(false);
    else
               ui->mainToolBar->actions().at(4)->setVisible(true);

    if (!configuracion->beiro_up())
               ui->mainToolBar->actions().at(5)->setVisible(false);
    else
               ui->mainToolBar->actions().at(5)->setVisible(true);

    if (!configuracion->glpi_up())
               ui->mainToolBar->actions().at(6)->setVisible(false);
    else
               ui->mainToolBar->actions().at(6)->setVisible(true);

    if (!configuracion->ocs_up())
               ui->mainToolBar->actions().at(7)->setVisible(false);
    else
               ui->mainToolBar->actions().at(7)->setVisible(true);

    if (!configuracion->ts_up())
               ui->mainToolBar->actions().at(8)->setVisible(false);
    else
               ui->mainToolBar->actions().at(8)->setVisible(true);


    if (!configuracion->isl_up())
               ui->mainToolBar->actions().at(9)->setVisible(false);
    else
               ui->mainToolBar->actions().at(9)->setVisible(true);

     if (!configuracion->atalaya_up())
               ui->mainToolBar->actions().at(10)->setVisible(false);
     else
               ui->mainToolBar->actions().at(10)->setVisible(true);



        //QToolBar::actions().at(3)->setVisible(false);
    ui->mainToolBar->repaint();
    ui->mainToolBar->show();
return true;
}

bool Botonera::cargaVariables(){

    db = QSqlDatabase::addDatabase("QMYSQL");
    Configuracion *configuracion = new Configuracion;
    home = qgetenv("HOME");
    GrxMenu = home + "/.grxconf.ini";
    path =qgetenv("PATH");
    if (!fileExists(GrxMenu)){
        QMessageBox::critical(this, "Configurar", "Es la primera vez que ejecuta GrxMenu\no se ha borrado el archivo de configuración\nDebe configurar la aplicación y guardar los cambios",QMessageBox::Ok);
        on_actionConfigurar_triggered();
        return false;
    }
    datos.keyfile1=configuracion->cual_es_keyfile_publica();
    datos.keyfile2=configuracion->cual_es_keyfile_privada();
    datos.username_ssh=configuracion->cual_es_usuarioSSH();
    datos.password_ssh=configuracion->cual_es_password_ssh();
    datos.username_DB=configuracion->cual_es_usernameDB();
    datos.password_DB=configuracion->cual_es_passwordDB();
    datos.hostname_DB=configuracion->cual_es_hostnameDB();
    datos.local_listenip="127.0.0.1";

     if (configuracion->es_usarSSH()){
         datos.remote_port=configuracion->cual_es_puerto_remoto_ssh();
         datos.server_ip=configuracion->cual_es_servidorSSH().toLatin1().data();
     }
     else{
         datos.remote_port=configuracion->cual_es_PuertoDB().toInt();
         datos.server_ip="127.0.0.1";
     }
    datos.remote_desthost="127.0.0.1";
    datos.databasename=configuracion->cual_es_DataBaseName();

    NMap* nmap = new NMap();
    if (datos.remote_port!=0){
        nmap->nmap_run_scan(QString::number(datos.remote_port),datos.server_ip);
        if (nmap->nmap_is_open_port(datos.server_ip, QString::number(datos.remote_port))){
        if (configuracion->es_usarSSH()){ //Tenemos seleccionado usar tunel ssh
                datos.local_listenport=puerto_libre();
                datos.usar_ssh=true;
                db.setPort(datos.local_listenport);
                creaConexion();
        }
        else{
                datos.local_listenport=configuracion->cual_es_PuertoDB().toInt();
                datos.usar_ssh=false;
                db.setDatabaseName(datos.databasename);
                db.setHostName("127.0.0.1");
                db.setUserName(datos.username_DB);
                db.setPassword(datos.password_DB);
                db.setPort(datos.local_listenport);
                if (!db.open()){
                    ui->label_DB->setText("Cerrado");
                    ui->actionSedes->setDisabled(true);
                    ui->actionSoporte->setDisabled(true);
                    return false;
                }
                else  {
                    ui->label_DB->setText("Conectado");
                    ui->actionSedes->setEnabled(true);
                    ui->actionSoporte->setEnabled(true);
                    }
                }
    }
        else{
        ui->statusBar->messageChanged("Puerto Cerrado");
        ui->actionSedes->setDisabled(true);
        ui->actionSoporte->setDisabled(true);
    }
    }

    //Muestra la ip
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
            ui->label_ip->setText(address.toString());
    }
    //Muestra el usuario del sistema con el que nos hemos logado
    QString name = qgetenv("USER");
    if (name.isEmpty())
        name = qgetenv("USERNAME");
    ui->label_username->setText(name);

delete configuracion;
delete nmap;
return true;
}

bool Botonera::barraEstado(){
    QString name = qgetenv("USER");
    if (name.isEmpty())
        name = qgetenv("USERNAME");
    QLabel *bienvenido = new QLabel("Bienvenido ");
    QLabel *nombre = new QLabel(name);
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
    ui->statusBar->addWidget(ui->label_DB);
    ui->statusBar->addWidget(ui->pb_reconectaDB);
    ui->statusBar->addWidget(ui->label_ip);
}



void Botonera::on_actionAcerca_de_triggered()
{
    AcerdaDe *acerca = new AcerdaDe;
    acerca->show();

}

void Botonera::on_actionBorrame_triggered()
{
    Soporte *soporte = new Soporte;
    soporte->show();
}
