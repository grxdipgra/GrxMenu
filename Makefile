#############################################################################
# Makefile for building: GrxMenu
# Generated by qmake (3.0) (Qt 5.6.1)
# Project:  GrxMenu.pro
# Template: app
# Command: /usr/lib/x86_64-linux-gnu/qt5/bin/qmake -spec linux-g++-64 -o Makefile GrxMenu.pro
#############################################################################

MAKEFILE      = Makefile

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_NO_DEBUG -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_SQL_LIB -DQT_NETWORK_LIB -DQT_CORE_LIB
CFLAGS        = -fpermissive -O2 -Wall -W -D_REENTRANT -fPIC $(DEFINES)
CXXFLAGS      = -fpermissive -O2 -std=gnu++0x -Wall -W -D_REENTRANT -fPIC $(DEFINES)
INCPATH       = -I. -isystem /usr/include/x86_64-linux-gnu/qt5 -isystem /usr/include/x86_64-linux-gnu/qt5/QtWidgets -isystem /usr/include/x86_64-linux-gnu/qt5/QtGui -isystem /usr/include/x86_64-linux-gnu/qt5/QtSql -isystem /usr/include/x86_64-linux-gnu/qt5/QtNetwork -isystem /usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++-64
QMAKE         = /usr/lib/x86_64-linux-gnu/qt5/bin/qmake
DEL_FILE      = rm -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p
COPY          = cp -f
COPY_FILE     = cp -f
COPY_DIR      = cp -f -R
INSTALL_FILE  = install -m 644 -p
INSTALL_PROGRAM = install -m 755 -p
INSTALL_DIR   = cp -f -R
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
TAR           = tar -cf
COMPRESS      = gzip -9f
DISTNAME      = GrxMenu1.0.0
DISTDIR = /home/alberto/GrxMenu/.tmp/GrxMenu1.0.0
LINK          = g++
LFLAGS        = -fpermissive -Wl,-O1
LIBS          = $(SUBLIBS) -L/usr/X11R6/lib64 -lssh2 -lQt5Widgets -lQt5Gui -lQt5Sql -lQt5Network -lQt5Core -lGL -lpthread 
AR            = ar cqs
RANLIB        = 
SED           = sed
STRIP         = strip

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = main.cpp \
		botonera.cpp \
		configuracion.cpp \
		usuario.cpp \
		nmap_xml.cpp \
		sedes.cpp \
		soporte.cpp \
		equipos.cpp \
		tunel.cpp \
		ejecutahilo.cpp \
		tabescaner.cpp \
		basedatos.cpp \
		acerdade.cpp \
		mame.cpp qrc_iconos.cpp \
		moc_botonera.cpp \
		moc_configuracion.cpp \
		moc_usuario.cpp \
		moc_sedes.cpp \
		moc_soporte.cpp \
		moc_equipos.cpp \
		moc_tunel.cpp \
		moc_ejecutahilo.cpp \
		moc_tabescaner.cpp \
		moc_basedatos.cpp \
		moc_acerdade.cpp \
		moc_mame.cpp
OBJECTS       = main.o \
		botonera.o \
		configuracion.o \
		usuario.o \
		nmap_xml.o \
		sedes.o \
		soporte.o \
		equipos.o \
		tunel.o \
		ejecutahilo.o \
		tabescaner.o \
		basedatos.o \
		acerdade.o \
		mame.o \
		qrc_iconos.o \
		moc_botonera.o \
		moc_configuracion.o \
		moc_usuario.o \
		moc_sedes.o \
		moc_soporte.o \
		moc_equipos.o \
		moc_tunel.o \
		moc_ejecutahilo.o \
		moc_tabescaner.o \
		moc_basedatos.o \
		moc_acerdade.o \
		moc_mame.o
