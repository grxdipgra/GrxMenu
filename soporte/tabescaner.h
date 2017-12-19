#ifndef TABESCANER_H
#define TABESCANER_H

#include <QWidget>
#include "qlabel.h"
#include "qlineedit.h"
#include "qfileinfo.h"
#include "QVBoxLayout"
#include "nmap_xml.h"
class tabEscaner : public QWidget
{
    Q_OBJECT
public:
    explicit tabEscaner(NMapScan *nmapscan,QWidget *parent = nullptr);
    NMapScan *nmapscan;
    QList<Host> host_encontrados;
    int lineas = 6;// Numero de líneas de botones que se mostrarán
signals:

public slots:
    void equipoLinux(int i);
};

#endif // TABESCANER_H
