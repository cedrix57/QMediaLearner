#ifndef SUBVIDEOWIDGET_H
#define SUBVIDEOWIDGET_H

#include <QGraphicsView>
#include <QGraphicsVideoItem>
#include <QGraphicsTextItem>
#include <QList>
#include <MediaLearnerLib.h>

class SubVideoWidget : public QGraphicsView{
    Q_OBJECT
public:
    explicit SubVideoWidget(QWidget *parent = 0);
    void init(
            MediaLearner::MediaLearnerLib *mediaLearner);
    static void onDragEnterEvent(QDragEnterEvent* event);

public slots:
    void showFullScreenOrNormal();

signals:
    void urlsDropped(QList<QUrl> urls);
    
protected slots:
    void _onPlayerPositionChanged(
            qint64 position);
    void _onMediaPlayerStateChanged(
            QMediaPlayer::State state);
    void _drawSubtitles(
            qint64 position);

protected:
    QGraphicsVideoItem videoItem;
    QGraphicsTextItem textItems[
    MediaLearner::SubtitlesManager::N_MAX_TRACKS];
    MediaLearner::MediaLearnerLib *mediaLearner;
    MediaLearner::SubtitlesManager *subtitlesManager;
    virtual void resizeEvent(
            QResizeEvent * event);
    virtual void dragEnterEvent(
            QDragEnterEvent* event);
    virtual void dropEvent(
            QDropEvent* event);
    virtual void mouseDoubleClickEvent(
            QMouseEvent *event);
};

#endif // SUBVIDEOWIDGET_H
