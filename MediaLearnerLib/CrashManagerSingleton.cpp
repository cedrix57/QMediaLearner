#include "CrashManagerSingleton.h"

namespace ML{

//====================================
QSharedPointer<CrashManagerSingleton>
CrashManagerSingleton::instance;
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
QString keyHasCrashed = "hasCrashed";
//====================================
bool CrashManagerSingleton::getHasCrashed(){
    bool defaultHasCrashed = false;
    bool hasCrashed
        = this->settings->value(
        keyHasCrashed,
        defaultHasCrashed).toBool();
    return hasCrashed;
}
//====================================
void CrashManagerSingleton::setHasCrashed(bool hasCrashed){
    this->settings->setValue(keyHasCrashed, hasCrashed);
}
//===================================================
QString keyMediaPosition = "mediaPosition";
//===================================================
qint64 CrashManagerSingleton::getMediaPosition(){
    qint64 defaultMediaPosition = 0;
    qint64 mediaPosition
        = this->settings->value(
        keyMediaPosition,
        defaultMediaPosition).toLongLong();
    return mediaPosition;
}
//===================================================
void CrashManagerSingleton::setMediaPosition(qint64 mediaPosition){
    this->settings->setValue(
                keyMediaPosition,
                mediaPosition);
}
//===================================================
QString keyMediaFilePath = "mediaFilePath";
//===================================================
QString CrashManagerSingleton::getMediaFilePath(){
    QString defaultMediaFilePath = "";
    QString mediaFilePath
        = this->settings->value(
        keyMediaFilePath,
        defaultMediaFilePath).toString();
    return mediaFilePath;
}
//===================================================
void CrashManagerSingleton::setMediaFilePath(QString mediaFilePath){
    this->settings->setValue(
                keyMediaFilePath,
                mediaFilePath);
}
//===================================================
QString keySubtitle = "subtitle(";
//===================================================
QString CrashManagerSingleton::getSubtitle(int position){
    QString defaultSubtitle = "";
    QString subtitle
        = this->settings->value(
        keySubtitle + QString::number(position),
        defaultSubtitle).toString();
    return subtitle;
}
//===================================================
void CrashManagerSingleton::setSubtitle(
        int position,
        QString subtitle){
    this->settings->setValue(
                keySubtitle + QString::number(position),
                subtitle);
}
//===================================================
QString keySubtitleEnabled = "subtitleEnabled";
//===================================================
bool CrashManagerSingleton::getSubtitleEnabled(
        int position){
    bool defaultSubtitleEnabled = 0;
    bool subtitleEnabled
        = this->settings->value(
        keySubtitleEnabled,
        defaultSubtitleEnabled).toBool();
    return subtitleEnabled;
}
//===================================================
void CrashManagerSingleton::setSubtitleEnabled(
        int position,
        bool subtitleEnabled){
    this->settings->setValue(
                keySubtitleEnabled,
                subtitleEnabled);
}
//===================================================
QString keySubtitleShift = "subtitleShift";
//===================================================
int CrashManagerSingleton::getSubtitleShift(
        int position){
    int defaultSubtitleShift = 0;
    int subtitleShift
        = this->settings->value(
        keySubtitleShift + QString::number(position),
        defaultSubtitleShift).toInt();
    return subtitleShift;
}
//===================================================
void CrashManagerSingleton::setSubtitleShift(
        int position,
        int subtitleShift){
    this->settings->setValue(
                keySubtitleShift
                + QString::number(position),
                subtitleShift);
}
//===================================================
QString keyPlaybackRate = "playbackRate";
//===================================================
double CrashManagerSingleton::getPlaybackRate(){
    double defaultPlaybackRate = 0;
    double playbackRate
        = this->settings->value(
        keyPlaybackRate,
        defaultPlaybackRate).toFloat();
    return playbackRate;
}
//===================================================
void CrashManagerSingleton::setPlaybackRate(
        double playbackRate){
    this->settings->setValue(
                keyPlaybackRate,
                playbackRate);
}
//===================================================
QString keyVolume = "volume";
//===================================================
int CrashManagerSingleton::getVolume(){
    int defaultVolume = 0;
    int volume
        = this->settings->value(
        keyVolume,
        defaultVolume).toInt();
    return volume;
}
//===================================================
void CrashManagerSingleton::setVolume(int volume){
    this->settings->setValue(keyVolume, volume);
}
//===================================================
QString keySequence = "sequence";
QString keyNSequences = "nSequences";
//===================================================
QSharedPointer<QList<Sequence> >
CrashManagerSingleton::getSequences(){
    //QSharedPointer<QList<Sequence> > defaultSequence = 0;
    QSharedPointer<QList<Sequence> > sequences(
                new QList<Sequence>);
    int nSequences
            = this->settings->value(keyNSequences).toInt();
    ML::Sequence sequence;
    for(int i=0; i<nSequences; i++){
        sequence.beginInMs
                = this->settings->value(
                    keySequence + "L" + QString::number(i),
                    0).toLongLong();
        sequence.endInMs
                = this->settings->value(
                    keySequence + "R" + QString::number(i),
                    0).toLongLong();
        *sequences << sequence;
    }
        //= this->settings->value(
        //keySequence,
        //defaultSequence)
            //.toQSharedPointer<QList<Sequence> >();
    return sequences;
}
//===================================================
void CrashManagerSingleton::setSequences(
        QSharedPointer<QList<Sequence> > sequences){
    int nSequences = sequences->size();
    this->settings->setValue(
            keyNSequences,
            nSequences);
    int i = 0;
    for(QList<Sequence>::iterator it
        = sequences->begin();
        it != sequences->end();
        ++it){
        this->settings->setValue(
                    keySequence + "L" + QString::number(i),
                    it->beginInMs);
        this->settings->setValue(
                    keySequence + "R" + QString::number(i),
                    it->endInMs);
        i++;
    }
}
//===================================================

}
