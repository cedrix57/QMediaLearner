#include "SubtitlesManager.h"

namespace MediaLearner{

//====================================
SubtitlesManager::SubtitlesManager(QObject *parent) :
    QObject(parent){
}
//====================================
void SubtitlesManager::setTrack(QString subtitleFilePath){
    this->setTrack(subtitleFilePath, 0);
}
//====================================
void SubtitlesManager::setTrack(QString subtitleFilePath, int position){
    //TODO
}
//====================================

}
