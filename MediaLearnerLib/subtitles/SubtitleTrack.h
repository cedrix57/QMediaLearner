#ifndef SUBTITLETRACK_H
#define SUBTITLETRACK_H

#include <QObject>
#include <QString>

namespace MediaLearner{


class SubtitleTrack : public QObject
{
    Q_OBJECT
public:
    explicit SubtitleTrack(QObject *parent = 0);
    void parseSubtitleFileName(QString subtitleFileName);
    QString getText(int positionInMs);

    
signals:
    
public slots:
    
protected:
    QString subtitleFileName;
};

}

#endif // SUBTITLETRACK_H

