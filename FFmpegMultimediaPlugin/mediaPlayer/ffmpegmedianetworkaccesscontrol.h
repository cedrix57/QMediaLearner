#ifndef FFMPEGMEDIANETWORKACCESSCONTROL_H
#define FFMPEGMEDIANETWORKACCESSCONTROL_H

#include <QMediaNetworkAccessControl>

class FFmpegMediaNetworkAccessControl
        : public QMediaNetworkAccessControl{
    Q_OBJECT
public:
    explicit FFmpegMediaNetworkAccessControl(QObject *parent = 0);
    virtual void setConfigurations(
            const QList<QNetworkConfiguration> &configuration);
    virtual QNetworkConfiguration currentConfiguration() const;

protected:
    QList<QNetworkConfiguration> configurations;
    QNetworkConfiguration _currentConfiguration;
    
};

#endif // FFMPEGMEDIANETWORKACCESSCONTROL_H
