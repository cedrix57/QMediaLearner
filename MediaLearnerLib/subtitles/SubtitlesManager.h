#ifndef SUBTITLESMANAGER_H
#define SUBTITLESMANAGER_H

#include <QObject>
#include "SubtitleTrack.h"
#include "../sequenceExtractor/SequenceExtractor.h"


namespace ML{

class SubtitlesManager : public QObject{
    Q_OBJECT
public:
    static const int N_MAX_TRACKS = 3;
    explicit SubtitlesManager(QObject *parent = 0);
    int getNTracks();
    void setTrack(QString subtitleFilePath);
    void setTrack(QString subtitleFilePath, int position);
    //TODO feature to enable/disable track
    void enableTrack(int position);
    void disableTrack(int position);
    bool isSubTrackEnabled(int position);
    void setSubtitleShift(int position, int shiftInMs);
    void setDrawingSettings(
            int position,
            DrawingSettings &drawingSettings);
    DrawingSettings getDrawingSettings(
            int position);
    QList<SubSequenceDrawable> getSubsAt(
            qint64 positionInMs,
            QSize screenSize);
    static void setContext(
            QList<SubSequenceDrawable>
            &subSequencesDrawable,
            QSize screenSize);
    SubtitleTrack *getSubtitleTracks();
    /*
    QList<QList<DrawingSubtitleInfo> > getSubsAt(
            QSharedPointer<QList<Sequence> >
            sequences,
            QSize screenSize);
            //*/
    //TODO get all draming settings

signals:
    
public slots:

protected slots:
    void _initDrawingSettings();

protected:
    bool enabledTracks[N_MAX_TRACKS];
    int trackShifts[N_MAX_TRACKS];
    SubtitleTrack subtitleTracks[N_MAX_TRACKS];
    SubSequenceDrawable getSubAt(
            qint64 positionInMs,
            int trackPosition);
};

}
#endif // SUBTITLESMANAGER_H
