#ifndef MEDIALEARNERLIB_H
#define MEDIALEARNERLIB_H

#include "medialearnerlib_global.h"
#include <QMediaPlayer>
#include "sequenceExtractor/SequenceExtractor.h"

namespace MediaLearner{
class MEDIALEARNERLIBSHARED_EXPORT MediaLearnerLib{

public:
    MediaLearnerLib();
    ~MediaLearnerLib();
    QMediaPlayer *getMediaPlayer();
    SequenceExtractor *getSequenceExtractor();
    //void setMedia(QString mediaPath);

protected:
    QMediaPlayer mediaPlayer;
    SequenceExtractor sequenceExtractor;
};

}
#endif // MEDIALEARNERLIB_H
