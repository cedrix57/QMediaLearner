#include "SubtitlesManager.h"

#include <QFontMetrics>
namespace MediaLearner{

//====================================
SubtitlesManager::SubtitlesManager(QObject *parent) :
    QObject(parent){
    for(int i=0; i<N_MAX_TRACKS; i++){
        this->enabledTracks[i] = false;
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
