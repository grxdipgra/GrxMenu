#ifndef FORM_GRUPOS_H
#define FORM_GRUPOS_H

#include <QDialog>
#include "clase_ldap.h"
#include "usuarios/form_usuarios.h"
#include "configuracion/configuracion.h"
#include "lib/globals.h"


//para evitar dependencia cíclica indicamos al compilador que la clase existe
//y será definida más adelante, hay que definir class form_grupos antes de usarla
class form_usuarios;

namespace Ui {
class form_grupos;
}

class form_grupos : public QDialog
{
    Q_OBJECT

public:

    //modificamos el constructor pasandole el
    explicit form_grupos(QWidget *parent= 0);
    ~form_grupos();

    //para usarse en formulario_usuarios
    QStringList obtenerLista();

private slots:

    void on_buttonBox_accepted();

    void on_radioButton_GruposDom_clicked();

    void on_radioButton_GruposOU_clicked();

    void on_checkBox_clicked();

private:
    QString host;
    int  puerto;
    QString usuario;
    QString dominio;
    QString password;

    Ui::form_grupos *ui;

    QStringList grupos_temp;//grupos seleccionables
    QStringList grupos;//grupos que se van a devolver

    //lo usamos para llamar a las funciones de form_usuarios
    form_usuarios *ventana_usuarios;

};

#endif // FORM_GRUPOS_H
