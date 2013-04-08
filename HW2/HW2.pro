#-------------------------------------------------
#
# Project created by QtCreator 2013-04-08T12:22:41
#
#-------------------------------------------------

QT       += core gui

TARGET = HW2
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    hw2.cpp

HEADERS += \
    hw2.h \
    driver.h

INCLUDEPATH += ../3rdparty/glm-0.9.4.1/include

DEFINES += _USE_MATH_DEFINES

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../3rdparty/openmesh/lib/ -lOpenMeshCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../3rdparty/openmesh/lib/ -lOpenMeshCored
else:unix: LIBS += -L$$PWD/../3rdparty/openmesh/lib/ -lOpenMeshCore

INCLUDEPATH += $$PWD/../3rdparty/openmesh/include
DEPENDPATH += $$PWD/../3rdparty/openmesh/include

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../3rdparty/openmesh/lib/OpenMeshCore.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../3rdparty/openmesh/lib/OpenMeshCored.lib
else:unix: PRE_TARGETDEPS += $$PWD/../3rdparty/openmesh/lib/libOpenMeshCore.a
