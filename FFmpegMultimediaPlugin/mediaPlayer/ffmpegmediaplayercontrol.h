#ifndef FFMPEGMEDIAPLAYERCONTROL_H
#define FFMPEGMEDIAPLAYERCONTROL_H

#include <QMediaPlayerControl>
#include <QVideoFrame>

extern "C"
{
#include <avcodec.h>
#include <avformat.h>
}

class FFmpegMediaPlayerControl
        : public QMediaPlayerControl{
    Q_OBJECT
public:
    virtual ~FFmpegMediaPlayerControl();
    explicit FFmpegMediaPlayerControl(
            QObject *parent = NULL);
    
    QMediaPlayer::State state() const;
    QMediaPlayer::MediaStatus mediaStatus() const;

    qint64 duration() const;

    qint64 position() const;
    void setPosition(qint64 pos);


    int volume() const;
    void setVolume(int _volume);

    bool isMuted() const;
    void setMuted(bool muted);

    int bufferStatus() const;

    bool isAudioAvailable() const;
    bool isVideoAvailable() const;
    //void setVideoOutput(QObject *output);

    bool isSeekable() const;
    QMediaTimeRange availablePlaybackRanges() const;

    qreal playbackRate() const;
    void setPlaybackRate(qreal rate);

    QMediaContent media() const;
    const QIODevice *mediaStream() const;
    void setMedia(const QMediaContent&, QIODevice *);

    //QMediaPlayerResourceSetInterface* resources() const;

//public Q_SLOTS:

    void play();
    void pause();
    void stop();

signals:
    void frameAvailable(QVideoFrame frame);

protected:
    AVFormatContext *ff_formatContex;
    QMediaPlayer::State _state;
    QMediaPlayer::MediaStatus _mediaStatus;
    QIODevice *_device;
    int _volume;
    qreal _rate;
    bool _muted;
    QMediaContent mediaContent;
    qint64 _position;
    QVideoFrame currentFrame;
    qint64 _duration;

protected Q_SLOTS:
    void tempPlay();
};

#endif // FFMPEGMEDIAPLAYERCONTROL_H
