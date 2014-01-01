#include "ffmpegmedianetworkaccesscontrol.h"

//====================================
FFmpegMediaNetworkAccessControl::FFmpegMediaNetworkAccessControl(
        QObject *parent) :
    QMediaNetworkAccessControl(parent){
}
//====================================
void FFmpegMediaNetworkAccessControl::setConfigurations(
        const QList<QNetworkConfiguration> &configuration){
    this->configurations = configuration;
    if(this->configurations.size() > 0){
        this->_currentConfiguration
                = this->configurations.first();
        this->configurationChanged(
                    this->_currentConfiguration);
    }
}
//====================================
QNetworkConfiguration
FFmpegMediaNetworkAccessControl::currentConfiguration() const{
    return this->_currentConfiguration;
}
//====================================

