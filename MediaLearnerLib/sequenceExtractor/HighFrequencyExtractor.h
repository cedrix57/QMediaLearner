#ifndef HIGHFREQUENCYEXTRACTOR_H
#define HIGHFREQUENCYEXTRACTOR_H

#include "PluginSequenceExtractor.h"
#include <QQueue>

namespace ML{

class HighFrequencyExtractor
        : public PluginSequenceExtractor{
    Q_OBJECT
public:
    static HighFrequencyExtractor insance;
    explicit HighFrequencyExtractor(
            QObject *parent = 0);
    virtual QString getName();
    virtual QString getDescription();
    virtual void reset();
    virtual void analyseBuffer(
            QAudioBuffer audioBuffer);

protected:
    int nLowerFrequency;
    int nHighFrequency;
    qint64 sumMean;
    qint64 sumSmallerMean;
    qint64 nIteration;
    QQueue<int> lastValues;
    bool inPhrase;
    int meanAtPhraseBegin;
    int phraseDuration;
};

}

#endif // HIGHFREQUENCYEXTRACTOR_H
