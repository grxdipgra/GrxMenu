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
#include <QSqlField>
#include <QProgressDialog>
#include "QSqlField"
#include <QString>
#include <QAction>
#include <QDir>
#include <QToolButton>
#include "mailing/mailing.h"

//Constructor
Botonera::Botonera(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Botonera)
{
    ui->setupUi(this);
    cargaVariables();
    cargaDominio();
    muestraBotones();
    compruebaDB();

//systray
    crearAcciones();
    crearTrayIcon();
// popup en construccion
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(ctxMenu(const QPoint &)));



//Botones en cascada-----------------------
    //QToolButton *qt=new QToolButton;
    QToolButton *terminalButton=
        dynamic_cast<QToolButton*>(ui->mainToolBar->widgetForAction(ui->actionActiveDirectory));
        terminalButton->setPopupMode(QToolButton::InstantPopup);
        terminalButton->addAction(ui->actionViejo_Citrix);
        terminalButton->addAction(ui->actionNuevo_Citrix);

    QToolButton *linkButton=
        dynamic_cast<QToolButton*>(ui->mainToolBar->widgetForAction(ui->actionLink));
        linkButton->setPopupMode(QToolButton::InstantPopup);
        linkButton->addAction(ui->actionBeiro);
        linkButton->addAction(ui->actionCronos);
        linkButton->addAction(ui->actionGlpi);
        linkButton->addAction(ui->actionWebmail);
        linkButton->addAction(ui->actionOcs);
        linkButton->addAction(ui->actionAtalaya);
    QToolButton *appButton=
        dynamic_cast<QToolButton*>(ui->mainToolBar->widgetForAction(ui->actionApp));
        appButton->setPopupMode(QToolButton::InstantPopup);
        appButton->addAction(ui->actionISL);
        appButton->addAction(ui->actionactionVeleta );
        appButton->addAction(ui->actionVirtManager);
        appButton->addAction(ui->actionMailing);


//--------------




//Ejecutar una sola instancia de la aplicación
    _singular = new QSharedMemory("Aplicacion_unica", this);
}

Botonera::~Botonera()
{
    delete ui;
    unlock();// desbloquea que sólo haya una instancia
}

void Botonera::unlock(){
    if(_singular->isAttached())
            _singular->detach();
}


bool Botonera::lock()
{
    Configuracion *configuracion = new Configuracion;
    if (configuracion->multiples_instancias()){
        if(_singular->attach(QSharedMemory::ReadOnly)){
            _singular->detach();
            return false;
        }

        if(_singular->create(1))
            return true;
    }
delete configuracion;
return true;

}

//Comprueba DB
void Botonera::compruebaDB(){
    BaseDatos *DB = new BaseDatos(INCOMPLETO);
    if (DB->compruebaDB()){
        ui->statusBar->showMessage("Atencion, la base de datos necesita actualizarse. Actualícela en 'Configurar'");
    }
    delete DB;
}

//---Metodos para crear un systray

void Botonera::crearAcciones()
{
    QIcon iconSoporte(":/imagenes/iconos/botonera/asistencia.png");
    soporteAcciones = new QAction(iconSoporte,tr("S&oporte"), this);
    connect(soporteAcciones, &QAction::triggered, this, &Botonera::on_actionSoporte_triggered);

    QIcon iconUsuarios(":/imagenes/iconos/botonera/usuarios.png");
    usuariosAcciones = new QAction(iconUsuarios,tr("&Usuarios"), this);
    connect(usuariosAcciones, &QAction::triggered, this,&Botonera::on_actionUsuarios_triggered);

    QIcon iconSedes(":/imagenes/iconos/botonera/sedes.png");
    sedesAcciones = new QAction(iconSedes,tr("S&edes"), this);
    connect(sedesAcciones, &QAction::triggered, this, &Botonera::on_actionSedes_triggered);

    QIcon iconSalir(":/imagenes/iconos/botonera/salida.png");
    salirAccion = new QAction(iconSalir,tr("&Salir"), this);
    connect(salirAccion, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void Botonera::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {
        trayIcon->showMessage("GrxMenu", "El programa seguirá corriendo en segundo plano",QSystemTrayIcon::Information,5000);
        hide();
        event->ignore();
    }
}

void Botonera::crearTrayIcon()
{
    QIcon icon(":/imagenes/iconos/botonera/logo.png");
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(soporteAcciones);
    trayIconMenu->addAction(usuariosAcciones);
    trayIconMenu->addAction(sedesAcciones);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(salirAccion);
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setToolTip("GrxMenu");
    trayIcon->setIcon(icon);
    connect(trayIcon, &QSystemTrayIcon::activated, this, &Botonera::iconActivated);
    trayIcon->show();
}


void Botonera::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        if (this->isHidden()){
            this->showMaximized();
        }
        else{
            this->hide();
        }
        break;
    case QSystemTrayIcon::MiddleClick:
        showMessage();

        break;

    default:
        ;
    }
}

