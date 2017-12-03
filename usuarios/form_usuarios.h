#ifndef FORM_USUARIOS_H
#define FORM_USUARIOS_H

#include <QWidget>

#define LDAP_DEPRECATED 1     // se usan las funciones antiguas de ldap
#include <ldap.h>

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

    void on_comboBox_usuarios_activated(const QString &arg1);

    void on_form_usuarios_destroyed();

    void on_comboBox_nombres_activated(const QString &arg1);

private:
    Ui::form_usuarios *ui;

};

#endif // FORM_USUARIOS_H
