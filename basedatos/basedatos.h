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
    QSqlQueryModel *model_consulta = new QSqlQueryModel();
    QSqlTableModel *model_ldap = new QSqlTableModel();
    QSqlTableModel *model_municipio = new QSqlTableModel();
    QSqlTableModel *model_nodo = new QSqlTableModel();
    QSqlTableModel *model_poblacion = new QSqlTableModel();
    QSqlTableModel *model_programa = new QSqlTableModel();
    QSqlTableModel *model_emailnodo = new QSqlTableModel();
    QSqlTableModel *model_telefononodo = new QSqlTableModel();
    QSqlTableModel *model_comarca = new QSqlTableModel();
    QSqlTableModel *model_mancomunidad = new QSqlTableModel();
    QSqlTableModel *model_centro = new QSqlTableModel();
    QSqlTableModel *model_diafestivopoblacion = new QSqlTableModel();
    QSqlTableModel *model_aplicacion = new QSqlTableModel();

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
