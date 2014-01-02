#include "ffmpegmediaplayercontrol.h"

#include <QTimer>
#include <QDebug>

QT_BEGIN_NAMESPACE

//====================================
FFmpegMediaPlayerControl::FFmpegMediaPlayerControl(
        QObject *parent) :
    QMediaPlayerControl(parent){
    this->_rate = 1.0;
    this->_volume = 100;
    this->_position = 0;
    this->_duration = 30000;
    this->_muted = false;
    this->_state = QMediaPlayer::StoppedState;
    this->_mediaStatus = QMediaPlayer::NoMedia;
    this->_device = NULL;
}
//====================================
FFmpegMediaPlayerControl::~FFmpegMediaPlayerControl(){
}
//====================================
QMediaPlayer::State FFmpegMediaPlayerControl::state() const{
    return this->_state;
}
//====================================
QMediaPlayer::MediaStatus FFmpegMediaPlayerControl::mediaStatus() const{
    return this->_mediaStatus;
}
//====================================
qint64 FFmpegMediaPlayerControl::position() const{
    return this->_position;
}
//====================================
qint64 FFmpegMediaPlayerControl::duration() const{
    return this->_duration;
}
//====================================
int FFmpegMediaPlayerControl::bufferStatus() const{
    return 100;
}
//====================================
int FFmpegMediaPlayerControl::volume() const{
    return this->_volume;
}
//====================================
bool FFmpegMediaPlayerControl::isMuted() const{
    return this->_muted;
}
//====================================
bool FFmpegMediaPlayerControl::isAudioAvailable() const{
    return true;
}
//====================================
bool FFmpegMediaPlayerControl::isVideoAvailable() const{
    return true;
}
//====================================
bool FFmpegMediaPlayerControl::isSeekable() const{
    return true;
}
//====================================
QMediaTimeRange FFmpegMediaPlayerControl::availablePlaybackRanges() const{
    QMediaTimeRange range(0, 30000);
    return range;
}
//====================================
qreal FFmpegMediaPlayerControl::playbackRate() const{
    return this->_rate;
}
//====================================
void FFmpegMediaPlayerControl::setPlaybackRate(
        qreal rate){
    this->_rate = rate;
    this->playbackRateChanged(this->_rate);
}
//====================================
QMediaContent FFmpegMediaPlayerControl::media() const{
    return this->mediaContent;
}
//====================================
const QIODevice *FFmpegMediaPlayerControl::mediaStream() const{
    return this->_device;
}
//====================================
void FFmpegMediaPlayerControl::setMedia(
        const QMediaContent& mediaContent,
        QIODevice *device){
    this->_mediaStatus = QMediaPlayer::LoadedMedia;
    this->mediaContent = mediaContent;
    this->_device = device;
    if(device != NULL){
    }else{
    }
    this->mediaStatusChanged(this->_mediaStatus);
    this->mediaChanged(this->mediaContent);
}
//====================================
void FFmpegMediaPlayerControl::setPosition(
        qint64 pos){
    this->positionChanged(pos);
}
//====================================
void FFmpegMediaPlayerControl::play(){
    this->_state = QMediaPlayer::PlayingState;
    this->currentFrame
            = QVideoFrame(
                QImage(
                    "/home/cedric/Images/ffmpeg_qtplugin_01.png"));
    this->bufferStatusChanged(100);
    this->durationChanged(this->_duration);
    this->stateChanged(this->_state);
    this->seekableChanged(true);
    this->tempPlay();
}
//====================================
void FFmpegMediaPlayerControl::pause(){
    this->_state = QMediaPlayer::PausedState;
    this->stateChanged(this->_state);
}
//====================================
void FFmpegMediaPlayerControl::stop(){
    this->_state = QMediaPlayer::StoppedState;
    this->stateChanged(this->_state);
}
//====================================
void FFmpegMediaPlayerControl::setVolume(
        int volume){
    this->_volume = volume;
    this->volumeChanged(this->_volume);
}
//====================================
void FFmpegMediaPlayerControl::setMuted(
        bool muted){
    this->_muted = muted;
    this->mutedChanged(this->_muted);
}
//====================================
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void FFmpegMediaPlayerControl::tempPlay(){
    qDebug() << "Temp play...";
    if(this->_state == QMediaPlayer::PlayingState){
        int interval = 200;
        this->_position += interval;
        if(this->_position < this->duration()){
            QTimer::singleShot(
                        interval,
                        this,
                        SLOT(tempPlay()));
            this->positionChanged(this->_position);
            this->frameAvailable(this->currentFrame);
        }else{
            this->_state = QMediaPlayer::StoppedState;
            this->stateChanged(this->_state);
        }
    }
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

QT_END_NAMESPACE
