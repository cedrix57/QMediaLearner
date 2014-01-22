#ifndef FFMPEGMEDIAPLAYERCONTROL_H
#define FFMPEGMEDIAPLAYERCONTROL_H

#include <QMediaPlayerControl>
#include <QVideoFrame>

extern "C"
{
#include <avcodec.h>
#include <avformat.h>
#include <swscale.h>
}

class FFmpegPlayerSession;

class FFmpegMediaPlayerControl
        : public QMediaPlayerControl{
    Q_OBJECT
public:
    virtual ~FFmpegMediaPlayerControl();
    explicit FFmpegMediaPlayerControl(
            QObject *parent = NULL);
    void setFFmpegPlayerSession(
            FFmpegPlayerSession *session);
    
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


protected:
    FFmpegPlayerSession *session;
    QIODevice *_device;
    QMediaContent _mediaContent;

};

#endif // FFMPEGMEDIAPLAYERCONTROL_H
