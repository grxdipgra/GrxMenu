#include <QApplication>
#include <QMessageBox>
#include "botonera/botonera.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("GrxMenu"),
                              QObject::tr("No se detecto ningún sistema de iconos en el sistema "));
        return 1;
    }
    QApplication::setQuitOnLastWindowClosed(false);
    QIcon icono(":/iconos/logo.png");
    QApplication::setWindowIcon(icono);
    Botonera w;
    // Para bloquear la ejecucion multiple
    if(!w.lock()){
        QMessageBox::critical(0, QObject::tr("GrxMenu"),QObject::tr("Ya existe una instancia de la aplicación ejecutándose"));
        return -42;
    }else {
        w.show();
    }
    return a.exec();
}
