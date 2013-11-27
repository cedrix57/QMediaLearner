#include "SettingsManagerSingleton.h"
#include <QDebug>


namespace MediaLearner{

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
const QString KEY_SUBFONTFAMILY = "fontFamily";
//====================================
QString SettingsManagerSingleton::getSubFontFamily(
        int subPosition){
    QString key = KEY_SUBFONTFAMILY + subPosition;
    QString defaultFamily("Arial");
    QString family
            = this->settings->value(
                key,
                defaultFamily).toString();
    /*
    QVariant fontVariant
            = this->settings->value(
                key,
                defaultFont);
    QFont font = fontVariant.value<QFont>();
    font = defaultFont;
    //*/
    return family;
}
//====================================
void SettingsManagerSingleton::setSubFontFamily(
        int subPosition,
        QString family){
    QString key = KEY_SUBFONTFAMILY + subPosition;
    this->settings->setValue(
                key,
                family);
    this->subSettingsChanged();
    this->subSettingsChanged(subPosition);
}
//====================================
//====================================
const QString KEY_SUBCOLOR = "subColor";
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
const QString KEY_SUBSIZE = "subSize";
//====================================
SubSize
SettingsManagerSingleton::getSubSize(
        int subPosition){
    QString key = KEY_SUBSIZE + subPosition;
    SubSize defaultSize = Medium;
    SubSize size
            = (SubSize)this->settings->value(
                key,
                (int)defaultSize).toInt();
    return size;
}
//====================================
void SettingsManagerSingleton::setSubSize(
        int subPosition,
        SubSize size){
    QString key = KEY_SUBSIZE + subPosition;
    this->settings->setValue(
                key,
                (int)size);
    this->subSettingsChanged();
    this->subSettingsChanged(subPosition);
}
//====================================
const QString KEY_SUBPOSITION = "lang";
//====================================
SubPosition
SettingsManagerSingleton::getSubPosition(
        int subPosition){
    QString key = KEY_SUBPOSITION + subPosition;
    SubPosition defaultPosition = Bottom;
    SubPosition size
            = (SubPosition)this->settings->value(
                key,
                (int)defaultPosition).toInt();
    return size;
}
//====================================
void SettingsManagerSingleton::setSubPosition(
        int subPosition,
        SubPosition position){
    QString key = KEY_SUBPOSITION + subPosition;
    this->settings->setValue(
                key,
                (int)position);
    this->subSettingsChanged();
    this->subSettingsChanged(subPosition);
}
//====================================

}

