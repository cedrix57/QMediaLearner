#ifndef FFMPEGINTERFACE_H
#define FFMPEGINTERFACE_H

#include <QObject>

namespace MediaLearner{

class ffmpegInterface : public QObject{
    Q_OBJECT
public:
    explicit ffmpegInterface(QObject *parent = 0);
    
signals:
    
public slots:
    
};

}

#endif // FFMPEGINTERFACE_H
