#ifndef SEDES_H
#define SEDES_H

#define NUM_COL_MUNICIPIO_ID 0
#define NUM_COL_MUNICIPIO_IDCOMARCA 1
#define NUM_COL_MUNICIPIO_NOMBRE 3
#define NUM_COL_MUNICIPIO_NOMBREID 4
#define NUM_COL_MUNICIPIO_CIF 5
#define NUM_COL_MUNICIPIO_CODIGODIR3 6
#define NUM_COL_MUNICIPIO_CODIGOINE 7
#define NUM_COL_MUNICIPIO_NUMEROHABITANTES 8
#define NUM_COL_MUNICIPIO_SUPERFICIE 9
#define NUM_COL_MUNICIPIO_ALTITUD 10
#define NUM_COL_MUNICIPIO_LATITUD 11
#define NUM_COL_MUNICIPIO_LONGITUD 12
#define NUM_COL_MUNICIPIO_URLBANDERA 13
#define NUM_COL_MUNICIPIO_URLESCUDO 14
#define NUM_COL_MUNICIPIO_WEB 15
#define NUM_COL_MUNICIPIO_SIGLASPARTIDO 16
#define NUM_COL_MUNICIPIO_NOMBREPARTIDO 17
#define NUM_COL_MUNICIPIO_NOMBREALCALDE 18
#define NUM_COL_MUNICIPIO_FECHAELECCIONES 19
#define NUM_COL_MUNICIPIO_URLTABLON 20
#define NUM_COL_MUNICIPIO_URLPORTALTRANSPARENCIA 21

#define NUM_COL_NODO_ID 0
#define NUM_COL_NODO_IDPOBLACION 1
#define num_col_nodo_idCentro 2
#define NUM_COL_NODO_NOMBRE 3
#define NUM_COL_NODO_TIPOVIA 4
#define NUM_COL_NODO_NOMBREDIRECCION 5
#define NUM_COL_NODO_NUMERODIRECCION 6
#define NUM_COL_NODO_LETRADIRECCION 7
#define NUM_COL_NODO_ESCALERADIRECCION 8
#define NUM_COL_NODO_PISODIRECCION 9
#define NUM_COL_NODO_PUERTADIRECCION 10
#define NUM_COL_NODO_CODIGOPOSTAL 11
#define NUM_COL_NODO_LATITUD 12
#define NUM_COL_NODO_LONGITUD 13
#define NUM_COL_NODO_CONTACTO 14
#define NUM_COL_NODO_EXTENSION 15
#define NUM_COL_NODO_FAX 16
#define NUM_COL_NODO_WEB 17
#define NUM_COL_NODO_SEDE 18
#define NUM_COL_NODO_ADSLLINEA 19
#define NUM_COL_NODO_NUMADMINISTRATIVO 20
#define NUM_COL_NODO_IPLINEA 21
#define NUM_COL_NODO_IPCIFRADO 22
#define NUM_COL_NODO_SERVICIOLINEA 23
#define NUM_COL_NODO_CAUDAL 24
#define NUM_COL_NODO_EQUIPAMIENTO 25
#define NUM_COL_NODO_NUMEROSERIEROUTER 26
#define NUM_COL_NODO_ESAYUNTAMIENTO 27

#define NUM_COL_PROGRAMA_ID 0
#define NUM_COL_PROGRAMA_ANIO 1
#define NUM_COL_PROGRAMA_PORTALWEB 2
#define NUM_COL_PROGRAMA_EMAIL 3
#define NUM_COL_PROGRAMA_BASEDATOSJURIDICA 4
#define NUM_COL_PROGRAMA_SUSCRIPCIONDOMINIO 5
#define NUM_COL_PROGRAMA_PERFILCONTRATANTE 6
#define NUM_COL_PROGRAMA_GESTIONMUNICIPAL 7
#define NUM_COL_PROGRAMA_GESTIONECONOMICA 8
#define NUM_COL_PROGRAMA_SOPORTE 9
#define NUM_COL_PROGRAMA_SEDEELECTRONICA 10
#define NUM_COL_PROGRAMA_EPOL 11
#define NUM_COL_PROGRAMA_EPOLMOVIL 12
#define NUM_COL_PROGRAMA_SIAPOL 13

#define NUM_COL_POBLACION_ID 0
#define NUM_COL_POBLACION_IDMUNICIPIO 1
#define NUM_COL_POBLACION_IDELA 2
#define NUM_COL_POBLACION_NOMBRE 3
#define NUM_COL_POBLACION_NOMBREID 4

#define NUM_COL_TELEFONO_ID 0
#define NUM_COL_TELEFONO_TELEFONO 1

#define NUM_COL_CENTRO_ID 0
#define NUM_COL_CENTRO_NOMBRE 1
#define NUM_COL_CENTRO_DIRECCION 2
#define NUM_COL_CENTRO_TELEFONO 3

#define NUM_COL_MANCOMUNIDAD_ID 0
#define NUM_COL_MANCOMUNIDAD_NOMBRE 1

#define NUM_COL_COMARCA_ID 0
#define NUM_COL_COMARCA_NOMBRE 1

#define NUM_COL_EMAILNODO_ID 0
#define NUM_COL_EMAILNODO_EMAIL 1

#define NUM_COL_DIAFESTIVOPOBLACION_ID 0
#define NUM_COL_DIAFESTIVOPOBLACION_DATE 1
#define NUM_COL_DIAFESTIVOPOBLACION_DESCRIPCION 2


#define RUTA_IMAGENES "/usr/share/grx/asistencia/imagenes/"


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

        void on_comboBox_anio_activated(const QString &arg1);

        void on_comboBox_NODO_activated(const QString &arg1);

        void on_comboBox_IP_activated(const QString &arg1);

        void on_pB_googleMaps_clicked();

        void on_pB_municipio_bandera_clicked();

        void on_pB_municipio_escudo_clicked();

        void on_pB_municipio_mapa_clicked();

        void on_pB_municipio_web_clicked();

        void on_pB_nodo_mapa_clicked();

        void on_pB_nodo_web_clicked();

        void on_pB_portalTransparencia_clicked();

        void on_pB_router_clicked();

        void on_pB_sede_clicked();

        void on_pB_tablon_clicked();

        void on_pB_wikipedia_clicked();

        void on_pB_escudo_2_clicked();

        void on_pB_bandera_2_clicked();

        void on_pB_wikipedia_2_clicked();

        void on_pB_web_clicked();

        void on_pB_sede_2_clicked();

        void on_pB_tablon_2_clicked();

        void on_pB_portalTransparencia_2_clicked();

        void on_pB_mapa_2_clicked();

        void on_comboBox_anio_2_activated(const QString &arg1);

        void on_comboBox_mancomunidad_2_activated(const QString &arg1);

        void on_pB_mancomunidad_clicked();

        void on_pB_comarca_clicked();

        void on_comboBox_TLF_activated(const QString &arg1);

        void on_comboBox_extension_activated(const QString &arg1);

private:
        Ui::Sedes *ui;
        QString modelo_router;
        QString municipio_nombreID;
        int id_nodo;
};

#endif // SEDES_H
