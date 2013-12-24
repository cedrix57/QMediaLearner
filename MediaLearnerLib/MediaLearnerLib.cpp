#include "MediaLearnerLib.h"
#include "CrashManagerSingleton.h"


namespace ML{

//====================================
MediaLearnerLib::MediaLearnerLib(){
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
}
//====================================
EncoderInterface *MediaLearnerLib::getEncoder(){
    return &this->encoder;
}
//====================================

}

