#ifndef CODECINFO_H
#define CODECINFO_H

#include <QObject>

class CodecInfo : public QObject
{
    Q_OBJECT
public:
    explicit CodecInfo(QObject *parent = 0) : QObject(parent) {}

    QTextCodec *codec() const { return m_codec; }
    void setCodec(QTextCodec *codec) { m_codec = codec; }

    bool bom() const { return m_bom; }
    void setBom(const bool bom) { m_bom = bom; }

    QString lineEnd() const { return m_lineEnd; }
    void setLineEnd(const QString &lineEnd) { m_lineEnd = lineEnd; }

private:
    QTextCodec *m_codec;
    bool        m_bom;
    QString     m_lineEnd;

};

#endif // CODECINFO_H
