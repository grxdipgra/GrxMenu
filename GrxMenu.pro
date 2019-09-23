#-------------------------------------------------
#
# Project created by QtCreator 2017-07-07T12:17:11
#
#-------------------------------------------------

QT       += core gui  widgets sql network webenginewidgets
CONFIG += debug
TARGET = GrxMenu
TEMPLATE = app
CONFIG += c++11
QMAKE_CFLAGS = -fpermissive
QMAKE_CXXFLAGS = -fpermissive
QMAKE_LFLAGS = -fpermissive
SOURCES += main.cpp \
    botonera/botonera.cpp \
    configuracion/configuracion.cpp \
    soporte/nmap_xml.cpp \
    sedes/sedes.cpp \
    soporte/soporte.cpp \
    soporte/equipos.cpp \
    soporte/ejecutahilo.cpp \
    soporte/tabescaner.cpp \
    basedatos/basedatos.cpp \
    mame/mame.cpp \
    usuarios/hilousuarios.cpp \
    usuarios/clase_ldap.cpp \
    usuarios/usuario.cpp \
    usuarios/form_grupos.cpp \
    usuarios/form_usuarios.cpp \
    configuracion/simplecrypt.cpp \
    tunel.cpp \
    lib/lib.cpp \
    lib/globals.cpp \
    soporte/paginaweb.cpp \
    mailing/mailing.cpp \
    mailing/sqlquerymodel.cpp \
    acercade/acercade.cpp \
    soporte/usuarios.cpp \
    configuracion/buscarou.cpp



HEADERS  += \
    botonera/botonera.h \
    configuracion/configuracion.h \
    soporte/nmap_xml.h \
    sedes/sedes.h \
    soporte/soporte.h \
    soporte/equipos.h \
    soporte/ejecutahilo.h \
    soporte/tabescaner.h \
    basedatos/basedatos.h \
    mame/mame.h \
    usuarios/hilousuarios.h \
    usuarios/form_usuarios.h \
    usuarios/ldap.h \
    usuarios/clase_ldap.h \
    usuarios/usuario.h \
    usuarios/form_grupos.h \
    configuracion/simplecrypt.h \
    tunel.h \
    lib/lib.h \
    lib/globals.h \
    soporte/paginaweb.h \
    mailing/mailing.h \
    mailing/sqlquerymodel.h \
    acercade/acercade.h \
    soporte/usuarios.h \
    configuracion/buscarou.h


FORMS    += \
    botonera/botonera.ui \
    configuracion/configuracion.ui \
    sedes/sedes.ui \
    soporte/soporte.ui \
    soporte/equipos.ui \
    basedatos/basedatos.ui \
    mame/mame.ui \
    usuarios/form_usuarios.ui \
    usuarios/form_grupos.ui \
    soporte/paginaweb.ui \
    mailing/mailing.ui \
    acercade/acercade.ui \
    soporte/usuarios.ui \
    configuracion/buscarou.ui


INCLUDEPATH +=/lib /usr/lib/x86_64-linux-gnu/
LIBS += -lssh2 -lldap -lcrypto -lssl -lsqlite3

RESOURCES += \
    iconos.qrc

