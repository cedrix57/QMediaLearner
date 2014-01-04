#include "ffmpegaudiodecoderservice.h"

#include "audioDecoder/ffmpegaudiodecodercontrol.h"

//====================================
FFmpegAudioDecoderService::FFmpegAudioDecoderService(
        QObject *parent) :
    QMediaService(parent){
    this->createControls();
    this->connectSlots();
}
//====================================
void FFmpegAudioDecoderService::createControls(){
    this->audioDecoderControl
            = new FFmpegAudioDecoderControl(this);
}
//====================================
void FFmpegAudioDecoderService::connectSlots(){
}
//====================================
QMediaControl
*FFmpegAudioDecoderService::requestControl(
        const char *name){
    QMediaControl *mediaControl = NULL;
    if (qstrcmp(name, QAudioDecoderControl_iid) == 0){
        mediaControl = this->audioDecoderControl;
    }
    return mediaControl;
}
//====================================
void FFmpegAudioDecoderService::releaseControl(
        QMediaControl *control){
}
//====================================
