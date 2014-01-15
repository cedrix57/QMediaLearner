#-------------------------------------------------
#
# Project created by QtCreator 2013-12-29T22:40:44
#
#-------------------------------------------------

QT       += multimedia multimedia-private

qtHaveModule(widgets) {
    QT += multimediawidgets-private
    DEFINES += HAVE_WIDGETS
}


PLUGIN_TYPE=mediaservice
PLUGIN_CLASS_NAME = FFmpegServicePlugin
load(qt_build_config)

QT       -= gui

TARGET = FFmpegMultimediaPlugin
TEMPLATE = lib

DESTDIR = ../binaries/plugins/mediaservice
OBJECTS_DIR = ../tmpcompil/$$TARGET/obj
MOC_DIR = ../tmpcompil/$$TARGET/moc
RCC_DIR = ../tmpcompil/$$TARGET/rcc
UI_DIR = ../tmpcompil/$$TARGET/ui

#ffmpeg

unix{
DEFINES += __STDC_LIMIT_MACROS
DEFINES += __STDC_CONSTANT_MACROS
}

INCLUDEPATH += ffmpeg
INCLUDEPATH += ffmpeg/libavcodec
INCLUDEPATH += ffmpeg/libavformat
INCLUDEPATH += ffmpeg/libswscale

LIBS += -L$$PWD/ffmpeg/libavcodec -lavcodec
LIBS += -L$$PWD/ffmpeg/libavformat -lavformat
LIBS += -L$$PWD/ffmpeg/libavutil -lavutil
LIBS += -L$$PWD/ffmpeg/libswscale -lswscale
#QMAKE_CXXFLAGS += -fPIC

SOURCES += ffmpegmultimediaplugin.cpp \
    mediaPlayer/ffmpegmediaplayercontrol.cpp \
    mediaPlayer/ffmpegplayerservice.cpp \
    mediaPlayer/ffmpegvideowidgetcontrol.cpp \
    mediaPlayer/ffmpegvideorendercontrol.cpp \
    mediaPlayer/ffmpegmetadatareadercontrol.cpp \
    mediaPlayer/ffmpegmediaavailabilitycontrol.cpp \
    mediaPlayer/ffmpegmedianetworkaccesscontrol.cpp \
    audioDecoder/ffmpegaudiodecoderservice.cpp \
    audioDecoder/ffmpegaudiodecodercontrol.cpp

HEADERS += ffmpegmultimediaplugin.h\
    mediaPlayer/ffmpegmediaplayercontrol.h \
    mediaPlayer/ffmpegplayerservice.h \
    mediaPlayer/ffmpegvideowidgetcontrol.h \
    mediaPlayer/ffmpegvideorendercontrol.h \
    mediaPlayer/ffmpegmetadatareadercontrol.h \
    mediaPlayer/ffmpegmediaavailabilitycontrol.h \
    mediaPlayer/ffmpegmedianetworkaccesscontrol.h \
    audioDecoder/ffmpegaudiodecoderservice.h \
    audioDecoder/ffmpegaudiodecodercontrol.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

OTHER_FILES += \
    ffmpeg.json
