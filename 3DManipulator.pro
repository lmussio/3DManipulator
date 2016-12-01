#-------------------------------------------------
#
# Project created by QtCreator 2016-11-12T16:43:18
#
#-------------------------------------------------

QT += core gui widgets
QT += opengl
LIBS += -lGLU
QMAKE_CXXFLAGS += -std=c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3DManipulator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    camera.cpp \
    object.cpp \
    spnav.c \
    mouse3d.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    camera.h \
    object.h \
    spnav.h \
    mouse3d.h

FORMS    += mainwindow.ui

DISTFILES += \
    .gitignore \
    README.md

RESOURCES +=
