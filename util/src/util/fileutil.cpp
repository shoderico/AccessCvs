#include "fileutil.h"

#include "codecinfo.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QTextCodec>

#include <QCryptographicHash>
#include <qt_windows.h>

void FileUtil::saveToFile(const QString &contents, const QString &filePath, CodecInfo *codec)
{
    QFile file( filePath );

    // delete existing file
    deleteFile( filePath );

    // open file
    file.open(QIODevice::WriteOnly);

    // open stream
    QTextStream stream( &file );
    stream.setCodec( codec->codec() );
    stream.setGenerateByteOrderMark( codec->bom() );

    // write contents
    stream << contents;

    // close file
    file.close();
}

void FileUtil::copyContents(const QString &filePathSrc, CodecInfo *codecSrc, const QString &filePathDst, CodecInfo *codecDst, const bool removeTrailingSpaces)
{

    QFile fileSrc( filePathSrc );
    QFile fileDst( filePathDst );

    // delete existing file
    deleteFile( filePathDst );

    // open files
    if ( !fileSrc.open( QIODevice::ReadOnly) )
    {
        return;
    }
    if ( !fileDst.open(QIODevice::WriteOnly) )
    {
        fileSrc.close();
        return;
    }

    // open streams
    QTextStream streamSrc( &fileSrc );
    streamSrc.setCodec(codecSrc->codec());
    // bom resolved automatically

    QTextStream streamDst( &fileDst );
    streamDst.setCodec(codecDst->codec());
    streamDst.setGenerateByteOrderMark(codecDst->bom());

    // copy contents
    if ( codecSrc->codec()->name() == "Shift_JIS" )
    {
        while (!fileSrc.atEnd())
        {
            QByteArray lineData = fileSrc.readLine();
            QString line = QString::fromLocal8Bit( lineData );

            if (line.endsWith( codecSrc->lineEnd() ))
                line.chop( codecSrc->lineEnd().length() );

            if ( removeTrailingSpaces )
            {
                while ( line.size() > 0 && line.at( line.size() - 1 ).isSpace() )
                    line.chop( 1 );
            }

            streamDst << line << codecDst->lineEnd();
        }
    }
    else if ( codecDst->codec()->name() == "Shift_JIS" )
    {
        while (!streamSrc.atEnd())
        {
            QString line = streamSrc.readLine() + codecDst->lineEnd();
            QByteArray lineData = line.toLocal8Bit();
            fileDst.write( lineData );
        }
    }
    else
    {
        while (!streamSrc.atEnd())
            streamDst << streamSrc.readLine() << codecDst->lineEnd();
    }

    // close files
    fileSrc.close();
    fileDst.close();

}

void FileUtil::deleteFile(const QString &filePath)
{
    QFile file( filePath );

    if ( file.exists() )
        file.remove();
}

bool FileUtil::compare(const QString &filePathA, const QString &filePathB, const bool isSameIfBothNonExist)
{
    bool isSame = true; // default is same

    QFile fileA( filePathA );
    QFile fileB( filePathB );

    if (isSame && fileA.exists() != fileB.exists())
    {
        // one exists and other doesn't exists
        isSame = false;
    }

    if (isSame && fileA.exists() && fileB.exists())
    {
        // both exists
        isSame = FileUtil::compare( &fileA, &fileB );
    }
    else if (isSame && !fileA.exists() && !fileB.exists())
    {
        // both NON-exists
        // caller calls this method only when corresponging extension is set.
        // so the case when both NON-exists must be treat as DIFFERENT.
        // BUT for optional files such as table-data, report-prop,
        // both-Non-Exists is as SAME.
        // so it must be specified by argument.
        isSame = isSameIfBothNonExist;
    }

    return isSame;
}

bool FileUtil::compare(QFile *fileA, QFile *fileB)
{
    // Retrieve the SHA1 signature of the file
    QByteArray sigA = fileSignature(fileA->fileName());
    QByteArray sigB = fileSignature(fileB->fileName());

    return (sigA == sigB);
}

