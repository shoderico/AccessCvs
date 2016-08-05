#ifndef LOGFILE_H
#define LOGFILE_H

#include "util_global.h"

#include <QObject>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QReadWriteLock>

class UTIL_SHARED_EXPORT LogFile : public QObject
{
    Q_OBJECT
public:
    LogFile(const QString &path, const QString &baseFileName, const bool append, QObject *parent = 0);
    virtual ~LogFile();

    static void MessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    static LogFile *instance();

private:
    QFile m_file;
    QTextStream m_stream;
    QReadWriteLock m_lock;
};

#endif // LOGFILE_H
