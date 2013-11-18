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
int FFmpegEncoder::getFrameRate(){
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
            = process.readAllStandardOutput();
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
    int frameRate = this->getFrameRate();
    QStringList arguments;
    arguments << "-i";
    arguments << this->inVideoFilePath;
    QStringList filterParams;
    foreach(QList<DrawingSubtitleInfo> infos,
            this->texts){
        foreach(DrawingSubtitleInfo info,
                infos){
            DrawingSettings drawingSettings
                    = info.text.getDrawingSettings();
            QString drawTextParam = "drawtext=";
            drawTextParam += "text=";
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
            QString fontPath = fontFilePaths[fontFamily];
            drawTextParam += fontPath;
            drawTextParam += ":";
            drawTextParam += "fontcolor=";
            drawTextParam += drawingSettings.font.toString();
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
            //TODO font size
            int begin = this->getNFrame(
                        info.startPosition,
                        frameRate);
            int end = this->getNFrame(
                        info.endPosition,
                        frameRate);
            drawTextParam += ":";
            drawTextParam += "draw=";
            drawTextParam += "'gt(n,"
                    + QString::number(begin)
                    + ")*lt(n,"
                    + QString::number(end)
                    + ")'";
            filterParams << drawTextParam;
            arguments << "-ss";
            arguments << this->getFormatedTime(
                                    info.startPosition);
            arguments << "-to";
            arguments << this->getFormatedTime(
                                    info.endPosition);
        }
    }
    //add texts
    arguments << "-vf";
    QString filterParamsConcatened = filterParams.join(",");
    arguments << filterParamsConcatened;
    arguments << outFilePath;
    QString ffmpegFilePath
            = this->getFFmpegFilePath();
    this->encodingProcess.start(
                ffmpegFilePath,
                arguments);
}
//====================================
void FFmpegEncoder::_onProcessFinished(
        int exitCode,
        QProcess::ExitStatus exitStatus){
    if(exitCode == 0){
        this->encodingFinished();
    }else{
        this->encodingFailed();
    }
}
//====================================


}
