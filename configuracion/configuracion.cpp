#include "configuracion.h"
#include "ui_configuracion.h"
#include <QStandardPaths>
#include <QtDebug>
#include <QProcess>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QSqlQueryModel>
#include <QMenu>
#include "buscarou.h"

void valor(QAbstractItemModel* model,QStringList *lista, QModelIndex parent = QModelIndex()) {
    for(int r = 0; r < model->rowCount(parent); ++r) {
        QModelIndex index = model->index(r, 0, parent);
        QVariant name = model->data(index);
        if (lista->indexOf(name.toString()) == -1){
            lista->append(name.toString());
        }
        if( model->hasChildren(index) ) {
            valor(model,lista ,index);
        }
    }
}

Configuracion::Configuracion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Configuracion)
{

    ui->setupUi(this);

    //Vamos a poner en el constructor la máscara para validar la ip introducida
    mascara_ip();

    //Vamos a poner en el constructor la máscara para validar los puertos para ssh
    mascara_puertos_nmap();

    //Vamos a poner en el constructor la máscara para validar el puerto para acceso remoto
    mascara_puerto_remoto();

    //Cargamos todos los valores guardados en el archivo de configuracion
    carga_configuracion();

    //Si le tenemos puestos colores los cargamos
    carga_configuracion_color();
    //Muestra en la consola de kerberos el ticket actual
    muestra_Kerberos();
    //Esto es para crear menus de boton secundario
   //ui->pB_tablasDB->setContextMenuPolicy(Qt::CustomContextMenu); //popup_menu boton secundario para tablas
   // ui->Btn_Kerberos->setContextMenuPolicy(Qt::CustomContextMenu);//popup_menu boton secundario para kerberos
   // connect(ui->pB_tablasDB, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(ctxMenu(const QPoint &)));
    QObject::connect(this,&Configuracion::finished , this, &Configuracion::on_buttonBox_accepted);
}

QDate Configuracion::ultimaActualizacionDB(){
     return fechaActualizacionDB;
}

void Configuracion::help(){
    qDebug()<< "ayuda";
}

void Configuracion::color_widget(QObject * sender){
    change_color(sender);
}

void Configuracion::ctxMenu(const QPoint &pos) {
    QMenu *menu = new QMenu;
    menu->addAction(tr("Color"), this, SLOT(Configuracion::color_widget(sender())));
    menu->addAction(tr("Ayuda"), this, SLOT(help()));
    menu->exec(this->mapToGlobal(pos));
}

Configuracion::~Configuracion()
{

    delete ui;
}

void Configuracion::mascara_puerto_remoto(){
    //Vamos a poner en el constructor la máscara para validar el puerto para acceso remoto
    //y para el puerto de ldap
    QString puerto = "(?:[0-5]?[0-9]?[0-9]?[0-9]?[0-9])";
    QRegExp ipRegex2 ("^" + puerto+ "$");
    QRegExpValidator *ipValidator2 = new QRegExpValidator(ipRegex2, this);
    ui->puerto->setValidator(ipValidator2);
    ui->lineEdit_ldap_puerto->setValidator(ipValidator2);

}

void Configuracion::mascara_puertos_nmap(){
    //Vamos a poner en el constructor la máscara para validar los puertos para ssh
    //ponemos 15 puertos como maximo, si quieres poner mas copia y pega + "\\," + puertoIN+
    QString puertoIN = "(?:[0-5]?[0-9]?[0-9]?[0-9]?[0-9])";
    QRegExp puertoRegex ("^" + puertoIN+ "\\," + puertoIN+ "\\," + puertoIN
                     + "\\," + puertoIN+ "\\," + puertoIN+ "\\," + puertoIN
                     + "\\," + puertoIN+ "\\," + puertoIN+ "\\," + puertoIN
                     + "\\," + puertoIN+ "\\," + puertoIN+ "\\," + puertoIN
                     + "\\," + puertoIN+ "\\," + puertoIN+ "\\," + puertoIN + "$");
    QRegExpValidator *puertoValidator = new QRegExpValidator(puertoRegex, this);
    ui->lineEdit_puertos->setValidator(puertoValidator);
}

