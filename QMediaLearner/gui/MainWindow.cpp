#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QMediaPlaylist>
#include <QTime>
#include <sequenceExtractor/PluginSequenceExtractor.h>

//====================================
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);
    this->editExtractionDialog = NULL;
    this->_initMediaPlayer();
    this->_initExtractor();
    this->_connectSlots();
}
//====================================
MainWindow::~MainWindow(){
    delete ui;
}
//====================================
void MainWindow::_initMediaPlayer(){
    this->mediaPlayer
            = this->mediaLearner.getMediaPlayer();
    this->mediaPlayer->setVideoOutput(
                this->ui->videoWidget);
    this->_onMediaPlayerStateChanged(
                QMediaPlayer::StoppedState);
}
//====================================
void MainWindow::_initExtractor(){
    QMap<QString, MediaLearner::PluginSequenceExtractor*>
            availableExtractors
            = MediaLearner
            ::PluginSequenceExtractor
            ::getExtractors();
    MediaLearner::PluginSequenceExtractor*
            firstExtractor
            = availableExtractors
            .values()
            .first();
    MediaLearner::SequenceExtractor *extractor
            = this->mediaLearner
            .getSequenceExtractor();
    extractor->setExtractor(firstExtractor);
}
//====================================
void MainWindow::_connectSlots(){
    this->_connectMediaPlayerSlots();
    this->_connectToolBarSlots();
    this->_connectMenaBarSlots();
}
//====================================
void MainWindow::_connectMediaPlayerSlots(){
    this->connect(
                this->mediaPlayer,
                SIGNAL(stateChanged(QMediaPlayer::State)),
                SLOT(_onMediaPlayerStateChanged(
                         QMediaPlayer::State)));
    this->connect(
                this->mediaPlayer,
                SIGNAL(playbackRateChanged(qreal)),
                SLOT(_onPlaybackRateChanged(qreal)));
    this->connect(
                this->mediaPlayer,
                SIGNAL(positionChanged(qint64)),
                SLOT(_onPositionChanged(qint64)));
    this->connect(
                this->mediaPlayer,
                SIGNAL(durationChanged(qint64)),
                SLOT(_onDurationChanged(qint64)));
    this->connect(
                this->ui->sliderVolume,
                SIGNAL(sliderMoved(int)),
                SLOT(_setVolume(int)));
    this->connect(
                this->ui->sliderPosition,
                SIGNAL(sliderMoved(int)),
                SLOT(_onPositionSliderMoved(int)));
    //TODO on playback rate changed
}
//====================================
void MainWindow::_connectToolBarSlots(){
    this->connect(
                this->ui->buttonPlayPause,
                SIGNAL(clicked()),
                SLOT(playOrPause()));
    this->connect(
                this->ui->buttonStop,
                SIGNAL(clicked()),
                SLOT(stop()));
    this->connect(
                this->ui->buttonRepeat,
                SIGNAL(clicked(bool)),
                SLOT(repeatMode(bool)));
    this->connect(
                this->ui->buttonEdit,
                SIGNAL(clicked()),
                SLOT(editExtractions()));
    this->connect(
                this->ui->buttonExtract,
                SIGNAL(clicked()),
                SLOT(extract()));
    this->connect(
                this->ui->buttonSlower,
                SIGNAL(clicked()),
                SLOT(slower()));
    this->connect(
                this->ui->buttonFaster,
                SIGNAL(clicked()),
                SLOT(faster()));
}
//====================================
void MainWindow::_connectMenaBarSlots(){
    //Media
    this->connect(
                this->ui->actionOpenFile,
                SIGNAL(triggered()),
                SLOT(openFile()));
    this->connect(
                this->ui->actionOpenFiles,
                SIGNAL(triggered()),
                SLOT(openFiles()));
    this->connect(
                this->ui->actionClose,
                SIGNAL(triggered()),
                SLOT(close()));
    //Extractions
    //Subtitles
    //Tools
    this->connect(
                this->ui->actionPlayingList,
                SIGNAL(triggered()),
                SLOT(showPlayingList()));
    this->connect(
                this->ui->actionVideoEffects,
                SIGNAL(triggered()),
                SLOT(showVideoEffects()));
    this->connect(
                this->ui->actionAudioEffects,
                SIGNAL(triggered()),
                SLOT(showAudioEffects()));
    this->connect(
                this->ui->actionSettings,
                SIGNAL(triggered()),
                SLOT(showSettings()));
    //Help
    this->connect(
                this->ui->actionHelp,
                SIGNAL(triggered()),
                SLOT(help()));
    this->connect(
                this->ui->actionAbout,
                SIGNAL(triggered()),
                SLOT(about()));
}
//====================================
//Media
//====================================
void MainWindow::openFile(){
    QString filePath
            = QFileDialog::getOpenFileName(
                this,
                tr("Open a file"),
                tr("."));
    //TODO add filter according available formats
    if(!filePath.isEmpty()){
        this->mediaLearner.setMedia(filePath);
        this->playOrPause();
    }
}
//====================================
void MainWindow::openFiles(){
}
//====================================
void MainWindow::close(){
}
//====================================
//Bottom tool bar
//====================================
void MainWindow::playOrPause(){
    QMediaPlayer::State state
            = this->mediaPlayer->state();
    if(state == QMediaPlayer::PlayingState){
        this->mediaPlayer->pause();
    }else if(state == QMediaPlayer::PausedState
             || state == QMediaPlayer::StoppedState){
        this->mediaPlayer->play();
    }
}
//====================================
void MainWindow::_onMediaPlayerStateChanged(
        QMediaPlayer::State state){
    if(state == QMediaPlayer::PlayingState){
        this->ui->buttonPlayPause
                ->setText(tr("Pause"));
        this->ui->sliderPosition->setEnabled(true);
        this->ui->buttonExtract->setEnabled(true);
    }else if(state == QMediaPlayer::PausedState
             || state == QMediaPlayer::StoppedState){
        this->ui->buttonPlayPause
                ->setText(tr("Play"));
        if(state == QMediaPlayer::StoppedState){
            this->ui->videoWidget->update();
            this->ui->sliderPosition->setValue(0);
            this->ui->sliderPosition->setEnabled(false);
            this->ui->buttonExtract->setEnabled(false);
            this->mediaPlayer->setPosition(0);
        }
    }
}
//====================================
void MainWindow::stop(){
    this->mediaPlayer->stop();
    this->ui->buttonPlayPause
            ->setText(tr("Play"));
}
//====================================
void MainWindow::repeatMode(bool enabled){
    QMediaPlaylist *playlist
            = this->mediaPlayer->playlist();
    if(playlist != NULL){
        //TODO put in settings and take into account
        //from the first media played
        if(enabled){
            playlist->setPlaybackMode(
                        QMediaPlaylist::Loop);
        }else{
            playlist->setPlaybackMode(
                        QMediaPlaylist::Sequential);
        }
    }
}
//====================================
void MainWindow::editExtractions(){
    delete this->editExtractionDialog;
    this->editExtractionDialog
            = new EditExtractionDialog(
                &this->mediaLearner,
                this);
    this->editExtractionDialog->show();
}
//====================================
void MainWindow::extract(){
        MediaLearner::SequenceExtractor *extractor
                = this->mediaLearner
                .getSequenceExtractor();
        qint64 currentPosition
                = this->mediaPlayer
                ->position();
        extractor->extractSequence(currentPosition);
}
//====================================
const float rateStep = 0.1;
const float rateMax = 2;
const float rateMin = 0.2;
//====================================
void MainWindow::faster(){
    float rate = this->mediaPlayer->playbackRate();
    rate = qMin(rateMax, rate + rateStep);
    this->mediaPlayer->setPlaybackRate(rate);
}
//====================================
void MainWindow::slower(){
    float rate = this->mediaPlayer->playbackRate();
    rate = qMax(rateMin, rate - rateStep);
    this->mediaPlayer->setPlaybackRate(rate);
}
//====================================
void MainWindow::_onPlaybackRateChanged(
        qreal rate){
    QString rateString
            = QString::number(
                rate,
                'h', //TODO check letter
                1);
    rateString += "x";
    this->ui->labelPlaybackRate
            ->setText(
                rateString);
}
//====================================
void MainWindow::_onDurationChanged(
        qint64 duration){
    this->ui->sliderPosition
            ->setRange(0, duration);
    QTime time(0, 0, 0, 0);
    time = time.addMSecs(duration);
    QString format = "mm:ss";
    int nHours = time.hour();
    if(nHours > 0){
        format = "h:" + format;
    }
    QString durationString
            = time.toString(
                format);
    this->ui->labelDuration
            ->setText(
                durationString);
}
//====================================
void MainWindow::_onPositionChanged(
        qint64 position){
    this->ui->sliderPosition
            ->setValue(
                position);
    QTime time(0, 0, 0, 0);
    time = time.addMSecs(position);
    QString format = "mm:ss";
    int nHours = time.hour();
    if(nHours > 0){
        format = "h:" + format;
    }
    QString positionString
            = time.toString(
                format);
    this->ui->labelPlayingPosition
            ->setText(
                positionString);
}
//====================================
void MainWindow::_onPositionSliderMoved(
        int position){
    this->mediaPlayer->setPosition(position);
}
//====================================
void MainWindow::_setVolume(
        int percentage){
    this->mediaPlayer->setVolume(percentage);
    QString percentageString
            = QString::number(
                percentage);
    percentageString += "%";
    this->ui->labelVolume
            ->setText(
                percentageString);
}
//====================================
//Tools
//====================================
void MainWindow::showPlayingList(){
}
//====================================
void MainWindow::showVideoEffects(){
}
//====================================
void MainWindow::showAudioEffects(){
}
//====================================
void MainWindow::showSettings(){
}
//====================================
//Help
//====================================
void MainWindow::help(){
}
//====================================
void MainWindow::about(){
}
//====================================
