#ifndef FFMPEGMULTIMEDIAPLUGIN_H
#define FFMPEGMULTIMEDIAPLUGIN_H

//#include "ffmpegmultimediaplugin_global.h"
#include <QMediaServiceProviderPlugin>

QT_USE_NAMESPACE

class FFmpegMultimediaPlugin
        : public QMediaServiceProviderPlugin
        , public QMediaServiceSupportedFormatsInterface
        , public QMediaServiceFeaturesInterface
{
    Q_OBJECT
    Q_INTERFACES(QMediaServiceSupportedFormatsInterface)
    Q_INTERFACES(QMediaServiceFeaturesInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.qt.mediaserviceproviderfactory/5.0" FILE "ffmpeg.json")

public:
    FFmpegMultimediaPlugin();
    virtual ~FFmpegMultimediaPlugin();
    virtual QMediaService* create(QString const& key);
    virtual void release(QMediaService *service);

    virtual QMediaServiceProviderHint::Features supportedFeatures(
            const QByteArray &service) const;

    virtual QMultimedia::SupportEstimate hasSupport(
            const QString &mimeType,
            const QStringList& codecs) const;
    virtual QStringList supportedMimeTypes() const;
};

QT_END_NAMESPACE

#endif // FFMPEGMULTIMEDIAPLUGIN_H
