#include "ffmpegplayersession.h"
#include <QDebug>
#include <QMutexLocker>

//====================================
FFmpegPlayerSession::FFmpegPlayerSession(
        QObject *parent) :
    QObject(parent){
    this->_playbackRate = 1;
    this->_muted = false;
    this->_volume = 100;
    this->_position = 0;
    this->producer.init(this);
    this->consumer.init(this);
    this->timePosition.start();
    static bool isRegisteredFFmpegFormats = false;
    if(!isRegisteredFFmpegFormats){
        av_register_all();
        isRegisteredFFmpegFormats = true;
    }
    this->reset();
    this->alDevice = NULL;
    this->alContext = NULL;
    this->_initOpenAl();
}
//====================================
void FFmpegPlayerSession::_initOpenAl(){
    this->alDevice = alcOpenDevice(NULL);
    if(this->alDevice != NULL){
        this->alContext
                = alcCreateContext(this->alDevice, NULL);
    }
    if(this->alDevice == NULL){
        qWarning() << "Couldn't init OpenAL. No audio device found.";
    }else if(this->alContext == NULL){
        qWarning() << "Couldn't create OpenAL context";
    }else if(!alcMakeContextCurrent(this->alContext)){
        qWarning() << "Couldn't select the opened OpenAL context";
    }
}
//====================================
FFmpegPlayerSession::~FFmpegPlayerSession(){
    this->freeMemory();
    //delete this->timer;
}
//====================================
void FFmpegPlayerSession::reset(){
    qDebug() << "void FFmpegPlayerSession::reset() called";
    this->_position = 0;
    this->_toSeek = -1;
    this->videoStreamId = -1;
    this->audioStreamId = -1;
    this->avFormatContex = NULL;
    this->avVideoCodecContex = NULL;
    this->avAudioCodecContex = NULL;
    this->swImageConvertContext = NULL;
    qDebug() << "void FFmpegPlayerSession::reset() end";
}
//====================================
void FFmpegPlayerSession::freeMemory(){
    qDebug() << "void FFmpegPlayerSession::freeMemory() called";
    this->bufferOfEmptyImages << this->bufferOfImages;
    foreach(FFmpegBufferedImage imageBuffer, this->bufferOfEmptyImages){
        av_free(imageBuffer.avFrameBuffer);
        imageBuffer.avFrameBuffer = NULL;
        av_free(imageBuffer.avFrameRGB);
        imageBuffer.avFrameRGB = NULL;
    }
    this->bufferOfEmptyImages.clear();
    this->bufferOfImages.clear();

    avcodec_close(this->avAudioCodecContex);
    this->avAudioCodecContex = NULL;
    qDebug() << "Closing this->avVideoCodecContex...";
    avcodec_close(this->avVideoCodecContex);
    qDebug() << "this->avVideoCodecContex closed.";
    this->avVideoCodecContex = NULL;

    av_close_input_file(this->avFormatContex);
    this->avFormatContex = NULL;
    qDebug() << "void FFmpegPlayerSession::freeMemory() end";
}
//====================================
QString FFmpegPlayerSession::mediaUrl() const{
    return this->_mediaUrl;
}
//====================================
void FFmpegPlayerSession::setMediaUrl(
        QString mediaUrl){
    qDebug() << "void FFmpegPlayerSession::setMediaUrl(...) called";
    this->stop();
    this->_setMediaStatus(QMediaPlayer::LoadingMedia);
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
                    int numBytes
                            = avpicture_get_size(
                                PIX_FMT_RGB24,
                                this->avVideoCodecContex->width,
                                this->avVideoCodecContex->height);
                    for(int i=0; i<bufferSize; i++){
                        FFmpegBufferedImage imageBuffer;
                        imageBuffer.avFrameRGB = av_frame_alloc();
                        imageBuffer.avFrameBuffer
                                = (quint8 *) av_malloc(
                                    numBytes*sizeof(quint8));
                        avpicture_fill(
                                    (AVPicture *)imageBuffer.avFrameRGB,
                                    imageBuffer.avFrameBuffer,
                                    PIX_FMT_RGB24,
                                    this->avVideoCodecContex->width,
                                    this->avVideoCodecContex->height);
                        this->bufferOfEmptyImages.enqueue(imageBuffer);
                    }
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
                    if(this->swImageConvertContext ==  NULL){
                        this->videoStreamId = -1;
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
        qDebug() << "Setting NoMedia status...";
        this->_setMediaStatus(QMediaPlayer::NoMedia);
    }else{
        this->_setMediaStatus(QMediaPlayer::LoadedMedia);
        qint64 duration = this->duration();
        this->durationChanged(duration);
    }
    qDebug() << "void FFmpegPlayerSession::setMediaUrl(...) end";
}
//====================================
void FFmpegPlayerSession::play(){
    qDebug() << "void FFmpegPlayerSession::play() called";
    if(this->avFormatContex == NULL){
        qDebug() << "if(this->avFormatContex == NULL){ called";
        this->setMediaUrl(this->_mediaUrl);
    }
    if(this->_mediaStatus == QMediaPlayer::LoadedMedia
            || this->_mediaStatus == QMediaPlayer::BufferingMedia
            || this->_mediaStatus == QMediaPlayer::BufferedMedia
            || this->_mediaStatus == QMediaPlayer::EndOfMedia){
        if(this->_state != QMediaPlayer::PlayingState){
            this->timePosition.restart();
            this->_setState(QMediaPlayer::PlayingState);
        }
        if(!this->producer.isRunning()){
            this->producer.start();
        }else{
            this->producer.unpause();
        }
        if(!this->consumer.isRunning()){
            this->consumer.start();
        }else{
            this->consumer.unpause();
        }
    }else{
        qWarning() << "Can't play the media yet.";
    }
    qDebug() << "void FFmpegPlayerSession::play() end";
}
//====================================
void FFmpegPlayerSession::pause(){
    qDebug() << "ok1";
    if(this->producer.isRunning()){
        this->producer.pause();
    }
    qDebug() << "ok2";
    if(this->consumer.isRunning()){
        this->consumer.pause();
    }
    qDebug() << "ok3";
    this->_setState(QMediaPlayer::PausedState);
    qDebug() << "ok4";
}
//====================================
void FFmpegPlayerSession::stop(){
    this->_setState(QMediaPlayer::StoppedState);
    this->producer.stop();
    this->consumer.stop();
    this->producer.pause();
    this->producer.unpause();
    this->consumer.pause();
    this->consumer.unpause();
    this->freeMemory();
    this->reset();
    //this->positionChanged(0);
    //this->durationChanged(0);
}
//====================================
void FFmpegProducer::run(){
    qDebug() << "void FFmpegProducer::run() called";
    this->stopAsked = false;
    AVFrame *avFrame = av_frame_alloc();
    double timeBase
            = av_q2d(
                this->session->avFormatContex
                ->streams[this->session->videoStreamId]->time_base);
    double msTimeBase = timeBase * 1000;
    while(av_read_frame(this->session->avFormatContex, &this->session->_avPacket) >= 0){
        if(this->stopAsked){
            qDebug() << "stop asked";
            av_free(avFrame);
            return;
        }
        qDebug() << "Reading frame...";
        forever{
            this->session->emptyImagesMutex.lock();
            int nEmptyImages = this->session->bufferOfEmptyImages.size();
            this->session->emptyImagesMutex.unlock();
            qDebug() << "nEmptyImages: " << nEmptyImages;
            if(this->stopAsked){
                qDebug() << "stop asked";
                av_free(avFrame);
                return;
            }else if(nEmptyImages == 0){
                qDebug() << "Waiting before buffering...";
                this->msleep(100);
            }else{
                break;
            }
        }
        this->pauseMutex.lock();
        //while(this->session->_state == QMediaPlayer::PausedState){
            //qDebug() << "Sleeping during pausing...";
            //this->usleep(100);
        //}
        if(this->session->_state == QMediaPlayer::StoppedState){
            qDebug() << "Stopping...";
            av_free(avFrame);
            return;
        }
        if(this->session->_toSeek != -1){
            qDebug() << "if(this->session->_toSeek != -1){";
            this->session->consumer.pause();
            while(this->session->bufferOfImages.size() > 0){
                FFmpegBufferedImage buffer
                        = this->session->bufferOfImages.takeFirst();
                this->session->bufferOfEmptyImages.enqueue(buffer);;
            }
            this->session->consumer.unpause();
            qint64 toSeek = this->session->_toSeek;
            this->session->_toSeek = -1;
            qDebug() << "msTimeBase : " << msTimeBase;
            qint64 toSeekFFmpeg = toSeek / msTimeBase;
            qDebug() << "toSeekFFmpeg: " << toSeekFFmpeg;
            if(this->session->videoStreamId != -1 || this->session->audioStreamId != -1){
                int streamInedex = this->session->videoStreamId;
                if(streamInedex == -1){
                    streamInedex = this->session->audioStreamId;
                }
                int seekFlag = 0;
                if(toSeek < this->session->_position){
                    seekFlag = AVSEEK_FLAG_BACKWARD;
                }
                av_seek_frame(
                            this->session->avFormatContex,
                            streamInedex,
                            toSeekFFmpeg,
                            seekFlag);
                this->session->_setPosition(toSeek);
                qDebug() << "Seeking " << toSeek << " in ffmpeg unit: " << toSeekFFmpeg;
            }
        }
        //qint64 dts = this->session->timePosition.elapsed();
        //qint64 dts = this->session->_getElapsed();
        qint64 pts = this->session->_avPacket.pts * msTimeBase;
        int frameFinished = 0;
        qDebug() << "this->session->_avPacket.stream_index: " << this->session->_avPacket.stream_index;
        if(this->session->_avPacket.stream_index == this->session->videoStreamId){
            qDebug() << "if ok";
            qDebug() << "this->session->_state in thread 1: " << this->session->_state;
            avcodec_decode_video2(this->session->avVideoCodecContex,
                                  avFrame,
                                  &frameFinished,
                                  &this->session->_avPacket);
            if(frameFinished){
                qDebug() << "frame finished";
                this->session->emptyImagesMutex.lock();
                FFmpegBufferedImage imageBuffer
                        = this->session->bufferOfEmptyImages.takeFirst();
                this->session->emptyImagesMutex.unlock();
                imageBuffer.pts = pts;
                sws_scale(
                            this->session->swImageConvertContext,
                            avFrame->data,
                            avFrame->linesize,
                            0,
                            this->session->avVideoCodecContex->height,
                            imageBuffer.avFrameRGB->data,
                            imageBuffer.avFrameRGB->linesize);
                this->session->imagesMutex.lock();
                this->session->bufferOfImages.enqueue(
                            imageBuffer);
                this->session->imagesMutex.unlock();
            }
        }
        av_free_packet(&this->session->_avPacket);
        this->pauseMutex.unlock();
    }
    av_free(avFrame);
    this->session->stop();
    this->session->_setMediaStatus(QMediaPlayer::EndOfMedia);
    qDebug() << "void FFmpegProducer::run() end";
}
//====================================
void FFmpegConsumer::run(){
    this->stopAsked = false;
    qDebug() << "void FFmpegConsumer::run() called";
    while(this->session->_state != QMediaPlayer::StoppedState){
        if(this->stopAsked){
            qDebug() << "stop asked";
            return;
        }
        qDebug() << "Locking pause mutex in thread 2...";
        this->pauseMutex.lock();
        qDebug() << "Pause mutex locked in thread 2.";
        if(this->session->_state == QMediaPlayer::PlayingState
                && this->session->bufferOfImages.size() > 0){
            qDebug() << "this->session->bufferOfImages.size() > 0";
            this->session->imagesMutex.lock();
            FFmpegBufferedImage bufferedImage
                    = this->session->bufferOfImages.takeFirst();
            this->session->imagesMutex.unlock();
            this->session->currentImage
                    = QSharedPointer<QImage>(
                        new QImage(
                            bufferedImage.avFrameRGB->data[0],
                            this->session->avVideoCodecContex->width,
                            this->session->avVideoCodecContex->height,
                            bufferedImage.avFrameRGB->linesize[0],
                            QImage::Format_RGB555));
            //static int i = 0;
            //this->session->currentImage->save("/home/cedric/Images/atmp/qmediaLearner" + QString::number(i) + ".bmp");
            //i++;
            qint64 dts = this->session->_getElapsed();
            qint64 advance
                    = (bufferedImage.pts - dts)
                    / this->session->_playbackRate;
            if(advance > 1){
                qDebug() << "Sleeping " << advance << " ms...";
                this->msleep(advance);
                qDebug() << "Sleeping done";
            }
            qDebug() << "Sending new frame...";
            qDebug() << "this->session->_state: " << this->session->_state;
            this->session->currentFrameChanged(this->session->currentImage);
            qDebug() << "Frame sent.";
            this->session->_setPosition(bufferedImage.pts);
            qDebug() << "this->session->emptyImagesMutex.lock()...";
            this->session->emptyImagesMutex.lock();
            qDebug() << "this->session->emptyImagesMutex.lock().";
            this->session->bufferOfEmptyImages.enqueue(bufferedImage);
            qDebug() << "this->session->emptyImagesMutex.unlock()...";
            this->session->emptyImagesMutex.unlock();
            qDebug() << "this->session->emptyImagesMutex.unlock().";
        }else{
            this->msleep(100);
        }
        qDebug() << "unlocking pause mutex in thread 2...";
        this->pauseMutex.unlock();
        qDebug() << "Pause mutex unlocked in thread 2.";
    }
    qDebug() << "void FFmpegConsumer::run() end";
}
//====================================
qint64 FFmpegPlayerSession::duration() const{
    qint64 duration = 0;
    if(this->avFormatContex != NULL){
        duration = this->avFormatContex->duration;
        qDebug() << "DURATION: " << this->avFormatContex->duration;
        duration /= AV_TIME_BASE / 1000;
    }
    return duration;
}
//====================================
bool FFmpegPlayerSession::isAudioAvailable() const{
    bool available = this->audioStreamId != -1;
    return available;
}
//====================================
bool FFmpegPlayerSession::isVideoAvailable() const{
    bool available = this->videoStreamId != -1;
    return available;
}
//====================================
bool FFmpegPlayerSession::isSeekable() const{
    bool seekable
            = this->videoStreamId != -1;
    return seekable;
}
//====================================
qint64 FFmpegPlayerSession::position() const{
    return this->_position;
}
//====================================
void FFmpegPlayerSession::setPosition(
        qint64 pos){
    this->_toSeek = pos;
}
//====================================
void FFmpegPlayerSession::_setPosition(qint64 pos){
    this->_position = pos;
    this->timePosition.restart();
    this->positionChanged(this->_position);
}
//====================================
qint64 FFmpegPlayerSession::_getElapsed(){
    qint64 elapsed = this->timePosition.elapsed();
    elapsed += this->_position;
    return elapsed;
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
    qDebug() << "void FFmpegPlayerSession::_setState(...) called";
    //QMutexLocker mutexLocker(&this->stateMutex);
    this->_state = state;
    this->stateChanged(this->_state);
    qDebug() << "void FFmpegPlayerSession::_setState(...) end";
}
//====================================
void FFmpegPlayerSession::_setMediaStatus(
        QMediaPlayer::MediaStatus mediaStatus){
    this->_mediaStatus = mediaStatus;
    this->mediaStatusChanged(this->_mediaStatus);
}
//====================================

