#include "SequenceExtractor.h"
#include <QtConcurrentRun>
#include <QDebug>
#include "../CrashManagerSingleton.h"
#include "HighFrequencyExtractor.h"
//#include <QVideoWidget>

namespace ML{

//QList<PluginSequenceExtractor*>
    //SequenceExtractor::extractors;
//====================================
SequenceExtractor::SequenceExtractor(
        QObject *parent) :
    QObject(parent){
    qDebug() << "SequenceExtractor::SequenceExtractor(...) called";
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
    this->_pausing = false;
    this->reset();
    qDebug() << "SequenceExtractor::SequenceExtractor(...) end";
}
//====================================
void SequenceExtractor::loadExtractors(){
    PluginSequenceExtractor *plugin
            = new HighFrequencyExtractor;
    //PluginSequenceExtractor::extractors
            //<< plugin;
}
//====================================
void SequenceExtractor::closeExtractors(){
    //foreach(
            //PluginSequenceExtractor *plugin,
            //PluginSequenceExtractor::extractors){
        //delete plugin;
    //}
}

//====================================
void SequenceExtractor::reset(){
    qDebug() << "void SequenceExtractor::reset() called";
    this->durationInMs = 0;
    this->durationProcessedInMs = 0;
    this->sequencesToExtract.clear();
    this->_isExtractionFinished = true;
    this->selectedSequence = -1;
    qDebug() << "void SequenceExtractor::reset() end";
}
//====================================
void SequenceExtractor::stop(){
    this->mediaPlayer.stop();
    this->decoder.stop();
    this->futurBuffer.waitForFinished();
}
//====================================
void SequenceExtractor::_connectSlots(){
    this->connect(
                &this->decoder,
                SIGNAL(bufferReady()),
                SLOT(onBufferReadyThread()));
    this->connect(
                &this->decoder,
                SIGNAL(finished()),
                SLOT(onDecodingFinished())); //TODO Shouldn't block the main thread
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
    qDebug() << "void SequenceExtractor::setExtractorQString) called";
    QMap<QString, PluginSequenceExtractor*>
            extractors
            = PluginSequenceExtractor::getExtractors();
    PluginSequenceExtractor* extractor
            = extractors[name];
    this->setExtractor(extractor);
    qDebug() << "void SequenceExtractor::setExtractor(QString) end";
}
//====================================
void SequenceExtractor::setExtractor(
        PluginSequenceExtractor *extractor){
    qDebug() << "void SequenceExtractor::setExtractor(PluginSequenceExtractor) called";
    this->selectedExtractor = extractor;
    qDebug() << "void SequenceExtractor::setExtractor(PluginSequenceExtractor) end";
}
//====================================
void SequenceExtractor::setMaxShifts(
        int lowerMaxInMs,
        int upperMaxInMs){
    qDebug() << "void SequenceExtractor::setMaxShifts(...) called";
    this->lowerMaxShiftMs = lowerMaxInMs;
    this->upperNaxShiftMs = upperMaxInMs;
    qDebug() << "void SequenceExtractor::setMaxShifts(...) end";
}
//====================================
//====================================
void SequenceExtractor::extractSequence(int position){
    //TODO remeve...we do that the time we get a good extractor
    qDebug() << "void SequenceExtractor::extractSequence(int position) called";
    Sequence sequence;
    sequence.beginInMs = qMax(0, position - 2000);
    int duration = this->mediaPlayer.duration();
    sequence.endInMs = qMin(duration, position + 2000);
    *this->extractedSequences << sequence;
    CrashManagerSingleton::getInstance()
            ->setSequences(
                this->extractedSequences);
    this->sequenceExtracted(sequence);
    int nSequences = this->extractedSequences->size();
    this->numberOfSequencesChanged(nSequences);
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
            if(sequence.beginInMs <= position
                    && sequence.endInMs >= position){
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
    qDebug() << "void SequenceExtractor::extractSequence(int position) end";
}
//====================================
void SequenceExtractor::extractQueuedSequences(){
    qDebug() << "void SequenceExtractor::extractQueuedSequences() called";
    int n = this->sequencesToExtract.size();
    for(int i=0; i<n; i++){
        int currentPos
                = this->sequencesToExtract.dequeue();
        this->extractSequence(currentPos);
    }
    qDebug() << "void SequenceExtractor::extractQueuedSequences() end";
}
//====================================
//====================================
void SequenceExtractor::setMediaSource(
        QString filePath){
    qDebug() << "void SequenceExtractor::setMediaSource(...) called";
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
    qDebug() << "void SequenceExtractor::setMediaSource(...) end";
}
//====================================
void SequenceExtractor::analyseMediaSource(){
    qDebug() << "void SequenceExtractor::analyseMediaSource() called";
    this->reset();
    this->_isExtractionFinished = false;
    this->decoder.start();
    qDebug() << "void SequenceExtractor::analyseMediaSource() end";
}
//====================================
bool SequenceExtractor::isExtractionFinished(){
    return this->_isExtractionFinished;
}
#include <unistd.h>//TODO
//====================================
void SequenceExtractor::waitForExtractionFinished(){
    qDebug() << "void SequenceExtractor::waitForExtractionFinished() called";
    while(!this->_isExtractionFinished){
        sleep(100);
    }
    qDebug() << "void SequenceExtractor::waitForExtractionFinished() end";
}
//====================================
void SequenceExtractor::onDecodingFinished(){
    qDebug() << "void SequenceExtractor::onDecodingFinished() called";
    this->extractQueuedSequences();
    this->futurBuffer.waitForFinished();
    this->allSequences
            = this->selectedExtractor
            ->getExtractedSequences();
    this->_isExtractionFinished = true;
    this->sequencesExtracted();
    qDebug() << "void SequenceExtractor::onDecodingFinished() end";
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
void SequenceExtractor::removeSequence(
        int position){
    qDebug() << "void SequenceExtractor::removeSequence() called";
    this->extractedSequences
            ->removeAt(
                position);
    if(this->selectedSequence
            >= position){
        this->selectedSequence--;
    }
    CrashManagerSingleton::getInstance()
            ->setSequences(
                this->extractedSequences);
    int nSequences = this->extractedSequences->size();
    this->numberOfSequencesChanged(nSequences);
    qDebug() << "void SequenceExtractor::removeSequence() end";
}
//====================================
void SequenceExtractor::changeSequence(
        int position,
        Sequence newSequence){
    qDebug() << "void SequenceExtractor::changeSequence(...) called";
    (*this->extractedSequences)
            [position]
            = newSequence;
    CrashManagerSingleton::getInstance()
            ->setSequences(
                this->extractedSequences);
    qDebug() << "void SequenceExtractor::changeSequence(...) end";
}
//====================================
void SequenceExtractor::changeMinSequence(
        int position,
        int newMin){
    qDebug() << "void SequenceExtractor::changeMinSequence((...) called";
    (this->extractedSequences
            ->begin() + position)
            ->beginInMs = newMin;
    CrashManagerSingleton::getInstance()
            ->setSequences(
                this->extractedSequences);
    qDebug() << "void SequenceExtractor::changeMinSequence((...) end";
}
//====================================
void SequenceExtractor::changeMaxSequence(
        int position,
        int newMax){
    qDebug() << "void SequenceExtractor::changeMaxSequence(...) called";
    (this->extractedSequences
            ->begin() + position)
            ->endInMs = newMax;
    CrashManagerSingleton::getInstance()
            ->setSequences(
                this->extractedSequences);
    qDebug() << "void SequenceExtractor::changeMaxSequence(...) end";
}
//====================================
void SequenceExtractor::selectSequence(
        int position){
    qDebug() << "void SequenceExtractor::selectSequence(...) called";
    this->selectedSequence = position;
    Sequence selectedSequence
            = this->getSelectedSequence();
    this->mediaPlayer.stop();
    this->mediaPlayer.setPosition(
                selectedSequence.beginInMs);
    qDebug() << "void SequenceExtractor::selectSequence(...) end";
}
//====================================
void SequenceExtractor::_onMediaChanged(
        QMediaContent mediaContent){
    qDebug() << "void SequenceExtractor::_onMediaChanged(...) called";
    Q_UNUSED(mediaContent);
    this->reset();
    qDebug() << "void SequenceExtractor::_onMediaChanged(...) end";
}
//====================================
void SequenceExtractor::_onPlayerPositionChanged(
        qint64 position){
    qDebug() << "void SequenceExtractor::_onPlayerPositionChanged(...) called";
    if(this->selectedSequence >= 0){
        Sequence selectedSequence
                = this->getSelectedSequence();
        if(position >= selectedSequence.endInMs){
            if(!this->_pausing){
                this->_pausing = true;
                this->mediaPlayer.pause();
                if(position > selectedSequence.endInMs){
                    this->mediaPlayer.setPosition(
                                selectedSequence.beginInMs);
                }
                this->_pausing = false;
            }
        }
    }
    qDebug() << "void SequenceExtractor::_onPlayerPositionChanged(...) end";
}
//====================================
Sequence SequenceExtractor::getSelectedSequence(){
    Sequence selectedSequence
            = (*this->extractedSequences)
            [this->selectedSequence];
    return selectedSequence;
}
//====================================
void SequenceExtractor::setSequences(
        QSharedPointer<
        QList<Sequence> >
        sequences){
    qDebug() << "void SequenceExtractor::setSequences(...) called";
    int nOldSequences = this->extractedSequences->size();
    this->extractedSequences
            = sequences;
    CrashManagerSingleton::getInstance()
            ->setSequences(sequences);
    int nSequences = this->extractedSequences->size();
    if(nSequences != nOldSequences){
         this->numberOfSequencesChanged(nSequences);
    }
    qDebug() << "void SequenceExtractor::setSequences(...) end";
}
//====================================
QMediaPlayer *SequenceExtractor::getMediaPlayer(){
    return &this->mediaPlayer;
}
//====================================
void SequenceExtractor::sortAndMergeSequences(){
    //Sorting
    qDebug() << "void SequenceExtractor::sortAndMergeSequences() called";
    int nSequences = this->extractedSequences->size();
    for(int i=0; i<nSequences; i++){
        int idMinSequence = i;
        for(int j=i+1; j<nSequences; j++){
            if(this->extractedSequences->at(j).beginInMs
                    < this->extractedSequences->at(idMinSequence)
                    .beginInMs){
                idMinSequence = j;
            }
        }
        if(i != idMinSequence){
            Sequence temp
                    = this->extractedSequences
                    ->at(idMinSequence);
            (*this->extractedSequences)[idMinSequence]
                    = (*this->extractedSequences)[i];
            (*this->extractedSequences)[i]
                    = temp;
        }
    }
    //Merging
    for(int i=0; i<this->extractedSequences->size()-1; i++){
        Sequence sequence
                = (*this->extractedSequences)[i];
        Sequence nextSequence
                = (*this->extractedSequences)[i+1];
        if(sequence.endInMs >= nextSequence.beginInMs){
            sequence.endInMs
                    = qMax(
                        sequence.endInMs,
                        nextSequence.endInMs);
            (*this->extractedSequences)[i] = sequence;
            this->extractedSequences->removeAt(i+1);
            i--;
        }
    }
    CrashManagerSingleton::getInstance()
            ->setSequences(
                this->extractedSequences);
    qDebug() << "void SequenceExtractor::sortAndMergeSequences() end";
}
//====================================
}

