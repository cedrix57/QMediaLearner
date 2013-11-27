#include "FFmpegEncoder.h"

#include <QDir>
#include <QString>
#include <QMap>

namespace MediaLearner{

//====================================
FFmpegEncoder::FFmpegEncoder(QObject *parent) :
    EncoderInterface(parent){
    this->connect(
                &this->encodingProcess,
                SIGNAL(finished(int,QProcess::ExitStatus)),
                SLOT(_onProcessFinished(int,QProcess::ExitStatus)));

}
//====================================
QList<EncodingInfo> FFmpegEncoder::getAvailableFormats(){
    QList<EncodingInfo>
            infos;
    QString ffmpegFilePath
            = this->getFFmpegFilePath();
    QStringList arguments;
    arguments << "-formats";
    QProcess process;
    process.start(
                ffmpegFilePath,
                arguments);
    process.waitForFinished();
    QByteArray bashOutput
            = process.readAllStandardOutput();
    QList<QByteArray> bashRows
            = bashOutput.split('\n');
    foreach(QString bashRow, bashRows){
        if(bashRow[2] == 'E'){
            if(bashRow[4] != '='){
                int lenRow = bashRow.length();
                bashRow = bashRow.right(lenRow - 4);
                QString row = bashRow.trimmed();
                QStringList rowElements
                        = row.split(" ");
                EncodingInfo info;
                info.name = rowElements.takeFirst();
                info.description = rowElements.join(" ");
                infos << info;
            }
        }
    }
    return infos;
}
//====================================
QString FFmpegEncoder::getFFmpegFilePath(){
    QDir currentPath = QDir::currentPath();
    QString ffmpegFilePath
            = currentPath.filePath("ffmpeg");
    return ffmpegFilePath;
}
//====================================
QList<EncodingInfo> FFmpegEncoder::getAvailableVideoCodecs(){
    QList<EncodingInfo> infos
            = this->_getAvailableCodecs(
                'V');
    return infos;
}
//====================================
QList<EncodingInfo> FFmpegEncoder::_getAvailableCodecs(
        QChar type){
    QList<EncodingInfo>
            infos;
    QString ffmpegFilePath
            = this->getFFmpegFilePath();
    QStringList arguments;
    arguments << "-encoders";
    QProcess process;
    process.start(
                ffmpegFilePath,
                arguments);
    process.waitForFinished();
    QByteArray bashOutput
            = process.readAllStandardOutput();
    QList<QByteArray> bashRows
            = bashOutput.split('\n');
    foreach(QString bashRow, bashRows){
        if(bashRow[1] == type){
            if(bashRow[8] != '='){
                int lenRow = bashRow.length();
                bashRow = bashRow.right(lenRow - 8);
                QString row = bashRow.trimmed();
                QStringList rowElements
                        = row.split(" ");
                EncodingInfo info;
                info.name = rowElements.takeFirst();
                info.description = rowElements.join(" ");
                infos << info;
            }
        }
    }
    return infos;
}
//====================================
QList<EncodingInfo> FFmpegEncoder::getAvailableAudioCodecs(){
    QList<EncodingInfo> infos
            = this->_getAvailableCodecs(
                'A');
    return infos;
}
//====================================
QList<EncodingInfo> FFmpegEncoder::getAvailableSubtitlesCodecs(){
    QList<EncodingInfo> infos
            = this->_getAvailableCodecs(
                'S');
    return infos;
}
//====================================
int FFmpegEncoder::getFps(){
    QString ffmpegFilePath
            = this->getFFmpegFilePath();
    QStringList arguments;
    arguments << "-i";
    arguments << this->inVideoFilePath;
    QProcess process;
    process.start(
                ffmpegFilePath,
                arguments);
    process.waitForFinished();
    QString bashOutput
            = process.readAllStandardOutput()
            + process.readAllStandardError();
    bashOutput = bashOutput.split(" fps").first();
    bashOutput = bashOutput.split(" ").last();
    int fps = bashOutput.toInt();
    return fps;
}
//====================================
int FFmpegEncoder::getNFrame(qint64 ms, int frameRate){
    float frameRateMs = frameRate / 1000.0;
    int n = ms * frameRateMs + 0.5;
    return n;
}
//====================================
QString FFmpegEncoder::getFormatedTime(qint64 ms){
    QTime time(0, 0, 0, 0);
    time = time.addMSecs(ms);
    QString formatedTime
            = time.toString("hh:mm:ss.z");
    return formatedTime;
}
//====================================
void FFmpegEncoder::encode(QString outFilePath){
    QMap<QString, QString> fontFilePaths;
    this->argumentsList.clear();
    this->fps = this->getFps();
    this->_encodeTempVideoCommand();
    /*
    foreach(QList<DrawingSubtitleInfo> infos,
            this->texts){
        this->_encodeSequenceCommand(
                    infos);
    }
    //*/
            //add texts
    /*
    QString ffmpegFilePath
            = this->getFFmpegFilePath();
    QString commandLine
            = ffmpegFilePath
            + " " + arguments.join(' ');
    qDebug() << commandLine;
    this->encodingProcess.start(
                ffmpegFilePath,
                arguments);
                //*/
}
//====================================
void FFmpegEncoder::_encodeTempVideoCommand(){
    QDir ffmpegTempDir("ffmpegTemp");
    if(ffmpegTempDir.exists()){
        ffmpegTempDir.mkpath(".");
    }
    this->tempInVideoFilePath
             = ffmpegTempDir.filePath(
                "temp.mpg");
    QStringList arguments;
    arguments << "-i";
    arguments << this->inVideoFilePath;
    arguments << "-b";
    arguments << "2250k";
    arguments << "-minrate";
    arguments << "2250k";
    arguments << "-maxrate";
    arguments << "2250k";
    arguments << "-bufsize";
    arguments << "1000k";
    arguments << this->tempInVideoFilePath;
    this->argumentsList << arguments;
}
//====================================
void FFmpegEncoder::_encodeSequenceCommand(
        QList<SequencesWithSubs> &sequencesWithSubs){
    /*
    QStringList arguments;
    arguments << "-i";
    arguments << this->tempInVideoFilePath;
    QStringList filterParams;
    foreach(DrawingSubtitleInfo info,
            infos){
        DrawingSettings drawingSettings
                = info.text.getDrawingSettings();
        QString drawTextParam = "drawtext=";
        drawTextParam += "\"text=";
        QString lines = info.text.getLines().join('\n');
        drawTextParam += "'" + lines + "'";
        drawTextParam += ":";
        drawTextParam += "fontfile=";
        QString fontFamily = drawingSettings.font.family();
        if(!fontFilePaths.contains(
                    fontFamily)){
            fontFilePaths[fontFamily]
                    = this->getFontPath(
                        fontFamily);
        }
        //QString fontPath = fontFilePaths[fontFamily];
        QString fontPath = "/usr/share/fonts/truetype/msttcorefonts/Arial.ttf";
        drawTextParam += fontPath;
        drawTextParam += ":";
        drawTextParam += "fontcolor=";
        drawTextParam += drawingSettings.fontColor.name();
        drawTextParam += ":";
        drawTextParam += "x=";
        int x = info.text.getRect().x();
        drawTextParam += QString::number(x);
        drawTextParam += ":";
        drawTextParam += "y=";
        int y = info.text.getRect().y();
        drawTextParam += QString::number(y);
        drawTextParam += ":";
        drawTextParam += "fontsize=";
        drawTextParam += QString::number(drawingSettings.font.pixelSize());
        int begin = this->getNFrame(
                    info.startPosition,
                    this->frameRate);
        int end = this->getNFrame(
                    info.endPosition,
                    this->frameRate);
        drawTextParam += ":";
        drawTextParam += "enable=";
        drawTextParam += "'between(n,"
                + QString::number(begin)
                + ","
                + QString::number(end)
                + ")'";
        drawTextParam += "\"";
        filterParams << drawTextParam;
        arguments << "-ss";
        arguments << this->getFormatedTime(
                                info.startPosition);
        arguments << "-to";
        arguments << this->getFormatedTime(
                                info.endPosition);
    }
    arguments << "-vf";
    QString filterParamsConcatened = filterParams.join(",");
    arguments << filterParamsConcatened;
    arguments << outFilePath;
    this->argumentsList << arguments;
    //*/
}
//====================================
void FFmpegEncoder::_onProcessFinished(
        int exitCode,
        QProcess::ExitStatus exitStatus){
    QString bashOutput
            = this->encodingProcess
            .readAllStandardError()
            + "\n"
            + this->encodingProcess
            .readAllStandardOutput();
    qDebug() << "encoding finished.";
    qDebug() << bashOutput;
    if(exitCode == 0){
        this->encodingFinished();
    }else{
        this->encodingFailed();
    }
}
//====================================
QSize FFmpegEncoder::getSize(){
    this->_evalSizeEventually();
    return this->size;
}
//====================================
void FFmpegEncoder::_evalSizeEventually(){
    if(this->size.isEmpty()){
        QString ffmpegFilePath
                = this->getFFmpegFilePath();
        QStringList arguments;
        arguments << "-i";
        arguments << this->inVideoFilePath;
        QProcess process;
        process.start(
                    ffmpegFilePath,
                    arguments);
        process.waitForFinished();
        QString bashOutput
                = process.readAllStandardOutput()
                + process.readAllStandardError();
        QRegExp sizeReg(" \\d+x\\d+ ");
        sizeReg.indexIn(bashOutput);
        bashOutput = sizeReg.capturedTexts().first();
        QStringList sizeStringList
                = bashOutput.trimmed().split("x");
        int width = sizeStringList.first().toInt();
        int height = sizeStringList.last().toInt();
        this->size = QSize(width, height);
        //int pos = bashOutput.indexOf(sizeReg);
        //int bashOutpoutLen = bashOutput.size();
    }
}
//====================================


}
