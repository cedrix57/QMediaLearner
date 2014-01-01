#include "ffmpegmetadatareadercontrol.h"

//====================================
FFmpegMetaDataReaderControl::FFmpegMetaDataReaderControl(QObject *parent) :
    QMetaDataReaderControl(parent){
}
//====================================
bool FFmpegMetaDataReaderControl::isMetaDataAvailable() const{
    bool available
            = this->metaDatas.size() > 0;
    return available;
}
//===================================
QVariant FFmpegMetaDataReaderControl::metaData(
        const QString &key) const{
    QVariant data = this->metaDatas[key];
    return data;
}
//====================================
QStringList FFmpegMetaDataReaderControl::availableMetaData() const{
    QStringList metaDataKeys
            = this->metaDatas.keys();
    return metaDataKeys;
}
//====================================
void FFmpegMetaDataReaderControl::clearMetaData(){
    this->metaDatas.clear();
    this->metaDataAvailableChanged(false);
}
//====================================
void FFmpegMetaDataReaderControl::addMetaData(
        QString key,
        QVariant value){
    int nVals = this->metaDatas.size();
    this->metaDatas[key] = value;
    if(nVals == 0){
        this->metaDataAvailableChanged(true);
    }
    this->metaDataChanged();
}
//====================================
void FFmpegMetaDataReaderControl::addMetaData(
        QMap<QString, QVariant> &metaDatas){
    foreach(QString key, metaDatas.keys()){
        QVariant value = metaDatas[key];
        this->metaDatas[key] = value;
    }
    this->metaDataAvailableChanged(true);
    this->metaDataChanged();
}
//====================================
void FFmpegMetaDataReaderControl::setMetaData(
        QMap<QString, QVariant> &metaDatas){
    this->clearMetaData();
    this->addMetaData(metaDatas);
}
//====================================
