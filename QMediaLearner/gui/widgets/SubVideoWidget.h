#ifndef SUBVIDEOWIDGET_H
#define SUBVIDEOWIDGET_H

#include <QGraphicsView>
#include <QGraphicsVideoItem>
//#include <QGraphicsProxyWidget>
//#include <QVideoWidget>
#include <QGraphicsTextItem>
#include <QList>
#include <MediaLearnerLib.h>

class SubVideoWidget : public QGraphicsView{
    Q_OBJECT
public:
    explicit SubVideoWidget(QWidget *parent = 0);
    ~SubVideoWidget();
    void init(
            ML::MediaLearnerLib *mediaLearner);
    void onDragEnterEvent(QDragEnterEvent* event);

public slots:
    void showFullScreenOrNormal();
    void clearText();

signals:
    void urlsDropped(
            QList<QUrl> urls,
            int position);
    
protected slots:
    void _onPlayerPositionChanged(
            qint64 position);
    void _onMediaPlayerStateChanged(
            QMediaPlayer::State state);
    void _drawSubtitles(
            qint64 position);
    void initDragRectColors();

protected:
    QGraphicsVideoItem videoItem;
    //QGraphicsProxyWidget *proxyWidget;
    //QVideoWidget videoWidget;
    //QGraphicsTextItem textItems[
    //MediaLearner::SubtitlesManager::N_MAX_TRACKS];
    ML::MediaLearnerLib *mediaLearner;
    ML::SubtitlesManager *subtitlesManager;
    QList<QGraphicsTextItem *> textItems;
    QGraphicsRectItem dragRects[
    ML::SubtitlesManager::N_MAX_TRACKS];
    QGraphicsTextItem dragTexts[
    ML::SubtitlesManager::N_MAX_TRACKS];
    QGraphicsTextItem * _getTextItem(int index);
    virtual void resizeEvent(
            QResizeEvent * event);
    virtual void dragEnterEvent(
            QDragEnterEvent* event);
    virtual void dragLeaveEvent(
            QDragLeaveEvent* event);
    virtual void dropEvent(
            QDropEvent* event);
    virtual void mouseDoubleClickEvent(
            QMouseEvent *event);
    void initDragRects();
    void shouDragRects();
    void hideDragRects();
};

#endif // SUBVIDEOWIDGET_H
