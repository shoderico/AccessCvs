#include "tabledefsanitizesetting.h"

#include <QRegularExpression>

#include "util/codecinfo.h"


TableDefSanitizeSetting::TableDefSanitizeSetting(QObject *parent) : QObject(parent)
{
    QString sPattern;

    // line
    m_reLine = new QRegularExpression();
    sPattern = "^\\s*";
    sPattern += "<od:tableProperty name=\"";
    sPattern += "(NameMap|GUID)";
    sPattern += "\".*$";
    m_reLine->setPattern( sPattern );

    // line-end
    m_reLineEnd = new QRegularExpression();
    sPattern = "^.*/>$";
    sPattern += "";
    m_reLineEnd->setPattern( sPattern );

}

TableDefSanitizeSetting::~TableDefSanitizeSetting()
{
    delete m_reLine;
    delete m_reLineEnd;
}

void TableDefSanitizeSetting::sanitize(QTextStream &streamSrc, QTextStream &streamDst, CodecInfo *codecDst)
{
    bool getLine = true;
    QString txt;

    while (!streamSrc.atEnd())
    {
        if ( getLine )
            txt = streamSrc.readLine();
        else
            getLine = true;

        QRegularExpressionMatch matchesLine;

        // Line
        matchesLine = m_reLine->match(txt);
        if ( matchesLine.hasMatch() )
        {
            if (m_reLineEnd->match(txt).hasMatch())
                goto end_of_while;

            while ( !streamSrc.atEnd() )
            {
                txt = streamSrc.readLine();
                if (m_reLineEnd->match(txt).hasMatch())
                    break;
            }
            getLine = true;
            goto end_of_while;
        }

        streamDst << txt << codecDst->lineEnd();

end_of_while:
        ;
    }
}

