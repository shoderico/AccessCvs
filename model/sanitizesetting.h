#ifndef SANITIZESETTING_H
#define SANITIZESETTING_H

#include <QObject>
#include <QTextStream>

class QRegularExpression;

class CodecInfo;

class SanitizeSetting : public QObject
{
    Q_OBJECT
public:
    explicit SanitizeSetting(QObject *parent = 0);
    ~SanitizeSetting();

    void sanitize(QTextStream &streamSrc, QTextStream &streamDstDesign, QTextStream &streamDstModule, CodecInfo *codecDst);
    QByteArray blockData( const QString &elementName);

private:
    QRegularExpression *m_reBlock;
    QRegularExpression *m_reSingleLine;
    QRegularExpression *m_reMultiLine;

    QMap<QString, QString> m_blockData;
};

#endif // SANITIZESETTING_H
