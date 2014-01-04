#ifndef FFMPEGAUDIODECODERCONTROL_H
#define FFMPEGAUDIODECODERCONTROL_H

#include <QAudioDecoderControl>

class FFmpegAudioDecoderControl
        : public QAudioDecoderControl{
    Q_OBJECT
public:
    explicit FFmpegAudioDecoderControl(
            QObject *parent);
    
    virtual QAudioDecoder::State state() const;

    virtual QString sourceFilename() const;
    virtual void setSourceFilename(const QString &fileName);

    virtual QIODevice* sourceDevice() const;
    virtual void setSourceDevice(QIODevice *device);

    virtual void start();
    virtual void stop();

    virtual QAudioFormat audioFormat() const;
    virtual void setAudioFormat(const QAudioFormat &format);

    virtual QAudioBuffer read();
    virtual bool bufferAvailable() const;

    virtual qint64 position() const;
    virtual qint64 duration() const;

protected:
    QString _sourceFileName;
    QIODevice *_device;
    QAudioDecoder::State _state;
    QAudioFormat _format;
};

#endif // FFMPEGAUDIODECODERCONTROL_H
