#ifndef FFMPEGPLAYERSERVICE_H
#define FFMPEGPLAYERSERVICE_H

#include <QMediaService>

QT_BEGIN_NAMESPACE

class FFmpegMediaPlayerControl;
class FFmpegVideoWidgetControl;
class FFmpegVideoRenderControl;
class FFmpegMetaDataReaderControl;
class FFmpegMediaNetworkAccessControl;
class FFmpegMediaAvailabilityControl;

class FFmpegPlayerService : public QMediaService{
    Q_OBJECT
public:
    explicit FFmpegPlayerService(QObject *parent = 0);
    
    QMediaControl *requestControl(const char *name);
    void releaseControl(QMediaControl *control);
    
protected:
    FFmpegMediaPlayerControl *mediaPlayerControl;
    FFmpegVideoWidgetControl *videoWidgetControl;
    FFmpegVideoRenderControl *videoRenderControl;
    FFmpegMetaDataReaderControl *metaDataReaderControl;
    FFmpegMediaNetworkAccessControl *mediaNetworkAccessControl;
    FFmpegMediaAvailabilityControl *mediaAvailabilityControl;
    void createControls();
    void connectSlots();
    void disconnectFrameSlots();
};

QT_END_NAMESPACE

#endif // FFMPEGPLAYERSERVICE_H
