#ifndef HIGHFREQUENCYEXTRACTOR_H
#define HIGHFREQUENCYEXTRACTOR_H

#include "PluginSequenceExtractor.h"

namespace MediaLearner{

class HighFrequencyExtractor
        : public PluginSequenceExtractor{
    Q_OBJECT
public:
    static HighFrequencyExtractor insance;
    explicit HighFrequencyExtractor(
            QObject *parent = 0);
    virtual QString getName();
    virtual QString getDescription();
    virtual QList<Sequence> extractSequences(
            AudioBuffer buffer,
            int positionInMs);

};

}

#endif // HIGHFREQUENCYEXTRACTOR_H
