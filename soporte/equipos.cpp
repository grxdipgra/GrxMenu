#include "equipos.h"
#include "ui_equipos.h"
#include <QTemporaryDir>
#include "qdebug.h"
#include <QProcess>
#include <QTcpServer>
#include <QDesktopServices>
#include "configuracion/configuracion.h"
#include <QEventLoop>
#include "soporte/usuarios.h"

QString host_ports_open_string(Host *host){
    QString lista;
    for (int i=0;i<host->ports.port.count();i++)
            if (host->ports.port.at(i).state.state=="open")
                lista.append(host->ports.port.at(i).portid+" ");
    return lista;
}

QString host_ports_find(Host *host){
    QString lista;
    for (int i=0;i<host->ports.port.count();i++)
            lista.append(host->ports.port.at(i).portid+" ");
    return lista;
}

Equipos::Equipos(Host *host, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Equipos)
{
    ui->setupUi(this);

    ui->tabWidget->setTabsClosable(true);
    ui->consola->setStyleSheet("color: white; background-color: black;");
    host_tmp=host;//Guardamos para toda la clase el valor de host en variable global
    mac=macEquipo(host);
    ip=ipEquipo(host);
    version();
    QString puertos_abiertos = host_ports_open_string(host);
    botonesActivos(puertos_abiertos);
    ui->lineEdit_ip->setText(ip);
    ui->lineEdit_mac->setText(mac);
    ui->lineEdit_puertos->setText(puertos_abiertos);
    ui->lineEdit_puertosBuscados->setText(QString::number(host->ports.port.count())+": "+host_ports_find(host));
    ui->lineEdit_hostname->setText(host->hostnames.hostname.name);
    ui->lineEdit_status->setText(host->status.state);
    ui->lineEdit_8->setText(Version);
}

Equipos::~Equipos()
{
    delete ui;
}


/****************ipEquipo****************************
 * Devuelve la ip del equipo pasado por parametro
 * *******************************************************/

QString Equipos::ipEquipo(Host *host)
{
    int num_equipos=host->address.count();
    for (int i=0;i<num_equipos;i++)
        if (host->address[i].addrtype=="ipv4")
            return host->address[i].addr;
return "";
}



/****************macEquipo****************************
 * Devuelve los la mac del equipo pasado por parametro
 * *******************************************************/

QString Equipos::macEquipo(Host *host)
{
    int num_equipos=host->address.count();
    for (int i=0;i<num_equipos;i++)
        if (host->address[i].addrtype=="mac")
            return host->address[i].addr;
return "";
}

/****************host_ports_open****************************
 * Devuelve los puertos buscados del host pasado por parametro
 * *******************************************************/


QList <QString> Equipos::host_ports_open(Host *host){
    QList <QString> lista;
    for (int i=0;i<host->ports.port.count();i++)
           lista.append(host->ports.port.at(i).portid);
    return lista;
}

void Equipos::desactivaBotones(){


    ui->pB_usb->setEnabled(false);
    ui->pB_proxy->setEnabled(false);
    ui->pB_devices->setEnabled(false);
    ui->pB_memoria->setEnabled(false);
    ui->pB_clock->setEnabled(false);
    ui->pB_impresora->setEnabled(false);
    ui->pB_pci->setEnabled(false);



    ui->pB_carpeta->setEnabled(false);
    ui->pB_CUPS->setEnabled(false);
    ui->pB_discos->setEnabled(false);
    ui->pB_equipo->setEnabled(false);
    ui->pB_instala->setEnabled(false);
    ui->pB_networkManager->setEnabled(false);
    ui->pB_ssh->setEnabled(false);
    ui->pB_systemconfig->setEnabled(false);
    ui->pB_telnet->setEnabled(false);
    ui->pB_web->setEnabled(false);
    ui->pB_webssl->setEnabled(false);
    ui->pB_konekta->setEnabled(false);
    ui->pB_vnc->setEnabled(false);
    ui->pB_update->setEnabled(false);
    ui->pB_usuarios->setEnabled(false);
    ui->pB_procesos->setEnabled(false);
}


