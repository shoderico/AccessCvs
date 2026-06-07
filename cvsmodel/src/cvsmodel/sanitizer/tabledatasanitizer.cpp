#include "tabledatasanitizer.h"

#include <QRegularExpression>

#include "util/codecinfo.h"

#include "pch.hpp"

TableDataSanitizer::TableDataSanitizer(QObject *parent) : QObject(parent)
{
    QString sPattern;

    // line
    m_reLine = new QRegularExpression();
    // <dataroot xmlns:od="urn:schemas-microsoft-com:officedata" generated="2015-08-12T09:17:53">
    // <dataroot xmlns:od="urn:schemas-microsoft-com:officedata" generated="2015-12-21T22:23:50"/>
    sPattern = "^\\s*";
    sPattern += "<dataroot xmlns:od=\"urn:schemas-microsoft-com:officedata\" generated=\"(?:[^\"]*)\"(/*)>";
    sPattern += "\\s*$";
    m_reLine->setPattern( sPattern );

}

TableDataSanitizer::~TableDataSanitizer()
{
    delete m_reLine;
}

void TableDataSanitizer::sanitize(QTextStream &streamSrc, QTextStream &streamDst, CodecInfo *codecDst)
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
            QString endSlash = matchesLine.captured(1);
            txt = "<dataroot xmlns:od=\"urn:schemas-microsoft-com:officedata\" generated=\"\"" + endSlash + ">";
            goto write_line;
        }

write_line:
        streamDst << txt << codecDst->lineEnd();

end_of_while:
        ;
    }
}

