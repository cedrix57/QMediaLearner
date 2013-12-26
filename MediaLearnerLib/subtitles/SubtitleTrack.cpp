#include "SubtitleTrack.h"

#include <QFile>
#include <QDebug>

namespace ML{

//====================================
SubtitleTrack::SubtitleTrack(QObject *parent) :
    QObject(parent){
    qDebug() << "SubtitleTrack::SubtitleTrack(...) called";
    this->_zeroTime
            = QTime(0, 0, 0, 0);
    this->subInfos
            = QSharedPointer<QList<SubtitleInfo> >(
                new QList<SubtitleInfo>);
    qDebug() << "SubtitleTrack::SubtitleTrack(...) end";
}
//====================================
void SubtitleTrack::setDrawingSettings(
        DrawingSettings &drawingSettings){
    this->currentDrawableText.setDrawingSettings(
                drawingSettings);
}
//====================================
void SubtitleTrack::parseSubtitleFileName(
        QString subtitleFilePath){
    qDebug() << "void SubtitleTrack::parseSubtitleFileName(...) called";
    this->subInfos->clear();
    if(subtitleFilePath.endsWith("srt")){
        this->parseSubtitleFileNameSrt(
                    subtitleFilePath);
    }
    qDebug() << "void SubtitleTrack::parseSubtitleFileName(...) end";
}
//====================================
void SubtitleTrack::parseSubtitleFileNameSrt(
        QString subtitleFilePath){
    qDebug() << "void SubtitleTrack::parseSubtitleFileNameSrt(...) called";
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
    qDebug() << "void SubtitleTrack::parseSubtitleFileNameSrt(...) end";
}
//====================================
SubtitleInfo SubtitleTrack::_getNextSubtitleInfoSrt(
        QTextStream &textStream){
    qDebug() << "SubtitleInfo SubtitleTrack::_getNextSubtitleInfoSrt(...) called";
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
    qDebug() << "SubtitleInfo SubtitleTrack::_getNextSubtitleInfoSrt(...) end";
    return subInfo;
}
//====================================
SubSequenceDrawable SubtitleTrack::getText(
        qint64 positionInMs){
    qDebug() << "SubSequenceDrawable SubtitleTrack::getText(...) called";
    int nTexts
            = this->subInfos->size();
    this->currentDrawableText.clearLines();
    for(int i=0; i<nTexts; i++){
        SubtitleInfo sub
                = this->subInfos->at(i);
        if((sub.startPosition <= positionInMs)
                && (sub.endPosition >= positionInMs)){
            this->currentDrawableText.setLines(
                    sub.lines);
            break;
        }
    }
    qDebug() << "SubSequenceDrawable SubtitleTrack::getText(...) end";
    return this->currentDrawableText;
}
//====================================
QList<SubSequenceDrawable> SubtitleTrack::getTexts(
        Sequence intervalInMs){
    qDebug() << "QList<SubSequenceDrawable> SubtitleTrack::getTexts(...) called";
    QList<SubSequenceDrawable> drawableTexts;
    for(QList<SubtitleInfo>::iterator sub
        = this->subInfos->begin();
        sub != this->subInfos->end();
        ++sub){
        SubtitleInfo tmp = *sub;
        this->currentDrawableText.beginInMs
                = sub->startPosition;
        this->currentDrawableText.endInMs
                = sub->endPosition;
        bool inIntersection
                = intervalInMs.intersect(
                    this->currentDrawableText);
        if(inIntersection){
            this->currentDrawableText.clearLines();
            this->currentDrawableText.setLines(
                    sub->lines);
            drawableTexts << this->currentDrawableText;
        }
    }
    qDebug() << "QList<SubSequenceDrawable> SubtitleTrack::getTexts(...) end";
    return drawableTexts;
}
//====================================

}

