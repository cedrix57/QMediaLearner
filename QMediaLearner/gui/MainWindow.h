#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <MediaLearnerLib.h>

class SettingsDialog;
class EditExtractionDialog;
class CurrentSessionSettingsDialog;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void reloadAfterCrashEventually();
    void reloadAfterCrash();

public slots:
    //Media
    void openFile();
    void openFiles();
    void close();

    //Subtitles
    void openSubtrack(int position);
    void openSubtrack(
            int position,
            QString filePath);
    void openSubtrack1();
    void openSubtrack2();
    void openSubtrack3();
    void setEnabledSubtrack(
            int position,
            bool enabled);
    void setEnabledSubtrack1(bool enable);
    void setEnabledSubtrack2(bool enable);
    void setEnabledSubtrack3(bool enable);

    //Bottom tool bar
    void playOrPause();
    void playVideo(QString fileName);
    void openUrls(QList<QUrl> urls);
    void stop();
    void repeatMode(bool enabled);
    void editExtractions();
    void extract();
    void faster();
    void slower();

    //Tools
    void showPlayingList();
    void showCurrentSessionSettings();
    void showSettings();

    //Help
    void help();
    void about();

    //Other
    void seekLater(int position, int ms = 100);

protected:
    Ui::MainWindow *ui;
    ML::MediaLearnerLib mediaLearner;
    QMediaPlayer *mediaPlayer;
    EditExtractionDialog *editExtractionDialog;
    SettingsDialog *settingsDialog;
    int _toSeek;
    CurrentSessionSettingsDialog
    *currentSettingsDialog;
    void _initMediaPlayer();
    void _initExtractor();
    void _connectSlots();
    void _connectMediaPlayerSlots();
    void _connectToolBarSlots();
    void _connectMenaBarSlots();
    //virtual void dragEnterEvent(QDragEnterEvent *event);
    //virtual void dropEvent(QDropEvent *event);
    virtual bool eventFilter(QObject *object, QEvent *event);
    virtual void closeEvent(QCloseEvent * event);
    virtual void keyPressEvent(QKeyEvent *event);
    bool _stopping;

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
    void _onNumberOfSequencesChanged(
            int nSequences);
    void _seekLater();
};

#endif // MAINWINDOW_H
