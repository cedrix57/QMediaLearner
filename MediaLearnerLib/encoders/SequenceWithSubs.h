#ifndef SEQUENCEWITHSUBS_H
#define SEQUENCEWITHSUBS_H

#include "../Utils/Sequence.h"
#include "../subtitles/SubSequence.h"

namespace MediaLearner{

//====================================
struct SequenceWithSubs : Sequence{
public:
    virtual void substract(int ms);
    QList<SubSequenceDrawable>
    subSequences;
};
//====================================
//====================================
class SequencesWithSubs{
public:
    SequencesWithSubs();
    void init(
            QList<Sequence> &sequences,
            QList<SubSequenceDrawable> &texts);
    QList<SequenceWithSubs> getSequencesWithTexts();


protected:
    QList<SequenceWithSubs> sequencesWithTexts;
};
//====================================

}

#endif // SEQUENCEWITHSUBS_H