QByteArray FileUtil::fileSignature(const QString &filePath)
{
    QCryptographicHash hash( QCryptographicHash::Sha1 );
    QFile file( filePath );

    if ( file.open( QIODevice::ReadOnly ) ) {
        hash.addData( file.readAll() );
    } else {
        // Handle "cannot open file" error
        return QByteArray();
    }

    // Retrieve the SHA1 signature of the file
    return hash.result();
}

QStringList FileUtil::loadAsStringList(const QString &fileName, CodecInfo *codec)
{
    QStringList lines;

    QFile file( fileName );
    if ( file.exists() )
    {
        file.open(QIODevice::ReadOnly);
        QTextStream stream( &file );
        stream.setCodec( codec->codec() );

        while (!stream.atEnd())
            lines << stream.readLine();

        file.close();
    }

    return lines;
}

QString FileUtil::loadAsString(const QString &fileName, CodecInfo *codec)
{
    QStringList lines = loadAsStringList(fileName, codec);
    return lines.join( codec->lineEnd() );
}

QDateTime FileUtil::fileTime(const QString &fileName)
{
    QFileInfo fi( fileName );
    if (!fi.exists())
        return QDateTime();

    if ( fi.lastModified().isValid() && fi.created().isValid() )
        return fi.lastModified() > fi.created() ? fi.lastModified() : fi.created();

    if ( fi.lastModified().isValid() )
        return fi.lastModified();

    if ( fi.created().isValid() )
        return fi.created();

    return QDateTime();
}


namespace {

// http://stackoverflow.com/questions/19704817/qdatetime-to-filetime
// Convert a QDateTime to a FILETIME.
FILETIME toWinFileTime(const QDateTime &dateTime)
{
    // Definition of FILETIME from MSDN:
    // Contains a 64-bit value representing the number of 100-nanosecond intervals since January 1, 1601 (UTC).
    QDateTime origin(QDate(1601, 1, 1), QTime(0, 0, 0, 0), Qt::UTC);
    // Get offset - note we need 100-nanosecond intervals, hence we multiply by
    // 10000.
    qint64 _100nanosecs = 10000 * origin.msecsTo(dateTime);
    // Pack _100nanosecs into the structure.
    FILETIME fileTime;
    fileTime.dwLowDateTime = _100nanosecs;
    fileTime.dwHighDateTime = (_100nanosecs >> 32);
    return fileTime;
}

// https://www.liveboxcloud.com/qt-and-windows-api/
void TimetToFileTime( time_t t, LPFILETIME pft ) {
    LONGLONG ll = Int32x32To64(t, 10000000) + 116444736000000000;
    pft->dwLowDateTime = (DWORD) ll;
    pft->dwHighDateTime = ll >> 32;
}

}

bool FileUtil::setFileTime(const QString &fileName, const QDateTime &createDate, const QDateTime &updateDate)
{

    if ( !QFile(fileName).exists() )
    {
        qDebug() << "file does not exist" << fileName;
        return false;
    }

    qint64 lastFSModifiedDate = updateDate.toTime_t();
    qint64 FSCreationDate = createDate.toTime_t();

    // https://www.liveboxcloud.com/qt-and-windows-api/

    // get FILETIME structure from lastFSModifiedDate
    FILETIME lmFileTime;
    TimetToFileTime((time_t)lastFSModifiedDate, &lmFileTime);

    // get FILETIME structure from FSCreationDate
    FILETIME fcFileTime;
    TimetToFileTime((time_t)FSCreationDate, &fcFileTime);

    // handle to file
    HANDLE filename = CreateFile(fileName.toStdWString().c_str(), GENERIC_READ|GENERIC_WRITE,
                      FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);

    if (filename == INVALID_HANDLE_VALUE)
    {
        int err = ::GetLastError();
        qDebug() << "invalid file handle " << filename << "; Error number: " << err;
    }

    // set last modification time
    bool setFileTimeReturnVal = SetFileTime(filename, &fcFileTime, (LPFILETIME) NULL, &lmFileTime);

    if(!setFileTimeReturnVal) {
        int err = ::GetLastError();
        qDebug() << "Error while setting the date attributes to " << fileName << "; Error number: " << err;
    }

    CloseHandle(filename);

    return setFileTimeReturnVal;
}

