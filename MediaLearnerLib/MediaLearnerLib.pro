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


DESTDIR = ../binaries
OBJECTS_DIR = ../tmpcompil/$$TARGET/obj
MOC_DIR = ../tmpcompil/$$TARGET/moc
RCC_DIR = ../tmpcompil/$$TARGET/rcc
UI_DIR = ../tmpcompil/$$TARGET/ui

SOURCES += MediaLearnerLib.cpp \
    sequenceExtractor/SequenceExtractor.cpp \
    sequenceExtractor/PluginSequenceExtractor.cpp \
    subtitles/SubtitleTrack.cpp \
    subtitles/SubtitlesManager.cpp \
    encoders/FFmpegEncoder.cpp \
    sequenceExtractor/HighFrequencyExtractor.cpp \
    SettingsManagerSingleton.cpp \
    encoders/EncoderInterface.cpp \
    Utils/Sequence.cpp \
    subtitles/SubSequence.cpp \
    encoders/SequenceWithSubs.cpp \
    CrashManagerSingleton.cpp \
    Utils/log.cpp \
    FontManagerSingleton.cpp

HEADERS += MediaLearnerLib.h\
        medialearnerlib_global.h \
    sequenceExtractor/SequenceExtractor.h \
    sequenceExtractor/PluginSequenceExtractor.h \
    subtitles/SubtitlesManager.h \
    subtitles/SubtitleTrack.h \
    encoders/FFmpegEncoder.h \
    sequenceExtractor/HighFrequencyExtractor.h \
    SettingsManagerSingleton.h \
    encoders/EncoderInterface.h \
    Utils/Sequence.h \
    subtitles/SubSequence.h \
    encoders/SequenceWithSubs.h \
    CrashManagerSingleton.h \
    Utils/log.h \
    FontManagerSingleton.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

