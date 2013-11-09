#include "SubVideoWidget.h"
#include <QMediaPlayer>
#include <QPaintEvent>
#include <QPainter>
#include <QDragEnterEvent>
#include <QMimeData>

//====================================
SubVideoWidget::SubVideoWidget(QWidget *parent) :
    QGraphicsView(parent){
    QGraphicsScene *scene
            = new QGraphicsScene(this);
    this->setScene(scene);
    scene->addItem(
                &this->videoItem);
    this->videoItem.setAcceptDrops(true);
    for(int i=0; i<N_MAX_SUBTRACK; i++){
        scene->addItem(
                &this->textItems[i]);
        this->textItems[i].setAcceptDrops(true);
        //this->texts[i].hide();
    }
}
//====================================
void SubVideoWidget::init(
        MediaLearner::MediaLearnerLib *mediaLearner){
    this->mediaLearner = mediaLearner;
    this->subtitlesManager
    = this->mediaLearner->getSubtitlesManager();
    QMediaPlayer *mediaPlayer
    = this->mediaLearner->getMediaPlayer();
    mediaPlayer->setVideoOutput(&this->videoItem);
    this->connect(
                mediaPlayer,
                SIGNAL(positionChanged(qint64)),
                SLOT(_onPlayerPositionChanged(qint64)));
    this->connect(
                mediaPlayer,
                SIGNAL(stateChanged(QMediaPlayer::State)),
                SLOT(_onMediaPlayerStateChanged(QMediaPlayer::State)));
}
//====================================
void SubVideoWidget::_onPlayerPositionChanged(
        qint64 position){
    this->_drawSubtitles(
                position);
}
//====================================
void SubVideoWidget::_drawSubtitles(
        qint64 position){
    QSize size = this->size();
    QList<MediaLearner::DrawingText>
            drawingTexts
            = this->subtitlesManager
            ->getTexts(position,
                       size);
    int i=0;
    foreach(MediaLearner::DrawingText drawingText,
            drawingTexts){
        QString text
                = drawingText.getLines().join("\n");
        this->textItems[i].setPlainText(
                    text);
        QPoint topLeft = drawingText.getRect().topLeft();
        this->textItems[i].setPos(
                    topLeft);
        MediaLearner::DrawingSettings drawingSettings
                = drawingText.getDrawingSettings();
        this->textItems[i].setDefaultTextColor(
                    drawingSettings.fontColor);
        this->textItems[i].setFont(
                    drawingSettings.font);
        i++;
    }
}
//====================================
void SubVideoWidget::_onMediaPlayerStateChanged(
        QMediaPlayer::State state){
    if(state == QMediaPlayer::PlayingState){
        this->videoItem.show();
    }else if(state == QMediaPlayer::StoppedState){
        this->videoItem.hide();
    }
}
//====================================
void SubVideoWidget::resizeEvent(
        QResizeEvent *event){
    QGraphicsView::resizeEvent(event);
    QRect currentRect = this->rect();
    QSize currentSize = currentRect.size();
    this->videoItem.setPos(0, 0);
    this->videoItem.setSize(currentSize);
    QMediaPlayer *mediaPlayer
    = this->mediaLearner->getMediaPlayer();
    qint64 position = mediaPlayer->position();
    this->_drawSubtitles(
                position);
}
//====================================
//*
//====================================
void SubVideoWidget::dragEnterEvent(QDragEnterEvent* event){
    SubVideoWidget::onDragEnterEvent(event);
}
//====================================
void SubVideoWidget::onDragEnterEvent(
        QDragEnterEvent* event){
    QList<QUrl> urls = event->mimeData()->urls();
    foreach(QUrl url, urls){
        QString path = url.path();
        QString lowerPath = path.toLower();
        if(lowerPath.endsWith("srt")
                || lowerPath.endsWith("avi")
                || lowerPath.endsWith("ts")
                || lowerPath.endsWith("mp2")
                || lowerPath.endsWith("mp3")
                || lowerPath.endsWith("mp4")
                || lowerPath.endsWith("ogg")
                || lowerPath.endsWith("ogv")
                || lowerPath.endsWith("flv")
                || lowerPath.endsWith("mkv")){
                event->accept();
                break;
        }
    }
}
//====================================
void SubVideoWidget::dropEvent(QDropEvent* event){
    QList<QUrl> urls = event->mimeData()->urls();
    this->urlsDropped(urls);
}
//====================================
//*/

