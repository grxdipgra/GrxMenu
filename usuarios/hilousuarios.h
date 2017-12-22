#ifndef HILOUSUARIOS_H
#define HILOUSUARIOS_H
#include <QThread>
#include <QObject>
#include "usuarios/form_usuarios.h"
#include "QList"

class HiloUsuarios: public QThread
{
Q_OBJECT
public:
    explicit HiloUsuarios();
     ~HiloUsuarios();
    void ejecuta_hilo();

signals:
   void resultadoListo(QList<QString>);

};
#endif // HILOUSUARIOS_H
