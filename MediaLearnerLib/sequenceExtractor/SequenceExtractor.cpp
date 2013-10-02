#include "SequenceExtractor.h"
#include <QtConcurrentRun>

namespace MediaLearner{

//====================================
SequenceExtractor::SequenceExtractor(
        QObject *parent) :
    QObject(parent){
    this->_connectSlots();
    this->lowerMaxShiftMs = 5;
    this->upperNaxShiftMs = 5;
    this->reset();
}
//====================================
void SequenceExtractor::reset(){
    this->durationInMs = 0;
    this->durationProcessedInMs = 0;
    this->sequencesToExtract.clear();
}
//====================================
void SequenceExtractor::_connectSlots(){
    //this->connect(
             //&this->threadData.watcherSequence,
             //SIGNAL(finished()),
             //SLOT(onSequenceExtracted()));
    this->connect(
                &this->decoder,
                SIGNAL(bufferReady()),
                SLOT(onBufferReadyThread()));
}
//====================================
void SequenceExtractor::setExtractor(
        QString name){
    QMap<QString, PluginSequenceExtractor*>
            extractors
            = PluginSequenceExtractor::getExtractors();
    PluginSequenceExtractor* extractor
            = extractors[name];
    this->setExtractor(extractor);
}
//====================================
void SequenceExtractor::setExtractor(
        PluginSequenceExtractor *extractor){
    this->selectedExtractor = extractor;
}
//====================================
void SequenceExtractor::setMaxShifts(
        int lowerMaxInMs,
        int upperMaxInMs){
    this->lowerMaxShiftMs = lowerMaxInMs;
    this->upperNaxShiftMs = upperMaxInMs;
}
//====================================
//====================================
void SequenceExtractor::extractSequence(int position){
    int msecAhead = 5000;
    if(
            this->durationInMs == this->durationProcessedInMs
            || (position + msecAhead
                < this->durationProcessedInMs)){
        foreach(Sequence sequence, this->allSequences){
            if(sequence.minInMs <= position
                    && sequence.maxInMs >= position){
                this->extractedSequences << sequence;
                this->sequenceExtracted(sequence);
                break;
            }
        }
    }else{
        this->sequencesToExtract.enqueue(position);
    }
}
//====================================
void SequenceExtractor::extractQueuedSequences(){
    int n = this->sequencesToExtract.size();
    for(int i=0; i<n; i++){
        int currentPos
                = this->sequencesToExtract.dequeue();
        this->extractSequence(currentPos);
    }
}
//====================================
//====================================
void SequenceExtractor::setMediaSource(
        QString filePath){
    this->decoder.stop();
    this->allSequences.clear();
    this->extractedSequences.clear();
    if(!filePath.isEmpty()){
        this->reset();
        this->decoder.setSourceFilename(
                    filePath);
    }
}
//====================================
void SequenceExtractor::analyseMediaSource(){
    this->reset();
    this->decoder.start();
    this->decoder.duration();
}
//====================================
void SequenceExtractor::onBufferReadyThread(){
    QtConcurrent::run(
                this,
                &SequenceExtractor::onBufferReady);
}
//====================================
void SequenceExtractor::onBufferReady(){
    QAudioBuffer qtbuffer //TODO in thread
            = this->decoder.read();
    this->mutexBuffer.lock();
    AudioBuffer buffer;
    buffer.init(qtbuffer);
    int positionInMs = this->decoder.position();
    QList<Sequence> newSequences
            = this->selectedExtractor
            ->extractSequences(
                buffer,
                positionInMs);
    if(this->allSequences.size() > 0
            && newSequences.size() > 0){
        Sequence lastExistingSequence
                = this->allSequences.last();
        Sequence firstNewSequence
                = newSequences.first();
        if(lastExistingSequence.maxInMs
                == firstNewSequence.minInMs){
            Sequence mergedSequence;
            mergedSequence.minInMs
                    = lastExistingSequence.minInMs;
            mergedSequence.maxInMs
                    = firstNewSequence.maxInMs;
            this->allSequences.pop_back();
            newSequences.pop_front();
            this->allSequences << mergedSequence;
        }
    }
    this->allSequences << newSequences;
    this->durationProcessedInMs
            += qtbuffer.duration();
    this->extractQueuedSequences();
    this->mutexBuffer.unlock();
}
//====================================

}

