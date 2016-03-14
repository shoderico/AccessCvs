#ifndef TABLEDATASANITIZER_H
#define TABLEDATASANITIZER_H

#include "cvsmodel/cvsmodel_global.h"

#include <QObject>
#include <QTextStream>

class QRegularExpression;

class CodecInfo;

class CVSMODEL_SHARED_EXPORT TableDataSanitizer : public QObject
{
    Q_OBJECT
public:
    explicit TableDataSanitizer(QObject *parent = 0);
    ~TableDataSanitizer();

    void sanitize(QTextStream &streamSrc, QTextStream &streamDst, CodecInfo *codecDst);

private:
    QRegularExpression *m_reLine;
};

#endif // TABLEDATASANITIZER_H
