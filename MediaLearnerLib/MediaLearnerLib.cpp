#include "MediaLearnerLib.h"


namespace ML{

//====================================
MediaLearnerLib::MediaLearnerLib(){
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
}
//====================================
EncoderInterface *MediaLearnerLib::getEncoder(){
    return &this->encoder;
}
//====================================

}

