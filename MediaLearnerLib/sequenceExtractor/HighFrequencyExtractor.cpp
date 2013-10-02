#include "HighFrequencyExtractor.h"
#include <qmath.h>

namespace MediaLearner{
//====================================
HighFrequencyExtractor
    HighFrequencyExtractor::insance;
//====================================
HighFrequencyExtractor::HighFrequencyExtractor(
        QObject *parent) :
    PluginSequenceExtractor(parent){
}
//====================================
QString HighFrequencyExtractor::getName(){
    QString name = tr("High frequncy");
    return name;
}
//====================================
QString HighFrequencyExtractor::getDescription(){
    QString description = tr("Extractor that assumes the higher frequency as speech");
    return description;
}
//====================================
QList<Sequence> HighFrequencyExtractor::extractSequences(
        AudioBuffer buffer,
        int positionInMs){
    QList<Sequence> sequences;
    int minSpeachInMs = 1000;
    int timeStartSpeachThreshold = 1000;
    int nStartSpeachThreshold
            = timeStartSpeachThreshold
            * buffer.hzFreq / 1000.0 + 0.5;
    int timeNoSpeachThreshold = 1000;
    int nNoSpeachThreshold
            = timeNoSpeachThreshold
            * buffer.hzFreq / 1000.0 + 0.5;
    int frequencyThreshold
            = buffer.mean
            * 0.5 * qSqrt(buffer.var);
    int nLowerFrequency = 0;
    int nHighFrequency = 0;
    for(int i=0; i<buffer.bufferSize; i++){
        //int val = (buffer.buffer.data())[i];
        int val = buffer.buffer->buffer[i];
        bool speach = false;
        Sequence currentSequence;
        if(val >= frequencyThreshold){
            nLowerFrequency = 0;
            nHighFrequency++;
        }else{
            nLowerFrequency++;
            nHighFrequency--;
        }
        if(nLowerFrequency >= nNoSpeachThreshold){
            if(speach){
                currentSequence.maxInMs
                        = positionInMs
                        + i * 1000.0 / buffer.hzFreq + 0.5;
                int speachDurationInMs
                        = currentSequence.maxInMs
                        -  currentSequence.minInMs;
                if(speachDurationInMs >= minSpeachInMs){
                    sequences << currentSequence;
                }
                speach = false;
            }
        }else if(nHighFrequency >= nStartSpeachThreshold){
            if(!speach){
                int begin = qMin(0, i-nStartSpeachThreshold);
                currentSequence.minInMs
                        = positionInMs
                        + begin * 1000.0 / buffer.hzFreq + 0.5;
                speach = true;
            }
        }
    }
    return sequences;
}
//====================================

}

