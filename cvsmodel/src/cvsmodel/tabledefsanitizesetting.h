#ifndef TABLEDEFSANITIZESETTING_H
#define TABLEDEFSANITIZESETTING_H

#include "cvsmodel_global.h"

#include <QObject>
#include <QTextStream>

class QRegularExpression;

class CodecInfo;

class CVSMODEL_SHARED_EXPORT TableDefSanitizeSetting : public QObject
{
    Q_OBJECT
public:
    explicit TableDefSanitizeSetting(QObject *parent = 0);
    ~TableDefSanitizeSetting();

    void sanitize(QTextStream &stSrc, QTextStream &stDst, CodecInfo *codecDst);

private:
    QRegularExpression *m_reLine;
    QRegularExpression *m_reLineEnd;
};

#endif // TABLEDEFSANITIZESETTING_H
