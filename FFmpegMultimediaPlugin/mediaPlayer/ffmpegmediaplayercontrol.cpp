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
    return 30000;
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
    this->tempPlay();
    this->stateChanged(this->_state);
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
    if(this->_state == QMediaPlayer::PlayingState){
        this->_position += 1000;
        if(this->_position < this->duration()){
            QTimer::singleShot(
                        1000,
                        this,
                        SLOT(tempPlay()));
            this->positionChanged(this->_position);
        }else{
            this->_state = QMediaPlayer::StoppedState;
            this->stateChanged(this->_state);
        }
    }
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

QT_END_NAMESPACE
