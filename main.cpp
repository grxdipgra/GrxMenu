#include <QApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QSqlQueryModel>
#include <QTcpServer>
#include <QMessageBox>
#include "qthread.h"

#include "botonera/botonera.h"
#include "configuracion/configuracion.h"
#include "tunel.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("GrxMenu"),
                              QObject::tr("No se detecto ningún sistema de iconos en el sistema "));
        return 1;
    }
    QApplication::setQuitOnLastWindowClosed(false);
    Botonera w;
    w.show();
    return a.exec();
}
