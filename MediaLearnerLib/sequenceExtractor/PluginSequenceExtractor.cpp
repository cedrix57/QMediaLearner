#include "PluginSequenceExtractor.h"
#include <qmath.h>

namespace MediaLearner{

//====================================
//====================================
int PluginSequenceExtractor::getValue(
        QAudioBuffer &audioBuffer,
        QAudioFormat::SampleType &sampleType,
        int framePosition,
        int nChannels,
        int bytesPerFrame,
        int bytesPerValue){
    void *firstData = audioBuffer.data();
    int currentValue = 0;
    for(int j=0; j<nChannels; j++){
        int currentPos
                = framePosition*bytesPerFrame
                + j*bytesPerValue;
        void *valPos = (void *)(((quint8 *)firstData) + currentPos);
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
    return currentValue;
}
void AudioBuffer::init(QAudioBuffer &qtbuffer){
    QAudioFormat audioFormat = qtbuffer.format();
    this->hzFreq = audioFormat.sampleRate();
    this->durationInMs = qtbuffer.duration()*1000;
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
    const int tempReducFactor = 500;
    qint64 meanOfSquare = 0;
    for(int i=0; i<frameCount; i++){
        int currentValue = 0;
        for(int j=0; j<nChannels; j++){
            int currentPos
                    = i*bytesPerFrame
                    + j*bytesPerValue;
            void *valPos = (void *)(((quint8 *)firstData) + currentPos);
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
        currentValue /= tempReducFactor;
        this->buffer->buffer[i] = currentValue;
        this->mean += currentValue;
        meanOfSquare += currentValue*currentValue;
        if(meanOfSquare < 0){
            Q_ASSERT(false);
        }
    }
    this->mean /= frameCount;
    meanOfSquare /= frameCount;
    qint64 squaredMean =
            this->mean
            * this->mean;
    this->var = meanOfSquare
            - squaredMean;
    this->var *= tempReducFactor * tempReducFactor;
    this->mean *= tempReducFactor;
    this->sd = qSqrt(this->var);
}
//====================================
QList<PluginSequenceExtractor*>
        PluginSequenceExtractor::extractors;
//====================================
//====================================
PluginSequenceExtractor::PluginSequenceExtractor(
        QObject *parent) :
    QObject(parent){
    this->_isAnalysing = false;
    PluginSequenceExtractor::extractors
            << this;
    QList<Parameter> parameters = this->getParameters();
    foreach(Parameter parameter, parameters){
        this->parameterValues[parameter.name]
                = parameter.defaultValue;
    }
    this->extractedSequences
            = QSharedPointer<QList<Sequence> >
                (new QList<Sequence>);
    this->reset();
}
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
void PluginSequenceExtractor::reset(){
    this->extractedSequences->clear();
    this->_currentTimeStampInMs = 0;
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
void PluginSequenceExtractor::onBufferReady(
        QAudioDecoder *audioDecoder){
    this->_mutexAnalyse.lock();
    this->_isAnalysing = true;
    QAudioBuffer audioBuffer
            = audioDecoder->read();
    this->analyseBuffer(audioBuffer);
    qint64 duration
            = audioBuffer.duration() / 1000;
    this->_currentTimeStampInMs
            += duration;
    //qint64 totalDuration = audioDecoder->duration();
    //int percentage
            //= (int)this->_currentTimeStampInMs * 1.0
            /// totalDuration * 100;
    //qDebug() << percentage << "%...";
    this->_isAnalysing = false;
    this->_mutexAnalyse.unlock();
}
//====================================
bool PluginSequenceExtractor::isAnalysing(){
    return this->_isAnalysing;
}
//====================================
QSharedPointer<QList<Sequence> >
        PluginSequenceExtractor::getExtractedSequences(){
    return this->extractedSequences;
}
//====================================
qint64 PluginSequenceExtractor::getCurrentTimeStampInMs(){
    return this->_currentTimeStampInMs;
}
//====================================

}
