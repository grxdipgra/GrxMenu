#ifndef BOTONERA_H
#define BOTONERA_H

#include <QMainWindow>
#include <QSettings>
#include <QStandardPaths>
#include "QProcess"
#include "configuracion.h"
#include "QtDebug"
#include <QtSql/QSqlDatabase>
#include "acerdade.h"


namespace Ui {
class Botonera;
}

class Botonera : public QMainWindow
{
    Q_OBJECT

public:
    QSqlDatabase db;
    explicit Botonera(QWidget *parent = 0);
    ~Botonera();
    QString path,home,user,GrxMenu;

private slots:

    void ctxMenu(const QPoint &pos);

    void test_slot();

    void on_actionUsuarios_triggered();

    void on_actionSalir_triggered();

    void on_actionCronos_triggered();

    void on_actionWebmail_triggered();

    void on_actionBeiro_triggered();

    void on_actionGlpi_triggered();

    void on_actionActiveDirectory_triggered();

    void on_actionISL_triggered();

    void on_actionConfigurar_triggered();

    void on_actionNuevo_Men_triggered();

    void on_actionSedes_triggered();

    void on_actionSoporte_triggered();

    bool cargaVariables();

    void muestraBotones();

    bool creaConexion();

    bool barraEstado();

    char* convierte(QString dato);

    bool basedatos();

    void on_actionAtalaya_triggered();

    void on_actionAcerca_de_triggered();

    void on_actionBorrame_triggered();

    void on_actionMame_triggered();

private:
    Ui::Botonera *ui;

};

#endif // BOTONERA_H
