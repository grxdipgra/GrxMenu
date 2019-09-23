#ifndef SQLQUERYMODEL_H
#define SQLQUERYMODEL_H
#include <QObject>
#include <QMap>
#include <QModelIndex>
#include <QSqlQueryModel>

class SqlQueryModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit SqlQueryModel(QObject *parent = 0);
    Qt::ItemFlags flags(const QModelIndex & index) const;
    QVariant data(const QModelIndex & index, int role) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role);
private:
    QMap<int, Qt::CheckState> check_state_map;
};
#endif // SQLQUERYMODEL_H

