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

signals:
    void urlsDropped(QList<QUrl> urls);
    
protected slots:
    void _onPlayerPositionChanged(
            qint64 position);
    void _onMediaPlayerStateChanged(
            QMediaPlayer::State state);

protected:
    QGraphicsVideoItem videoItem;
    QGraphicsTextItem texts[N_MAX_SUBTRACK];
    MediaLearner::MediaLearnerLib *mediaLearner;
    MediaLearner::SubtitlesManager *subtitlesManager;
    virtual void resizeEvent(QResizeEvent * event);
    virtual void dragEnterEvent(QDragEnterEvent* event);
    virtual void dropEvent(QDropEvent* event);
};

#endif // SUBVIDEOWIDGET_H