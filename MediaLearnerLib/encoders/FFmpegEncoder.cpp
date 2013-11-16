#include "FFmpegEncoder.h"

#include <QDir>
#include <QString>

namespace MediaLearner{

//====================================
FFmpegEncoder::FFmpegEncoder(QObject *parent) :
    EncoderInterface(parent){
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
void FFmpegEncoder::encode(QString outFilePath){
}
//====================================


}
