#include "EditExtractionDialog.h"
#include "ui_EditExtractionDialogs.h"
#include <sequenceExtractor/PluginSequenceExtractor.h>

//====================================
EditExtractionDialog::EditExtractionDialog(
            MediaLearner::MediaLearnerLib
            *mediaLearner,
            QWidget *parent)
    : QDialog(parent),
    ui(new Ui::EditExtractionDialog){
    ui->setupUi(this);
    this->mediaLearner = mediaLearner;
    this->_initVideoPlayer();
    this->_connectSlots();
    this->_loadExtractions();
    this->ui->sliderBoundaries
            ->setHandleMovementMode(
                QxtSpanSlider
                ::NoCrossing);
}
//====================================
EditExtractionDialog::~EditExtractionDialog(){
    delete ui;
}
//====================================
void EditExtractionDialog::_initVideoPlayer(){
    MediaLearner::SequenceExtractor *extractor
            = this->mediaLearner
            ->getSequenceExtractor();
    this->mediaPlayer
            = extractor->getMediaPlayer();
    this->mediaPlayer->setVideoOutput(
                this->ui->videoWidget);
}
//====================================
void EditExtractionDialog::_connectSlots(){
    this->connect(
                this->ui->listSequences,
                SIGNAL(currentRowChanged(int)),
                SLOT(_onSelectionChanged(int)));
    this->connect(
                this->ui->buttonPlayPause,
                SIGNAL(clicked()),
                SLOT(playOrPause()));
    this->connect(
                this->ui->buttonRemoveSequence,
                SIGNAL(clicked()),
                SLOT(removeSelectedSequence()));
    this->connect(
                this->mediaPlayer,
                SIGNAL(stateChanged(QMediaPlayer::State)),
                SLOT(_onMediaPlayerStateChanged(
                         QMediaPlayer::State)));
    this->connect(
                this->mediaPlayer,
                SIGNAL(positionChanged(qint64)),
                SLOT(_onPositionChanged(qint64)));
    this->connect(
                this->ui->sliderPosition,
                SIGNAL(sliderMoved(int)),
                SLOT(_onPositionSliderMoved(int)));
    this->connect(
                this->ui->sliderBoundaries,
                SIGNAL(lowerPositionChanged(int)),
                SLOT(_onLowerBoundarySliderChanged(int)));
    this->connect(
                this->ui->sliderBoundaries,
                SIGNAL(upperPositionChanged(int)),
                SLOT(_onUpperBoundarySliderChanged(int)));
}
//====================================
void EditExtractionDialog::_loadExtractions(){
    MediaLearner::SequenceExtractor *extractor
            = this->mediaLearner
            ->getSequenceExtractor();
    extractor->getExtractedSequences();
    this->connect(
                extractor,
                SIGNAL(sequenceExtracted(MediaLearner::Sequence)),
                SLOT(addSequence(MediaLearner::Sequence)));
    QSharedPointer<QList<MediaLearner::Sequence> >
            sequences
            = extractor->getExtractedSequences();
    foreach(MediaLearner::Sequence sequence,
            *sequences){
        this->addSequence(sequence);
    }
}
//====================================
void EditExtractionDialog::addSequence(
        MediaLearner::Sequence sequence){
    QString sequenceString
            = sequence.toString();
    this->ui->listSequences->addItem(
                sequenceString);
}
//====================================
void EditExtractionDialog::_onSelectionChanged(int index){
    MediaLearner::SequenceExtractor *extractor
            = this->mediaLearner
            ->getSequenceExtractor();
    extractor->selectSequence(index);
    MediaLearner::Sequence
            sequence
            = extractor
            ->getSelectedSequence();
    int sequenceShift = 2000;
    int minSlider
            = qMax(
                0l,
                sequence.minInMs - 2000);
    long int duration = this->mediaPlayer->duration();
    int maxSlider
            = qMin(
                duration,
                sequence.maxInMs + 2000);
    this->ui->sliderBoundaries
            ->setMinimum(minSlider);
    //span slider
    this->ui->sliderBoundaries
            ->setMinimum(minSlider);
    this->ui->sliderBoundaries
            ->setMaximum(maxSlider);
    this->ui->sliderBoundaries
            ->setLowerPosition(
                sequence.minInMs);
    this->ui->sliderBoundaries
            ->setUpperPosition(
                sequence.maxInMs);
    this->_onLowerBoundarySliderChanged(
                sequence.minInMs);
    this->_onUpperBoundarySliderChanged(
                sequence.maxInMs);
    //Position
    this->ui->sliderPosition
            ->setMinimum(
                sequence.minInMs);
    this->ui->sliderPosition
            ->setMaximum(
                sequence.maxInMs);
    this->ui->sliderPosition
            ->setValue(
                sequence.minInMs);
}
//====================================
void EditExtractionDialog::playOrPause(){
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
void EditExtractionDialog::removeSelectedSequence(){
    //TODO
}
//====================================
void EditExtractionDialog::accept(){
    //TODO quit
    QDialog::accept();
}
//====================================
void EditExtractionDialog::reject(){
    //TODO restore originales sequences
    QDialog::reject();
}
//====================================
void EditExtractionDialog::_onMediaPlayerStateChanged(
        QMediaPlayer::State state){
    if(state == QMediaPlayer::PlayingState){
        this->ui->buttonPlayPause
                ->setText(tr("Pause"));
    }else if(state == QMediaPlayer::PausedState
             || state == QMediaPlayer::StoppedState){
        this->ui->buttonPlayPause
                ->setText(tr("Play"));
    }
}
//====================================
void EditExtractionDialog::_onPositionChanged(
        qint64 position){
    this->ui->sliderPosition
            ->setValue(
                position);
    QTime time(0, 0, 0, 0);
    time = time.addMSecs(position);
    this->ui->timeEditPosition
            ->setTime(
                time);
}
//====================================
void EditExtractionDialog::_onPositionSliderMoved(
        int position){
    this->mediaPlayer->setPosition(position);
}

//====================================
void EditExtractionDialog::_onLowerBoundarySliderChanged(
        int position){
    QTime time(0, 0, 0, 0);
    time = time.addMSecs(position);
    this->ui->timeEditBegin
            ->setTime(
                time);
    int minBoundarie
            = this->ui->sliderBoundaries
            ->minimum();
    int newMinBoundarie = minBoundarie;
    int shift = position - minBoundarie;
    if(shift < 1000){
        newMinBoundarie
                = qMax(
                    0,
                    minBoundarie - 2000);
    }else if(shift > 4000){
        newMinBoundarie += 2000;
    }
    if(newMinBoundarie != minBoundarie){
        this->ui->sliderBoundaries
                ->setMinimum(
                    newMinBoundarie);
    }
    this->ui->sliderPosition->setMinimum(
                position);
    this->mediaPlayer->setPosition(
                position);
}
//====================================
void EditExtractionDialog::_onUpperBoundarySliderChanged(
        int position){
    QTime time(0, 0, 0, 0);
    time = time.addMSecs(position);
    this->ui->timeEditEnd
            ->setTime(
                time);
    int maxBoundarie
            = this->ui->sliderBoundaries
            ->maximum();
    int shift = maxBoundarie - position;
    if(shift < 1000){
        int duration
                = (int)this->mediaPlayer->duration();
        maxBoundarie
                = qMin(
                    duration,
                    maxBoundarie + 2000);
        this->ui->sliderBoundaries
                ->setMaximum(
                    maxBoundarie);
    }else if(shift > 4000){
        maxBoundarie -= 2000;
        this->ui->sliderBoundaries
                ->setMaximum(
                    maxBoundarie);
    }
    this->ui->sliderPosition->setMaximum(
                position);
    int currentPosition
            = this->mediaPlayer->position();
    if(currentPosition > position){
        this->mediaPlayer->setPosition(
                    position);
    }
}
//====================================
//TODO changer les bornes dans la liste de gauche + dans le modèle
//Add sequence removing
//If extractions are computing, I should display a progress dialog
//I should add a export sequences button