DIST          = /usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/spec_pre.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/unix.conf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/linux.conf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/sanitize.conf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/gcc-base.conf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/gcc-base-unix.conf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/g++-base.conf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/g++-unix.conf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/qconfig.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_bootstrap_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_concurrent.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_concurrent_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_core.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_core_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_dbus.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_dbus_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_eglfs_device_lib_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_gui.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_gui_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_network.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_network_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_opengl.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_opengl_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_openglextensions.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_openglextensions_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_platformsupport_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_printsupport.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_printsupport_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_qml.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_qmltest.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_quick.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_quickwidgets.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_sql.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_sql_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_testlib.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_testlib_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_widgets.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_widgets_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_xcb_qpa_lib_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_xml.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_xml_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/qt_functions.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/qt_config.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++-64/qmake.conf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/spec_post.prf \
		.qmake.stash \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/exclusive_builds.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/default_pre.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/resolve_config.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/default_post.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/warn_on.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/qt.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/resources.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/moc.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/unix/opengl.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/uic.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/unix/thread.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/file_copies.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/testcase_targets.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/exceptions.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/yacc.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/lex.prf \
		GrxMenu.pro botonera.h \
		configuracion.h \
		usuario.h \
		nmap_xml.h \
		sedes.h \
		soporte.h \
		equipos.h \
		tunel.h \
		ejecutahilo.h \
		tabescaner.h \
		basedatos.h \
		acerdade.h \
		mame.h main.cpp \
		botonera.cpp \
		configuracion.cpp \
		usuario.cpp \
		nmap_xml.cpp \
		sedes.cpp \
		soporte.cpp \
		equipos.cpp \
		tunel.cpp \
		ejecutahilo.cpp \
		tabescaner.cpp \
		basedatos.cpp \
		acerdade.cpp \
		mame.cpp
QMAKE_TARGET  = GrxMenu
DESTDIR       = 
TARGET        = GrxMenu


first: all
####### Build rules

$(TARGET): ui_botonera.h ui_configuracion.h ui_usuario.h ui_sedes.h ui_soporte.h ui_equipos.h ui_basedatos.h ui_acerdade.h ui_mame.h $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: GrxMenu.pro /usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++-64/qmake.conf /usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/spec_pre.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/unix.conf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/linux.conf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/sanitize.conf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/gcc-base.conf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/gcc-base-unix.conf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/g++-base.conf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/g++-unix.conf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/qconfig.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_bootstrap_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_concurrent.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_concurrent_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_core.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_core_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_dbus.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_dbus_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_eglfs_device_lib_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_gui.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_gui_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_network.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_network_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_opengl.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_opengl_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_openglextensions.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_openglextensions_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_platformsupport_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_printsupport.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_printsupport_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_qml.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_qmltest.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_quick.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_quickwidgets.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_sql.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_sql_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_testlib.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_testlib_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_widgets.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_widgets_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_xcb_qpa_lib_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_xml.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_xml_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/qt_functions.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/qt_config.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++-64/qmake.conf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/spec_post.prf \
		.qmake.stash \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/exclusive_builds.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/default_pre.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/resolve_config.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/default_post.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/warn_on.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/qt.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/resources.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/moc.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/unix/opengl.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/uic.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/unix/thread.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/file_copies.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/testcase_targets.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/exceptions.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/yacc.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/lex.prf \
		GrxMenu.pro \
		iconos.qrc \
		/usr/lib/x86_64-linux-gnu/libQt5Widgets.prl \
		/usr/lib/x86_64-linux-gnu/libQt5Gui.prl \
		/usr/lib/x86_64-linux-gnu/libQt5Sql.prl \
		/usr/lib/x86_64-linux-gnu/libQt5Network.prl \
		/usr/lib/x86_64-linux-gnu/libQt5Core.prl
	$(QMAKE) -spec linux-g++-64 -o Makefile GrxMenu.pro
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/spec_pre.prf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/unix.conf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/linux.conf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/sanitize.conf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/gcc-base.conf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/gcc-base-unix.conf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/g++-base.conf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/g++-unix.conf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/qconfig.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_bootstrap_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_concurrent.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_concurrent_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_core.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_core_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_dbus.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_dbus_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_eglfs_device_lib_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_gui.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_gui_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_network.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_network_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_opengl.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_opengl_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_openglextensions.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_openglextensions_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_platformsupport_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_printsupport.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_printsupport_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_qml.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_qmltest.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_quick.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_quickwidgets.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_sql.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_sql_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_testlib.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_testlib_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_widgets.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_widgets_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_xcb_qpa_lib_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_xml.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_xml_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/qt_functions.prf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/qt_config.prf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++-64/qmake.conf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/spec_post.prf:
.qmake.stash:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/exclusive_builds.prf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/default_pre.prf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/resolve_config.prf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/default_post.prf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/warn_on.prf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/qt.prf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/resources.prf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/moc.prf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/unix/opengl.prf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/uic.prf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/unix/thread.prf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/file_copies.prf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/testcase_targets.prf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/exceptions.prf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/yacc.prf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/lex.prf:
GrxMenu.pro:
iconos.qrc:
/usr/lib/x86_64-linux-gnu/libQt5Widgets.prl:
/usr/lib/x86_64-linux-gnu/libQt5Gui.prl:
/usr/lib/x86_64-linux-gnu/libQt5Sql.prl:
/usr/lib/x86_64-linux-gnu/libQt5Network.prl:
/usr/lib/x86_64-linux-gnu/libQt5Core.prl:
qmake: FORCE
	@$(QMAKE) -spec linux-g++-64 -o Makefile GrxMenu.pro

