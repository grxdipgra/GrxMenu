#include "lib.h"
#include <QFileInfo>
#include <QTcpServer>
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
