#ifndef SOPORTE_H
#define SOPORTE_H
#include <QDialog>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QSqlQueryModel>
#include "QtDebug"
#include "nmap_xml.h"
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QToolBar>
namespace Ui {
class Soporte;
}

class Soporte : public QDialog
{
    Q_OBJECT

public:
    explicit Soporte(QWidget *parent = 0);
    ~Soporte();
    QSqlQueryModel *model = new QSqlQueryModel();
private slots:
    void on_lineEdit_ip_textChanged(const QString &arg1);

    void on_cb_sede_activated(const QString &nom);

    void on_Btn_Buscar_clicked();

    void on_Btn_Ping_clicked();

    void on_pushButton_4_clicked();

    void ejecuta_nmap();

    void resultados(QList<NMapScan>);

    void closeTab(int indice);

    void activa_barra_progreso();

    void desActiva_barra_progreso();

    void on_Btn_Limpiar_clicked();

    int valida_ip();

    void on_Btn_Mtr_clicked();

    void Ping();

    void on_Btn_Incidencia_clicked();

    void on_Btn_Atalaya_clicked();

   void resultado_html(QNetworkReply* p);

   void insertaTexto(QString texto);

   void on_pB_gmenu_clicked();

   void on_pB_portafirmas_clicked();

   void on_pB_gtablon_clicked();

   void on_pB_epol_clicked();

   void on_pB_guadaltel_clicked();

   void on_pB_perilContratante_clicked();

   void on_pB_citrix_clicked();

   void on_pB_aytos_clicked();

   void on_pB_isl_clicked();

   void on_pB_listin_clicked();

   void on_Btn_Glpi_clicked();

   void mascaraIP();

   void cargaSedes();

private:
    Ui::Soporte *ui;
    QSqlDatabase db;
    QSqlQuery consultar;
    QProcess *ping;
    QString terminal="x-terminal-emulator";
    int atalaya, glpi, ocs;

 };



#endif // SOPORTE_H
