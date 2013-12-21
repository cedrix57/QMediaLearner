#include "SubtitlesManager.h"

#include <QFontMetrics>
#include <QDebug>
#include <QSet>

#include "../SettingsManagerSingleton.h"
#include "CrashManagerSingleton.h"
namespace ML{

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
        DrawingSettings drawingSettings;
        drawingSettings.textColor
                = settingsManager
                ->getSubColor(i);
        drawingSettings.fontFamily
                = settingsManager
                ->getSubFontFamily(i);
        drawingSettings.subPosition
                = settingsManager
                ->getSubPosition(i);
        drawingSettings.subSize
                = settingsManager
                ->getSubSize(i);
        this->subtitleTracks[i].
                setDrawingSettings(
                    drawingSettings);
    }
}
//====================================
void SubtitlesManager::enableTrack(int position){
    this->enabledTracks[position] = true;
    CrashManagerSingleton::getInstance()
            ->setSubtitleEnabled(position, true);
}
//====================================
void SubtitlesManager::setSubtitleShift(
        int position,
        int shiftInMs){
    this->trackShifts[position] = shiftInMs;
    CrashManagerSingleton::getInstance()
            ->setSubtitleShift(
                position,
                shiftInMs);
}
//====================================
void SubtitlesManager::disableTrack(int position){
    this->enabledTracks[position] = false;
    CrashManagerSingleton::getInstance()
            ->setSubtitleEnabled(position, false);
}
//====================================
bool SubtitlesManager::isSubTrackEnabled(int position){
    return this->enabledTracks[position];
}
//====================================
void SubtitlesManager::setTrack(QString subtitleFilePath){
    this->setTrack(subtitleFilePath, 0);
}
//====================================
SubtitleTrack *SubtitlesManager::getSubtitleTracks(){
    return this->subtitleTracks;
}
//====================================
void SubtitlesManager::setTrack(
        QString subtitleFilePath,
        int position){
    this->subtitleTracks[position]
            .parseSubtitleFileName(
                subtitleFilePath);
    this->enabledTracks[position] = true;
    this->setSubtitleShift(
                position,
                0);
    CrashManagerSingleton::getInstance()
            ->setSubtitle(position, subtitleFilePath);
}
//====================================
QList<SubSequenceDrawable> SubtitlesManager::getSubsAt(
        qint64 positionInMs,
        QSize screenSize){
    QList<SubSequenceDrawable> drawingTexts;
    int phrasesSpacing = screenSize.height() * 0.04;
    int firstCoords[3];
    firstCoords[0] = phrasesSpacing;
    firstCoords[1] = 0;
    firstCoords[2] = phrasesSpacing;
    for(int i=0; i<N_MAX_TRACKS; i++){
        if(this->enabledTracks[i]){
            //TODO better height calculation
            SubSequenceDrawable drawingText
                    = this->subtitleTracks[i]
                    .getText(positionInMs);
            drawingTexts << drawingText;
        }
    }
    SubtitlesManager::setContext(
                drawingTexts,
                screenSize);
    return drawingTexts;
}
//====================================
void SubtitlesManager::setContext(
        QList<SubSequenceDrawable>
        &subSequencesDrawable,
        QSize screenSize){
    int phrasesSpacing = screenSize.height() * 0.04;
    int firstCoords[3];
    firstCoords[0] = phrasesSpacing;
    firstCoords[1] = 0;
    firstCoords[2] = phrasesSpacing;
    for(QList<SubSequenceDrawable>::iterator it
        = subSequencesDrawable.begin();
        it != subSequencesDrawable.end();
        ++it){
        ML::DrawingSettings
                drawingSettings
                = it->getDrawingSettings();
        ML::SubPosition position
                = drawingSettings.subPosition;
        int positionIndex = 0;
        if(position == ML::Center){
            positionIndex = 1;
        }else if(position == ML::Top){
            positionIndex = 2;
        }
        int firstCoord = firstCoords[positionIndex];
        it->setContext(
                    screenSize,
                    firstCoord);
        int heightLines = it->getHeightLines();
        firstCoords[positionIndex]
                += heightLines
                + phrasesSpacing;
    }
}
/*
//====================================
QList<QList<DrawingSubtitleInfo> >
SubtitlesManager::getSubsAt(
        QSharedPointer<QList<Sequence> >
        sequences,
        QSize screenSize){
    QList<QList<DrawingSubtitleInfo> >
            drawingSubtitleInfosList;
    QSet<long> boundaries;
    foreach(Sequence sequence, *sequences){
        boundaries << sequence.minInMs;
        boundaries << sequence.maxInMs;
    }
    QList<long> sortedBoundaries
            = QList<long>::fromSet(boundaries);
    qSort(sortedBoundaries);
    for(QList<long>::iterator it
        =sortedBoundaries.begin() + 1;
        it != sortedBoundaries.end();
        ++it){
        long begin = *(it-1);
        long end = *it;
        long middle = begin + (end - begin)/2;
        if(middle == begin){
            middle++;
        }
        QList<SubSequenceDrawable> drawingTexts
                = this->getSubsAt(
                    middle,
                    screenSize);
        QList<DrawingSubtitleInfo>
                drawingSubtitleInfos;
        foreach(SubSequenceDrawable drawingText,
                drawingTexts){
            int nLines = drawingText.nLines();
            if(nLines > 0){
                DrawingSubtitleInfo info;
                info.startPosition = begin;
                info.endPosition = end;
                info.text = drawingText;
                drawingSubtitleInfos << info;
            }
        }
        if(!drawingSubtitleInfos.isEmpty()){
            drawingSubtitleInfosList
                    << drawingSubtitleInfos;
        }
    }
    return drawingSubtitleInfosList; //TODO shared pointer?
}
//====================================
//*/
}
