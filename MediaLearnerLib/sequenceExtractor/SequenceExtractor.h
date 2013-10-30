#ifndef SEQUENCEEXTRACTOR_H
#define SEQUENCEEXTRACTOR_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QThread>
#include <QQueue>
#include <QMutex>
#include <QAudioDecoder>
#include <QDateTime>
#include <QFutureWatcher>
#include <QMediaPlayer>
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
    getAvailableExtractors();
    void setExtractor(PluginSequenceExtractor *extractor);
    void setExtractor(QString name);
    PluginSequenceExtractor *getSelectedExtractor();
    void setMaxShifts(int lowerMaxInMs, int upperMaxInMs);
    void setMediaSource(QString filePath);
    void analyseMediaSource();
    void extractSequence(int position);
    QSharedPointer<
    QList<Sequence> > getExtractedSequences();
    QSharedPointer<
    QList<Sequence> > getAllSequences();
    bool isExtractionFinished();
    void waitForExtractionFinished();
    void deleteSequence(
            int position);
    void changeSequence(
            int position,
            Sequence newSequence);
    void selectSequence(
            int position);
    Sequence getSelectedSequence();
    QMediaPlayer *getMediaPlayer();

signals:
    void sequenceExtracted(
            MediaLearner::Sequence sequence);
    
public slots:

protected slots:
    void extractQueuedSequences();
    //void onBufferReady();
    void onBufferReadyThread();
    void onDecodingFinished();
    void _onPlayerPositionChanged(
            qint64 position);
    void _onMediaChanged(
            QMediaContent mediaContent);

protected:
    PluginSequenceExtractor* selectedExtractor;
    QAudioDecoder decoder;
    QSharedPointer<
    QList<Sequence> > allSequences;
    QSharedPointer<
    QList<Sequence> > extractedSequences;
    void _connectSlots();
    bool _isExtractionFinished;
    int upperNaxShiftMs;
    int lowerMaxShiftMs;
    int durationProcessedInMs;
    int durationInMs;
    QQueue<int> sequencesToExtract;
    QMutex mutexBuffer;
    QFuture<void> futurBuffer;
    QMediaPlayer mediaPlayer;
    int selectedSequence;
    bool _pausing;
};

}

#endif // SEQUENCEEXTRACTOR_H
