#-------------------------------------------------
#
# Project created by QtCreator 2013-04-05T13:33:40
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = M3DEdit
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    axisrenderer.cpp \
    camera.cpp \
    geometrymanager.cpp \
    geometry.cpp \
    box.cpp \
    axiscamera.cpp \
    persepectivecamera.cpp \
    axisviewwidget.cpp \
    perspectiverenderer.cpp \
    renderedviewwidget.cpp \
    materialmanager.cpp \
    material.cpp

HEADERS  += mainwindow.h \
    axisrenderer.h \
    camera.h \
    geometrymanager.h \
    geometry.h \
    box.h \
    axiscamera.h \
    persepectivecamera.h \
    axisviewwidget.h \
    perspectiverenderer.h \
    renderedviewwidget.h \
    materialmanager.h \
    material.h

FORMS    += mainwindow.ui

RESOURCES += \
    shaders.qrc

OTHER_FILES += \
    axisvert.vert \
    axisfrag.frag \
    grid.vert \
    perview.vert \
    perview.frag
