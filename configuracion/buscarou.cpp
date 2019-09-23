#include "buscarou.h"
#include "ui_buscarou.h"
#include <QStandardItemModel>
#include "usuarios/clase_ldap.h"
#include <QDebug>

BuscarOU::BuscarOU(QString servidor_ldap,int puerto_ldap,QString usuario_ldap, QString dominio_ldap, QString clave_ldap,QStringList *ListaOU,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BuscarOU)
{
    ui->setupUi(this);
    servidor = servidor_ldap;
    usuario  =usuario_ldap;
    dominio  =dominio_ldap;
    clave    =clave_ldap;
    puerto   =puerto_ldap;
    lista_OU = ListaOU;
    buscar();
}



BuscarOU::~BuscarOU()
{
    delete ui;
}


void BuscarOU::on_checkBox_todoDominio_clicked()
{
    if (ui->checkBox_todoDominio->isChecked())
         ui->treeView_OU->selectAll();
    else
         ui->treeView_OU->selectionModel()->clearSelection();

}

//Función para crear una fila del treeview
QList<QStandardItem *> BuscarOU::creaFila(const QString &first)
{
    QList<QStandardItem *> rowItems;
    rowItems << new QStandardItem(first);
    return rowItems;
}

void BuscarOU::buscar()

{
    QStandardItemModel *standardModel = new QStandardItemModel ;

    QList<QStandardItem *> fila;
    typedef QList<QStandardItem *> tipomio;
    QList<tipomio> listas;
    //añadiendo una fila al invisibleRootItem producimos un elemento raiz
    QStandardItem *item = standardModel->invisibleRootItem();

    QString temp, basedn, info;

    clase_ldap * cldap = new clase_ldap(servidor,puerto,usuario,dominio,clave,&info);
    ui->label_estado->setText(info);
    QStringList lista_OU, lista_OU_temp;

    lista_OU_temp=cldap->Consultar_OU(&info);

    delete cldap;

    QString str = dominio;

    //sacamos las partes de la cadena del dominio separadas por puntos
    QStringList strList = str.split(".");
    //lo recorremos y lo guardamos en el formato basedn
    for (int i = 0; i < strList.size(); ++i) {
        basedn = basedn + "DC=" + strList.at(i);
        //añadimos una coma si no es la ultima parte
        if (i+1 < strList.size())
            basedn=basedn+",";
    }

    int nivel_0=strList.size();
    int nivel_actual=nivel_0;
    int i=0,j;
    QString strtemp;

    for (int x=0; x<lista_OU_temp.size(); x++) {

        strtemp=lista_OU_temp[x];

        //lo convertimos a lista de cadenas
        strList = strtemp.split("=");
        nivel_actual=strList.size()-1-nivel_0;
        lista_OU.insert(i,strtemp);
        fila =creaFila(strtemp);
        listas.insert(i,fila);

        //si el nivel es 1 es un elemento raiz
        if (nivel_actual==1){
            item->appendRow(listas[i]);
        }
        else{
            //recorremos todas las OU y vemos si quitandole la ultima parte OU=..., de la cadena
            //es igual a alguna de las que ya hay, si es así se crea como una rama de esta
            strtemp.remove(0,strtemp.indexOf("=",4)-2);
            qDebug()<<strtemp;
            for (j=0;j<lista_OU.count();j++)
                if (strtemp==lista_OU[j]){
                    listas[j].first()->appendRow(listas[i]);
                    j=lista_OU.count();
                }
        }

        i++;

    }
    emit resultadoListo(lista_OU);

    ui->treeView_OU->setModel(standardModel);
    ui->treeView_OU->expandAll();
    //activamos multiselección en el arbol
    ui->treeView_OU->setSelectionMode(3);
}


void BuscarOU::on_buttonBox_accepted()
{
    QStringList lista_OU;
    if (ui->treeView_OU->selectionModel()->selectedRows().count() > 0) {

        QModelIndexList idxSelectedItems = ui->treeView_OU->selectionModel()->selectedRows();

        if (idxSelectedItems.size()>0) {
            for (int i = 0; i < idxSelectedItems.size(); i++){
               lista_OU.append(idxSelectedItems[i].data().toString());
            }
        }
        emit botonAceptar(lista_OU);

    }
}
