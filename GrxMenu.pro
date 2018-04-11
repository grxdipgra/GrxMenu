#-------------------------------------------------
#
# Project created by QtCreator 2017-07-07T12:17:11
#
#-------------------------------------------------

QT       += core gui  widgets sql network
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
    tunel.cpp \
    soporte/ejecutahilo.cpp \
    soporte/tabescaner.cpp \
    basedatos/basedatos.cpp \
    acercade/acerdade.cpp \
    mame/mame.cpp \
    usuarios/hilousuarios.cpp \
    usuarios/form_usuarios.cpp \
    configuracion/simplecrypt.cpp

HEADERS  += \
    botonera/botonera.h \
    configuracion/configuracion.h \
    soporte/nmap_xml.h \
    sedes/sedes.h \
    soporte/soporte.h \
    soporte/equipos.h \
    tunel.h \
    soporte/ejecutahilo.h \
    soporte/tabescaner.h \
    basedatos/basedatos.h \
    acercade/acerdade.h \
    mame/mame.h \
    usuarios/hilousuarios.h \
    usuarios/form_usuarios.h \
    usuarios/ldap.h \
    configuracion/simplecrypt.h \
    lib/lib.h

FORMS    += \
    botonera/botonera.ui \
    configuracion/configuracion.ui \
    sedes/sedes.ui \
    soporte/soporte.ui \
    soporte/equipos.ui \
    basedatos/basedatos.ui \
    acercade/acerdade.ui \
    mame/mame.ui \
    usuarios/form_usuarios.ui

INCLUDEPATH +=/lib /usr/lib/x86_64-linux-gnu/
LIBS += -lssh2 -lldap -lcrypto -lssl -lsqlite3

RESOURCES += \
    iconos.qrc

