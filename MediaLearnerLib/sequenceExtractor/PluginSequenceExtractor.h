#ifndef PLUGINSEQUENCEEXTRACTOR_H
#define PLUGINSEQUENCEEXTRACTOR_H

#include <QObject>
#include <QMap>
#include <QAudioBuffer>
#include <QVariant>
#include <QSharedPointer>

namespace MediaLearner{

struct SharedBuffer{
    int *buffer;
    SharedBuffer(){
        this->buffer = NULL;
    }
    ~SharedBuffer(){
        delete[] this->buffer;
    }
};


struct AudioBuffer{
    //int *buffer;
    QSharedPointer<SharedBuffer> buffer;
    int bufferSize;
    int hzFreq;
    int durationInMs;
    float mean;
    float var;
    //float deciles[11];
    //TODO quantiles et dérivés
    void init(QAudioBuffer &qtbuffer);
};
struct Sequence{
    long minInMs;
    long maxInMs;
};
struct Parameter{
    QString name;
    QVariant defaultValue;
};

class PluginSequenceExtractor : public QObject{
    Q_OBJECT

public:
    explicit PluginSequenceExtractor(
            QObject *parent = 0);
    static QMap<QString, PluginSequenceExtractor*>
        getExtractors();
    virtual QList<Parameter> getParameters();
    virtual void setParameterValues(
            QMap<QString, QVariant> parameterValues);
    virtual void setParameterValue(
            QString key,
            QVariant value);
    virtual QString getName() = 0;
    virtual QString getDescription() = 0;
    virtual QList<Sequence> extractSequences(
            AudioBuffer buffer,
            int positionInMs) = 0;

signals:

public slots:
    
protected:
    QMap<QString, QVariant> parameterValues;
    static QList<PluginSequenceExtractor*>
        extractors;
};

}

#endif // PLUGINSEQUENCEEXTRACTOR_H
