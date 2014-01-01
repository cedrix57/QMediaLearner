#ifndef FFMPEGMEDIAAVAILABILITYCONTROL_H
#define FFMPEGMEDIAAVAILABILITYCONTROL_H

#include <QMediaAvailabilityControl>

class FFmpegMediaAvailabilityControl
        : public QMediaAvailabilityControl{
    Q_OBJECT
public:
    explicit FFmpegMediaAvailabilityControl(
            QObject *parent = 0);
    virtual QMultimedia::AvailabilityStatus availability() const;

protected:
    QMultimedia::AvailabilityStatus status;
    
};

#endif // FFMPEGMEDIAAVAILABILITYCONTROL_H
