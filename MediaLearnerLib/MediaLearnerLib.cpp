#include "MediaLearnerLib.h"


namespace MediaLearner{

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
//====================================
void MediaLearnerLib::setMedia(QString mediaPath){
    QUrl url = QUrl::fromLocalFile(mediaPath);
    QMediaContent mediaSource(url);
    this->sequenceExtractor.setMediaSource(
                mediaPath);
    this->mediaPlayer.setMedia(
                mediaSource);
    this->sequenceExtractor.analyseMediaSource();
}
//====================================

}

