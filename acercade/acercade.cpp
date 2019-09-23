#include "acercade.h"
#include "ui_acercade.h"
#include <QDesktopServices>
#include <QUrl>
AcercaDe::AcercaDe(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AcercaDe)
{
    ui->setupUi(this);
}

AcercaDe::~AcercaDe()
{
    delete ui;
}

/*!
 * \file acercade.cpp
 * \brief Esta clase muestra a los programadores de la aplicación
 * \author Alberto Avidad Fernandez
 * \date 18/09/2019
 */
void AcercaDe::on_pushButton_clicked()
{
    QDesktopServices::openUrl(QUrl("https://incidencias.dipgra.es/GrxMenu/grx-asistencia.deb"));
}

