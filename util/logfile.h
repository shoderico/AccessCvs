#ifndef LOGFILE_H
#define LOGFILE_H

#include <QString>
#include <QFile>
#include <QTextStream>

class LogFile
{
public:
    LogFile(const QString &path, const QString &baseFileName, const bool append);
    ~LogFile();

    static void MessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    static LogFile *instance();

private:
    QFile m_file;
    QTextStream m_stream;
};

#endif // LOGFILE_H
