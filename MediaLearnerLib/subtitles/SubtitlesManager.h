#ifndef SUBTITLESMANAGER_H
#define SUBTITLESMANAGER_H

#include <QObject>
#include "SubtitleTrack.h"

#define N_MAX_SUBTRACK 3

namespace MediaLearner{

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
    SubtitleTrack subtitleTracks[N_MAX_SUBTRACK];
    DrawingText getText(
            qint64 positionInMs,
            int trackPosition);
};

}
#endif // SUBTITLESMANAGER_H
