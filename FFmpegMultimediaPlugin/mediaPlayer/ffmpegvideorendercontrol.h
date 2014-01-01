#ifndef FFMPEGVIDEORENDRECONTROL_H
#define FFMPEGVIDEORENDRECONTROL_H

#include <QVideoRendererControl>
#include <QVideoFrame>

class FFmpegVideoRenderControl
        : public QVideoRendererControl{
    Q_OBJECT
public:
    explicit FFmpegVideoRenderControl(
            QObject *parent = 0);
    
    virtual QAbstractVideoSurface *surface() const;
    virtual void setSurface(QAbstractVideoSurface *surface);

    
public slots:
    void setDisplayedFrame(QVideoFrame frame);

protected:
    QAbstractVideoSurface *_surface;
    QVideoFrame currentFrame;

};

#endif // FFMPEGVIDEORENDRECONTROL_H
