#include "ffmpegplayersession.h"
#include <QDebug>
#include <QMutexLocker>

//====================================
FFmpegPlayerSession::FFmpegPlayerSession(
        QObject *parent) :
    QThread(parent){
    this->_playbackRate = 1;
    this->_muted = false;
    this->_volume = 100;
    this->_position = 0;
    this->reset();
}
//====================================
FFmpegPlayerSession::~FFmpegPlayerSession(){
    this->freeMemory();
}
//====================================
void FFmpegPlayerSession::reset(){
    this->_position = 0;
    this->_toSeek = -1;
    //TODO free memory eventually
    this->avFormatContex = NULL;
    this->avVideoCodecContex = NULL;
    this->avAudioCodecContex = NULL;
    this->swImageConvertContext = NULL;
    this->avFrameRGB = NULL;
    this->_rbgFrameBuffer = NULL;
}
//====================================
void FFmpegPlayerSession::freeMemory(){
    av_free(this->_rbgFrameBuffer);
    av_free(this->avFrameRGB);

    avcodec_close(this->avAudioCodecContex);
    avcodec_close(this->avVideoCodecContex);

    av_close_input_file(this->avFormatContex);
}
//====================================
QString FFmpegPlayerSession::mediaUrl() const{
    return this->_mediaUrl;
}
//====================================
void FFmpegPlayerSession::setMediaUrl(
        QString mediaUrl){
    this->_setMediaStatus(QMediaPlayer::LoadingMedia);
    this->freeMemory();
    this->reset();
    this->_mediaUrl = mediaUrl;
    QByteArray filePathBytes
            = this->_mediaUrl.toUtf8();
    char *filePathChar = filePathBytes.data();
    int ret = avformat_open_input(
                &this->avFormatContex,
                filePathChar,
                NULL,
                NULL);
    if(ret != 0){
        qWarning() << "FFmpeg: Couldn't open " << mediaUrl << ". return code: " << ret;
        char ffmpegError[500];
        av_strerror(ret, ffmpegError, 500);
        qWarning() << ffmpegError;
    }else{
        ret = avformat_find_stream_info(this->avFormatContex, NULL);
        if(ret < 0){
            qWarning() << "FFmpeg: Couldn't find stream info of " << mediaUrl << ". return code: " << ret;
        }else{
            av_dump_format(this->avFormatContex, 0, filePathChar, 0);
            this->videoStreamId = -1;
            this->audioStreamId = -1;
            for(int i=0; i<this->avFormatContex->nb_streams; i++){
                if(this->avFormatContex->streams[i]->codec->codec_type
                        == AVMEDIA_TYPE_VIDEO) {
                    //TODO add list that reccord all streams and add stream selection feature
                    this->videoStreamId = i;
                }else if(this->avFormatContex->streams[i]->codec->codec_type
                         == AVMEDIA_TYPE_AUDIO) {
                    this->audioStreamId = i;
                }else if(this->avFormatContex->streams[i]->codec->codec_type
                         == AVMEDIA_TYPE_SUBTITLE) {
                    //TODO handle subtitle
                }
            }
            if(this->videoStreamId != -1){
                this->avVideoCodecContex
                        = this->avFormatContex->streams[
                        this->videoStreamId]->codec;
                AVCodec *codec //TODO free?
                        = avcodec_find_decoder(
                            this->avVideoCodecContex->codec_id);
                ret = avcodec_open2(this->avVideoCodecContex, codec, NULL);
                if(ret<0){
                    qWarning() << "FFmpeg: Couldn't open the requiered video codec"
                               << this->avVideoCodecContex->codec_name
                               << " of " << mediaUrl;
                }else{
                    this->avFrameRGB = av_frame_alloc();
                    int numBytes
                            = avpicture_get_size(
                                PIX_FMT_RGB24,
                                this->avVideoCodecContex->width,
                                this->avVideoCodecContex->height);
                    this->_rbgFrameBuffer
                            = (quint8 *) av_malloc(
                                numBytes*sizeof(quint8));
                    avpicture_fill(
                                (AVPicture *)this->avFrameRGB,
                                this->_rbgFrameBuffer,
                                PIX_FMT_RGB24,
                                this->avVideoCodecContex->width,
                                this->avVideoCodecContex->height);
                    this->swImageConvertContext
                            = sws_getContext(
                                this->avVideoCodecContex->width,
                                this->avVideoCodecContex->height,
                                this->avVideoCodecContex->pix_fmt,
                                this->avVideoCodecContex->width,
                                this->avVideoCodecContex->height,
                                PIX_FMT_RGB555,
                                SWS_FAST_BILINEAR,
                                NULL,
                                NULL,
                                NULL);
                    if(this->swImageConvertContext != NULL){
                        this->videoStreamId = -1;
                    }else{
                    qWarning() << "FFmpeg: Couldn't create ffmpeg sws image conveverter context of "
                               << mediaUrl;
                    }
                }
            }
            if(this->audioStreamId != -1){
                this->avAudioCodecContex
                        = this->avFormatContex->streams[
                        this->audioStreamId]->codec;
                AVCodec *codec //TODO free?
                        = avcodec_find_decoder(
                            this->avAudioCodecContex->codec_id);
                ret = avcodec_open2(this->avAudioCodecContex, codec, NULL);
                if(ret<0){
                    qWarning() << "FFmpeg: Couldn't open the requiered audio codec"
                               << this->avAudioCodecContex->codec_name
                               << " of " << mediaUrl;
                    this->audioStreamId = -1;
                }else{
                    //TODO alocate audioBuffer
                }
            }
        }
    }
    if(this->audioStreamId == -1 && this->audioStreamId == -1){
        this->_setMediaStatus(QMediaPlayer::LoadedMedia);
    }else{
        this->_setMediaStatus(QMediaPlayer::NoMedia);
    }
}
//====================================
void FFmpegPlayerSession::play(){
    if(this->_mediaStatus == QMediaPlayer::LoadedMedia
            || this->_mediaStatus == QMediaPlayer::BufferingMedia
            || this->_mediaStatus == QMediaPlayer::BufferedMedia
            || this->_mediaStatus == QMediaPlayer::EndOfMedia){
        this->_setState(QMediaPlayer::PlayingState);
        if(!this->isRunning()){
            this->start();
        }
    }else{
        qWarning() << "Can't play the media yet.";
    }
}
//====================================
void FFmpegPlayerSession::pause(){
    this->_setState(QMediaPlayer::PausedState);
}
//====================================
void FFmpegPlayerSession::stop(){
    this->_setState(QMediaPlayer::StoppedState);
    this->freeMemory();
    this->reset();
}
//====================================
void FFmpegPlayerSession::run(){
    AVFrame *avFrame = av_frame_alloc();
    double timeBase
            = av_q2d(
                this->avFormatContex
                ->streams[this->videoStreamId]->time_base);
    double msTimeBase = timeBase * 1000;
    while(av_read_frame(this->avFormatContex, &this->_avPacket)>=0){
        //TODO if(this->_toSeek != -1)
        qint64 dts = this->_avPacket.dts * msTimeBase;
        qint64 pts = this->_avPacket.pts * msTimeBase;
        QDateTime decodingDateTime = QDateTime::currentDateTime();
        int frameFinished = 0;
        if(this->_avPacket.stream_index == this->videoStreamId){
            while(this->_state == QMediaPlayer::PausedState){
                this->usleep(100);
            }
            if(this->_state == QMediaPlayer::StoppedState){
                break;
            }
            this->stateMutex.lock();
            //TODO decode up to 5 frame in a buffer
            avcodec_decode_video2(this->avVideoCodecContex,
                                  avFrame,
                                  &frameFinished,
                                  &this->_avPacket);
            // Did we get a video frame?
            if(frameFinished){
                // Convert the image from its native format to RGB
                sws_scale(
                            this->swImageConvertContext,
                            avFrame->data,
                            avFrame->linesize,
                            0,
                            this->avVideoCodecContex->height,
                            this->avFrameRGB->data,
                            this->avFrameRGB->linesize);
                QDateTime currentDateTime
                        = QDateTime::currentDateTime();
                int ellapsedMSecs = decodingDateTime.
                        msecsTo(currentDateTime);
                qint64 advance = (pts - dts) / this->_playbackRate - ellapsedMSecs;
                if(advance > 0){
                    this->usleep(advance);
                }
                //this->imageMutex.lock();
                this->currentImage
                        = QSharedPointer<QImage>(
                            new QImage(
                                this->avFrameRGB->data[0],
                                this->avVideoCodecContex->width,
                                this->avVideoCodecContex->height,
                                this->avFrameRGB->linesize[0],
                                QImage::Format_RGB555));
                this->_setPosition(pts);
                //this->imageMutex.unlock();
            }
            this->stateMutex.unlock();
        }
        av_free_packet(&this->_avPacket);
    }
    av_free(avFrame);
    this->stop();
    this->_setMediaStatus(QMediaPlayer::EndOfMedia);
}
//====================================
qint64 FFmpegPlayerSession::duration() const{
    qint64 duration = 0;
    if(this->avFormatContex != NULL){
        duration = this->avFormatContex->duration;
        duration *= AV_TIME_BASE * 1000;
    }
    return duration;
}
//====================================
qint64 FFmpegPlayerSession::position() const{
    return this->_position;
}
//====================================
void FFmpegPlayerSession::setPosition(
        qint64 pos){
    //process with a toSeek variable
    this->_toSeek = pos;
    this->_setPosition(pos);
}
//====================================
void FFmpegPlayerSession::_setPosition(qint64 pos){
    this->_position = pos;
    this->positionChanged(this->_position);
}
//====================================
int FFmpegPlayerSession::volume() const{
    return this->_volume;
}
//====================================
void FFmpegPlayerSession::setVolume(int volume){
    this->_volume = volume;
}
//====================================
bool FFmpegPlayerSession::isMuted() const{
    return this->_muted;
}
//====================================
void FFmpegPlayerSession::setMuted(bool muted){
    this->_muted = muted;
}
//====================================
qreal FFmpegPlayerSession::playbackRate() const{
    return this->_playbackRate;
}
//====================================
void FFmpegPlayerSession::setPlaybackRate(qreal rate){
    this->_playbackRate = rate;
    this->playbackRateChanged(rate);
}
//====================================
QMediaPlayer::State FFmpegPlayerSession::state() const{
    return this->_state;
}
//====================================
QMediaPlayer::MediaStatus
FFmpegPlayerSession::mediaStatus() const{
    return this->_mediaStatus;
}
//====================================
void FFmpegPlayerSession::_setState(
        QMediaPlayer::State state){
    QMutexLocker mutexLocker(&this->stateMutex);
    this->_state = state;
    this->stateChanged(this->_state);
}
//====================================
void FFmpegPlayerSession::_setMediaStatus(
        QMediaPlayer::MediaStatus mediaStatus){
    this->_mediaStatus = mediaStatus;
    this->mediaStatusChanged(this->_mediaStatus);
}
//====================================