void Equipos::botonesActivos(QString puertos){
    QStringList lista = puertos.split(" ");
    desactivaBotones();
    for (int i=0;i<lista.length();i++){
        if (Version!="Desconocida")
         {

             ui->pB_update->setEnabled(true);
             ui->pB_usb->setEnabled(true);
             ui->pB_proxy->setEnabled(true);
             ui->pB_devices->setEnabled(true);
             ui->pB_memoria->setEnabled(true);
             ui->pB_clock->setEnabled(true);
             ui->pB_impresora->setEnabled(true);
             ui->pB_pci->setEnabled(true);

             ui->pB_ssh->setEnabled(true);
             ui->pB_carpeta->setEnabled(true);
             ui->pB_CUPS->setEnabled(true);
             ui->pB_networkManager->setEnabled(true);
             ui->pB_vnc->setEnabled(true);
             ui->pB_systemconfig->setEnabled(true);
             ui->pB_instala->setEnabled(true);
             ui->pB_equipo->setEnabled(true);
             ui->pB_discos->setEnabled(true);
             ui->pB_procesos->setEnabled(true);
             ui->pB_usuarios->setEnabled(true);
             ui->pB_konekta->setEnabled(true);

        }
        if (lista[i]=="80")
             ui->pB_web->setEnabled(true);
        if (lista[i]=="443")
             ui->pB_webssl->setEnabled(true);
        if (lista[i]=="23")
            ui->pB_telnet->setEnabled(true);
     }

}



/****************host_ports_open_int****************************
 * Devuelve los puerto abiertos del host pasado por parametro
 * *******************************************************/

int Equipos::host_ports_open_int(Host *host){
    return host->ports.port.count();
}

int Equipos::puerto_libre(){
    QTcpServer server;
    if(server.listen(QHostAddress::Any, 0))
           return server.serverPort();

 return 0;
}

void Equipos::on_pB_carpeta_clicked()
{

    QTemporaryDir dir;
    QProcess process;
    Configuracion config;
    dir.setAutoRemove(false);
    if (dir.isValid()) {
       process.startDetached("sshfs -p "+config.cual_es_puerto()+" -o reconnect -C -o workaround=all "+config.cual_es_usuario_remoto()+"@"+ip+":/home/administrador "+dir.path());
       process.startDetached("xdg-open "+dir.path());
    }

}

void Equipos::on_pB_web_clicked()
{
    QDesktopServices::openUrl(QUrl("http://"+ip));
}

void Equipos::on_pB_webssl_clicked()
{
    QDesktopServices::openUrl(QUrl("https://"+ip));
}

void Equipos::on_pB_telnet_clicked()
{
    QProcess process;
    process.startDetached("x-terminal-emulator -e telnet "+ip);
}

void Equipos::on_pB_ssh_clicked()
{
    Configuracion config;
    QProcess process;
    process.startDetached("x-terminal-emulator -e ssh -p "+config.cual_es_puerto()+" "+config.cual_es_usuario_remoto()+"@"+ip);
}

void Equipos::on_pB_systemconfig_clicked()
{
    Configuracion config;
    QProcess process;
    if (Version == "18.04"){
        process.startDetached("ssh -p "+config.cual_es_puerto()+" "+config.cual_es_usuario_remoto()+"@"+ip+" -A -C -X -2 -4 -f  /usr/bin/systemsettings5 ");
    }
    else if (Version == "14.04"){
        process.startDetached("ssh -p "+config.cual_es_puerto()+" "+config.cual_es_usuario_remoto()+"@"+ip+" -A -C -X -2 -4 -f  /usr/bin/hardinfo ");
    }
}

void Equipos::on_pB_networkManager_clicked()
{
    Configuracion config;
    QProcess process;
    if (Version == "18.04"){
    process.startDetached("ssh -p "+config.cual_es_puerto()+" "+config.cual_es_usuario_remoto()+"@"+ip+" -A -C -X -2 -4 -f /usr/bin/kcmshell5 kcm_networkmanagement.desktop ");
    }
    else if (Version == "14.04"){
        process.startDetached("ssh -p "+config.cual_es_puerto()+" "+config.cual_es_usuario_remoto()+"@"+ip+" -A -C -X -2 -4 -f  /usr/bin/nm-connection-editor ");
    }

}

