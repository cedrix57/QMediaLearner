#include "ffmpegplayerservice.h"
#include "ffmpegmediaplayercontrol.h"

QT_BEGIN_NAMESPACE

//====================================
FFmpegPlayerService::FFmpegPlayerService(QObject *parent) :
    QMediaService(parent){
    this->mediaPlayerControl
            = new FFmpegMediaPlayerControl(this);
}
//====================================
QMediaControl *FFmpegPlayerService::requestControl(
        const char *name){
    QMediaControl *mediaControl = NULL;
    if (qstrcmp(name, QMediaPlayerControl_iid) == 0){
        mediaControl = this->mediaPlayerControl;
        /*
    }else if (qstrcmp(name, QMetaDataReaderControl_iid) == 0){
    }else if (qstrcmp(name, QMediaStreamsControl_iid) == 0){
    }else if (qstrcmp(name, QMediaAvailabilityControl_iid) == 0){
    }else if (qstrcmp(name, QMediaVideoProbeControl_iid) == 0){
    }else if (qstrcmp(name, QMediaAudioProbeControl_iid) == 0){
    //*/
    }
    return mediaControl;
}
//====================================
void FFmpegPlayerService::releaseControl(
        QMediaControl *control){
    delete control;
}
//====================================

QT_END_NAMESPACE
