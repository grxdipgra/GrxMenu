#include "equipos.h"
#include "ui_equipos.h"
#include <QTemporaryDir>
#include "qdebug.h"
#include <QProcess>
#include <QTcpServer>
#include <QDesktopServices>
#include "configuracion/configuracion.h"
#include <QEventLoop>

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
    host_tmp=host;//Guardamos para toda la clase el valor de host en variable global
    QString puertos;
    ip=host->address.addr;
    QString puertos_abiertos = host_ports_open_string(host);
    ui->setupUi(this);
    botonesActivos(puertos_abiertos);
    ui->lineEdit_ip->setText(ip);
    ui->lineEdit_puertos->setText(puertos_abiertos);
    ui->lineEdit_puertosBuscados->setText(QString::number(host->ports.port.count())+": "+host_ports_find(host));
    ui->lineEdit_hostname->setText(host->hostnames.hostname.name);
    ui->lineEdit_status->setText(host->status.state);
    ui->lineEdit_tipo->setText(host->address.addrtype);
}

Equipos::~Equipos()
{
    delete ui;
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
    for (int i=0;i<lista.length();i++)
        if ((lista[i]=="22")||(lista[i]=="8080")){
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

        }else if (lista[i]=="80"){
             ui->pB_web->setEnabled(true);
        }else if (lista[i]=="443"){
             ui->pB_webssl->setEnabled(true);
        }else if (lista[i]=="23"){
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
    process.startDetached("ssh -p "+config.cual_es_puerto()+" "+config.cual_es_usuario_remoto()+"@"+ip+" -A -C -X -2 -4 -f  /usr/bin/systemsettings5 ");
}

void Equipos::on_pB_networkManager_clicked()
{
    Configuracion config;
    QProcess process;
    process.startDetached("ssh -p "+config.cual_es_puerto()+" "+config.cual_es_usuario_remoto()+"@"+ip+" -A -C -X -2 -4 -f /usr/bin/kde5-nm-connection-editor ");
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
