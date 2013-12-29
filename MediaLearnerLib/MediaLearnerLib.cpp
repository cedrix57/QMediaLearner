#include "MediaLearnerLib.h"
#include "CrashManagerSingleton.h"
#include "SettingsManagerSingleton.h"


namespace ML{

//====================================
MediaLearnerLib::MediaLearnerLib(QObject *parent)
    : QObject(parent){
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
    SettingsManagerSingleton
            *settingsManager
            = SettingsManagerSingleton::getInstance();
    this->connect(
                settingsManager,
                SIGNAL(subSettingsChanged()),
                SLOT(_onDrawingSettingsChanged()));
    qDebug() << "MediaLearnerLib::MediaLearnerLib() end";
}
//====================================
MediaLearnerLib::~MediaLearnerLib(){
    this->stop();
}
//====================================
void MediaLearnerLib::stop(){
    this->mediaPlayer.stop();
    this->sequenceExtractor.stop();
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
QString MediaLearnerLib::getMediaFilePath(){
    QUrl mediaUrl
            = this->mediaPlayer
            .media()
            .canonicalUrl();
    QString mediaFilePath
            = mediaUrl.path();
    return mediaFilePath;
}
//====================================
EncoderInterface *MediaLearnerLib::getEncoder(){
    return &this->encoder;
}
//====================================
QStringList MediaLearnerLib::getSupportedFormats(){
    QStringList supportedFormats;
    //TODO find out available supported format & codecs
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
    supportedFormats << "wmv";
    return supportedFormats;
}
//====================================
bool MediaLearnerLib::isFormatSupported(
        QString &filePath){
    qDebug() << "bool MediaLearnerLib::isFormatSupported(...) called";
    qDebug() << "filePath: " << filePath;
    QStringList supportedFormats
            = this->getSupportedFormats();
    bool supportedVideoFormat = false;
    foreach(QString format, supportedFormats){
        if(filePath.endsWith(format, Qt::CaseInsensitive)){
            supportedVideoFormat = true;
            break;
        }
    }
    qDebug() << "supportedVideoFormat: " << supportedVideoFormat;
    qDebug() << "bool MediaLearnerLib::isFormatSupported(...) end";
    return supportedVideoFormat;
}
//====================================
void MediaLearnerLib::_onDrawingSettingsChanged(){
    bool isPaused
            = this->mediaPlayer.state()
            == QMediaPlayer::PausedState;
    if(isPaused){
        int position = this->mediaPlayer.position();
        this->mediaPlayer.setPosition(position);
    }

}
//====================================

}

