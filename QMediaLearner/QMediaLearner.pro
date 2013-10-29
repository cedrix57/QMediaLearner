#-------------------------------------------------
#
# Project created by QtCreator 2013-10-27T10:47:58
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia concurrent multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QMediaLearner
TEMPLATE = app

DESTDIR = ../binaries
OBJECTS_DIR = ../tmpcompil/$$TARGET/obj
MOC_DIR = ../tmpcompil/$$TARGET/moc
RCC_DIR = ../tmpcompil/$$TARGET/rcc
UI_DIR = ../tmpcompil/$$TARGET/ui

LIBS += -L$$DESTDIR -lMediaLearnerLib

INCLUDEPATH += ../MediaLearnerLib
INCLUDEPATH += gui/widgets
DEPENDPATH += ../MediaLearnerLib

SOURCES += main.cpp\
        gui/MainWindow.cpp \
    gui/dialogs/EditExtractionDialog.cpp \
    gui/widgets/qxtspanslider.cpp \
    gui/widgets/qxtglobal.cpp

HEADERS  += gui/MainWindow.h \
    gui/dialogs/EditExtractionDialog.h \
    gui/widgets/qxtspanslider.h \
    gui/widgets/qxtspanslider_p.h \
    gui/widgets/qxtnamespace.h \
    gui/widgets/qxtglobal.h

FORMS    += gui/MainWindow.ui \
    gui/dialogs/EditExtractionDialogs.ui
