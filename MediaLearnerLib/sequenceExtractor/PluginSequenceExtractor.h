#ifndef PLUGINSEQUENCEEXTRACTOR_H
#define PLUGINSEQUENCEEXTRACTOR_H

#include <QObject>
#include <QMap>
#include <QAudioBuffer>
#include <QVariant>
#include <QSharedPointer>
#include <QMutex>
#include <QAudioDecoder>
#include <QTime>

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
    double mean;
    double var;
    double sd;
    //float deciles[11];
    //TODO quantiles et dérivés
    void init(QAudioBuffer &qtbuffer);
};
struct Sequence{
    long minInMs;
    long maxInMs;
    QString toString(){
        QTime minTime(0, 0, 0, 0);
        minTime
                = minTime
                .addMSecs(
                    this->minInMs + 500);
        QTime maxTime(0, 0, 0, 0);
        maxTime
                = maxTime
                .addMSecs(
                    this->maxInMs + 500);
        QString res
                = minTime.toString("hh:mm:ss");
        res += " -> ";
        res += maxTime.toString("hh:mm:ss");
        return res;
    }
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
    virtual void reset();
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
    virtual void analyseBuffer(
            QAudioBuffer audioBuffer) = 0;
    void onBufferReady(
            QAudioDecoder *audioDecoder);
    int getValue(
            QAudioBuffer &audioBuffer,
            QAudioFormat::SampleType &sampleType,
            int framePosition,
            int nChannels,
            int bytesPerFrame,
            int bytesPerValue);
    bool isAnalysing();
    QSharedPointer<QList<Sequence> >
            getExtractedSequences();
    qint64 getCurrentTimeStampInMs();

signals:

public slots:
    
protected:
    QMap<QString, QVariant> parameterValues;
    static QList<PluginSequenceExtractor*>
        extractors;
    QSharedPointer<QList<Sequence> > extractedSequences;
    qint64 _currentTimeStampInMs;
    bool _isAnalysing;
    QMutex _mutexAnalyse;
};

}

#endif // PLUGINSEQUENCEEXTRACTOR_H
