#ifndef FFMPEGINTERFACE_H
#define FFMPEGINTERFACE_H

#include <QObject>
#include <QStringList>
#include <QProcess>
#include "medialearnerlib_global.h"

#include "EncoderInterface.h"

namespace ML{


class MEDIALEARNERLIBSHARED_EXPORT
        FFmpegEncoder : public EncoderInterface{ //TODO add abstract EncoderInterface
    Q_OBJECT
public:
    explicit FFmpegEncoder(
            QObject *parent = 0);
    virtual QMap<QString, ProfileInfo> getAvailableVideoProfiles();
    virtual QMap<QString, ProfileInfo> getAvailableAudioProfiles();
    virtual QList<EncodingInfo> getAvailableFormats(); //description?
    virtual QList<EncodingInfo> getAvailableVideoCodecs();
    virtual QList<EncodingInfo> getAvailableAudioCodecs();
    virtual QList<EncodingInfo> getAvailableSubtitlesCodecs();
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
    virtual void startEncoding(QString outFilePath);
    QString getFFmpegFilePath();
    QList<EncodingInfo> _getAvailableCodecs(
            QChar type);
    QProcess encodingProcess;
    QList<QStringList> argumentsList;
    QString tempInVideoFilePath;
    QStringList tempSequenceFilePaths;
    void _encodeTempVideoCommand();
    void _encodeSequenceCommand();
    void _encodeCuttedSequencesCommand(
            QString outFilePath);
    QString _getTempFilePath(QString fileName);
    void _removeTempFiles();
    int nFFmpegSteps;
    int nFFmpegStepsDone;

};

}

#endif // FFMPEGINTERFACE_H
