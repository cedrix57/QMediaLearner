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
/*
void MediaLearnerLib::setMedia(QString mediaPath){
    QMediaContent mediaSource(mediaPath);
    this->mediaPlayer.setMedia(
                mediaSource);
    this->sequenceExtractor.setMediaSource(
                mediaPath);
}
//====================================
//*/

}

