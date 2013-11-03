#include "SubtitlesManager.h"

namespace MediaLearner{

//====================================
SubtitlesManager::SubtitlesManager(QObject *parent) :
    QObject(parent){
    this->_zeroTime
            = QTime(0, 0, 0, 0);
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
    QFile file(subtitleFilePath);
    if(file.open(QIODevice::ReadOnly)){
        QTextStream textStream(&file);
        while(!textStream.atEnd()){
            SubtitleInfo sub
                    = this->_getNextSubtitleInfo(
                        textStream);
            this->texts[position]
                    .append(sub);
        }
        this->enabledTracks[position] = true;
    }
}
//====================================
SubtitleInfo SubtitlesManager::_getNextSubtitleInfo(
        QTextStream &textStream){
    QString line = textStream.readLine();
    while(line == "\n"){
        line = textStream.readLine();
    }
    SubtitleInfo subInfo;
    line = textStream.readLine();
    QStringList times = line.split(" --> ");
    QTime startTime = QTime::fromString(times.at(0));
    QTime endTime = QTime::fromString(times.at(1));
    subInfo.startPosition
            = this->_zeroTime
            .msecsTo(startTime);
    subInfo.endPosition
            = this->_zeroTime
            .msecsTo(endTime);
    line = textStream.readLine();
    while(line != ""){
        subInfo.texts.append(line);
        line = textStream.readLine();
    }
    return subInfo;
}
//====================================
QList<DrawingText> SubtitlesManager::getTexts(
        qint64 positionInMs){
    QList<DrawingText> drawingTexts;
    for(int i=0; i<N_MAX_TRACKS; i++){
        if(this->enabledTracks[i]){
            drawingTexts.
                    append(
                        this->getText(
                            positionInMs,
                            i));
        }
    }
    return drawingTexts;
}
//====================================
DrawingText SubtitlesManager::getText(
        qint64 positionInMs,
        int trackPosition){
    int nTexts
            = this->texts[trackPosition].size();
    this->drawingTexts[trackPosition]
            .texts.clear();
    for(int i=0; i<nTexts; i++){
        SubtitleInfo &sub
                = this->texts[trackPosition][i];
        if((sub.startPosition <= positionInMs)
                && (sub.endPosition >= positionInMs)){
            this->drawingTexts[trackPosition]
                    .texts
                    = sub.texts;
            break;
        }
    }
    //TODO eval rect
    return this->drawingTexts[trackPosition];
}
//====================================

}