void Equipos::version()
{
    Configuracion config;
    QProcess *Ver = new QProcess(this);
    Ver->start("/usr/bin/version.sh", QStringList() << config.cual_es_usuario_remoto() << ip << config.cual_es_puerto());
    Ver->waitForFinished();
    if (Ver->exitCode() == 14 ){
        Version="14.04";
    }
    else if (Ver->exitCode() == 18 )
    {
        Version="18.04";
    }
    else if (Ver->exitCode() == 1)
    {
            Version="Desconocida";
    }
}
void Equipos::on_pB_cups_clicked()
{

    Configuracion config;
    QProcess ssh;
    QProcess cups;
    QProcess espera;
    QString puerto = QString::number(puerto_libre());

    ssh.startDetached("ssh -t  -p "+config.cual_es_puerto()+" -L "+puerto+":localhost:631 "+config.cual_es_usuario_remoto()+"@"+ip);
    espera.start("sleep 1");
    espera.waitForFinished();
    cups.startDetached("/usr/bin/remoto-cli.sh "+config.cual_es_clave_remoto()+" "+puerto);


}

void Equipos::on_pB_vnc_clicked()
{
    Configuracion config;
    QProcess ssh;
    QProcess vnc;
    QProcess espera;
    QString puerto = QString::number(puerto_libre());

    ssh.startDetached("ssh -t -p "+config.cual_es_puerto()+" -L "+puerto+":localhost:5900 "+config.cual_es_usuario_remoto()+"@"+ip+" /usr/bin/remoto.sh");
    espera.start("sleep 1");
    espera.waitForFinished();
    vnc.startDetached("/usr/bin/remoto-cli.sh "+puerto);

}

void Equipos::on_pushButton_12_clicked()
{
    Configuracion config;
    QProcess process;
    process.startDetached("ssh -p "+config.cual_es_puerto()+" "+config.cual_es_usuario_remoto()+"@"+ip+" -A -C -X -2 -4 -f /usr/bin/lxtask ");
}

void Equipos::on_pB_update_clicked()
{

}

void Equipos::resultado(QNetworkReply *reply){
qDebug()<<reply->readAll();
}

void Equipos::autenticando(QNetworkReply *aReply,QAuthenticator *aAuthenticator)
{
    Configuracion *config = new Configuracion();
    aAuthenticator->setUser(config->cual_es_tecnico());
    aAuthenticator->setPassword(config->cual_es_clave());
    qDebug()<<"se ha necesitado clave";
    delete config;
}


QString Equipos::glpi_Login(QByteArray glpi){
    QNetworkAccessManager mgr;
    QByteArray datos;
    datos.append("<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                 "<SOAP-ENV:Envelope "
                     "xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                     "xmlns:ns1=\"https://glpi.dipgra.es/glpi/plugins/webservices/soap.php\" "
                     "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
                     "xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" "
                     "xmlns:ns2=\"http://xml.apache.org/xml-soap\" "
                     "xmlns:SOAP-ENC=\"http://schemas.xmlsoap.org/soap/encoding/\" "
                     "SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
                     "<SOAP-ENV:Body>"
                         "<ns1:genericExecute>"
                             "<params xsi:type=\"ns2:Map\">"
                                 "<item><key xsi:type=\"xsd:string\">method</key><value xsi:type=\"xsd:string\">glpi.doLogin</value></item>"
                                 "<item><key xsi:type=\"xsd:string\">login_name</key><value xsi:type=\"xsd:string\">prueba</value></item>"
                                 "<item><key xsi:type=\"xsd:string\">login_password</key><value xsi:type=\"xsd:string\">password</value></item>"
                                 "</params>"
                             "</ns1:genericExecute>"
                         "</SOAP-ENV:Body>"
                     "</SOAP-ENV:Envelope>"
                 );


    QNetworkRequest req( QUrl( QString("https://glpi.dipgra.es/glpi/plugins/webservices/soap.php") ) );

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/soap+xml; charset=utf-8 ");
    req.setHeader(QNetworkRequest::UserAgentHeader, "PHP-SOAP");
    req.setRawHeader("Host","https://glpi.dipgra.es/glpi");
    req.setRawHeader("SOAPAction","\"https://glpi.dipgra.es/glpi/plugins/webservices/soap.php#genericExecute\"");
    QByteArray postDataSize = QByteArray::number(datos.size());
    req.setHeader(QNetworkRequest::ContentLengthHeader, postDataSize);


    QNetworkReply *reply = mgr.post(req,datos);
    connect (&mgr, SIGNAL (authenticationRequired(QNetworkReply *,QAuthenticator *)),this,SLOT(autenticando(QNetworkReply *,QAuthenticator *)) );
    QEventLoop loop;
    connect(&mgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(resultado(QNetworkReply*)));
    loop.exec();
 }

