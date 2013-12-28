#include "SubVideoWidget.h"
#include <QMediaPlayer>
#include <QPaintEvent>
#include <QPainter>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QFontMetrics>
#include <SettingsManagerSingleton.h>

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
        ML::MediaLearnerLib *mediaLearner){
    this->mediaLearner = mediaLearner;
    this->subtitlesManager
    = this->mediaLearner->getSubtitlesManager();
    QMediaPlayer *mediaPlayer
    = this->mediaLearner->getMediaPlayer();
    mediaPlayer->setVideoOutput(&this->videoItem);
    this->initDragRects();
    this->connect(
                mediaPlayer,
                SIGNAL(positionChanged(qint64)),
                SLOT(_onPlayerPositionChanged(qint64)));
    this->connect(
                mediaPlayer,
                SIGNAL(stateChanged(QMediaPlayer::State)),
                SLOT(_onMediaPlayerStateChanged(QMediaPlayer::State)));
    ML::SettingsManagerSingleton
            *settingsManager
            = ML::SettingsManagerSingleton::getInstance();
    this->connect(
                settingsManager,
                SIGNAL(subSettingsChanged()),
                SLOT(initDragRectColors()));
}
//====================================
void SubVideoWidget::_onPlayerPositionChanged(
        qint64 position){
    this->_drawSubtitles(
                position);
}
//====================================
void SubVideoWidget::initDragRects(){
    QGraphicsScene *scene = this->scene();
    for(int i=0;
        i<ML::SubtitlesManager::N_MAX_TRACKS;
        i++){
        scene->addItem(
                &this->dragRects[i]);
        scene->addItem(
                &this->dragTexts[i]);
        QString subTrackText
                = tr("Subtrack n°")
                + QString::number(i+1);
        this->dragTexts[i].setPlainText(subTrackText);
        this->dragRects[i].setAcceptDrops(true);
        this->dragTexts[i].setAcceptDrops(true);
    }
    this->initDragRectColors();
    this->hideDragRects();
}
//====================================
void SubVideoWidget::initDragRectColors(){
    ML::SettingsManagerSingleton
            *settingsManager
            = ML::SettingsManagerSingleton::getInstance();
    for(int i=0;
        i<ML::SubtitlesManager::N_MAX_TRACKS;
        i++){
        QColor color
                = settingsManager
                ->getSubColor(i);
        this->dragRects[i].setPen(color);
        this->dragTexts[i].setDefaultTextColor(color);
        color.setAlpha(80);
        QBrush brush(color);
        this->dragRects[i].setBrush(brush);
    }
}
//====================================
void SubVideoWidget::shouDragRects(){
    QSize size = this->size();
    int widthRect = size.width();
    int heightRect = size.height() / 3;
    int fontSize = heightRect * 0.08 + 0.5;
    QFont font("Arial", fontSize);
    QFontMetrics fontMetrics(font);
    int yTextShift = heightRect*0.05 + 0.5;
    yTextShift = qMin(8, yTextShift);
    for(int i=0;
        i<ML::SubtitlesManager::N_MAX_TRACKS;
        i++){
        this->dragRects[i].setRect(
                    0,
                    0,
                    widthRect,
                    heightRect);
        this->dragRects[i].setX(0);
        int yRect
                = (ML::SubtitlesManager::N_MAX_TRACKS
                   - i - 1)
                * heightRect;
        this->dragRects[i].setY(yRect);
        QString text = this->dragTexts[i].toPlainText();
        int textWidth = fontMetrics.width(text);
        int xText = (widthRect - textWidth) / 2;
        this->dragTexts[i].setX(xText);
        this->dragTexts[i].setY(yRect + yTextShift);
        this->dragTexts[i].setFont(font);
        this->dragRects[i].show();
        this->dragTexts[i].show();
    }
}
//====================================
void SubVideoWidget::hideDragRects(){
    for(int i = 0;
        i<ML::SubtitlesManager::N_MAX_TRACKS;
        i++){
        this->dragRects[i].hide();
        this->dragTexts[i].hide();
    }
}
//====================================
void SubVideoWidget::_drawSubtitles(
        qint64 position){
    QSize screenSize = this->size();
    int screenHeight = screenSize.height();
    QList<ML::SubSequenceDrawable>
            drawingTexts
            = this->subtitlesManager
            ->getSubsAt(position,
                       screenSize);
    int i=0;
    this->clearText();
    foreach(ML::SubSequenceDrawable drawingText,
            drawingTexts){
        QList<ML::FittedLine> fittedLines
                = drawingText.getFittedLines();
        ML::DrawingSettings drawingSettings
                = drawingText.getDrawingSettings();
        foreach(ML::FittedLine fittedLine,
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
                        drawingSettings.textColor);
                        //drawingSettings.colorText);
            //int fontSize = drawingSettings.getFontSize(screenHeight);
            int fontSize = drawingText.getPixelSize();
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
    this->onDragEnterEvent(event);
}
//*/
//====================================
void SubVideoWidget::onDragEnterEvent(
        QDragEnterEvent* event){
    const QMimeData *data = event->mimeData();
    QList<QUrl> urls = event->mimeData()->urls();
    foreach(QUrl url, urls){
        QString path = url.path();
        if(path.endsWith("srt", Qt::CaseInsensitive)){
            this->shouDragRects();
        }
    }
    foreach(QUrl url, urls){
        QString path = url.path();
        bool isFormatSupported
                = this->mediaLearner
                ->isFormatSupported(
                    path);
        if(isFormatSupported){
                event->accept();
                break;
        }
    }
}
//*
//====================================
void SubVideoWidget::dropEvent(QDropEvent* event){
    this->hideDragRects();
    QList<QUrl> urls = event->mimeData()->urls();
    event->accept();
    QSize size = this->size();
    int heightRect = size.height() / 3;
    QPoint pos = event->pos();
    int y = pos.y();
    int idSub
            = ML::SubtitlesManager::N_MAX_TRACKS
            - (y / heightRect)
            - 1;
    if(idSub >= ML::SubtitlesManager::N_MAX_TRACKS){
        idSub = ML::SubtitlesManager::N_MAX_TRACKS - 1;
    }
    this->urlsDropped(urls, idSub);
}
//====================================
void SubVideoWidget::dragLeaveEvent(
        QDragLeaveEvent* event){
    this->hideDragRects();
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

