#include "mame.h"
#include "ui_mame.h"

#include "QPushButton"
#include "QSignalMapper"
#include <QGridLayout>
#include <QScrollArea>
#include <QDirIterator>
#include <QDebug>

Mame::Mame(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Mame)
{
    ui->setupUi(this);

    QSignalMapper mapper;
    QGridLayout *gridlayout = new QGridLayout;
    QPixmap pixmap;
    QList <QPushButton*> ListButton;
    QSignalMapper *signalMapper = new QSignalMapper(this); //Lo usamos para mapear las señales de los botones
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(juego(int)));
    int columna=0,fila=0,contador=0;
    QDirIterator it("/usr/share/games/mame/roms", QDirIterator::Subdirectories);
    while (it.hasNext()) {
        it.next();
        if ((it.fileName()!=("."))&&(it.fileName()!=(".."))){
            columna = (contador / 5);//Para mostrar las filas y columnas
            fila = (contador % 5);
            auto button = new QPushButton();
            gridlayout->addWidget(button,fila,columna);
            QString nombre = "/usr/share/games/mame/artwork/"+it.fileName().split(".").first()+".png";
            pixmap.load(nombre);
            QIcon ButtonIcon(pixmap);
            button->setIcon(ButtonIcon);
            button->setIconSize(pixmap.rect().size());
            //button->setFixedSize(90,90);
            ListButton << button;
            ListaJuegos <<  it.fileName().split(".").first();
            signalMapper->setMapping(ListButton[contador], contador);
            connect(ListButton[contador], SIGNAL(released()), signalMapper, SLOT(map()));
            contador++;
        }

    }
    ui->scrollMame->setLayout(gridlayout);
}

Mame::~Mame()
{
    delete ui;
}
void Mame::juego(int i)
{

    QProcess process;
    process.startDetached("mame "+ListaJuegos[i]);


}


