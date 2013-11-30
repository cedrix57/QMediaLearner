#include "SubSequence.h"

#include <QFont>
#include <QFontMetrics>

namespace MediaLearner{

//====================================
SubSequence::SubSequence(){
}
//====================================
int DrawingSettings::getFontSize(int screenHeight){
    float heightFontFactor = 0.0638;
    if(this->subSize == Small){
        heightFontFactor = 0.04;
    }else if(this->subSize == Big){
        heightFontFactor = 0.1;
    }
    int fontSize
            = screenHeight * heightFontFactor + 0.5;
    return fontSize;
}
//====================================
SubSequenceDrawable::SubSequenceDrawable(){
}
//====================================
int SubSequenceDrawable::getPixelSize(){
    return this->fontSize;
}
//====================================
QColor SubSequenceDrawable::getColor(){
    return this->drawingsSettings.colorText;
}
//====================================
void SubSequenceDrawable::setDrawingSettings(
        DrawingSettings &drawingsSettings){
    this->drawingsSettings = drawingsSettings;
}
//====================================
DrawingSettings SubSequenceDrawable::getDrawingSettings(){
    return this->drawingsSettings;
}
//====================================
void SubSequenceDrawable::upText(int shift){
    for(QList<FittedLine>::iterator it
        = this->fittedLines.begin();
        it != this->fittedLines.end();
        ++it){
        int newY = it->position.y() - shift;
        it->position.setY(newY);
    }
}
//====================================
void SubSequenceDrawable::downText(int shift){
    for(QList<FittedLine>::iterator it
        = this->fittedLines.begin();
        it != this->fittedLines.end();
        ++it){
        int newY = it->position.y() + shift;
        it->position.setY(newY);
    }
}
//====================================
void SubSequenceDrawable::setContext(
        QSize screenSize,
        int firstCoord){
    this->fittedLines.clear();
    int screenHeight = screenSize.height();
    this->_assessFontSize(
                screenHeight);
    int screenWidth = screenSize.width();
    //TODO Fixed the two following algos
    this->_assessFittedLineText(
                screenWidth);
    this->_assessFittedLinePosition(
                screenSize,
                firstCoord);
}
//====================================
void SubSequenceDrawable::_assessFontSize(
        int screenHeight){
    this->fontSize
            = this->drawingsSettings.getFontSize(screenHeight);
}

//====================================
void SubSequenceDrawable::_assessFittedLineText(
        int screenWidth){
    int maxPhraseLength = 0.9*screenWidth + 0.5;
    QFont font(
                this->drawingsSettings.fontFamily,
                this->fontSize);
    QFontMetrics fontMetrics(font);
    foreach(QString line, this->lines){
        int lineWidth
                = fontMetrics.width(line);
        lineWidth++;
        if(lineWidth > maxPhraseLength){
            QStringList words
                    = line.split(" ");
            QString currentPhrase = words.takeFirst();
            foreach(QString word, words){
                int futurLength
                        = fontMetrics.width(currentPhrase)
                        + fontMetrics.width(word);
                if(futurLength > maxPhraseLength){
                    this->_addFittedLine(currentPhrase);
                    currentPhrase.clear();
                }
                currentPhrase += " " + word;
            }
            this->_addFittedLine(currentPhrase);
        }else{
            this->_addFittedLine(line);
        }
    }
}
//====================================
void SubSequenceDrawable::_addFittedLine(
        QString &line){
    FittedLine fittedLine;
    fittedLine.text = line;
    this->fittedLines << fittedLine;
}
//====================================
void SubSequenceDrawable::_assessFittedLinePosition(
        QSize screenSize,
        int firstCoord){
    int screenWidth = screenSize.width();
    int screenHeight = screenSize.height();
    //int bottomShift = screenHeight * 0.02 + 0.5;
    //firstCoord += bottomShift;
    QFont font(
                this->drawingsSettings.fontFamily,
                this->fontSize);
    QFontMetrics fontMetrics(font);
    //int lineSpacing2 = fontMetrics.lineSpacing();
    //int leading = fontMetrics.leading();
    //int height = fontMetrics.height();
    //int ascent = fontMetrics.ascent();
    //int descent = fontMetrics.descent();
    //int lineSpacing = qMin(ascent, descent) * 2 + 1;
    int lineSpacing = fontMetrics.lineSpacing();
    int y = firstCoord;
    int heightLines = this->fittedLines.size() * lineSpacing;
    if(this->drawingsSettings.subPosition == Bottom){
        y = screenHeight - heightLines - firstCoord - lineSpacing;
    }else if(this->drawingsSettings.subPosition == Center){
        y = screenHeight/2 - firstCoord;
    }
    int xShift = fontMetrics.width(" ");
    int xShift2 = xShift - (fontMetrics.width("  ") - xShift);
    for(QList<FittedLine>::iterator it
        = this->fittedLines.begin();
        it != this->fittedLines.end();
        ++it){
        QString line = it->text;
        int lineWidth
                = fontMetrics.width(line);
        int x = (screenWidth - lineWidth - xShift) / 2;
        y += lineSpacing;
        it->position.setX(x);
        it->position.setY(y);
    }
}
//====================================
QList<FittedLine> SubSequenceDrawable::getFittedLines(){
    return this->fittedLines;
}
//====================================
int SubSequenceDrawable::getHeightLines(){
    QFont font(
                this->drawingsSettings.fontFamily,
                this->fontSize);
    QFontMetrics fontMetrics(font);
    int lineSpacing = fontMetrics.lineSpacing();
    int nLines = this->getNLines();
    int heightLines = nLines * lineSpacing;
    return heightLines;
}
//====================================
int SubSequenceDrawable::getNLines(){
    int nLines = this->fittedLines.size();
    return nLines;
}
//====================================
void SubSequenceDrawable::clearLines(){
    this->lines.clear();
    this->fittedLines.clear();
}
//====================================
void SubSequenceDrawable::setLines(
        QStringList &lines){
    this->lines = lines;
}
//====================================
//====================================
//====================================

}
