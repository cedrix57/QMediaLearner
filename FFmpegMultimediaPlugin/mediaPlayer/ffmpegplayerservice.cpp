#include "ffmpegplayerservice.h"
#include "ffmpegmediaplayercontrol.h"
#include "ffmpegvideowidgetcontrol.h"
#include "ffmpegvideorendercontrol.h"
#include "ffmpegmediaavailabilitycontrol.h"
#include "ffmpegmedianetworkaccesscontrol.h"
#include "ffmpegmetadatareadercontrol.h"
#include "ffmpegplayersession.h"

QT_BEGIN_NAMESPACE

//====================================
FFmpegPlayerService::FFmpegPlayerService(QObject *parent) :
    QMediaService(parent){
    this->createControls();
    this->connectSlots();
    //TODO connect slots
}
//====================================
void FFmpegPlayerService::createControls(){
    this->playerSession
            = new FFmpegPlayerSession(this);
    this->mediaPlayerControl
            = new FFmpegMediaPlayerControl(this);
    this->mediaPlayerControl
            ->setFFmpegPlayerSession(
                this->playerSession);
    this->videoWidgetControl
            = new FFmpegVideoWidgetControl(this);
    this->videoRenderControl
            = new FFmpegVideoRenderControl(this);
    this->metaDataReaderControl
            = new FFmpegMetaDataReaderControl(this);
    this->mediaNetworkAccessControl
            = new FFmpegMediaNetworkAccessControl(this);
    this->mediaAvailabilityControl
            = new FFmpegMediaAvailabilityControl(this);
}
//====================================
void FFmpegPlayerService::connectSlots(){
}
//====================================
void FFmpegPlayerService::disconnectFrameSlots(){
    //this->videoWidgetControl->disconnect(
                //this->playerSession,
                //SIGNAL(currentFrameChanged(QSharedPointer<QImage>))
                //this->videoWidgetControl,
                //SLOT(set
    this->videoRenderControl->disconnect(
                this->playerSession,
                SIGNAL(currentFrameChanged(QSharedPointer<QImage>)),
                this->videoRenderControl,
                SLOT(setDisplayedFrame(QSharedPointer<QImage>)));
}
//====================================
QMediaControl *FFmpegPlayerService::requestControl(
        const char *name){
    QMediaControl *mediaControl = NULL;
    if (qstrcmp(name, QMediaPlayerControl_iid) == 0){
        mediaControl = this->mediaPlayerControl;
    }else if (qstrcmp(name, QVideoWidgetControl_iid) == 0){
        mediaControl = this->videoWidgetControl;
        /*
        this->disconnectFrameSlots();
        this->videoWidgetControl->connect(
                    this->mediaPlayerControl,
                    SIGNAL(frameAvailable(QVideoFrame)),
                    SLOT(setDisplayedFrame(QVideoFrame)));
                    //*/
    }else if (qstrcmp(name, QMetaDataReaderControl_iid) == 0){
        mediaControl = this->metaDataReaderControl;
    }else if (qstrcmp(name, QMediaNetworkAccessControl_iid) == 0){
        mediaControl = this->mediaNetworkAccessControl;
    }else if (qstrcmp(name, QMediaAvailabilityControl_iid) == 0){
        mediaControl = this->mediaAvailabilityControl;
    }else if (qstrcmp(name, QVideoRendererControl_iid) == 0){
        mediaControl = this->videoRenderControl;
        //this->disconnectFrameSlots();
        this->connect(
                    this->playerSession,
                    SIGNAL(currentFrameChanged(QSharedPointer<QImage>)),
                    this->videoRenderControl,
                    SLOT(setDisplayedFrame(QSharedPointer<QImage>)),
                    Qt::BlockingQueuedConnection);
    }
    //}else if (qstrcmp(name, QMediaVideoProbeControl_iid) == 0){
    //}else if (qstrcmp(name, QMediaAudioProbeControl_iid) == 0){
    //*/
    return mediaControl;
}
//====================================
void FFmpegPlayerService::releaseControl(
        QMediaControl *control){
    //TODO
}
//====================================

QT_END_NAMESPACE
