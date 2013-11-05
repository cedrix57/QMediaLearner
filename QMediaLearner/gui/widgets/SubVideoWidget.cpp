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
                &this->texts[i]);
        this->texts[i].setAcceptDrops(true);
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
    QList<MediaLearner::DrawingText>
            drawingTexts
            = this->subtitlesManager
            ->getTexts(position);
    int i=0;
    foreach(MediaLearner::DrawingText drawingText,
            drawingTexts){
        QString text
                = drawingText.texts.join("\n");
        this->texts[i].setPlainText(
                    text);
        //this->texts[i].show();
        this->texts[i].setDefaultTextColor(
                    drawingText.fontColor);
        this->texts[i].setFont(
                    drawingText.font);
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

