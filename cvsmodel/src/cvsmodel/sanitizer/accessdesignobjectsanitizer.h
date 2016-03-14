#ifndef ACCESSDESIGNOBJECTSANITIZER_H
#define ACCESSDESIGNOBJECTSANITIZER_H

#include "../cvsmodel_global.h"

#include <QObject>
#include <QTextStream>

class QRegularExpression;
class QTextDecoder;
class QTextEncoder;

class CodecInfo;

class CVSMODEL_SHARED_EXPORT AccessDesignObjectSanitizer : public QObject
{
    Q_OBJECT
public:
    explicit AccessDesignObjectSanitizer(QObject *parent = 0);
    ~AccessDesignObjectSanitizer();

    void sanitize(QTextStream &streamSrc, CodecInfo *codecSrc, QTextStream &streamDstDesign, QTextStream &streamDstModule, CodecInfo *codecDst);
    QByteArray blockData( const QString &elementName);

private:
    QString readLine();
    void writeLine( QTextStream &stDesign, QTextStream &stModule, bool isCodeBehind, QString &txt, const QString &lineEnd);
    bool atEnd();

private:
    QRegularExpression *m_reBlock;
    QRegularExpression *m_reSingleLine;
    QRegularExpression *m_reMultiLine;
    QRegularExpression *m_rePictureData;
    QRegularExpression *m_rePictureDataFirstLine;


    QIODevice    *m_deviceSrc;
    CodecInfo    *m_codecInfoSrc;
    QTextStream  *m_streamSrc;

    QMap<QString, QString> m_blockData;
};

#endif // ACCESSDESIGNOBJECTSANITIZER_H
