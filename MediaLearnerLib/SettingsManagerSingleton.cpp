#include "SettingsManagerSingleton.h"
#include <QDebug>
#include <QDir>


namespace ML{

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
        QSharedPointer<SettingsManagerSingleton>
                localInstance(
                    new SettingsManagerSingleton);
        SettingsManagerSingleton::instance = localInstance;
    }
    SettingsManagerSingleton *pointer
            = SettingsManagerSingleton::instance.data();
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
const QString KEY_DIRVIDEO = "dirvideo";
//====================================
QString SettingsManagerSingleton::getExtractedVideoPath(){
    QString defaultPath = QDir::currentPath();
    QString path
            = this->settings->value(
                KEY_DIRVIDEO, defaultPath).toString();
    return path;
}
//====================================
void SettingsManagerSingleton::setExtractedVideoPath(
        QString path){
    this->settings->setValue(KEY_DIRVIDEO, path);
}
//====================================
//====================================
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
const QString KEY_SUBPOSITION = "subposition";
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
const QString KEY_SORTSEQUENCESANDREMOVEINTERSECT = "sortSequences";
//====================================
bool SettingsManagerSingleton::isSortSequencesAndRemoveIntersect(){
    bool enabled
            = this->settings->value(
                KEY_SORTSEQUENCESANDREMOVEINTERSECT,
                true)
            .toBool();
    return enabled;
}
//====================================
void SettingsManagerSingleton::setSortSequencesAndRemoveIntersect(
        bool enabled){
    bool oldEnabled
            = this->isSortSequencesAndRemoveIntersect();
    this->settings->setValue(
                KEY_SORTSEQUENCESANDREMOVEINTERSECT,
                enabled);
    if(oldEnabled != enabled){
        this->sortSequencesAndRemoveIntersect(enabled);
    }
}
//====================================
const QString KEY_SAVEINLOGFILE = "saveInLogFile";
//====================================
bool SettingsManagerSingleton::isSaveInLogFile(){
    bool enabled
            = this->settings->value(
                KEY_SAVEINLOGFILE,
                true)
            .toBool();
    return enabled;
}
//====================================
void SettingsManagerSingleton::setSaveInLogFile(
        bool enabled){
    this->settings->setValue(
                KEY_SAVEINLOGFILE,
                enabled);
}
//====================================
const QString KEY_SENDLOGFILEWHENCRASHED = "sendLogFile";
//====================================
bool SettingsManagerSingleton::isSendLogFileWhenCrashed(){
    bool enabled
            = this->settings->value(
                KEY_SENDLOGFILEWHENCRASHED,
                false)
            .toBool();
    return enabled;
}
//====================================
void SettingsManagerSingleton::setSendLogFileWhenCrashed(
        bool enabled){
    this->settings->setValue(
                KEY_SENDLOGFILEWHENCRASHED,
                enabled);
}
//====================================
const QString KEY_ISLASPROFILEVIDEO = "isLastProfileVideo";
//====================================
bool SettingsManagerSingleton::isLastProfileVideo(){
    bool is
            = this->settings->value(
                KEY_ISLASPROFILEVIDEO,
                false)
            .toBool();
    return is;
}
//====================================
void SettingsManagerSingleton::setLastProfileVideo(
        bool is){
    this->settings->setValue(
                KEY_ISLASPROFILEVIDEO,
                is);
}
//====================================
const QString KEY_LASTPROFILENAME = "lastProfileName";
//====================================
QString SettingsManagerSingleton::getLastProfileName(){
    QString lastProfileName
            = this->settings->value(
                KEY_LASTPROFILENAME,
                "H.264 + acc (MP4)")
            .toString();
    return lastProfileName;
}
//====================================
void SettingsManagerSingleton::setLastProfileName(
        QString name){
    this->settings->setValue(
                KEY_LASTPROFILENAME,
                name);
}
//====================================

}

