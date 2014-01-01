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
            QObject *parent = NULL);
    virtual QWidget *videoWidget();

    virtual Qt::AspectRatioMode aspectRatioMode() const;
    virtual void setAspectRatioMode(Qt::AspectRatioMode mode);

    virtual bool isFullScreen() const;
    virtual void setFullScreen(bool fullScreen);

    virtual int brightness() const;
    virtual void setBrightness(int brightness);

    virtual int contrast() const;
    virtual void setContrast(int contrast);

    virtual int hue() const;
    virtual void setHue(int hue);

    virtual int saturation() const;
    virtual void setSaturation(int saturation);

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
