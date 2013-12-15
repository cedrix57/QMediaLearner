#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <MediaLearnerLib.h>
#include <QThread>
#include <QSignalSpy>
#include <encoders/FFmpegEncoder.h>

class MediaLearnerLibTests : public QObject{
    Q_OBJECT
    
public:
    MediaLearnerLibTests();
    
private Q_SLOTS:
    void testInitialisation();
    void testExtractorExists();
    void testExtractorPluginsExist();
    void testSequencesExtractions();
    void testFFmpegFormats();
    void testFFmpegSubEncoders();
    void testFFmpegVideoEncoders();
    void testFFmpegAudioEncoders();
};
//====================================
MediaLearnerLibTests::MediaLearnerLibTests(){
}
//====================================
void MediaLearnerLibTests::testInitialisation(){
    ML::MediaLearnerLib mediaLearner;
    QVERIFY(true);

}
//====================================
void MediaLearnerLibTests::testExtractorExists(){
    ML::MediaLearnerLib mediaLearner;
    ML::SequenceExtractor *extractor
            = mediaLearner.getSequenceExtractor();
    QVERIFY(extractor != NULL);
}
//====================================
void MediaLearnerLibTests::testExtractorPluginsExist(){
    ML::MediaLearnerLib mediaLearner;
    QMap<QString, ML::PluginSequenceExtractor*>
            availableExtractor
            = ML
            ::PluginSequenceExtractor
            ::getExtractors();
    int nExtractors = availableExtractor.size();
    QVERIFY(nExtractors > 0);
}
//====================================
void MediaLearnerLibTests::testSequencesExtractions(){
    ML::MediaLearnerLib mediaLearner;
    ML::SequenceExtractor *extractor
            = mediaLearner.getSequenceExtractor();
    QMap<QString, ML::PluginSequenceExtractor*>
            availableExtractors
            = ML
            ::PluginSequenceExtractor
            ::getExtractors();
    ML::PluginSequenceExtractor*
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

    //qRegisterMetaType<MediaLearner::Sequence>();
    QSignalSpy spy(
                extractor,
                SIGNAL(sequencesExtracted()));
    extractor->analyseMediaSource();
    QTRY_COMPARE_WITH_TIMEOUT(spy.count(), 1, 50000);

    //extractor->waitForExtractionFinished();
    QSharedPointer<QList<ML::Sequence> >
            sequences
            = extractor->getAllSequences();
    int nSequences = sequences->size();
    qDebug() << "nSequences: " << nSequences;
    QVERIFY(nSequences > 0);
}
//====================================
void MediaLearnerLibTests::testFFmpegFormats(){
    ML::EncoderInterface
            *encoder = new ML::FFmpegEncoder();
    QList<ML::EncodingInfo> formats
            = encoder->getAvailableFormats();
    int nFormats = formats.size();
    QVERIFY(nFormats > 0);
    delete encoder;
}
//====================================
void MediaLearnerLibTests::testFFmpegSubEncoders(){
    ML::EncoderInterface
            *encoder = new ML::FFmpegEncoder();
    QList<ML::EncodingInfo> subEncoders
            = encoder->getAvailableSubtitlesCodecs();
    int nSubEncoders = subEncoders.size();
    QVERIFY(nSubEncoders > 0);
    delete encoder;
}
//====================================
void MediaLearnerLibTests::testFFmpegAudioEncoders(){
    ML::EncoderInterface
            *encoder = new ML::FFmpegEncoder();
    QList<ML::EncodingInfo> audioCodecs
            = encoder->getAvailableAudioCodecs();
    int nAudioCodecs = audioCodecs.size();
    QVERIFY(nAudioCodecs > 0);
    delete encoder;
}
//====================================
void MediaLearnerLibTests::testFFmpegVideoEncoders(){
    ML::EncoderInterface
            *encoder = new ML::FFmpegEncoder();
    QList<ML::EncodingInfo> videoCodecs
            = encoder->getAvailableVideoCodecs();
    int nVideoCodecs = videoCodecs.size();
    QVERIFY(nVideoCodecs > 0);
    delete encoder;
}
//====================================
QTEST_MAIN(MediaLearnerLibTests)

#include "tst_medialearnerlibtests.moc"
