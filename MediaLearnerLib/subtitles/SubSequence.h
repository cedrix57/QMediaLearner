#ifndef SUBSEQUENCE_H
#define SUBSEQUENCE_H

#include "../Utils/Sequence.h"
#include <QStringList>
#include <QColor>
#include <QPoint>
#include <QSize>
#include <QList>

namespace ML{

enum SubSize{Small, Medium, Big};
enum SubPosition{Top, Center, Bottom};

//====================================
struct SubSequence : Sequence{
public:
    SubSequence();
    QStringList lines;
};
//====================================
//====================================
struct FittedLine{
    QPoint position;
    QString text;
};
//====================================
//====================================
struct DrawingSettings{
    QColor textColor;
    QString fontFamily;
    SubSize subSize;
    SubPosition subPosition;
    int getFontSize(int screenHeight);
    ~DrawingSettings(){
    }
};
//====================================
//====================================
class SubSequenceDrawable : public SubSequence{
    public:
    SubSequenceDrawable();
    int getPixelSize();
    QColor getColor();
    void setDrawingSettings(
            DrawingSettings &drawingSettings);
    DrawingSettings getDrawingSettings();
    void upText(int shift);
    void downText(int shift);
    void clearLines();
    void setContext(
            QSize screenSize,
            int firstCoord);
    QList<FittedLine> getFittedLines();
    int getHeightLines();
    int getNLines();
    void setLines(QStringList &lines);

    protected:
    DrawingSettings drawingsSettings;
    QSize parentSize;
    int firstCoord;
    int fontSize;
    int heightLines;
    QList<FittedLine> fittedLines;
    void _assessFontSize(
            int screenHeight);
    void _addFittedLine(
            QString &line);
    void _assessFittedLineText(
            int screenWidth);
    void _assessFittedLinePosition(
            QSize screenSize,
            int firstCoord);

};
//====================================

}

#endif // SUBSEQUENCE_H
