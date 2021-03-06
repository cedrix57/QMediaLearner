#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QMediaPlaylist>
#include <QTime>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QFileDialog>
#include <QApplication>
#include <sequenceExtractor/PluginSequenceExtractor.h>
#include "dialogs/SettingsDialog.h"
#include "dialogs/EditExtractionDialog.h"
#include "dialogs/CurrentSessionSettingsDialog.h"
#include <CrashManagerSingleton.h>
#include <QTimer>
#include <QFontDatabase>

//====================================
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);
    QApplication::instance()
            ->installEventFilter(this);
    this->_stopping = false;
    this->editExtractionDialog = NULL;
    this->settingsDialog = NULL;
    this->currentSettingsDialog = NULL;
    this->_initMediaPlayer();
    this->_initExtractor();
    this->_connectSlots();
    this->reloadAfterCrashEventually();
}
//====================================
MainWindow::~MainWindow(){
    delete ui;
    ML::CrashManagerSingleton *crashManager
            = ML::CrashManagerSingleton
            ::getInstance();
    crashManager->setHasCrashed(false);
}
//====================================
void MainWindow::closeEvent(QCloseEvent * event){
    ML::SequenceExtractor *extractor
            = this->mediaLearner
            .getSequenceExtractor();
    QSharedPointer<QList<ML::Sequence> >
            sequences
            = extractor->getExtractedSequences();
    bool extractedSequencesSaved
            = ML::SequencesWithSubs::isSequencesEncoded(
                *sequences);
    if(!extractedSequencesSaved){
        QMessageBox::StandardButton reply
                = QMessageBox::question(
                    this,
                    tr("Extracted sequences are not saved"),
                    tr("Do you really want to close the application?"),
                    QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes){
            this->stop();
            this->mediaLearner.stop();
            QMainWindow::closeEvent(event);
        }
    }else{
        this->stop();
        this->mediaLearner.stop();
        QMainWindow::closeEvent(event);
    }
}
//====================================
void MainWindow::keyPressEvent(
        QKeyEvent *event){
    if(event->key() == Qt::Key_Escape){
        event->accept();
        this->playOrPause();
    }
}
//====================================
void MainWindow::_initMediaPlayer(){
    this->mediaPlayer
            = this->mediaLearner.getMediaPlayer();
    //this->mediaPlayer->setVideoOutput(
                //this->ui->videoWidget);
    this->_onMediaPlayerStateChanged(
                QMediaPlayer::StoppedState);
    this->ui->graphicsViewVideo->
            init(&this->mediaLearner);
}
//====================================
void MainWindow::_initExtractor(){
    QMap<QString, ML::PluginSequenceExtractor*>
            availableExtractors
            = ML
            ::PluginSequenceExtractor
            ::getExtractors();
    ML::PluginSequenceExtractor*
            firstExtractor
            = availableExtractors
            .values()
            .first();
    ML::SequenceExtractor *extractor
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
    ML::SequenceExtractor *extractor
            = this->mediaLearner
            .getSequenceExtractor();
    this->connect(
                extractor,
                SIGNAL(numberOfSequencesChanged(int)),
                SLOT(_onNumberOfSequencesChanged(int)));
    //TODO on playback rate changed
}
//====================================
void MainWindow::_connectToolBarSlots(){
    this->connect(
                this->ui->buttonPlay,
                SIGNAL(clicked()),
                SLOT(playOrPause()));
    this->connect(
                this->ui->buttonPause,
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
    this->ui->graphicsViewVideo->connect(
                this->ui->actionFullScreen,
                SIGNAL(triggered()),
                SLOT(showFullScreenOrNormal()));
    this->connect(
                this->ui->actionClose,
                SIGNAL(triggered()),
                SLOT(close()));
    this->connect(
                this->ui->graphicsViewVideo,
                SIGNAL(urlsDropped(QList<QUrl>, int)),
                SLOT(openUrls(QList<QUrl>, int)));
    //Extractions
    //Subtitles
    this->connect(
                this->ui->actionSubFile1,
                SIGNAL(triggered()),
                SLOT(openSubtrack1()));
    this->connect(
                this->ui->actionSubFile2,
                SIGNAL(triggered()),
                SLOT(openSubtrack2()));
    this->connect(
                this->ui->actionSubFile3,
                SIGNAL(triggered()),
                SLOT(openSubtrack3()));
    this->connect(
                this->ui->actionEnabled1,
                SIGNAL(toggled(bool)),
                SLOT(setEnabledSubtrack1(bool)));
    this->connect(
                this->ui->actionEnabled2,
                SIGNAL(toggled(bool)),
                SLOT(setEnabledSubtrack2(bool)));
    this->connect(
                this->ui->actionEnabled3,
                SIGNAL(toggled(bool)),
                SLOT(setEnabledSubtrack3(bool)));
    //Tools
    this->connect(
                this->ui->actionPlayingList,
                SIGNAL(triggered()),
                SLOT(showPlayingList()));
    this->connect(
                this->ui->actionCurrentSessionSettings,
                SIGNAL(triggered()),
                SLOT(showCurrentSessionSettings()));
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
void MainWindow::reloadAfterCrashEventually(){
    ML::CrashManagerSingleton *crashManager
            = ML::CrashManagerSingleton
            ::getInstance();
    bool hasCrashed =
            crashManager->getHasCrashed();
    bool toReset = true;
    if(hasCrashed){
        QString mediaFilePath
                = crashManager
                ->getMediaFilePath();
        if(!mediaFilePath.isEmpty()){
            QMessageBox::StandardButton reply
                    = QMessageBox::question(
                        this,
                        tr("The application crashed"),
                        tr("Do you want to reload previous session with the extracted sequences?"),
                        QMessageBox::No | QMessageBox::Yes);
            if(reply == QMessageBox::Yes){
                this->reloadAfterCrash();
                toReset = false;
            }

        }
    }
    if(toReset){
        crashManager->resetInfos();
    }
    crashManager->setHasCrashed(true);
}
//====================================
void MainWindow::reloadAfterCrash(){
    ML::CrashManagerSingleton *crashManager
            = ML::CrashManagerSingleton
            ::getInstance();
    QString mediaFilePath
            = crashManager
            ->getMediaFilePath();
    int position
        = crashManager->getMediaPosition();
    this->playVideo(mediaFilePath);
    this->mediaPlayer->pause();
    qDebug() << "Crash position: " << position;
    bool seekable
            = this->mediaPlayer->isSeekable();
    qDebug() << "Seekable: " << seekable;
    if(seekable){
        this->mediaPlayer->setPosition(
                    position);
    }else{
        this->seekLater(position);
    }
    //qDebug() << "aa1";
    int volume
        = crashManager->getVolume();
    //qDebug() << "aa2";
    //this->mediaPlayer->setVolume(
                //volume);
    this->ui->sliderVolume->setValue(
                volume);
    this->_setVolume(volume);
    double playbackRate
        = crashManager->getPlaybackRate();
    this->mediaPlayer->setPlaybackRate(
                playbackRate);
    //qDebug() << "aa3";
    ML::SubtitlesManager
            *subtitlesManager
            = this->mediaLearner
            .getSubtitlesManager();
    //qDebug() << "aa4";
    for(int i=0;
        i<ML::SubtitlesManager::N_MAX_TRACKS;
        i++){
        QString subtitleFilePath
                = crashManager
                ->getSubtitle(i);
        if(!subtitleFilePath.isEmpty()){
            this->openSubtrack(
                        i,
                        subtitleFilePath);
            bool subEnabled
                    = crashManager
                    ->getSubtitleEnabled(i);
            this->setEnabledSubtrack(
                        i,
                        subEnabled);
            int subShift
                    = crashManager
                    ->getSubtitleShift(i);
            subtitlesManager->setSubtitleShift(
                        i,
                        subShift);

        }
    }
    //qDebug() << "a5";
    QSharedPointer<QList<ML::Sequence> > sequences
    = crashManager->getSequences();
    ML::SequenceExtractor *
            sequenceExtractor
            = this->mediaLearner
            .getSequenceExtractor();
    //qDebug() << "a6";
    sequenceExtractor->setSequences(
                sequences);
    //qDebug() << "a7";
}
//====================================
void MainWindow::seekLater(
        int position,
        int ms){
    this->_toSeek = position;
    QTimer::singleShot(
                ms,
                this,
                SLOT(_seekLater()));
    //qDebug() << "ok1";
}
//====================================
void MainWindow::_seekLater(){
    //qDebug() << "ok2";
    bool seekable
            = this->mediaPlayer->isSeekable();
    qDebug() << "Seekable: " << seekable;
    if(seekable){
        qDebug() << "this->_toSeek: " << this->_toSeek;
        this->mediaPlayer->setPosition(
                    this->_toSeek);
    }else{
        this->seekLater(this->_toSeek);
    }
}
//====================================
// Media
//====================================
/*
void MainWindow::dragEnterEvent(QDragEnterEvent* event){
    SubVideoWidget::onDragEnterEvent(event);
}
//====================================
void MainWindow::dropEvent(QDropEvent* event){
    QList<QUrl> urls = event->mimeData()->urls();
    this->openUrls(urls);
}
//*/
//====================================
bool MainWindow::eventFilter(QObject *object, QEvent *event) {
    if(event->type() == QEvent::KeyRelease){
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        int key = keyEvent->key();
        if(key == Qt::Key_Space){
            this->playOrPause();
            return true;
        }
    }
    return false;
}
//====================================
void MainWindow::openUrls(
        QList<QUrl> urls,
        int subPos){
    QList<QUrl> srtUrls;
    QList<QUrl> videoUrls;
    foreach(QUrl url, urls){
        QString path = url.path();
        if(path.endsWith("srt", Qt::CaseInsensitive)){
            srtUrls << url;
        }else{
            bool isFormatSupported
                    = this->mediaLearner
                    .isFormatSupported(
                        path);
            if(isFormatSupported){
                videoUrls << url;
            }
        }
    }
    int nVideoUrls = videoUrls.size();
    if(nVideoUrls > 0){
        ML::SubtitlesManager
                *subtitlesManager
                = this->mediaLearner
                .getSubtitlesManager();
        subtitlesManager->disableAllSubTrack();
        QString path = videoUrls.first().path();
        this->playVideo(path);
    }
    QString videoFilePath
            = this->mediaLearner
            .getMediaFilePath();
    if(!videoFilePath.isEmpty()){
        int nSubs = srtUrls.size();
        if(nSubs == 1){
            QString path = srtUrls.first().path();
            this->openSubtrack(
                        subPos,
                        path);
        }else if(nSubs > 0){
            int nextSubPosition = 0;
            foreach(QUrl url, srtUrls){
                QString path = url.path();
                this->openSubtrack(
                            nextSubPosition,
                            path);
                nextSubPosition++;
                if(nextSubPosition
                        == ML::SubtitlesManager
                        ::N_MAX_TRACKS){
                    break;
                }
            }
        }
    }
}
//====================================
void MainWindow::openFile(){
    QString filePath
            = QFileDialog::getOpenFileName(
                this,
                tr("Open a file"),
                tr("."));
    //TODO add filter according available formats
    if(!filePath.isEmpty()){
        this->playVideo(filePath);
    }
}
//====================================
void MainWindow::playVideo(
        QString fileName){
        this->mediaLearner.setMedia(fileName);
        this->playOrPause();
}
//====================================
void MainWindow::openFiles(){
}
//====================================
void MainWindow::close(){
}
//====================================
// Subtitle files
//====================================
void MainWindow::openSubtrack1(){
    this->openSubtrack(0);
}
//====================================
void MainWindow::openSubtrack(int position){
    QString filePath =
            QFileDialog::getOpenFileName(
                this,
                tr("Open a subtitle file"),
                "", //TODO current video directory
                "SubRip (*.srt)");
    if(!filePath.isNull()){
        this->openSubtrack(
                    position,
                    filePath);
    }
}
//====================================
void MainWindow::openSubtrack(
        int position,
        QString filePath){
    ML::SubtitlesManager
            *subtitlesManager
            = this->mediaLearner
            .getSubtitlesManager();
    subtitlesManager->setSubTrack(
                position,
                filePath);
    if(position == 0){
        this->ui->actionEnabled1->setChecked(
                    true);
    }else if(position == 1){
        this->ui->actionEnabled2->setChecked(
                    true);
    }else if(position == 2){
        this->ui->actionEnabled3->setChecked(
                    true);
    }
}
//====================================
void MainWindow::openSubtrack2(){
    this->openSubtrack(1);
}
//====================================
void MainWindow::openSubtrack3(){
    this->openSubtrack(2);
}
//====================================
void MainWindow::setEnabledSubtrack1(bool enable){
    this->setEnabledSubtrack(0, enable);
}
//====================================
void MainWindow::setEnabledSubtrack(
        int position,
        bool enabled){
    ML::SubtitlesManager
            *subtitlesManager
            = this->mediaLearner
            .getSubtitlesManager();
    if(enabled){
        subtitlesManager->enableSubTrack(position);
    }else{
        subtitlesManager->disableSubTrack(position);
    }
}
//====================================
void MainWindow::setEnabledSubtrack2(bool enable){
    this->setEnabledSubtrack(1, enable);
}
//====================================
void MainWindow::setEnabledSubtrack3(bool enable){
    this->setEnabledSubtrack(2, enable);
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
        this->ui->buttonPlay->hide();
        this->ui->buttonPause->show();
        //this->ui->buttonPlayPause
                //->setIcon(QIcon(":/icons/pause-24.ico"));
                //->setText(tr("Pause"));
        this->ui->sliderPosition->setEnabled(true);
        this->ui->buttonExtract->setEnabled(true);
    }else if(state == QMediaPlayer::PausedState
             || state == QMediaPlayer::StoppedState){
        this->ui->buttonPlay->show();
        this->ui->buttonPause->hide();
        //this->ui->buttonPlayPause
                //->setIcon(QIcon(":/icons/play-24.ico"));
                //->setText(tr("Play"));
        if(state == QMediaPlayer::StoppedState){
            bool repeatMode
                    = this->ui->buttonRepeat->isChecked();
            if(!this->_stopping && repeatMode){
                this->mediaPlayer->setPosition(0);
                this->ui->sliderPosition->setValue(0);
                this->playOrPause();
            }else{
                this->ui->sliderPosition->setValue(0);
                this->ui->sliderPosition->setEnabled(false);
                this->ui->buttonExtract->setEnabled(false);
                this->mediaPlayer->setPosition(0);
            }
        }
    }
}
//====================================
void MainWindow::stop(){
    this->_stopping = true;
    this->mediaPlayer->stop();
    this->ui->buttonPlay->show();
    this->ui->buttonPause->hide();
    this->_stopping = false;
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
    this->mediaPlayer->pause();
    //TODO launch a progress dialog if extractions not finished yet
    delete this->editExtractionDialog;
    this->editExtractionDialog
            = new EditExtractionDialog(
                &this->mediaLearner,
                this);
    this->editExtractionDialog->show();
}
//====================================
void MainWindow::extract(){
        ML::SequenceExtractor *extractor
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
                'h',
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
void MainWindow::_onNumberOfSequencesChanged(
        int nSequences){
    if(nSequences > 0){
        this->ui->buttonEdit
                ->setEnabled(true);
    }else{
        this->ui->buttonEdit
                ->setEnabled(false);
    }
}
//====================================
//Tools
//====================================
void MainWindow::showPlayingList(){
}
//====================================
void MainWindow::showCurrentSessionSettings(){
    delete this->currentSettingsDialog;
    this->currentSettingsDialog
            = new CurrentSessionSettingsDialog(
                &this->mediaLearner,
                this);
    this->currentSettingsDialog->show();
}
//====================================
void MainWindow::showSettings(){
    delete this->settingsDialog;
    this->settingsDialog
            = new SettingsDialog(
                this);
    this->settingsDialog->show();
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
