#include "SubtitlesManager.h"
#include "../SettingsManagerSingleton.h"

#include <QFontMetrics>
#include <QDebug>
namespace MediaLearner{

//====================================
SubtitlesManager::SubtitlesManager(QObject *parent) :
    QObject(parent){
    for(int i=0; i<N_MAX_TRACKS; i++){
        this->enabledTracks[i] = false;
    }
    SettingsManagerSingleton
            *settingsManager
            = SettingsManagerSingleton::getInstance();
    this->connect(
                settingsManager,
                SIGNAL(subSettingsChanged()),
                SLOT(_initDrawingSettings()));
    this->_initDrawingSettings();
}
//====================================
void SubtitlesManager::_initDrawingSettings(){
    SettingsManagerSingleton
            *settingsManager
            = SettingsManagerSingleton::getInstance();
    for(int i=0; i<N_MAX_TRACKS; i++){
        QString defaultFamily
                = settingsManager
                ->getSubFontFamily(i);
        QFont defaultFont;
        defaultFont.setFamily(defaultFamily);
        qDebug() << "Default font: " << defaultFont.family();
        QColor defaultColor
                = settingsManager
                ->getSubColor(i);
        qDebug() << "Default color: " << defaultColor.toRgb();
        DrawingSettings drawingSettings;
        drawingSettings.font
                = defaultFont;
        drawingSettings.fontColor
                = defaultColor;
        this->subtitleTracks[i].
                setDrawingSettings(
                    drawingSettings);
    }
}
//====================================
void SubtitlesManager::enableTrack(int position){
    this->enabledTracks[position] = true;
}
//====================================
void SubtitlesManager::disableTrack(int position){
    this->enabledTracks[position] = false;
}
//====================================
void SubtitlesManager::setTrack(QString subtitleFilePath){
    this->setTrack(subtitleFilePath, 0);
}
//====================================
void SubtitlesManager::setTrack(
        QString subtitleFilePath,
        int position){
    this->subtitleTracks[position]
            .parseSubtitleFileName(
                subtitleFilePath);
    this->enabledTracks[position] = true;
}
//====================================
QList<DrawingText> SubtitlesManager::getTexts(
        qint64 positionInMs,
        QSize screenSize){
    QList<DrawingText> drawingTexts;
    int phrasesSpacing = screenSize.height() * 0.04;
    int bottomCoord = phrasesSpacing;
    for(int i=0; i<N_MAX_TRACKS; i++){
        if(this->enabledTracks[i]){
            DrawingText drawingText
                    = this->subtitleTracks[i]
                    .getText(positionInMs);
            drawingText.assessSizes(
                        screenSize,
                        bottomCoord);
            drawingTexts.
                    append(
                        drawingText);
            bottomCoord
                    += drawingText.getRect().height()
                    + phrasesSpacing;
        }
    }
    return drawingTexts;
}

}
