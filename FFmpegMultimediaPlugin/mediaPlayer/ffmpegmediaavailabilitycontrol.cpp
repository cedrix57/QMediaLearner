#include "ffmpegmediaavailabilitycontrol.h"

//====================================
FFmpegMediaAvailabilityControl::FFmpegMediaAvailabilityControl(
        QObject *parent) :
    QMediaAvailabilityControl(parent){
    this->status = QMultimedia::Available;
}
//====================================
QMultimedia::AvailabilityStatus
FFmpegMediaAvailabilityControl::availability() const{
    return this->status;
}
//====================================
