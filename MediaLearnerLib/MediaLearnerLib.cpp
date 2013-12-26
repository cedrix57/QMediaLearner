#include "MediaLearnerLib.h"
#include "CrashManagerSingleton.h"


namespace ML{

//====================================
MediaLearnerLib::MediaLearnerLib(){
    qDebug() << "MediaLearnerLib::MediaLearnerLib() called";
    CrashManagerSingleton *
            crashManager
            = CrashManagerSingleton::getInstance();
    crashManager->connect(
                &this->mediaPlayer,
                SIGNAL(positionChanged(qint64)),
                SLOT(setMediaPosition(qint64)));
    crashManager->connect(
                &this->mediaPlayer,
                SIGNAL(playbackRateChanged(qreal)),
                SLOT(setPlaybackRate(double)));
    crashManager->connect(
                &this->mediaPlayer,
                SIGNAL(volumeChanged(int)),
                SLOT(setVolume(int)));
    QObject::connect(
                &this->encoder,
                &EncoderInterface::encodingFinished,
                &SequencesWithSubs::setEnodingFinished);
    qDebug() << "MediaLearnerLib::MediaLearnerLib() end";
}
//====================================
MediaLearnerLib::~MediaLearnerLib(){
}
//====================================
QMediaPlayer *MediaLearnerLib::getMediaPlayer(){
    return &this->mediaPlayer;
}
//====================================
SequenceExtractor
*MediaLearnerLib::getSequenceExtractor(){
    return &this->sequenceExtractor;
}
SubtitlesManager
*MediaLearnerLib::getSubtitlesManager(){
    return &this->subtitlesManager;
}
//====================================
void MediaLearnerLib::setMedia(QString mediaPath){
    qDebug() << "void MediaLearnerLib::setMedia(QString mediaPath) called";
    QUrl url = QUrl::fromLocalFile(mediaPath);
    QMediaContent mediaSource(url);
    this->sequenceExtractor.setMediaSource(
                mediaPath);
    this->mediaPlayer.setMedia(
                mediaSource);
    this->encoder.setInVideoFilePath(
                mediaPath);
    this->sequenceExtractor.analyseMediaSource();
    CrashManagerSingleton::getInstance()
            ->setMediaFilePath(
                mediaPath);
    qDebug() << "void MediaLearnerLib::setMedia(QString mediaPath) end";
}
//====================================
EncoderInterface *MediaLearnerLib::getEncoder(){
    return &this->encoder;
}
//====================================
QStringList MediaLearnerLib::getSupportedFormats(){
    QStringList supportedFormats;
    supportedFormats << "avi";
    supportedFormats << "ts";
    supportedFormats << "mp2";
    supportedFormats << "mpg";
    supportedFormats << "mp3";
    supportedFormats << "mp4";
    supportedFormats << "ogg";
    supportedFormats << "ogv";
    supportedFormats << "flv";
    supportedFormats << "mkv";
    supportedFormats << "srt";
    supportedFormats << "mts";
    return supportedFormats;
}
//====================================
bool MediaLearnerLib::isFormatSupported(
        QString &filePath){
    qDebug() << "bool MediaLearnerLib::isFormatSupported(...) called";
    qDebug() << "filePath: " << filePath;
    QString lowerPath = filePath.toLower();
    QStringList supportedFormats
            = this->getSupportedFormats();
    bool supportedVideoFormat = false;
    foreach(QString format, supportedFormats){
        if(lowerPath.endsWith(format)){
            supportedVideoFormat = true;
            break;
        }
    }
    qDebug() << "supportedVideoFormat: " << supportedVideoFormat;
    qDebug() << "bool MediaLearnerLib::isFormatSupported(...) end";
    return supportedVideoFormat;
}
//====================================

}

