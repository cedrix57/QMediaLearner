#ifndef SETTINGSMANAGERSINGLETON_H
#define SETTINGSMANAGERSINGLETON_H

#include <QObject>
#include <QSharedPointer>
#include <QSettings>
#include <QFont>
#include <QColor>


class SettingsManagerSingleton : public QObject{
    Q_OBJECT
public:
    static SettingsManagerSingleton *getInstance();
    QString getLanguage();
    QFont getSubFont(
            int subPosition);
    QColor getSubColor(
            int subPosition);

signals:
    void languageChanged(QString language);
    void subSettingsChanged();
    void subSettingsChanged(int position);

public slots:
    void setLanguage(QString language);
    void setSubFont(
            int subPosition,
            QFont font);
    void setSubColor(
            int subPosition,
            QColor color);

protected:
    explicit SettingsManagerSingleton(
            QObject *parent = 0);
    static QSharedPointer<SettingsManagerSingleton>
    instance;
    QSettings *settings;
};

#endif // SETTINGSMANAGERSINGLETON_H
