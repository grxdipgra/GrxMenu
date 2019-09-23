#ifndef SSHCOMMAND_H
#define SSHCOMMAND_H

#include <QObject>

class sshCommand : public QObject
{
    Q_OBJECT
public:
    explicit sshCommand(const char *commandline,const char *hostname,const char *username,const char *password,int puerto,QObject *parent = nullptr);
    explicit sshCommand(QObject *parent = nullptr);

    QString salida;
signals:
    void datos(QString);
public slots:
    int conecta(const char *commandline,const char *hostname,const char *username,const char *password,int puerto);

};

#endif // SSHCOMMAND_H
