#include "SequenceWithSubs.h"

#include "../subtitles/SubtitlesManager.h"

namespace ML{

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
}
//====================================
void SequencesWithSubs::setScreenSize(
        QSize screenSize){
    for(QList<SequenceWithSubs>::iterator it
        = this->sequencesWithTexts.begin();
        it != this->sequencesWithTexts.end();
        ++it){
            SubtitlesManager::setContext(
                        it->subSequences,
                        screenSize);
    }
}
//====================================
QList<SequenceWithSubs>
SequencesWithSubs::getSequencesWithTexts(){
    return this->sequencesWithTexts;
}
//====================================

}
