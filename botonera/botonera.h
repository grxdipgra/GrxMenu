#ifndef BOTONERA_H
#define BOTONERA_H

#include <QMainWindow>
#include <QSettings>
#include <QStandardPaths>
#include "QProcess"
#include "QtDebug"
#include <QtSql/QSqlDatabase>
#include "configuracion/configuracion.h"
#include "acercade/acercade.h"
#include "tunel.h"
#include <QSystemTrayIcon>
#include <QAction>
#include <QCloseEvent>
#include <QSharedMemory>


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
    bool lock();
    bool conectadoMysql = false;

private slots:

    void compruebaDB();

    void unlock();

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

    bool cargaDominio();

    void muestraBotones();

    void on_actionAtalaya_triggered();

    void on_actionAcerca_de_triggered();

    void on_actionMame_triggered();

    void on_pb_kerberos_clicked();

    void showMessage();

    void iconActivated(QSystemTrayIcon::ActivationReason reason);

    void crearAcciones();

    void closeEvent(QCloseEvent *event);

    void crearTrayIcon();

    void on_actionactionVeleta_triggered();

    void on_actionBorrame_triggered();

    void on_actionNuevo_Citrix_triggered();

    void on_actionViejo_Citrix_triggered();

    void on_actionOcs_triggered();

    void on_actionLink_triggered();

    void on_actionApp_triggered();

    void on_actionVirtManager_triggered();

    void on_actionMailing_triggered();

private:
    Ui::Botonera *ui;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;


    QAction *soporteAcciones;
    QAction *usuariosAcciones;
    QAction *sedesAcciones;
    QAction *salirAccion;

    QSharedMemory *_singular;
};

#endif // BOTONERA_H
