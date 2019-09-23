#ifndef ACERCADE_H
#define ACERCADE_H

#include <QDialog>

namespace Ui {
class AcercaDe;
}

class AcercaDe : public QDialog
{
    Q_OBJECT

public:
    /*!
     * \file acercade.h
     * \brief Esta clase muestra a los programadores de la aplicación
     * \author Alberto Avidad Fernandez
     * \date 18/09/2019
     */
    explicit AcercaDe(QWidget *parent = 0);
    ~AcercaDe();

private slots:
    void on_pushButton_clicked();



private:
    Ui::AcercaDe *ui;
};

#endif // ACERCADE_H



