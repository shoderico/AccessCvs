#include "tabledatasanitizesetting.h"

#include <QRegularExpression>

#include "util/codecinfo.h"

TableDataSanitizeSetting::TableDataSanitizeSetting(QObject *parent) : QObject(parent)
{
    QString sPattern;

    // line
    m_reLine = new QRegularExpression();
    // <dataroot xmlns:od="urn:schemas-microsoft-com:officedata" generated="2015-08-12T09:17:53">
    sPattern = "^\\s*";
    sPattern += "<dataroot xmlns:od=\"urn:schemas-microsoft-com:officedata\" generated=\"([^\"]*)\">";
    sPattern += "\\s*$";
    m_reLine->setPattern( sPattern );

}

TableDataSanitizeSetting::~TableDataSanitizeSetting()
{
    delete m_reLine;
}

void TableDataSanitizeSetting::sanitize(QTextStream &streamSrc, QTextStream &streamDst, CodecInfo *codecDst)
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
            txt = "<dataroot xmlns:od=\"urn:schemas-microsoft-com:officedata\" generated=\"\">";
            goto write_line;
        }

write_line:
        streamDst << txt << codecDst->lineEnd();

end_of_while:
        ;
    }
}

