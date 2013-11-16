#include "EncoderInterface.h"

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
void EncoderInterface::setSize(QSize size){
    this->size = size;
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

}
