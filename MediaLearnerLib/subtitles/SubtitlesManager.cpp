#include "SubtitlesManager.h"

#include <QFontMetrics>
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
        qint64 positionInMs,
        QSize screenSize){
    QList<DrawingText> drawingTexts;
    int phrasesSpacing = screenSize.height() * 0.04;
    int bottomCoord = phrasesSpacing;
    for(int i=0; i<N_MAX_TRACKS; i++){
        if(this->enabledTracks[i]){
            DrawingText drawingText
                    = this->getText(
                        positionInMs,
                        i);
            drawingText.assessSizes(
                        screenSize,
                        bottomCoord);
            drawingTexts.
                    append(
                        drawingText);
            bottomCoord
                    += drawingText.rect.height()
                    + phrasesSpacing;
        }
    }
    return drawingTexts;
}
//====================================
void DrawingText::assessSizes(
        QSize parentSize,
        int bottomCoord){
    int height = parentSize.height();
    int fontSize = height * 0.0638 + 0.5;
    this->font.setPixelSize(fontSize);
    int width = parentSize.width();
    QFontMetrics fontMetrics(this->font);
    QStringList oldTexts = this->texts;
    this->texts.clear();
    int maxWidth = 0;
    foreach(QString text, oldTexts){
        int textWidth
                = fontMetrics.width(text);
        textWidth++;
        int maxPhraseLength = 0.95*width + 0.5;
        if(textWidth > maxPhraseLength){
            QStringList words
                    = text.split(" ");
            QString currentPhrase = words.takeFirst();
            foreach(QString word, words){
                int futurLength
                        = fontMetrics.width(currentPhrase)
                        + fontMetrics.width(word);
                if(futurLength > maxPhraseLength){
                    this->texts << currentPhrase;
                    this->_assessMaxWidth(
                                maxWidth,
                                fontMetrics,
                                currentPhrase);
                    currentPhrase.clear();
                }
                currentPhrase += " " + word;
            }
            this->texts << currentPhrase;
            this->_assessMaxWidth(
                        maxWidth,
                        fontMetrics,
                        currentPhrase);
        }else{
            this->texts << text;
            this->_assessMaxWidth(
                        maxWidth,
                        fontMetrics,
                        text);
        }
    }
    int nTexts = this->texts.size();
    if(nTexts > 1){
        int spaceWidth
                = fontMetrics.width(" ");
        for(QStringList::Iterator it
            = this->texts.begin();
            it != this->texts.end();
            ++it){
                int currentWidth
                        = fontMetrics.width(
                            *it);
                if(currentWidth < maxWidth){
                    int halfShift
                            = (maxWidth - currentWidth)
                            / 2;
                    int nSpaces = halfShift / spaceWidth;
                    for(int i=0; i<nSpaces; i++){
                        *it = " " + *it;
                    }
                }
        }
    }
    int lineSpacing = fontMetrics.lineSpacing();
    int heightTexts
            = nTexts * lineSpacing;
    int bottomShift = height * 0.035 + 0.5;
    int top = height
            - bottomCoord
            - bottomShift
            - heightTexts;
    int left = (width - maxWidth) / 2;
    this->rect = QRect(
                left,
                top,
                maxWidth,
                heightTexts);
}
//====================================
void DrawingText::_assessMaxWidth(
        int &maxWidth,
        QFontMetrics &fontMetrics,
        QString &phrase){
    int currentWidth
            = fontMetrics.width(
                phrase);
    maxWidth = qMax(
                currentWidth,
                maxWidth);
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
