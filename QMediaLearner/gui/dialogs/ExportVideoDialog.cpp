#include "ExportVideoDialog.h"
#include "ui_ExportVideoDialog.h"
#include <QFileDialog>

//====================================
ExportVideoDialog::ExportVideoDialog(
        MediaLearner::MediaLearnerLib
        *mediaLearner,
        QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportVideoDialog){
    ui->setupUi(this);
    this->mediaLearner
            = mediaLearner;
    this->progressDialog.setWindowTitle(
                tr("Encoding..."));
    this->loadInfos();
    MediaLearner::EncoderInterface
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
}
//====================================
void ExportVideoDialog::loadInfos(){
    MediaLearner::EncoderInterface
            *encoder
            = this->mediaLearner
            ->getEncoder();
    QMap<QString, MediaLearner::EncodingInfo>
            formatProfiles
            = encoder->getAvailableFormatProfiles();
    foreach(QString profile, formatProfiles.keys()){
        this->ui->comboBoxProfiles->addItem(
                    profile);
    }
    MediaLearner::SubtitlesManager *subManager
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
}
//====================================
ExportVideoDialog::~ExportVideoDialog(){
    delete ui;
}
//====================================
void ExportVideoDialog::accept(){
    MediaLearner::EncoderInterface
            *encoder
            = this->mediaLearner
            ->getEncoder();
    //*
    MediaLearner::SequenceExtractor
            *sequenceExtractor
            = this->mediaLearner
            ->getSequenceExtractor();
    MediaLearner::SubtitlesManager
            *subtitlesManager
            = this->mediaLearner
            ->getSubtitlesManager();
    QSharedPointer<QList<MediaLearner::Sequence> >
            sequences
            = sequenceExtractor
            ->getExtractedSequences();
    QSize videoSize
            = encoder->getOriginalSize();
    MediaLearner::SubtitleTrack*
            subs
            = subtitlesManager
            ->getSubtitleTracks();
    this->sequencesWithSubs.init(
                *sequences,
                subs);
    this->sequencesWithSubs.setScreenSize(
                videoSize);
    QString outVideoFilePath
            = this->ui->lineEditFilePath->text();
    QList<MediaLearner::SequenceWithSubs>
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
    QString filePath
            = QFileDialog::getSaveFileName(
                this,
                tr("Choose a new file"));
    if(!filePath.isNull()){
        this->ui->lineEditFilePath
                ->setText(filePath);
    }
}
//====================================
