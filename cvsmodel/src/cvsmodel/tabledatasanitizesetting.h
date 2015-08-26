#ifndef TABLEDATASANITIZESETTING_H
#define TABLEDATASANITIZESETTING_H

#include <QObject>
#include <QTextStream>

class QRegularExpression;

class CodecInfo;

class TableDataSanitizeSetting : public QObject
{
    Q_OBJECT
public:
    explicit TableDataSanitizeSetting(QObject *parent = 0);
    ~TableDataSanitizeSetting();

    void sanitize(QTextStream &streamSrc, QTextStream &streamDst, CodecInfo *codecDst);

private:
    QRegularExpression *m_reLine;
};

#endif // TABLEDATASANITIZESETTING_H
