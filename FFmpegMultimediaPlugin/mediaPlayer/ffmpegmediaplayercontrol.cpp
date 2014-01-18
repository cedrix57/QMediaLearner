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
    this->ff_frame = NULL;
    static bool isRegisteredFFmpegFormats = false;
    if(!isRegisteredFFmpegFormats){
        av_register_all();
        isRegisteredFFmpegFormats = true;
    }
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
void FFmpegMediaPlayerControl::_setMediaStatus(
        QMediaPlayer::MediaStatus status){
    this->_mediaStatus = status;
    this->mediaStatusChanged(this->_mediaStatus);
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
    this->_setMediaStatus(QMediaPlayer::LoadingMedia);
    this->_device = device;
    if(device != NULL){
        qWarning() << "FFmpeg: Device reading not implemented. ";
    }else{
        QUrl url = mediaContent.canonicalUrl();
        QString filePath = url.url(); //QUrl::EncodeUnicode);
        QByteArray filePathBytes = filePath.toUtf8();
        char *filePathChar = filePathBytes.data();
        int ret = avformat_open_input(
                    &this->ff_formatContex,
                    filePathChar,
                    NULL,
                    NULL);
        if(ret != 0){
            qWarning() << "FFmpeg: Couldn't open " << filePath << ". return code: " << ret;
            char ffmpegError[500];
            av_strerror(ret, ffmpegError, 500);
            qWarning() << ffmpegError;
        }else{
            ret = av_find_stream_info(this->ff_formatContex);
            if(ret < 0){
                qWarning() << "FFmpeg: Couldn't find stream info of " << filePath << ". return code: " << ret;
            }else{
                av_dump_format(this->ff_formatContex, 0, filePathChar, 0);
                int videoStream = -1;
                for(int i=0; i<this->ff_formatContex->nb_streams; i++){
                    if(this->ff_formatContex->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
                        videoStream=i;
                        break;
                    }
                }
                if(videoStream != -1){ //not audio only
                    AVCodecContext *codecContex
                            = this->ff_formatContex->streams[videoStream]->codec;
                    AVCodec *codec = avcodec_find_decoder(codecContex->codec_id);
                    ret = avcodec_open2(codecContex, codec, NULL);
                    if(ret<0){
                        qWarning() << "FFmpeg: Couldn't open the requiered codec" << codecContex->codec_name << " of " << filePath;
                    }else{
                        this->ff_frame = avcodec_alloc_frame();
                        this->ff_frameRGB = avcodec_alloc_frame();
                        int numBytes
                                = avpicture_get_size(
                                    PIX_FMT_RGB24,
                                    codecContex->width,
                                    codecContex->height);
                        quint8 *buffer
                                = (quint8 *) av_malloc(
                                    numBytes*sizeof(quint8));
                        avpicture_fill(
                                    (AVPicture *)this->ff_frameRGB,
                                    buffer,
                                    PIX_FMT_RGB24,
                                    codecContex->width,
                                    codecContex->height);
                        SwsContext *img_convert_ctx
                                = sws_getContext(
                                    codecContex->width,
                                    codecContex->height,
                                    codecContex->pix_fmt,
                                    codecContex->width,
                                    codecContex->height,
                                    PIX_FMT_RGB555,
                                    SWS_FAST_BILINEAR,
                                    NULL,
                                    NULL,
                                    NULL);
                        if(img_convert_ctx != NULL){
                            int frameFinished;
                            AVPacket packet;

                            int i=0;
                            while(av_read_frame(this->ff_formatContex, &packet)>=0){
                                if(packet.stream_index==videoStream){
                                // Decode video frame
                                    avcodec_decode_video2(codecContex,
                                                          this->ff_frame,
                                                          &frameFinished,
                                                          &packet);
                                    // Did we get a video frame?
                                    if(frameFinished){
                                        // Convert the image from its native format to RGB
                                        sws_scale(
                                                    img_convert_ctx,
                                                    this->ff_frame->data,
                                                    this->ff_frame->linesize,
                                                    0,
                                                    codecContex->height,
                                                    this->ff_frameRGB->data,
                                                    this->ff_frameRGB->linesize);
                                        this->tempImage
                                                = QImage(
                                                    this->ff_frameRGB->data[0],
                                                    codecContex->width,
                                                    codecContex->height,
                                                    this->ff_frameRGB->linesize[0],
                                                    QImage::Format_RGB555);
                                        this->tempImage.save(
                                                    "/home/cedric/Images/qmedia.bmp");
                                        this->currentFrame
                                                = QVideoFrame(
                                                    this->tempImage);
                                        if(i==30){
                                            break;
                                        }
                                    }
                                }
                                av_free_packet(&packet);
                                i++;
                            }
                        }
                    }
                }
                this->_setMediaStatus(QMediaPlayer::LoadedMedia);
                this->mediaContent = mediaContent;
                this->mediaChanged(this->mediaContent);
            }
        }
    }
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
                this->tempImage);
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
    qDebug() << "FFmpeg: Temp play...";
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
