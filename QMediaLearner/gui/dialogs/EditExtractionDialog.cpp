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
                this->mediaPlayer,
                SIGNAL(stateChanged(QMediaPlayer::State)),
                SLOT(_onMediaPlayerStateChanged(
                         QMediaPlayer::State)));
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
    this->ui->sliderBoundaries
            ->setMinimum(minSlider);
    long int duration = this->mediaPlayer->duration();
    int maxSlider
            = qMin(
                duration,
                sequence.maxInMs + 2000);
    this->ui->sliderBoundaries
            ->setMaximum(maxSlider);
    this->ui->sliderBoundaries
            ->setMinimum(minSlider);
    this->ui->sliderBoundaries
            ->setLowerValue(
                sequence.minInMs);
    this->ui->sliderBoundaries
            ->setUpperValue(
                sequence.maxInMs);
    this->ui->sliderPosition
            ->setMinimum(
                sequence.minInMs);
    this->ui->sliderPosition
            ->setMaximum(
                sequence.maxInMs);
    this->ui->sliderPosition
            ->setMinimum(
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