void Botonera::showMessage()
{
    QIcon icon("./imagenes/iconos/botonera/icono.png");
    trayIcon->showMessage("GrxMenu", "Esto es una prueba", icon,
                           10000);
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
    qApp->quit();

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
    if (QDir("/opt/ISLOnline").exists()){
        if (configuracion->usarproxy_chains())
            process.startDetached(configuracion->cual_es_proxychains(), QStringList() << configuracion->cual_es_isl());
        else
            process.startDetached(configuracion->cual_es_isl());

    }
    else
    {
           process.startDetached("/opt/ISL_Light_4.4");
    }
    delete configuracion;
}

void Botonera::on_actionConfigurar_triggered()
{
    Configuracion *configuracion= new Configuracion;
    configuracion->setFixedSize(862,715);
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



//Muestra los botones y ajusta el menú al número de botones visibles
void Botonera::muestraBotones(){

/*    int tamano = 2;
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


    if (configuracion->veleta_up()){
        ui->mainToolBar->actions().at(9)->setVisible(true);
        tamano++;
    } else
        ui->mainToolBar->actions().at(9)->setVisible(false);

    if (configuracion->isl_up()){
        ui->mainToolBar->actions().at(10)->setVisible(true);
        tamano++;
    } else
        ui->mainToolBar->actions().at(10)->setVisible(false);

    if (configuracion->atalaya_up()){
        ui->mainToolBar->actions().at(11)->setVisible(true);
        tamano++;
    } else
        ui->mainToolBar->actions().at(11)->setVisible(false);
*/
    ui->mainToolBar->show();
    this->setFixedSize(590,135);
}

bool Botonera::cargaDominio(){
    Configuracion *configuracion = new Configuracion;
    configuracion->actualiza_Kerberos();
    delete configuracion;
}

bool Botonera::cargaVariables(){
    Configuracion *configuracion = new Configuracion;
   // BaseDatos db;
   // db.inicia_sqlite();
  //Creamos los conectores de las bases de datos
    db_sqlite = QSqlDatabase::addDatabase("QSQLITE","sqlite");
    db_mysql = QSqlDatabase::addDatabase("QMYSQL","mysql");
    //Configuramos sqlite y la abrimos
    QString rutaDB_sqlite = configuracion->cual_es_ruta_sqlite();
    db_sqlite.setDatabaseName(rutaDB_sqlite);
    if (!db_sqlite.open()){
        QMessageBox::critical(this, "Error SQLite", "No hemos podido abrir sqlite, compruebe la configuracion ",QMessageBox::Ok);
                //ui->label_sqlite_DB->setText("Cerrado SQLITE");
                ui->actionSedes->setDisabled(true);
                ui->actionSoporte->setDisabled(true);
                return false;
     }
     else{
         //ui->label_sqlite_DB->setText("Conectado SQLITE");
     }
    home = configuracion->cual_es_home();
    grxconf_ini = home + ".grx/.grxconf.ini";

    if (!dirExists(home+".grx"))    //Si no existe el directorio .grx lo creamos
       QDir().mkdir(home+".grx");

    if (!fileExists(grxconf_ini)){
        QMessageBox::critical(this, "Configurar", "Es la primera vez que ejecuta GrxMenu\no se ha borrado el archivo de configuración\nSe han puesto los datos por defecto, revíselos\nDebe configurar la aplicación y guardar los cambios",QMessageBox::Ok);
        on_actionConfigurar_triggered();
        return false;
    }


    //Muestra la ip

    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
           ui->statusBar->showMessage(address.toString());

    }

delete configuracion;
return true;

}


void Botonera::on_actionAcerca_de_triggered()
{
    AcercaDe *acerca = new AcercaDe;
    acerca->show();
}

void Botonera::on_actionMame_triggered()
{
    Mame *juego = new Mame;
    juego->show();
}

void Botonera::on_pb_kerberos_clicked()
{
    cargaDominio();
}


void Botonera::on_actionactionVeleta_triggered()
{
    QDesktopServices::openUrl(QUrl("smb://veleta.grx/usuarios"));
}

void Botonera::on_actionBorrame_triggered()
{
   QMainWindow::close();
}

void Botonera::on_actionNuevo_Citrix_triggered()
{
    QDesktopServices::openUrl(QUrl("rdp://municipal@10.1.100.71"));
}

void Botonera::on_actionViejo_Citrix_triggered()
{
    QDesktopServices::openUrl(QUrl("rdp://municipal@10.1.1.155"));
}

void Botonera::on_actionOcs_triggered()
{
    QDesktopServices::openUrl(QUrl("https://incidencias.dipgra.es/ocsreports/"));
}

void Botonera::on_actionLink_triggered()
{
    on_actionOcs_triggered();
    on_actionGlpi_triggered();
    on_actionCronos_triggered();
    on_actionWebmail_triggered();
    on_actionBeiro_triggered();
    on_actionAtalaya_triggered();

}

void Botonera::on_actionApp_triggered()
{
    on_actionISL_triggered();
    on_actionactionVeleta_triggered();
}

void Botonera::on_actionVirtManager_triggered()
{
    Configuracion *configuracion = new Configuracion;
    QProcess process;
       process.startDetached("virt-manager -c qemu+ssh://municipios@10.7.15.40/system");
    delete configuracion;
}

void Botonera::on_actionMailing_triggered()
{
Mailing *mailing = new Mailing;
mailing->show();
}
