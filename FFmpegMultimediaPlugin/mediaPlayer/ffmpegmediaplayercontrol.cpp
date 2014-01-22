
#include <QTimer>
#include <QDebug>

#include "ffmpegmediaplayercontrol.h"
#include "ffmpegplayersession.h"

QT_BEGIN_NAMESPACE

//====================================
FFmpegMediaPlayerControl::FFmpegMediaPlayerControl(
        QObject *parent) :
    QMediaPlayerControl(parent){
    this->session = NULL;
    this->_device = NULL;
}
//====================================
FFmpegMediaPlayerControl::~FFmpegMediaPlayerControl(){
}
//====================================
void FFmpegMediaPlayerControl::setFFmpegPlayerSession(
        FFmpegPlayerSession *session){
    this->session = session;
    this->connect(
                this->session,
                SIGNAL(audioAvailableChanged(bool)),
                SIGNAL(audioAvailableChanged(bool)));
    this->connect(
                this->session,
                SIGNAL(availablePlaybackRangesChanged(const QMediaTimeRange)),
                SIGNAL(availablePlaybackRangesChanged(const QMediaTimeRange)));
    this->connect(
                this->session,
                SIGNAL(durationChanged(qint64)),
                SIGNAL(durationChanged(qint64)));
    this->connect(
                this->session,
                SIGNAL(error(int, const QString)),
                SIGNAL(error(int, const QString)));
    this->connect(
                this->session,
                SIGNAL(mediaChanged(const QMediaContent)),
                SIGNAL(mediaChanged(const QMediaContent)));
    this->connect(
                this->session,
                SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
                SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)));
    this->connect(
                this->session,
                SIGNAL(mutedChanged(bool)),
                SIGNAL(mutedChanged(bool)));
    this->connect(
                this->session,
                SIGNAL(playbackRateChanged(qreal)),
                SIGNAL(playbackRateChanged(qreal)));
    this->connect(
                this->session,
                SIGNAL(positionChanged(qint64)),
                SIGNAL(positionChanged(qint64)));
    this->connect(
                this->session,
                SIGNAL(seekableChanged(bool)),
                SIGNAL(seekableChanged(bool)));
    this->connect(
                this->session,
                SIGNAL(stateChanged(QMediaPlayer::State)),
                SIGNAL(stateChanged(QMediaPlayer::State)));
    this->connect(
                this->session,
                SIGNAL(videoAvailableChanged(bool)),
                SIGNAL(videoAvailableChanged(bool)));
    this->connect(
                this->session,
                SIGNAL(volumeChanged(int)),
                SIGNAL(volumeChanged(int)));

}
//====================================
QMediaPlayer::State FFmpegMediaPlayerControl::state() const{
    QMediaPlayer::State state
            = this->session->state();
    return state;
}
//====================================
QMediaPlayer::MediaStatus FFmpegMediaPlayerControl::mediaStatus() const{
    QMediaPlayer::MediaStatus mediaStatus
            = this->session->mediaStatus();
    return mediaStatus;
}
//====================================
qint64 FFmpegMediaPlayerControl::position() const{
    int position = this->session->position();
    return position;
}
//====================================
qint64 FFmpegMediaPlayerControl::duration() const{
    int duration = this->session->duration();
    return duration;
}
//====================================
int FFmpegMediaPlayerControl::bufferStatus() const{
    return 100;
}
//====================================
int FFmpegMediaPlayerControl::volume() const{
    int volume = this->session->volume();
    return volume;
}
//====================================
bool FFmpegMediaPlayerControl::isMuted() const{
    bool muted = this->session->isMuted();
    return muted;
}
//====================================
bool FFmpegMediaPlayerControl::isAudioAvailable() const{
    bool available = this->session->isAudioAvailable();
    return available;
}
//====================================
bool FFmpegMediaPlayerControl::isVideoAvailable() const{
    bool available = this->session->isVideoAvailable();
    return available;
}
//====================================
bool FFmpegMediaPlayerControl::isSeekable() const{
    bool seekable = this->session->isSeekable();
    return seekable;
}
//====================================
QMediaTimeRange FFmpegMediaPlayerControl::availablePlaybackRanges() const{
    QMediaTimeRange range(0, 30000);
    return range;
}
//====================================
qreal FFmpegMediaPlayerControl::playbackRate() const{
    qreal rate = this->session->playbackRate();
    return rate;
}
//====================================
void FFmpegMediaPlayerControl::setPlaybackRate(
        qreal rate){
    this->session->setPlaybackRate(rate);
}
//====================================
QMediaContent FFmpegMediaPlayerControl::media() const{
    return this->_mediaContent;
}
//====================================
const QIODevice *FFmpegMediaPlayerControl::mediaStream() const{
    return this->_device;
}
//====================================
void FFmpegMediaPlayerControl::setMedia(
        const QMediaContent& mediaContent,
        QIODevice *device){
    this->_mediaContent = mediaContent;
    this->_device = device;
    if(device != NULL){
        qWarning() << "FFmpeg: Device management is not available yet.";
    }else{
        QUrl url = mediaContent.canonicalUrl();
        QString fileUrl = url.url(); //QUrl::EncodeUnicode);
        this->session->setMediaUrl(fileUrl);
        this->mediaChanged(this->_mediaContent);
    }
}
//====================================
void FFmpegMediaPlayerControl::setPosition(
        qint64 pos){
    this->session->setPosition(pos);
}
//====================================
void FFmpegMediaPlayerControl::play(){
    this->session->play();
}
//====================================
void FFmpegMediaPlayerControl::pause(){
    this->session->pause();
}
//====================================
void FFmpegMediaPlayerControl::stop(){
    this->session->stop();
}
//====================================
void FFmpegMediaPlayerControl::setVolume(
        int volume){
    this->session->setVolume(volume);
}
//====================================
void FFmpegMediaPlayerControl::setMuted(
        bool muted){
    this->session->setMuted(muted);
}
//====================================

QT_END_NAMESPACE
