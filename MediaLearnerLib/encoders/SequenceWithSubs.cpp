#include "SequenceWithSubs.h"

namespace MediaLearner{

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
void SequencesWithSubs::init(
        QList<Sequence> &sequences,
        QList<SubSequenceDrawable> &texts){
    this->sequencesWithTexts.clear();
}
//====================================
QList<SequenceWithSubs>
SequencesWithSubs::getSequencesWithTexts(){
    return this->sequencesWithTexts;
}
//====================================

}
