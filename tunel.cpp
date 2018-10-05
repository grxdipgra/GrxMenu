#include "tunel.h"
#include "lib/lib.h"
Tunel::Tunel(){
}
Tunel::~Tunel(){
}


char* Tunel::convierte(QString dato){
    char *c = dato.toStdString().c_str();
    return c;
}

int Tunel::crea_conexion(){

    if (this->inicia_libssh2()){
        QMessageBox::warning(0,"Sql Error", "¡ATENCION!\nNo ha sido posible inicializa libssh\nCompruebe que tiene instalada la libreria\nNo podrá usar el apartado 'Soporte'' ni 'Sedes'",QMessageBox::Ok);
        return -1;
    }
    else{
         printf ("Se ha podido inicializar libssh2\n");
         if ((this->crea_socket())){
            QMessageBox::warning(0,"Error ", "¡ATENCION!\nNo he podido crear el socket\nCompruebe que tiene acceso a la red y al puerto ssh\no que la ip del servidor es correcta\n",QMessageBox::Ok);
            return -1;
         }
         else{
             fprintf (stderr, "He podido crear un socket \n");
             if (this->crea_sesion()){
                 fprintf (stderr, "No he podido crear un socket \n");
                 return -1;
             }
             else{
                 fprintf (stderr, "He podido mostrar el fingerprint \n");
                 if (this->muestra_fingerprint())
                    fprintf (stderr, "No he podido mostrar el fingerprint \n");
                    else
                        if (this->autenticacion()){
                            fprintf (stderr, "No he podido autenticarme \n");
                            return -1;
                        }
                        else{
                            printf("He podido autenticarme\n");
                             if (this->escucha())
                                 fprintf (stderr, "No he podido ejecutar escucha \n");
                        }
                    }

               }
        }
    this->cierra_conexion();
return 0;
}

/*
     * Inicializa la libreria libssh2
     * Devuelve -1 en caso de error();
     * Es el primer paso para conectar por ssh
    */
int Tunel::inicia_libssh2()//primer paso
    {
        printf("Inicializando libssh2\n");
        rc = libssh2_init (0);
        if (rc != 0)
               return -1;
    return 0;
    }

/*
     * Crea un socket con la ip y el puerto como parametro
     * Devuelve -1 en caso de error();
     * Es el segundo paso para conectar por ssh
    */
int Tunel::crea_socket()//segundo paso,
{
        sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sock == -1)
                return -1;

        sin.sin_family = AF_INET;
        if (INADDR_NONE == (sin.sin_addr.s_addr = inet_addr(server_ip))){
            fprintf (stderr, "La ip del servidor esta mal formada\n");
             return -1;
        }
        sin.sin_port = htons(remote_port);
        if (::connect(sock, (struct sockaddr*)(&sin),sizeof(struct sockaddr_in)) != 0){
             fprintf (stderr, "No he podido crear el socket");
             return -1;
        }
    return 0;
}
/*
     * Crea una sesion con el socket creado en crea_socket
     * Devuelve -1 en caso de error();
     * Es el tercer paso para conectar por ssh
    */
int Tunel::crea_sesion(){
        session = libssh2_session_init();
        if(!session) {
            fprintf(stderr, "No puedo inicializar una sesion\n");
            return -1;
        }
        rc = libssh2_session_handshake(session, sock);
        if(rc) {
            fprintf(stderr, "Error mientras comenzaba una sesion SSH: %d\n", rc);
            return -1;
        }
    return 0;
    }

/*
     * Muestra el fingerprint (hostkey hash) de la conexion
     * Devuelve -1 en caso de error();
     * Este paso es el cuarto (*?no es obligatorio)
    */
int Tunel::muestra_fingerprint(){

        fingerprint = libssh2_hostkey_hash(session, LIBSSH2_HOSTKEY_HASH_SHA1);

        fprintf(stderr, "Fingerprint: ");
        for(i = 0; i < 20; i++)
            fprintf(stderr, "%02X ", (unsigned char)fingerprint[i]);
        fprintf(stderr, "\n");
    return 0;
    }

