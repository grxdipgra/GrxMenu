#include "acerdade.h"
#include "ui_acerdade.h"
#include <QDesktopServices>
#include <QUrl>
AcerdaDe::AcerdaDe(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AcerdaDe)
{
    ui->setupUi(this);
}

AcerdaDe::~AcerdaDe()
{
    delete ui;
}

void AcerdaDe::on_pushButton_clicked()
{
    QDesktopServices::openUrl(QUrl("https://incidencias.dipgra.es/GrxMenu/grx-asistencia.deb"));
}

