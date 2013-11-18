#ifndef SUBTITLETRACK_H
#define SUBTITLETRACK_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QList>
#include <QSharedPointer>
#include <QTextStream>
#include <QTime>
#include "DrawingText.h"

namespace MediaLearner{

struct SubtitleInfo{
    qint64 startPosition;
    qint64 endPosition;
    QStringList lines;
};

struct DrawingSubtitleInfo{
    qint64 startPosition;
    qint64 endPosition;
    DrawingText text;
};

class SubtitleTrack : public QObject{
    Q_OBJECT
public:
    explicit SubtitleTrack(QObject *parent = 0);
    void setDrawingSettings(
            DrawingSettings drawingSettings);
    void parseSubtitleFileName(
            QString subtitleFilePath);
    void parseSubtitleFileNameSrt(
            QString subtitleFilePath);
    DrawingText getText(
            qint64 positionInMs);

    
signals:
    
public slots:
    
protected:
    QSharedPointer<QList<SubtitleInfo> >
        subInfos;
    QTime _zeroTime;
    SubtitleInfo _getNextSubtitleInfoSrt(
            QTextStream &textStream);
    DrawingText drawingText;
};

}

#endif // SUBTITLETRACK_H

