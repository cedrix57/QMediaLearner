#include "SubVideoWidget.h"
#include <QMediaPlayer>
#include <QPaintEvent>
#include <QPainter>
#include <QDragEnterEvent>
#include <QMimeData>

//====================================
SubVideoWidget::~SubVideoWidget(){
    foreach(QGraphicsTextItem *item,
            this->textItems){
        delete item;
    }
}
//====================================
SubVideoWidget::SubVideoWidget(QWidget *parent) :
    QGraphicsView(parent){
    QGraphicsScene *scene
            = new QGraphicsScene(this);
    //this->proxyWidget
            //= scene->addWidget(&this->videoWidget);
    scene->addItem(
                &this->videoItem);
    this->setScene(scene);
    this->videoItem.setAcceptDrops(true);
    /*
    for(
        int i=0;
        i<MediaLearner::SubtitlesManager::N_MAX_TRACKS;
        i++){
        scene->addItem(
                &this->textItems[i]);
        this->textItems[i].setAcceptDrops(true);
    }
    //*/
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
    QSize screenSize = this->size();
    int screenHeight = screenSize.height();
    QList<MediaLearner::SubSequenceDrawable>
            drawingTexts
            = this->subtitlesManager
            ->getSubsAt(position,
                       screenSize);
    int i=0;
    this->clearText();
    foreach(MediaLearner::SubSequenceDrawable drawingText,
            drawingTexts){
        QList<MediaLearner::FittedLine> fittedLines
                = drawingText.getFittedLines();
        MediaLearner::DrawingSettings drawingSettings
                = drawingText.getDrawingSettings();
        foreach(MediaLearner::FittedLine fittedLine,
                fittedLines){
            //TODO je m'arrête la...transformer en liste
            QGraphicsTextItem *textItem
                    = this->_getTextItem(i);
            textItem->setPlainText(
                        fittedLine.text);
            textItem->setPos(
                        fittedLine.position.x(),
                        fittedLine.position.y());
            textItem->setDefaultTextColor(
                        Qt::yellow);
                        //drawingSettings.colorText);
            int fontSize = drawingSettings.getFontSize(screenHeight);
            QFont font(
                        drawingSettings.fontFamily,
                        fontSize);
            textItem->setFont(
                        font);
            i++;
        }
    }
}
//====================================
void SubVideoWidget::clearText(){
    foreach(QGraphicsTextItem *item, this->textItems){
        item->setPlainText("");
    }
}
//====================================
QGraphicsTextItem *SubVideoWidget::_getTextItem(
        int index){
    while(this->textItems.size() <= index){
        QGraphicsScene *scene = this->scene();
        QGraphicsTextItem *textItem
                = new QGraphicsTextItem();
        scene->addItem(
                textItem);
        textItem->setAcceptDrops(true);
        this->textItems << textItem;
    }
    QGraphicsTextItem *textItem
            = this->textItems[index];
    return textItem;
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
    //QSizeF size = this->proxyWidget->size();
    //QRectF boundingRect = this->proxyWidget->boundingRect();
    this->videoItem.setPos(0, 0);
    this->videoItem.setSize(currentSize);
    //QGraphicsScene *scene = this->scene();
    //scene->update(scene->sceneRect());
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
//*/
//====================================
void SubVideoWidget::onDragEnterEvent(
        QDragEnterEvent* event){
    const QMimeData *data = event->mimeData();
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
//*
//====================================
void SubVideoWidget::dropEvent(QDropEvent* event){
    QList<QUrl> urls = event->mimeData()->urls();
    this->urlsDropped(urls);
}
//====================================
void SubVideoWidget::mouseDoubleClickEvent(
        QMouseEvent *event){
    QGraphicsView:: mouseDoubleClickEvent(
                event);
    this->showFullScreenOrNormal();
}
//====================================
void SubVideoWidget::showFullScreenOrNormal(){
    if(this->isFullScreen()){
        this->showNormal();
    }else{
#ifdef Q_OS_WINDOWS
        this->showFullScreen();
#else
#endif
    }
}
//====================================
//*/

