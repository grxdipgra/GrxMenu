/********************************************************************************
** Form generated from reading UI file 'soporte.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SOPORTE_H
#define UI_SOPORTE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Soporte
{
public:
    QFrame *frame;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QLabel *label;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QVBoxLayout *verticalLayout_2;
    QComboBox *cb_sede;
    QLineEdit *lineEdit_ip;
    QLineEdit *lineEdit_telefono;
    QLineEdit *lineEdit_extension;
    QLineEdit *lineEdit_direccion;
    QLineEdit *lineEdit_numero;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QVBoxLayout *verticalLayout_4;
    QLineEdit *lineEdit_adsl;
    QLineEdit *lineEdit_n_adm;
    QLineEdit *lineEdit_centro;
    QLineEdit *lineEdit_telefono_centro;
    QLineEdit *lineEdit_servicio;
    QLineEdit *lineEdit_caudal;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *Btn_Buscar;
    QPushButton *Btn_Ping;
    QPushButton *Btn_Incidencia;
    QPushButton *Btn_Mtr;
    QPushButton *Btn_Atalaya;
    QPushButton *Btn_Glpi;
    QLabel *Estado;
    QPushButton *Btn_Limpiar;
    QFrame *frame_2;
    QPushButton *pB_gmenu;
    QPushButton *pB_gtablon;
    QPushButton *pB_isl;
    QPushButton *pB_citrix;
    QPushButton *pB_epol;
    QPushButton *pB_aytos;
    QPushButton *pB_portafirmas;
    QPushButton *pB_listin;
    QPushButton *pB_perilContratante;
    QPushButton *pB_guadaltel;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout_2;
    QTabWidget *tabWidget;
    QWidget *consola;
    QGridLayout *gridLayout;
    QPlainTextEdit *TextoSalida;

    void setupUi(QDialog *Soporte)
    {
        if (Soporte->objectName().isEmpty())
            Soporte->setObjectName(QStringLiteral("Soporte"));
        Soporte->resize(841, 804);
        Soporte->setStyleSheet(QStringLiteral("color= red;"));
        frame = new QFrame(Soporte);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(10, 90, 821, 231));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush1(QColor(210, 239, 228, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        frame->setPalette(palette);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Plain);
        frame->setLineWidth(2);
        layoutWidget = new QWidget(frame);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 10, 785, 211));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setSpacing(2);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_2);

        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_3);

        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_4);

        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_5);

        label_6 = new QLabel(layoutWidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_6);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        cb_sede = new QComboBox(layoutWidget);
        cb_sede->setObjectName(QStringLiteral("cb_sede"));
        cb_sede->setMinimumSize(QSize(320, 0));
        cb_sede->setEditable(true);

        verticalLayout_2->addWidget(cb_sede);

        lineEdit_ip = new QLineEdit(layoutWidget);
        lineEdit_ip->setObjectName(QStringLiteral("lineEdit_ip"));
        lineEdit_ip->setMinimumSize(QSize(320, 27));
        lineEdit_ip->setFocusPolicy(Qt::StrongFocus);

        verticalLayout_2->addWidget(lineEdit_ip);

        lineEdit_telefono = new QLineEdit(layoutWidget);
        lineEdit_telefono->setObjectName(QStringLiteral("lineEdit_telefono"));
        lineEdit_telefono->setMinimumSize(QSize(320, 27));

        verticalLayout_2->addWidget(lineEdit_telefono);

        lineEdit_extension = new QLineEdit(layoutWidget);
        lineEdit_extension->setObjectName(QStringLiteral("lineEdit_extension"));
        lineEdit_extension->setMinimumSize(QSize(320, 27));

        verticalLayout_2->addWidget(lineEdit_extension);

        lineEdit_direccion = new QLineEdit(layoutWidget);
        lineEdit_direccion->setObjectName(QStringLiteral("lineEdit_direccion"));
        lineEdit_direccion->setMinimumSize(QSize(320, 27));

        verticalLayout_2->addWidget(lineEdit_direccion);

        lineEdit_numero = new QLineEdit(layoutWidget);
        lineEdit_numero->setObjectName(QStringLiteral("lineEdit_numero"));
        lineEdit_numero->setMinimumSize(QSize(320, 27));

        verticalLayout_2->addWidget(lineEdit_numero);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label_7 = new QLabel(layoutWidget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_7);

        label_8 = new QLabel(layoutWidget);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_8);

        label_9 = new QLabel(layoutWidget);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_9);

        label_10 = new QLabel(layoutWidget);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_10);

        label_11 = new QLabel(layoutWidget);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_11);

        label_12 = new QLabel(layoutWidget);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_12);


        horizontalLayout->addLayout(verticalLayout_3);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        lineEdit_adsl = new QLineEdit(layoutWidget);
        lineEdit_adsl->setObjectName(QStringLiteral("lineEdit_adsl"));
        lineEdit_adsl->setMinimumSize(QSize(320, 27));

        verticalLayout_4->addWidget(lineEdit_adsl);

        lineEdit_n_adm = new QLineEdit(layoutWidget);
        lineEdit_n_adm->setObjectName(QStringLiteral("lineEdit_n_adm"));
        lineEdit_n_adm->setMinimumSize(QSize(320, 27));

        verticalLayout_4->addWidget(lineEdit_n_adm);

        lineEdit_centro = new QLineEdit(layoutWidget);
        lineEdit_centro->setObjectName(QStringLiteral("lineEdit_centro"));
        lineEdit_centro->setMinimumSize(QSize(320, 27));

        verticalLayout_4->addWidget(lineEdit_centro);

        lineEdit_telefono_centro = new QLineEdit(layoutWidget);
        lineEdit_telefono_centro->setObjectName(QStringLiteral("lineEdit_telefono_centro"));
        lineEdit_telefono_centro->setMinimumSize(QSize(320, 27));

        verticalLayout_4->addWidget(lineEdit_telefono_centro);

        lineEdit_servicio = new QLineEdit(layoutWidget);
        lineEdit_servicio->setObjectName(QStringLiteral("lineEdit_servicio"));
        lineEdit_servicio->setMinimumSize(QSize(320, 27));

        verticalLayout_4->addWidget(lineEdit_servicio);

        lineEdit_caudal = new QLineEdit(layoutWidget);
        lineEdit_caudal->setObjectName(QStringLiteral("lineEdit_caudal"));
        lineEdit_caudal->setMinimumSize(QSize(320, 27));

        verticalLayout_4->addWidget(lineEdit_caudal);


        horizontalLayout->addLayout(verticalLayout_4);

        layoutWidget1 = new QWidget(Soporte);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 325, 517, 51));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        Btn_Buscar = new QPushButton(layoutWidget1);
        Btn_Buscar->setObjectName(QStringLiteral("Btn_Buscar"));
        Btn_Buscar->setMinimumSize(QSize(0, 40));

        horizontalLayout_2->addWidget(Btn_Buscar);

        Btn_Ping = new QPushButton(layoutWidget1);
        Btn_Ping->setObjectName(QStringLiteral("Btn_Ping"));
        Btn_Ping->setMinimumSize(QSize(0, 40));

        horizontalLayout_2->addWidget(Btn_Ping);

        Btn_Incidencia = new QPushButton(layoutWidget1);
        Btn_Incidencia->setObjectName(QStringLiteral("Btn_Incidencia"));
        Btn_Incidencia->setMinimumSize(QSize(0, 40));

        horizontalLayout_2->addWidget(Btn_Incidencia);

        Btn_Mtr = new QPushButton(layoutWidget1);
        Btn_Mtr->setObjectName(QStringLiteral("Btn_Mtr"));
        Btn_Mtr->setMinimumSize(QSize(0, 40));

        horizontalLayout_2->addWidget(Btn_Mtr);

        Btn_Atalaya = new QPushButton(layoutWidget1);
        Btn_Atalaya->setObjectName(QStringLiteral("Btn_Atalaya"));
        Btn_Atalaya->setMinimumSize(QSize(0, 40));

        horizontalLayout_2->addWidget(Btn_Atalaya);

        Btn_Glpi = new QPushButton(layoutWidget1);
        Btn_Glpi->setObjectName(QStringLiteral("Btn_Glpi"));
        Btn_Glpi->setMinimumSize(QSize(0, 40));

        horizontalLayout_2->addWidget(Btn_Glpi);

        Estado = new QLabel(Soporte);
        Estado->setObjectName(QStringLiteral("Estado"));
        Estado->setGeometry(QRect(560, 330, 41, 41));
        Btn_Limpiar = new QPushButton(Soporte);
        Btn_Limpiar->setObjectName(QStringLiteral("Btn_Limpiar"));
        Btn_Limpiar->setGeometry(QRect(700, 334, 80, 31));
        QIcon icon;
        icon.addFile(QStringLiteral(":/imagenes/iconos/soporte/escoba.svg"), QSize(), QIcon::Normal, QIcon::Off);
        Btn_Limpiar->setIcon(icon);
        Btn_Limpiar->setIconSize(QSize(30, 30));
        frame_2 = new QFrame(Soporte);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setGeometry(QRect(130, 10, 571, 61));
        frame_2->setMinimumSize(QSize(570, 61));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        pB_gmenu = new QPushButton(frame_2);
        pB_gmenu->setObjectName(QStringLiteral("pB_gmenu"));
        pB_gmenu->setGeometry(QRect(40, 10, 41, 41));
        pB_gmenu->setToolTipDuration(-1);
        QIcon icon1;
        icon1.addFile(QStringLiteral("../../../usr/share/grx/asistencia/imagenes/soporte/moad.png"), QSize(), QIcon::Normal, QIcon::Off);
        pB_gmenu->setIcon(icon1);
        pB_gmenu->setIconSize(QSize(30, 30));
        pB_gtablon = new QPushButton(frame_2);
        pB_gtablon->setObjectName(QStringLiteral("pB_gtablon"));
        pB_gtablon->setGeometry(QRect(140, 10, 41, 41));
        pB_gtablon->setToolTipDuration(-1);
        QIcon icon2;
        icon2.addFile(QStringLiteral("../../../usr/share/grx/asistencia/imagenes/soporte/gtablon.png"), QSize(), QIcon::Normal, QIcon::Off);
        pB_gtablon->setIcon(icon2);
        pB_gtablon->setIconSize(QSize(30, 30));
        pB_isl = new QPushButton(frame_2);
        pB_isl->setObjectName(QStringLiteral("pB_isl"));
        pB_isl->setGeometry(QRect(440, 10, 41, 41));
        QIcon icon3;
        icon3.addFile(QStringLiteral("../../../usr/share/grx/asistencia/imagenes/soporte/ISL50x50.png"), QSize(), QIcon::Normal, QIcon::Off);
        pB_isl->setIcon(icon3);
        pB_isl->setIconSize(QSize(30, 30));
        pB_citrix = new QPushButton(frame_2);
        pB_citrix->setObjectName(QStringLiteral("pB_citrix"));
        pB_citrix->setGeometry(QRect(340, 10, 41, 41));
        pB_citrix->setToolTipDuration(-1);
        QIcon icon4;
        icon4.addFile(QStringLiteral("../../../usr/share/grx/asistencia/imagenes/soporte/citrix.png"), QSize(), QIcon::Normal, QIcon::Off);
        pB_citrix->setIcon(icon4);
        pB_citrix->setIconSize(QSize(30, 30));
        pB_epol = new QPushButton(frame_2);
        pB_epol->setObjectName(QStringLiteral("pB_epol"));
        pB_epol->setGeometry(QRect(190, 10, 41, 41));
        pB_epol->setToolTipDuration(-1);
        QIcon icon5;
        icon5.addFile(QStringLiteral("../../../usr/share/grx/asistencia/imagenes/soporte/epol.png"), QSize(), QIcon::Normal, QIcon::Off);
        pB_epol->setIcon(icon5);
        pB_epol->setIconSize(QSize(30, 30));
        pB_aytos = new QPushButton(frame_2);
        pB_aytos->setObjectName(QStringLiteral("pB_aytos"));
        pB_aytos->setGeometry(QRect(390, 10, 41, 41));
        pB_aytos->setToolTipDuration(-1);
        QIcon icon6;
        icon6.addFile(QStringLiteral("../../../usr/share/grx/asistencia/imagenes/soporte/aytos.png"), QSize(), QIcon::Normal, QIcon::Off);
        pB_aytos->setIcon(icon6);
        pB_aytos->setIconSize(QSize(30, 30));
        pB_portafirmas = new QPushButton(frame_2);
        pB_portafirmas->setObjectName(QStringLiteral("pB_portafirmas"));
        pB_portafirmas->setGeometry(QRect(90, 10, 41, 41));
        pB_portafirmas->setToolTipDuration(-1);
        QIcon icon7;
        icon7.addFile(QStringLiteral("../../../usr/share/grx/asistencia/imagenes/soporte/portafirmas.png"), QSize(), QIcon::Normal, QIcon::Off);
        pB_portafirmas->setIcon(icon7);
        pB_portafirmas->setIconSize(QSize(30, 30));
        pB_listin = new QPushButton(frame_2);
        pB_listin->setObjectName(QStringLiteral("pB_listin"));
        pB_listin->setGeometry(QRect(490, 10, 41, 41));
        pB_listin->setToolTipDuration(-1);
        QIcon icon8;
        icon8.addFile(QStringLiteral("../../../usr/share/grx/asistencia/imagenes/soporte/listin.jpeg"), QSize(), QIcon::Normal, QIcon::Off);
        pB_listin->setIcon(icon8);
        pB_listin->setIconSize(QSize(30, 30));
        pB_perilContratante = new QPushButton(frame_2);
        pB_perilContratante->setObjectName(QStringLiteral("pB_perilContratante"));
        pB_perilContratante->setGeometry(QRect(290, 10, 41, 41));
        pB_perilContratante->setToolTipDuration(-1);
        QIcon icon9;
        icon9.addFile(QStringLiteral("../../../usr/share/grx/asistencia/imagenes/soporte/perfilcontratante.png"), QSize(), QIcon::Normal, QIcon::Off);
        pB_perilContratante->setIcon(icon9);
        pB_perilContratante->setIconSize(QSize(30, 30));
        pB_guadaltel = new QPushButton(frame_2);
        pB_guadaltel->setObjectName(QStringLiteral("pB_guadaltel"));
        pB_guadaltel->setGeometry(QRect(240, 10, 41, 41));
        pB_guadaltel->setToolTipDuration(-1);
        QIcon icon10;
        icon10.addFile(QStringLiteral("../../../usr/share/grx/asistencia/imagenes/soporte/guadaltel.png"), QSize(), QIcon::Normal, QIcon::Off);
        pB_guadaltel->setIcon(icon10);
        pB_guadaltel->setIconSize(QSize(30, 30));
        scrollArea = new QScrollArea(Soporte);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setGeometry(QRect(20, 380, 811, 421));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 809, 419));
        gridLayout_2 = new QGridLayout(scrollAreaWidgetContents);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        tabWidget = new QTabWidget(scrollAreaWidgetContents);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setStyleSheet(QStringLiteral("color=blue"));
        consola = new QWidget();
        consola->setObjectName(QStringLiteral("consola"));
        gridLayout = new QGridLayout(consola);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        TextoSalida = new QPlainTextEdit(consola);
        TextoSalida->setObjectName(QStringLiteral("TextoSalida"));
        TextoSalida->setFocusPolicy(Qt::NoFocus);
        TextoSalida->setStyleSheet(QStringLiteral("color=blue"));
        TextoSalida->setReadOnly(false);

        gridLayout->addWidget(TextoSalida, 0, 0, 1, 1);

        tabWidget->addTab(consola, QString());

        gridLayout_2->addWidget(tabWidget, 0, 0, 1, 1);

        scrollArea->setWidget(scrollAreaWidgetContents);

        retranslateUi(Soporte);
        QObject::connect(lineEdit_ip, SIGNAL(returnPressed()), Btn_Buscar, SLOT(click()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Soporte);
    } // setupUi

    void retranslateUi(QDialog *Soporte)
    {
        Soporte->setWindowTitle(QApplication::translate("Soporte", "Soporte", 0));
        label_2->setText(QApplication::translate("Soporte", "Sede", 0));
        label->setText(QApplication::translate("Soporte", "Ip/red ", 0));
        label_3->setText(QApplication::translate("Soporte", "Tel\303\251fono", 0));
        label_4->setText(QApplication::translate("Soporte", "Extensi\303\263n", 0));
        label_5->setText(QApplication::translate("Soporte", "Direcci\303\263n", 0));
        label_6->setText(QApplication::translate("Soporte", "N\303\272mero", 0));
        label_7->setText(QApplication::translate("Soporte", "ADSL", 0));
        label_8->setText(QApplication::translate("Soporte", "N\302\272Adm", 0));
        label_9->setText(QApplication::translate("Soporte", "Centro", 0));
        label_10->setText(QApplication::translate("Soporte", "Tel\303\251fono", 0));
        label_11->setText(QApplication::translate("Soporte", "Servicio", 0));
        label_12->setText(QApplication::translate("Soporte", "Caudal", 0));
        Btn_Buscar->setText(QApplication::translate("Soporte", "Escanear", 0));
        Btn_Ping->setText(QApplication::translate("Soporte", "Ping", 0));
        Btn_Incidencia->setText(QApplication::translate("Soporte", "Incidencia", 0));
        Btn_Mtr->setText(QApplication::translate("Soporte", "mtr", 0));
        Btn_Atalaya->setText(QApplication::translate("Soporte", "SolarWinds", 0));
        Btn_Glpi->setText(QApplication::translate("Soporte", "GLPI", 0));
        Estado->setText(QString());
        Btn_Limpiar->setText(QApplication::translate("Soporte", "Limpiar", 0));
#ifndef QT_NO_TOOLTIP
        pB_gmenu->setToolTip(QApplication::translate("Soporte", "MOAD", 0));
#endif // QT_NO_TOOLTIP
        pB_gmenu->setText(QString());
#ifndef QT_NO_TOOLTIP
        pB_gtablon->setToolTip(QApplication::translate("Soporte", "GTabl\303\263n", 0));
#endif // QT_NO_TOOLTIP
        pB_gtablon->setText(QString());
#ifndef QT_NO_TOOLTIP
        pB_isl->setToolTip(QApplication::translate("Soporte", "ISLOnline", 0));
#endif // QT_NO_TOOLTIP
        pB_isl->setText(QString());
#ifndef QT_NO_TOOLTIP
        pB_citrix->setToolTip(QApplication::translate("Soporte", "Citrix", 0));
#endif // QT_NO_TOOLTIP
        pB_citrix->setText(QString());
#ifndef QT_NO_TOOLTIP
        pB_epol->setToolTip(QApplication::translate("Soporte", "EPol", 0));
#endif // QT_NO_TOOLTIP
        pB_epol->setText(QString());
#ifndef QT_NO_TOOLTIP
        pB_aytos->setToolTip(QApplication::translate("Soporte", "Incidencias Sede", 0));
#endif // QT_NO_TOOLTIP
        pB_aytos->setText(QString());
#ifndef QT_NO_TOOLTIP
        pB_portafirmas->setToolTip(QApplication::translate("Soporte", "Portafirmas", 0));
#endif // QT_NO_TOOLTIP
        pB_portafirmas->setText(QString());
#ifndef QT_NO_TOOLTIP
        pB_listin->setToolTip(QApplication::translate("Soporte", "Listin Telef\303\263nico", 0));
#endif // QT_NO_TOOLTIP
        pB_listin->setText(QString());
#ifndef QT_NO_TOOLTIP
        pB_perilContratante->setToolTip(QApplication::translate("Soporte", "Portal del Contratante", 0));
#endif // QT_NO_TOOLTIP
        pB_perilContratante->setText(QString());
#ifndef QT_NO_TOOLTIP
        pB_guadaltel->setToolTip(QApplication::translate("Soporte", "Redmine", 0));
#endif // QT_NO_TOOLTIP
        pB_guadaltel->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(consola), QApplication::translate("Soporte", "Consola", 0));
    } // retranslateUi

};

namespace Ui {
    class Soporte: public Ui_Soporte {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SOPORTE_H
