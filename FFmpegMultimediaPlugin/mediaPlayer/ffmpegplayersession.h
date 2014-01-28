#ifndef FFMPEGPLAYERSESSION_H
#define FFMPEGPLAYERSESSION_H

#include <QThread>
#include <QMediaPlayer>
#include <QMediaTimeRange>
#include <QImage>
#include <QSharedPointer>
#include <QMutex>
#include <QTime>
#include <QQueue>
#include <QTimer>
#include <QDebug>


extern "C"
{
#include <avcodec.h>
#include <avformat.h>
#include <swscale.h>
}

struct FFmpegBufferedImage{
    AVFrame *avFrameRGB;
    quint8 *avFrameBuffer;
    qint64 pts;
};

class FFmpegPlayerSession;

class FFmpegProducer: public QThread{
    Q_OBJECT
    public:
    FFmpegProducer(QObject *parent = NULL)
        : QThread(parent){
        this->session = NULL;
    }
    void init(FFmpegPlayerSession *playerSession){
        this->session = playerSession;
    }
    void pause(){
        if(!this->pauseMutex.tryLock(500)){
            qWarning() << "FFmpegConsumer couldn't pause.";
        }
    }
    void unpause(){
        this->pauseMutex.unlock();
    }

    protected:
    virtual void run();
    FFmpegPlayerSession *session;
    QMutex pauseMutex;
};

class FFmpegConsumer: public QThread{
    Q_OBJECT
    public:
    FFmpegConsumer(QObject *parent = NULL)
        : QThread(parent){
        this->session = NULL;
    }
    void init(FFmpegPlayerSession *playerSession){
        this->session = playerSession;
    }
    void pause(){
        if(!this->pauseMutex.tryLock(500)){
            qWarning() << "FFmpegConsumer couldn't pause.";
        }
    }

    void unpause(){
        qDebug() << "void FFmpegConsumer::unpause() called";
        this->pauseMutex.unlock();
        qDebug() << "void FFmpegConsumer::unpause() end";
    }

    protected:
    virtual void run();
    FFmpegPlayerSession *session;
    QMutex pauseMutex;
};


class FFmpegPlayerSession : public QObject{
    Q_OBJECT

friend FFmpegProducer;
friend FFmpegConsumer;

public:
    explicit FFmpegPlayerSession(
            QObject *parent = NULL);
    ~FFmpegPlayerSession();
    QSharedPointer<QImage> getCurrentImage();


    QMediaPlayer::State state() const;
    QMediaPlayer::MediaStatus mediaStatus() const;

    qint64 duration() const;

    qint64 position() const;
    void setPosition(qint64 pos);

    int volume() const;
    void setVolume(int volume);

    bool isMuted() const;
    void setMuted(bool muted);

    int bufferStatus() const; //TODO

    bool isAudioAvailable() const;
    bool isVideoAvailable() const;
    //void setVideoOutput(QObject *output);

    bool isSeekable() const; //TODO
    QMediaTimeRange availablePlaybackRanges() const;  //TODO

    qreal playbackRate() const;
    void setPlaybackRate(qreal rate);

    QString mediaUrl() const;
    void setMediaUrl(QString mediaUrl);

    //QMediaPlayerResourceSetInterface* resources() const;

//public Q_SLOTS:

    void play();
    void pause();
    void stop();

signals:
    void audioAvailableChanged(bool audio); //TODO send this signal
    void availablePlaybackRangesChanged(const QMediaTimeRange & ranges);
    void bufferStatusChanged(int progress);
    void durationChanged(qint64 duration); //TODO
    void error(int error, const QString & errorString); //TODO
    void mediaChanged(const QMediaContent & content);
    void mediaStatusChanged(QMediaPlayer::MediaStatus status);
    void mutedChanged(bool mute);
    void playbackRateChanged(qreal rate);
    void positionChanged(qint64 position);
    void seekableChanged(bool seekable);
    void stateChanged(QMediaPlayer::State state);
    void videoAvailableChanged(bool video);
    void volumeChanged(int volume);

    void currentFrameChanged(QSharedPointer<QImage>);


public:
    static const int bufferSize = 5;
protected:
    //virtual void run();
    void reset();
    void freeMemory();
    void _setState(QMediaPlayer::State state);
    void _setMediaStatus(QMediaPlayer::MediaStatus mediaStatus);
    void _setPosition(qint64 pos);
    qint64 _getElapsed();
    AVFormatContext *avFormatContex;
    SwsContext *swImageConvertContext;
    AVCodecContext *avVideoCodecContex;
    AVCodecContext *avAudioCodecContex;
    AVPacket _avPacket;
    //FFmpegBufferedImage imageBuffer[bufferSize];
    int videoStreamId;
    int audioStreamId;
    qint64 _toSeek;
    QQueue<FFmpegBufferedImage> bufferOfImages;
    QQueue<FFmpegBufferedImage> bufferOfEmptyImages;
    QMutex imagesMutex;
    QMutex emptyImagesMutex;
    FFmpegConsumer consumer;
    FFmpegProducer producer;
    QTime timePosition;

    QSharedPointer<QImage> currentImage;
    QMediaPlayer::State _state;
    QMediaPlayer::MediaStatus _mediaStatus;
    QIODevice *_device;
    int _volume;
    qreal _playbackRate;
    bool _muted;
    QString _mediaUrl;
    qint64 _position;

};


#endif // FFMPEGPLAYERSESSION_H
