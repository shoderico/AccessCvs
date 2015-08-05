#ifndef FILEUTIL_H
#define FILEUTIL_H

#include <QString>
#include <QDateTime>

class CodecInfo;

class QFile;

class FileUtil
{
public:
    FileUtil(){}

    static void saveToFile(const QString &contents, const QString &filePath, CodecInfo *codec);
    static void copyContents(const QString &filePathSrc, CodecInfo *codecSrc, const QString &filePathDst, CodecInfo *codecDst);
    static void deleteFile(const QString &filePath);

    static bool compare(const QString &filePathA, const QString &filePathB);
    static bool compare(QFile *fileA, QFile *fileB);

    static QByteArray fileSignature(const QString &filePath);

    static QStringList loadAsStringList(const QString &fileName, CodecInfo *codec);
    static QString     loadAsString    (const QString &fileName, CodecInfo *codec);

    static QDateTime fileTime(const QString &fileName);
    static bool setFileTime(const QString &fileName, const QDateTime &createDate, const QDateTime &updateDate);

};

#endif // FILEUTIL_H
