#ifndef ML_LOG_H
#define ML_LOG_H

#include <QDebug>

namespace ML{

QString getLogFilePath();
void resetLogFile();
void fileMessageHandler(
        QtMsgType type,
        const QMessageLogContext &context,
        const QString &msg);

}

#endif // LOG_H
