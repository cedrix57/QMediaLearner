#ifndef ENCODERINTERFACE_H
#define ENCODERINTERFACE_H

#include <QObject>
#include <QMap>
#include <QSize>
#include <QSet>
#include "medialearnerlib_global.h"
#include "../sequenceExtractor/SequenceExtractor.h"
#include "../subtitles/SubtitleTrack.h"
#include "SequenceWithSubs.h"

namespace ML{

struct MEDIALEARNERLIBSHARED_EXPORT
        EncodingInfo{
    QString name;
    QString description;
};
struct MEDIALEARNERLIBSHARED_EXPORT
        ProfileInfo{
    QString name;
    QString description;
    QString ext;
};

class MEDIALEARNERLIBSHARED_EXPORT
        EncoderInterface : public QObject{
    Q_OBJECT
public:
    explicit EncoderInterface(QObject *parent = 0);
    virtual QMap<QString, ProfileInfo> getAvailableVideoProfiles() = 0;
    virtual QMap<QString, ProfileInfo> getAvailableAudioProfiles() = 0;
    QMap<QString, ProfileInfo> getAvailableProfiles();
    ProfileInfo getProfile(QString profileName);
    QSet<QString> getAvailableVideoProfileExts();
    QSet<QString> getAvailableAudioProfileExts();
    QString getSelectedProfileExt();
    QString getProfileExt(QString profileName);
    virtual QList<EncodingInfo> getAvailableFormats() = 0;
    virtual QList<EncodingInfo> getAvailableVideoCodecs() = 0;
    virtual QList<EncodingInfo> getAvailableAudioCodecs() = 0;
    virtual QList<EncodingInfo> getAvailableSubtitlesCodecs() = 0;
    void encode(QString outFilePath);
    void setInVideoFilePath(QString inVideoFilePath);
    void setSequences(
            QList<SequenceWithSubs> &sequencesWithSubs);
    void setFormat(QString format);
    void setAudioCodec(QString codec);
    void setVideoCodec(QString codec);
    void selectFormatProfile(QString profileName);
    void setSubtitleCodec(QString codec);
    virtual void setNewSize(QSize newSize);
    virtual QSize getOriginalSize();
    void setPlaybackRate(double rate);

signals:
    void encodingProgressed(int percentage);
    void encodingStarted();
    void encodingFinished();
    void encodingFailed();
    void encodingFailed(
            QString errorMessage);

protected slots:
    void _onEncodingSuccessful();

protected:
    virtual void startEncoding(QString outFilePath) = 0;
    QString inVideoFilePath;
    QString format;
    QString audioCodec;
    QString videoCodec;
    QString subCodec;
    QSize newSize;
    double playbackRate;
    QList<SequenceWithSubs> sequencesWithSubs;
    QList<SequenceWithSubs> lastSavedSequences;
    QString profileName;
    QString getFontPath(QString fontName);

    
};

}

#endif // ENCODERINTERFACE_H
