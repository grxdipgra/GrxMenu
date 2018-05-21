#include <QSqlQuery>
#include <QHostAddress>
#include <QDesktopServices>
#include "qdebug.h"
#include "sedes.h"
#include "ui_sedes.h"
#include "basedatos/basedatos.h"


Sedes::Sedes(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Sedes)
{ //Constructor
    id_nodo = 0;
    ui->setupUi(this);
    ui->checkBox_nodo_esAyuntamiento->hide();
    mascaraIP();
    cargaCombo(); //Cargamos las sedes en los combobox
    checkBox_Enabled(false);
}


Sedes::~Sedes() {
    delete ui;
}


void Sedes::mascaraIP(){
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ipRegex ("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    ui->comboBox_top_ip->setValidator(ipValidator);
}


bool Sedes::existeFichero(QString path) {
    //Comprueba si existe y si es un fichero
    QFileInfo check_file(path);
    return (check_file.exists() && check_file.isFile());

}


void Sedes::checkBox_Enabled(bool estado) {
    ui->checkBox_concertaciones_basedatos_juridica->setEnabled(estado);
    ui->checkBox_concertaciones_correo->setEnabled(estado);
    ui->checkBox_concertaciones_epol->setEnabled(estado);
    ui->checkBox_concertaciones_epol_movil->setEnabled(estado);
    ui->checkBox_concertaciones_gestion_economica->setEnabled(estado);
    ui->checkBox_concertaciones_gestion_municipal->setEnabled(estado);
    ui->checkBox_concertaciones_perfil_contratante->setEnabled(estado);
    ui->checkBox_concertaciones_portal_web->setEnabled(estado);
    ui->checkBox_concertaciones_sede_electronica->setEnabled(estado);
    ui->checkBox_concertaciones_siapol->setEnabled(estado);
    ui->checkBox_concertaciones_soporte->setEnabled(estado);
    ui->checkBox_concertaciones_suscripcion->setEnabled(estado);

    ui->checkBox_todo_concertaciones_basedatos_juridica->setEnabled(estado);
    ui->checkBox_todo_concertaciones_correo->setEnabled(estado);
    ui->checkBox_todo_concertaciones_epol->setEnabled(estado);
    ui->checkBox_todo_concertaciones_epol_movil->setEnabled(estado);
    ui->checkBox_todo_concertaciones_gestion_economica->setEnabled(estado);
    ui->checkBox_todo_concertaciones_gestion_municipal->setEnabled(estado);
    ui->checkBox_todo_concertaciones_perfil_contratante->setEnabled(estado);
    ui->checkBox_todo_concertaciones_portal_web->setEnabled(estado);
    ui->checkBox_todo_concertaciones_sede_electronica->setEnabled(estado);
    ui->checkBox_todo_concertaciones_siapol->setEnabled(estado);
    ui->checkBox_todo_concertaciones_soporte->setEnabled(estado);
    ui->checkBox_todo_concertaciones_suscripcion->setEnabled(estado);
}


void Sedes::clean_checkbox() {
    ui->checkBox_concertaciones_portal_web->setCheckState((Qt::CheckState)0);
    ui->checkBox_concertaciones_correo->setCheckState((Qt::CheckState)0);
    ui->checkBox_concertaciones_basedatos_juridica->setCheckState((Qt::CheckState)0);
    ui->checkBox_concertaciones_gestion_municipal->setCheckState((Qt::CheckState)0);
    ui->checkBox_concertaciones_suscripcion->setCheckState((Qt::CheckState)0);
    ui->checkBox_concertaciones_perfil_contratante->setCheckState((Qt::CheckState)0);
    ui->checkBox_concertaciones_gestion_economica->setCheckState((Qt::CheckState)0);
    ui->checkBox_concertaciones_soporte->setCheckState((Qt::CheckState)0);
    ui->checkBox_concertaciones_sede_electronica->setCheckState((Qt::CheckState)0);
    ui->checkBox_concertaciones_epol->setCheckState((Qt::CheckState)0);
    ui->checkBox_concertaciones_epol_movil->setCheckState((Qt::CheckState)0);
    ui->checkBox_concertaciones_siapol->setCheckState((Qt::CheckState)0);
    ui->checkBox_concertaciones_portal_web->setStyleSheet("color:black");
    ui->checkBox_concertaciones_correo->setStyleSheet("color:black");
    ui->checkBox_concertaciones_basedatos_juridica->setStyleSheet("color:black");
    ui->checkBox_concertaciones_gestion_municipal->setStyleSheet("color:black");
    ui->checkBox_concertaciones_suscripcion->setStyleSheet("color:black");
    ui->checkBox_concertaciones_perfil_contratante->setStyleSheet("color:black");
    ui->checkBox_concertaciones_gestion_economica->setStyleSheet("color:black");
    ui->checkBox_concertaciones_soporte->setStyleSheet("color:black");
    ui->checkBox_concertaciones_sede_electronica->setStyleSheet("color:black");
    ui->checkBox_concertaciones_epol->setStyleSheet("color:black");
    ui->checkBox_concertaciones_epol_movil->setStyleSheet("color:black");
    ui->checkBox_concertaciones_siapol->setStyleSheet("color:black");

    ui->checkBox_todo_concertaciones_portal_web->setCheckState((Qt::CheckState)0);
    ui->checkBox_todo_concertaciones_correo->setCheckState((Qt::CheckState)0);
    ui->checkBox_todo_concertaciones_basedatos_juridica->setCheckState((Qt::CheckState)0);
    ui->checkBox_todo_concertaciones_gestion_municipal->setCheckState((Qt::CheckState)0);
    ui->checkBox_todo_concertaciones_suscripcion->setCheckState((Qt::CheckState)0);
    ui->checkBox_todo_concertaciones_perfil_contratante->setCheckState((Qt::CheckState)0);
    ui->checkBox_todo_concertaciones_gestion_economica->setCheckState((Qt::CheckState)0);
    ui->checkBox_todo_concertaciones_soporte->setCheckState((Qt::CheckState)0);
    ui->checkBox_todo_concertaciones_sede_electronica->setCheckState((Qt::CheckState)0);
    ui->checkBox_todo_concertaciones_epol->setCheckState((Qt::CheckState)0);
    ui->checkBox_todo_concertaciones_epol_movil->setCheckState((Qt::CheckState)0);
    ui->checkBox_todo_concertaciones_siapol->setCheckState((Qt::CheckState)0);
    ui->checkBox_todo_concertaciones_portal_web->setStyleSheet("color:black");
    ui->checkBox_todo_concertaciones_correo->setStyleSheet("color:black");
    ui->checkBox_todo_concertaciones_basedatos_juridica->setStyleSheet("color:black");
    ui->checkBox_todo_concertaciones_gestion_municipal->setStyleSheet("color:black");
    ui->checkBox_todo_concertaciones_suscripcion->setStyleSheet("color:black");
    ui->checkBox_todo_concertaciones_perfil_contratante->setStyleSheet("color:black");
    ui->checkBox_todo_concertaciones_gestion_economica->setStyleSheet("color:black");
    ui->checkBox_todo_concertaciones_soporte->setStyleSheet("color:black");
    ui->checkBox_todo_concertaciones_sede_electronica->setStyleSheet("color:black");
    ui->checkBox_todo_concertaciones_epol->setStyleSheet("color:black");
    ui->checkBox_todo_concertaciones_epol_movil->setStyleSheet("color:black");
    ui->checkBox_todo_concertaciones_siapol->setStyleSheet("color:black");
}


void Sedes::clear_comboBox() {
    ui->comboBox_top_ip->clear();

    ui->comboBox_municipio_mancomunidad->clear();
    ui->comboBox_todo_municipio_mancomunidad->clear();

    ui->comboBox_nodo_telefonos->clear();
    ui->comboBox_todo_nodo_telefonos->clear();
}


void Sedes::cargaCombo() {
    Configuracion *configuracion = new Configuracion();
    db_sqlite = QSqlDatabase::database("sqlite");
    QSqlQuery* query = new QSqlQuery(db_sqlite);
    QSqlQuery* query_tlf = new QSqlQuery(db_sqlite);
    QString sql;
    QString sql_tlf;

    if (configuracion->solo_aytos()) {
        sql = "SELECT nombre,ipLinea,codigoPostal,extension,id FROM nodo WHERE esAyuntamiento=1 ORDER BY nombre";
        sql_tlf = "SELECT idNodo,telefono FROM nodo N, telefononodo T WHERE N.id=T.idNodo AND esAyuntamiento=1";

    }
    else {
        sql = "SELECT nombre,ipLinea,codigoPostal,extension,id FROM nodo ORDER BY nombre";
        sql_tlf = "SELECT * FROM telefononodo";
    }

    query->prepare(sql);
    query_tlf->prepare(sql_tlf);

    if (!query->exec()) {
        QMessageBox::critical(this, "Sql Error", "Error en la consulta: \n" + query->lastError().text(), QMessageBox::Ok);
    }
    else {
        model->setQuery(*query);
        ui->comboBox_top_ip->setModel(model);
        ui->comboBox_top_ip->setModelColumn(1);
        ui->comboBox_top_nodo->setModel(model);
        on_comboBox_top_nodo_activated(ui->comboBox_top_nodo->itemText(0));
        ui->comboBox_top_nodo->setFocus();
        ui->comboBox_top_extension->setModel(model);
        ui->comboBox_top_extension->setModelColumn(3);
    }

    if (!query_tlf->exec()) {
        QMessageBox::critical(this, "Sql Error", "Error en la consulta: \n" + query_tlf->lastError().text(), QMessageBox::Ok);
    }
    else {
        model_tlf->setQuery(*query_tlf);
        ui->comboBox_top_telefono->setModel(model_tlf);
        ui->comboBox_top_telefono->setModelColumn(1);
    }

    delete query;
    delete query_tlf;
    delete configuracion;
}


void Sedes::consultaNodo(const QString &nombre) {
    QSqlQuery consultar_nodo(db_sqlite);
    QSqlQuery consultar_telefono(db_sqlite);
    QSqlQuery consultar_email(db_sqlite);
    QSqlQuery consultar_poblacion(db_sqlite);
    QSqlQuery consultar_municipio(db_sqlite);
    QSqlQuery consultar_ela(db_sqlite);
    QString anio_programa;
    QString idEla;
    QString nombreEla;
    int id_municipio;
    int id_comarca;

    ui->textEdit_consola->clear(); //limpia la consola

    consultar_nodo.prepare(QString("SELECT * FROM nodo WHERE nombre = :nombre"));
    consultar_nodo.bindValue(":nombre", nombre);

    if (consultar_nodo.exec() and consultar_nodo.first()) {
        clean_checkbox();
        clear_comboBox();

        id_nodo = consultar_nodo.value(NUM_COL_NODO_ID).toInt();
        anio_programa = ui->comboBox_concertaciones_anio->currentText();

        // Cargar datos del nodo seleccionado
        ui->checkBox_nodo_esAyuntamiento->setCheckState((Qt::CheckState)consultar_nodo.value(NUM_COL_NODO_ESAYUNTAMIENTO).toInt());

        ui->lineEdit_nodo_tipoVia->setText(consultar_nodo.value(NUM_COL_NODO_TIPOVIA).toString());
        ui->lineEdit_todo_nodo_tipoVia->setText(consultar_nodo.value(NUM_COL_NODO_TIPOVIA).toString());

        ui->lineEdit_nodo_direccion->setText(consultar_nodo.value(NUM_COL_NODO_NOMBREDIRECCION).toString());
        ui->lineEdit_todo_nodo_direccion->setText(consultar_nodo.value(NUM_COL_NODO_NOMBREDIRECCION).toString());

        ui->lineEdit_nodo_numero->setText(consultar_nodo.value(NUM_COL_NODO_NUMERODIRECCION).toString());
        ui->lineEdit_todo_nodo_numero->setText(consultar_nodo.value(NUM_COL_NODO_NUMERODIRECCION).toString());

        ui->lineEdit_nodo_letra->setText(consultar_nodo.value(NUM_COL_NODO_LETRADIRECCION).toString());
        ui->lineEdit_todo_nodo_letra->setText(consultar_nodo.value(NUM_COL_NODO_LETRADIRECCION).toString());

        ui->lineEdit_nodo_piso->setText(consultar_nodo.value(NUM_COL_NODO_PISODIRECCION).toString());
        ui->lineEdit_todo_nodo_piso->setText(consultar_nodo.value(NUM_COL_NODO_PISODIRECCION).toString());

        ui->lineEdit_nodo_puerta->setText(consultar_nodo.value(NUM_COL_NODO_PUERTADIRECCION).toString());
        ui->lineEdit_todo_nodo_puerta->setText(consultar_nodo.value(NUM_COL_NODO_PUERTADIRECCION).toString());

        ui->lineEdit_nodo_escalera->setText(consultar_nodo.value(NUM_COL_NODO_ESCALERADIRECCION).toString());
        ui->lineEdit_todo_nodo_escalera->setText(consultar_nodo.value(NUM_COL_NODO_ESCALERADIRECCION).toString());

        ui->lineEdit_nodo_cp->setText(consultar_nodo.value(NUM_COL_NODO_CODIGOPOSTAL).toString());
        ui->lineEdit_todo_nodo_cp->setText(consultar_nodo.value(NUM_COL_NODO_CODIGOPOSTAL).toString());

        ui->lineEdit_nodo_latitud->setText(consultar_nodo.value(NUM_COL_NODO_LATITUD).toString());
        ui->lineEdit_todo_nodo_latitud->setText(consultar_nodo.value(NUM_COL_NODO_LATITUD).toString());

        ui->lineEdit_nodo_longitud->setText(consultar_nodo.value(NUM_COL_NODO_LONGITUD).toString());
        ui->lineEdit_todo_nodo_longitud->setText(consultar_nodo.value(NUM_COL_NODO_LONGITUD).toString());

        ui->lineEdit_nodo_extension->setText(consultar_nodo.value(NUM_COL_NODO_EXTENSION).toString());
        ui->lineEdit_todo_nodo_extension->setText(consultar_nodo.value(NUM_COL_NODO_EXTENSION).toString());

        ui->lineEdit_nodo_fax->setText(consultar_nodo.value(NUM_COL_NODO_FAX).toString());
        ui->lineEdit_todo_nodo_fax->setText(consultar_nodo.value(NUM_COL_NODO_FAX).toString());

        ui->lineEdit_nodo_web->setText(consultar_nodo.value(NUM_COL_NODO_WEB).toString());
        ui->lineEdit_todo_nodo_web->setText(consultar_nodo.value(NUM_COL_NODO_WEB).toString());

        ui->lineEdit_nodo_sede->setText(consultar_nodo.value(NUM_COL_NODO_SEDE).toString());
        ui->lineEdit_todo_nodo_sede->setText(consultar_nodo.value(NUM_COL_NODO_SEDE).toString());

        ui->lineEdit_nodo_tablon->setText(consultar_nodo.value(NUM_COL_NODO_URLTABLON).toString());
        ui->lineEdit_todo_nodo_tablon->setText(consultar_nodo.value(NUM_COL_NODO_URLTABLON).toString());

        ui->lineEdit_nodo_portalTransparencia->setText(consultar_nodo.value(NUM_COL_NODO_URLPORTALTRANSPARENCIA).toString());
        ui->lineEdit_todo_nodo_portalTransparencia->setText(consultar_nodo.value(NUM_COL_NODO_URLPORTALTRANSPARENCIA).toString());

        ui->lineEdit_linea_adsl->setText(consultar_nodo.value(NUM_COL_NODO_ADSLLINEA).toString());
        ui->lineEdit_todo_linea_adsl->setText(consultar_nodo.value(NUM_COL_NODO_ADSLLINEA).toString());

        ui->lineEdit_linea_num_administrativo->setText(consultar_nodo.value(NUM_COL_NODO_NUMADMINISTRATIVO).toString());
        ui->lineEdit_todo_linea_n_adm->setText(consultar_nodo.value(NUM_COL_NODO_NUMADMINISTRATIVO).toString());

        ui->lineEdit_linea_ip_cifrado->setText(consultar_nodo.value(NUM_COL_NODO_IPCIFRADO).toString());
        ui->lineEdit_todo_linea_ip_cifrado->setText(consultar_nodo.value(NUM_COL_NODO_IPCIFRADO).toString());

        ui->lineEdit_linea_servicio->setText(consultar_nodo.value(NUM_COL_NODO_SERVICIOLINEA).toString());
        ui->lineEdit_todo_linea_servicio->setText(consultar_nodo.value(NUM_COL_NODO_SERVICIOLINEA).toString());

        ui->lineEdit_linea_caudal->setText(consultar_nodo.value(NUM_COL_NODO_CAUDAL).toString());
        ui->lineEdit_todo_linea_caudal->setText(consultar_nodo.value(NUM_COL_NODO_CAUDAL).toString());

        ui->lineEdit_linea_equipamiento->setText(consultar_nodo.value(NUM_COL_NODO_EQUIPAMIENTO).toString());
        ui->lineEdit_todo_linea_equipamiento->setText(consultar_nodo.value(NUM_COL_NODO_EQUIPAMIENTO).toString());

        ui->lineEdit_linea_numero_serie->setText(consultar_nodo.value(NUM_COL_NODO_NUMEROSERIEROUTER).toString());
        ui->lineEdit_todo_linea_numeroSerieRouter->setText(consultar_nodo.value(NUM_COL_NODO_NUMEROSERIEROUTER).toString());

        ui->comboBox_top_extension->setCurrentIndex(ui->comboBox_top_extension->findText(consultar_nodo.value(NUM_COL_NODO_EXTENSION).toString()));

        modelo_router = consultar_nodo.value(NUM_COL_NODO_EQUIPAMIENTO).toString();

        // Cargar datos de programa del año actual
        consultaPrograma(id_nodo, anio_programa);

        // Cargar teléfonos del nodo
        consultar_telefono.prepare(QString("SELECT * FROM telefononodo WHERE idNodo =:idNodo"));
        consultar_telefono.bindValue(":idNodo", id_nodo);

        if (consultar_telefono.exec()) {
            if (consultar_telefono.first()) {
                ui->comboBox_top_telefono->setCurrentIndex(ui->comboBox_top_telefono->findText(consultar_telefono.value(NUM_COL_TELEFONO_TELEFONO).toString()));

                do {
                    ui->comboBox_nodo_telefonos->addItem(consultar_telefono.value(NUM_COL_TELEFONO_TELEFONO).toString());
                    ui->comboBox_todo_nodo_telefonos->addItem(consultar_telefono.value(NUM_COL_TELEFONO_TELEFONO).toString());
                } while(consultar_telefono.next());
            }
            else {
                insertaTexto("Datos vacios en la consulta de telefonos " + consultar_nodo.value(NUM_COL_NODO_NOMBRE).toString());
            }
        }
        else {
             QMessageBox::critical(this, "Sql Error", "Ha ocurrido un error consultando telefono: \n" + consultar_telefono.lastError().text(), QMessageBox::Ok);
        }

        // Cargar datos de población del nodo
        consultar_poblacion.prepare(QString("SELECT * FROM poblacion WHERE id =:idPoblacion"));
        consultar_poblacion.bindValue(":idPoblacion", consultar_nodo.value(NUM_COL_NODO_IDPOBLACION).toString());

        if (consultar_poblacion.exec()) {
            if (consultar_poblacion.first()) {
                ui->lineEdit_nodo_poblacion->setText(consultar_poblacion.value(NUM_COL_POBLACION_NOMBRE).toString());

                if (!consultar_poblacion.value(NUM_COL_POBLACION_IDELA).isNull()) { //Comprueba si es una ELA
                    idEla = consultar_poblacion.value(NUM_COL_POBLACION_IDMUNICIPIO).toString();
                    consultar_ela.prepare(QString("SELECT nombre FROM municipio WHERE id = :idEla"));
                    consultar_ela.bindValue(":idEla", idEla);

                    if ((consultar_ela.exec()) && (consultar_ela.first())) {
                        nombreEla = consultar_ela.value(0).toString();
                    }
                }

                id_municipio = consultar_poblacion.value(NUM_COL_POBLACION_IDMUNICIPIO).toInt();

                ui->lineEdit_municipio_habitantes->setText(consultar_poblacion.value(NUM_COL_POBLACION_NOMBRE).toString());
                ui->lineEdit_municipio_superficie->setText(consultar_poblacion.value(NUM_COL_POBLACION_NOMBREID).toString());

                consultar_municipio.prepare(QString("SELECT * FROM municipio WHERE id = :idMunicipio"));
                consultar_municipio.bindValue(":idMunicipio", id_municipio);


                if (consultar_municipio.exec()) {
                    if (consultar_municipio.first()) {
                        id_comarca = consultar_municipio.value(NUM_COL_MUNICIPIO_IDCOMARCA).toInt();
                        municipio_nombreID = consultar_municipio.value(NUM_COL_MUNICIPIO_NOMBREID).toString(); //carga en la variable global nodo, el municipio actual

                        if (consultar_poblacion.value(NUM_COL_POBLACION_IDELA).isNull()) { //Comprueba si es una ELA
                            ui->label_municipio_nombre->setText(consultar_municipio.value(NUM_COL_MUNICIPIO_NOMBRE).toString());
                            ui->lineEdit_nodo_municipio->setText(consultar_municipio.value(NUM_COL_MUNICIPIO_NOMBRE).toString());
                            ui->lineEdit_todo_nodo_municipio->setText(consultar_municipio.value(NUM_COL_MUNICIPIO_NOMBRE).toString());
                        }
                        else {
                            ui->label_municipio_nombre->setText(nombreEla);
                            ui->lineEdit_nodo_municipio->setText(nombreEla);
                            ui->lineEdit_todo_nodo_municipio->setText(nombreEla);
                        }

                        //ui->lineEdit_municipio_cif->setText(consultar_municipio.value(NUM_COL_MUNICIPIO_CIF).toString());
                        //ui->lineEdit_todo_municipio_cif->setText(consultar_municipio.value(NUM_COL_MUNICIPIO_CIF).toString());

                        //ui->lineEdit_municipio_dir3->setText(consultar_municipio.value(NUM_COL_MUNICIPIO_CODIGODIR3).toString());
                        //ui->lineEdit_todo_municipio_dir3->setText(consultar_municipio.value(NUM_COL_MUNICIPIO_CODIGODIR3).toString());

                        ui->lineEdit_municipio_ine->setText(consultar_municipio.value(NUM_COL_MUNICIPIO_CODIGOINE).toString());
                        ui->lineEdit_todo_municipio_ine->setText(consultar_municipio.value(NUM_COL_MUNICIPIO_CODIGOINE).toString());

                        ui->lineEdit_municipio_habitantes->setText(consultar_municipio.value(NUM_COL_MUNICIPIO_NUMEROHABITANTES).toString());
                        ui->lineEdit_todo_municipio_habitantes->setText(consultar_municipio.value(NUM_COL_MUNICIPIO_NUMEROHABITANTES).toString());

                        ui->lineEdit_municipio_superficie->setText(QString::number(consultar_municipio.value(NUM_COL_MUNICIPIO_SUPERFICIE).toFloat(), 'g', 3));
                        ui->lineEdit_todo_municipio_superficie->setText(QString::number(consultar_municipio.value(NUM_COL_MUNICIPIO_SUPERFICIE).toFloat(), 'g', 3));

                        ui->lineEdit_municipio_altitud->setText(consultar_municipio.value(NUM_COL_MUNICIPIO_ALTITUD).toString());
                        ui->lineEdit_todo_municipio_altitud->setText(consultar_municipio.value(NUM_COL_MUNICIPIO_ALTITUD).toString());

                        ui->lineEdit_municipio_latitud->setText(consultar_municipio.value(NUM_COL_MUNICIPIO_LATITUD).toString());
                        ui->lineEdit_todo_municipio_latitud->setText(consultar_municipio.value(NUM_COL_MUNICIPIO_LATITUD).toString());

                        ui->lineEdit_municipio_longitud->setText(consultar_municipio.value(NUM_COL_MUNICIPIO_LONGITUD).toString());
                        ui->lineEdit_todo_municipio_longitud->setText(consultar_municipio.value(NUM_COL_MUNICIPIO_LONGITUD).toString());

                        ui->lineEdit_municipio_web->setText(consultar_municipio.value(NUM_COL_MUNICIPIO_WEB).toString());

                        // Obtener nombre de la comarca del municipio
                        consultaComarca(id_comarca);

                        // Obtener mancomunidades del municipio
                        consultaMancomunidades(id_municipio);

                        // Cargar las imagenes de router, banderas, escudo, etc...
                        carga_imagenes(municipio_nombreID, ui->lineEdit_linea_equipamiento->text());
                    }
                    else {
                        insertaTexto("Datos vacios en la consulta de municipio " + consultar_nodo.value(NUM_COL_NODO_NOMBRE).toString());
                    }
                }
                else {
                    QMessageBox::critical(this, "Sql Error", "Ha ocurrido un error consultando municipio: \n" + consultar_municipio.lastError().text(), QMessageBox::Ok);
                }
            }
            else {
                QMessageBox::critical(this, "Sql Error", "Ha ocurrido un error consultando poblacion: \n" + consultar_poblacion.lastError().text(), QMessageBox::Ok);
            }

            // Obtener emails del nodo
            consultar_email.prepare(QString("SELECT * FROM emailnodo WHERE idNodo = :idNodo"));
            consultar_email.bindValue(":idNodo", id_nodo);

            ui->comboBox_nodo_emails->clear();
            ui->comboBox_todo_nodo_emails->clear();

            if (consultar_email.exec()) {
                if (consultar_email.first()) {
                    do {
                        ui->comboBox_nodo_emails->addItem(consultar_email.value(NUM_COL_EMAILNODO_EMAIL).toString());
                        ui->comboBox_todo_nodo_emails->addItem(consultar_email.value(NUM_COL_EMAILNODO_EMAIL).toString());
                    }while(consultar_email.next());
                }
                else {
                    insertaTexto("Datos vacios en la consulta en la consulta de email: \n" + consultar_nodo.value(NUM_COL_NODO_NOMBRE).toString());
                }
            }
            else {
                QMessageBox::critical(this, "Sql Error", "Ha ocurrido un error consultando email: \n" + consultar_email.lastError().text(), QMessageBox::Ok);
            }
        }
    }
    else {
        QMessageBox::critical(this, "Sql Error", "Ha ocurrido un error consultando nodo: \n" + consultar_nodo.lastError().text(), QMessageBox::Ok);
    }

    activa_botones(); //activa/desactiva los botones
}


void Sedes::consultaComarca(int id_comarca) {
    QSqlQuery consultar_comarca(db_sqlite);

    consultar_comarca.prepare(QString("SELECT nombre FROM comarca WHERE id = :idComarca"));
    consultar_comarca.bindValue(":idComarca", id_comarca);

    if (consultar_comarca.exec()) {
        if (consultar_comarca.first()) {
            ui->lineEdit_municipio_comarca->setText(consultar_comarca.value(NUM_COL_COMARCA_ID).toString());
            ui->lineEdit_todo_municipio_comarca->setText(consultar_comarca.value(NUM_COL_COMARCA_ID).toString());
        }
        else {
            //insertaTexto("Datos vacios en la consulta de comarca " + consultar_nodo.value(num_col_nodo_nombre).toString());
        }
    }
    else {
        QMessageBox::critical(this, "Sql Error", "Ha ocurrido un error: \n" + consultar_comarca.lastError().text(), QMessageBox::Ok);
    }
}


void Sedes::consultaMancomunidades(int id_municipio) {
    QSqlQuery consultar_mancomunidad(db_sqlite);
    QString sql;

    sql = "SELECT nombre FROM mancomunidad man "
          "INNER JOIN mancomunidadmunicipio mm ON man.id = mm.idMancomunidad "
          "WHERE mm.idMunicipio = :id_municipio";
    consultar_mancomunidad.prepare(sql);
    consultar_mancomunidad.bindValue(":id_municipio", id_municipio);

    if (consultar_mancomunidad.exec()) {
        if (consultar_mancomunidad.first()) {
            ui->comboBox_municipio_mancomunidad->setCurrentIndex(ui->comboBox_municipio_mancomunidad->findText(consultar_mancomunidad.value(0).toString()));

            do {
                ui->comboBox_municipio_mancomunidad->addItem(consultar_mancomunidad.value(NUM_COL_MANCOMUNIDAD_ID).toString());
                ui->comboBox_todo_municipio_mancomunidad->addItem(consultar_mancomunidad.value(NUM_COL_MANCOMUNIDAD_ID).toString());
            }while(consultar_mancomunidad.next());


        }
        else {
            //insertaTexto("Datos vacios en la consulta de mancomunidad " + consultar_nodo.value(num_col_nodo_nombre).toString());
        }
    }
    else {
        QMessageBox::critical(this, "Sql Error", "Ha ocurrido un error consultando mancomunidades: \n" + consultar_mancomunidad.lastError().text(), QMessageBox::Ok);
    }
}


void Sedes::consultaPrograma(int id_nodo, const QString &anio) {
    QSqlQuery consultar_programa(db_sqlite);

    consultar_programa.prepare(QString("SELECT * FROM programa WHERE idNodo =:id_nodo and anio =:anio"));
    consultar_programa.bindValue(":id_nodo", id_nodo);
    consultar_programa.bindValue(":anio", anio);

    if (consultar_programa.exec()) {
        if (consultar_programa.first()) {
            ui->checkBox_concertaciones_portal_web->setCheckState((Qt::CheckState)consultar_programa.value(NUM_COL_PROGRAMA_PORTALWEB).toInt());
            ui->checkBox_todo_concertaciones_portal_web->setCheckState((Qt::CheckState)consultar_programa.value(NUM_COL_PROGRAMA_PORTALWEB).toInt());

            ui->checkBox_concertaciones_correo->setCheckState((Qt::CheckState)consultar_programa.value(NUM_COL_PROGRAMA_EMAIL).toInt());
            ui->checkBox_todo_concertaciones_correo->setCheckState((Qt::CheckState)consultar_programa.value(NUM_COL_PROGRAMA_EMAIL).toInt());

            ui->checkBox_concertaciones_basedatos_juridica->setCheckState((Qt::CheckState)consultar_programa.value(NUM_COL_PROGRAMA_BASEDATOSJURIDICA).toInt());
            ui->checkBox_todo_concertaciones_basedatos_juridica->setCheckState((Qt::CheckState)consultar_programa.value(NUM_COL_PROGRAMA_BASEDATOSJURIDICA).toInt());

            ui->checkBox_concertaciones_suscripcion->setCheckState((Qt::CheckState)consultar_programa.value(NUM_COL_PROGRAMA_SUSCRIPCIONDOMINIO).toInt());
            ui->checkBox_todo_concertaciones_suscripcion->setCheckState((Qt::CheckState)consultar_programa.value(NUM_COL_PROGRAMA_SUSCRIPCIONDOMINIO).toInt());

            ui->checkBox_concertaciones_perfil_contratante->setCheckState((Qt::CheckState)consultar_programa.value(NUM_COL_PROGRAMA_PERFILCONTRATANTE).toInt());
            ui->checkBox_todo_concertaciones_perfil_contratante->setCheckState((Qt::CheckState)consultar_programa.value(NUM_COL_PROGRAMA_PERFILCONTRATANTE).toInt());

            ui->checkBox_concertaciones_gestion_municipal->setCheckState((Qt::CheckState)consultar_programa.value(NUM_COL_PROGRAMA_GESTIONMUNICIPAL).toInt());
            ui->checkBox_todo_concertaciones_gestion_municipal->setCheckState((Qt::CheckState)consultar_programa.value(NUM_COL_PROGRAMA_GESTIONMUNICIPAL).toInt());

            ui->checkBox_concertaciones_gestion_economica->setCheckState((Qt::CheckState)consultar_programa.value(NUM_COL_PROGRAMA_GESTIONECONOMICA).toInt());
            ui->checkBox_todo_concertaciones_gestion_economica->setCheckState((Qt::CheckState)consultar_programa.value(NUM_COL_PROGRAMA_GESTIONECONOMICA).toInt());

            ui->checkBox_concertaciones_soporte->setCheckState((Qt::CheckState)consultar_programa.value(NUM_COL_PROGRAMA_SOPORTE).toInt());
            ui->checkBox_todo_concertaciones_soporte->setCheckState((Qt::CheckState)consultar_programa.value(NUM_COL_PROGRAMA_SOPORTE).toInt());

            ui->checkBox_concertaciones_sede_electronica->setCheckState((Qt::CheckState)consultar_programa.value(NUM_COL_PROGRAMA_SEDEELECTRONICA).toInt());
            ui->checkBox_todo_concertaciones_sede_electronica->setCheckState((Qt::CheckState)consultar_programa.value(NUM_COL_PROGRAMA_SEDEELECTRONICA).toInt());

            ui->checkBox_concertaciones_epol->setCheckState((Qt::CheckState)consultar_programa.value(NUM_COL_PROGRAMA_EPOL).toInt());
            ui->checkBox_todo_concertaciones_epol->setCheckState((Qt::CheckState)consultar_programa.value(NUM_COL_PROGRAMA_EPOL).toInt());

            ui->checkBox_concertaciones_epol_movil->setCheckState((Qt::CheckState)consultar_programa.value(NUM_COL_PROGRAMA_EPOLMOVIL).toInt());
            ui->checkBox_todo_concertaciones_epol_movil->setCheckState((Qt::CheckState)consultar_programa.value(NUM_COL_PROGRAMA_EPOLMOVIL).toInt());

            ui->checkBox_concertaciones_siapol->setCheckState((Qt::CheckState)consultar_programa.value(NUM_COL_PROGRAMA_SIAPOL).toInt());
            ui->checkBox_todo_concertaciones_siapol->setCheckState((Qt::CheckState)consultar_programa.value(NUM_COL_PROGRAMA_SIAPOL).toInt());
        }
        else {
            //insertaTexto("Datos vacios en la consulta de programas.");
        }
    }
    else {
        QMessageBox::critical(this, "Sql Error", "Ha ocurrido un error consultando la tabla programa: \n" + consultar_programa.lastError().text(), QMessageBox::Ok);
    }
}


void Sedes::activa_botones() {
    //Sirve para activar los botones si tienen texto los lineEdit
    bool web_municipio_activa = !(ui->lineEdit_municipio_web->text().isEmpty());
    bool web_nodo_activa = !(ui->lineEdit_nodo_web->text().isEmpty());
    bool sede_activa = !(ui->lineEdit_nodo_sede->text().isEmpty());
    bool tablon_activa = !(ui->lineEdit_nodo_tablon->text().isEmpty());
    bool portal_activa = !(ui->lineEdit_nodo_portalTransparencia->text().isEmpty());

    ui->pB_municipio_web->setEnabled(web_municipio_activa);

    ui->pB_nodo_portalTransparencia->setEnabled(portal_activa);
    ui->pB_nodo_sede->setEnabled(sede_activa);
    ui->pB_nodo_tablon->setEnabled(tablon_activa);
    ui->pB_nodo_web->setEnabled(web_nodo_activa);

    ui->pB_todo_nodo_portalTransparencia->setEnabled(portal_activa);
    ui->pB_todo_nodo_sede->setEnabled(sede_activa);
    ui->pB_todo_nodo_tablon->setEnabled(tablon_activa);
    ui->pB_todo_nodo_web->setEnabled(web_nodo_activa);
}


void Sedes::carga_imagenes(QString municipio_nombreID, QString modelo_router) {
    QString ruta_imagen_bandera         = QString(RUTA_IMAGENES) + "banderas/" + municipio_nombreID + ".svg";
    QString ruta_imagen_bandera_no      = QString(RUTA_IMAGENES) + "banderas/0_provincia_granada.svg";
    QString ruta_imagen_escudo          = QString(RUTA_IMAGENES) + "escudos/" + municipio_nombreID + ".svg";
    QString ruta_imagen_escudo_no       = QString(RUTA_IMAGENES) + "escudos/0_constitucional.svg";
    QString ruta_imagen_mapa            = QString(RUTA_IMAGENES) + "mapas/" + municipio_nombreID + ".png";
    QString ruta_imagen_mancomunidad    = QString(RUTA_IMAGENES) + "mancomunidades/" + ui->comboBox_municipio_mancomunidad->currentText().toLower().replace(" ","_") + ".png";
    QString ruta_imagen_mancomunidad_no = QString(RUTA_IMAGENES) + "mancomunidades/mancomunidad_no.png";
    QString ruta_imagen_comarca         = QString(RUTA_IMAGENES) + "comarcas/" + ui->lineEdit_municipio_comarca->text().toLower().replace(" ","_") + ".png";
    QString ruta_imagen_comarca_no      = QString(RUTA_IMAGENES) + "comarcas/comarca_no.png";
    QString ruta_imagen_router          = QString(RUTA_IMAGENES) + "routers/" + modelo_router + ".jpg";

    // Establecer imagen de la bandera
    QIcon icon_bandera = existeFichero(ruta_imagen_bandera) ? QIcon(ruta_imagen_bandera) : QIcon(ruta_imagen_bandera_no);
    ui->pB_municipio_bandera->setIcon(icon_bandera);
    ui->pB_todo_municipio_bandera->setIcon(icon_bandera);

    // Establecer imagen del escudo
    QIcon icon_escudo = existeFichero(ruta_imagen_escudo) ? QIcon(ruta_imagen_escudo) : QIcon(ruta_imagen_escudo_no);
    ui->pB_municipio_escudo->setIcon(icon_escudo);
    ui->pB_todo_municipio_escudo->setIcon(icon_escudo);

    // Establecer imagen del mapa
    QIcon icon_mapa = QIcon(ruta_imagen_mapa);
    ui->pB_municipio_mapa->setIcon(icon_mapa);
    ui->pB_nodo_mapa->setIcon(icon_mapa);
    ui->pB_todo_nodo_mapa->setIcon(icon_mapa);

    // Establecer imagen del router
    QIcon icon_router = QIcon(ruta_imagen_router);
    ui->pB_linea_router->setIcon(icon_router);
    ui->pB_todo_linea_router->setIcon(icon_router);

    // Establecer imagen de la mancomunidad
    QIcon icon_mancomunidad = existeFichero(ruta_imagen_mancomunidad) ? QIcon(ruta_imagen_mancomunidad) : QIcon(ruta_imagen_mancomunidad_no);
    ui->pB_todo_municipio_mancomunidad->setIcon(icon_mancomunidad);

    // Establecer imagen del comarca
    QIcon icon_comarca = existeFichero(ruta_imagen_comarca) ? QIcon(ruta_imagen_comarca) : QIcon(ruta_imagen_comarca_no);
    ui->pB_todo_municipio_comarca->setIcon(icon_comarca);
}


void Sedes::insertaTexto(QString texto) {
    ui->textEdit_consola->moveCursor(QTextCursor::End);
    ui->textEdit_consola->insertPlainText(texto);
    ui->textEdit_consola->moveCursor(QTextCursor::End);
}


void Sedes::on_comboBox_concertaciones_anio_activated(const QString &arg1) {
    consultaPrograma(id_nodo, arg1);
}


void Sedes::on_comboBox_todo_concertaciones_anio_activated(const QString &arg1) {
    on_comboBox_concertaciones_anio_activated(arg1);
}


void Sedes::on_comboBox_todo_municipio_mancomunidad_activated(const QString &arg1) {
    QString ruta_imagen_mancomunidad    = QString(RUTA_IMAGENES) + "mancomunidades/" + arg1.toLower().replace(" ","_") + ".png";
    QString ruta_imagen_mancomunidad_no = QString(RUTA_IMAGENES) + "mancomunidades/mancomunidad_no.png";

    // Establecer imagen de la mancomunidad
    QIcon icon_mancomunidad = existeFichero(ruta_imagen_mancomunidad) ? QIcon(ruta_imagen_mancomunidad) : QIcon(ruta_imagen_mancomunidad_no);
    ui->pB_todo_municipio_mancomunidad->setIcon(icon_mancomunidad);
}


void Sedes::on_comboBox_top_ip_activated(const QString &ip) {
    QHostAddress myIP;
    QSqlQuery query = model->query();
    QString nombre = query.value(0).toString();

    if (myIP.setAddress(ip)) {
        consultaNodo(nombre); //query.value(0).toString() contiene el nombre de la consulta actual
        ui->comboBox_top_nodo->setCurrentIndex(ui->comboBox_top_nodo->findText(nombre));
    }
    else {
        insertaTexto("IP (" + ip + ") no valida");
    }
}


void Sedes::on_comboBox_top_nodo_activated(const QString &nombre) {
    QSqlQuery query = model->query();
    QString ip = query.value(1).toString();

    consultaNodo(nombre);
    ui->comboBox_top_ip->setCurrentIndex(ui->comboBox_top_ip->findText(ip));
}


void Sedes::on_comboBox_top_telefono_activated(const QString &arg1) {
    QSqlQuery query = model->query();
    QSqlQuery query_tlf = model_tlf->query();
    QString indice_tlf = query_tlf.value(0).toString();
    QString nombre_ayto = query.value(0).toString();
    QString ip = query.value(1).toString();

    query.first();
    while (query.next()) {
        if (( query.value(4).toString() == indice_tlf )&&(nombre_ayto!=query.value(0).toString())) {
            nombre_ayto = query.value(0).toString();
            ip = query.value(1).toString();
            consultaNodo(nombre_ayto); //query.value(0).toString() contiene el nombre del ayto de la consulta actual
            ui->comboBox_top_nodo->setCurrentIndex(ui->comboBox_top_nodo->findText(nombre_ayto));
            ui->comboBox_top_ip->setCurrentIndex(ui->comboBox_top_ip->findText(ip));
            return;
        }
        else{
            if (( query.value(4).toString() == indice_tlf )&&(nombre_ayto==query.value(0).toString())) {
                return;
            }
        }
    }
}


void Sedes::on_comboBox_top_extension_activated(const QString &arg1) {
    QSqlQuery query = model->query();
    QString id = query.value(0).toString();
    QString nombre = query.value(1).toString();

    if (!arg1.isEmpty()) {
        consultaNodo(id); //query.value(0).toString() contiene el nombre de la consulta actual
        ui->comboBox_top_nodo->setCurrentIndex(ui->comboBox_top_nodo->findText(id));
        ui->comboBox_top_ip->setCurrentIndex(ui->comboBox_top_ip->findText(nombre));
    }
}


void Sedes::on_pB_linea_router_clicked() {
    QDesktopServices::openUrl(QUrl("file:" + QString(RUTA_IMAGENES) + "routers/" + modelo_router + ".jpg"));
}


void Sedes::on_pB_municipio_bandera_clicked() {
    QString ruta_bandera = QString(RUTA_IMAGENES) + "banderas/" + municipio_nombreID + ".svg";

    if (existeFichero(ruta_bandera)) {
        QDesktopServices::openUrl(QUrl(ruta_bandera, QUrl::TolerantMode));
    }
    else {
        QDesktopServices::openUrl(QUrl(QString(RUTA_IMAGENES) + "banderas/0_provincia_granada.svg", QUrl::TolerantMode));
    }
}


void Sedes::on_pB_municipio_escudo_clicked() {
    QString ruta_escudo = QString(RUTA_IMAGENES) + "escudos/" + municipio_nombreID + ".svg";

    if (existeFichero(ruta_escudo)) {
        QDesktopServices::openUrl(QUrl(ruta_escudo, QUrl::TolerantMode));
    }
    else {
        QDesktopServices::openUrl(QUrl(QString(RUTA_IMAGENES) + "escudos/0_constitucional.svg", QUrl::TolerantMode));
    }
}


void Sedes::on_pB_municipio_googleMaps_clicked() {
    QDesktopServices::openUrl(QUrl("https://www.google.es/maps/place/" + ui->comboBox_top_nodo->currentText() + " +Granada"));
}


void Sedes::on_pB_municipio_mapa_clicked() {
    QDesktopServices::openUrl(QUrl("file:" + QString(RUTA_IMAGENES) + "mapas/" + municipio_nombreID + ".png"));
}


void Sedes::on_pB_municipio_web_clicked() {
    QDesktopServices::openUrl(QUrl(ui->lineEdit_municipio_web->text(), QUrl::TolerantMode));
}


void Sedes::on_pB_municipio_wikipedia_clicked() {
    QDesktopServices::openUrl(QUrl("https://es.wikipedia.org/wiki/" + ui->label_municipio_nombre->text() + "_(Granada)"));
}


void Sedes::on_pB_nodo_googleMaps_clicked() {
    QDesktopServices::openUrl(QUrl("https://www.google.es/maps/place/" + ui->comboBox_top_nodo->currentText() + " +Granada"));
}


void Sedes::on_pB_nodo_mapa_clicked() {
    QDesktopServices::openUrl(QUrl("file:" + QString(RUTA_IMAGENES) + "mapas/" + municipio_nombreID + ".png"));
}


void Sedes::on_pB_nodo_portalTransparencia_clicked() {
    QDesktopServices::openUrl(QUrl(ui->lineEdit_nodo_portalTransparencia->text(), QUrl::TolerantMode));
}


void Sedes::on_pB_nodo_sede_clicked() {
    QDesktopServices::openUrl(QUrl(ui->lineEdit_nodo_sede->text(), QUrl::TolerantMode));
}


void Sedes::on_pB_nodo_tablon_clicked() {
    QDesktopServices::openUrl(QUrl(ui->lineEdit_nodo_tablon->text(), QUrl::TolerantMode));
}


void Sedes::on_pB_nodo_web_clicked() {
    QDesktopServices::openUrl(QUrl(ui->lineEdit_nodo_web->text(), QUrl::TolerantMode));
}


void Sedes::on_pB_nodo_wikipedia_clicked() {
    // Generar enlace de Wikipedia
    QString url_final = ui->comboBox_top_nodo->currentText(); // Agregar nombre del nodo

    // Si es un ayuntamiento y en el nombre del nodo hay municipio y población, eliminar la parte de municipio
    if (ui->checkBox_nodo_esAyuntamiento->isChecked()) {
        QStringList split = url_final.split("-");

        if (split.count() > 1) {
            url_final = split[1];
        }
    }

    url_final.remove("|"); // Eliminar separadores '|'
    url_final.remove(" -"); // Eliminar separadores que sean ' - '

    url_final += "_(Granada)"; // Añadir terminación de provincia para que se abra bien en Wikipedia

    // Abrir página de Wikipedia con el enlace generado
    QDesktopServices::openUrl(QUrl("https://es.wikipedia.org/wiki/" + url_final));
}


void Sedes::on_pB_todo_linea_router_clicked() {
    on_pB_linea_router_clicked();
}


void Sedes::on_pB_todo_municipio_bandera_clicked() {
    on_pB_municipio_bandera_clicked();
}


void Sedes::on_pB_todo_municipio_escudo_clicked() {
    on_pB_municipio_escudo_clicked();
}


void Sedes::on_pB_todo_municipio_comarca_clicked() {
    QString ruta_comarca = QString(RUTA_IMAGENES) + "comarcas/" + ui->lineEdit_todo_municipio_comarca->text().toLower().replace(" ","_") + ".png";

    if (existeFichero(ruta_comarca)) {
        QDesktopServices::openUrl(QUrl(ruta_comarca, QUrl::TolerantMode));
    }
    else {
        QDesktopServices::openUrl(QUrl(QString(RUTA_IMAGENES) + "comarcas/comarca_no.png", QUrl::TolerantMode));
    }
}


void Sedes::on_pB_todo_municipio_mancomunidad_clicked() {
    QString ruta_mancomunidad = QString(RUTA_IMAGENES) + "mancomunidades/" + ui->comboBox_todo_municipio_mancomunidad->currentText().toLower().replace(" ","_") + ".png";

    if (existeFichero(ruta_mancomunidad)) {
        QDesktopServices::openUrl(QUrl(ruta_mancomunidad, QUrl::TolerantMode));
    }
    else {
        QDesktopServices::openUrl(QUrl(QString(RUTA_IMAGENES) + "mancomunidades/mancomunidad_no.png", QUrl::TolerantMode));
    }
}


void Sedes::on_pB_todo_nodo_googleMaps_clicked() {
    on_pB_nodo_googleMaps_clicked();
}


void Sedes::on_pB_todo_nodo_mapa_clicked() {
    on_pB_nodo_mapa_clicked();
}


void Sedes::on_pB_todo_nodo_sede_clicked() {
    on_pB_nodo_sede_clicked();
}


void Sedes::on_pB_todo_nodo_tablon_clicked() {
    on_pB_nodo_tablon_clicked();
}


void Sedes::on_pB_todo_nodo_portalTransparencia_clicked() {
    on_pB_nodo_portalTransparencia_clicked();
}


void Sedes::on_pB_todo_nodo_web_clicked() {
    on_pB_nodo_web_clicked();
}


void Sedes::on_pB_todo_nodo_wikipedia_clicked() {
    on_pB_nodo_wikipedia_clicked();
}
