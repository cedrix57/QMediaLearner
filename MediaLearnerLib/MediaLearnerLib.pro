#-------------------------------------------------
#
# Project created by QtCreator 2013-08-21T21:37:30
#
#-------------------------------------------------

QT       += multimedia concurrent

QT       -= gui

TARGET = MediaLearnerLib
TEMPLATE = lib

DEFINES += MEDIALEARNERLIB_LIBRARY

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

SOURCES += MediaLearnerLib.cpp \
    sequenceExtractor/SequenceExtractor.cpp \
    sequenceExtractor/PluginSequenceExtractor.cpp \
    subtitles/SubtitleTrack.cpp \
    subtitles/SubtitlesManager.cpp \
    ffmpeg/ffmpegInterface.cpp \
    sequenceExtractor/HighFrequencyExtractor.cpp

HEADERS += MediaLearnerLib.h\
        medialearnerlib_global.h \
    sequenceExtractor/SequenceExtractor.h \
    sequenceExtractor/PluginSequenceExtractor.h \
    subtitles/SubtitlesManager.h \
    subtitles/SubtitleTrack.h \
    ffmpeg/ffmpegInterface.h \
    sequenceExtractor/HighFrequencyExtractor.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

OTHER_FILES += \
    TODO.txt
