#-------------------------------------------------
#
# Project created by QtCreator 2017-07-07T12:17:11
#
#-------------------------------------------------

QT       += core gui widgets sql network

TARGET = GrxMenu
TEMPLATE = app
CONFIG += c++11 static
QMAKE_CFLAGS = -fpermissive
QMAKE_CXXFLAGS = -fpermissive
QMAKE_LFLAGS = -fpermissive
SOURCES += main.cpp \
    botonera.cpp \
    configuracion.cpp \
    nmap_xml.cpp \
    sedes.cpp \
    soporte.cpp \
    equipos.cpp \
    tunel.cpp \
    ejecutahilo.cpp \
    tabescaner.cpp \
    basedatos.cpp \
    acerdade.cpp \
    mame.cpp \
    hilousuarios.cpp \
    usuarios/form_usuarios.cpp

HEADERS  += \
    botonera.h \
    configuracion.h \
    nmap_xml.h \
    sedes.h \
    soporte.h \
    equipos.h \
    tunel.h \
    ejecutahilo.h \
    tabescaner.h \
    basedatos.h \
    acerdade.h \
    mame.h \
    hilousuarios.h \
    usuarios/form_usuarios.h \
    usuarios/ldap.h

FORMS    += \
    botonera.ui \
    configuracion.ui \
    sedes.ui \
    soporte.ui \
    equipos.ui \
    basedatos.ui \
    acerdade.ui \
    mame.ui \
    usuarios/form_usuarios.ui

INCLUDEPATH +=/lib
LIBS += -lssh2 -lldap

RESOURCES += \
    iconos.qrc

