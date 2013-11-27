#include "ExportVideoDialog.h"
#include "ui_ExportVideoDialog.h"

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
    //TODO setInVideoFilePath
    /*
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
            = encoder->getSize();
    QList<QList<MediaLearner::DrawingSubtitleInfo > >
            texts
            = subtitlesManager
            ->getSubsAt(
                sequences,
                videoSize);
    encoder->setSequences(
                sequences);
    encoder->setTexts(
                texts);
    QString outVideoFilePath
            = this->ui->lineEditFilePath->text();
    encoder->encode(
                outVideoFilePath);
    this->progressDialog.show();
    //*/
    //TODO progress dialog
}
//====================================
