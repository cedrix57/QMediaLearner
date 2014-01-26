#ifndef FFMPEGVIDEORENDRECONTROL_H
#define FFMPEGVIDEORENDRECONTROL_H

#include <QVideoRendererControl>
#include <QVideoFrame>
#include <QSharedPointer>
#include <QImage>
#include <QMutex>

//class FFmpegPlayerSession;

class FFmpegVideoRenderControl
        : public QVideoRendererControl{
    Q_OBJECT
public:
    explicit FFmpegVideoRenderControl(
            QObject *parent = 0);
    //void setFFmpegPlayerSession(
            //FFmpegPlayerSession *session);

    virtual QAbstractVideoSurface *surface() const;
    virtual void setSurface(QAbstractVideoSurface *surface);

public Q_SLOTS:
    void setDisplayedFrame(QSharedPointer<QImage> image);

protected:
    //FFmpegPlayerSession *session;
    QAbstractVideoSurface *_surface;
    QVideoFrame currentFrame;
};

#endif // FFMPEGVIDEORENDRECONTROL_H
