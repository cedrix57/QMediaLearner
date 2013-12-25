#include "SequenceWithSubs.h"

#include "../subtitles/SubtitlesManager.h"

namespace ML{

//====================================
QList<Sequence> SequencesWithSubs::lastEncodedSequences;
bool SequencesWithSubs::encodingFinished = true;
//====================================
//====================================
void SequenceWithSubs::substract(int ms){
    Sequence::substract(ms);
    for(QList<SubSequenceDrawable>::iterator it
        = this->subSequences.begin();
        it != this->subSequences.end();
        ++it){
        it->substract(ms);
    }
}
//====================================
//====================================
SequencesWithSubs::SequencesWithSubs(){
}
//====================================
void SequencesWithSubs::init(
        QList<Sequence> &sequences,
        SubtitleTrack *subtitleTracks){
    qDebug() << "QSize void SequencesWithSubs::init(...) called";
    SequencesWithSubs::encodingFinished = false;
    SequencesWithSubs::lastEncodedSequences
            = sequences;
    this->sequencesWithTexts.clear();
    for(QList<Sequence>::iterator seq
        = sequences.begin();
        seq != sequences.end();
        ++seq){
        SequenceWithSubs sequencesWithSubs;
        sequencesWithSubs.beginInMs = seq->beginInMs;
        sequencesWithSubs.endInMs = seq->endInMs;
        for(int i=0; i<SubtitlesManager::N_MAX_TRACKS; i++){
            QList<SubSequenceDrawable>
                    subSequencesDrawables
                    = subtitleTracks[i].getTexts(*seq);
            for(QList<SubSequenceDrawable>::iterator it
                = subSequencesDrawables.begin();
                it != subSequencesDrawables.end();
                ++it){
                it->project(*seq);
            }
            sequencesWithSubs.subSequences
                    << subSequencesDrawables;
        }
        this->sequencesWithTexts << sequencesWithSubs;
    }
    qDebug() << "QSize void SequencesWithSubs::init(...) end";
}
//====================================
void SequencesWithSubs::setScreenSize(
        QSize screenSize){
    qDebug() << "void SequencesWithSubs::setScreenSize(...) called";
    for(QList<SequenceWithSubs>::iterator it
        = this->sequencesWithTexts.begin();
        it != this->sequencesWithTexts.end();
        ++it){
            SubtitlesManager::setContext(
                        it->subSequences,
                        screenSize);
    }
    qDebug() << "void SequencesWithSubs::setScreenSize(...) end";
}
//====================================
QList<SequenceWithSubs>
SequencesWithSubs::getSequencesWithTexts(){
    return this->sequencesWithTexts;
}
//====================================
void SequencesWithSubs::setEnodingFinished(){
    SequencesWithSubs::encodingFinished = true;
}
//====================================
bool SequencesWithSubs::isSequencesEncoded(
        QList<Sequence> &sequences){
    qDebug() << "bool SequencesWithSubs::isSequencesEncoded(...) called";
    bool encoded
            = SequencesWithSubs::encodingFinished;
    if(encoded){
        int nSequences = sequences.size();
        int nLastSequences
                = SequencesWithSubs
                ::lastEncodedSequences.size();
        encoded = nSequences == nLastSequences;
        if(encoded){
            for(int i=0; i<nSequences; i++){
                if(sequences[i]
                        != SequencesWithSubs
                        ::lastEncodedSequences[i]){
                    encoded = false;
                    break;
                }
            }
        }
    }
    qDebug() << "bool SequencesWithSubs::isSequencesEncoded(...) end";
    return encoded;
}
//====================================

}
