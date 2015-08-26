#ifndef LOGFILE_H
#define LOGFILE_H

#include "addin_global.h"

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QReadWriteLock>

class ADDINSHARED_EXPORT LogFile
{
public:
    LogFile(const QString &path, const QString &baseFileName, const bool append);
    ~LogFile();

    static void MessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    static LogFile *instance();

private:
    QFile m_file;
    QTextStream m_stream;
    QReadWriteLock m_lock;
};

#endif // LOGFILE_H
