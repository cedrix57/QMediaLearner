#ifndef DRAWINGTEXT_H
#define DRAWINGTEXT_H

#include <QStringList>
#include <QFont>
#include <QColor>
#include <QRect>

namespace MediaLearner{

struct DrawingSettings{
    QFont font;
    QColor fontColor;
    DrawingSettings(){
        this->fontColor = Qt::yellow;
    }
    DrawingSettings &operator=(const DrawingSettings &other){
        this->font = other.font;
        this->fontColor = other.fontColor;
        return *this;
    }
};


class DrawingText{
    public:
    DrawingText();
    const DrawingSettings & getDrawingSettings();
    void setDrawingSettings(
            DrawingSettings &drawingSettings);
    QRect getRect();
    const QStringList & getLines();
    void setLines(QStringList &lines);
    void clearLines();
    void assessSizes(
            QSize parentSize,
            int bottomCoord);

    protected:
    DrawingSettings drawingSettings;
    QStringList lines;
    QStringList originalLines;
    QRect rect;
    void _assessMaxWidth(
        int &maxWidth,
        QFontMetrics &fontMetrics,
        QString &phrase);
};

/*
class DrawingText
{
public:
    DrawingText();
};
//*/
}

#endif // DRAWINGTEXT_H
