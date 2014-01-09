#-------------------------------------------------
#
# Project created by QtCreator 2013-10-27T10:47:58
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia-private
QT       += multimedia concurrent multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QMediaLearner
TEMPLATE = app

DESTDIR = ../binaries
OBJECTS_DIR = ../tmpcompil/$$TARGET/obj
MOC_DIR = ../tmpcompil/$$TARGET/moc
RCC_DIR = ../tmpcompil/$$TARGET/rcc
UI_DIR = ../tmpcompil/$$TARGET/ui

unix{
DEFINES += __STDC_LIMIT_MACROS
DEFINES += __STDC_CONSTANT_MACROS
}

INCLUDEPATH += $$PWD/../FFmpegMultimediaPlugin/ffmpeg
INCLUDEPATH += $$PWD/../FFmpegMultimediaPlugin/ffmpeg/libavcodec
INCLUDEPATH += $$PWD/../FFmpegMultimediaPlugin/ffmpeg/libavformat

LIBS += -L$$DESTDIR -lMediaLearnerLib
LIBS += -L$$PWD/../FFmpegMultimediaPlugin/ffmpeg/libavcodec -lavcodec
LIBS += -L$$PWD/../FFmpegMultimediaPlugin/ffmpeg/libavformat -lavformat
LIBS += -L$$PWD/../FFmpegMultimediaPlugin/ffmpeg/libavutil -lavutil

INCLUDEPATH += gui/widgets
INCLUDEPATH += ../MediaLearnerLib

DEPENDPATH += ../MediaLearnerLib

SOURCES += main.cpp\
        gui/MainWindow.cpp \
    gui/dialogs/EditExtractionDialog.cpp \
    gui/widgets/qxtspanslider.cpp \
    gui/widgets/qxtglobal.cpp \
    gui/widgets/SubVideoWidget.cpp \
    gui/dialogs/SettingsDialog.cpp \
    gui/dialogs/ExportVideoDialog.cpp \
    gui/widgets/ClickableSlider.cpp \
    gui/dialogs/CurrentSessionSettingsDialog.cpp

HEADERS  += gui/MainWindow.h \
    gui/dialogs/EditExtractionDialog.h \
    gui/widgets/qxtspanslider.h \
    gui/widgets/qxtspanslider_p.h \
    gui/widgets/qxtnamespace.h \
    gui/widgets/qxtglobal.h \
    gui/widgets/SubVideoWidget.h \
    gui/dialogs/SettingsDialog.h \
    gui/dialogs/ExportVideoDialog.h \
    gui/widgets/ClickableSlider.h \
    gui/dialogs/CurrentSessionSettingsDialog.h

FORMS    += gui/MainWindow.ui \
    gui/dialogs/EditExtractionDialogs.ui \
    gui/dialogs/SettingsDialog.ui \
    gui/dialogs/ExportVideoDialog.ui \
    gui/dialogs/CurrentSessionSettingsDialog.ui

RESOURCES += \
    QMediaLearner.qrc
