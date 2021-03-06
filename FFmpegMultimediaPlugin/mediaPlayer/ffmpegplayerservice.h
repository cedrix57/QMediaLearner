#ifndef FFMPEGPLAYERSERVICE_H
#define FFMPEGPLAYERSERVICE_H

#include <QMediaService>

QT_BEGIN_NAMESPACE

class FFmpegMediaPlayerControl;

class FFmpegPlayerService : public QMediaService{
    Q_OBJECT
public:
    explicit FFmpegPlayerService(QObject *parent = 0);
    
    QMediaControl *requestControl(const char *name);
    void releaseControl(QMediaControl *control);
    
protected:
    FFmpegMediaPlayerControl *mediaPlayerControl;
};

QT_END_NAMESPACE

#endif // FFMPEGPLAYERSERVICE_H
