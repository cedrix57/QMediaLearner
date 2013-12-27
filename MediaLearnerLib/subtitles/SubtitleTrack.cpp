#include "SubtitleTrack.h"

#include <QFile>
#include <QTextStream>
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
void SubtitleTrack::saveSubtitle(
        QString subtitleFilePath){
    QFile file(subtitleFilePath);
    file.open(QIODevice::WriteOnly);
    QTextStream stream(&file);
    int currentN = 1;
    for(QList<SubtitleInfo>::iterator it
        = this->subInfos->begin();
        it != this->subInfos->end();
        ++it){
        stream << QString::number(currentN) << endl;
        QTime startTime(0, 0, 0, 0);
        startTime
                = startTime
                .addMSecs(it->startPosition);
        QTime endTime(0, 0, 0, 0);
        endTime
                = endTime
                .addMSecs(it->endPosition);
        QString startTimeStrimg
                = startTime.toString(
                    "hh:mm:ss,zzz");
        QString endTimeStrimg
                = endTime.toString(
                    "hh:mm:ss,zzz");
        stream << startTimeStrimg
                  << " --> "
                  << endTimeStrimg
                  << endl;
        foreach(QString line, it->lines){
            stream << line << endl;
        }
        stream << endl;
        currentN++;
    }
    file.close();
    //TODO
}
//====================================
void SubtitleTrack::saveSubtitle(
        QString subtitleFilePath,
        QList<Sequence> sequences){
    QFile file(subtitleFilePath);
    file.open(QIODevice::WriteOnly);
    QTextStream stream(&file);
    int currentN = 1;
    int totalLength = 0;
    foreach(Sequence sequence, sequences){
        int toRemove = sequence.beginInMs - totalLength;
        QList<SubSequenceDrawable>
                subSequencesDrawable
                = this->getTexts(
                    sequence);
        foreach(SubSequenceDrawable seq, subSequencesDrawable){
            seq.substract(toRemove);
            stream << QString::number(currentN) << endl;
            QTime startTime(0, 0, 0, 0);
            startTime
                    = startTime
                    .addMSecs(seq.beginInMs);
            QTime endTime(0, 0, 0, 0);
            endTime
                    = endTime
                    .addMSecs(seq.endInMs);
            QString startTimeStrimg
                    = startTime.toString(
                        "hh:mm:ss,zzz");
            QString endTimeStrimg
                    = endTime.toString(
                        "hh:mm:ss,zzz");
            stream << startTimeStrimg
                      << " --> "
                      << endTimeStrimg
                      << endl;
            foreach(QString line, seq.lines){
                stream << line << endl;
            }
            stream << endl;
            currentN++;
        }
        int sequenceLength
                = sequence.getDuration();
        totalLength += sequenceLength;
    }
    file.close();
}
//====================================

}