/*
     * Muestra los metodos de autentificacion del servidor y nos autentifica
     * Devuelve -1 en caso de error();
     * Paso quinto
     */
int Tunel::autenticacion(){

        /* chequea los metodos de autenticacion*/
        userauthlist = libssh2_userauth_list(session, username_ssh, strlen(username_ssh));

        fprintf(stderr, "Metodos de autentificacion: %s\n", userauthlist);
        if (strstr(userauthlist, "password"))
            auth |= AUTH_PASSWORD;
        if (strstr(userauthlist, "publickey"))
            auth |= AUTH_PUBLICKEY;

        if (auth & AUTH_PASSWORD) {
            if (libssh2_userauth_password(session, username_ssh, password_ssh)) {
                fprintf(stderr, "Autentificacion por password fallida.\n");
                return -1;
            }
        } else if (auth & AUTH_PUBLICKEY) {
            if (libssh2_userauth_publickey_fromfile(session, username_ssh, keyfile1,keyfile2,password_ssh)) {
                fprintf(stderr, "\tAutentificacion por clave publica fallida.\n");
                return -1;
            }
            fprintf(stderr, "\tAutentificacion por clave publica correcta.\n");
        } else {
            fprintf(stderr, "No se han encontrado metodos de autenticacion soportados\n");
            return -1;
        }
    return 0;
    }

/*
     * Muestra los metodos de autentificacion del servidor y nos autentifica
     * Devuelve -1 en caso de error();
     * Paso sexto
     */
int Tunel::escucha(){
        listensock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (listensock == -1)
            return -1;

        sin.sin_family = AF_INET;
        sin.sin_port = htons(local_listenport);
        if (INADDR_NONE == (sin.sin_addr.s_addr = inet_addr(local_listenip)))
            return -1;
        sockopt = 1;
        setsockopt(listensock, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(sockopt));
        sinlen=sizeof(sin);
        if (-1 == bind(listensock, (struct sockaddr *)&sin, sinlen)){
            printf("El puerto está ocupado");
            return -1;
        }
        if (-1 == listen(listensock, 2))
            return -1;
        printf("Emitiendo señal sshConectado");
        fprintf(stderr, "Esperando conexiones TCP en %s:%d...\n",
            inet_ntoa(sin.sin_addr), ntohs(sin.sin_port));
        emit sshConectado(); // Emitimos la señal de esperando conexiones
        forwardsock = accept(listensock, (struct sockaddr *)&sin, &sinlen);

        if (forwardsock == -1)
            return -1;
        shost = inet_ntoa(sin.sin_addr);
        sport = ntohs(sin.sin_port);
        fprintf(stderr, "Se ha creado un tunel desde desde %s:%d a %s:%d\n",
            shost, sport, remote_desthost, remote_destport);

        channel = libssh2_channel_direct_tcpip_ex(session, remote_desthost,remote_destport, shost, sport);
        if (!channel)
            return -1;

        libssh2_session_set_blocking(session, 0);

        while (1) {
            FD_ZERO(&fds);
            FD_SET(forwardsock, &fds);
            tv.tv_sec = 0;
            tv.tv_usec = 100;
            rc = select(forwardsock + 1, &fds, NULL, NULL, &tv);
            if (-1 == rc)  return -1;
            if (rc && FD_ISSET(forwardsock, &fds)) {
                len = recv(forwardsock, buf, sizeof(buf), 0);
                if (len < 0) {
                    return -1;
                } else if (0 == len) {
                    fprintf(stderr, "El cliente en %s:%d se ha desconectado\n"
                                    "compruebe que la clave de mysql es correcta", shost,
                        sport);
                     emit sshDesconectado();
                     return -1;
                }
                wr = 0;
                while(wr < len) {
                    i = libssh2_channel_write(channel, buf + wr, len - wr);

                    if (LIBSSH2_ERROR_EAGAIN == i) {
                        continue;
                    }
                    if (i < 0) {
                         return -1;
                    }
                    wr += i;
                }
            }
            while (1) {
                len = libssh2_channel_read(channel, buf, sizeof(buf));

                if (LIBSSH2_ERROR_EAGAIN == len)
                    break;
                else if (len < 0) {
                    fprintf(stderr, "libssh2_channel_leido: %d", (int)len);
                    emit sshDesconectado();
                     return -1;
                }
                wr = 0;
                while (wr < len) {
                    i = send(forwardsock, buf + wr, len - wr, 0);
                    if (i <= 0) {
                        emit sshDesconectado();
                         return -1;
                    }
                    wr += i;
                }
                if (libssh2_channel_eof(channel)) {
                     fprintf(stderr, "El servidor %s:%d se ha desconectado\n"
                                     "compruebe que la clave de mysql es correcta",remote_desthost, remote_destport);
                     emit sshDesconectado();
                     return -1;
                }
            }
        }
    }

