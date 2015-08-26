#ifndef SANITIZESETTING_H
#define SANITIZESETTING_H

#include "cvsmodel_global.h"

#include <QObject>
#include <QTextStream>

class QRegularExpression;
class QTextDecoder;
class QTextEncoder;

class CodecInfo;

class CVSMODELSHARED_EXPORT SanitizeSetting : public QObject
{
    Q_OBJECT
public:
    explicit SanitizeSetting(QObject *parent = 0);
    ~SanitizeSetting();

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

#endif // SANITIZESETTING_H
