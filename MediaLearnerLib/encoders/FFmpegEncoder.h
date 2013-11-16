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
    //*/

signals:

public slots:

protected:
    QString getFFmpegFilePath();
    QList<EncodingInfo> _getAvailableCodecs(
            QChar type);

};

}

#endif // FFMPEGINTERFACE_H