void Configuracion::mascara_ip(){
    //Máscara para validar la ip introducida
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ipRegex ("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    ui->servidor->setValidator(ipValidator);

}

void Configuracion::valoresPorDefecto(){

    home_usuario = QStandardPaths::locate(QStandardPaths::HomeLocation, QString(), QStandardPaths::LocateDirectory);
    QSettings s(home_usuario+".grx/.grxconf.ini", QSettings::IniFormat);
    fechaActualizacionDB = QDate::currentDate();
    RutaSqlite = home_usuario+".grx/grx.sqlite";
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
    UserName = "usuario-basico";
    PasswordDB = "dpt.2018GRX";
    UsarSSH = "true";
    UsarProxyChains = "true";
    ProxyChains = "/usr/bin/proxychains";
    ServidorSSH = "10.7.15.193";
    UsuarioSSH =  "gorgojo";
    ClaveSSH = "0";
    PuertoRemotoSSH = "8080";
    PuertoLocalSSH = "3306";
    ISL = "/opt/ISLOnline/ISLLight/ISLLight";
    Atalaya = "http://atalaya.grx";
    UsarUsuarios = "true";
    UsarSoporte = "true";
    UsarSedes = "true";
    UsarCronos = "true";
    UsarWebmail = "true";
    UsarBeiro = "true";
    UsarGlpi = "true";
    UsarOCS = "true";
    UsarTS = "true";
    UsarISL = "true";
    UsarVeleta = "true";
    UsarAtalaya = "true";
    SoloAytos = "false";
    multiplesInstancias = "false";

    UsarOuExternos = "true";
    UsarOuPerrera  = "true";
    UsarOuCie  = "true";
    UsarOuCpd  = "true";
    UsarOuAyuntamientos  = "true";

    OCS = "https://incidencias.dipgra.es/ocsreport";
    GLPI = "https://incidencias.dipgra.es/glpi";
    Beiro = "http://beiro.grx:55555/portal";
    Cronos = "http://cronos.grx";
    UsuarioRemoto = "administrador";
    Puerto = "8080";
    Correo = "https://correoweb.dipgra.es/";
    Password = "";
    ClaveCifrado = "";
    ClaveRemoto = "";
    Rdesktop = "true";
    Resolucion = "1024x768";
    Para = "ie2.cg22@telefonica.com; op.cg22@telefonica.com; marialeticia.larapalomino@telefonica.com; jtoro@dipgra.es; miguel@dipgra.es";
    Asunto = "Problemas de conexión en %1";
    Cuerpo = "Buenos días."
             "Nos llaman desde %1 y nos comentan que no tienen conexión."
             "Hemos comprobado que efectivamente no llegamos a su router."
             "Haced el favor de echarle un vistazo:"
             "+------------------------------------------------------------------+"
             "|  %1	|  %2  |   %4	|   %5	|   %6	|   %7	|   %9    |"
             "+------------------------------------------------------------------+"
             "Gracias de antemano y saludos cordiales.";

    Para_Rutas = "ie2.cg22@telefonica.com; op.cg22@telefonica.com; marialeticia.larapalomino@telefonica.com; jtoro@dipgra.es; miguel@dipgra.es";
    Asunto_Rutas = "Configuracion del enrutado de la conexion de %1";
    Cuerpo_Rutas = "Buenos días."
             "Para configurar la linea de %1 debemos poner las siguientes rutas"
             "Hemos comprobado que efectivamente no llegamos a su router."
             "Haced el favor de echarle un vistazo:"
             "+------------------------------------------------------------------+"
             "|  %1	|  %2  |   %4	|   %5	|   %6	|   %7	|   %9    |"
             "+------------------------------------------------------------------+"
             "Gracias de antemano y saludos cordiales.";

    PuertosBuscados_ssh="true";
    PuertosBuscados_portPrinter="true";
    PuertosBuscados_netbios="true";
    PuertosBuscados_telnet="true";
    PuertosBuscados_webssl="true";
    PuertosBuscados_web="true";
    PuertosBuscados_lineEdit="";

    Servidor_ldap="10.1.1.50";
    Puerto_ldap=389;
    Usuario_ldap=name;
    Clave_ldap="password";
    Dominio_ldap="grx";
    //intentamos recuperar el nombre del dominio a partir del nombre de usuario
    if (name.contains("@",Qt::CaseSensitive)){
        Dominio_ldap=name.right(name.length()-(name.indexOf("@")+1));
    }

    carga_editLine();
}

QString  Configuracion::cual_es_servidor_ldap(){
    return Servidor_ldap;
}

int  Configuracion::cual_es_puerto_ldap(){
    return Puerto_ldap;
}

QString  Configuracion::cual_es_usuario_ldap(){
    return Usuario_ldap;
}

QString  Configuracion::cual_es_usuario_logado(){
    QString name = qgetenv("USER");
    if (name.isEmpty())
        name = qgetenv("USERNAME");
    return name;
}

QString  Configuracion::cual_es_clave_ldap(){
    return Clave_ldap;
}

QString  Configuracion::cual_es_dominio_ldap(){
    return Dominio_ldap;
}
//-------------------------------Serafin


bool Configuracion::listaOU_vacio(){
    return ListaOU.isEmpty();
}

QStringList Configuracion::listaOU_datos(){
    return ListaOU;
}

bool Configuracion::lineEdit_OU_vacio(){
    return lineEdit_OU.isEmpty();
}

QString Configuracion::lineEdit_OU_datos(){
    return lineEdit_OU;
}


void Configuracion::carga_servidor_ldap(QString Sldap){
    Servidor_ldap=Sldap;
}

void  Configuracion::carga_puerto_ldap(int Pldap){
    Puerto_ldap=Pldap;
}

void Configuracion::carga_usuario_ldap(QString Uldap){
    Usuario_ldap=Uldap;
}

void Configuracion::carga_clave_ldap(QString Cldap){
    Clave_ldap=Cldap;
}

void Configuracion::carga_home(QString home){
    home_usuario=home;
}

void Configuracion::carga_dominio_ldap(QString Dldap){
    Dominio_ldap=Dldap;
}

void Configuracion::carga_listaOU(QStringList OU){
    ListaOU.clear();
    foreach (const QString &qstr, OU) {
        ListaOU<<qstr;
    }
}

void Configuracion::lineEdit_OU_datos(QString LEOU){
    lineEdit_OU=LEOU;
}

void Configuracion::carga_ruta_sqlite(QString ruta){
    RutaSqlite= ruta;
}

QString  Configuracion::cual_es_home(){
    return home_usuario;
}

QString  Configuracion::cual_es_ini(){
    return home_usuario+".grx/.grxconf.ini";
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

int  Configuracion::cual_es_PuertoDB(){
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

QString Configuracion::cual_es_ruta_sqlite(){
    return home_usuario + ".grx/grx.sqlite";
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

bool  Configuracion::cual_es_rdesktop(){
    return Rdesktop;
}

bool Configuracion::es_rdesktop(){
    return Rdesktop;
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

QString  Configuracion::cual_es_asunto_rutas(){
    return Asunto_Rutas;
}

QString  Configuracion::cual_es_cuerpo_rutas(){
    return Cuerpo_Rutas;
}

QString  Configuracion::cual_es_para_rutas(){
    return Para_Rutas;
}

QString Configuracion::cual_es_proxychains(){
    return ProxyChains;
}

bool Configuracion::usarproxy_chains(){
    return UsarProxyChains;
}

bool Configuracion::usarSSH(){
    return UsarSSH;
}

bool Configuracion::usar_ou_externos(){
    return UsarOuExternos;
}

bool Configuracion::usar_ou_perrera(){
    return UsarOuPerrera;
}

bool Configuracion::usar_ou_cie(){
    return UsarOuCie;
}

bool Configuracion::usar_ou_cpd(){
    return UsarOuCpd;
}

bool Configuracion::usar_ou_ayuntamientos(){
    return UsarOuAyuntamientos;
}

bool Configuracion::usuarios_up(){
    return  UsarUsuarios;
}

bool Configuracion::soporte_up(){
    return UsarSoporte;
}

bool Configuracion::sedes_up(){
    return UsarSedes;
}

bool Configuracion::cronos_up(){
    return UsarCronos;
}

bool Configuracion::webmail_up(){
    return UsarWebmail;
}

bool Configuracion::beiro_up(){
    return UsarBeiro;
}

bool Configuracion::glpi_up(){
    return UsarGlpi;
}

bool Configuracion::ocs_up(){
    return UsarOCS;
}

bool Configuracion::ts_up(){
    return UsarTS;
}

bool Configuracion::isl_up(){
    return UsarISL;
}

bool Configuracion::veleta_up(){
    return UsarVeleta;
}

bool Configuracion::atalaya_up(){
    return UsarAtalaya;
}

bool Configuracion::solo_aytos(){
    return SoloAytos;
}

bool Configuracion::multiples_instancias(){
    return multiplesInstancias;
}


bool Configuracion::puertoSSH(){
    return PuertosBuscados_ssh;
}

bool Configuracion::puertoTelnet(){
    return PuertosBuscados_telnet;
}

bool Configuracion::puertoWeb(){
    return PuertosBuscados_web;
}

bool Configuracion::puertoWebssl(){
    return PuertosBuscados_webssl;
}

bool Configuracion::puertoPortPrinter(){
    return PuertosBuscados_portPrinter;
}

bool Configuracion::puertoNetbios(){
    return PuertosBuscados_netbios;
}

QString  Configuracion::cual_es_lineEditSSH(){
    return PuertosBuscados_lineEdit;
}

QString  Configuracion::cual_es_resolucion(){
    return Resolucion;
}

QString Configuracion::puertos_buscados(){
    QString tmp="";
    tmp.append("22,23,80,139,443,8080,9100");
    if (!ui->lineEdit_puertos->text().isEmpty())
        if (!tmp.isEmpty())
           tmp.append(",");
        tmp.append(ui->lineEdit_puertos->text());
return tmp;
}

void Configuracion::carga_configuracion_color(){
    ui->consola_kerberos->setStyleSheet("color: white; background-color: black;");
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
    QSettings s(home_usuario+".grx/.grxconf.ini", QSettings::IniFormat);

    fechaActualizacionDB = s.value("Configuracion/fechaActualizacionDB").toDate();
    RutaSqlite = s.value("Configuracion/RutaSqlite").toString();
    Tecnico = s.value("Configuracion/Tecnico").toString();
    Clave = cifra->decryptToString( s.value("Configuracion/Clave").toString());
    ServidorAD = s.value("Directorio_Activo/ServidorAD").toString();
    UsuarioAD = s.value("Directorio_Activo/UsuarioAD").toString();
    ClaveAD = cifra->decryptToString( s.value("Directorio_Activo/ClaveAD").toString());
    DataBaseName = s.value("Configuracion/DataBaseName").toString();
    HostName = s.value("Configuracion/HostName").toString();
    PuertoDB = s.value("Configuracion/PuertoDB").toInt();
    UserName = s.value("Configuracion/UserName").toString();
    PasswordDB =  cifra->decryptToString(s.value("Configuracion/PasswordDB").toString());
    UsarSSH = s.value("Configuracion/UsarSSH").toBool();
    UsarProxyChains = s.value("Configuracion/UsarProxyChains").toBool();
    ProxyChains = s.value("Configuracion/ProxyChains").toString();
    UsuarioRemoto = s.value("Configuracion/UsuarioRemoto").toString();
    Puerto = s.value("Configuracion/Puerto").toString();
    ServidorSSH = s.value("Configuracion/ServidorSSH").toString();
    UsuarioSSH =  s.value("Configuracion/UsuarioSSH").toString();
    ClaveSSH = cifra->decryptToString(s.value("Configuracion/ClaveSSH").toString());
    PuertoRemotoSSH = s.value("Configuracion/PuertoRemotoSSH").toInt();
    PuertoLocalSSH = s.value("Configuracion/PuertoLocalSSH").toInt();
    KeyFile_privada = s.value("Configuracion/KeyFile_privada").toString();
    KeyFile_publica = s.value("Configuracion/KeyFile_publica").toString();
    Password = cifra->decryptToString(s.value("Configuracion/Password").toString());
    ClaveCifrado = cifra->decryptToString(s.value("Configuracion/ClaveCifrado").toString());
    ClaveRemoto =cifra->decryptToString( s.value("Configuracion/ClaveRemoto").toString());
    Rdesktop = s.value("Configuracion/Rdesktop").toBool();
    Resolucion = s.value("Configuracion/Resolucion").toString();
    SoloAytos = s.value("Configuracion/SoloAytos").toBool();
    multiplesInstancias = s.value("Configuracion/multiplesInstancias").toBool();

    //Botones mostrados en botonera
    UsarUsuarios = s.value("Botonera/UsarUsuarios").toBool();
    UsarSoporte = s.value("Botonera/UsarSoporte").toBool();
    UsarSedes = s.value("Botonera/UsarSedes").toBool();
    UsarCronos = s.value("Botonera/UsarCronos").toBool();
    UsarWebmail = s.value("Botonera/UsarWebmail").toBool();
    UsarBeiro = s.value("Botonera/UsarBeiro").toBool();
    UsarGlpi = s.value("Botonera/UsarGLPI").toBool();
    UsarOCS = s.value("Botonera/UsarOCS").toBool();
    UsarTS = s.value("Botonera/UsarTS").toBool();
    UsarVeleta = s.value("Botonera/UsarVeleta").toBool();
    UsarISL = s.value("Botonera/UsarISL").toBool();
    UsarAtalaya = s.value("Botonera/UsarAtalaya").toBool();


    //Rutas
    ISL = s.value("Rutas/ISL").toString();
    Atalaya = s.value("Rutas/Atalaya").toString();
    OCS = s.value("Rutas/OCS").toString();
    GLPI = s.value("Rutas/GLPI").toString();
    Beiro = s.value("Rutas/Beiro").toString();
    Cronos = s.value("Rutas/Cronos").toString();
    Correo = s.value("Rutas/Correo").toString();

    //Correo de incidencias
    Para = s.value("Correo/Para").toString();
    Asunto = s.value("Correo/Asunto").toString();
    Cuerpo = s.value("Correo/Cuerpo").toString();

    //Correo de rutas
    Para_Rutas = s.value("Correo/Para_Rutas").toString();
    Asunto_Rutas = s.value("Correo/Asunto_Rutas").toString();
    Cuerpo_Rutas = s.value("Correo/Cuerpo_Rutas").toString();


    //Colores
    Fr_linux = s.value("Colores/fr_linux").toString();
    Fr_kerberos = s.value("Colores/fr_kerberos").toString();
    Fr_DB = s.value("Colores/fr_DB").toString();
    Fr_TS = s.value("Colores/fr_TS").toString();
    Fr_rutas = s.value("Colores/fr_rutas").toString();

    //Busqueda por nmap
    PuertosBuscados_ssh = s.value("Nmap/PuertosBuscados_ssh").toBool();
    PuertosBuscados_telnet = s.value("Nmap/PuertosBuscados_telnet").toBool();
    PuertosBuscados_web = s.value("Nmap/PuertosBuscados_web").toBool();
    PuertosBuscados_webssl = s.value("Nmap/PuertosBuscados_webssl").toBool();
    PuertosBuscados_portPrinter = s.value("Nmap/PuertosBuscados_portPrinter").toBool();
    PuertosBuscados_netbios = s.value("Nmap/PuertosBuscados_netbios").toBool();
    PuertosBuscados_lineEdit = s.value("Nmap/PuertosBuscados_lineEdit").toString();
    //LDAP
    Servidor_ldap = s.value("ldap/Servidor_ldap").toString();
    Puerto_ldap = s.value("ldap/Puerto_ldap").toInt();
    Usuario_ldap = s.value("ldap/Usuario_ldap").toString();
    Clave_ldap = cifra->decryptToString(s.value("ldap/Clave_ldap").toString());
    Dominio_ldap=s.value("ldap/Dominio_ldap").toString();

    int numero_ou=s.value("ldap/Numero_OU").toInt();

    if (numero_ou>0)
        for (int i=1;i<=numero_ou;i++){
            ListaOU<<s.value("ldap/OU_"+QString::number(i)).toString();
        }
    UsarOuAyuntamientos = s.value("ldap/UsarOuAyuntamientos").toBool();
    lineEdit_OU = s.value("ldap/lineEdit_OU").toString();

/*
    UsarOuExternos = s.value("ldap/UsarOuExternos").toBool();
    UsarOuPerrera = s.value("ldap/UsarOuPerrera").toBool();
    UsarOuCie = s.value("ldap/UsarOuCie").toBool();
    UsarOuCpd = s.value("ldap/UsarOuCpd").toBool();
*/



    carga_editLine();

}


void Configuracion::carga_treeView(){

//cargamos el treeview de UO

QStandardItemModel *standardModel = new QStandardItemModel;

QList<QStandardItem *> fila;
typedef QList<QStandardItem *> tipomio;
QList<tipomio> listas;
//añadiendo una fila al invisibleRootItem producimos un elemento raiz
QStandardItem *item = standardModel->invisibleRootItem();

int i=0,j;
QStringList lista_OU;

QStringList LOU;
QString dominio=dominio_basedn();

for (int i = 0; i < ListaOU.size(); ++i)
        LOU << ListaOU.at(i).toLocal8Bit().constData();

foreach (const QString &strou, LOU) {

    lista_OU.insert(i,strou);

    fila =creaFila(strou);

    listas.insert(i,fila);

    bool encontrado=false;

    //recorremos las OU y vemos si quitandole la ultima parte OU=..., de la cadena
    //es igual a alguna de las que ya hay, si es así se crea como una rama de esta
    while (strou.remove(0,strou.indexOf(",")+1)!=dominio && !encontrado){
        for (j=0;j<lista_OU.count();j++){
            if (strou==lista_OU[j]){
                listas[j].first()->appendRow(listas[i]);
                j=lista_OU.count();
                encontrado=true;
            }
         }
    }

    if (!encontrado)
        item->appendRow(listas[i]);
    i++;

}

ui->treeView_OU->setModel(standardModel);
ui->treeView_OU->expandAll();

}


//Este metodo carga todos los valores de los editline guardados en el archivo de configuracion
// $HOME/.grxconfig
void Configuracion::carga_editLine(){

    if (es_rdesktop())
        ui->rb_rdesktop->setChecked(true);
    else
        ui->rb_freerdp->setChecked(true);

    if (usarSSH()){
        ui->checkBox_ssh->setChecked(true);
        habilitaSSH();
    }
    if (usarproxy_chains()){
        ui->checkBox_proxychains->setChecked(true);
        habilitaProxyChains();
    }
    else{
        ui->checkBox_proxychains->setChecked(false);
        deshabilitaProxyChains();
    }

    ui->checkBox_soloAytos->setChecked(solo_aytos());
    ui->checkBox_multiplesInstancias->setChecked(multiples_instancias());

    ui->checkBox_SSH->setChecked(puertoSSH());
    ui->checkBox_telnet->setChecked(puertoTelnet());
    ui->checkBox_web->setChecked(puertoWeb());
    ui->checkBox_webssl->setChecked(puertoWebssl());
    ui->checkBox_portPrinter->setChecked(puertoPortPrinter());
    ui->checkBox_netbios->setChecked(puertoNetbios());

   /* ui->checkBox_recursos_externos->setChecked(UsarOuExternos);
    ui->checkBox_recursos_perrera->setChecked(UsarOuPerrera);
    ui->checkBox_cie->setChecked(UsarOuCie);
    ui->checkBox_CPD->setChecked(UsarOuCpd);
    ui->checkBox_ayuntamientos->setChecked(UsarOuAyuntamientos);
    ui->lineEdit_OU->setText(lineEdit_OU);
*/
    ui->RutaSqlite->setText(RutaSqlite);
    ui->tecnico->setText(Tecnico);
    ui->clave->setText(Clave);
    ui->servidor->setText(ServidorAD);
    ui->usuario_ad->setText(UsuarioAD);
    ui->DataBaseName->setText(DataBaseName);
    ui->puerto_DB->setText(QString::number(PuertoDB));
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
    ui->asunto_rutas->setText(Asunto_Rutas);
    ui->para->setText(Para);
    ui->asunto->setText(Asunto);
    ui->cuerpo->setText(Cuerpo);
    ui->atalaya->setText(Atalaya);
    ui->proxychains->setText(ProxyChains);
    ui->lineEdit_puertos->setText(PuertosBuscados_lineEdit);
    ui->lineEdit_ldap_servidorLdap->setText(Servidor_ldap);
    ui->lineEdit_ldap_puerto->setText(QString::number(Puerto_ldap));
    ui->lineEdit_ldap_usuarioDominio->setText(Usuario_ldap);
    ui->lineEdit_ldap_Dominio->setText(Dominio_ldap);
    ui->lineEdit_ldap_clave->setText(Clave_ldap);
    carga_treeView();
    ui->fechaActualizacionDB->setDate(fechaActualizacionDB);
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
    QSettings s(home_usuario+".grx/.grxconf.ini", QSettings::IniFormat);
    QStringList lista_OU;

    s.setValue("Configuracion/RutaSqlite", ui->RutaSqlite->text());
    s.setValue("Configuracion/Tecnico", ui->tecnico->text());
    s.setValue("Configuracion/Clave", cifra->encryptToString(ui->clave->text()));
    s.setValue("Configuracion/UsuarioRemoto",ui->usuario_remoto->text());
    s.setValue("Configuracion/Puerto", ui->puerto->text());


    s.setValue("Configuracion/DataBaseName",ui->DataBaseName->text());
    s.setValue("Configuracion/HostName", ui->servidor_DB->text());
    s.setValue("Configuracion/UserName", ui->Usuario_DB->text());
    s.setValue("Configuracion/PasswordDB", cifra->encryptToString(ui->password_DB->text()));
    s.setValue("Configuracion/PuertoDB", ui->puerto_DB->text());
    s.setValue("Configuracion/ServidorSSH", ui->servidor_SSH->text());
    s.setValue("Configuracion/UsuarioSSH", ui->usuario_ssh_BD->text());
    s.setValue("Configuracion/ClaveSSH", cifra->encryptToString(ui->clave_ssh_BD->text()));
    s.setValue("Configuracion/PuertoRemotoSSH", ui->puerto_Remoto_ssh->text());
    s.setValue("Configuracion/KeyFile_privada", ui->keyfile_privada->text());
    s.setValue("Configuracion/KeyFile_publica", ui->keyfile_publica->text());
    s.setValue("Configuracion/ProxyChains", ui->proxychains->text());
    s.setValue("Configuracion/Password", cifra->encryptToString(ui->password->text()));
    s.setValue("Configuracion/ClaveCifrado",cifra->encryptToString(ui->clave_cifrado->text()));
    s.setValue("Configuracion/ClaveRemoto",cifra->encryptToString(ui->clave_remoto->text()));
    s.setValue("Configuracion/Resolucion",ui->cb_resolucion->currentText());
    s.setValue("Configuracion/PuertosBuscados_lineEdit",ui->lineEdit_puertos->text());
    s.setValue("Configuracion/UsarSSH",ui->checkBox_ssh->isChecked());
    s.setValue("Configuracion/UsarProxyChains",ui->checkBox_ssh->isChecked());
    s.setValue("Configuracion/Rdesktop",ui->rb_rdesktop->isChecked());
    s.setValue("Configuracion/SoloAytos",ui->checkBox_soloAytos->isChecked());
    s.setValue("Configuracion/multiplesInstancias",ui->checkBox_multiplesInstancias->isChecked());

    s.setValue("Configuracion/fechaActualizacionDB",ui->fechaActualizacionDB->date());

    s.setValue("Directorio_Activo/ServidorAD", ui->servidor->text());
    s.setValue("Directorio_Activo/UsuarioAD", ui->usuario_ad->text());
    s.setValue("Directorio_Activo/ClaveAD", cifra->encryptToString(ui->clave_ad->text()));


    s.setValue("Rutas/ISL", ui->ISL->text());
    s.setValue("Rutas/Atalaya", ui->atalaya->text());
    s.setValue("Rutas/OCS", ui->OCS->text());
    s.setValue("Rutas/GLPI", ui->GLPI->text());
    s.setValue("Rutas/Beiro", ui->beiro->text());
    s.setValue("Rutas/Cronos", ui->cronos->text());
    s.setValue("Rutas/Correo", ui->correoweb->text());


    s.setValue("Colores/fr_linux",Fr_linux);
    s.setValue("Colores/fr_kerberos",Fr_kerberos);
    s.setValue("Colores/fr_DB",Fr_DB);
    s.setValue("Colores/fr_TS",Fr_TS);
    s.setValue("Colores/fr_rutas",Fr_rutas);


    s.setValue("Correo/Para",ui->para->text());
    s.setValue("Correo/Asunto",ui->asunto->text());
    s.setValue("Correo/Cuerpo",ui->cuerpo->toPlainText());
    s.setValue("Correo/Para_Rutas",ui->para_rutas->text());
    s.setValue("Correo/Asunto_Rutas",ui->asunto_rutas->text());
    s.setValue("Correo/Cuerpo_Rutas",ui->cuerpo_rutas->toPlainText());


    s.setValue("Nmap/PuertosBuscados_ssh",ui->checkBox_SSH->isChecked());
    s.setValue("Nmap/PuertosBuscados_telnet",ui->checkBox_telnet->isChecked());
    s.setValue("Nmap/PuertosBuscados_web",ui->checkBox_web->isChecked());
    s.setValue("Nmap/PuertosBuscados_webssl",ui->checkBox_webssl->isChecked());
    s.setValue("Nmap/PuertosBuscados_portPrinter",ui->checkBox_portPrinter->isChecked());
    s.setValue("Nmap/PuertosBuscados_netbios",ui->checkBox_netbios->isChecked());

    s.setValue("ldap/Servidor_ldap", ui->lineEdit_ldap_servidorLdap->text());
    s.setValue("ldap/Puerto_ldap", ui->lineEdit_ldap_puerto->text());
    s.setValue("ldap/Usuario_ldap", ui->lineEdit_ldap_usuarioDominio->text());
    s.setValue("ldap/Clave_ldap", cifra->encryptToString(ui->lineEdit_ldap_clave->text()));
    s.setValue("ldap/lineEdit_OU",ui->lineEdit_OU->text());
    s.setValue("ldap/Dominio_ldap", ui->lineEdit_ldap_Dominio->text());

    valor(ui->treeView_OU->model(),&lista_OU);

    s.setValue("ldap/Numero_OU",QString::number(lista_OU.size()));
    if (lista_OU.size()>0)
        for (int i = 0; i < lista_OU.size(); i++){
            s.setValue("ldap/OU_"+QString::number(i+1), lista_OU[i]);
        }
    

   /* s.setValue("ldap/UsarOuExternos",ui->checkBox_recursos_externos->isChecked());
    s.setValue("ldap/UsarOuPerrera",ui->checkBox_recursos_perrera->isChecked());
    s.setValue("ldap/UsarOuCie",ui->checkBox_cie->isChecked());
    s.setValue("ldap/UsarOuCpd",ui->checkBox_CPD->isChecked());
    s.setValue("ldap/UsarOuAyuntamientos",ui->checkBox_ayuntamientos->isChecked());

*/
}

void Configuracion::on_Btn_Kerberos_clicked()
{
   actualiza_Kerberos();
}

void Configuracion::actualiza_Kerberos()
{
    QProcess *kinit= new QProcess;
    QProcess *klist= new QProcess;
    QString ruta_sh = "/bin/sh";
    QStringList arg_kinit,arg_klist,resultado;
    arg_kinit << "-c" << "echo "+cual_es_clave()+"| kinit "+cual_es_tecnico();
    arg_klist << "-c" << "klist";
    kinit->start(ruta_sh, arg_kinit);
    kinit->waitForFinished(-1);
    klist->start(ruta_sh, arg_klist);
    klist->waitForFinished(-1);
    QString sh_klist_stdout = klist->readAllStandardOutput();
    QString sh_klist_stderr = klist->readAllStandardError();
    QString sh_kinit_stdout = kinit->readAllStandardOutput();
    QString sh_kinit_stderr = kinit->readAllStandardError();
    ui->consola_kerberos->appendPlainText(sh_kinit_stdout);
    ui->consola_kerberos->appendPlainText(sh_klist_stdout);
    ui->consola_kerberos->appendPlainText(sh_klist_stderr);
    resultado = sh_kinit_stdout.split('\n',QString::SkipEmptyParts);
    delete kinit, klist;
}
void Configuracion::muestra_Kerberos()
{
    QProcess *klist= new QProcess;
    QString ruta_sh = "/bin/sh";
    QStringList arg_klist,resultado;
    arg_klist << "-c" << "klist";
    klist->start(ruta_sh, arg_klist);
    klist->waitForFinished(-1);
    QString sh_klist_stdout = klist->readAllStandardOutput();
    QString sh_klist_stderr = klist->readAllStandardError();
    ui->consola_kerberos->appendPlainText(sh_klist_stdout);
    ui->consola_kerberos->appendPlainText(sh_klist_stderr);
    delete klist;
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

void Configuracion::change_color(QWidget *objeto){
    QString tmp_color;
    QColor color = QColorDialog::getColor(Qt::blue, this );
        if( color.isValid() )
        {
          tmp_color = color.name();
          objeto->setStyleSheet("background-color:"+tmp_color+";");
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
void Configuracion::actualizaFechaDB(){
    ui->fechaActualizacionDB->setDate(QDate::currentDate());
    on_buttonBox_accepted();
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

void Configuracion::on_pB_tablasDB_clicked()
{
    BaseDatos *basedatos = new BaseDatos();
    basedatos->show();
}

//Función para crear una fila del treeview
QList<QStandardItem *> Configuracion::creaFila(const QString &first)
{
    QList<QStandardItem *> rowItems;
    rowItems << new QStandardItem(first);
    return rowItems;
}



void Configuracion::resultados(QStringList lista){

    QStandardItemModel *standardModel = new QStandardItemModel ;
    QList<QStandardItem *> fila;
    typedef QList<QStandardItem *> tipomio;
    QList<tipomio> listas;
    //añadiendo una fila al invisibleRootItem producimos un elemento raiz
    QStandardItem *item = standardModel->invisibleRootItem();
    int i=0,j;
    QStringList lista_OU;
    QStringList LOU;
    QString dominio=dominio_basedn();

    //Añadimos a lista los item que ya habia en el model si estan repes no lo mete
    valor(ui->treeView_OU->model(),&lista);

    for (int i = 0; i < lista.size(); ++i){
            LOU << lista.at(i).toLocal8Bit().constData();
    }

    foreach (const QString &strou, LOU) {
        lista_OU.insert(i,strou);
        fila =creaFila(strou);
        listas.insert(i,fila);
        bool encontrado=false;
        //recorremos las OU y vemos si quitandole la ultima parte OU=..., de la cadena
        //es igual a alguna de las que ya hay, si es así se crea como una rama de esta
        while (strou.remove(0,strou.indexOf(",")+1)!=dominio && !encontrado){
            for (j=0;j<lista_OU.count();j++){
                if (strou==lista_OU[j]){
                    listas[j].first()->appendRow(listas[i]);
                    j=lista_OU.count();
                    encontrado=true;
                }
             }
        }
        if (!encontrado)
            item->appendRow(listas[i]);
        i++;
    }


    ui->treeView_OU->setModel(standardModel);
    ui->treeView_OU->expandAll();
    //activamos multiselección en el arbol
    ui->treeView_OU->setSelectionMode(3);
}


void Configuracion::on_boton_recargar_clicked(){
    QStringList lista;
    BuscarOU *buscarOU = new BuscarOU(cual_es_servidor_ldap(),cual_es_puerto_ldap(),cual_es_usuario_ldap(),cual_es_dominio_ldap() ,cual_es_clave_ldap(),&lista);
   // connect (buscarOU,SIGNAL(resultadoListo(QStringList)),this,(SLOT(resultados(QStringList))));
    connect (buscarOU,SIGNAL(botonAceptar(QStringList)),this,(SLOT(resultados(QStringList))));
    buscarOU->show();

}

void Configuracion::on_pB_anadirOU_clicked()
{
    QStandardItemModel *model= ui->treeView_OU->model();
    QStandardItem *item = model->invisibleRootItem();
    item->appendRow(new QStandardItem(ui->lineEdit_OU_2->text()));
}

void Configuracion::on_pB_limpiar_clicked()
{
    QStandardItemModel *model= ui->treeView_OU->model();
    model->clear();
}
