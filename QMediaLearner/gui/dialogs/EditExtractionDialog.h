#ifndef EDITEXTRACTIONDIALOG_H
#define EDITEXTRACTIONDIALOG_H

#include <QDialog>
#include <MediaLearnerLib.h>

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
    void _connectSlots();
    void _loadExtractions();
    void _initVideoPlayer();
    QMediaPlayer *mediaPlayer;
};

#endif // EDITEXTRACTIONDIALOG_H
