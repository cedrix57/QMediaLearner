#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <MediaLearnerLib.h>

class MediaLearnerLibTests : public QObject{
    Q_OBJECT
    
public:
    MediaLearnerLibTests();
    
private Q_SLOTS:
    void testInitialisation();
    void testExtraction();
};

//====================================
MediaLearnerLibTests::MediaLearnerLibTests(){
}
//====================================
void MediaLearnerLibTests::testInitialisation(){
    MediaLearner::MediaLearnerLib mediaLearner;
    QVERIFY2(true, "Failure");

}
//====================================
void MediaLearnerLibTests::testExtraction(){
    QVERIFY2(true, "Failure");
}
//====================================

QTEST_MAIN(MediaLearnerLibTests)

#include "tst_medialearnerlibtests.moc"
