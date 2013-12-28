#ifndef SETTINGSMANAGERSINGLETON_H
#define SETTINGSMANAGERSINGLETON_H

#include <QObject>
#include <QSharedPointer>
#include <QSettings>
#include <QFont>
#include <QColor>

#include "subtitles/SubSequence.h"
#include "medialearnerlib_global.h"

namespace ML{

class MEDIALEARNERLIBSHARED_EXPORT
        SettingsManagerSingleton : public QObject{
    Q_OBJECT
public:
    static SettingsManagerSingleton *getInstance();
    QString getLanguage();
    QString getSubFontFamily(
            int subPosition);
    QColor getSubColor(
            int subPosition);
    SubSize getSubSize(
            int subPosition);
    SubPosition getSubPosition(
            int subPosition);
    QString getExtractedVideoPath();
    bool isSortSequencesAndRemoveIntersect();
    bool isSaveInLogFile();
    bool isSendLogFileWhenCrashed();
    bool isLastProfileVideo();
    bool isSaveSRTsubs();
    QString getLastVideoProfileName();
    QString getLastAudioProfileName();

signals:
    void languageChanged(QString language);
    void subSettingsChanged();
    void subSettingsChanged(int position);
    void sortSequencesAndRemoveIntersect(bool enabled);

public slots:
    void setLanguage(QString language);
    void setSubFontFamily(
            int subPosition,
            QString family);
    void setSubColor(
            int subPosition,
            QColor color);
    void setSubSize(
            int subPosition,
            SubSize size);
    void setSubPosition(
            int subPosition,
            SubPosition position);
    void setExtractedVideoPath(QString path);
    void setSortSequencesAndRemoveIntersect(
            bool enabled);
    void setSaveInLogFiles(bool enabled);
    void setSendLogFileWhenCrashed(bool enabled);
    void setLastProfileVideo(bool is);
    bool setSaveSRTsubs(bool is);
    void setLastVideoProfileName(QString name);
    void setLastAudioProfileName(QString name);

protected:
    explicit SettingsManagerSingleton(
            QObject *parent = 0);
    static QSharedPointer<SettingsManagerSingleton>
    instance;
    QSettings *settings;
};

}
#endif // SETTINGSMANAGERSINGLETON_H
