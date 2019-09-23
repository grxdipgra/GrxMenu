#include "form_grupos.h"
#include "ui_form_grupos.h"

form_grupos::form_grupos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::form_grupos)
{
    ui->setupUi(this);
    ui->radioButton_GruposOU->setChecked(true);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Aceptar");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("Cancelar");

    Configuracion *config = new Configuracion;
    host = config->cual_es_servidor_ldap();
    puerto = config->cual_es_puerto_ldap();
    usuario = config->cual_es_usuario_ldap();
    dominio=config->cual_es_dominio_ldap();
    password=config->cual_es_clave_ldap();
    //para manejar metodos de la clase form_usuarios
    //le asignamos el formulario padre desde el que se abrió
    ventana_usuarios =parent;

    QStringList grupos_actuales=ventana_usuarios->lee_grupos();
    QString info;
    clase_ldap *ldap1 = new clase_ldap(host,puerto,usuario,dominio,password,&info);

    QStringList grupos_temp_2;//=ldap1->Consultar_grupos_dominio(&info);


    foreach (const QString &qstr, config->listaOU_datos()) {
        foreach (const QString &qstr1, ldap1->Consultar_grupos_OU(qstr,&info))
            grupos_temp_2<<qstr1;
    }

    for (int i=0; i<grupos_temp_2.size(); i++) {
        //insertamos solo los grupos a los que no pertenece el usuario
        if (!grupos_actuales.contains(grupos_temp_2.at(i),0)){
            //insertamos en la lista el nombre del grupo
            ui->listWidget_grupos->addItem(grupos_temp_2.at(i).mid(grupos_temp_2.at(i).indexOf("=")+1,grupos_temp_2.at(i).indexOf(",")-grupos_temp_2.at(i).indexOf("=")-1));
            grupos_temp.append(grupos_temp_2.at(i));
        }
    }

    //activamos multiselección
    ui->listWidget_grupos->setSelectionMode(3);

}


form_grupos::~form_grupos()
{
    //form_usuarios* n = form_usuarios(this);
    delete ui;
}


QStringList form_grupos::obtenerLista(){
    return grupos;
}


void form_grupos::on_buttonBox_accepted()
{
    //si se ha seleccionado algun item de la lista
    if (ui->listWidget_grupos->selectionModel()->selectedRows().count() > 0) {

        QModelIndexList idxSelectedItems = ui->listWidget_grupos->selectionModel()->selectedRows();
        if (idxSelectedItems.size()>0) {
            for (int i = 0; i < idxSelectedItems.size(); i++){
              grupos<<grupos_temp[idxSelectedItems[i].row()];
            }
        }

        ventana_usuarios->refresca_grupos();

    }

}


void form_grupos::on_radioButton_GruposDom_clicked()
{

    grupos_temp.clear();
    ui->listWidget_grupos->clear();

    QStringList grupos_actuales=ventana_usuarios->lee_grupos();
    QString info;

    clase_ldap *ldap1 = new clase_ldap(host,puerto,usuario,dominio,password,&info);

    QStringList grupos_temp_2=ldap1->Consultar_grupos_dominio(&info);

    //grupos_temp=grupos_temp_2;

    for (int i=0; i<grupos_temp_2.size(); i++) {
        //insertamos solo los grupos a los que no pertenece el usuario
        if (!grupos_actuales.contains(grupos_temp_2.at(i),0)){
            //insertamos en la lista el nombre del grupo
            ui->listWidget_grupos->addItem(grupos_temp_2.at(i).mid(grupos_temp_2.at(i).indexOf("=")+1,grupos_temp_2.at(i).indexOf(",")-grupos_temp_2.at(i).indexOf("=")-1));
            grupos_temp.append(grupos_temp_2.at(i));
        }
    }
}

void form_grupos::on_radioButton_GruposOU_clicked()
{
    grupos_temp.clear();
    ui->listWidget_grupos->clear();

    QStringList grupos_actuales=ventana_usuarios->lee_grupos();
    QString info;

    clase_ldap *ldap1 = new clase_ldap(host,puerto,usuario,dominio,password,&info);

    QStringList grupos_temp_2;//=ldap1->Consultar_grupos_dominio(&info);

    Configuracion *config = new Configuracion;


    foreach (const QString &qstr, config->listaOU_datos()) {
        foreach (const QString &qstr1, ldap1->Consultar_grupos_OU(qstr,&info))
            grupos_temp_2<<qstr1;
    }

    //grupos_temp=grupos_temp_2;

    for (int i=0; i<grupos_temp_2.size(); i++) {
        //insertamos solo los grupos a los que no pertenece el usuario
        if (!grupos_actuales.contains(grupos_temp_2.at(i),0)){
            //insertamos en la lista el nombre del grupo
            ui->listWidget_grupos->addItem(grupos_temp_2.at(i).mid(grupos_temp_2.at(i).indexOf("=")+1,grupos_temp_2.at(i).indexOf(",")-grupos_temp_2.at(i).indexOf("=")-1));
            grupos_temp.append(grupos_temp_2.at(i));
        }
    }
}

void form_grupos::on_checkBox_clicked()
{
    if (ui->checkBox->isChecked())
        ui->listWidget_grupos->selectAll();
    else
        ui->listWidget_grupos->clearSelection();
}
