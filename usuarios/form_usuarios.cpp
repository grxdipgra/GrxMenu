#include "form_usuarios.h"
#include "configuracion/configuracion.h"
#include "basedatos/basedatos.h"
#include <QFileInfo>
#include <QProgressDialog>
#include <QCloseEvent>
#include <QStringListModel>

 form_usuarios::form_usuarios(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::form_usuarios)
{
    ui->setupUi(this);

    Configuracion *config = new Configuracion();
    host = config->cual_es_servidor_ldap();
    puerto = config->cual_es_puerto_ldap();
    usuarioLdap = config->cual_es_usuario_ldap();
    dominio=config->cual_es_dominio_ldap();
    password=config->cual_es_clave_ldap();
    delete config;
    db_sqlite = QSqlDatabase::database("sqlite");
    QSqlQuery* consulta = new QSqlQuery(db_sqlite);

    // Si no hay usuarios en la BD actualizamos la BD

    consulta->exec("select usuario from ldap");

    if(!consulta->first()){
        BaseDatos *DB = new BaseDatos();
        DB->actualizar_usuarios();
        delete DB;
    }

    QString sql;
    QSqlQueryModel *model = new QSqlQueryModel();
    sql = "select usuario from ldap order by usuario";
    consulta->prepare(sql);
    if(!consulta->exec()){
        qDebug() <<"Error en la consulta 1: "<< consulta->lastError();
    }else{
        qDebug() <<"Consulta realizada con exito 1: "<<consulta->lastQuery();
        model->setQuery(*consulta);
        ui->comboBox_usuarios->setModel(model);
        on_comboBox_usuarios_activated(ui->comboBox_usuarios->itemText(0));
    }
    QSqlQueryModel *model1 = new QSqlQueryModel();
    sql = "select nombre from ldap order by usuario";
    consulta->prepare(sql);
    if(!consulta->exec()){
        qDebug() <<"Error en la consulta 2: "<< consulta->lastError();
    }else{
        qDebug() <<"Consulta realizada con exito 2: "<<consulta->lastQuery();
        model1->setQuery(*consulta);
        ui->comboBox_nombres->setModel(model1);
        on_comboBox_nombres_activated(ui->comboBox_nombres->itemText(0));
    }

    delete consulta;
}


form_usuarios::~form_usuarios()
{
    delete ui;
    db_sqlite.~QSqlDatabase();

}


