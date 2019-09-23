//*************************************************************************************
//CLASE que permite utilizar las librerias OpenLdap
//sirve de clase intermedia o intefaz entre ldap.h y el usuario
//permite consultar usuarios, grupos, dominios y Unidades O. de LDAP
//*************************************************************************************

#ifndef CLASE_LDAP_H
#define CLASE_LDAP_H

#include <QString>
#include <QVector>

//utilizamos las librerias OpenLdap
// ¡¡¡¡ siempre ponerlo antes del #include <ldap.h>
#define LDAP_DEPRECATED 1     // se usan las funciones antiguas de ldap
#include "usuarios/ldap.h"
#include "lib/lib.h"
#include "usuarios/usuario.h"

class clase_ldap
{

public:

   clase_ldap(QString *info);
   clase_ldap(QString host, int puerto, QString usuario,QString dominio, QString password, QString *info);

    QStringList Consultar_OU(QString *info);
    bool existe_atributo(QString cuenta_usuario, QString qstr_dn, QString atributo);
    usuario Consulta_Usuario(QString cuenta_usuario, QString qstr_dn, QString *info);

    QVector<usuario> Consulta_Usuarios(QString qstr_dn_OU, QString *info);
    void desbloquear_usuario(QString DN_usuario, QString *info);
    void renovar_clave_usuario(QString DN_usuario, QString *info);
    void habilitar_usuario(QString cuenta_usuario,QString DN_usuario, QString *info);
    //int estado_habilitado(int valor);
    void guardar_fecha_caduca(QString fecha, QString DN_usuario, QString *info);
    void miembro_grupo(QString DN_grupo, QString DN_usuario, QString *info);
    void quitar_grupo(QString DN_grupo, QString DN_usuario, QString *info);
    void guarda_otrosdatos(QStringList otrosdatos_valor, QString cuenta_usuario, QString DN_usuario, QString *info);

    void guarda_correo(QString correo, QString DN_usuario, QString *info);
    void guarda_telefono(QString telefono, QString DN_usuario, QString *info);
    void guarda_descripcion(QString descripcion, QString DN_usuario, QString *info);
    void guarda_debe_cambiar_pass(bool valor, QString DN_usuario, QString *info);
    void guarda_pass_nunca_expira(bool no_expira, QString cuenta_usuario, QString DN_usuario, QString *info);
    QStringList Consultar_grupos_dominio(QString *info);
    QStringList Consultar_grupos_OU(QString qstr_dn_OU, QString *info);

    ~clase_ldap();

private:

    QString Dominio;

    bool conecta_oldap( QString *info);
    LDAPMessage * consulta_oldap(char *filtro, char *attrs[], int attrsonly, const char * base_dn, int scope, QString *info);
    int num_entradas_oldap(LDAPMessage *resul_consul);
    bool rellena_dominio(QString *info);


    //fijamos maximo numero de resultados devueltos y otros valores de configuracion
    const int max_result=1000;
    const int ldap_version  = LDAP_VERSION3;
    //const int auth_method    = LDAP_AUTH_SIMPLE;

    LDAP * ldap=nullptr;

    //valores de conexión al servidor
    const char *ldap_host;
    int   ldap_port;
    const char *ldap_dn;
    const char *ldap_pw;

     struct domain{
         // inicializamos a -1 como marca de que no se ha consultado aun los datos del dominio en LDAP
         int intentos_fallidos=-1;// lockoutThreshold -> num de intentos fallidos para el bloqueo
         QString vigencia_clave;// maxPwdAge -> duracion de vigencia de contraseña
         QString tiempo_bloqueo;// lockoutDuration -> duración de bloqueo de usuario
     }dominio;

     usuario rellena_entrada(LDAPMessage *entry);

};

#endif // CLASE_LDAP_H
