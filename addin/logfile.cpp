#include "logfile.h"

#include <QDir>
#include <QTextCodec>
#include <QDateTime>

static LogFile *static_instance = NULL;

LogFile::LogFile(const QString &path, const QString &baseFileName, const bool append)
{
    static_instance = this;

    QDir dir(path);
    dir.mkpath(".");

    QString filePath = path + "\\" + baseFileName;
    if (!append)
        filePath += QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss");
    filePath += ".txt";

    m_file.setFileName(filePath);

    if (!append)
    {
        if (m_file.exists())
            m_file.remove();
        m_file.open(QIODevice::WriteOnly);
    }
    else
        m_file.open(QIODevice::Append);

    m_stream.setDevice(&m_file);
    m_stream.setCodec( QTextCodec::codecForName("UTF-8") );
    m_stream.setGenerateByteOrderMark(false);

}

LogFile::~LogFile()
{
    m_file.close();
    static_instance = NULL;
}

void LogFile::MessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // https://gist.github.com/ajithbh/9383850

    QWriteLocker locker(&static_instance->m_lock);

    QString datetime = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    QString contextMsg = QString(" : %1:%2 %3").arg(context.file).arg(context.line).arg(context.function);
    QString typeMsg = "";
    switch (type)
    {
        case QtInfoMsg:     typeMsg = " [Info]     ";   break;
        case QtDebugMsg:    typeMsg = " [Debug]    ";   break;
        case QtCriticalMsg: typeMsg = " [Critical] ";   break;
        case QtWarningMsg:  typeMsg = " [Warning]  ";   break;
        case QtFatalMsg:    typeMsg = " [Fatal]    ";   break;
    }
    static_instance->m_stream << datetime << typeMsg << msg << contextMsg << "\n";

    if (type == QtFatalMsg)
        abort();
}

LogFile *LogFile::instance()
{
    return static_instance;
}

