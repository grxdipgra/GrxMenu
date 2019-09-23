#include "mailing.h"
#include "ui_mailing.h"
#include <QStandardItem>
#include <QDebug>
#include "lib/lib.h"
#include <QDesktopServices>
#include <QUrl>
Mailing::Mailing(QWidget *parent) :
    QScrollArea(parent),
    ui(new Ui::Mailing)
{

    ui->setupUi(this);
    connect (ui->tableView,SIGNAL( doubleClicked(QModelIndex)),this,SLOT(selecciona_todo()));
    QString sql = "Select nombre,id FROM nodo WHERE esAyuntamiento=1 ORDER BY nombre;";
    ui->tableView->setColumnHidden(1, true);
    db_sqlite = QSqlDatabase::database("sqlite");
    QSqlQuery* consultar_nodo = new QSqlQuery(db_sqlite);
    consultar_nodo->prepare(sql);
    if (!consultar_nodo->exec()) {
        QMessageBox::critical(this, "Sql Error", "Error en la consulta: \n", QMessageBox::Ok);
    }
    else {

        model->setQuery(*consultar_nodo);
        ui->tableView->setModel(model);
        ui->tableView->horizontalHeader()->setStretchLastSection(true);
        ui->tableView->setColumnHidden(1, true);
    }
}
Mailing::~Mailing()
{
    delete ui;
}

void Mailing::on_pushButton_clicked()
{
    QString sql_email, para,asunto,cuerpo;
    asunto=" ";
    cuerpo=" ";
    int idnodo=0;
    QSqlQuery* consultar_email = new QSqlQuery(db_sqlite);
    QStringList lista;
    for (int i=0;i<model->rowCount();i++){
           if ((model->data(model->index(i, 0),Qt::CheckStateRole))== Qt::Checked){
               consultar_email->prepare(QString("SELECT email FROM emailnodo WHERE idnodo = :idnodo"));
               idnodo=model->data(model->index(i, 1),0).toInt();
               consultar_email->bindValue(":idnodo", idnodo);
               if (consultar_email->exec()) {
                   if (consultar_email->first()) {
                       do {
                           lista << consultar_email->value(0).toString();
                       }while(consultar_email->next());
                   }
                   else {
                       QMessageBox::critical(this, "Sql Error", "Ha ocurrido un error consultando el correo", QMessageBox::Ok);
                   }
               }
           }
       }
   QStringList::const_iterator constIterator;
   for (constIterator = lista.constBegin(); constIterator != lista.constEnd();++constIterator){
       if (constIterator != lista.constBegin()){
           para.append(";");
       }
       para.append((*constIterator).toLocal8Bit().constData());
   }

   QDesktopServices::openUrl(QUrl("mailto: incidencias.mulhacen@dipgra.es?subject="+asunto+"&bcc="+para+"&body="+cuerpo, QUrl::TolerantMode));
}

void Mailing::on_comboBox_activated(const QString &sql)
{
    QString sql_consulta;

    sql_consulta = "Select nombre,id FROM nodo "+sql;

    if (!sql_consulta.isNull()) {
        QSqlQuery* query = new QSqlQuery(db_sqlite);
        query->prepare(sql_consulta);

        if (!query->exec(sql_consulta)) {
            QMessageBox::critical(this, "Sql Error", "Error en la consulta: \n", QMessageBox::Ok);
        }
        else {
            model->setQuery(*query);
            ui->tableView->setModel(model);
            ui->tableView->horizontalHeader()->setStretchLastSection(true);
        }
    }
}

void Mailing::selecciona_todo(){
    for (int i=0;i<model->rowCount();i++){
        if ((model->data(model->index(i, 0),Qt::CheckStateRole))== Qt::Checked)
            model->setData(model->index(i, 0),Qt::Unchecked,Qt::CheckStateRole);
        else
            model->setData(model->index(i, 0),Qt::Checked,Qt::CheckStateRole);
    }
    ui->tableView->clearFocus();

}
