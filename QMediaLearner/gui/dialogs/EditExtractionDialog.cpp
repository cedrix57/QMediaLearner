#include "EditExtractionDialog.h"
#include "ui_EditExtractionDialogs.h"
#include <sequenceExtractor/PluginSequenceExtractor.h>
#include "ExportVideoDialog.h"

//====================================
EditExtractionDialog::EditExtractionDialog(
            ML::MediaLearnerLib
            *mediaLearner,
            QWidget *parent)
    : QDialog(parent),
    ui(new Ui::EditExtractionDialog){
    ui->setupUi(this);
    this->mediaLearner = mediaLearner;
    this->exportVideoDialog = NULL;
    this->_initVideoPlayer();
    this->_connectSlots();
    this->ui->sliderBoundaries
            ->setHandleMovementMode(
                QxtSpanSlider
                ::NoCrossing);
}
//====================================
void EditExtractionDialog::showEvent(
        QShowEvent * event){
    this->_loadExtractions();
    this->_hideExportButtonEventually();
}
//====================================
EditExtractionDialog::~EditExtractionDialog(){
    delete ui;
}
//====================================
void EditExtractionDialog::_initVideoPlayer(){
    ML::SequenceExtractor *extractor
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
                this->ui->buttonExport,
                SIGNAL(clicked()),
                SLOT(exportVideo()));
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
    ML::SequenceExtractor *extractor
            = this->mediaLearner
            ->getSequenceExtractor();
    this->connect(
                extractor,
                SIGNAL(sequenceExtracted(ML::Sequence)),
                SLOT(addSequence(ML::Sequence)));
    QSharedPointer<QList<ML::Sequence> >
            sequences
            = extractor->getExtractedSequences();
    this->backupSequences
            = QSharedPointer<
            QList<
            ML::Sequence> >(
                new QList<
                ML::Sequence>);
    *this->backupSequences
            = *sequences;
    foreach(ML::Sequence sequence,
            *sequences){
        this->addSequence(sequence);
    }
}
//====================================
void EditExtractionDialog::_hideExportButtonEventually(){
    int nSequences = this->ui->listSequences->count();
    if(nSequences > 0){
        this->ui->buttonExport->setEnabled(true);
    }else{
        this->ui->buttonExport->setEnabled(false);
    }
}
//====================================
void EditExtractionDialog::addSequence(
        ML::Sequence sequence){
    QString sequenceString
            = sequence.toString();
    this->ui->listSequences->addItem(
                sequenceString);
}
//====================================
void EditExtractionDialog::_onSelectionChanged(int index){
    if(index >= 0){
        ML::SequenceExtractor *extractor
                = this->mediaLearner
                ->getSequenceExtractor();
        extractor->selectSequence(index);
        ML::Sequence
                sequence
                = extractor
                ->getSelectedSequence();
        int sequenceShift = 2000;
        int minSlider
                = qMax(
                    0l,
                    sequence.beginInMs - 2000);
        long int duration = this->mediaPlayer->duration();
        int maxSlider
                = qMin(
                    duration,
                    sequence.endInMs + 2000);
        this->ui->sliderBoundaries
                ->setMinimum(minSlider);
        //span slider
        this->ui->sliderBoundaries
                ->setMinimum(minSlider);
        this->ui->sliderBoundaries
                ->setMaximum(maxSlider);
        this->ui->sliderBoundaries
                ->setLowerPosition(
                    sequence.beginInMs);
        this->ui->sliderBoundaries
                ->setUpperPosition(
                    sequence.endInMs);
        this->_adjustLowerBoundary(
                    sequence.beginInMs);
        this->_adjustUpperBoundary(
                    sequence.endInMs);
        //Position
        this->ui->sliderPosition
                ->setMinimum(
                    sequence.beginInMs);
        this->ui->sliderPosition
                ->setMaximum(
                    sequence.endInMs);
        this->ui->sliderPosition
                ->setValue(
                    sequence.beginInMs);
    }
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
    ML::SequenceExtractor *extractor
            = this->mediaLearner
            ->getSequenceExtractor();
    int currentPosition
            = this->ui->listSequences
            ->currentRow();
    this->ui->listSequences->takeItem(
                currentPosition);
    extractor->deleteSequence(currentPosition);
    this->_hideExportButtonEventually();
}
//====================================
void EditExtractionDialog::accept(){
    this->mediaPlayer->stop();
    QDialog::accept();
}
//====================================
void EditExtractionDialog::reject(){
    this->mediaPlayer->stop();
    ML::SequenceExtractor *extractor
            = this->mediaLearner
            ->getSequenceExtractor();
    extractor->setSequences(
                this->backupSequences);
    QDialog::reject();
}
//====================================
void EditExtractionDialog::exportVideo(){
    delete this->exportVideoDialog;
    this->exportVideoDialog
            = new ExportVideoDialog(
                this->mediaLearner,
                this);
    this->exportVideoDialog->show();
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
    this->_adjustLowerBoundary(position);
}
//====================================
void EditExtractionDialog::_adjustLowerBoundary(
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
    ML::SequenceExtractor *extractor
            = this->mediaLearner
            ->getSequenceExtractor();
    int currentRow
            = this->ui->listSequences
            ->currentRow();
    extractor->changeMinSequence(
                currentRow,
                position);
}
//====================================
void EditExtractionDialog::_onUpperBoundarySliderChanged(
        int position){
    this->_adjustUpperBoundary(position);
    ML::SequenceExtractor *extractor
            = this->mediaLearner
            ->getSequenceExtractor();
    int currentRow
            = this->ui->listSequences
            ->currentRow();
    extractor->changeMaxSequence(
                currentRow,
                position);
    QListWidgetItem *item
            = this->ui->listSequences
            ->item(currentRow);
    ML::Sequence
            selectedSequence
            = extractor
            ->getSelectedSequence();
    QString sequenceString
            = selectedSequence.toString();
    item->setText(sequenceString);
}
//====================================
void EditExtractionDialog::_adjustUpperBoundary(
        int position){
    QTime time(0, 0, 0, 0);
    time = time.addMSecs(position);
    this->ui->timeEditEnd
            ->setTime(
                time);
    int maxBoundarie
            = this->ui->sliderBoundaries
            ->maximum();
    int newMaxBoundarie = maxBoundarie;
    int shift = maxBoundarie - position;
    if(shift < 1000){
        int duration
                = (int)this->mediaPlayer->duration();
        newMaxBoundarie
                = qMin(
                    duration,
                    maxBoundarie + 2000);
        this->ui->sliderBoundaries
                ->setMaximum(
                    maxBoundarie);
    }else if(shift > 4000){
        newMaxBoundarie -= 2000;
    }
    if(newMaxBoundarie != maxBoundarie){
        this->ui->sliderBoundaries
                ->setMaximum(
                    newMaxBoundarie);
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
