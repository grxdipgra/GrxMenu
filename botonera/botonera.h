#ifndef BOTONERA_H
#define BOTONERA_H

#include <QMainWindow>
#include <QSettings>
#include <QStandardPaths>
#include "QProcess"
#include "QtDebug"
#include <QtSql/QSqlDatabase>
#include "configuracion/configuracion.h"
#include "acercade/acerdade.h"
#include "tunel.h"
#include <QSystemTrayIcon>
#include <QAction>
#include <QCloseEvent>

namespace Ui {
class Botonera;
}

class Botonera : public QMainWindow
{
    Q_OBJECT

public:
    QSqlDatabase db_sqlite,db_mysql;
    explicit Botonera(QWidget *parent = 0);
    ~Botonera();
    QString path,home,user,grxconf_ini;

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

    void barraEstado();

    void basedatos();

    void on_actionAtalaya_triggered();

    void on_actionAcerca_de_triggered();

    void on_actionMame_triggered();

    bool crearDB(QString rutaDB);

    bool actualizaDB(QString rutaDB);

    void on_pb_reconectaDB_clicked();

    void on_pushButton_clicked();

    void on_pb_kerberos_clicked();

    void crearAcciones();

    void closeEvent(QCloseEvent *event);

    void createTrayIcon();

    void showMessage();

    void iconActivated(QSystemTrayIcon::ActivationReason reason);


private:
    Ui::Botonera *ui;

    QAction *soporteAcciones;
    QAction *usuariosAcciones;
    QAction *sedesAcciones;
    QAction *salirAccion;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

};

#endif // BOTONERA_H
