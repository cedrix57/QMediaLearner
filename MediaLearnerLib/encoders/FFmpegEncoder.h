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
    virtual QMap<QString, EncodingInfo> getAvailableFormatProfiles();
    virtual QList<EncodingInfo> getAvailableFormats(); //description?
    virtual QList<EncodingInfo> getAvailableVideoCodecs();
    virtual QList<EncodingInfo> getAvailableAudioCodecs();
    virtual QList<EncodingInfo> getAvailableSubtitlesCodecs();
    virtual void encode(QString outFilePath);
    int getFps();
    int getNFrame(qint64 ms, int frameRate);
    virtual QSize getOriginalSize();
    QString getFormatedTime(qint64 ms);
    //*/

signals:

protected slots:
    void _onProcessFinished(
            int exitCode,
            QProcess::ExitStatus exitStatus);
    void _onProcessError(
            QProcess::ProcessError error);
protected:
    QString getFFmpegFilePath();
    QList<EncodingInfo> _getAvailableCodecs(
            QChar type);
    void _evalSizeEventually();
    QProcess encodingProcess;
    QList<QStringList> argumentsList;
    QString tempInVideoFilePath;
    QStringList tempSequenceFilePaths;
    int fps;
    void _encodeTempVideoCommand();
    void _encodeSequenceCommand();
    void _encodeCuttedSequencesCommand(
            QString outFilePath);
    QString _getTempFilePath(QString fileName);
    void _removeTempFiles();

};

}

#endif // FFMPEGINTERFACE_H
