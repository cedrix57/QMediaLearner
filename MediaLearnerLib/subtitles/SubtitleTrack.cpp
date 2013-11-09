#include "SubtitleTrack.h"

#include <QFile>

namespace MediaLearner{

//====================================
SubtitleTrack::SubtitleTrack(QObject *parent) :
    QObject(parent){
    this->_zeroTime
            = QTime(0, 0, 0, 0);
    this->subInfos
            = QSharedPointer<QList<SubtitleInfo> >(
                new QList<SubtitleInfo>);
}
//====================================
void SubtitleTrack::setDrawingSettings(
        DrawingSettings drawingSettings){
    *( (DrawingSettings *)&this->drawingText )
            = drawingSettings;
}
//====================================
void SubtitleTrack::parseSubtitleFileName(
        QString subtitleFilePath){
    this->subInfos->clear();
    if(subtitleFilePath.endsWith("srt")){
        this->parseSubtitleFileNameSrt(
                    subtitleFilePath);
    }
}
//====================================
void SubtitleTrack::parseSubtitleFileNameSrt(
        QString subtitleFilePath){
    QFile file(subtitleFilePath);
    if(file.open(QIODevice::ReadOnly)){
        QTextStream textStream(&file);
        while(!textStream.atEnd()){
            SubtitleInfo sub
                    = this->_getNextSubtitleInfoSrt(
                        textStream);
            *this->subInfos << sub;
        }
    }
}
//====================================
SubtitleInfo SubtitleTrack::_getNextSubtitleInfoSrt(
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
        subInfo.lines.append(line);
        line = textStream.readLine();
    }
    return subInfo;
}
//====================================
DrawingText SubtitleTrack::getText(
        qint64 positionInMs){
    int nTexts
            = this->subInfos->size();
    this->drawingText.clearLines();
    for(int i=0; i<nTexts; i++){
        SubtitleInfo sub
                = this->subInfos->at(i);
        if((sub.startPosition <= positionInMs)
                && (sub.endPosition >= positionInMs)){
            this->drawingText.setLines(
                    sub.lines);
            break;
        }
    }
    return this->drawingText;
}
//====================================

}

