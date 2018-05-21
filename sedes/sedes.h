#ifndef SEDES_H
#define SEDES_H

#include "lib/lib.h"
#include <QWidget>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QSqlQueryModel>
#include "QtDebug"
#include <QMessageBox>

#include "soporte/nmap_xml.h"
#include "configuracion/configuracion.h"


namespace Ui {
    class Sedes;
}

class Sedes : public QWidget {
    Q_OBJECT

    public:
        explicit Sedes(QWidget *parent = 0);
        ~Sedes();
        QSqlQueryModel *model = new QSqlQueryModel();
        QSqlQueryModel *model_tlf = new QSqlQueryModel();
        QSqlDatabase db_sqlite;
    private slots:

        void mascaraIP();

        void activa_botones();

        void cargaCombo();

        void carga_imagenes(QString municipio_nombreID, QString router);

        void checkBox_Enabled(bool estado);

        void clean_checkbox();

        void clear_comboBox();

        void consultaComarca(int id_comarca);

        void consultaMancomunidades(int id_municipio);

        void consultaNodo(const QString &nombre);

        void consultaPrograma(int id_nodo, const QString &anio);

        bool existeFichero(QString path);

        void insertaTexto(QString texto);

        void on_comboBox_concertaciones_anio_activated(const QString &arg1);

        void on_comboBox_todo_concertaciones_anio_activated(const QString &arg1);

        void on_comboBox_todo_municipio_mancomunidad_activated(const QString &arg1);

        void on_comboBox_top_nodo_activated(const QString &arg1);

        void on_comboBox_top_ip_activated(const QString &arg1);

        void on_comboBox_top_telefono_activated(const QString &arg1);

        void on_comboBox_top_extension_activated(const QString &arg1);

        void on_pB_linea_router_clicked();

        void on_pB_municipio_bandera_clicked();

        void on_pB_municipio_escudo_clicked();

        void on_pB_municipio_googleMaps_clicked();

        void on_pB_municipio_mapa_clicked();

        void on_pB_municipio_web_clicked();

        void on_pB_municipio_wikipedia_clicked();

        void on_pB_nodo_googleMaps_clicked();

        void on_pB_nodo_mapa_clicked();

        void on_pB_nodo_portalTransparencia_clicked();

        void on_pB_nodo_sede_clicked();

        void on_pB_nodo_tablon_clicked();

        void on_pB_nodo_web_clicked();

        void on_pB_nodo_wikipedia_clicked();

        void on_pB_todo_linea_router_clicked();

        void on_pB_todo_municipio_bandera_clicked();

        void on_pB_todo_municipio_comarca_clicked();

        void on_pB_todo_municipio_escudo_clicked();

        void on_pB_todo_municipio_mancomunidad_clicked();

        void on_pB_todo_nodo_googleMaps_clicked();

        void on_pB_todo_nodo_mapa_clicked();

        void on_pB_todo_nodo_portalTransparencia_clicked();

        void on_pB_todo_nodo_sede_clicked();

        void on_pB_todo_nodo_tablon_clicked();

        void on_pB_todo_nodo_web_clicked();

        void on_pB_todo_nodo_wikipedia_clicked();

private:
        Ui::Sedes *ui;
        QString modelo_router;
        QString municipio_nombreID;
        int id_nodo;
};

#endif // SEDES_H
