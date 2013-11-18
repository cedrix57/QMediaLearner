#ifndef EDITEXTRACTIONDIALOG_H
#define EDITEXTRACTIONDIALOG_H

#include <QDialog>
#include <MediaLearnerLib.h>

class ExportVideoDialog;

namespace Ui {
class EditExtractionDialog;
}

class EditExtractionDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit EditExtractionDialog(
            MediaLearner::MediaLearnerLib
            *mediaLearner,
            QWidget *parent = 0);
    ~EditExtractionDialog();

public slots:
    void addSequence(
            MediaLearner::Sequence sequence);
    void playOrPause();
    void removeSelectedSequence();
    void exportVideo();
    void accept();
    void reject();

protected slots:
    void _onSelectionChanged(int index);
    void _onMediaPlayerStateChanged(
            QMediaPlayer::State state);
    void _onPositionChanged(
            qint64 position);
    void _onPositionSliderMoved(
            int position);
    void _onLowerBoundarySliderChanged(
            int position);
    void _onUpperBoundarySliderChanged(
            int position);

protected:
    Ui::EditExtractionDialog *ui;
    MediaLearner::MediaLearnerLib *mediaLearner;
    QMediaPlayer *mediaPlayer;
    QSharedPointer<QList<MediaLearner::Sequence> >
            backupSequences;
    ExportVideoDialog *exportVideoDialog;
    void _connectSlots();
    void _loadExtractions();
    void _hideExportButtonEventually();
    void _initVideoPlayer();
    void showEvent(QShowEvent * event);
    void _adjustUpperBoundary(
            int position);
    void _adjustLowerBoundary(
            int position);
};

#endif // EDITEXTRACTIONDIALOG_H
