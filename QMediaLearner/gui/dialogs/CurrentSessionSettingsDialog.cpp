#include "CurrentSessionSettingsDialog.h"
#include "ui_CurrentSessionSettingsDialog.h"

#include <MediaLearnerLib.h>

//====================================
CurrentSessionSettingsDialog::CurrentSessionSettingsDialog(
        ML::MediaLearnerLib
        *mediaLearner,
        QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CurrentSessionSettingsDialog){
    ui->setupUi(this);
    this->mediaLearner = mediaLearner;
}
//====================================
CurrentSessionSettingsDialog::~CurrentSessionSettingsDialog(){
    delete ui;
}
//====================================
void CurrentSessionSettingsDialog::accept(){
    ML::SubtitlesManager *subtitleManager
            = this->mediaLearner
            ->getSubtitlesManager();
    int subShift1
            = this->ui->spinBoxSubShift1
            ->value();
    subtitleManager->setSubtitleShift(0, subShift1);
    int subShift2
            = this->ui->spinBoxSubShift2
            ->value();
    subtitleManager->setSubtitleShift(1, subShift2);
    int subShift3
            = this->ui->spinBoxSubShift3
            ->value();
    subtitleManager->setSubtitleShift(2, subShift3);
    QString subFilePath1
            = this->ui->lineEditSubFilePath1
            ->text();
    if(!subFilePath1.isEmpty()){
        subtitleManager->setSubTrack(0, subFilePath1);
    }
    QString subFilePath2
            = this->ui->lineEditSubFilePath2
            ->text();
    if(!subFilePath2.isEmpty()){
        subtitleManager->setSubTrack(1, subFilePath2);
    }
    QString subFilePath3
            = this->ui->lineEditSubFilePath3
            ->text();
    if(!subFilePath3.isEmpty()){
        subtitleManager->setSubTrack(2, subFilePath3);
    }
    bool subEnabled1
            = this->ui->checkBoxSubEnabled1
            ->isChecked();
    subtitleManager->setSubTrackEnabled(2, subEnabled1);
    bool subEnabled2
            = this->ui->checkBoxSubEnabled2
            ->isChecked();
    subtitleManager->setSubTrackEnabled(2, subEnabled2);
    bool subEnabled3
            = this->ui->checkBoxSubEnabled3
            ->isChecked();
    subtitleManager->setSubTrackEnabled(2, subEnabled3);
    int volume
            = this->ui->spinBoxVolume
            ->value();
    QMediaPlayer *mediaPlayer
            = this->mediaLearner->getMediaPlayer();
    mediaPlayer->setVolume(volume);
}
//====================================
