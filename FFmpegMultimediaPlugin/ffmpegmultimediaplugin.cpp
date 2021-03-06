#include "ffmpegmultimediaplugin.h"
#include <QDebug>

QT_BEGIN_NAMESPACE

//====================================
FFmpegMultimediaPlugin::FFmpegMultimediaPlugin(){
}
//====================================
FFmpegMultimediaPlugin::~FFmpegMultimediaPlugin(){
}
//====================================
QMediaService* FFmpegMultimediaPlugin::create(
        QString const& key){
    QMediaService* mediaService = NULL;
    if (key == QLatin1String(Q_MEDIASERVICE_MEDIAPLAYER)){
        //mediaService = new FFmpegMediaPlayerControl;
    }else{
        qWarning() << "FFmpeg service plugin: unsupported key:" << key;
    }
    return mediaService; //TODO
}
//====================================
void FFmpegMultimediaPlugin::release(
        QMediaService *service){
    delete service;
}
//====================================
QMultimedia::SupportEstimate
FFmpegMultimediaPlugin::hasSupport(
        const QString &mimeType,
        const QStringList& codecs) const{
    return QMultimedia::MaybeSupported;
}
//====================================
QStringList
FFmpegMultimediaPlugin::supportedMimeTypes() const{
    return QStringList();
}
//====================================
QMediaServiceProviderHint::Features
FFmpegMultimediaPlugin::supportedFeatures(
        const QByteArray &service) const{
    if(service == Q_MEDIASERVICE_MEDIAPLAYER){
        return QMediaServiceProviderHint::StreamPlayback;
    }else{
        return QMediaServiceProviderHint::Feature();
    }
}
//====================================

QT_END_NAMESPACE

