#include "log.h"
#include <QDateTime>
#include <QFile>
#include <QDir>
#include <QTextStream>

namespace ML{


QString logDir = "log";
//================================================
void resetLogFile(){
    QDir dir(logDir);
    if(!dir.exists()){
        dir.mkpath(".");
    }
    QString logFilePath = getLogFilePath();
    QFile outFile(logFilePath);
    outFile.open(QIODevice::WriteOnly);
    QTextStream ts(&outFile);
    ts << "VERSION BETA 01" << endl;
    outFile.close();
}
QString logFilePath = "";
//================================================
QString getLogFilePath(){
    if(logFilePath.isEmpty()){
        QDir relDir(logDir);
        QDateTime currentDateTime
                = QDateTime::currentDateTime();
        QString logFileName
                = currentDateTime
                .toString("yyyy_MM_dd__hh_mm_ss");
        logFileName += ".log";
        logFilePath = relDir.filePath(logFileName);
    }
    return logFilePath;
}
//================================================
void fileMessageHandler(QtMsgType type,
                     const QMessageLogContext &context,
                     const QString &msg){
    Q_UNUSED(context)
    QString txt;
    QTime time = QTime::currentTime();
    QString timeString = time.toString();
    switch (type) {
    case QtDebugMsg:
        txt += "Debug: " + timeString + ": " + msg;
        break;
    case QtWarningMsg:
        txt += "Wàrning: " + timeString + ": " + msg;
    break;
    case QtCriticalMsg:
        txt += "Critical: " + timeString + ": " + msg;
    break;
    case QtFatalMsg:
        txt += "Debug: " + timeString + ": " + msg;
        abort();
    }
    QString logFilePath = getLogFilePath();
    QFile outFile(logFilePath);
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
    outFile.close();
}

}
