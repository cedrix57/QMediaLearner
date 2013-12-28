#ifndef ML_LOG_H
#define ML_LOG_H

#include <QDebug>

#include "medialearnerlib_global.h"

namespace ML{

QString MEDIALEARNERLIBSHARED_EXPORT
getLogFilePath();

void MEDIALEARNERLIBSHARED_EXPORT
resetLogFile();

void MEDIALEARNERLIBSHARED_EXPORT
fileMessageHandler(
        QtMsgType type,
        const QMessageLogContext &context,
        const QString &msg);

}

#endif // LOG_H
