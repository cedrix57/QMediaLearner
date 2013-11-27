#ifndef ENCODERINTERFACE_H
#define ENCODERINTERFACE_H

#include <QObject>
#include <QSize>
#include "../sequenceExtractor/SequenceExtractor.h"
#include "../subtitles/SubtitleTrack.h"
#include "SequenceWithSubs.h"

namespace MediaLearner{

struct EncodingInfo{
    QString name;
    QString description;
};

class EncoderInterface : public QObject{
    Q_OBJECT
public:
    explicit EncoderInterface(QObject *parent = 0);
    virtual QList<EncodingInfo> getAvailableFormats() = 0; //description?
    virtual QList<EncodingInfo> getAvailableVideoCodecs() = 0;
    virtual QList<EncodingInfo> getAvailableAudioCodecs() = 0;
    virtual QList<EncodingInfo> getAvailableSubtitlesCodecs() = 0;
    virtual void encode(QString outFilePath) = 0;
    void setInVideoFilePath(QString inVideoFilePath);
    void setSequences(
            QList<SequenceWithSubs> &sequencesWithSubs);
    void setFormat(QString format);
    void setAudioCodec(QString codec);
    void setVideoCodec(QString codec);
    void setSubtitleCodec(QString codec);
    virtual void setSize(QSize size);
    virtual QSize getSize();
    void setPlaybackRate(double rate);

signals:
    void encodingFinished();
    void encodingFailed();
    void encodingFailed(
            QString errorMessage);

protected:
    QString inVideoFilePath;
    QString format;
    QString audioCodec;
    QString videoCodec;
    QString subCodec;
    QSize size;
    double playbackRate;
    QList<SequenceWithSubs> sequencesWithSubs;
    QString getFontPath(QString fontName);

    
};

}

#endif // ENCODERINTERFACE_H
