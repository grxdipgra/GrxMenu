#ifndef PAGINAWEB_H
#define PAGINAWEB_H

#include <QDialog>
#include <QAuthenticator>
#include <QNetworkReply>
namespace Ui {
class paginaWeb;
}

class paginaWeb : public QDialog
{
    Q_OBJECT

public:
    explicit paginaWeb(QString url,int llama,int Tipo,QString centro,int ID, QWidget *parent = 0);
    ~paginaWeb();

private:
    Ui::paginaWeb *ui;
    int padre=0,contador=0,tipo=0,id=0;
    QString centro;
private slots:

void autenticacion(QUrl, QAuthenticator* auth);

void paginaCargada(bool carga);

void porcentaje(int porcentaje);

};
#endif // PAGINAWEB_H
