#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <MediaLearnerLib.h>
#include <QThread>s

class MediaLearnerLibTests : public QObject{
    Q_OBJECT
    
public:
    MediaLearnerLibTests();
    
private Q_SLOTS:
    void testInitialisation();
    void testExtractorExists();
    void testExtractorPluginsExist();
    void testSequencesExtractions();
};
//====================================
MediaLearnerLibTests::MediaLearnerLibTests(){
}
//====================================
void MediaLearnerLibTests::testInitialisation(){
    MediaLearner::MediaLearnerLib mediaLearner;
    QVERIFY(true);

}
//====================================
void MediaLearnerLibTests::testExtractorExists(){
    MediaLearner::MediaLearnerLib mediaLearner;
    MediaLearner::SequenceExtractor *extractor
            = mediaLearner.getSequenceExtractor();
    QVERIFY(extractor != NULL);
}
//====================================
void MediaLearnerLibTests::testExtractorPluginsExist(){
    MediaLearner::MediaLearnerLib mediaLearner;
    QMap<QString, MediaLearner::PluginSequenceExtractor*>
            availableExtractor
            = MediaLearner
            ::PluginSequenceExtractor
            ::getExtractors();
    int nExtractors = availableExtractor.size();
    QVERIFY(nExtractors > 0);
}
//====================================
void MediaLearnerLibTests::testSequencesExtractions(){
    MediaLearner::MediaLearnerLib mediaLearner;
    MediaLearner::SequenceExtractor *extractor
            = mediaLearner.getSequenceExtractor();
    QMap<QString, MediaLearner::PluginSequenceExtractor*>
            availableExtractors
            = MediaLearner
            ::PluginSequenceExtractor
            ::getExtractors();
    MediaLearner::PluginSequenceExtractor*
            firstExtractor
            = availableExtractors
            .values()
            .first();
    extractor->setExtractor(firstExtractor);
    QString videoFilePath
            = "/home/cedric/Vidéos/musique_21sec.mp4";
    //videoFilePath
            //= "/home/cedric/Vidéos/toBeginAgain.mp3";
    extractor->setMediaSource(
                videoFilePath);
    extractor->analyseMediaSource();
    //*
    //QThread::sleep(5);
    QTime finalTime
            = QTime::currentTime().addSecs(15);
    while(QTime::currentTime() < finalTime){
        QCoreApplication::processEvents(
                    QEventLoop::AllEvents,
                    100);
    }
    //*/
    //extractor->waitForExtractionFinished();
    QSharedPointer<QList<MediaLearner::Sequence> >
            sequences
            = extractor->getAllSequences();
    int nSequences = sequences->size();
    qDebug() << "nSequences: " << nSequences;
    QVERIFY(nSequences > 0);
}
//====================================
QTEST_MAIN(MediaLearnerLibTests)

#include "tst_medialearnerlibtests.moc"