void form_usuarios::on_comboBox_usuarios_activated(const QString &arg1)
{

    if (cambios_1 || cambios_2) {

        QMessageBox msgBox;

        msgBox.setText("El usuario ha sido modificado");
        msgBox.setInformativeText("¿Deseas guardar los cambios?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int elegido = msgBox.exec();

        switch (elegido) {
           case QMessageBox::Save:
                if (cambios_1){
                    guarda_datos_1();
                }
                if (cambios_2){
                    guarda_datos_2();
                }
               break;
           case QMessageBox::Cancel:
               break;
         }
    }

    carga_datos_usuario(0, arg1);

}

void form_usuarios::on_comboBox_nombres_activated(const QString &arg1)
{

    if (cambios_1 || cambios_2) {

        QMessageBox msgBox;
        msgBox.setText("El usuario ha sido modificado");
        msgBox.setInformativeText("¿Deseas guardar los cambios?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int elegido = msgBox.exec();

        switch (elegido) {
           case QMessageBox::Save:
                if (cambios_1){
                    guarda_datos_1();
                }
                if (cambios_2){
                    guarda_datos_2();
                }
               break;
           case QMessageBox::Cancel:
               break;
         }
    }
carga_datos_usuario(1, arg1);

}

void form_usuarios::clear_text(){

    ui->text_cambio_clave->clear();
    ui->text_clave_caduca->clear();
    ui->text_correo->clear();
    ui->text_creada->clear();
    ui->text_cuenta_caduca->clear();
    ui->dateTEdit_cuenta_caduca->clear();
    ui->text_estado->clear();
    ui->text_fecha_correo->clear();
    ui->text_intentos->clear();
    ui->text_logon->clear();
    ui->text_modif_cuenta->clear();
    ui->text_telefono->clear();
    ui->text_ulti_login->clear();
    ui->text_descripcion->clear();
    ui->label_habilitada->clear();

}

// Procedimiento para rellenar los campos del formulario desde la base de datos
// tipo=0->filtro por usuario, tipo=1->filtro por nombre
void form_usuarios::carga_datos_usuario(int tipo, QString filtro){


    BaseDatos *basedatos = new BaseDatos(INCOMPLETO);
    QSqlQuery* consultar = new QSqlQuery(db_sqlite);
    usuario usu;
    usu=basedatos-> consulta_usuario(consultar, filtro, tipo);

    id_usuario=usu.cual_es_id();
    us_usuario=usu.cual_es_usuario();

    ui->comboBox_usuarios->setCurrentText(usu.cual_es_usuario());//usuario
    ui->comboBox_nombres->setCurrentText(usu.cual_es_nombre());//nombre
    ui->text_cuenta_caduca->setText(usu.cual_es_caduca_cuenta());
    ui->text_cambio_clave->setText(usu.cual_es_ultimo_cambio_clave());
    ui->text_clave_caduca->setText(usu.cual_es_caduca_clave());
    ui->text_correo->setText(usu.cual_es_correo());
    ui->text_creada->setText(usu.cual_es_creada());
    ui->text_estado->setText(usu.cual_es_estado());
    ui->text_fecha_correo->setText(usu.cual_es_fecha_correo());
    ui->text_intentos->setText(QString::number(usu.cual_es_intentos()));
    ui->text_logon->setText(QString::number(usu.cual_es_logon()));
    ui->text_modif_cuenta->setText(usu.cual_es_modificacion_cuenta());
    ui->text_telefono->setText(usu.cual_es_telefono());
    ui->text_ulti_login->setText(usu.cual_es_ultimo_login());
    ui->text_descripcion->setText(usu.cual_es_descripcion());
    ui->text_usuario->setText(usu.cual_es_user());
    ui->text_usuariow2000->setText(usu.cual_es_usuario_preW2000());
    ui->text_nombre->setText(usu.cual_es_nombre_solo());
    ui->text_apellidos->setText(usu.cual_es_apellidos());
    ui->text_nombremostrar->setText(usu.cual_es_nombre_mostrar());
    ui->text_movil->setText(usu.cual_es_movil());
    ui->text_fax->setText(usu.cual_es_fax());
    ui->text_departamento->setText(usu.cual_es_departamento());
    ui->text_oficina->setText(usu.cual_es_oficina());
    ui->text_organizacion->setText(usu.cual_es_organizacion());
    ui->text_puesto->setText(usu.cual_es_puesto());
    ui->text_provincia->setText(usu.cual_es_provincia());
    ui->text_localidad->setText(usu.cual_es_localidad());
    ui->text_direccion->setText(usu.cual_es_direccion());
    ui->text_cp->setText(usu.cual_es_cp());
    ui->text_notas->setText(usu.cual_es_notas());
    ui->label_18->setText("Ultima sincronización el "+ usu.cual_es_ultima_sincro());
    //guardamos la fecha de ultima sincronizacion del usuario
    ultima_sincro=QDateTime::fromString(usu.cual_es_ultima_sincro(),"dd-MM-yyyy hh:mm:ss");

    //actualizamos el valor: DN de usuario actual
    DN=usu.cual_es_dn();
    //qDebug()<<DN;
    uac=usu.cual_es_useraccountcontrol();

    //guardamos los DN de los grupos del usuario
    grupos.clear();
    grupos = basedatos->consulta_grupos(consultar,id_usuario);
    //foreach (const QString &str, grupos)
    //    qDebug()<<str;

    //le pasamos al listView_grupos un QStringList solo con los nombres de los grupos
    QStringList nombre_grupo;
    foreach (const QString &str, grupos) {
        nombre_grupo.append(str.mid(str.indexOf("=")+1,str.indexOf(",")-str.indexOf("=")-1));
    }

    QStringListModel *listModel = new QStringListModel(nombre_grupo, NULL);
    ui->listView_grupos->setModel(listModel);

    if (ui->text_estado->text().contains("Bloqueada hasta")){
        ui->text_estado->setStyleSheet("color: rgb(164, 0, 0)");
    }
    else{
        ui->text_estado->setStyleSheet("color: rgb(11, 97, 29)");
    }


    if (usu.cual_es_cambio_clave().contains("01-01-1601")){
        ui->checkBox_cambiar_pass->setChecked(true);
    }
    else{
        ui->checkBox_cambiar_pass->setChecked(false);
    }

    QDateTime caduca;
    //fecha actual:
    QDateTime ahora = QDateTime::currentDateTime();

    //qDebug()<<ahora;

    if (ui->text_clave_caduca->text()=="No Caduca"){
        ui->text_clave_caduca->setStyleSheet("color: rgb(5, 31, 137)");
        ui->checkBox_pass_nunca_expira->setChecked(true);
        ui->checkBox_cambiar_pass->setChecked(false);
    }
    else{
        ui->checkBox_pass_nunca_expira->setChecked(false);
        caduca = QDateTime::fromString(ui->text_clave_caduca->text(),"dd-MM-yyyy  hh:mm");
        if (ahora>caduca){
            //color rojo
            ui->text_clave_caduca->setStyleSheet("color: rgb(164, 0, 0)");
        }
        else{
            //color verde
            ui->text_clave_caduca->setStyleSheet("color: rgb(11, 97, 29)");
        }
    }


    if (ui->text_cuenta_caduca->text()=="No Caduca"){
        //color azul
        ui->text_cuenta_caduca->setStyleSheet("color: rgb(5, 31, 137)");
        ui->checkCaduca->setChecked(true);
    }
    else{
        ui->checkCaduca->setChecked(false);
        caduca = QDateTime::fromString(ui->text_cuenta_caduca->text(),"dd-MM-yyyy  hh:mm");
        if (ahora>caduca){
            //color rojo
            ui->text_cuenta_caduca->setStyleSheet("color: rgb(164, 0, 0)");
        }
        else{
            //color verde
            ui->text_cuenta_caduca->setStyleSheet("color: rgb(11, 97, 29)");
        }
    }

    if (estado_habilitado(uac)==1){
        ui->boton_habilitar->setEnabled(true);
        ui->label_habilitada->setStyleSheet("color: rgb(11, 97, 29)");
        ui->label_habilitada->setText("LA CUENTA ESTA HABILITADA "); // + QString::number(uac));
        ui->boton_habilitar->setText("Deshabilitar");
    }
    else{
        if (estado_habilitado(uac)==0){
            ui->boton_habilitar->setEnabled(true);
            ui->label_habilitada->setStyleSheet("color: rgb(164, 0, 0)");
            ui->label_habilitada->setText("LA CUENTA ESTA DESHABILITADA "); // + QString::number(uac));
            ui->boton_habilitar->setText("Habilitar");
        }
        else {
            ui->label_habilitada->setText(""); // + QString::number(uac));
            ui->boton_habilitar->setEnabled(false);
        }
    }

    cambios_1=false;
    cambios_2=false;

}

// Procedimiento para actualizar el usuario actual en la base de datos
void form_usuarios::actualiza_usuario(){
    BaseDatos *basedatos = new BaseDatos();
    QSqlQuery* consulta = new QSqlQuery(db_sqlite);
    QString info;

     usuario *usu = new usuario;
     clase_ldap *ldap1 = new clase_ldap(host,puerto,usuarioLdap,dominio,password,&info);

     if (!info.contains("ERROR")) {

         *usu=ldap1->Consulta_Usuario(us_usuario, DN, &info);

         if (!info.contains("ERROR")) {

            //actualizamos el usuario en la base de datos
             basedatos->actualiza_usuario(consulta,id_usuario,*usu);

             //eliminar los grupos del usuario
             basedatos->elimina_grupos_usuario(consulta,id_usuario);
             //consulta->exec("delete from grupos where id_usuario=" + QString::number(id_usuario));

            //actualizar los grupos del usuario
            for (int j = 0; j < usu->cual_es_grupos().size(); ++j) {

                //insertamos los grupos del usuario en la base de datos
                basedatos->insertar_grupo(consulta, id_usuario, *usu,j);

            }
            delete ldap1;
            delete usu;
            delete basedatos;

            carga_datos_usuario(0, ui->comboBox_usuarios->currentText());
         }
    }

    ui->label_estado->setText(info);
}


void form_usuarios::on_boton_actualiza_usuarios_clicked()
{
    BaseDatos *DB = new BaseDatos();
        DB->actualizar_usuarios();
    delete DB;
}


void form_usuarios::on_boton_actualiza_usuario_clicked()
{
    actualiza_usuario();
}



//este evento se activa cuando se modifica la fecha del DateTimeEdit
void form_usuarios::on_dateTEdit_cuenta_caduca_dateTimeChanged(const QDateTime &dateTime)
{

    //qDebug()<<"Fecha modificada";
    //le damos el mismo valos al text_cuenta_caduca
    ui->text_cuenta_caduca->setText(ui->dateTEdit_cuenta_caduca->dateTime().toString("dd-MM-yyyy  hh:mm"));

}

//este evento se activa cuando se despliega el calendario del DateTimeEdit
void form_usuarios::on_dateTEdit_cuenta_caduca_editingFinished()
{

    //qDebug()<<"Calendario desplegado";
    //si la cuenta no caduca le damos al calendario la fecha de hoy
    if (ui->text_cuenta_caduca->text()=="No Caduca") {
        ui->dateTEdit_cuenta_caduca->setDateTime(QDateTime::currentDateTime());
    }
    //si la cuenta caduca le damos al calendario la fecha de caducidad
    else {
        ui->dateTEdit_cuenta_caduca->setDateTime(QDateTime::fromString(ui->text_cuenta_caduca->text(),"dd-MM-yyyy  hh:mm"));
    }

}

//desbloquea un usuario bloqueado
void form_usuarios::on_boton_desbloquear_clicked()
{
    QString info;

    clase_ldap *ldap1 = new clase_ldap(host,puerto,usuarioLdap,dominio,password,&info);

    if (!info.contains("ERROR")) {

        ldap1->desbloquear_usuario(DN, &info);

        actualiza_usuario();

   }
    else
         ui->label_estado->setStyleSheet("background-color:yellow");

    ui->label_estado->setText(info);

    delete ldap1;
}


//renovar caducidad de la contraseña
void form_usuarios::on_boton_renovar_clicked()
{

    QString info;

    clase_ldap *ldap1 = new clase_ldap(host,puerto,usuarioLdap,dominio,password,&info);

    if (!info.contains("ERROR")) {

        ldap1->renovar_clave_usuario(DN, &info);

        actualiza_usuario();
   }
   else
        ui->label_estado->setStyleSheet("background-color:yellow");

    ui->label_estado->setText(info);

    delete ldap1;

}

//habilita o deshabilita el usuario actual, dependiendo de su estado actual
void form_usuarios::on_boton_habilitar_clicked()
{

    QString info;

    clase_ldap *ldap1 = new clase_ldap(host,puerto,usuarioLdap,dominio,password,&info);

    if (!info.contains("ERROR")) {

        ldap1->habilitar_usuario(ui->comboBox_usuarios->currentText(), DN, &info);

        actualiza_usuario();
   }
   else
        ui->label_estado->setStyleSheet("background-color:yellow");

    ui->label_estado->setText(info);

    delete ldap1;
}


void form_usuarios::on_boton_guardar_clicked()
{
    if (cambios_1 || cambios_2){

        //antes de guardar comprobamos que la fecha de la ultima sincronización siempre
        //sea posterior a la fecha de la ultima modificación del usuario en el servidor
        QString info;

        usuario *usu = new usuario;
        clase_ldap *ldap1 = new clase_ldap(host,puerto,usuarioLdap,dominio,password,&info);

        if (!info.contains("ERROR")) {

            *usu=ldap1->Consulta_Usuario(us_usuario, DN, &info);
            qDebug()<<"Ultima modif "<<usu->cual_es_modificacion_cuenta();
            qDebug()<<"Ultima sincro "<<ultima_sincro.toString("dd-MM-yyyy hh:mm:ss");

            QDateTime fecha_modif = QDateTime::fromString(usu->cual_es_modificacion_cuenta(),"dd-MM-yyyy hh:mm:ss");

            if(fecha_modif>=ultima_sincro){

                QMessageBox msgBox;
                msgBox.setText("El usuario no está actualizado con los últimos cambios en el servidor");
                msgBox.setInformativeText("¿Desea actualizar el ususario?");
                msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
                msgBox.setDefaultButton(QMessageBox::Yes);
                int elegido = msgBox.exec();

                switch (elegido) {
                   case QMessageBox::Yes:
                       actualiza_usuario();
                       break;
                   case QMessageBox::No:
                       break;
                 }
            }

            else {
                if (cambios_1)
                    guarda_datos_1();
                if (cambios_2)
                    guarda_datos_2();
            }

            delete usu;

        }

        ui->label_estado->setText(info);
        //*****************************************************************************


    }
}

void form_usuarios::guarda_datos_1(){

    QString info;
    qDebug()<<"guarda datos 1";

    clase_ldap *ldap1 = new clase_ldap(host,puerto,usuarioLdap,dominio,password,&info);

    if (!info.contains("ERROR")) {

        ldap1->guardar_fecha_caduca(ui->text_cuenta_caduca->text(), DN, &info);
        ldap1->guarda_correo(ui->text_correo->text(), DN, &info);
        ldap1->guarda_telefono(ui->text_telefono->text(), DN, &info);
        ldap1->guarda_descripcion(ui->text_descripcion->toPlainText(), DN, &info);

        //si está marcada
        ldap1->guarda_debe_cambiar_pass(ui->checkBox_cambiar_pass->isChecked(), DN, &info);
        ldap1->guarda_pass_nunca_expira(ui->checkBox_pass_nunca_expira->isChecked(),ui->comboBox_usuarios->currentText(), DN, &info);

        if (cambios_2 == false)
            actualiza_usuario();

        //cambios_1=false;
    }
    else
        ui->label_estado->setStyleSheet("background-color:yellow");

    ui->label_estado->setText(info);

    delete ldap1;

}


void form_usuarios::guarda_datos_2(){

    QString info;
    QStringList otrosdatos_valor;

    qDebug()<<"guarda datos 2";

    clase_ldap *ldap1 = new clase_ldap(host,puerto,usuarioLdap,dominio,password,&info);

    if (!info.contains("ERROR")) {

        //----guardamos los distintos campos del usuario
        /*otrosdatos_valor<<ui->text_usuario->text();
        otrosdatos_valor<<ui->text_usuariow2000->text();*/
        otrosdatos_valor<<ui->text_nombre->text();
        otrosdatos_valor<<ui->text_apellidos->text();
        otrosdatos_valor<<ui->text_nombremostrar->text();
        otrosdatos_valor<<ui->text_movil->text();
        otrosdatos_valor<<ui->text_fax->text();
        otrosdatos_valor<<ui->text_departamento->text();
        otrosdatos_valor<<ui->text_oficina->text();
        otrosdatos_valor<<ui->text_organizacion->text();
        otrosdatos_valor<<ui->text_puesto->text();
        otrosdatos_valor<<ui->text_provincia->text();
        otrosdatos_valor<<ui->text_localidad->text();
        otrosdatos_valor<<ui->text_direccion->text();
        otrosdatos_valor<<ui->text_cp->text();
        otrosdatos_valor<<ui->text_notas->toPlainText();;

        ldap1->guarda_otrosdatos(otrosdatos_valor,ui->comboBox_usuarios->currentText(),DN,&info);

        actualiza_usuario();

        //cambios_2=false;
    }
    else
        ui->label_estado->setStyleSheet("background-color:yellow");

    ui->label_estado->setText(info);

    delete ldap1;

}


void form_usuarios::on_boton_grupo_clicked()
{

    //llamamos al constructor de form_grupos pasandole el puntero al formulario padre (this)
    //para poder llamar desde form_grupos a los metodos de form_usuarios
    ventana_grupos= new form_grupos(this);
    ventana_grupos->setModal(true);
    ventana_grupos->show();

}


void form_usuarios::on_checkCaduca_clicked()
{
    if (ui->checkCaduca->isChecked()==false)
        ui->text_cuenta_caduca->setText(QDateTime::currentDateTime().toString("dd-MM-yyyy  hh:mm"));
    else
        ui->text_cuenta_caduca->setText("No Caduca");

}

void form_usuarios::on_text_correo_textEdited(const QString &arg1)
{
    //ui->label_3->setStyleSheet("color: rgb(164, 0, 0)");
    cambios_1=true;
}


void form_usuarios::on_text_telefono_textEdited(const QString &arg1)
{
    cambios_1=true;

}

void form_usuarios::on_text_descripcion_textChanged()
{
    cambios_1=true;

}

void form_usuarios::on_text_cuenta_caduca_textChanged(const QString &arg1)
{
    cambios_1=true;

}


//esta función es para capturar el evento cuando se cierra la ventana
void form_usuarios::closeEvent(QCloseEvent *event){


     if (cambios_1 || cambios_2) {

         QMessageBox msgBox;
         msgBox.setText("El usuario ha sido modificado");
         msgBox.setInformativeText("¿Deseas guardar los cambios?");
         msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Ignore| QMessageBox::Cancel );
         msgBox.setDefaultButton(QMessageBox::Save);
         int elegido = msgBox.exec();

         switch (elegido) {
            case QMessageBox::Save:
                if (cambios_1){
                    guarda_datos_1();
                }
                if (cambios_2){
                    guarda_datos_2();
                }
                //on_boton_guardar_clicked();
                event->accept();
                break;
            case QMessageBox::Ignore:
                event->accept();
                break;
            case QMessageBox::Cancel:
                event->ignore();
                break;
          }
     }

}


void form_usuarios::on_checkBox_cambiar_pass_clicked()
{
    cambios_1=true;
    if (ui->checkBox_cambiar_pass->isChecked()==true)
        ui->checkBox_pass_nunca_expira->setChecked(false);

}

void form_usuarios::on_checkBox_pass_nunca_expira_clicked()
{
    cambios_1=true;
    if (ui->checkBox_pass_nunca_expira->isChecked()==true)
        ui->checkBox_cambiar_pass->setChecked(false);

}


void form_usuarios::on_boton_sacar_clicked()
{
    BaseDatos *basedatos = new BaseDatos();
    QString info;

    //si hay algun grupo sellecionado de la lista
    if (ui->listView_grupos->currentIndex().row()>=0){

        clase_ldap *ldap1 = new clase_ldap(host,puerto,usuarioLdap,dominio,password,&info);

        if (!info.contains("ERROR")) {

            ldap1->quitar_grupo(grupos[ui->listView_grupos->currentIndex().row()],DN, &info);
            grupos.removeAt(ui->listView_grupos->currentIndex().row());

            actualiza_usuario();
       }
       else
            ui->label_estado->setStyleSheet("background-color:yellow");

        ui->label_estado->setText(info);

        delete ldap1;

        //actualizamoslos grupos en la base de datos ******************************************
        QSqlQuery* consulta = new QSqlQuery(db_sqlite);

         usuario *usu = new usuario;
         ldap1 = new clase_ldap(host,puerto,usuarioLdap,dominio,password,&info);

         if (!info.contains("ERROR")) {

             *usu=ldap1->Consulta_Usuario(us_usuario, DN, &info);

             if (!info.contains("ERROR")) {

                 //eliminar los grupos del usuario
                 basedatos->elimina_grupos_usuario(consulta,id_usuario);
                 //consulta->exec("delete from grupos where id_usuario=" + QString::number(id_usuario));

                //actualizar los grupos del usuario
                for (int j = 0; j < usu->cual_es_grupos().size(); ++j) {

                    //insertamos los grupos del usuario en la base de datos
                    basedatos->insertar_grupo(consulta, id_usuario, *usu,j);

                }

                //id_tmp=id_tmp+1;

                //qDebug()<<entrada.estado;

                delete ldap1;
                delete usu;
                delete basedatos;
             }
        }

        ui->label_estado->setText(info);
    }
}

QStringList form_usuarios::lee_grupos(){

    return(grupos);

}


void form_usuarios::refresca_grupos(){


    nuevos_grupos.clear();

    //añadimos a grupos los seleccionados en la ventana form_grupos
    foreach (const QString &str, ventana_grupos->obtenerLista()) {
        //guardamos como grupos nuevos los grupos a los que no pertenece ya el usuario
        if (!grupos.contains(str,0)){
            grupos.append(str);
            nuevos_grupos.append(str);
        }
     }

    delete (ventana_grupos);

    //actualizamos los grupos en LDAP ******************************************************
    QString info;
    clase_ldap *ldap1 = new clase_ldap(host,puerto,usuarioLdap,dominio,password,&info);

    if (!info.contains("ERROR")) {

        //grupos
        foreach (const QString &str, nuevos_grupos) {
            ldap1->miembro_grupo(str,DN, &info);
        }
        nuevos_grupos.clear();

    }
    else
        ui->label_estado->setStyleSheet("background-color:yellow");

    ui->label_estado->setText(info);

    delete ldap1;

    QStringList nombre_grupo;
    foreach (const QString &str, grupos) {
        nombre_grupo.append(str.mid(str.indexOf("=")+1,str.indexOf(",")-str.indexOf("=")-1).toUpper());
    }

    QStringListModel *listModel = new QStringListModel(nombre_grupo, NULL);
    ui->listView_grupos->setModel(listModel);

    //actualizamoslos grupos en la base de datos ******************************************
    QSqlQuery* consulta = new QSqlQuery(db_sqlite);

     usuario *usu = new usuario;
     ldap1 = new clase_ldap(host,puerto,usuarioLdap,dominio,password,&info);

     if (!info.contains("ERROR")) {

         *usu=ldap1->Consulta_Usuario(us_usuario, DN, &info);

         if (!info.contains("ERROR")) {
            BaseDatos *basedatos = new BaseDatos();
             //eliminar los grupos del usuario
             basedatos->elimina_grupos_usuario(consulta,id_usuario);
             //consulta->exec("delete from grupos where id_usuario=" + QString::number(id_usuario));

            //actualizar los grupos del usuario
            for (int j = 0; j < usu->cual_es_grupos().size(); ++j) {

                //insertamos los grupos del usuario en la base de datos
                basedatos->insertar_grupo(consulta, id_usuario, *usu,j);

            }

            delete ldap1;
            delete basedatos;
            delete usu;
         }
    }

    ui->label_estado->setText(info);



    //qDebug()<<grupos;
}



void form_usuarios::on_text_usuario_textChanged(const QString &arg1)
{
    cambios_2=true;

}

void form_usuarios::on_text_usuariow2000_textChanged(const QString &arg1)
{
    cambios_2=true;

}

void form_usuarios::on_text_nombre_textChanged(const QString &arg1)
{
    cambios_2=true;

}

void form_usuarios::on_text_apellidos_textChanged(const QString &arg1)
{
    cambios_2=true;

}

void form_usuarios::on_text_nombremostrar_textChanged(const QString &arg1)
{
    cambios_2=true;

}

void form_usuarios::on_text_movil_textChanged(const QString &arg1)
{
    cambios_2=true;

}

void form_usuarios::on_text_fax_textChanged(const QString &arg1)
{
    cambios_2=true;

}

void form_usuarios::on_text_departamento_textChanged(const QString &arg1)
{
    cambios_2=true;

}

void form_usuarios::on_text_oficina_textChanged(const QString &arg1)
{
    cambios_2=true;

}

void form_usuarios::on_text_organizacion_textChanged(const QString &arg1)
{
    cambios_2=true;

}

void form_usuarios::on_text_puesto_textChanged(const QString &arg1)
{
    cambios_2=true;

}

void form_usuarios::on_text_provincia_textChanged(const QString &arg1)
{
    cambios_2=true;

}

void form_usuarios::on_text_localidad_textChanged(const QString &arg1)
{
    cambios_2=true;

}

void form_usuarios::on_text_direccion_textChanged(const QString &arg1)
{
    cambios_2=true;

}

void form_usuarios::on_text_cp_textChanged(const QString &arg1)
{
    cambios_2=true;

}

void form_usuarios::on_text_notas_textChanged()
{
    cambios_2=true;

}
