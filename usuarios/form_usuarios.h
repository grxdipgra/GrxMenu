#ifndef FORM_USUARIOS_H
#define FORM_USUARIOS_H

#include <QWidget>

#include "clase_ldap.h"

#include <QMessageBox>
#include <QProcess>
#include <QDebug>
#include <QTextStream>
#include <QFile>
#include <QDateTime>
#include <QEvent>
#include <ldif.h>
#include <stdio.h>
#include <stdlib.h>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQueryModel>
#include "ui_form_usuarios.h"
#include "configuracion/configuracion.h"
#include "usuarios/usuario.h"
#include <basedatos/basedatos.h>
#include "lib/lib.h"
#include "lib/globals.h"
#include "usuarios/form_grupos.h"
#include "usuarios/clase_ldap.h"
//para evitar dependencia cíclica, porque en form_grupos tambien hay include de form_usuarios,
// indicamos al compilador que la clase existe
//y será definida más adelante, hay que definir class form_grupos antes de usarla
class form_grupos;

namespace Ui {
class form_usuarios;
}

class form_usuarios : public QWidget
{
    Q_OBJECT


public:
    explicit form_usuarios(QWidget *parent = 0);
    ~form_usuarios();

    //para usarse en formulario_grupos
    void refresca_grupos();
    QStringList lee_grupos();


private slots:

    void clear_text();

    void on_comboBox_usuarios_activated(const QString &arg1);

    void on_comboBox_nombres_activated(const QString &arg1);

    void carga_datos_usuario(int tipo, QString filtro);

    void on_boton_actualiza_usuarios_clicked();

    void actualiza_usuario();

    void on_boton_actualiza_usuario_clicked();

    void on_boton_desbloquear_clicked();

    void on_boton_habilitar_clicked();

    void on_boton_renovar_clicked();

    void on_dateTEdit_cuenta_caduca_dateTimeChanged(const QDateTime &dateTime);

    void on_dateTEdit_cuenta_caduca_editingFinished();

    void on_boton_guardar_clicked();

    void on_boton_grupo_clicked();

    void on_checkCaduca_clicked();

    void on_text_correo_textEdited(const QString &arg1);

    void on_text_telefono_textEdited(const QString &arg1);

    void on_text_descripcion_textChanged();

    void on_text_cuenta_caduca_textChanged(const QString &arg1);

    void closeEvent(QCloseEvent *event);

    void on_checkBox_cambiar_pass_clicked();

    void on_checkBox_pass_nunca_expira_clicked();

    void on_boton_sacar_clicked();

    void on_text_usuario_textChanged(const QString &arg1);

    void on_text_usuariow2000_textChanged(const QString &arg1);

    void on_text_nombre_textChanged(const QString &arg1);

    void on_text_apellidos_textChanged(const QString &arg1);

    void on_text_nombremostrar_textChanged(const QString &arg1);

    void on_text_movil_textChanged(const QString &arg1);

    void on_text_fax_textChanged(const QString &arg1);

    void on_text_departamento_textChanged(const QString &arg1);

    void on_text_oficina_textChanged(const QString &arg1);

    void on_text_organizacion_textChanged(const QString &arg1);

    void on_text_puesto_textChanged(const QString &arg1);

    void on_text_provincia_textChanged(const QString &arg1);

    void on_text_localidad_textChanged(const QString &arg1);

    void on_text_direccion_textChanged(const QString &arg1);

    void on_text_cp_textChanged(const QString &arg1);

    void on_text_notas_textChanged();

    void guarda_datos_1();

    void guarda_datos_2();


private:

    Ui::form_usuarios *ui;

    QString host;
    int  puerto;
    QString usuarioLdap;
    QString dominio;
    QString password;
    QSqlDatabase db_sqlite;
    int id_usuario;//id del usuario actual
    QString us_usuario;//username del usuario actual
    QString info;

    // lista de Unidades Organizativas que se van a ir consultando
    QStringList OU;
    //QStringList subUO;

    // aqui guardamos el par usuario basedn separado por $ para que podamos saber en que UO está el usuario
    // y no tengamos que buscar en el dominio entero
    // para acelerar la busqueda a la hora de consultar los datos del usuario
    //QStringList usuario_basedn;
    //QStringList usuario_basedn2;
    QString DN; //guardamos la cadena dn del objeto actual
    int uac; //guardamos el valor useraccountcontrol del objeto actual

    bool cambios_1=false;
    bool cambios_2=false;

    QDateTime ultima_sincro;

    QStringList grupos;//para pasar datos entre este y el formulario grupos
    QStringList nuevos_grupos;//grupos que se debarán añadir al usuario

    //lo usamos para llamar a las funciones de form_grupos
    form_grupos *ventana_grupos;

};

#endif // FORM_USUARIOS_H

















