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
    this->connect(
                &this->encodingProcess,
                SIGNAL(error(QProcess::ProcessError)),
                SLOT(_onProcessError(QProcess::ProcessError)));

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
    this->tempSequenceFilePaths.clear();
    this->fps = this->getFps();
    this->_encodeTempVideoCommand();
    this->_encodeSequenceCommand();
    this->_encodeCuttedSequencesCommand(outFilePath);
    foreach(QStringList l, this->argumentsList){
        QString command = l.join(" ");
        qDebug() << command;
        qDebug() << "---";
    }
    this->_onProcessFinished(
                0,
                QProcess::NormalExit);
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
QString FFmpegEncoder::_getTempFilePath(
        QString fileName){
    QString filePath
            = QDir(QDir::tempPath())
            .filePath(fileName);
    return filePath;
}
//====================================
void FFmpegEncoder::_encodeTempVideoCommand(){
    this->tempInVideoFilePath
            = this->_getTempFilePath(
                "tempmedialearner.mpg");
    QFile(this->tempInVideoFilePath).remove();
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
void FFmpegEncoder::_encodeSequenceCommand(){
    //*
    QStringList filterParams;
    int idSeq = 0;
    for(QList<SequenceWithSubs>::iterator seqIt
            = this->sequencesWithSubs.begin();
            seqIt != this->sequencesWithSubs.end();
            ++seqIt){
        QStringList arguments;
        arguments << "-i";
        arguments << this->tempInVideoFilePath;
        for(QList<SubSequenceDrawable>::iterator subSeqIt
            = seqIt->subSequences.begin();
            subSeqIt != seqIt->subSequences.end();
            ++subSeqIt){
            QList<FittedLine> fittedLines
                    = subSeqIt->getFittedLines();
            DrawingSettings drawingSettings
                    = subSeqIt->getDrawingSettings();
            for(QList<FittedLine>::iterator lineIt
                = fittedLines.begin();
                lineIt != fittedLines.end();
                ++lineIt){

                QString drawTextParam = "drawtext=";
                drawTextParam += "text=";
                drawTextParam += "'" + lineIt->text + "'";
                drawTextParam += ":";
                drawTextParam += "fontfile=";
                /*
                QString fontFamily = drawingSettings.fontFamily;
                if(!fontFilePaths.contains(
                            fontFamily)){
                    fontFilePaths[fontFamily]
                            = this->getFontPath(
                                fontFamily);
                }
                //QString fontPath = fontFilePaths[fontFamily];
                //*/
                QString fontPath = "/usr/share/fonts/truetype/msttcorefonts/Arial.ttf";
                drawTextParam += fontPath;
                drawTextParam += ":";
                drawTextParam += "fontcolor=";
                drawTextParam += drawingSettings.textColor.name();
                drawTextParam += ":";
                drawTextParam += "x=";
                int x = lineIt->position.x();
                drawTextParam += QString::number(x);
                drawTextParam += ":";
                drawTextParam += "y=";
                int y = lineIt->position.y();
                drawTextParam += QString::number(y);
                drawTextParam += ":";
                drawTextParam += "fontsize=";
                int screenHeight = this->size.height();
                int fontSize = drawingSettings.getFontSize(
                            screenHeight);
                drawTextParam += QString::number(fontSize);
                int begin = this->getNFrame(
                            subSeqIt->beginInMs,
                            this->fps);
                int end = this->getNFrame(
                            subSeqIt->endInMs,
                            this->fps);
                drawTextParam += ":";
                drawTextParam += "enable=";
                drawTextParam += "'between(n,"
                        + QString::number(begin)
                        + ","
                        + QString::number(end)
                        + ")'";
                //drawTextParam += "\"";
                filterParams << drawTextParam;
                QString filterParamsConcatened = filterParams.join(",");
                arguments << "-vf";
                arguments << filterParamsConcatened;
            }
        }
        arguments << "-ss";
        arguments << this->getFormatedTime(
                         seqIt->beginInMs);
        arguments << "-to";
        arguments << this->getFormatedTime(
                         seqIt->endInMs);
        QString tempSequenceFileName
                = "tempseqmedialearner_"
                + QString::number(idSeq)
                + ".mpg";
        QString tempSequenceFilePath
            = this->_getTempFilePath(
                tempSequenceFileName);
        QFile(tempSequenceFilePath).remove();
        arguments << tempSequenceFilePath;
        this->tempSequenceFilePaths
                << tempSequenceFilePath;
        this->argumentsList << arguments;
        idSeq++;
    }
}
//====================================
void FFmpegEncoder::_encodeCuttedSequencesCommand(
        QString outFilePath){
    QStringList arguments;
    arguments << "-i";
    QString concatArg
            = "concat:"
            + this->tempSequenceFilePaths
            .join("|");
    arguments << concatArg;
    arguments << "-c";
    arguments << "copy";
    arguments << outFilePath;
    this->argumentsList << arguments;
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
    qDebug() << "bash output: " << bashOutput;
    if(exitCode == 0){
        int nCommands = this->argumentsList.size();
        if(nCommands > 0){
            QString ffmpegFilePath
                    = this->getFFmpegFilePath();
            QStringList arguments
                    = this->argumentsList.takeFirst();
            qDebug() << ffmpegFilePath + " " + arguments.join(" ");
            this->encodingProcess
                    .start(ffmpegFilePath,
                            arguments);
        }else{
            this->_removeTempFiles();
            this->encodingFinished();
        }
    }else{
        this->encodingFailed();
    }
}
//====================================
void FFmpegEncoder::_onProcessError(
        QProcess::ProcessError error){
    this->encodingFailed();
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
void FFmpegEncoder::_removeTempFiles(){
    QFile(this->tempInVideoFilePath).remove();
    foreach(QString tempSequenceFilePath,
            this->tempSequenceFilePaths){
        QFile(tempSequenceFilePath).remove();
    }
}
//====================================


}
