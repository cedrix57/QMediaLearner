#include "SettingsManagerSingleton.h"
#include <QDebug>


//====================================
//====================================
QSharedPointer<SettingsManagerSingleton> SettingsManagerSingleton::instance;
//====================================
SettingsManagerSingleton::SettingsManagerSingleton(QObject *parent) :
    QObject(parent){
    this->settings
            = new QSettings(
                "QMediaRecorder",
                "QMediaRecorder",
                this);
    //QString currentPath = QDir::currentPath();
    //this->settings = new QSettings("WebcamRecorder", currentPath, this);
}
//====================================
SettingsManagerSingleton *SettingsManagerSingleton::getInstance(){
    if(SettingsManagerSingleton::instance.isNull()){
        QSharedPointer<SettingsManagerSingleton> localInstance(new SettingsManagerSingleton);
        SettingsManagerSingleton::instance = localInstance;
    }
    SettingsManagerSingleton *pointer = SettingsManagerSingleton::instance.data();
    return pointer;
}
//====================================
const QString KEY_LANGUAGE = "lang";
//====================================
QString SettingsManagerSingleton::getLanguage(){
    QString language
            = this->settings->value(
                KEY_LANGUAGE, "en").toString();
    return language;
}
//====================================
void SettingsManagerSingleton::setLanguage(
        QString language){
    this->settings->setValue(KEY_LANGUAGE, language);
    emit this->languageChanged(language);
}
//====================================
//====================================
const QString KEY_SUBFONT = "lang";
//====================================
QFont SettingsManagerSingleton::getSubFont(
        int subPosition){
    QString key = KEY_SUBFONT + subPosition;
    QFont defaultFont("Arial");
    QVariant fontVariant
            = this->settings->value(
                key,
                defaultFont);
    QFont font = fontVariant.value<QFont>();
    font = defaultFont;
    return font;
}
//====================================
void SettingsManagerSingleton::setSubFont(
        int subPosition,
        QFont font){
    QString key = KEY_SUBFONT + subPosition;
    this->settings->setValue(
                key,
                font);
    this->subSettingsChanged();
    this->subSettingsChanged(subPosition);
}
//====================================
//====================================
const QString KEY_SUBCOLOR = "lang";
//====================================
QColor SettingsManagerSingleton::getSubColor(
        int subPosition){
    QString key = KEY_SUBCOLOR + subPosition;
    QColor defaultColor = Qt::yellow;
    if(subPosition == 1){
        defaultColor = Qt::cyan;
    }else if(subPosition == 2){
        defaultColor = Qt::green;
    }
    QVariant colorVariant
            = this->settings->value(
                key,
                defaultColor);
    QColor color = colorVariant.value<QColor>();
    return color;
}
//====================================
void SettingsManagerSingleton::setSubColor(
        int subPosition,
        QColor color){
    QString key = KEY_SUBCOLOR + subPosition;
    this->settings->setValue(
                key,
                color);
    this->subSettingsChanged();
    this->subSettingsChanged(subPosition);
}
//====================================


