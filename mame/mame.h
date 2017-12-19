#ifndef MAME_H
#define MAME_H

#include <QDialog>
#include "QProcess"
namespace Ui {
class Mame;
}

class Mame : public QDialog
{
    Q_OBJECT

public:
    explicit Mame(QWidget *parent = 0);
    QList <QString> ListaJuegos;
    ~Mame();
private slots:
    void juego(int i);

private:
    Ui::Mame *ui;
};

#endif // MAME_H
