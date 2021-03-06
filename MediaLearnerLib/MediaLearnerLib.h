#ifndef MEDIALEARNERLIB_H
#define MEDIALEARNERLIB_H

#include "medialearnerlib_global.h"
#include <QMediaPlayer>
#include "sequenceExtractor/SequenceExtractor.h"
#include "subtitles/SubtitlesManager.h"
#include "encoders/FFmpegEncoder.h"
#include <QObject>


namespace ML{
class MEDIALEARNERLIBSHARED_EXPORT MediaLearnerLib
        : QObject{
    Q_OBJECT

public:
    MediaLearnerLib(QObject *parent = NULL);
    ~MediaLearnerLib();
    void stop();
    QMediaPlayer *getMediaPlayer();
    SequenceExtractor *getSequenceExtractor();
    SubtitlesManager *getSubtitlesManager();
    EncoderInterface *getEncoder();
    void setMedia(QString mediaPath);
    QString getMediaFilePath();
    QStringList getSupportedFormats();
    bool isFormatSupported(
            QString &filePath);

protected:
    QMediaPlayer mediaPlayer;
    SequenceExtractor sequenceExtractor;
    SubtitlesManager subtitlesManager;
    FFmpegEncoder encoder;

protected slots:
    void _onDrawingSettingsChanged();


};

}
#endif // MEDIALEARNERLIB_H
