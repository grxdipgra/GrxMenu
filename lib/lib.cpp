#include "lib.h"
#include <QFileInfo>
// Esta funcion comprueba que el archivo pasado por parametro
// exista y no sea un directorio

bool fileExists(QString path) {
    QFileInfo check_file(path);
    return (check_file.exists() && check_file.isFile());
}
