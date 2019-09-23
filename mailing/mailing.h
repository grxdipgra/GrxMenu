#ifndef MAILING_H
#define MAILING_H

#include <QScrollArea>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QMessageBox>
#include "mailing/sqlquerymodel.h"

namespace Ui {
class Mailing;
}

class Mailing : public QScrollArea
{
    Q_OBJECT

public:
    explicit Mailing(QWidget *parent = 0);
    ~Mailing();
    SqlQueryModel *model=new SqlQueryModel;
    QSqlQueryModel *model_correo = new QSqlQueryModel();
    QSqlDatabase db_sqlite;
private slots:
    void on_pushButton_clicked();

    void selecciona_todo();

    void on_comboBox_activated(const QString &arg1);

private:
    Ui::Mailing *ui;
};

#endif // MAILING_H
