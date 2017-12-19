#ifndef SSH_H
#define SSH_H
#include <QTcpServer>
#include <QProcess>
#include "tunel.h"
#include <QtDebug>
#include "qthread.h"
#include <QMessageBox>

#include "configuracion/configuracion.h"

extern "C"
{
#include "libssh2_config.h"
#include <libssh2.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <sys/types.h>
#ifdef HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif
}
#include <QThread>
#include <QString>
#include "qdebug.h"

class Tunel: public QObject
{
Q_OBJECT
private:

    enum {
        AUTH_NONE = 0,
        AUTH_PASSWORD,
        AUTH_PUBLICKEY
    };

    int rc, i, auth = AUTH_NONE;
    struct sockaddr_in sin;
    socklen_t sinlen;
    const char *fingerprint;
    char *userauthlist;
    LIBSSH2_SESSION *session;
    LIBSSH2_CHANNEL *channel = NULL;
    const char *shost;
    unsigned int sport;
    fd_set fds;
    struct timeval tv;
    ssize_t len, wr;
    char buf[65536];
    int sockopt, sock = -1;
    int listensock = -1, forwardsock = -1;


public:
    explicit Tunel();
    ~Tunel();
    char *keyfile1;
    char *keyfile2;
    char *username_ssh;
    char *password_ssh;
    char *server_ip;
    char *local_listenip;
    unsigned int remote_port;
    unsigned int local_listenport;
    const char *remote_desthost;
    unsigned int remote_destport;//mysql



    /*
     * Hace todo el proceso
     * Usa los datos de arriba
     */
   void crea_fordwarding();
   void crea_conexion();

    /*
     * Inicializa la libreria libssh2
     * Devuelve -1 en caso de error();
     * Es el primer paso para conectar por ssh
    */
    int inicia_libssh2();

    /*
     * Crea un socket con la ip y el puerto como parametro
     * Devuelve -1 en caso de error();
     * Es el segundo paso para conectar por ssh
    */
    int crea_socket();

    /*
     * Crea una sesion con el socket creado en crea_socket
     * Devuelve -1 en caso de error();
     * Es el tercer paso para conectar por ssh
    */
    int crea_sesion();
    /*
     * Muestra el fingerprint (hostkey hash) de la conexion
     * Devuelve -1 en caso de error();
     * Este paso es el cuarto (*?no es obligatorio)
    */
    int muestra_fingerprint();

    /*
     * Muestra los metodos de autentificacion del servidor y nos autentifica
     * Devuelve -1 en caso de error();
     * Paso quinto
     */
    int autenticacion();

    int escucha();

    int cierra_conexion();

    unsigned int puerto_libre();

    char* convierte(QString dato);




signals:
    void sshConectado();
    void sshDesconectado();
};

#endif
