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
            QList<Sequence> &sequences);
    void init(
            QList<Sequence> &sequences,
            SubtitleTrack *subtitleTracks);
    void setScreenSize(QSize screenSize);
    QList<SequenceWithSubs> getSequencesWithTexts();
    static void setEnodingFinished();
    static bool isSequencesEncoded(
            QList<Sequence> &sequences);
    //void apply


protected:
    QList<SequenceWithSubs> sequencesWithTexts;
    static QList<Sequence> lastEncodedSequences;
    static bool encodingFinished;
};
//====================================

}

#endif // SEQUENCEWITHSUBS_H
