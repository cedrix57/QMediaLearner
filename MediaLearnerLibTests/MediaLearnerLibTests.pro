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


release:DESTDIR = ../release
release:OBJECTS_DIR = ../../tmpcompil/release/obj
release:MOC_DIR = ../../tmpcompil/release/moc
release:RCC_DIR = ../../tmpcompil/release/rcc
release:UI_DIR = ../../tmpcompil/release/ui

debug:DESTDIR = ../debug
debug:OBJECTS_DIR = ../../tmpcompil/debug/obj
debug:MOC_DIR = ../../tmpcompil/debug/moc
debug:RCC_DIR = ../../tmpcompil/debug/rcc
debug:UI_DIR = ../../tmpcompil/debug/ui



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Desktop_Qt_5_1_0_GCC_64bit/release/ -lMediaLearnerLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Desktop_Qt_5_1_0_GCC_64bit/debug/ -lMediaLearnerLib
else:unix:CONFIG(release, debug|release): LIBS += -L$$PWD/../Desktop_Qt_5_1_0_GCC_64bit/release/ -lMediaLearnerLib
else:unix:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Desktop_Qt_5_1_0_GCC_64bit/debug/ -lMediaLearnerLib

INCLUDEPATH += $$PWD/../MediaLearnerLib
DEPENDPATH += $$PWD/../MediaLearnerLib
