#include "fileutil.h"

#include "codecinfo.h"

#include <QFile>
#include <QTextStream>

#include <QCryptographicHash>

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

void FileUtil::copyContents(const QString &filePathSrc, CodecInfo *codecSrc, const QString &filePathDst, CodecInfo *codecDst)
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
    while (!streamSrc.atEnd())
        streamDst << streamSrc.readLine() << codecDst->lineEnd();

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

bool FileUtil::compare(const QString &filePathA, const QString &filePathB)
{
    bool isSame = true; // default is same

    QFile fileA( filePathA );
    QFile fileB( filePathB );

    if (isSame && fileA.exists() != fileB.exists())
        isSame = false;

    if (isSame && fileA.exists())
    {
        // both exists
        isSame = FileUtil::compare( &fileA, &fileB );
    }
    else if (isSame && !fileA.exists())
    {
        // both NON-exists
        // TODO: what to do ?
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

