#include "PluginSequenceExtractor.h"

namespace MediaLearner{

//====================================
//====================================
void AudioBuffer::init(QAudioBuffer &qtbuffer){
    QAudioFormat audioFormat = qtbuffer.format();
    this->hzFreq = audioFormat.sampleRate();
    this->durationInMs = qtbuffer.duration();
    QAudioFormat::SampleType sampleType
            = audioFormat.sampleType();
    int frameCount = qtbuffer.frameCount();
    int nChannels = audioFormat.channelCount();
    int bytesPerFrame = audioFormat.bytesPerFrame();
    int bytesPerValue = bytesPerFrame/nChannels;
    void *firstData = qtbuffer.data();
    this->bufferSize = frameCount;
    this->buffer = QSharedPointer<SharedBuffer>(
                new SharedBuffer);
    this->buffer->buffer = new int[this->bufferSize];
    this->mean = 0;
    int meanOfSquare = 0;
    for(int i=0; i<frameCount; i++){
        int currentValue = 0;
        for(int j=0; j<nChannels; j++){
            int currentPos
                    = i*bytesPerFrame
                    + j*bytesPerValue;
            void *valPos = (void *)((quint8 *)firstData + currentPos);
            if(sampleType == QAudioFormat::SignedInt){
                if(bytesPerValue == 1){
                    quint8 val = *((quint8*)valPos);
                    currentValue += val;
                }else if(bytesPerValue == 2){
                    quint16 val = *((quint16*)valPos);
                    currentValue += val;
                }else if(bytesPerValue == 4){
                    quint32 val = *((quint32*)valPos);
                    currentValue += val;
                }else if(bytesPerValue == 8){
                    quint64 val = *((quint64*)valPos);
                    currentValue += val;
                }
            }else if(sampleType == QAudioFormat::UnSignedInt){
                if(bytesPerValue == 1){
                    qint8 val = *((qint8*)valPos);
                    currentValue += val;
                }else if(bytesPerValue == 2){
                    qint16 val = *((qint16*)valPos);
                    currentValue += val;
                }else if(bytesPerValue == 4){
                    qint32 val = *((qint32*)valPos);
                    currentValue += val;
                }else if(bytesPerValue == 8){
                    qint64 val = *((qint64*)valPos);
                    currentValue += val;
                }
            }else if(sampleType == QAudioFormat::Float){
                qreal val = *((qreal*)valPos);
                currentValue += val;
            }
        }
        currentValue /= nChannels;
        this->buffer->buffer[i] = currentValue;
        this->mean += currentValue;
        meanOfSquare += currentValue*currentValue;
    }
    this->mean /= frameCount;
    meanOfSquare /= frameCount;
    this->var = meanOfSquare - this->mean*this->mean;
}
//====================================
//====================================
QMap<QString, PluginSequenceExtractor*>
        PluginSequenceExtractor::getExtractors(){
    QMap<QString, PluginSequenceExtractor*>
            extractorsMap;
    foreach(
            PluginSequenceExtractor*
            extractor,
            PluginSequenceExtractor::extractors){
        QString name = extractor->getName();
        extractorsMap[name] = extractor;
    }
    return extractorsMap;
}
//====================================
PluginSequenceExtractor::PluginSequenceExtractor(
        QObject *parent) :
    QObject(parent){
    QList<Parameter> parameters = this->getParameters();
    foreach(Parameter parameter, parameters){
        this->parameterValues[parameter.name]
                = parameter.defaultValue;
    }
}
//====================================
QList<Parameter> PluginSequenceExtractor::getParameters(){
    QList<Parameter> parameters;
    return parameters;
}
//====================================
void PluginSequenceExtractor::setParameterValues(
        QMap<QString, QVariant> parameterValues){
    foreach(QString key, parameterValues.keys()){
        QVariant value = parameterValues[key];
        this->parameterValues[key] = value;
    }
}
//====================================
void PluginSequenceExtractor::setParameterValue(
        QString key,
        QVariant value){
    this->parameterValues[key] = value;
}
//====================================

}
