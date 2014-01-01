#include "HighFrequencyExtractor.h"
#include <qmath.h>

namespace ML{
//====================================
HighFrequencyExtractor
    HighFrequencyExtractor::instance;
//====================================
HighFrequencyExtractor::HighFrequencyExtractor(
        QObject *parent) :
    PluginSequenceExtractor(parent){
}
//====================================
QString HighFrequencyExtractor::getName(){
    QString name = "High frequency";
    return name;
}
//====================================
QString HighFrequencyExtractor::getDescription(){
    QString description = tr("Extractor that assumes the higher frequency as speech");
    return description;
}
//====================================
void HighFrequencyExtractor::reset(){
    qDebug() << "void HighFrequencyExtractor::reset() called";
    PluginSequenceExtractor::reset();
    this->nLowerFrequency = 0;
    this->nHighFrequency = 0;
    this->sumMean = 0;
    this->sumSmallerMean = 0;
    this->nIteration = 0;
    this->inPhrase = false;
    this->meanAtPhraseBegin = 0;
    this->phraseDuration = 0;
    this->lastValues.clear();
    qDebug() << "void HighFrequencyExtractor::reset() end";
}
//====================================
void HighFrequencyExtractor::analyseBuffer(
        QAudioBuffer audioBuffer){
    qDebug() << "void HighFrequencyExtractor::analyseBuffer(...) called";
    int timeMeanInMs = 600;
    int timeSmallerMeanInMs = 300;
    int timeStartSpeachThresholdInMs = 200;
    int timeNoSpeachThresholdInMs = 200;
    int minSpeachInMs = 500;
    int maxSpeachInMs = 4000;
    QAudioFormat format = audioBuffer.format();
    int sampleRate = format.sampleRate();
    int nStartSpeachThreshold
            = timeStartSpeachThresholdInMs
            * sampleRate / 1000.0 + 0.5;
    int nNoSpeachThreshold
            = timeNoSpeachThresholdInMs
            * sampleRate / 1000.0 + 0.5;
    int nMinSpeach
            = minSpeachInMs
            * sampleRate / 1000.0 + 0.5;
    int nMaxSpeach
            = maxSpeachInMs
            * sampleRate / 1000.0 + 0.5;
    int sizeMeanInMs
            = timeMeanInMs
            * sampleRate / 1000.0 + 0.5;
    int sizeSmallerMeanInMs
            = timeSmallerMeanInMs
            * sampleRate / 1000.0 + 0.5;
    float percDiffMeans = 0.10;
    QAudioFormat::SampleType sampleType
            = format.sampleType();
    //int frameCount = audioBuffer.frameCount();
    int nChannels = format.channelCount();
    int bytesPerFrame = format.bytesPerFrame();
    int bytesPerValue = bytesPerFrame/nChannels;
    /*
    int frequencyThreshold
            = buffer.mean
            + 0.5 * buffer.sd;
            //*/
    int sizeBuffer = audioBuffer.byteCount();
    for(int i=0; i<sizeBuffer; i++){
        int currentValue =
                this->getValue(
                    audioBuffer,
                    sampleType,
                    i,
                    nChannels,
                    bytesPerFrame,
                    bytesPerValue);
        this->nIteration++;
        this->lastValues.enqueue(currentValue);
        this->sumMean += currentValue;
        this->sumSmallerMean += currentValue;
        if(this->nIteration > sizeSmallerMeanInMs){
            int pos = this->lastValues.size() - sizeSmallerMeanInMs;
            int olderSmallerValue
                    = this->lastValues.at(pos);
            this->sumSmallerMean -= olderSmallerValue;
        }
        if(this->nIteration > sizeMeanInMs){
            int olderValue
                    = this->lastValues.dequeue();
            this->sumMean -= olderValue;
            int mean = (this->sumMean - this->sumSmallerMean)
                    / (sizeMeanInMs - sizeSmallerMeanInMs);
            int smallerMean = this->sumSmallerMean / sizeSmallerMeanInMs;
            if(!this->inPhrase){
                if(smallerMean >= mean*percDiffMeans){
                    this->nHighFrequency++;
                }else{
                    //int duration = this->getCurrentTimeStampInMs();
                    if(this->nHighFrequency > 10){
                            //&& duration > 10500){
                    }
                    this->nHighFrequency = 0;
                }
                if(this->nHighFrequency > nStartSpeachThreshold){
                    this->nHighFrequency = 0;
                    this->inPhrase = true;
                    this->meanAtPhraseBegin = mean;
                    this->phraseDuration = 0;
                }
            }else{
                if(smallerMean <= mean*(1-percDiffMeans)){
                    this->nLowerFrequency++;
                }else{
                    if(this->nLowerFrequency > 10){
                    }
                    this->nLowerFrequency = 0;
                }
                this->phraseDuration++;
                if(this->nLowerFrequency > nNoSpeachThreshold
                    || this->phraseDuration > nMaxSpeach){
                    this->nLowerFrequency = 0;
                    this->inPhrase = false;
                    this->meanAtPhraseBegin = 0;
                    if(this->phraseDuration >= nMinSpeach
                            && this->phraseDuration <= nMaxSpeach){
                        Sequence sequence;
                        int nMax = this->nIteration;
                        int nMin = nMax - this->phraseDuration;
                        sequence.beginInMs =
                                nMin * 1.0 / sampleRate * 1000.0 + 0.5;
                        sequence.endInMs =
                                nMax * 1.0 / sampleRate * 1000.0 + 0.5;
                        *this->extractedSequences << sequence;
                    }
                }
            }
        }
    }
    qDebug() << "void HighFrequencyExtractor::analyseBuffer(...) end";
}
//====================================

}

