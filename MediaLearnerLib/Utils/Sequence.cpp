#include "Sequence.h"

#include <QTime>

namespace ML{

//====================================
Sequence::Sequence(){
    this->beginInMs = -1;
    this->endInMs = -1;
}
//====================================
Sequence::~Sequence(){
}
//====================================
bool Sequence::isNull(){
    bool isNull
            = this->beginInMs == -1
            || this->endInMs == -1;
    return isNull;
}
//====================================
bool Sequence::contains(long value){
    bool contains
            = value >= this->beginInMs
            && value <= this->endInMs;
    return contains;
}
//====================================
bool Sequence::intersect(Sequence &other){
    bool intersect
            = this->contains(other.beginInMs)
            || this->contains(other.endInMs)
            || other.contains(this->endInMs);
    return intersect;
}
//====================================
void Sequence::project(Sequence &other){
    if(this->contains(other.beginInMs)){
        this->beginInMs = other.beginInMs;
    }
    if(this->contains(other.endInMs)){
        this->endInMs = other.endInMs;
    }
}
//====================================
void Sequence::substract(int ms){
    this->beginInMs -= ms;
    this->endInMs -= ms;
}
//====================================
int Sequence::getDuration(){
    int duration
            = this->endInMs - this->beginInMs;
    return duration;
}
//====================================
QString Sequence::toString(){
    QTime minTime(0, 0, 0, 0);
    minTime
            = minTime
            .addMSecs(
                this->beginInMs);
    QTime maxTime(0, 0, 0, 0);
    maxTime
            = maxTime
            .addMSecs(
                this->endInMs);
    QString res
            = minTime.toString("hh:mm:ss");
    res += " -> ";
    res += maxTime.toString("hh:mm:ss");
    return res;
}
//====================================
bool Sequence::operator==(
        const Sequence &other){
    bool equals
            = this->beginInMs == other.beginInMs
            && this->endInMs == other.endInMs;
    return equals;
}
//====================================
bool Sequence::operator!=(
        const Sequence &other){
    bool different
            = !(*this == other);
    return different;
}
//====================================
bool Sequence::operator<(
        const Sequence &other) const{
    bool inf = this->beginInMs < other.beginInMs;
    return inf;
}
//====================================
}
