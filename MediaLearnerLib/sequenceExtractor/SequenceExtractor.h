#ifndef SEQUENCEEXTRACTOR_H
#define SEQUENCEEXTRACTOR_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QThread>
#include <QQueue>
#include <QMutex>
#include <QAudioDecoder>
//#include <QtConcurrentRun>
//#include <QFutureWatcher>
#include "PluginSequenceExtractor.h"

namespace MediaLearner{

class SequenceExtractor : public QObject{
    Q_OBJECT
public:
    explicit SequenceExtractor(QObject *parent = 0);
    void reset();
    QMap<QString, PluginSequenceExtractor*>
    getAvailableExtractor();
    void setExtractor(PluginSequenceExtractor *extractor);
    void setExtractor(QString name);
    PluginSequenceExtractor *getSelectedExtractor();
    void setMaxShifts(int lowerMaxInMs, int upperMaxInMs);
    void setMediaSource(QString filePath);
    void analyseMediaSource();
    void extractSequence(int position);
    QList<Sequence> getExtractedSequences();

signals:
    void sequenceExtracted(Sequence sequence);
    
public slots:

protected slots:
    void extractQueuedSequences();
    void onBufferReady();
    void onBufferReadyThread();

protected:
    PluginSequenceExtractor* selectedExtractor;
    QAudioDecoder decoder;
    QList<Sequence> allSequences;
    QList<Sequence> extractedSequences;
    void _connectSlots();
    int upperNaxShiftMs;
    int lowerMaxShiftMs;
    int durationProcessedInMs;
    int durationInMs;
    QQueue<int> sequencesToExtract;
    QMutex mutexBuffer;
};

}

#endif // SEQUENCEEXTRACTOR_H
