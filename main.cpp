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
    Botonera w;
    w.setFixedSize(1100,135);
    w.show();
    return a.exec();
}
