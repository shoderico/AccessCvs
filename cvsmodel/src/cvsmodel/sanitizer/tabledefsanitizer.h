#ifndef TABLEDEFSANITIZER_H
#define TABLEDEFSANITIZER_H

#include "cvsmodel/cvsmodel_global.h"

#include <QObject>
#include <QTextStream>

class QRegularExpression;

class CodecInfo;

class CVSMODEL_SHARED_EXPORT TableDefSanitizer : public QObject
{
    Q_OBJECT
public:
    explicit TableDefSanitizer(QObject *parent = 0);
    ~TableDefSanitizer();

    void sanitize(QTextStream &stSrc, QTextStream &stDst, CodecInfo *codecDst);

private:
    QRegularExpression *m_reLine;
    QRegularExpression *m_reLineEnd;
};

#endif // TABLEDEFSANITIZER_H
