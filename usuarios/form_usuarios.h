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

    void rellena(QString consulta, QString base_DN);

    void on_comboBox_usuarios_activated(const QString &arg1);

    void on_comboBox_nombres_activated(const QString &arg1);

    void on_boton_desbloquear_clicked();

    bool carga_OU();

    LDAPMessage * consulta_oldap(char *filtro, char *attrs[], int attrsonly, const char * base_dn, int scope);

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





};

#endif // FORM_USUARIOS_H
