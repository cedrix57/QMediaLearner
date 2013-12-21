#ifndef CRASHMANAGERSINGLETON_H
#define CRASHMANAGERSINGLETON_H

#include <QObject>
#include <QSharedPointer>
#include <QList>
#include <QSettings>
#include <sequenceExtractor/PluginSequenceExtractor.h>

namespace ML{

class CrashManagerSingleton : public QObject{
    Q_OBJECT
public:
    static CrashManagerSingleton *getInstance();
    ~CrashManagerSingleton();
    bool getHasCrashed();
    qint64 getMediaPosition();
    QString getMediaFilePath();
    QString getSubtitle(int position);
    bool getSubtitleEnabled(int position);
    int getSubtitleShift(int position);
    double getPlaybackRate();
    int getVolume();
    QSharedPointer<QList<Sequence> > getSequences();

    
public slots:
    //void resetInfos();
    void setHasCrashed(bool crashed);
    void setMediaPosition(qint64 position);
    void setMediaFilePath(QString filePath);
    void setSubtitle(int position, QString filePath);
    void setSubtitleEnabled(int position, bool enabled);
    void setSubtitleShift(int position, int shiftInMs);
    void setPlaybackRate(double playbackRate);
    void setVolume(int volume);
    void setSequences(
            QSharedPointer<
            QList<Sequence> >
            sequences);

protected:
    explicit CrashManagerSingleton(
            QObject *parent = 0);
    static QSharedPointer<CrashManagerSingleton>
    instance;
    QSettings *settings;
};

}

#endif // CRASHMANAGERSINGLETON_H