/*
 *
 *
 *
 */
int Tunel::cierra_conexion(){

        if (forwardsock != -1)
            close(forwardsock);
        if (listensock != -1)
            close(listensock);
        if (channel)
            libssh2_channel_free(channel);

        if (session){
            libssh2_session_disconnect(session, "Desconectado de forma segura");
            libssh2_session_free(session);
        }
        if (sock != -1)
            close(sock);
        if (rc != 0)
            libssh2_exit();

        return 0;
    }

void Tunel::crea_forwarding(){
        printf("Creando tunel\n");
        rc = libssh2_init (0);
        if (rc != 0) {
            fprintf (stderr, "La libreria libssh2 no se ha podido inicializar (%d)\n", rc);

        }
        sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sock == -1) {
            fprintf(stderr, "socket \n");
            perror("socket");

        }
        fprintf(stderr, "Se ha creado el socket\n");
        sin.sin_family = AF_INET;
        if (INADDR_NONE == (sin.sin_addr.s_addr = inet_addr(server_ip))) {
            perror("inet_addr");
        }

        sin.sin_port = htons(remote_port);
        fprintf(stderr, "conecta\n");
        if (::connect(sock, (struct sockaddr*)(&sin),sizeof(struct sockaddr_in)) != 0) {
            fprintf(stderr, "Fallo en la conexion!\n");
        }
        printf( "conectado sin problemas");

        session = libssh2_session_init();

        if(!session) {
            fprintf(stderr, "No se ha podido inicializar una sesion SSH\n", session);
        }

        rc = libssh2_session_handshake(session, sock);

        if(rc) {
            fprintf(stderr, "Error inicializando SSH: %d\n", rc);
        }

        fingerprint = libssh2_hostkey_hash(session, LIBSSH2_HOSTKEY_HASH_SHA1);

        fprintf(stderr, "Fingerprint: ");
        for(i = 0; i < 20; i++)
            fprintf(stderr, "%02X ", (unsigned char)fingerprint[i]);
        fprintf(stderr, "\n");

        userauthlist = libssh2_userauth_list(session, username_ssh, strlen(username_ssh));
        fprintf(stderr, "Metodos de autenticacion : %s\n", userauthlist);
        if (strstr(userauthlist, "password"))
            auth |= AUTH_PASSWORD;
        if (strstr(userauthlist, "publickey"))
            auth |= AUTH_PUBLICKEY;

        if (auth & AUTH_PASSWORD) {
            if (libssh2_userauth_password(session, username_ssh, password_ssh)) {

                fprintf(stderr, "Autentificacion por password ha fallado.\n");
                return ;
            }
        } else if (auth & AUTH_PUBLICKEY) {
            if (libssh2_userauth_publickey_fromfile(session, username_ssh, keyfile1,keyfile2, password_ssh)) {
                fprintf(stderr, "\tAuthentication by public key failed!\n");
                return ;
            }
            fprintf(stderr, "\tAuthentication by public key succeeded.\n");
                } else {
                    fprintf(stderr, "No supported authentication methods found!\n");
                    return ;
                }

        listensock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (listensock == -1) {
            perror("socket");
            return ;
        }

        sin.sin_family = AF_INET;
        sin.sin_port = htons(local_listenport);
        if (INADDR_NONE == (sin.sin_addr.s_addr = inet_addr(local_listenip))) {
            perror("inet_addr");
            return ;
        }
        sockopt = 1;
        setsockopt(listensock, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(sockopt));
        sinlen=sizeof(sin);
        if (-1 == bind(listensock, (struct sockaddr *)&sin, sinlen)) {
            perror("bind");
            return ;
        }
        if (-1 == listen(listensock, 2)) {
            perror("listen");
            printf( "Error en la conexion");
            return ;
        }

        fprintf(stderr, "Esperando conexiones por TCP en %s:%d...\n",
            inet_ntoa(sin.sin_addr), ntohs(sin.sin_port));
        //emit sshConectado();
        printf( "Emitida señal sshConectado()");
        forwardsock = accept(listensock, (struct sockaddr *)&sin, &sinlen);
        if (forwardsock == -1) {
            perror("aceptar");
            return ;
        }

        shost = inet_ntoa(sin.sin_addr);
        sport = ntohs(sin.sin_port);
        fprintf(stderr, "Conexion desde %s:%d al servidor remoto %s:%d\n",
            shost, sport, remote_desthost, remote_destport);

        channel = libssh2_channel_direct_tcpip_ex(session, remote_desthost,
            remote_destport, shost, sport);
        if (!channel) {
            fprintf(stderr, "No he podido abrir una conexion direct-tcpip!\n"
                    "(Puede ser un problema del servidor"
                    " Revise los logs para verificarlo.)\n");
            return ;
        }


        libssh2_session_set_blocking(session, 0);
        fprintf(stderr, "Entrando en el while");

        while (1) {
            FD_ZERO(&fds);
            FD_SET(forwardsock, &fds);
            tv.tv_sec = 0;
            tv.tv_usec = 100;
            rc = select(forwardsock + 1, &fds, NULL, NULL, &tv);
            if (-1 == rc) {
                perror("select");
                return ;
            }
            if (rc && FD_ISSET(forwardsock, &fds)) {
                len = recv(forwardsock, buf, sizeof(buf), 0);
                if (len < 0) {
                    perror("read");
                    return ;
                } else if (0 == len) {
                    fprintf(stderr, "El cliente se ha desconectado de %s:%d\n", shost,sport);
                    return ;
                }
                wr = 0;
                while(wr < len) {
                    i = libssh2_channel_write(channel, buf + wr, len - wr);

                    if (LIBSSH2_ERROR_EAGAIN == i) {
                        continue;
                    }
                    if (i < 0) {
                        fprintf(stderr, "libssh2_channel_write: %d\n", i);
                        return ;
                    }
                    wr += i;
                }
            }
            while (1) {
                len = libssh2_channel_read(channel, buf, sizeof(buf));

                if (LIBSSH2_ERROR_EAGAIN == len)
                    break;
                else if (len < 0) {
                    fprintf(stderr, "libssh2_channel_read: %d", (int)len);
                    return ;
                }
                wr = 0;
                while (wr < len) {
                    i = send(forwardsock, buf + wr, len - wr, 0);
                    if (i <= 0) {
                        perror("write");
                        return ;
                    }
                    wr += i;
                }
                if (libssh2_channel_eof(channel)) {

                    fprintf(stderr, "El cliente se ha desconectado de %s:%d\n\n",
                        remote_desthost, remote_destport);
                    return ;
                }
            }
        }
}

