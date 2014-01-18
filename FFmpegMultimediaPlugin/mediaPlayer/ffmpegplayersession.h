#ifndef FFMPEGPLAYERSESSION_H
#define FFMPEGPLAYERSESSION_H

#include <QThread>
#include <QMediaPlayer>
#include <QMediaTimeRange>
#include <QImage>
#include <QSharedPointer>
#include <QMutex>
#include <QDateTime>


extern "C"
{
#include <avcodec.h>
#include <avformat.h>
#include <swscale.h>
}

class FFmpegPlayerSession : public QThread{
    Q_OBJECT
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
    void audioAvailableChanged(bool audio);
    void availablePlaybackRangesChanged(const QMediaTimeRange & ranges);
    void bufferStatusChanged(int progress);
    void durationChanged(qint64 duration);
    void error(int error, const QString & errorString);
    void mediaChanged(const QMediaContent & content);
    void mediaStatusChanged(QMediaPlayer::MediaStatus status);
    void mutedChanged(bool mute);
    void playbackRateChanged(qreal rate);
    void positionChanged(qint64 position);
    void seekableChanged(bool seekable);
    void stateChanged(QMediaPlayer::State state);
    void videoAvailableChanged(bool video);
    void volumeChanged(int volume);

protected:
    virtual void run();
    void reset();
    void freeMemory();
    void _setState(QMediaPlayer::State state);
    void _setMediaStatus(QMediaPlayer::MediaStatus mediaStatus);
    void _setPosition(qint64 pos);
    AVFormatContext *avFormatContex;
    SwsContext *swImageConvertContext;
    AVCodecContext *avVideoCodecContex;
    AVCodecContext *avAudioCodecContex;
    AVPacket _avPacket;
    AVFrame *avFrameRGB;
    int videoStreamId;
    int audioStreamId;
    qint64 _toSeek;
    quint8 *_rbgFrameBuffer;

    QSharedPointer<QImage> currentImage;
    QMutex stateMutex;
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