void Equipos::on_pB_creaIncidencia_clicked()
{
    Configuracion *config = new Configuracion();
    QByteArray tmp;
    tmp.append(config->cual_es_glpi());
    glpi_Login(tmp);
    delete config;

}

void Equipos::on_pB_ISL_clicked()
{
    Configuracion *configuracion = new Configuracion;
    QProcess process;
    if (configuracion->usarproxy_chains())
        process.startDetached(configuracion->cual_es_proxychains(), QStringList() << configuracion->cual_es_isl());
    else
        process.startDetached(configuracion->cual_es_isl());
    delete configuracion;
}

void Equipos::on_pB_procesos_clicked()
{
    Configuracion config;
    QProcess process;
    if (Version == "18.04")
        process.startDetached("ssh -p "+config.cual_es_puerto()+" "+config.cual_es_usuario_remoto()+"@"+ip+" -A -C -X -2 -4 -f /usr/bin/systemmonitor ");
    else if (Version == "14.04")
        process.startDetached("ssh -p "+config.cual_es_puerto()+" "+config.cual_es_usuario_remoto()+"@"+ip+" -A -C -X -2 -4 -f  /usr/bin/lxtask ");

}

void Equipos::on_pB_CUPS_clicked()
{
    Configuracion config;
    QProcess process;
    if (Version == "18.04")
        process.startDetached("ssh -p "+config.cual_es_puerto()+" "+config.cual_es_usuario_remoto()+"@"+ip+" -A -C -X -2 -4 -f /usr/bin/systemmonitor ");
    else if (Version == "14.04")
        process.startDetached("ssh -p "+config.cual_es_puerto()+" "+config.cual_es_usuario_remoto()+"@"+ip+" -A -C -X -2 -4 -f  /usr/bin/lxtask ");
}

void Equipos::on_pB_usuarios_clicked()
{
    Configuracion config;
    QProcess process;
    if (Version == "18.04")
        process.startDetached("ssh -p "+config.cual_es_puerto()+" "+config.cual_es_usuario_remoto()+"@"+ip+" -A -C -X -2 -4 -f /usr/lib/x86_64-linux-gnu/libexec/kf5/kdesu -u root -c /usr/bin/kcmshell5 user_manager ");
    else if (Version == "14.04")
        process.startDetached("ssh -p "+config.cual_es_puerto()+" "+config.cual_es_usuario_remoto()+"@"+ip+" -A -C -X -2 -4 -f  /usr/bin/users-admin ");
}

void Equipos::on_pB_proxy_clicked()
{
    Configuracion config;
    QProcess process;
    process.startDetached("ssh -p "+config.cual_es_puerto()+" "+config.cual_es_usuario_remoto()+"@"+ip+" -A -C -X -2 -4 -f /usr/bin/kcmshell5 proxy ");
}

void Equipos::on_pB_usb_clicked()
{
    Configuracion config;
    QProcess process;
    process.startDetached("ssh -p "+config.cual_es_puerto()+" "+config.cual_es_usuario_remoto()+"@"+ip+" -A -C -X -2 -4 -f /usr/bin/kcmshell5 kcmusb ");


}

void Equipos::on_pB_pci_clicked()
{
    Configuracion config;
    QProcess process;
    process.startDetached("ssh -p "+config.cual_es_puerto()+" "+config.cual_es_usuario_remoto()+"@"+ip+" -A -C -X -2 -4 -f /usr/bin/kcmshell5 kcm_pci ");
}

