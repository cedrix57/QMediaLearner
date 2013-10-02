#ifndef SUBTITLESMANAGER_H
#define SUBTITLESMANAGER_H

#include <QObject>
#include <QFont>
#include <QColor>

namespace MediaLearner{

struct SubtitleDrawSettings{
    QFont font;
    QColor fontColor;
    SubtitleDrawSettings(){
        this->fontColor = Qt::yellow;
    }
};

class SubtitlesManager : public QObject{
    Q_OBJECT
public:
    static const int N_MAX_TRACKS = 5;
    explicit SubtitlesManager(QObject *parent = 0);
    int getNTracks();
    void setTrack(QString subtitleFilePath);
    void setTrack(QString subtitleFilePath, int position);
    void setSubtitleDrawSettings(
            int position,
            SubtitleDrawSettings &subtitleDrawSettings);
    SubtitleDrawSettings getSubtitleDrawSettings(int position);

signals:
    
public slots:

protected:
    QList<SubtitleDrawSettings> allSubtitleDrawSettings;
};

}
#endif // SUBTITLESMANAGER_H