qmake_all: FORCE


all: Makefile $(TARGET)

dist: distdir FORCE
	(cd `dirname $(DISTDIR)` && $(TAR) $(DISTNAME).tar $(DISTNAME) && $(COMPRESS) $(DISTNAME).tar) && $(MOVE) `dirname $(DISTDIR)`/$(DISTNAME).tar.gz . && $(DEL_FILE) -r $(DISTDIR)

distdir: FORCE
	@test -d $(DISTDIR) || mkdir -p $(DISTDIR)
	$(COPY_FILE) --parents $(DIST) $(DISTDIR)/
	$(COPY_FILE) --parents iconos.qrc $(DISTDIR)/
	$(COPY_FILE) --parents botonera.h configuracion.h usuario.h nmap_xml.h sedes.h soporte.h equipos.h tunel.h ejecutahilo.h tabescaner.h basedatos.h acerdade.h mame.h $(DISTDIR)/
	$(COPY_FILE) --parents main.cpp botonera.cpp configuracion.cpp usuario.cpp nmap_xml.cpp sedes.cpp soporte.cpp equipos.cpp tunel.cpp ejecutahilo.cpp tabescaner.cpp basedatos.cpp acerdade.cpp mame.cpp $(DISTDIR)/
	$(COPY_FILE) --parents botonera.ui configuracion.ui usuario.ui sedes.ui soporte.ui equipos.ui basedatos.ui acerdade.ui mame.ui $(DISTDIR)/


clean: compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


distclean: clean 
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) .qmake.stash
	-$(DEL_FILE) Makefile


####### Sub-libraries

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

check: first

benchmark: first

compiler_rcc_make_all: qrc_iconos.cpp
compiler_rcc_clean:
	-$(DEL_FILE) qrc_iconos.cpp
