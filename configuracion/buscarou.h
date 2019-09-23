#ifndef BUSCAROU_H
#define BUSCAROU_H

#include <QDialog>
#include <QStandardItem>

namespace Ui {
class BuscarOU;
}

class BuscarOU : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief BuscarOU Esto es una prueba de como funciona
     * @param servidor_ldap y esto
     * @param puerto_ldap y otro
     * @param usuario_ldap puff
     * @param dominio_ldap la po
     * @param clave_ldap si
     * @param ListaOU sss
     * @param parent
     */
    explicit BuscarOU(QString servidor_ldap,int puerto_ldap,QString usuario_ldap, QString dominio_ldap, QString clave_ldap, QStringList *ListaOU,QWidget *parent = 0);
    ~BuscarOU();

signals:
    /**
     * @brief resultadoListo
     * @param lista
     */
    void resultadoListo(QStringList lista);

    /**
     * @brief botonAceptar
     * @param lista
     */
    void botonAceptar(QStringList lista);
private:

    Ui::BuscarOU *ui;
    QString servidor, usuario, dominio, clave;
    int puerto;
    QStringList *lista_OU;
 private slots:
    QList<QStandardItem *> creaFila(const QString &first);
    void on_checkBox_todoDominio_clicked();
    void buscar();
    void on_buttonBox_accepted();
};

#endif // BUSCAROU_H
