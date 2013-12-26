#include "ExportVideoDialog.h"
#include "ui_ExportVideoDialog.h"
#include <QFileDialog>
#include <SettingsManagerSingleton.h>

//====================================
ExportVideoDialog::ExportVideoDialog(
        ML::MediaLearnerLib
        *mediaLearner,
        QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportVideoDialog){
    ui->setupUi(this);
    this->mediaLearner
            = mediaLearner;
    this->progressDialog.setWindowTitle(
                tr("Encoding..."));
    this->progressDialog.setMinimum(
                0);
    this->progressDialog.setMaximum(
                0);
    this->ui->buttonEditProfiles->hide();
    this->_loadInfos();
    this->_connectSlots();
    this->ui->radioButtonAudioProfile->setChecked(true);
    this->ui->radioButtonVideoProfile->toggle();
    this->changingSize = false;
}
//====================================
void ExportVideoDialog::onAudioProfileToogled(
        bool val){
    this->ui->comboBoxProfilesAudio->setEnabled(val);
    this->ui->comboBoxProfilesVideo->setEnabled(!val);
    this->ui->checkBoxSaveSubEmbedded->setEnabled(!val);
    this->ui->checkBoxSaveSubEncoded->setEnabled(!val);
}
//====================================
void ExportVideoDialog::onProfileChanged(
        QString profileName){
    QString filePath
            = this->ui->lineEditFilePath
            ->text();
    if(!filePath.isEmpty() && filePath.indexOf(".") != -1){
        ML::EncoderInterface
                *encoder
                = this->mediaLearner
                ->getEncoder();
        QString ext
                = encoder->getProfileExt(profileName);
        if(!filePath.endsWith(ext)){
            QStringList cuttedFilePath
                    = filePath.split(".");
            cuttedFilePath.takeLast();
            cuttedFilePath << ext;
            QString newFilePath
                    = cuttedFilePath.join(".");
            this->ui->lineEditFilePath
                    ->setText(newFilePath);
        }
    }
}
//====================================
void ExportVideoDialog::_connectSlots(){
    ML::EncoderInterface
            *encoder
            = this->mediaLearner
            ->getEncoder();
    this->progressDialog.connect(
                encoder,
                SIGNAL(encodingFinished()),
                SLOT(reset()));
    this->progressDialog.connect(
                encoder,
                SIGNAL(encodingFailed()),
                SLOT(reset()));
    this->connect(
                this->ui->buttonBrowseFilePath,
                SIGNAL(clicked()),
                SLOT(browseNewVideoFilePath()));
    this->connect(
                this->ui->radioButtonAudioProfile,
                SIGNAL(toggled(bool)),
                SLOT(onAudioProfileToogled(bool)));
    this->connect(
                this->ui->comboBoxProfilesAudio,
                SIGNAL(currentIndexChanged(QString)),
                SLOT(onProfileChanged(QString)));
    this->connect(
                this->ui->comboBoxProfilesVideo,
                SIGNAL(currentIndexChanged(QString)),
                SLOT(onProfileChanged(QString)));
    this->connect(
                this->ui->spinBoxHeight,
                SIGNAL(valueChanged(int)),
                SLOT(onHeightChanged(int)));
    this->connect(
                this->ui->spinBoxWidth,
                SIGNAL(valueChanged(int)),
                SLOT(onWidthChanged(int)));
}
//====================================
void ExportVideoDialog::_loadInfos(){
    ML::EncoderInterface
            *encoder
            = this->mediaLearner
            ->getEncoder();
    QMap<QString, ML::ProfileInfo>
            videoProfiles
            = encoder->getAvailableVideoProfiles();
    foreach(QString profile, videoProfiles.keys()){
        this->ui->comboBoxProfilesVideo->addItem(
                    profile);
    }
    QMap<QString, ML::ProfileInfo>
            audioProfiles
            = encoder->getAvailableAudioProfiles();
    foreach(QString profile, audioProfiles.keys()){
        this->ui->comboBoxProfilesAudio->addItem(
                    profile);
    }
    ML::SubtitlesManager *subManager
            = this->mediaLearner->getSubtitlesManager();
    bool sub1 = subManager->isSubTrackEnabled(0);
    bool sub2 = subManager->isSubTrackEnabled(1);
    bool sub3 = subManager->isSubTrackEnabled(2);
    bool saveSubs = sub1 || sub2 || sub3;
    this->ui->groupBoxSaveSubtitles
            ->setEnabled(saveSubs);
    this->ui->groupBoxSaveSubtitles
            ->setChecked(saveSubs);
    this->ui->checkBoxTrack1->setChecked(sub1);
    this->ui->checkBoxTrack2->setChecked(sub2);
    this->ui->checkBoxTrack3->setChecked(sub3);
    QSize videoSize = encoder->getOriginalSize();
    this->ui->spinBoxWidth->setValue(videoSize.width());
    this->ui->spinBoxHeight->setValue(videoSize.height());
    this->originalVideoSize = videoSize;
}
//====================================
ExportVideoDialog::~ExportVideoDialog(){
    delete ui;
}
//====================================
void ExportVideoDialog::accept(){
    ML::EncoderInterface
            *encoder
            = this->mediaLearner
            ->getEncoder();
    ML::SequenceExtractor
            *sequenceExtractor
            = this->mediaLearner
            ->getSequenceExtractor();
    ML::SubtitlesManager
            *subtitlesManager
            = this->mediaLearner
            ->getSubtitlesManager();
    QSharedPointer<QList<ML::Sequence> >
            sequences
            = sequenceExtractor
            ->getExtractedSequences();
    ML::SubtitleTrack*
            subs
            = subtitlesManager
            ->getSubtitleTracks();
    this->sequencesWithSubs.init(
                *sequences,
                subs);
    QSize videoSize
            = encoder->getOriginalSize();
    this->sequencesWithSubs.setScreenSize(
                videoSize);
    if(this->ui->groupBoxPlaybackRate->isChecked()){
        double playbackRate = this->ui->spinBoxRate->value();
        encoder->setPlaybackRate(playbackRate);
    }
    if(this->ui->groupBoxResize->isChecked()){
        int videoWidth = this->ui->spinBoxWidth->value();
        int videoHeight = this->ui->spinBoxHeight->value();
        QSize newVideoSize(videoWidth, videoHeight);
        encoder->setNewSize(
                    newVideoSize);
    }
    QString outVideoFilePath
            = this->ui->lineEditFilePath->text();
    QList<ML::SequenceWithSubs>
            sequencesWithSubs
            = this->sequencesWithSubs.getSequencesWithTexts();
    encoder->setSequences(
                sequencesWithSubs);
    encoder->encode(
                outVideoFilePath);
    this->progressDialog.show();
    //*/
    //TODO progress dialog
}
//====================================
void ExportVideoDialog::browseNewVideoFilePath(){
    QString lastDirPath
            = ML::SettingsManagerSingleton::getInstance()
            ->getExtractedVideoPath();
    QString filePath
            = QFileDialog::getSaveFileName(
                this,
                tr("Choose a new file"),
                lastDirPath);
    if(!filePath.isNull()){
        QString profileName;
        if(this->ui->radioButtonAudioProfile->isChecked()){
            profileName
                    = this->ui->comboBoxProfilesAudio->currentText();
        }else{
            profileName
                    = this->ui->comboBoxProfilesVideo->currentText();
        }
        ML::EncoderInterface
                *encoder
                = this->mediaLearner
                ->getEncoder();
        QString ext
                = encoder->getProfileExt(profileName);
        filePath += "." + ext;
        this->ui->lineEditFilePath
                ->setText(filePath);
        QDir dirPath(filePath);
        dirPath.cd("..");
        QString currentPath = dirPath.path();
        ML::SettingsManagerSingleton::getInstance()
                ->setExtractedVideoPath(
                    currentPath);
    }
}
//====================================
void ExportVideoDialog::onWidthChanged(int width){
    if(!this->changingSize){
        this->changingSize = true;
        int keepRatio
                = this->ui->checkBoxKeepRatio->isChecked();
        if(keepRatio){
            int newHeight
                    = width
                    * this->originalVideoSize.height()
                    / this->originalVideoSize.width();
            this->ui->spinBoxHeight->setValue(newHeight);
        }
        this->changingSize = false;
    }
}
//====================================
void ExportVideoDialog::onHeightChanged(int height){
    if(!this->changingSize){
        this->changingSize = true;
        int keepRatio
                = this->ui->checkBoxKeepRatio->isChecked();
        if(keepRatio){
            int newWidth
                    = height
                    * this->originalVideoSize.width()
                    / this->originalVideoSize.height();
            this->ui->spinBoxWidth->setValue(newWidth);
        }
        this->changingSize = false;
    }
}
//====================================
