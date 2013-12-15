#ifndef SEQUENCEWITHSUBS_H
#define SEQUENCEWITHSUBS_H

#include "../Utils/Sequence.h"
#include "../subtitles/SubSequence.h"
#include "../subtitles/SubtitleTrack.h"

namespace ML{

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
            SubtitleTrack *subtitleTracks);
    void setScreenSize(QSize screenSize);
    QList<SequenceWithSubs> getSequencesWithTexts();
    //void apply


protected:
    QList<SequenceWithSubs> sequencesWithTexts;
};
//====================================

}

#endif // SEQUENCEWITHSUBS_H
