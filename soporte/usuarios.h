#ifndef USUARIOS_H
#define USUARIOS_H

#include <QFrame>

namespace Ui {
class Usuarios;
}

class Usuarios : public QFrame
{
    Q_OBJECT

public:
    explicit Usuarios(QWidget *parent = 0);
    ~Usuarios();

private:
    Ui::Usuarios *ui;
};

#endif // USUARIOS_H
