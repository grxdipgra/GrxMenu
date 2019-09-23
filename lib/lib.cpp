#include "lib.h"
#include <QFileInfo>
#include <QTcpServer>
#include <QRegularExpression>

// Esta funcion comprueba que el archivo pasado por parametro
// exista y no sea un directorio

bool fileExists(QString path) {
    QFileInfo check_file(path);
    return (check_file.exists() && check_file.isFile());
}
bool dirExists(QString path) {
    QFileInfo check_file(path);
    return (check_file.exists() && check_file.isDir());
}


char* convierte(QString dato){
    char* cstr;
    std::string fname = dato.toStdString();
    cstr = new char [fname.size()+1];
    strcpy( cstr, fname.c_str() );
    return cstr;
}

// Esta funcion busca un puerto tcp libre en el sistema
// Devuelve -1 si no ha podido encontrarlo
unsigned int puerto_libre(){
    QTcpServer server;
    if(server.listen(QHostAddress::Any, 0))
           return server.serverPort();
return -1;
}

//devuelve si el usuario está habilitado o no dependiendo del valor del campo useraccountcontrol
//estado_habilitado=0 si no está habilitado
//estado_habilitado=1 si está habilitado
//estado_habilitado=2 si no es un valor estandar
int estado_habilitado(int valor){

    //o divisores de 8
    if (valor==512 || valor==528 || valor==544 || valor==560 || valor==640 || valor==66048 || valor==66064 || valor==66080 || valor==131584 || valor==1049088) {
        return(1);//está habilitado
    }
    //o no divisores de 8
    else{
        if (valor==514 || valor==530 || valor==546 || valor==562 || valor==642 || valor==66050 || valor==66066 || valor==66082 || valor==131586 || valor==1049090) {
            return(0);//esta inhabilitado
        }
        //indeterminado, estos valores no nos sirven para modificar el valor entra Habil. e Inhabil.
        else {
            return(2);//valor no valido para modificar
        }
    }

}


bool es_fecha (QString fecha) {

    //buscamos si fecha es una fecha con formato "dd-MM-yyyy  hh:mm" o no
    //porque puede ser que contenga una cadena como p.e. "No Caduca"
    QRegularExpression re("\\d\\d-\\d\\d-\\d\\d\\d\\d  \\d\\d:\\d\\d");
    QRegularExpressionMatch match = re.match(fecha);
    return (match.hasMatch());

}


QString dominio_basedn(){



    QString str = "grx";

    QString basedn;
    //sacamos las partes de la cadena del dominio separadas por puntos
    QStringList strList = str.split(".");
    //lo recorremos y lo guardamos en el formato basedn
    for (int i = 0; i < strList.size(); ++i) {
        basedn = basedn + "DC=" + strList.at(i);
        //añadimos una coma si no es la ultima parte
        if (i+1 < strList.size())
            basedn=basedn+",";
    }

    return (basedn);

}

//devuelve una cadena con la fecha en formato dd-MM-yyyy hh:mm:ss
//pasandole como argumento una fecha en formato yyyyMMddHHmmss.0Z
QString gtime_to_string(QString fecha){

    QDateTime date = QDateTime::fromString(fecha, "yyyyMMddHHmmss.0'Z'");
    date.setTimeSpec(Qt::UTC);
    return(date.toLocalTime().toString("dd-MM-yyyy hh:mm:ss"));

}

