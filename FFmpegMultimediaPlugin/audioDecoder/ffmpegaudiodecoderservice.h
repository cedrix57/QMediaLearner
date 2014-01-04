#ifndef FFMPEGAUDIODECODERSERVICE_H
#define FFMPEGAUDIODECODERSERVICE_H

#include <QMediaService>

class FFmpegAudioDecoderControl;

class FFmpegAudioDecoderService
        : public QMediaService{
    Q_OBJECT
public:
    explicit FFmpegAudioDecoderService(
            QObject *parent = 0);
    
    virtual QMediaControl *requestControl(const char *name);
    virtual void releaseControl(QMediaControl *control);

protected:
    FFmpegAudioDecoderControl *audioDecoderControl;
    void createControls();
    void connectSlots();

};

#endif // FFMPEGAUDIODECODERSERVICE_H
