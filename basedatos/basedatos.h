#ifndef BASEDATOS_H
#define BASEDATOS_H

#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QTableView>
#include <QMessageBox>


namespace Ui {
class BaseDatos;
}

class BaseDatos : public QMainWindow
{
    Q_OBJECT

public:
    QSqlDatabase db_sqlite,db_mysql;
    QString sql[10];
    explicit BaseDatos(QWidget *parent = 0);
    ~BaseDatos();
    QSqlDatabase db = QSqlDatabase::database("sqlite");
    QSqlQueryModel *model_consulta = new QSqlQueryModel();
    QSqlTableModel *model_ldap = new QSqlTableModel(this,db);
    QSqlTableModel *model_municipio = new QSqlTableModel(this,db);
    QSqlTableModel *model_nodo = new QSqlTableModel(this,db);
    QSqlTableModel *model_poblacion = new QSqlTableModel(this,db);
    QSqlTableModel *model_programa = new QSqlTableModel(this,db);
    QSqlTableModel *model_emailnodo = new QSqlTableModel(this,db);
    QSqlTableModel *model_telefononodo = new QSqlTableModel(this,db);
    QSqlTableModel *model_comarca = new QSqlTableModel(this,db);
    QSqlTableModel *model_mancomunidad = new QSqlTableModel(this,db);
    QSqlTableModel *model_centro = new QSqlTableModel(this,db);
    QSqlTableModel *model_diafestivopoblacion = new QSqlTableModel(this,db);
    QSqlTableModel *model_aplicacion = new QSqlTableModel(this,db);

private slots:
    void cabeceras();
    void inicia();
    void on_pB_sql_clicked();
    void on_comboBox_consulta_activated(const QString &arg1);
    void on_pB_csv_clicked();

private:
    Ui::BaseDatos *ui;
};

#endif // BASEDATOS_H
