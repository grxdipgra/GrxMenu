#ifndef FORM_USUARIOS_H
#define FORM_USUARIOS_H

#include <QWidget>

#define LDAP_DEPRECATED 1     // se usan las funciones antiguas de ldap
#include <ldap.h>


#include <QMessageBox>
#include <QProcess>
#include <QDebug>
#include <QTextStream>
#include <QFile>
#include <QDateTime>

#include <ldif.h>
#include <stdio.h>
#include <stdlib.h>

#include "ui_form_usuarios.h"
#include "configuracion/configuracion.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QSqlQueryModel>

namespace Ui {
class form_usuarios;
}

class form_usuarios : public QWidget
{
    Q_OBJECT

public:
    explicit form_usuarios(QWidget *parent = 0);
    ~form_usuarios();

private slots:

    void form_usuarios::clear_text();

    char* form_usuarios::convierte(QString dato);

    bool conecta_oldap();

    void desconecta_oldap();

    int num_entradas_oldap(LDAPMessage *resul_consul);

    void on_comboBox_usuarios_activated(const QString &arg1);

    void on_comboBox_nombres_activated(const QString &arg1);

    void on_boton_desbloquear_clicked();

    void limpia_entrada();

    bool carga_OU();

    void carga_datos_usuario(int tipo, QString filtro);

    LDAPMessage * consulta_oldap(char *filtro, char *attrs[], int attrsonly, const char * base_dn, int scope);

    void on_boton_renovar_clicked();

    void actualizar_usuarios();

    void on_boton_actualiza_usuarios_clicked();

    void actualiza_usuario();

    void on_boton_actualiza_usuario_clicked();

    void rellena_entrada(LDAPMessage *entry);


private:
    Ui::form_usuarios *ui;

    //fijamos maximo numero de resultados devueltos
    int max_result=999;


    // lista de Unidades Organizativas que se van a ir consultando
    QStringList OU;

    // aqui guardamos el par usuario basedn separado por $ para que podamos saber en que UO está el usuario
    // y no tengamos que buscar en el dominio grx entero
    // para acelerar la busqueda a la hora de consultar los datos del usuario
    QStringList usuario_basedn;
    QStringList usuario_basedn2;
    QString DN; //guardamos la cadena dn del objeto actual

    struct entry{
        int id_entry;
       QString usuario;
        QString nombre;
        QString cambio_clave;
        QString clave;//no hace falta no se guarda en la BD
        QString correo;
        QString creada;
        QString estado;
        QString fecha_correo;
        int intentos;
        int logon;
        QString modificacion_cuenta;
        QString telefono;
        QString ultimo_login;
        QString descripcion;
        QString caduca_clave;
        QString caduca_cuenta;
        QString dn;
    }entrada;

    struct group{
        int id_usuario;
        int id_grupo;
        QString usuario;
        QString nombre;
    }grupos;
    QVector<group> vec_grupos;

};

#endif // FORM_USUARIOS_H
