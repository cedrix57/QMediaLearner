#ifndef SUBSEQUENCE_H
#define SUBSEQUENCE_H

#include "medialearnerlib_global.h"
#include "../Utils/Sequence.h"
#include <QStringList>
#include <QColor>
#include <QPoint>
#include <QSize>
#include <QList>

namespace ML{

enum MEDIALEARNERLIBSHARED_EXPORT
        SubSize{Small, Medium, Big};
enum MEDIALEARNERLIBSHARED_EXPORT
        SubPosition{Top, Center, Bottom};

//====================================
struct MEDIALEARNERLIBSHARED_EXPORT
        SubSequence : Sequence{
public:
    SubSequence();
    QStringList lines;
};
//====================================
//====================================
struct MEDIALEARNERLIBSHARED_EXPORT
        FittedLine{
    QPoint position;
    QString text;
};
//====================================
//====================================
struct MEDIALEARNERLIBSHARED_EXPORT
        DrawingSettings{
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
class MEDIALEARNERLIBSHARED_EXPORT
        SubSequenceDrawable : public SubSequence{
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
    int priority;
    bool operator<(const SubSequenceDrawable &other) const;
    Sequence toSequence() const;

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