void Equipos::on_pB_impresora_clicked()
{
    Configuracion config;
    QProcess process;

    if (Version == "18.04")
        process.startDetached("ssh -p "+config.cual_es_puerto()+" "+config.cual_es_usuario_remoto()+"@"+ip+" -A -C -X -2 -4 -f /usr/bin/kcmshell5 kcm_printer_manager  ");
    else if (Version == "14.04")
        process.startDetached("ssh -p "+config.cual_es_puerto()+" "+config.cual_es_usuario_remoto()+"@"+ip+" -A -C -X -2 -4 -f /usr/bin/python /usr/share/system-config-printer/system-config-printer.py ");
 }

void Equipos::on_pB_memoria_clicked()
{
    Configuracion config;
    QProcess process;
    process.startDetached("ssh -p "+config.cual_es_puerto()+" "+config.cual_es_usuario_remoto()+"@"+ip+" -A -C -X -2 -4 -f /usr/bin/kcmshell5 kcm_memory ");
}

void Equipos::on_pB_devices_clicked()
{
    Configuracion config;
    QProcess process;
    process.startDetached("ssh -p "+config.cual_es_puerto()+" "+config.cual_es_usuario_remoto()+"@"+ip+" -A -C -X -2 -4 -f /usr/bin/kcmshell5 devinfo ");
}

void Equipos::on_pB_clock_clicked()
{
    Configuracion config;
    QProcess process;

    if (Version == "18.04")
        process.startDetached("ssh -p "+config.cual_es_puerto()+" "+config.cual_es_usuario_remoto()+"@"+ip+" -A -C -X -2 -4 -f /usr/bin/kcmshell5 clock ");
    else if (Version == "14.04")
        process.startDetached("ssh -p "+config.cual_es_puerto()+" "+config.cual_es_usuario_remoto()+"@"+ip+" -A -C -X -2 -4 -f  /usr/bin/time-admin ");

}

void Equipos::on_pB_captura_clicked()
{
    Configuracion config;
    QProcess process;

    //ruta_local="/tmp/captura-"+nombre+"-"+ip+time.strftime("%H:%M:%S")+time.strftime("%d/%m/%y")+".png"
   /* DISPLAY='+display+' XAUTHORITY=/home/'+nombre+'/.Xauthority scrot /tmp/captura.png')
                get (remote_path="/tmp/captura.png",local_path=ruta_local)
                os.system('xdg-open '+ruta_local+'&')

    */

    process.startDetached('ssh -Y -p'+config.cual_es_puerto()+' '+config.cual_es_usuario_remoto()+'@'+ip+' "/usr/lib/x86_64-linux-gnu/libexec/kf5/kdesu -u root -c DISPLAY=:0.0 XAUTHORITY=/home/pepe/.Xauthority spectacle -f -o /tmp/captura.png"');

}

void Equipos::on_pB_discos_clicked()
{
    Configuracion config;
    QProcess process;
    process.startDetached("ssh -p "+config.cual_es_puerto()+" "+config.cual_es_usuario_remoto()+"@"+ip+" -A -C -X -2 -4 -f /usr/bin/partitionmanager ");


}

void Equipos::on_pB_equipo_clicked()
{
    Configuracion config;
    QProcess process;
    process.startDetached("ssh -p "+config.cual_es_puerto()+" "+config.cual_es_usuario_remoto()+"@"+ip+" -A -C -X -2 -4 -f /usr/bin/hardinfo ");

}

void Equipos::on_pB_instala_clicked()
{

    Configuracion config;
    QProcess process;
    if (Version == "18.04")
        process.startDetached("ssh -p "+config.cual_es_puerto()+" "+config.cual_es_usuario_remoto()+"@"+ip+" -A -C -X -2 -4 -f /usr/lib/x86_64-linux-gnu/libexec/kf5/kdesu -u root -c /usr/bin/muon ");
    else if (Version == "14.04")
        process.startDetached("ssh -p "+config.cual_es_puerto()+" "+config.cual_es_usuario_remoto()+"@"+ip+" -A -C -X -2 -4 -f /usr/bin/python3 /usr/bin/software-properties-gtk ");
}


void Equipos::closeTab(int indice){
    if (indice>0)
        ui->tabWidget->removeTab(indice);
}

void Equipos::on_pB_usuarios_2_clicked()
{

    ui->tabWidget->insertTab(ui->tabWidget->count(),new Usuarios(),"nombre");
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

}
