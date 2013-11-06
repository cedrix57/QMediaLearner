#ifndef SUBTITLESMANAGER_H
#define SUBTITLESMANAGER_H

#include <QObject>
#include <QFont>
#include <QColor>
#include <QFile>
#include <QTextStream>
#include <QRect>
#include <QTime>

#define N_MAX_SUBTRACK 3

namespace MediaLearner{

struct DrawingSettings{
    QFont font;
    QColor fontColor;
    DrawingSettings(){
        this->fontColor = Qt::yellow;
    }
};

struct SubtitleInfo{
    qint64 startPosition;
    qint64 endPosition;
    QStringList texts;
};

struct DrawingText : DrawingSettings{
    DrawingText()
        : DrawingSettings(){
    }
    QStringList texts;
    QRect rect;
    void assessSizes(
            QSize parentSize,
            int bottomCoord);
    void _assessMaxWidth(
        int &maxWidth,
        QFontMetrics &fontMetrics,
        QString &phrase);
};

class SubtitlesManager : public QObject{
    Q_OBJECT
public:
    static const int N_MAX_TRACKS = 5;
    explicit SubtitlesManager(QObject *parent = 0);
    int getNTracks();
    void setTrack(QString subtitleFilePath);
    void setTrack(QString subtitleFilePath, int position);
    //TODO feature to enable/disable track
    void enableTrack(int position);
    void disableTrack(int position);
    void setDrawingSettings(
            int position,
            DrawingSettings &drawingSettings);
    DrawingSettings getDrawingSettings(
            int position);
    QList<DrawingText> getTexts(
            qint64 positionInMs,
            QSize screenSize);

signals:
    
public slots:

protected:
    QList<DrawingSettings> drawingSettings;
    bool enabledTracks[N_MAX_SUBTRACK];
    DrawingText drawingTexts[N_MAX_SUBTRACK];
    QMap<int, QList<SubtitleInfo> > texts;
    SubtitleInfo _getNextSubtitleInfo(
            QTextStream &textStream);
    QTime _zeroTime;
    DrawingText getText(
            qint64 positionInMs,
            int trackPosition);
};

}
#endif // SUBTITLESMANAGER_H
