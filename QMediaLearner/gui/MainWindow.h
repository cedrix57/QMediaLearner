#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <MediaLearnerLib.h>
#include "dialogs/EditExtractionDialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    //Media
    void openFile();
    void openFiles();
    void close();

    //Bottom tool bar
    void playOrPause();
    void stop();
    void repeatMode(bool enabled);
    void editExtractions();
    void extract();
    void faster();
    void slower();

    //Tools
    void showPlayingList();
    void showVideoEffects();
    void showAudioEffects();
    void showSettings();

    //Help
    void help();
    void about();

protected:
    Ui::MainWindow *ui;
    MediaLearner::MediaLearnerLib mediaLearner;
    QMediaPlayer *mediaPlayer;
    EditExtractionDialog *editExtractionDialog;
    void _initMediaPlayer();
    void _initExtractor();
    void _connectSlots();
    void _connectMediaPlayerSlots();
    void _connectToolBarSlots();
    void _connectMenaBarSlots();

protected slots:
    void _onMediaPlayerStateChanged(
            QMediaPlayer::State state);
    void _onPlaybackRateChanged(
            qreal rate);
    void _onDurationChanged(
            qint64 duration);
    void _onPositionChanged(
            qint64 position);
    void _onPositionSliderMoved(
            int position);
    void _setVolume(
            int percentage);
};

#endif // MAINWINDOW_H
