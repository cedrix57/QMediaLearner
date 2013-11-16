#ifndef ENCODERINTERFACE_H
#define ENCODERINTERFACE_H

#include <QObject>
#include <QSize>

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
    virtual void setFormat(QString format);
    virtual void setAudioCodec(QString codec);
    virtual void setVideoCodec(QString codec);
    virtual void setSubtitleCodec(QString codec);
    virtual void setSize(QSize size);
    virtual void setPlaybackRate(double rate);
    //TODO set subs
    //TODO set sequences
    virtual void setInVideoFilePath(QString inVideoFilePath);
    virtual void encode(QString outFilePath) = 0;

signals:
    void encodingFinished();
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

    
};

}

#endif // ENCODERINTERFACE_H
