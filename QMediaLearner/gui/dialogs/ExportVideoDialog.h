#ifndef EXPORTVIDEODIALOG_H
#define EXPORTVIDEODIALOG_H

#include <QDialog>
#include <QProgressDialog>
#include <MediaLearnerLib.h>
#include <encoders/FFmpegEncoder.h>
#include <encoders/SequenceWithSubs.h>

namespace Ui {
class ExportVideoDialog;
}

class ExportVideoDialog : public QDialog{
    Q_OBJECT
    
public:
    explicit ExportVideoDialog(
            MediaLearner::MediaLearnerLib
            *mediaLearner,
            QWidget *parent = 0);
    ~ExportVideoDialog();
    
protected:
    Ui::ExportVideoDialog *ui;
    MediaLearner::MediaLearnerLib *mediaLearner;
    MediaLearner::SequencesWithSubs sequencesWithSubs;
    void loadInfos();
    virtual void accept();
    QProgressDialog progressDialog;

public slots:
    void browseNewVideoFilePath();
};

#endif // EXPORTVIDEODIALOG_H
