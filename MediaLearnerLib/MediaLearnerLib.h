#ifndef MEDIALEARNERLIB_H
#define MEDIALEARNERLIB_H

#include "medialearnerlib_global.h"
#include <QMediaPlayer>
#include "sequenceExtractor/SequenceExtractor.h"
#include "subtitles/SubtitlesManager.h"

namespace MediaLearner{
class MEDIALEARNERLIBSHARED_EXPORT MediaLearnerLib{

public:
    MediaLearnerLib();
    ~MediaLearnerLib();
    QMediaPlayer *getMediaPlayer();
    SequenceExtractor *getSequenceExtractor();
    SubtitlesManager *getSubtitlesManager();
    void setMedia(QString mediaPath);

protected:
    QMediaPlayer mediaPlayer;
    SequenceExtractor sequenceExtractor;
    SubtitlesManager subtitlesManager;
};

}
#endif // MEDIALEARNERLIB_H
