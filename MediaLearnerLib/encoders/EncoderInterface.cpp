#include "EncoderInterface.h"
#include <QStandardPaths>
#include <QFile>
#include <QDir>

namespace ML{
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
    qDebug() << "QString EncoderInterface::getFontPath(QString fontName) called";
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
    qDebug() << "fontFilePath: " << fontFilePath;
    qDebug() << "QString EncoderInterface::getFontPath(QString fontName) end";
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
ProfileInfo EncoderInterface::getProfile(QString profileName){
    QMap<QString, ProfileInfo>
            profiles
            = this->getAvailableProfiles();
    ProfileInfo profile = profiles[profileName];
    return profile;
}
//====================================
QMap<QString, ProfileInfo>
EncoderInterface::getAvailableProfiles(){
    QMap<QString, ProfileInfo>  profiles
            = getAvailableVideoProfiles();
    QMap<QString, ProfileInfo>  audioProfiles
            = getAvailableAudioProfiles();
    for(QMap<QString, ProfileInfo>::iterator it
        = audioProfiles.begin();
        it != audioProfiles.end();
        ++it){
        profiles[it->name] = *it;
    }
    return profiles;
}
//====================================
QSet<QString> EncoderInterface::getAvailableVideoProfileExts(){
    QSet<QString> exts;
    QMap<QString, ProfileInfo>  profiles
            = getAvailableVideoProfiles();
    for(QMap<QString, ProfileInfo>::iterator it = profiles.begin();
        it != profiles.end();
        ++it){
        exts << it->ext;
    }
    return exts;
}
//====================================
QSet<QString> EncoderInterface::getAvailableAudioProfileExts(){
    QSet<QString> exts;
    QMap<QString, ProfileInfo>  profiles
            = getAvailableAudioProfiles();
    for(QMap<QString, ProfileInfo>::iterator it = profiles.begin();
        it != profiles.end();
        ++it){
        exts << it->ext;
    }
    return exts;
}
//====================================
QString EncoderInterface::getSelectedProfileExt(){
    QString ext = this->getProfileExt(
                this->profileName);
    return ext;
}
//====================================
QString EncoderInterface::getProfileExt(QString profileName){
    QMap<QString, ProfileInfo>  profiles
            = getAvailableProfiles();
    ProfileInfo profileInfo
            = profiles[profileName];
    return profileInfo.ext;
}
//====================================
void EncoderInterface::encode(QString outFilePath){
    qDebug() << "void EncoderInterface::encode(QString outFilePath) called";
    this->encodingStarted();
    this->startEncoding(outFilePath);
    qDebug() << "void EncoderInterface::encode(QString outFilePath) end";
}
//====================================
void EncoderInterface::_onEncodingSuccessful(){
    qDebug() << "void EncoderInterface::_onEncodingSuccessful() called";
    this->lastSavedSequences
            = sequencesWithSubs;
    qDebug() << "void EncoderInterface::_onEncodingSuccessful() end";
}
//====================================

}
