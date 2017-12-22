#ifndef ACERDADE_H
#define ACERDADE_H

#include <QDialog>

namespace Ui {
class AcerdaDe;
}

class AcerdaDe : public QDialog
{
    Q_OBJECT

public:
    explicit AcerdaDe(QWidget *parent = 0);
    ~AcerdaDe();

private slots:
    void on_pushButton_clicked();

private:
    Ui::AcerdaDe *ui;
};

#endif // ACERDADE_H
