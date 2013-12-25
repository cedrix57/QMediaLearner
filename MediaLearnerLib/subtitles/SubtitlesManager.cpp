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
    qDebug() << "SubtitlesManager::SubtitlesManager() called";
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
    qDebug() << "SubtitlesManager::SubtitlesManager() end";
}
//====================================
void SubtitlesManager::_initDrawingSettings(){
    qDebug() << "void SubtitlesManager::_initDrawingSettings() called";
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
    qDebug() << "void SubtitlesManager::_initDrawingSettings() end";
}
//====================================
void SubtitlesManager::enableSubTrack(int position){
    this->setSubTrackEnabled(
                position,
                true);
}
//====================================
void SubtitlesManager::setSubTrackEnabled(
        int position,
        bool enabled){
    this->enabledTracks[position] = enabled;
    CrashManagerSingleton::getInstance()
            ->setSubtitleEnabled(position, enabled);
}
//====================================
void SubtitlesManager::disableSubTrack(int position){
    this->setSubTrackEnabled(
                position,
                false);
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
bool SubtitlesManager::isSubTrackEnabled(int position){
    return this->enabledTracks[position];
}
//====================================
void SubtitlesManager::setSubTrack(QString subtitleFilePath){
    this->setSubTrack(0, subtitleFilePath);
}
//====================================
SubtitleTrack *SubtitlesManager::getSubtitleTracks(){
    return this->subtitleTracks;
}
//====================================
void SubtitlesManager::setSubTrack(
        int position,
        QString subtitleFilePath){
    qDebug() << "void SubtitlesManager::setSubTrack(...) called";
    this->subtitleTracks[position]
            .parseSubtitleFileName(
                subtitleFilePath);
    this->enabledTracks[position] = true;
    this->setSubtitleShift(
                position,
                0);
    CrashManagerSingleton::getInstance()
            ->setSubtitle(position, subtitleFilePath);
    qDebug() << "void SubtitlesManager::setSubTrack(...) end";
}
//====================================
QList<SubSequenceDrawable> SubtitlesManager::getSubsAt(
        qint64 positionInMs,
        QSize screenSize){
    qDebug() << "QList<SubSequenceDrawable> SubtitlesManager::getSubsAt(...) called";
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
    qDebug() << "QList<SubSequenceDrawable> SubtitlesManager::getSubsAt(...) end";
    return drawingTexts;
}
//====================================
void SubtitlesManager::setContext(
        QList<SubSequenceDrawable>
        &subSequencesDrawable,
        QSize screenSize){
    qDebug() << "void SubtitlesManager::setContext(...) called";
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
    qDebug() << "void SubtitlesManager::setContext(...) end";
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
