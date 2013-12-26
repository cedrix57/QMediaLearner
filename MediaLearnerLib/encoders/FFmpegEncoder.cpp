#include "FFmpegEncoder.h"

#include <QDir>
#include <QString>
#include <QMap>

namespace ML{

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
QMap<QString, ProfileInfo> FFmpegEncoder::getAvailableAudioProfiles(){
    qDebug() << "QMap<QString, ProfileInfo> FFmpegEncoder::getAvailableAudioProfiles() called";
    QMap<QString, ProfileInfo> profileInfos;
    ProfileInfo profileInfo;
    profileInfo.ext = "mp2";
    profileInfo.name = "MP2";
    profileInfo.description = "-vn";
    profileInfos[profileInfo.name] = profileInfo;
    profileInfo.ext = "mp3";
    profileInfo.name = "MP3";
    profileInfo.description = "-vn";
    profileInfos[profileInfo.name] = profileInfo;
    profileInfo.ext = "ogg";
    profileInfo.name = "Vorbis (OGG)";
    profileInfo.description = "-acodec libvorbis -vn";
    profileInfos[profileInfo.name] = profileInfo;
    qDebug() << "QMap<QString, ProfileInfo> FFmpegEncoder::getAvailableAudioProfiles() end";
    return profileInfos;
}

//====================================
QMap<QString, ProfileInfo> FFmpegEncoder::getAvailableVideoProfiles(){
    qDebug() << "QMap<QString, ProfileInfo> FFmpegEncoder::getAvailableVideoProfiles() called";
    QMap<QString, ProfileInfo> profileInfos;
    ProfileInfo profileInfo;
    profileInfo.ext = "mp4";
    profileInfo.name = "H.264 + acc (MP4)";
    profileInfo.description = "";
    profileInfos[profileInfo.name] = profileInfo;
    profileInfo.ext = "avi";
    profileInfo.name = "xvid + mp3 (AVI)";
    profileInfo.description = "-acodec mp3 -c:v mpeg4 -vtag xvid";
    profileInfos[profileInfo.name] = profileInfo;
    profileInfo.ext = "avi";
    profileInfo.name = "mjpeg + ac3 (AVI)";
    profileInfo.description = "-acodec ac3 -vcodec mjpeg";
    profileInfos[profileInfo.name] = profileInfo;
    profileInfo.ext = "webm";
    profileInfo.name = "VP8 + Vorbis (Webm)";
    profileInfo.description = "";
    profileInfos[profileInfo.name] = profileInfo;
    profileInfo.ext = "ogg";
    profileInfo.name = "Theora + Vorbis (ogg)";
    profileInfo.description = "-acodec libvorbis";
    profileInfos[profileInfo.name] = profileInfo;
    profileInfo.ext = "mpg";
    profileInfo.name = "mpeg-1 + mp2 (MPG)";
    profileInfo.description = "";
    profileInfo.ext = "asf";
    profileInfo.name = "wmv2 + wma2 (ASF)";
    profileInfo.description = "-vcodec wmv2 -acodec wmav2";
    profileInfos[profileInfo.name] = profileInfo; //msmpeg4v3
    profileInfo.ext = "mov";
    profileInfo.name = "H.264 + acc (MOV)";
    profileInfo.description = "";
    profileInfos[profileInfo.name] = profileInfo;
    qDebug() << "QMap<QString, ProfileInfo> FFmpegEncoder::getAvailableVideoProfiles() end";
    return profileInfos;
}
//====================================
QList<EncodingInfo> FFmpegEncoder::getAvailableFormats(){
    qDebug() << "QList<EncodingInfo> FFmpegEncoder::getAvailableFormats() called";
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
    qDebug() << "QList<EncodingInfo> FFmpegEncoder::getAvailableFormats() end";
    return infos;
}
//====================================
QString FFmpegEncoder::getFFmpegFilePath(){
    qDebug() << "QString FFmpegEncoder::getFFmpegFilePath() called";
    QDir currentPath = QDir::currentPath();
    QString ffmpegFilePath
            = currentPath.filePath("ffmpeg");
    qDebug() << "QString FFmpegEncoder::getFFmpegFilePath() end";
    return ffmpegFilePath;
}
//====================================
QList<EncodingInfo> FFmpegEncoder::getAvailableVideoCodecs(){
    qDebug() << "QList<EncodingInfo> FFmpegEncoder::getAvailableVideoCodecs() called";
    QList<EncodingInfo> infos
            = this->_getAvailableCodecs(
                'V');
    qDebug() << "QList<EncodingInfo> FFmpegEncoder::getAvailableVideoCodecs() end";
    return infos;
}
//====================================
QList<EncodingInfo> FFmpegEncoder::_getAvailableCodecs(
        QChar type){
    qDebug() << "QList<EncodingInfo> FFmpegEncoder::_getAvailableCodecs(...) called";
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
    qDebug() << "QList<EncodingInfo> FFmpegEncoder::_getAvailableCodecs(...) end";
    return infos;
}
//====================================
QList<EncodingInfo> FFmpegEncoder::getAvailableAudioCodecs(){
    qDebug() << "QList<EncodingInfo> FFmpegEncoder::getAvailableAudioCodecs() called";
    QList<EncodingInfo> infos
            = this->_getAvailableCodecs(
                'A');
    qDebug() << "QList<EncodingInfo> FFmpegEncoder::getAvailableAudioCodecs() end";
    return infos;
}
//====================================
QList<EncodingInfo> FFmpegEncoder::getAvailableSubtitlesCodecs(){
    qDebug() << "QList<EncodingInfo> FFmpegEncoder::getAvailableSubtitlesCodecs() called";
    QList<EncodingInfo> infos
            = this->_getAvailableCodecs(
                'S');
    qDebug() << "QList<EncodingInfo> FFmpegEncoder::getAvailableSubtitlesCodecs() end";
    return infos;
}
//====================================
int FFmpegEncoder::getFps(){
    qDebug() << "int FFmpegEncoder::getFps() called";
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
    qDebug() << "int FFmpegEncoder::getFps() end";
    return fps;
}
//====================================
int FFmpegEncoder::getNFrame(qint64 ms, int frameRate){
    qDebug() << "int FFmpegEncoder::getNFrame(qint64 ms, int frameRate) called";
    float frameRateMs = frameRate / 1000.0;
    int n = ms * frameRateMs + 0.5;
    qDebug() << "int FFmpegEncoder::getNFrame(qint64 ms, int frameRate) end";
    return n;
}
//====================================
QString FFmpegEncoder::getFormatedTime(qint64 ms){
    qDebug() << "QString FFmpegEncoder::getFormatedTime(qint64 ms) called";
    QTime time(0, 0, 0, 0);
    time = time.addMSecs(ms);
    QString formatedTime
            = time.toString("hh:mm:ss.z");
    qDebug() << "QString FFmpegEncoder::getFormatedTime(qint64 ms) end";
    return formatedTime;
}
//====================================
void FFmpegEncoder::startEncoding(QString outFilePath){
    qDebug() << "void FFmpegEncoder::startEncoding(QString outFilePath) called";
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
    qDebug() << "void FFmpegEncoder::startEncoding(QString outFilePath) end";
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
    qDebug() << "QString FFmpegEncoder::_getTempFilePath(...) called";
    QString filePath
            = QDir(QDir::tempPath())
            .filePath(fileName);
    qDebug() << "QString FFmpegEncoder::_getTempFilePath(...) end";
    return filePath;
}
//====================================
void FFmpegEncoder::_encodeTempVideoCommand(){
    qDebug() << "void FFmpegEncoder::_encodeTempVideoCommand() called";
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
    qDebug() << "ffmpeg " << arguments.join(" ");
    this->argumentsList << arguments;
    qDebug() << "void FFmpegEncoder::_encodeTempVideoCommand() end";
}
//====================================
void FFmpegEncoder::_encodeSequenceCommand(){
    qDebug() << "void FFmpegEncoder::_encodeSequenceCommand() called";
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
                //int x = lineIt->position.x();
                //drawTextParam += QString::number(x);
                drawTextParam += "(main_w-text_w)/2";
                drawTextParam += ":";
                drawTextParam += "y=";
                int y = lineIt->position.y();
                drawTextParam += QString::number(y);
                drawTextParam += ":";
                drawTextParam += "fontsize=";
                int fontSize = subSeqIt->getPixelSize();
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
        qDebug() << "ffmpeg " << arguments.join(" ");
        this->argumentsList << arguments;
        idSeq++;
    }
    qDebug() << "void FFmpegEncoder::_encodeSequenceCommand() end";
}
//====================================
void FFmpegEncoder::_encodeCuttedSequencesCommand(
        QString outFilePath){
    qDebug() << "void FFmpegEncoder::_encodeCuttedSequencesCommand(...) called";
    QStringList arguments;
    arguments << "-i";
    QString concatArg
            = "concat:"
            + this->tempSequenceFilePaths
            .join("|");
    arguments << concatArg;
    if(this->playbackRate != -1){
        arguments << "-filter:v";
        arguments << "setpts="
                     + QString::number(this->playbackRate)
                     + "*PTS";
    }
    bool audioOnly = false;
    if(!this->profileName.isEmpty()){
        QMap<QString, ProfileInfo> formatProfiles
                = this->getAvailableVideoProfiles();
        ProfileInfo encodingInfo
                = formatProfiles[this->profileName];
        QStringList encodingArguments
                = encodingInfo.description.split(" ");
        arguments << encodingArguments;
        if(encodingInfo.description.contains("-vn")){
            audioOnly = true;
        }
    }
    if(!this->newSize.isNull() && !audioOnly){
        arguments << "-vf";
        arguments << "scale="
                     + QString::number(this->newSize.width())
                     + ":"
                     + QString::number(this->newSize.height());
    }
    arguments << "-c";
    arguments << "copy";
    arguments << outFilePath;
    this->argumentsList << arguments;
    qDebug() << "ffmpeg " << arguments.join(" ");
    qDebug() << "void FFmpegEncoder::_encodeCuttedSequencesCommand(...) end";
}
//====================================
void FFmpegEncoder::_onProcessFinished(
        int exitCode,
        QProcess::ExitStatus exitStatus){
    qDebug() << "void FFmpegEncoder::_onProcessFinished(...) called";
    Q_UNUSED(exitStatus)
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
    qDebug() << "void FFmpegEncoder::_onProcessFinished(...) end";
}
//====================================
void FFmpegEncoder::_onProcessError(
        QProcess::ProcessError error){
    qDebug() << "void FFmpegEncoder::_onProcessError((...) called";
    qDebug() << "error: " << error;
    this->encodingFailed();
    qDebug() << "void FFmpegEncoder::_onProcessError((...) end";
}
//====================================
QSize FFmpegEncoder::getOriginalSize(){
    qDebug() << "QSize FFmpegEncoder::getOriginalSize() called";
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
    QSize originalSize = QSize(width, height);
    qDebug() << "QSize FFmpegEncoder::getOriginalSize() end";
    return originalSize;
}
//====================================
void FFmpegEncoder::_removeTempFiles(){
    qDebug() << "void FFmpegEncoder::_removeTempFiles() called";
    QFile(this->tempInVideoFilePath).remove();
    foreach(QString tempSequenceFilePath,
            this->tempSequenceFilePaths){
        qDebug() << "removing " << tempSequenceFilePath;
        QFile(tempSequenceFilePath).remove();
    }
    qDebug() << "void FFmpegEncoder::_removeTempFiles() end";
}
//====================================


}
