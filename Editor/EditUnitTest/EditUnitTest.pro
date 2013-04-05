#-------------------------------------------------
#
# Project created by QtCreator 2013-04-05T17:43:35
#
#-------------------------------------------------

QT       += opengl testlib

TARGET = tst_editunittesttest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_editunittesttest.cpp \
    ../M3DEdit/geometry.cpp \
    ../M3DEdit/box.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../M3DEdit/geometry.h \
    ../M3DEdit/box.h
