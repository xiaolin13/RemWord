#-------------------------------------------------
#
# Project created by QtCreator 2017-05-13T16:00:10
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += sql
QT       += multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ILC
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        login.cpp \
    codearea.cpp \
    registaccount.cpp \
    forgetpass.cpp \
    userinterface.cpp \
    usercenter.cpp \
    blowup.cpp \
    soundhttp.cpp

HEADERS  += login.h \
    codearea.h \
    registaccount.h \
    forgetpass.h \
    userinterface.h \
    usercenter.h \
    blowup.h \
    soundhttp.h

FORMS    += login.ui \
    registaccount.ui \
    forgetpass.ui \
    userinterface.ui \
    usercenter.ui \
    blowup.ui

RESOURCES += \
    background.qrc
