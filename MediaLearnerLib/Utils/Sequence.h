#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <QString>
#include <QMetaType>

#include "medialearnerlib_global.h"

namespace ML{

struct MEDIALEARNERLIBSHARED_EXPORT
        Sequence{
    Sequence();
    qint64 beginInMs;
    qint64 endInMs;
    bool isNull();
    bool contains(long value) const;
    bool intersect(Sequence &other) const;
    int getDuration();
    virtual void project(Sequence &other);
    virtual void substract(int ms);
    QString toString();
    virtual ~Sequence();
    bool operator==(const Sequence &other);
    bool operator!=(const Sequence &other);
    bool operator<(const Sequence &other) const;
};


}

//Q_DECLARE_METATYPE(ML::Sequence)

#endif // SEQUENCE_H
