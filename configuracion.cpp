#include "configuracion.h"
#include "ui_configuracion.h"
#include <QStandardPaths>
#include <QtDebug>
#include <QProcess>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QSqlQueryModel>

Configuracion::Configuracion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Configuracion)
{
    ui->setupUi(this);
    //Vamos a poner en el constructor la máscara para validar la ip introducida
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ipRegex ("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    ui->servidor->setValidator(ipValidator);

    //Vamos a poner en el constructor la máscara para validar los puertos para ssh
    //ponemos 15 puertos como maximo
    QString puertoIN = "(?:[0-5]?[0-9]?[0-9]?[0-9]?[0-9])";
    QRegExp puertoRegex ("^" + puertoIN+ "\\," + puertoIN+ "\\," + puertoIN
                     + "\\," + puertoIN+ "\\," + puertoIN+ "\\," + puertoIN
                     + "\\," + puertoIN+ "\\," + puertoIN+ "\\," + puertoIN
                     + "\\," + puertoIN+ "\\," + puertoIN+ "\\," + puertoIN
                     + "\\," + puertoIN+ "\\," + puertoIN+ "\\," + puertoIN + "$");
    QRegExpValidator *puertoValidator = new QRegExpValidator(puertoRegex, this);
    ui->lineEdit_puertos->setValidator(puertoValidator);


    //Vamos a poner en el constructor la máscara para validar el puerto introducido
    QString puerto = "(?:[0-5]?[0-9]?[0-9]?[0-9]?[0-9])";
    QRegExp ipRegex2 ("^" + puerto+ "$");
    QRegExpValidator *ipValidator2 = new QRegExpValidator(ipRegex2, this);
    ui->puerto->setValidator(ipValidator2);
    carga_configuracion();
    carga_configuracion_color();


    ui->lb_rutas->setText("<a href=\"/\">Configuración de Rutas</a>");

    }

Configuracion::~Configuracion()
{
    delete ui;
}

void Configuracion::valoresPorDefecto(){

    home_usuario = QStandardPaths::locate(QStandardPaths::HomeLocation, QString(), QStandardPaths::LocateDirectory);
    QSettings s(home_usuario+".grxconf.ini", QSettings::IniFormat);
    QString name = qgetenv("USER");
    if (name.isEmpty())
        name = qgetenv("USERNAME");
    Tecnico = name;
    Clave = "0";
    ServidorAD = "10.1.1.96";
    UsuarioAD = name;
    ClaveAD = "0";
    DataBaseName = "asismun";
    HostName = "127.0.0.1";
    PuertoDB = 3306;
    UserName = "root";
    PasswordDB = "0";
    UsarSSH = "TRUE";
    UsarProxyChains = "TRUE";
    ProxyChains = "/usr/bin/proxychains";
    ServidorSSH = "10.7.15.193";
    UsuarioSSH =  "gorgojo";
    ClaveSSH = "0";
    PuertoRemotoSSH = 22;
    PuertoLocalSSH = "0";
    ISL = "/opt/ISLOnline/ISLLight/ISLLight";
    Atalaya = "https://atalaya.grx";
    UsarUsuarios = "True";
    UsarSoporte = "True";
    UsarSedes = "True";
    UsarCronos = "True";
    UsarWebmail = "True";
    UsarBeiro = "True";
    UsarGlpi = "True";
    UsarOCS = "True";
    UsarTS = "True";
    UsarISL = "True";
    UsarAtalaya = "True";
    SoloAytos = "True";
    OCS = "https://incidencias.dipgra.es/ocsreport";
    GLPI = "https://incidencias.dipgra.es/glpi";
    Beiro = "http://beiro.grx:55555/portal";
    Cronos = "http://cronos.grx";
    UsuarioRemoto = "administrador";
    Puerto = 8080;
    Correo = "https://correoweb.dipgra.es/";
    Password = "";
    ClaveCifrado = "";
    ClaveRemoto = "";
    Rdesktop = "";
    Resolucion = "1024x768";
    Para = "Pon aqui los destinatarios";
    Asunto = "Pon aqui el asunto";
    Cuerpo = "Escribe aqui el cuerpo del mensaje.";

    PuertosBuscados_ssh="true";
    PuertosBuscados_portPrinter="true";
    PuertosBuscados_netbios="true";
    PuertosBuscados_telnet="true";
    PuertosBuscados_webssl="true";
    PuertosBuscados_web="true";
    PuertosBuscados_lineEdit="";

    carga_editLine();
}

QString  Configuracion::cual_es_home(){
    return home_usuario;
}
QString  Configuracion::cual_es_ini(){
    return home_usuario+".grxconf.ini";
}
QString  Configuracion::cual_es_tecnico(){
    return Tecnico;
}
QString  Configuracion::cual_es_clave(){
    return Clave;
}
QString  Configuracion::cual_es_servidorAD(){
    return ServidorAD;
}
QString  Configuracion::cual_es_claveAD(){
    return ClaveAD;
}
QString  Configuracion::cual_es_usuarioAD(){
    return UsuarioAD;
}
QString  Configuracion::cual_es_beiro(){
    return Beiro;
}
QString  Configuracion::cual_es_DataBaseName(){
    return DataBaseName;
}
QString  Configuracion::cual_es_hostnameDB(){
    return HostName;
}
QString  Configuracion::cual_es_PuertoDB(){
    return PuertoDB;
}
QString  Configuracion::cual_es_usernameDB(){
    return UserName;
}
QString  Configuracion::cual_es_passwordDB(){
    return PasswordDB;
}
QString  Configuracion::cual_es_usuario_remoto(){
    return UsuarioRemoto;
}
QString  Configuracion::cual_es_puerto(){
    return Puerto;
}
QString  Configuracion::cual_es_servidorSSH(){
    return ServidorSSH;
}
QString  Configuracion::cual_es_usuarioSSH(){
    return UsuarioSSH;
}
QString  Configuracion::cual_es_password_ssh(){
    return ClaveSSH;
}
int  Configuracion::cual_es_puerto_remoto_ssh(){
    return PuertoRemotoSSH;
}
int  Configuracion::cual_es_puerto_local_ssh(){
    return PuertoLocalSSH;
}
QString  Configuracion::cual_es_isl(){
    return ISL;
}
QString  Configuracion::cual_es_cronos(){
    return Cronos;
}
QString  Configuracion::cual_es_correo(){
    return Correo;
}
QString  Configuracion::cual_es_glpi(){
    return GLPI;
}
QString  Configuracion::cual_es_ocs(){
    return OCS;
}
QString  Configuracion::cual_es_atalaya(){
    return Atalaya;
}
QString  Configuracion::cual_es_clave_cifrado(){
    return ClaveCifrado;
}
QString  Configuracion::cual_es_clave_remoto(){
     return ClaveRemoto;
}
QString  Configuracion::cual_es_rdesktop(){
    return Rdesktop;
}
bool Configuracion::es_rdesktop(){
    if (Rdesktop =="True")
          return true;
return false;
}
QString  Configuracion::cual_es_keyfile_privada(){
    return KeyFile_privada;
}
QString  Configuracion::cual_es_keyfile_publica(){
    return KeyFile_publica;
}

QString  Configuracion::cual_es_para(){
    return Para;
}
QString  Configuracion::cual_es_asunto(){
    return Asunto;
}
QString  Configuracion::cual_es_cuerpo(){
    return Cuerpo;
}

QString Configuracion::cual_es_proxychains(){
    return ProxyChains;
}

bool Configuracion::es_usarproxy_chains(){
    if (UsarProxyChains =="True")
        return true;
return false;
}

bool Configuracion::es_usarSSH(){
      if (UsarSSH =="True")
          return true;
return false;
}

bool Configuracion::usuarios_up(){
      if (UsarUsuarios =="True")
          return true;
return false;
}

bool Configuracion::soporte_up(){
      if (UsarSoporte =="True")
          return true;
return false;
}
bool Configuracion::sedes_up(){
      if (UsarSedes =="True")
          return true;
return false;
}
bool Configuracion::cronos_up(){
      if (UsarCronos =="True")
          return true;
return false;
}
bool Configuracion::webmail_up(){
      if (UsarWebmail =="True")
          return true;
return false;
}
bool Configuracion::beiro_up(){
      if (UsarBeiro =="True")
          return true;
return false;
}
bool Configuracion::glpi_up(){
      if (UsarGlpi =="True")
          return true;
return false;
}
bool Configuracion::ocs_up(){
      if (UsarOCS =="True")
          return true;
return false;
}
bool Configuracion::ts_up(){
      if (UsarTS =="True")
          return true;
return false;
}
bool Configuracion::isl_up(){
      if (UsarISL =="True")
          return true;
return false;
}
bool Configuracion::atalaya_up(){
      if (UsarAtalaya =="True")
          return true;
return false;
}

bool Configuracion::solo_aytos(){
    if (SoloAytos =="True")
        return true;
return false;
}

bool Configuracion::puertoSSH(){
    if (PuertosBuscados_ssh =="True")
        return true;
return false;
}

bool Configuracion::puertoTelnet(){
    if (PuertosBuscados_telnet =="True")
        return true;
return false;
}

bool Configuracion::puertoWeb(){
    if (PuertosBuscados_web =="True")
        return true;
return false;
}

bool Configuracion::puertoWebssl(){
    if (PuertosBuscados_webssl =="True")
        return true;
return false;
}

bool Configuracion::puertoPortPrinter(){
    if (PuertosBuscados_portPrinter =="True")
        return true;
return false;
}

bool Configuracion::puertoNetbios(){
    if (PuertosBuscados_netbios =="True")
        return true;
return false;
}

QString  Configuracion::cual_es_lineEditSSH(){
    return PuertosBuscados_lineEdit;
}



QString  Configuracion::cual_es_resolucion(){
      return Resolucion;
}


QString Configuracion::puertos_buscados(){
    QString tmp="";
    if (ui->checkBox_SSH->isChecked()){
        tmp.append("22");
    }

    if (ui->checkBox_telnet->isChecked()){
        if (!tmp.isEmpty())
               tmp.append(",");

        tmp.append("23");
    }

    if (ui->checkBox_portPrinter ->isChecked()){
        if (!tmp.isEmpty())
               tmp.append(",");

        tmp.append("9100");
    }

    if (ui->checkBox_web->isChecked()){
        if (!tmp.isEmpty())
               tmp.append(",");

        tmp.append("80");
    }

    if (ui->checkBox_webssl->isChecked()){
        if (!tmp.isEmpty())
               tmp.append(",");
        tmp.append("443");
    }

    if (ui->checkBox_netbios->isChecked()){
        if (!tmp.isEmpty())
               tmp.append(",");
        tmp.append("139");
    }
    if (!ui->lineEdit_puertos->text().isEmpty())
        if (!tmp.isEmpty())
           tmp.append(",");
        tmp.append(ui->lineEdit_puertos->text());
return tmp;
}


void Configuracion::carga_configuracion_color(){

    ui->fr_linux->setStyleSheet("background-color:"+Fr_linux+";");
    ui->fr_rutas->setStyleSheet("background-color:"+Fr_rutas+";");
    ui->fr_DB->setStyleSheet("background-color:"+Fr_DB+";");
    ui->fr_TS->setStyleSheet("background-color:"+Fr_TS+";");
    ui->fr_kerberos->setStyleSheet("background-color:"+Fr_kerberos+";");
}

void Configuracion::carga_configuracion()
{
    //Carga de los valores de las variables de configuracion
    //Usamos Qsettings para leer los valores de las variables de un archivo .ini
    home_usuario = QStandardPaths::locate(QStandardPaths::HomeLocation, QString(), QStandardPaths::LocateDirectory);
    QSettings s(home_usuario+".grxconf.ini", QSettings::IniFormat);
    Tecnico = s.value("Configuracion/Tecnico").toString();
    Clave = s.value("Configuracion/Clave").toString();
    ServidorAD = s.value("Configuracion/ServidorAD").toString();
    UsuarioAD = s.value("Configuracion/UsuarioAD").toString();
    ClaveAD = s.value("Configuracion/ClaveAD").toString();
    DataBaseName = s.value("Configuracion/DataBaseName").toString();
    HostName = s.value("Configuracion/HostName").toString();
    PuertoDB = s.value("Configuracion/PuertoDB").toString();
    UserName = s.value("Configuracion/UserName").toString();
    PasswordDB = s.value("Configuracion/PasswordDB").toString();
    UsarSSH = s.value("Configuracion/UsarSSH").toString();
    UsarProxyChains = s.value("Configuracion/UsarProxyChains").toString();
    ProxyChains = s.value("Configuracion/ProxyChains").toString();
    ServidorSSH = s.value("Configuracion/ServidorSSH").toString();
    UsuarioSSH =  s.value("Configuracion/UsuarioSSH").toString();
    ClaveSSH = s.value("Configuracion/ClaveSSH").toString();
    PuertoRemotoSSH = s.value("Configuracion/PuertoRemotoSSH").toInt();
    PuertoLocalSSH = s.value("Configuracion/PuertoLocalSSH").toInt();
    ISL = s.value("Configuracion/ISL").toString();
    Atalaya = s.value("Configuracion/Atalaya").toString();

    UsarUsuarios = s.value("Configuracion/UsarUsuarios").toString();
    UsarSoporte = s.value("Configuracion/UsarSoporte").toString();
    UsarSedes = s.value("Configuracion/UsarSedes").toString();
    UsarCronos = s.value("Configuracion/UsarCronos").toString();
    UsarWebmail = s.value("Configuracion/UsarWebmail").toString();
    UsarBeiro = s.value("Configuracion/UsarBeiro").toString();
    UsarGlpi = s.value("Configuracion/UsarGLPI").toString();
    UsarOCS = s.value("Configuracion/UsarOCS").toString();
    UsarTS = s.value("Configuracion/UsarTS").toString();
    UsarISL = s.value("Configuracion/UsarISL").toString();
    UsarAtalaya = s.value("Configuracion/UsarAtalaya").toString();

    SoloAytos = s.value("Configuracion/SoloAytos").toString();

    OCS = s.value("Configuracion/OCS").toString();
    GLPI = s.value("Configuracion/GLPI").toString();
    Beiro = s.value("Configuracion/Beiro").toString();
    Cronos = s.value("Configuracion/Cronos").toString();
    UsuarioRemoto = s.value("Configuracion/UsuarioRemoto").toString();
    Puerto = s.value("Configuracion/Puerto").toString();
    Correo = s.value("Configuracion/Correo").toString();
    Password = s.value("Configuracion/Password").toString();
    ClaveCifrado = s.value("Configuracion/ClaveCifrado").toString();
    ClaveRemoto = s.value("Configuracion/ClaveRemoto").toString();
    Rdesktop = s.value("Configuracion/Rdesktop").toString();
    Resolucion = s.value("Configuracion/Resolucion").toString();

    Para = s.value("Configuracion/Para").toString();
    Asunto = s.value("Configuracion/Asunto").toString();
    Cuerpo = s.value("Configuracion/Cuerpo").toString();


    KeyFile_privada = s.value("Configuracion/KeyFile_privada").toString();
    KeyFile_publica = s.value("Configuracion/KeyFile_publica").toString();
    //Colores
    Fr_linux = s.value("Configuracion/fr_linux").toString();
    Fr_kerberos = s.value("Configuracion/fr_kerberos").toString();
    Fr_DB = s.value("Configuracion/fr_DB").toString();
    Fr_TS = s.value("Configuracion/fr_TS").toString();
    Fr_rutas = s.value("Configuracion/fr_rutas").toString();

    PuertosBuscados_ssh = s.value("Configuracion/PuertosBuscados_ssh").toString();
    PuertosBuscados_telnet = s.value("Configuracion/PuertosBuscados_telnet").toString();
    PuertosBuscados_web = s.value("Configuracion/PuertosBuscados_web").toString();
    PuertosBuscados_webssl = s.value("Configuracion/PuertosBuscados_webssl").toString();
    PuertosBuscados_portPrinter = s.value("Configuracion/PuertosBuscados_portPrinter").toString();
    PuertosBuscados_netbios = s.value("Configuracion/PuertosBuscados_netbios").toString();
    PuertosBuscados_lineEdit = s.value("Configuracion/PuertosBuscados_lineEdit").toString();
    carga_editLine();

}

void Configuracion::carga_editLine(){

    if (es_rdesktop())
        ui->rb_rdesktop->setChecked(true);
    else
        ui->rb_freerdp->setChecked(true);

    if (es_usarSSH()){
        ui->checkBox_ssh->setChecked(true);
        habilitaSSH();
    }
    if (es_usarproxy_chains()){
        ui->checkBox_proxychains->setChecked(true);
        habilitaProxyChains();
    }
    else{
        ui->checkBox_proxychains->setChecked(false);
        deshabilitaProxyChains();
    }

    ui->checkBox_Usuarios->setChecked(usuarios_up());
    ui->checkBox_Soporte->setChecked(soporte_up());
    ui->checkBox_Sedes->setChecked(sedes_up());
    ui->checkBox_Cronos->setChecked(cronos_up());
    ui->checkBox_Webmail->setChecked(webmail_up());
    ui->checkBox_Beiro->setChecked(beiro_up());
    ui->checkBox_GLPI->setChecked(glpi_up());
    ui->checkBox_OCS->setChecked(ocs_up());
    ui->checkBox_TS->setChecked(ts_up());
    ui->checkBox_ISL->setChecked(isl_up());
    ui->checkBox_Atalaya->setChecked(atalaya_up());
    ui->checkBox_soloAytos->setChecked(solo_aytos());

    ui->checkBox_SSH->setChecked(puertoSSH());
    ui->checkBox_telnet->setChecked(puertoTelnet());
    ui->checkBox_web->setChecked(puertoWeb());
    ui->checkBox_webssl->setChecked(puertoWebssl());
    ui->checkBox_portPrinter->setChecked(puertoPortPrinter());
    ui->checkBox_netbios->setChecked(puertoNetbios());

    ui->tecnico->setText(Tecnico);
    ui->clave->setText(Clave);
    ui->servidor->setText(ServidorAD);
    ui->usuario_ad->setText(UsuarioAD);
    ui->DataBaseName->setText(DataBaseName);
    ui->puerto_DB->setText((QString)PuertoDB);
    ui->servidor_DB->setText(HostName);
    ui->Usuario_DB->setText(UserName);
    ui->password_DB->setText(PasswordDB);
    ui->servidor_SSH->setText(ServidorSSH);
    ui->usuario_ssh_BD->setText(UsuarioSSH);
    ui->clave_ssh_BD->setText(ClaveSSH);
    ui->keyfile_privada->setText(KeyFile_privada);
    ui->keyfile_publica->setText(KeyFile_publica);
    ui->puerto_Remoto_ssh->setText(QString::number(PuertoRemotoSSH));
    ui->clave_ad->setText(ClaveAD);
    ui->usuario_remoto->setText(UsuarioRemoto);
    ui->puerto->setText(Puerto);
    ui->ISL->setText(ISL);
    ui->OCS->setText(OCS);
    ui->GLPI->setText(GLPI);
    ui->beiro->setText(Beiro);
    ui->cronos->setText(Cronos);
    ui->correoweb->setText(Correo);
    ui->password->setText(Password);
    ui->clave_cifrado->setText(ClaveCifrado);
    ui->clave_remoto->setText(ClaveRemoto);
    ui->cb_resolucion->setCurrentText(Resolucion);
    ui->para->setText(Para);
    ui->asunto->setText(Asunto);
    ui->cuerpo->setText(Cuerpo);
    ui->atalaya->setText(Atalaya);
    ui->proxychains->setText(ProxyChains);
    ui->lineEdit_puertos->setText(PuertosBuscados_lineEdit);
}




void Configuracion::habilitaProxyChains(){
    ui->proxychains->setEnabled(true);
}

void Configuracion::deshabilitaProxyChains(){
    ui->proxychains->setEnabled(false);
}

void Configuracion::habilitaSSH(){
    ui->keyfile_privada->setEnabled(true);
    ui->keyfile_publica->setEnabled(true);
    ui->servidor_SSH->setEnabled(true);
    ui->usuario_ssh_BD->setEnabled(true);
    ui->clave_ssh_BD->setEnabled(true);
    ui->puerto_Remoto_ssh->setEnabled(true);

}
void Configuracion::deshabilitaSSH(){
    ui->keyfile_privada->setEnabled(false);
    ui->keyfile_publica->setEnabled(false);
    ui->servidor_SSH->setEnabled(false);
    ui->usuario_ssh_BD->setEnabled(false);
    ui->clave_ssh_BD->setEnabled(false);
    ui->puerto_Remoto_ssh->setEnabled(false);
}
void Configuracion::on_buttonBox_accepted()
{
    //Usamos Qsettings para guardar los valores de las variables en un archivo .ini
    QString home_usuario = QStandardPaths::locate(QStandardPaths::HomeLocation, QString(), QStandardPaths::LocateDirectory);
    QSettings s(home_usuario+".grxconf.ini", QSettings::IniFormat);
    s.setValue("Configuracion/Tecnico", ui->tecnico->text());
    s.setValue("Configuracion/Clave", ui->clave->text());
    s.setValue("Configuracion/ServidorAD", ui->servidor->text());
    s.setValue("Configuracion/UsuarioAD", ui->usuario_ad->text());
    s.setValue("Configuracion/ClaveAD", ui->clave_ad->text());
    s.setValue("Configuracion/UsuarioRemoto",ui->usuario_remoto->text());
    s.setValue("Configuracion/Puerto", ui->puerto->text());
    s.setValue("Configuracion/Correo", ui->correoweb->text());
    s.setValue("Configuracion/ISL", ui->ISL->text());
    s.setValue("Configuracion/Atalaya", ui->atalaya->text());
    s.setValue("Configuracion/DataBaseName",ui->DataBaseName->text());
    s.setValue("Configuracion/HostName", ui->servidor_DB->text());
    s.setValue("Configuracion/UserName", ui->Usuario_DB->text());
    s.setValue("Configuracion/PasswordDB", ui->password_DB->text());
    s.setValue("Configuracion/PuertoDB", ui->puerto_DB->text());
    s.setValue("Configuracion/ServidorSSH", ui->servidor_SSH->text());
    s.setValue("Configuracion/UsuarioSSH", ui->usuario_ssh_BD->text());
    s.setValue("Configuracion/ClaveSSH", ui->clave_ssh_BD->text());
    s.setValue("Configuracion/PuertoRemotoSSH", ui->puerto_Remoto_ssh->text());
    s.setValue("Configuracion/KeyFile_privada", ui->keyfile_privada->text());
    s.setValue("Configuracion/KeyFile_publica", ui->keyfile_publica->text());
    s.setValue("Configuracion/ProxyChains", ui->proxychains->text());

    s.setValue("Configuracion/OCS", ui->OCS->text());
    s.setValue("Configuracion/GLPI", ui->GLPI->text());
    s.setValue("Configuracion/Beiro", ui->beiro->text());
    s.setValue("Configuracion/Cronos", ui->cronos->text());
    s.setValue("Configuracion/Password", ui->password->text());
    s.setValue("Configuracion/ClaveCifrado",ui->clave_cifrado->text());
    s.setValue("Configuracion/ClaveRemoto",ui->clave_remoto->text());
    s.setValue("Configuracion/Resolucion",ui->cb_resolucion->currentText());
    s.setValue("Configuracion/fr_linux",Fr_linux);
    s.setValue("Configuracion/fr_kerberos",Fr_kerberos);
    s.setValue("Configuracion/fr_DB",Fr_DB);
    s.setValue("Configuracion/fr_TS",Fr_TS);
    s.setValue("Configuracion/fr_rutas",Fr_rutas);
    s.setValue("Configuracion/PuertosBuscados_lineEdit",ui->lineEdit_puertos->text());

    s.setValue("Configuracion/Para",ui->para->text());
    s.setValue("Configuracion/Asunto",ui->asunto->text());
    s.setValue("Configuracion/Cuerpo",ui->cuerpo->toPlainText());



    if (ui->checkBox_ssh->isChecked())
        s.setValue("Configuracion/UsarSSH","True");
    else
        s.setValue("Configuracion/UsarSSH","False");

    if (ui->checkBox_proxychains->isChecked())
        s.setValue("Configuracion/UsarProxyChains","True");
    else
        s.setValue("Configuracion/UsarProxyChains","False");

    if (ui->checkBox_ssh->isChecked())
        s.setValue("Configuracion/UsarSSH","True");
    else
        s.setValue("Configuracion/UsarSSH","False");

    if (ui->checkBox_Usuarios->isChecked())
        s.setValue("Configuracion/UsarUsuarios","True");
    else
        s.setValue("Configuracion/UsarUsuarios","False");

    if (ui->checkBox_Soporte->isChecked())
        s.setValue("Configuracion/UsarSoporte","True");
    else
        s.setValue("Configuracion/UsarSoporte","False");

    if (ui->checkBox_Sedes->isChecked())
        s.setValue("Configuracion/UsarSedes","True");
    else
        s.setValue("Configuracion/UsarSedes","False");

    if (ui->checkBox_Cronos->isChecked())
        s.setValue("Configuracion/UsarCronos","True");
    else
        s.setValue("Configuracion/UsarCronos","False");

    if (ui->checkBox_Webmail->isChecked())
        s.setValue("Configuracion/UsarWebmail","True");
    else
        s.setValue("Configuracion/UsarWebmail","False");

    if (ui->checkBox_Beiro->isChecked())
        s.setValue("Configuracion/UsarBeiro","True");
    else
        s.setValue("Configuracion/UsarBeiro","False");

    if (ui->checkBox_GLPI->isChecked())
        s.setValue("Configuracion/UsarGLPI","True");
    else
        s.setValue("Configuracion/UsarGLPI","False");

    if (ui->checkBox_OCS->isChecked())
        s.setValue("Configuracion/UsarOCS","True");
    else
        s.setValue("Configuracion/UsarOCS","False");

    if (ui->checkBox_TS->isChecked())
        s.setValue("Configuracion/UsarTS","True");
    else
        s.setValue("Configuracion/UsarTS","False");

    if (ui->checkBox_ISL->isChecked())
        s.setValue("Configuracion/UsarISL","True");
    else
        s.setValue("Configuracion/UsarISL","False");

    if (ui->checkBox_Atalaya->isChecked())
        s.setValue("Configuracion/UsarAtalaya","True");
    else
        s.setValue("Configuracion/UsarAtalaya","False");

    if (ui->checkBox_soloAytos->isChecked())
        s.setValue("Configuracion/SoloAytos","True");
    else
        s.setValue("Configuracion/SoloAytos","False");

    if (ui->rb_rdesktop->isChecked())
        s.setValue("Configuracion/Rdesktop","True");
    else
        s.setValue("Configuracion/Rdesktop","False");





    if (ui->checkBox_SSH->isChecked())
        s.setValue("Configuracion/PuertosBuscados_ssh","True");
    else
        s.setValue("Configuracion/PuertosBuscados_ssh","False");

    if (ui->checkBox_telnet->isChecked())
        s.setValue("Configuracion/PuertosBuscados_telnet","True");
    else
        s.setValue("Configuracion/PuertosBuscados_telnet","False");

    if (ui->checkBox_web->isChecked())
        s.setValue("Configuracion/PuertosBuscados_web","True");
    else
        s.setValue("Configuracion/PuertosBuscados_web","False");

    if (ui->checkBox_webssl->isChecked())
        s.setValue("Configuracion/PuertosBuscados_webssl","True");
    else
        s.setValue("Configuracion/PuertosBuscados_webssl","False");

    if (ui->checkBox_portPrinter->isChecked())
        s.setValue("Configuracion/PuertosBuscados_portPrinter","True");
    else
        s.setValue("Configuracion/PuertosBuscados_portPrinter","False");

    if (ui->checkBox_netbios->isChecked())
        s.setValue("Configuracion/PuertosBuscados_netbios","True");
    else
        s.setValue("Configuracion/PuertosBuscados_netbios","False");







}
void Configuracion::on_Btn_Kerberos_clicked()
{
    QProcess process;
    process.startDetached("wbinfo -K", QStringList() << ui->tecnico->text());

}
void Configuracion::on_PB_linux_clicked()
{
    QColor color = QColorDialog::getColor(Qt::blue, this );
        if( color.isValid() )
        {
          Fr_linux = color.name();
          ui->fr_linux->setStyleSheet("background-color:"+Fr_linux+";");
        }
}
void Configuracion::on_PB_rutas_clicked()
{
    QColor color = QColorDialog::getColor(Qt::blue, this );
        if( color.isValid() )
        {
          Fr_rutas = color.name();
          ui->fr_rutas->setStyleSheet("background-color:"+Fr_rutas+";");
        }
}
void Configuracion::on_PB_TS_clicked()
{
    QColor color = QColorDialog::getColor(Qt::blue, this );
        if( color.isValid() )
        {
          Fr_TS = color.name();
          ui->fr_TS->setStyleSheet("background-color:"+Fr_TS+";");
        }
}
void Configuracion::on_PB_DB_clicked()
{
    QColor color = QColorDialog::getColor(Qt::blue, this );
        if( color.isValid() )
        {
          Fr_DB = color.name();
          ui->fr_DB->setStyleSheet("background-color:"+Fr_DB+";");
        }
}
void Configuracion::on_PB_kerberos_clicked()
{
    QColor color = QColorDialog::getColor(Qt::blue, this );
        if( color.isValid() )
        {
          Fr_kerberos = color.name();
          ui->fr_kerberos->setStyleSheet("background-color:"+Fr_kerberos+";");
        }
}

void Configuracion::on_lb_rutas_linkActivated(const QString &link)
{
    QColor color = QColorDialog::getColor(Qt::blue, this );
        if( color.isValid() )
        {
          Fr_rutas = color.name();
          ui->fr_rutas->setStyleSheet("background-color:"+Fr_rutas+";");
        }
}

void Configuracion::on_Btn_lupa_clicked()
{
    QString fichero = QFileDialog::getOpenFileName(this, tr("Selecciona el ejecutable"),ui->ISL->text());
    ui->ISL->setText(fichero);
}

void Configuracion::on_checkBox_ssh_clicked()
{
    if (ui->checkBox_ssh->isChecked())
        habilitaSSH();
    else
        deshabilitaSSH();
}

void Configuracion::on_Btn_lupa_2_clicked()
{
    QString fichero = QFileDialog::getOpenFileName(this, tr("Selecciona el keyfile publico"),ui->keyfile_publica->text());
    ui->keyfile_publica->setText(fichero);
}

void Configuracion::on_Btn_lupa_3_clicked()
{
    QString fichero = QFileDialog::getOpenFileName(this, tr("Selecciona el keyfile privado"),ui->keyfile_privada->text());
    ui->keyfile_privada->setText(fichero);
}

void Configuracion::on_checkBox_proxychains_toggled(bool checked)
{
    if (ui->checkBox_proxychains->isChecked())
        habilitaProxyChains();
    else
        deshabilitaProxyChains();

}

void Configuracion::on_buttonBox_clicked(QAbstractButton *button)
{
    if (button==ui->buttonBox->button(QDialogButtonBox::RestoreDefaults))
        valoresPorDefecto();

}

void Configuracion::on_pushButton_clicked()
{
    ui->label_ssh->setText(puertos_buscados());
}
