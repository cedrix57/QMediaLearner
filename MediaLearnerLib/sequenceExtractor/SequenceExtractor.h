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
#include "medialearnerlib_global.h"
#include "PluginSequenceExtractor.h"

namespace ML{

class MEDIALEARNERLIBSHARED_EXPORT
        SequenceExtractor : public QObject{
    Q_OBJECT
public:
    explicit SequenceExtractor(QObject *parent = 0);
    void reset();
    void stop();
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
    void removeSequence(
            int position);
    void changeSequence(
            int position,
            Sequence newSequence);
    void changeMinSequence(
            int position,
            int newMin);
    void changeMaxSequence(
            int position,
            int newMax);
    void selectSequence(
            int position);
    void setSequences(
            QSharedPointer<
            QList<Sequence> >
            sequences);
    Sequence getSelectedSequence();
    QMediaPlayer *getMediaPlayer();
    static void loadExtractors();
    static void closeExtractors();

signals:
    void sequenceExtracted(
            ML::Sequence sequence);
    void sequencesExtracted();
    void sequencesExtractionFailed();
    void numberOfSequencesChanged(int n);

public slots:
    void sortAndMergeSequences();

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
    //static QList<PluginSequenceExtractor*>
        //extractors;
};

}

#endif // SEQUENCEEXTRACTOR_H
