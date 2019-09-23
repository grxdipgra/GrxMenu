#include "paginaweb.h"
#include "ui_paginaweb.h"
#include "lib/lib.h"
#include <configuracion/configuracion.h>
#include <QNetworkReply>

paginaWeb::paginaWeb(QString url,int web,int Tipo,QString Centro,int ID, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::paginaWeb)
{
    ui->setupUi(this);
    ui->webView->load(url);
    padre = web;
    tipo=Tipo;
    centro=Centro;
    id=ID;
    connect(ui->webView->page(), SIGNAL(loadFinished(bool)), this, SLOT(paginaCargada(bool)));
    connect(ui->webView->page(), SIGNAL(loadProgress(int)),this,SLOT(porcentaje(int)));
    connect(ui->webView->page(), SIGNAL(authenticationRequired(QUrl,QAuthenticator*)) , SLOT(autenticacion(QUrl,QAuthenticator*)));
}

paginaWeb::~paginaWeb()
{
    delete ui;
}

void paginaWeb::autenticacion(QUrl,QAuthenticator* auth){
    Configuracion *configuracion = new Configuracion;
    auth->setUser(configuracion->cual_es_tecnico());
    auth->setPassword(configuracion->cual_es_clave());
    delete configuracion;
}

void paginaWeb::paginaCargada(bool carga) {

    if (carga){
        if (padre==WEB_ATALAYA) {

            ui->webView->page()->runJavaScript(QString("if (typeof (document.getElementById('ctl00_BodyContent_Username')) !=='undefined') "
                                                       "if (typeof (document.getElementById('ctl00_BodyContent_Username')) !=='null') "
                                                            "document.getElementById('ctl00_BodyContent_Username').value = 'municipios';"
                                                       "if (typeof (document.getElementById('ctl00_BodyContent_Password')) !=='undefined') "
                                                       "if (typeof (document.getElementById('ctl00_BodyContent_Password')) !=='null')"
                                                            "document.getElementById('ctl00_BodyContent_Password').value = 'CasaCuna';"
                                                       "if (typeof (document.getElementsByClassName('sw-btn-primary sw-btn')) !=='undefined') "
                                                       "if (typeof (document.getElementsByClassName('sw-btn-primary sw-btn')) !=='null')"
                                                            "document.getElementsByClassName('sw-btn-primary sw-btn')[0].click();"));
        }else if (padre==WEB_GLPI){
            if (tipo==0){
               ui->webView->page()->runJavaScript(QString("document.getElementById('ui-id-6').click();"));
            }else if (tipo==1){
                ui->webView->page()->runJavaScript(QString( "document.getElementById('ui-id-6').click();"
                                                            //"alert (window.location);"
                                                            "if (window.location == 'https://incidencias.dipgra.es/glpi/front/central.php'){"
                                                                "window.location.href = 'https://incidencias.dipgra.es/glpi/front/ticket.form.php';}"
                                                            "if (window.location == 'https://incidencias.dipgra.es/glpi/front/central.php?active_entity="+QString::number(id)+"'){"
                                                                "window.location.href = 'https://incidencias.dipgra.es/glpi/front/ticket.form.php';}"
                                                            ));

        }
     }
}
}
void paginaWeb::porcentaje(int porcentaje) {
   if (porcentaje==100){
      // paginaCargada(porcentaje);
       //qDebug()<< contador;
       //ui->progressBar->setValue(porcentaje);
    }
}
