#include "DrawingText.h"

#include <QFontMetrics>

namespace MediaLearner{
//====================================
DrawingText::DrawingText(){
}
//====================================
const DrawingSettings & DrawingText::getDrawingSettings(){
    return this->drawingSettings;
}
//====================================
void DrawingText::setDrawingSettings(
        DrawingSettings &drawingSettings){
    this->drawingSettings = drawingSettings;
}
//====================================
QRect DrawingText::getRect(){
    return this->rect;
}
//====================================
const QStringList & DrawingText::getLines(){
    return this->lines;
}
//====================================
void DrawingText::setLines(
        QStringList &lines){
    this->lines = lines;
    this->originalLines = lines;
}
//====================================
void DrawingText::clearLines(){
    this->lines.clear();
    this->originalLines.clear();
}
//====================================
void DrawingText::assessSizes(
        QSize parentSize,
        int bottomCoord){
    //init temp var
    int height = parentSize.height();
    int fontSize = height * 0.0638 + 0.5;
    this->drawingSettings.font.setPixelSize(fontSize);
    int width = parentSize.width();
    QFontMetrics fontMetrics(this->drawingSettings.font);
    this->lines.clear();
    int maxWidth = 0;
    //Make too long line shorter
    foreach(QString line, this->originalLines){
        int lineWidth
                = fontMetrics.width(line);
        lineWidth++;
        int maxPhraseLength = 0.95*width + 0.5;
        if(lineWidth > maxPhraseLength){
            QStringList words
                    = line.split(" ");
            QString currentPhrase = words.takeFirst();
            foreach(QString word, words){
                int futurLength
                        = fontMetrics.width(currentPhrase)
                        + fontMetrics.width(word);
                if(futurLength > maxPhraseLength){
                    this->lines << currentPhrase;
                    this->_assessMaxWidth(
                                maxWidth,
                                fontMetrics,
                                currentPhrase);
                    currentPhrase.clear();
                }
                currentPhrase += " " + word;
            }
            this->lines << currentPhrase;
            this->_assessMaxWidth(
                        maxWidth,
                        fontMetrics,
                        currentPhrase);
        }else{
            this->lines << line;
            this->_assessMaxWidth(
                        maxWidth,
                        fontMetrics,
                        line);
        }
    }
    //Center the texts
    int nLines = this->lines.size();
    if(nLines > 1){
        int spaceWidth
                = fontMetrics.width(" ");
        for(QStringList::Iterator it
            = this->lines.begin();
            it != this->lines.end();
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
    //Assess rect
    int lineSpacing = fontMetrics.lineSpacing();
    int heightTexts
            = nLines * lineSpacing;
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
}
