#ifndef SANITIZESETTING_H
#define SANITIZESETTING_H

#include <QObject>
#include <QTextStream>

class QRegularExpression;
class QTextDecoder;
class QTextEncoder;

class CodecInfo;

class SanitizeSetting : public QObject
{
    Q_OBJECT
public:
    explicit SanitizeSetting(QObject *parent = 0);
    ~SanitizeSetting();

    void sanitize(QTextStream &streamSrc, CodecInfo *codecSrc, QTextStream &streamDstDesign, QTextStream &streamDstModule, CodecInfo *codecDst);
    QByteArray blockData( const QString &elementName);

private:
    QString readLine();
    void writeLine( QTextStream &stDesign, QTextStream &stModule, bool isCodeBehind, const QString &txt, const QString &lineEnd);

private:
    QRegularExpression *m_reBlock;
    QRegularExpression *m_reSingleLine;
    QRegularExpression *m_reMultiLine;

    QIODevice    *m_deviceSrc;
    CodecInfo    *m_codecInfoSrc;

    QMap<QString, QString> m_blockData;
};

#endif // SANITIZESETTING_H
