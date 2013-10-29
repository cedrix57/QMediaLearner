#include "SequenceExtractor.h"
#include <QtConcurrentRun>
#include <QDebug>
//#include <QVideoWidget>

namespace MediaLearner{

//====================================
SequenceExtractor::SequenceExtractor(
        QObject *parent) :
    QObject(parent){
    this->extractedSequences
            = QSharedPointer<
            QList<Sequence> >(
                new QList<Sequence>);
    this->allSequences
            = QSharedPointer<
            QList<Sequence> >(
                new QList<Sequence>);
    this->_connectSlots();
    this->lowerMaxShiftMs = 5000;
    this->upperNaxShiftMs = 5000;
    this->selectedSequence = -1;
    this->reset();
}
//====================================
void SequenceExtractor::reset(){
    this->durationInMs = 0;
    this->durationProcessedInMs = 0;
    this->sequencesToExtract.clear();
    this->_isExtractionFinished = true;
    this->selectedSequence = -1;
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
    this->connect(
                &this->decoder,
                SIGNAL(finished()),
                SLOT(extractQueuedSequences()));
    this->connect(
                &this->decoder,
                SIGNAL(finished()),
                SLOT(onDecodingFinished()));
    this->connect(
                &this->mediaPlayer,
                SIGNAL(positionChanged(qint64)),
                SLOT(_onPlayerPositionChanged(qint64)));
    this->connect(
                &this->mediaPlayer,
                SIGNAL(mediaChanged(QMediaContent)),
                SLOT(_onMediaChanged(QMediaContent)));
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
    //TODO remeve...we do that the time we get a good extractor
    Sequence sequence;
    sequence.minInMs = qMax(0, position - 2000);
    int duration = this->mediaPlayer.duration();
    sequence.maxInMs = qMin(duration, position + 2000);
    *this->extractedSequences << sequence;
    this->sequenceExtracted(sequence);
    /*
    int msecAhead = 5000;
    qint64 durationProcessedInMs
            = this->selectedExtractor
            ->getCurrentTimeStampInMs();
    if(
            this->durationInMs >= durationProcessedInMs
            || (position + msecAhead
                < durationProcessedInMs)){
        foreach(Sequence sequence, *this->allSequences){
            if(sequence.minInMs <= position
                    && sequence.maxInMs >= position){
                //TODO check it is not to big and
                // do so it is less than 5 secondes
                // + put the parameter in settings
                *this->extractedSequences << sequence;
                this->sequenceExtracted(sequence);
                break;
            }
        }
    }else{
        this->sequencesToExtract.enqueue(position);
    }
    //*/
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
    this->allSequences->clear();
    this->extractedSequences->clear();
    if(!filePath.isEmpty()){
        this->reset();
        QUrl url = QUrl::fromLocalFile(filePath);
        QMediaContent mediaSource(url);
        //QVideoWidget w;
        //this->mediaPlayer.setVideoOutput(
                    //&w);
        this->mediaPlayer.setMedia(
                    mediaSource);
        this->decoder.setSourceFilename(
                    filePath);
    }
}
//====================================
void SequenceExtractor::analyseMediaSource(){
    this->reset();
    this->_isExtractionFinished = false;
    this->decoder.start();
}
//====================================
bool SequenceExtractor::isExtractionFinished(){
    return this->_isExtractionFinished;
}
#include <unistd.h>//TODO
//====================================
void SequenceExtractor::waitForExtractionFinished(){
    while(!this->_isExtractionFinished){
        sleep(100);
    }
}
//====================================
void SequenceExtractor::onDecodingFinished(){
    this->futurBuffer.waitForFinished();
    this->allSequences
            = this->selectedExtractor
            ->getExtractedSequences();
    this->_isExtractionFinished = true;
}
//====================================
void SequenceExtractor::onBufferReadyThread(){
    //TODO faire ça toutes les x seconde + à la fin
   this->futurBuffer
            = QtConcurrent::run(
                this->selectedExtractor,
                &PluginSequenceExtractor::onBufferReady,
                &this->decoder);
}
/*
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
//*/
//====================================
QSharedPointer<
QList<Sequence> >
SequenceExtractor::getExtractedSequences(){
    return this->extractedSequences;
}
//====================================
QSharedPointer<
QList<Sequence>
>SequenceExtractor::getAllSequences(){
    return this->allSequences;
}
//====================================
void SequenceExtractor::deleteSequence(
        int position){
    this->extractedSequences
            ->removeAt(
                position);
}
//====================================
void SequenceExtractor::changeSequence(
        int position,
        Sequence newSequence){
    (*this->extractedSequences)
            [position]
            = newSequence;
}
//====================================
void SequenceExtractor::selectSequence(
        int position){
    this->selectedSequence = position;
    Sequence selectedSequence
            = this->getSelectedSequence();
    this->mediaPlayer.setPosition(
                selectedSequence.minInMs);
}
//====================================
void SequenceExtractor::_onMediaChanged(
        QMediaContent mediaContent){
    this->reset();
}
//====================================
void SequenceExtractor::_onPlayerPositionChanged(
        qint64 position){
    if(this->selectedSequence >= 0){
        Sequence selectedSequence
                = this->getSelectedSequence();
        if(position >= selectedSequence.maxInMs){
            this->mediaPlayer.pause();
            if(position > selectedSequence.maxInMs){
                this->mediaPlayer.setPosition(
                            selectedSequence.maxInMs);
            }
        }
    }
}
//====================================
Sequence SequenceExtractor::getSelectedSequence(){
    Sequence selectedSequence
            = (*this->extractedSequences)
            [this->selectedSequence];
    return selectedSequence;
}
//====================================
QMediaPlayer *SequenceExtractor::getMediaPlayer(){
    return &this->mediaPlayer;
}
//====================================
}

