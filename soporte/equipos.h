#ifndef EQUIPOS_H
#define EQUIPOS_H
#include "nmap_xml.h"
#include <QWidget>
#include <QCoreApplication>
#include <QtCore/QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkCookie>
#include <QHttpPart>
#include <QDebug>
#include "configuracion/configuracion.h"
#include <QAuthenticator>
namespace Ui {
class Equipos;
}

class Equipos : public QWidget
{
    Q_OBJECT

public:
    explicit Equipos(Host *host,QWidget *parent = 0);
    QList <QString> host_ports_open(Host *host);
    ~Equipos();

private slots:
    void autenticando(QNetworkReply *aReply,QAuthenticator *aAuthenticator);

    int puerto_libre();

    int host_ports_open_int(Host *host);

    void on_pB_carpeta_clicked();

    void on_pB_web_clicked();

    void on_pB_webssl_clicked();

    void on_pB_telnet_clicked();

    void on_pB_ssh_clicked();

    void on_pB_systemconfig_clicked();

    void on_pB_networkManager_clicked();

    void on_pB_cups_clicked();

    void on_pB_vnc_clicked();

    void on_pushButton_12_clicked();

    void botonesActivos(QString puertos);

    void desactivaBotones();

    void on_pB_update_clicked();

    void on_pB_creaIncidencia_clicked();

    void on_pB_ISL_clicked();

    QString glpi_Login(QByteArray glpi);

    void resultado(QNetworkReply *reply);

private:
    Ui::Equipos *ui;
    QString ip;
    Host *host_tmp;
};

#endif // EQUIPOS_H