qrc_iconos.cpp: iconos.qrc \
		imagenes/iconos/equipos/hardware.png \
		imagenes/iconos/equipos/config_pam.png \
		imagenes/iconos/equipos/download.png \
		imagenes/iconos/equipos/network-share-icon_35.png \
		imagenes/iconos/equipos/vnc.jpeg \
		imagenes/iconos/equipos/network-share-icon.png \
		imagenes/iconos/equipos/administrador.png \
		imagenes/iconos/equipos/update.png \
		imagenes/iconos/equipos/cronos.png \
		imagenes/iconos/equipos/lspci.png \
		imagenes/iconos/equipos/procesos.png \
		imagenes/iconos/equipos/asistencia.png \
		imagenes/iconos/equipos/telnet.jpeg \
		imagenes/iconos/equipos/mount.png \
		imagenes/iconos/equipos/info_equipo.png \
		imagenes/iconos/equipos/correo.png \
		imagenes/iconos/equipos/carpeta_personal.png \
		imagenes/iconos/equipos/instala.png \
		imagenes/iconos/equipos/net.png \
		imagenes/iconos/equipos/cups32.png \
		imagenes/iconos/equipos/glpi.png \
		imagenes/iconos/equipos/discos.png \
		imagenes/iconos/equipos/web.jpeg \
		imagenes/iconos/equipos/lscpu.png \
		imagenes/iconos/equipos/lsmod.png \
		imagenes/iconos/equipos/info.png \
		imagenes/iconos/equipos/clamav.png \
		imagenes/iconos/equipos/atalaya.png \
		imagenes/iconos/equipos/lsusb.png \
		imagenes/iconos/equipos/firewall.png \
		imagenes/iconos/equipos/cifrar.png \
		imagenes/iconos/equipos/ssh_35.png \
		imagenes/iconos/equipos/captura.png \
		imagenes/iconos/equipos/konekta.png \
		imagenes/iconos/equipos/mensaje.png \
		imagenes/iconos/equipos/conexiones_red.png \
		imagenes/iconos/equipos/ssh.png \
		imagenes/iconos/equipos/escaner.png \
		imagenes/iconos/equipos/lsblk.png \
		imagenes/iconos/soporte/aytos.png \
		imagenes/iconos/soporte/portafirmas.png \
		imagenes/iconos/soporte/escoba.svg \
		imagenes/iconos/soporte/Desconocido_pc.png \
		imagenes/iconos/soporte/Windows_pc.png \
		imagenes/iconos/soporte/Printer.png \
		imagenes/iconos/soporte/gtablon.png \
		imagenes/iconos/soporte/buscando.gif \
		imagenes/iconos/soporte/guadaltel.png \
		imagenes/iconos/soporte/ISL50x50.png \
		imagenes/iconos/soporte/perfilcontratante.png \
		imagenes/iconos/soporte/moad.png \
		imagenes/iconos/soporte/Router.png \
		imagenes/iconos/soporte/Linux_pc.png \
		imagenes/iconos/soporte/epol.png \
		imagenes/iconos/soporte/atalaya.png \
		imagenes/iconos/soporte/citrix.png \
		imagenes/iconos/soporte/listin.jpeg \
		imagenes/iconos/sedes/color.png \
		imagenes/iconos/botonera/nmap.png \
		imagenes/iconos/botonera/online-96x96.png \
		imagenes/iconos/botonera/color.png \
		imagenes/iconos/botonera/ocs.png \
		imagenes/iconos/botonera/ISL.png \
		imagenes/iconos/botonera/sedes.png \
		imagenes/iconos/botonera/equipos.png \
		imagenes/iconos/botonera/listin.png \
		imagenes/iconos/botonera/cronos.png \
		imagenes/iconos/botonera/update.png \
		imagenes/iconos/botonera/asistencia.png \
		imagenes/iconos/botonera/mame.png \
		imagenes/iconos/botonera/WebMail.png \
		imagenes/iconos/botonera/salida.png \
		imagenes/iconos/botonera/usuario_on.png \
		imagenes/iconos/botonera/ISL50x50.png \
		imagenes/iconos/botonera/usuario_off.png \
		imagenes/iconos/botonera/moad.png \
		imagenes/iconos/botonera/Router.png \
		imagenes/iconos/botonera/AD.png \
		imagenes/iconos/botonera/beiro_cartera.png \
		imagenes/iconos/botonera/Linux_pc.png \
		imagenes/iconos/botonera/300px-Granada_-_Mapa_municipal.svg.png \
		imagenes/iconos/botonera/lupa.png \
		imagenes/iconos/botonera/glpi.png \
		imagenes/iconos/botonera/usuario.png \
		imagenes/iconos/botonera/logo.png \
		imagenes/iconos/botonera/atalaya.png \
		imagenes/iconos/botonera/Printer_pc.png \
		imagenes/iconos/botonera/sql.svg \
		imagenes/iconos/botonera/usuarios.png \
		imagenes/iconos/AcercaDe/GNU_and_Tux.svg
	/usr/lib/x86_64-linux-gnu/qt5/bin/rcc -name iconos iconos.qrc -o qrc_iconos.cpp

compiler_moc_header_make_all: moc_botonera.cpp moc_configuracion.cpp moc_usuario.cpp moc_sedes.cpp moc_soporte.cpp moc_equipos.cpp moc_tunel.cpp moc_ejecutahilo.cpp moc_tabescaner.cpp moc_basedatos.cpp moc_acerdade.cpp moc_mame.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_botonera.cpp moc_configuracion.cpp moc_usuario.cpp moc_sedes.cpp moc_soporte.cpp moc_equipos.cpp moc_tunel.cpp moc_ejecutahilo.cpp moc_tabescaner.cpp moc_basedatos.cpp moc_acerdade.cpp moc_mame.cpp
moc_botonera.cpp: configuracion.h \
		acerdade.h \
		botonera.h
	/usr/lib/x86_64-linux-gnu/qt5/bin/moc $(DEFINES) -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++-64 -I/home/alberto/GrxMenu -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/5 -I/usr/include/x86_64-linux-gnu/c++/5 -I/usr/include/c++/5/backward -I/usr/lib/gcc/x86_64-linux-gnu/5/include -I/usr/local/include -I/usr/lib/gcc/x86_64-linux-gnu/5/include-fixed -I/usr/include/x86_64-linux-gnu -I/usr/include botonera.h -o moc_botonera.cpp

