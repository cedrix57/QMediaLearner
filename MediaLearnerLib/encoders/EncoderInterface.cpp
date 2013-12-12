#include "EncoderInterface.h"
#include <QStandardPaths>
#include <QFile>
#include <QDir>

namespace MediaLearner{
//====================================
EncoderInterface::EncoderInterface(
        QObject *parent) :
    QObject(parent){
}
//====================================
void EncoderInterface::setFormat(QString format){
    this->format = format;
}
//====================================
void EncoderInterface::setAudioCodec(QString codec){
    this->audioCodec = codec;
}
//====================================
void EncoderInterface::setVideoCodec(QString codec){
    this->videoCodec = codec;
}
//====================================
void EncoderInterface::setSubtitleCodec(QString codec){
    this->subCodec = codec;
}
//====================================
void EncoderInterface::setNewSize(QSize size){
    this->newSize = size;
}
//====================================
void EncoderInterface::setPlaybackRate(double rate){
    this->playbackRate = rate;
}
//====================================
void EncoderInterface::setInVideoFilePath(
        QString inVideoFilePath){
    this->inVideoFilePath
            = inVideoFilePath;
}
//====================================
void EncoderInterface::setSequences(
        QList<SequenceWithSubs> &sequencesWithSubs){
    this->sequencesWithSubs = sequencesWithSubs;
}
//====================================
QString EncoderInterface::getFontPath(QString fontName){
    QStringList fontDirPaths
            = QStandardPaths::standardLocations(
                QStandardPaths::FontsLocation);
    QString fontFilePath = fontName;
    foreach(QString fontDirPath, fontDirPaths){
        QDir fontDir = fontDirPath;
        QString tempFontFilePath
                = fontDir.filePath(fontName);
        if(QFile::exists(tempFontFilePath)){
            fontFilePath = tempFontFilePath;
            break;
        }
    }
    return fontFilePath;
}
//====================================
QSize EncoderInterface::getOriginalSize(){
    return this->newSize;
}
//====================================
void EncoderInterface::selectFormatProfile(
        QString profileName){
    this->profileName = profileName;
}
//====================================
}
