#-------------------------------------------------
#
# Project created by QtCreator 2013-10-05T22:50:38
#
#-------------------------------------------------

QT       += multimedia testlib

TARGET = tst_medialearnerlibtests
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_medialearnerlibtests.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"


DESTDIR = ../binaries
OBJECTS_DIR = ../tmpcompil/$$TARGET/obj
MOC_DIR = ../tmpcompil/$$TARGET/moc
RCC_DIR = ../tmpcompil/$$TARGET/rcc
UI_DIR = ../tmpcompil/$$TARGET/ui

LIBS += -L$$DESTDIR -lMediaLearnerLib

INCLUDEPATH += ../MediaLearnerLib
DEPENDPATH += ../MediaLearnerLib
