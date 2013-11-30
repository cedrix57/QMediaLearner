#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <QString>

namespace MediaLearner{

struct Sequence{
    Sequence();
    long beginInMs;
    long endInMs;
    bool isNull();
    bool contains(long value);
    bool intersect(Sequence &other);
    int getDuration();
    virtual void project(Sequence &other);
    virtual void substract(int ms);
    QString toString();
    virtual ~Sequence();
};

}

#endif // SEQUENCE_H
