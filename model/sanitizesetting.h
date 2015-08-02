#ifndef SANITIZESETTING_H
#define SANITIZESETTING_H

#include <QObject>
#include <QRegularExpression>
#include <QTextStream>

class CodecInfo;

class SanitizeSetting : public QObject
{
    Q_OBJECT
public:
    explicit SanitizeSetting(QObject *parent = 0);

    void sanitize(QTextStream &stSrc, QTextStream &stOutDesign, QTextStream &stOutModule, CodecInfo *codecDst);

private:
    QRegularExpression reBlock;
    QRegularExpression reLine;

    QMap<QString, QString> m_blockData;
};

#endif // SANITIZESETTING_H
