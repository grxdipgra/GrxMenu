#ifndef EJECUTAHILO_H
#define EJECUTAHILO_H
#include <QThread>
#include <QObject>
#include "nmap_xml.h"

class ejecutaHilo : public QThread
{
Q_OBJECT
public:
   explicit ejecutaHilo(QString ip,QString opciones);
   void ejecuta();
   QString ip,opciones;

signals:
   void resultadoListo(QList<NMapScan>);
};

#endif // EJECUTAHILO_H
