#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include <QtGlobal>
#include <QDialog>
#include <QSettings>
#include <QColorDialog>
#include <QFileDialog>
#include <QDialogButtonBox>
#include "simplecrypt.h"

namespace Ui {
class Configuracion;
}

class Configuracion : public QDialog
{
    Q_OBJECT

public:
    explicit Configuracion(QWidget *parent = 0);



    QString cual_es_home();
    QString cual_es_ruta_grxmenu_config();
    QString cual_es_path();
    QString cual_es_ini();
    QString cual_es_tecnico();
    QString cual_es_clave();
    QString cual_es_correo();
    QString cual_es_servidorAD();
    QString cual_es_usuarioAD();
    QString cual_es_claveAD();
    QString cual_es_usuario_remoto();
    QString cual_es_beiro();
    QString cual_es_DataBaseName();
    QString cual_es_hostnameDB();
    QString cual_es_usernameDB();
    QString cual_es_passwordDB();
    int cual_es_PuertoDB();
    QString cual_es_usuario_ssh();
    QString cual_es_password_ssh();
    QString cual_es_servidorSSH();
    QString cual_es_usuarioSSH();
    int cual_es_puerto_remoto_ssh();
    int cual_es_puerto_local_ssh();
    QString cual_es_isl();
    QString cual_es_puerto();
    QString cual_es_ocs();
    QString cual_es_glpi();
    QString cual_es_cronos();
    QString cual_es_atalaya();
    QString cual_es_clave_cifrado();
    QString cual_es_clave_remoto();
    QString cual_es_proxychains();
    QString cual_es_usuario_logado();
    QString puertos_buscados();
    QString cual_es_ruta_sqlite();

    bool es_rdesktop();
    bool usarSSH();
    bool usarproxy_chains();
    bool usuarios_up();
    bool soporte_up();
    bool sedes_up();
    bool cronos_up();
    bool webmail_up();
    bool beiro_up();
    bool glpi_up();
    bool ocs_up();
    bool ts_up();
    bool isl_up();
    bool atalaya_up();
    bool solo_aytos();
    bool puertoSSH();
    bool puertoTelnet();
    bool puertoWeb();
    bool puertoWebssl();
    bool puertoPortPrinter();
    bool puertoNetbios();
    QString cual_es_lineEditSSH();
    bool cual_es_rdesktop();
    QString cual_es_resolucion();
    QString cual_es_keyfile_privada();
    QString cual_es_keyfile_publica();
    bool createConnection();
    QString cual_es_para();
    QString cual_es_asunto();
    QString cual_es_cuerpo();

    //Ldap
    QString  cual_es_servidor_ldap();
    int  cual_es_puerto_ldap();
    QString  cual_es_usuario_ldap();
    QString  cual_es_clave_ldap();

    bool usar_ou_externos();
    bool usar_ou_perrera();
    bool usar_ou_cie();
    bool usar_ou_cpd();
    bool usar_ou_ayuntamientos();
    bool lineEdit_OU_vacio();
    QString lineEdit_OU_datos();
    ~Configuracion();

private slots:


    void ctxMenu(const QPoint &pos);

    void help();

    void color_widget(QObject *sender);

    void mascara_ip();

    void mascara_puertos_nmap();

    void mascara_puerto_remoto();

    void on_buttonBox_accepted();

    void on_Btn_Kerberos_clicked();

    void carga_configuracion();

    void carga_configuracion_color();

    void change_color(QWidget *objeto);

    void on_PB_linux_clicked();

    void on_PB_rutas_clicked();

    void on_PB_TS_clicked();

    void on_PB_DB_clicked();

    void on_PB_kerberos_clicked();

    void on_lb_rutas_linkActivated(const QString &link);

    void on_Btn_lupa_clicked();

    void deshabilitaSSH();

    void habilitaSSH();

    void habilitaProxyChains();

    void deshabilitaProxyChains();

    void on_checkBox_ssh_clicked();

    void on_Btn_lupa_2_clicked();

    void valoresPorDefecto();

    void carga_editLine();

    void on_Btn_lupa_3_clicked();

    void on_checkBox_proxychains_toggled(bool checked);

    void on_buttonBox_clicked(QAbstractButton *button);

    void on_pushButton_clicked();

    void on_pB_tablasDB_clicked();

    void on_pB_actualiza_DB_clicked();

private:
 SimpleCrypt *cifra = new SimpleCrypt(Q_UINT64_C(0x0c2ad4a4acb9f023)); //lo usamos para cifrar las claves
    QString home_usuario;
    QString Tecnico;
    QString Correo;
    QString Clave;
    QString ServidorAD;
    QString UsuarioAD;
    QString ClaveAD;
    QString UsuarioRemoto;
    QString Puerto;
    QString ISL;
    QString OCS;
    QString GLPI;
    QString Cronos;
    QString Beiro;
    QString Atalaya;
    QString DataBaseName;
    QString HostName;
    QString UserName;
    QString PasswordDB;
    QString Path;
    QString RutaConfigGrxmenu;
    QString RutaSqlite;
    bool UsarSSH;
    //QString UsarUsuarios;
    bool UsarUsuarios;
    bool UsarSoporte;
    bool UsarSedes;
    bool UsarCronos;
    bool UsarWebmail;
    bool UsarBeiro;
    bool UsarGlpi;
    bool UsarOCS;
    bool UsarTS;
    bool UsarISL;
    bool UsarAtalaya;
    QString ServidorSSH;
    QString UsuarioSSH;
    QString ClaveSSH;
    int PuertoDB;
    int PuertoRemotoSSH;
    int PuertoLocalSSH;
    QString Password;
    QString ClaveCifrado;
    QString ClaveRemoto;
    bool Rdesktop;
    QString Resolucion;
    QString Fr_linux;
    QString Fr_rutas;
    QString Fr_DB;
    QString Fr_TS;
    QString Fr_kerberos;
    QString KeyFile_privada;
    QString KeyFile_publica;
    QString Para;
    QString Asunto;
    QString Cuerpo;
    bool UsarProxyChains;
    QString ProxyChains;
    bool SoloAytos;

    bool PuertosBuscados_ssh;
    bool PuertosBuscados_telnet;
    bool PuertosBuscados_web;
    bool PuertosBuscados_webssl;
    bool PuertosBuscados_portPrinter;
    bool PuertosBuscados_netbios;
    QString PuertosBuscados_lineEdit;

    bool UsarOuExternos;
    bool UsarOuPerrera;
    bool UsarOuCie;
    bool UsarOuCpd;
    bool UsarOuAyuntamientos;
    QString lineEdit_OU;



    QString Servidor_ldap;
    int Puerto_ldap;
    QString Usuario_ldap;
    QString Clave_ldap;



    Ui::Configuracion *ui;
};


#endif // CONFIGURACION_H