moc_configuracion.cpp: configuracion.h
	/usr/lib/x86_64-linux-gnu/qt5/bin/moc $(DEFINES) -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++-64 -I/home/alberto/GrxMenu -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/5 -I/usr/include/x86_64-linux-gnu/c++/5 -I/usr/include/c++/5/backward -I/usr/lib/gcc/x86_64-linux-gnu/5/include -I/usr/local/include -I/usr/lib/gcc/x86_64-linux-gnu/5/include-fixed -I/usr/include/x86_64-linux-gnu -I/usr/include configuracion.h -o moc_configuracion.cpp

moc_usuario.cpp: usuario.h
	/usr/lib/x86_64-linux-gnu/qt5/bin/moc $(DEFINES) -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++-64 -I/home/alberto/GrxMenu -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/5 -I/usr/include/x86_64-linux-gnu/c++/5 -I/usr/include/c++/5/backward -I/usr/lib/gcc/x86_64-linux-gnu/5/include -I/usr/local/include -I/usr/lib/gcc/x86_64-linux-gnu/5/include-fixed -I/usr/include/x86_64-linux-gnu -I/usr/include usuario.h -o moc_usuario.cpp

moc_sedes.cpp: nmap_xml.h \
		configuracion.h \
		sedes.h
	/usr/lib/x86_64-linux-gnu/qt5/bin/moc $(DEFINES) -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++-64 -I/home/alberto/GrxMenu -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/5 -I/usr/include/x86_64-linux-gnu/c++/5 -I/usr/include/c++/5/backward -I/usr/lib/gcc/x86_64-linux-gnu/5/include -I/usr/local/include -I/usr/lib/gcc/x86_64-linux-gnu/5/include-fixed -I/usr/include/x86_64-linux-gnu -I/usr/include sedes.h -o moc_sedes.cpp

moc_soporte.cpp: nmap_xml.h \
		soporte.h
	/usr/lib/x86_64-linux-gnu/qt5/bin/moc $(DEFINES) -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++-64 -I/home/alberto/GrxMenu -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/5 -I/usr/include/x86_64-linux-gnu/c++/5 -I/usr/include/c++/5/backward -I/usr/lib/gcc/x86_64-linux-gnu/5/include -I/usr/local/include -I/usr/lib/gcc/x86_64-linux-gnu/5/include-fixed -I/usr/include/x86_64-linux-gnu -I/usr/include soporte.h -o moc_soporte.cpp

moc_equipos.cpp: nmap_xml.h \
		equipos.h
	/usr/lib/x86_64-linux-gnu/qt5/bin/moc $(DEFINES) -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++-64 -I/home/alberto/GrxMenu -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/5 -I/usr/include/x86_64-linux-gnu/c++/5 -I/usr/include/c++/5/backward -I/usr/lib/gcc/x86_64-linux-gnu/5/include -I/usr/local/include -I/usr/lib/gcc/x86_64-linux-gnu/5/include-fixed -I/usr/include/x86_64-linux-gnu -I/usr/include equipos.h -o moc_equipos.cpp

moc_tunel.cpp: configuracion.h \
		libssh2_config.h \
		tunel.h
	/usr/lib/x86_64-linux-gnu/qt5/bin/moc $(DEFINES) -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++-64 -I/home/alberto/GrxMenu -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/5 -I/usr/include/x86_64-linux-gnu/c++/5 -I/usr/include/c++/5/backward -I/usr/lib/gcc/x86_64-linux-gnu/5/include -I/usr/local/include -I/usr/lib/gcc/x86_64-linux-gnu/5/include-fixed -I/usr/include/x86_64-linux-gnu -I/usr/include tunel.h -o moc_tunel.cpp

