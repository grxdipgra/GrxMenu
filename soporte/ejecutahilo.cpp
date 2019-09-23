#include "ejecutahilo.h"

ejecutaHilo::ejecutaHilo(QString ip_hosts,QString op){
     ip=ip_hosts;
     opciones=op;
}

void ejecutaHilo::ejecuta() {
    NMap *nmap = new NMap();
    NMapScan nmapscan;
    int errores=0;
    QList<NMapScan> resultado; //El resultado lo vamos a devolver en un QList


    /*int i=ip.length()-1;
    if (ip.at(i)=='0'){ //Si el ultimo numero de la ip despues del ultimo punto es un cero
        ip.chop(1);//ponemos un asterisco
        ip.append('*');
    }
    */
    errores = nmap->nmap_run_scan(opciones,ip);
    if (!errores){
            nmap->copy_nmapscan(nmapscan);
            resultado.append(nmapscan);
        }

    delete nmap;
    emit resultadoListo(resultado);
}
