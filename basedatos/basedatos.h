#ifndef BASEDATOS_H
#define BASEDATOS_H

#define COMPLETO true
#define INCOMPLETO false
#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QTableView>
#include <QMessageBox>
#include "usuarios/usuario.h"
#include "usuarios/clase_ldap.h"
#include "lib/globals.h"
#include <QDate>

extern bool conectado;

namespace Ui {
class BaseDatos;
}

class BaseDatos : public QMainWindow
{
    Q_OBJECT

public:

    struct variables{
        QString keyfile1;
        QString keyfile2;
        QString username_ssh;
        QString password_ssh;
        QString username_DB;
        QString password_DB;
        QString server_ip;
        QString local_listenip;
        unsigned int remote_port;
        unsigned int local_listenport;
        QString remote_desthost;
        QString databasename;
        QString hostname_DB;
        unsigned int remote_destport;
        bool usar_ssh;
    }datos;

    QString sql[10];
    explicit BaseDatos(QWidget *parent = 0);
    explicit BaseDatos(bool,QWidget *parent = 0);


    ~BaseDatos();
    //Creamos los conectores de las bases de datos

    //QSqlDatabase db_mysql = QSqlDatabase::addDatabase("QMYSQL","mysql");
    QSqlDatabase db_mysql;
    QSqlDatabase db_sqlite = QSqlDatabase::database("sqlite");

    QSqlQueryModel *model_consulta = new QSqlQueryModel();
    QSqlTableModel *model_ldap = new QSqlTableModel(this,db_sqlite);
    QSqlTableModel *model_municipio = new QSqlTableModel(this,db_sqlite);
    QSqlTableModel *model_nodo = new QSqlTableModel(this,db_sqlite);
    QSqlTableModel *model_poblacion = new QSqlTableModel(this,db_sqlite);
    QSqlTableModel *model_programa = new QSqlTableModel(this,db_sqlite);
    QSqlTableModel *model_emailnodo = new QSqlTableModel(this,db_sqlite);
    QSqlTableModel *model_telefononodo = new QSqlTableModel(this,db_sqlite);
    QSqlTableModel *model_comarca = new QSqlTableModel(this,db_sqlite);
    QSqlTableModel *model_mancomunidad = new QSqlTableModel(this,db_sqlite);
    QSqlTableModel *model_centro = new QSqlTableModel(this,db_sqlite);
    QSqlTableModel *model_diafestivopoblacion = new QSqlTableModel(this,db_sqlite);
    QSqlTableModel *model_aplicacion = new QSqlTableModel(this,db_sqlite);

public slots:
   bool compruebaDB();
   bool crearDB(QString rutaDB);
   bool actualizaDB();
   bool inicia_mysql();
   bool inicia_sqlite();
   void borrar_tablas(QSqlQuery* consulta);
   void borrar_datos(QSqlQuery* consulta);
   bool insertar_usuario(QSqlQuery* consulta, int id_usuario, usuario usu);
   QStringList consulta_grupos(QSqlQuery* consulta, int id_usuario);
   void insertar_grupo(QSqlQuery* consulta, int id_usuario, usuario usu, int id);
   void actualiza_usuario(QSqlQuery* consulta, int id_usuario, usuario usu);
   void elimina_grupos_usuario(QSqlQuery* consulta, int id_usuario);
   usuario consulta_usuario(QSqlQuery* consulta, QString filtro_usuario, int tipo_consulta);
   void inicia();
   void actualizar_usuarios();

private slots:
    void cabeceras();
    bool cargaVariables();
    bool creaConexion();
    void configuraBasedatos();
    void on_pB_sql_clicked();
    void on_comboBox_consulta_activated(const QString &arg1);
    void on_pB_csv_clicked();
    void on_pushButton_clicked();
    bool carga_OU();

signals:
    void conectadoDB();
private:
    Ui::BaseDatos *ui;
    QStringList OU;
};

#endif // BASEDATOS_H