moc_ejecutahilo.cpp: nmap_xml.h \
		ejecutahilo.h
	/usr/lib/x86_64-linux-gnu/qt5/bin/moc $(DEFINES) -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++-64 -I/home/alberto/GrxMenu -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/5 -I/usr/include/x86_64-linux-gnu/c++/5 -I/usr/include/c++/5/backward -I/usr/lib/gcc/x86_64-linux-gnu/5/include -I/usr/local/include -I/usr/lib/gcc/x86_64-linux-gnu/5/include-fixed -I/usr/include/x86_64-linux-gnu -I/usr/include ejecutahilo.h -o moc_ejecutahilo.cpp

moc_tabescaner.cpp: nmap_xml.h \
		tabescaner.h
	/usr/lib/x86_64-linux-gnu/qt5/bin/moc $(DEFINES) -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++-64 -I/home/alberto/GrxMenu -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/5 -I/usr/include/x86_64-linux-gnu/c++/5 -I/usr/include/c++/5/backward -I/usr/lib/gcc/x86_64-linux-gnu/5/include -I/usr/local/include -I/usr/lib/gcc/x86_64-linux-gnu/5/include-fixed -I/usr/include/x86_64-linux-gnu -I/usr/include tabescaner.h -o moc_tabescaner.cpp

moc_basedatos.cpp: basedatos.h
	/usr/lib/x86_64-linux-gnu/qt5/bin/moc $(DEFINES) -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++-64 -I/home/alberto/GrxMenu -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/5 -I/usr/include/x86_64-linux-gnu/c++/5 -I/usr/include/c++/5/backward -I/usr/lib/gcc/x86_64-linux-gnu/5/include -I/usr/local/include -I/usr/lib/gcc/x86_64-linux-gnu/5/include-fixed -I/usr/include/x86_64-linux-gnu -I/usr/include basedatos.h -o moc_basedatos.cpp

moc_acerdade.cpp: acerdade.h
	/usr/lib/x86_64-linux-gnu/qt5/bin/moc $(DEFINES) -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++-64 -I/home/alberto/GrxMenu -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/5 -I/usr/include/x86_64-linux-gnu/c++/5 -I/usr/include/c++/5/backward -I/usr/lib/gcc/x86_64-linux-gnu/5/include -I/usr/local/include -I/usr/lib/gcc/x86_64-linux-gnu/5/include-fixed -I/usr/include/x86_64-linux-gnu -I/usr/include acerdade.h -o moc_acerdade.cpp

moc_mame.cpp: mame.h
	/usr/lib/x86_64-linux-gnu/qt5/bin/moc $(DEFINES) -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++-64 -I/home/alberto/GrxMenu -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/5 -I/usr/include/x86_64-linux-gnu/c++/5 -I/usr/include/c++/5/backward -I/usr/lib/gcc/x86_64-linux-gnu/5/include -I/usr/local/include -I/usr/lib/gcc/x86_64-linux-gnu/5/include-fixed -I/usr/include/x86_64-linux-gnu -I/usr/include mame.h -o moc_mame.cpp

compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: ui_botonera.h ui_configuracion.h ui_usuario.h ui_sedes.h ui_soporte.h ui_equipos.h ui_basedatos.h ui_acerdade.h ui_mame.h
compiler_uic_clean:
	-$(DEL_FILE) ui_botonera.h ui_configuracion.h ui_usuario.h ui_sedes.h ui_soporte.h ui_equipos.h ui_basedatos.h ui_acerdade.h ui_mame.h
ui_botonera.h: botonera.ui
	/usr/lib/x86_64-linux-gnu/qt5/bin/uic botonera.ui -o ui_botonera.h

ui_configuracion.h: configuracion.ui
	/usr/lib/x86_64-linux-gnu/qt5/bin/uic configuracion.ui -o ui_configuracion.h

ui_usuario.h: usuario.ui
	/usr/lib/x86_64-linux-gnu/qt5/bin/uic usuario.ui -o ui_usuario.h

ui_sedes.h: sedes.ui
	/usr/lib/x86_64-linux-gnu/qt5/bin/uic sedes.ui -o ui_sedes.h

