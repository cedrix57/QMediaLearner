#include "CrashManagerSingleton.h"

namespace ML{

//====================================
CrashManagerSingleton::CrashManagerSingleton(
        QObject *parent) :
    QObject(parent){
    this->settings
            = new QSettings(
                "QMediaRecorder",
                "QMediaRecorderCrashInfo",
                this);
}
//====================================
CrashManagerSingleton *CrashManagerSingleton::getInstance(){
    if(CrashManagerSingleton::instance.isNull()){
        QSharedPointer<CrashManagerSingleton>
                localInstance(
                    new CrashManagerSingleton);
        CrashManagerSingleton::instance = localInstance;
    }
    CrashManagerSingleton *pointer
            = CrashManagerSingleton::instance.data();
    return pointer;
}
//====================================
CrashManagerSingleton::~CrashManagerSingleton(){
}
//====================================

}
