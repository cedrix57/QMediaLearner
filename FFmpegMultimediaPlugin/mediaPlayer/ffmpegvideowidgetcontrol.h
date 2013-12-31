#ifndef FFMPEGVIDEOWIDGETCONTROL_H
#define FFMPEGVIDEOWIDGETCONTROL_H

#include <QVideoWidgetControl>
#include <QVideoFrame>

class VideoWidget : public QWidget{
    Q_OBJECT

public:
    VideoWidget(QWidget *parent = NULL);

public slots:
    void setDisplayedFrame(QVideoFrame frame);

protected:
    QVideoFrame currentFrame;
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
};

class FFmpegVideoWidgetControl
        : public QVideoWidgetControl{
    Q_OBJECT
public:
    explicit FFmpegVideoWidgetControl(
            QObject *parent = 0);
    virtual QWidget *videoWidget() = 0;

    virtual Qt::AspectRatioMode aspectRatioMode() const = 0;
    virtual void setAspectRatioMode(Qt::AspectRatioMode mode) = 0;

    virtual bool isFullScreen() const = 0;
    virtual void setFullScreen(bool fullScreen) = 0;

    virtual int brightness() const = 0;
    virtual void setBrightness(int brightness) = 0;

    virtual int contrast() const = 0;
    virtual void setContrast(int contrast) = 0;

    virtual int hue() const = 0;
    virtual void setHue(int hue) = 0;

    virtual int saturation() const = 0;
    virtual void setSaturation(int saturation) = 0;

public slots:
    void setDisplayedFrame(QVideoFrame frame);

protected:
    VideoWidget _videoWidget;
    Qt::AspectRatioMode _aspectRatioMode;
    int _brightness;
    int _contrast;
    int _hue;
    int _saturation;

};

#endif // FFMPEGVIDEOWIDGETCONTROL_H