ui_soporte.h: soporte.ui
	/usr/lib/x86_64-linux-gnu/qt5/bin/uic soporte.ui -o ui_soporte.h

ui_equipos.h: equipos.ui
	/usr/lib/x86_64-linux-gnu/qt5/bin/uic equipos.ui -o ui_equipos.h

ui_basedatos.h: basedatos.ui
	/usr/lib/x86_64-linux-gnu/qt5/bin/uic basedatos.ui -o ui_basedatos.h

ui_acerdade.h: acerdade.ui
	/usr/lib/x86_64-linux-gnu/qt5/bin/uic acerdade.ui -o ui_acerdade.h

ui_mame.h: mame.ui
	/usr/lib/x86_64-linux-gnu/qt5/bin/uic mame.ui -o ui_mame.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_rcc_clean compiler_moc_header_clean compiler_uic_clean 

####### Compile

main.o: main.cpp botonera.h \
		configuracion.h \
		acerdade.h \
		tunel.h \
		libssh2_config.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

botonera.o: botonera.cpp botonera.h \
		configuracion.h \
		acerdade.h \
		ui_botonera.h \
		usuario.h \
		soporte.h \
		nmap_xml.h \
		sedes.h \
		equipos.h \
		tunel.h \
		libssh2_config.h \
		mame.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o botonera.o botonera.cpp

configuracion.o: configuracion.cpp configuracion.h \
		ui_configuracion.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o configuracion.o configuracion.cpp

usuario.o: usuario.cpp usuario.h \
		ui_usuario.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o usuario.o usuario.cpp

nmap_xml.o: nmap_xml.cpp nmap_xml.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o nmap_xml.o nmap_xml.cpp

sedes.o: sedes.cpp sedes.h \
		nmap_xml.h \
		configuracion.h \
		ui_sedes.h \
		basedatos.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o sedes.o sedes.cpp

soporte.o: soporte.cpp soporte.h \
		nmap_xml.h \
		ui_soporte.h \
		equipos.h \
		ejecutahilo.h \
		configuracion.h \
		tabescaner.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o soporte.o soporte.cpp

equipos.o: equipos.cpp equipos.h \
		nmap_xml.h \
		ui_equipos.h \
		configuracion.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o equipos.o equipos.cpp

tunel.o: tunel.cpp tunel.h \
		configuracion.h \
		libssh2_config.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tunel.o tunel.cpp

ejecutahilo.o: ejecutahilo.cpp ejecutahilo.h \
		nmap_xml.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ejecutahilo.o ejecutahilo.cpp

tabescaner.o: tabescaner.cpp tabescaner.h \
		nmap_xml.h \
		equipos.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tabescaner.o tabescaner.cpp

basedatos.o: basedatos.cpp basedatos.h \
		ui_basedatos.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o basedatos.o basedatos.cpp

acerdade.o: acerdade.cpp acerdade.h \
		ui_acerdade.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o acerdade.o acerdade.cpp

mame.o: mame.cpp mame.h \
		ui_mame.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o mame.o mame.cpp

qrc_iconos.o: qrc_iconos.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o qrc_iconos.o qrc_iconos.cpp

moc_botonera.o: moc_botonera.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_botonera.o moc_botonera.cpp

moc_configuracion.o: moc_configuracion.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_configuracion.o moc_configuracion.cpp

moc_usuario.o: moc_usuario.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_usuario.o moc_usuario.cpp

moc_sedes.o: moc_sedes.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_sedes.o moc_sedes.cpp

moc_soporte.o: moc_soporte.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_soporte.o moc_soporte.cpp

moc_equipos.o: moc_equipos.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_equipos.o moc_equipos.cpp

moc_tunel.o: moc_tunel.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_tunel.o moc_tunel.cpp

moc_ejecutahilo.o: moc_ejecutahilo.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_ejecutahilo.o moc_ejecutahilo.cpp

moc_tabescaner.o: moc_tabescaner.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_tabescaner.o moc_tabescaner.cpp

moc_basedatos.o: moc_basedatos.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_basedatos.o moc_basedatos.cpp

moc_acerdade.o: moc_acerdade.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_acerdade.o moc_acerdade.cpp

moc_mame.o: moc_mame.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_mame.o moc_mame.cpp

####### Install

install:  FORCE

uninstall:  FORCE

FORCE:

