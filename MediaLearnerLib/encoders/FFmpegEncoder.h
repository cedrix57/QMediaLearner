#ifndef FFMPEGINTERFACE_H
#define FFMPEGINTERFACE_H

#include <QObject>
#include <QStringList>
#include <QProcess>

#include "EncoderInterface.h"

namespace MediaLearner{


class FFmpegEncoder : public EncoderInterface{ //TODO add abstract EncoderInterface
    Q_OBJECT
public:
    explicit FFmpegEncoder(
            QObject *parent = 0);
    virtual QList<EncodingInfo> getAvailableFormats(); //description?
    virtual QList<EncodingInfo> getAvailableVideoCodecs();
    virtual QList<EncodingInfo> getAvailableAudioCodecs();
    virtual QList<EncodingInfo> getAvailableSubtitlesCodecs();
    virtual void encode(QString outFilePath);
    int getFps();
    int getNFrame(qint64 ms, int frameRate);
    virtual QSize getSize();
    QString getFormatedTime(qint64 ms);
    //*/

signals:

protected slots:
    void _onProcessFinished(
            int exitCode,
            QProcess::ExitStatus exitStatus);
protected:
    QString getFFmpegFilePath();
    QList<EncodingInfo> _getAvailableCodecs(
            QChar type);
    void _evalSizeEventually();
    QProcess encodingProcess;
    QList<QStringList> argumentsList;
    QString tempInVideoFilePath;
    int fps;
    void _encodeTempVideoCommand();
    void _encodeSequenceCommand(
            QList<SequencesWithSubs> &sequencesWithSubs);

};

}

#endif // FFMPEGINTERFACE_H
