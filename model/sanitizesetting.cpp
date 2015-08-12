#include "sanitizesetting.h"

#include <QRegularExpression>

#include "util/codecinfo.h"



SanitizeSetting::SanitizeSetting(QObject *parent) : QObject(parent)
{
    QString sPattern;

    // block
    sPattern = "^\\s*";
    sPattern += "(";
    sPattern += "PrtDev(?:Names|Mode)[W]?";
    sPattern += "|GUID|\"GUID\"|NameMap|dbLongBinary \"DOL\"";
    sPattern += ")";
    sPattern += " = Begin";
    m_reBlock = new QRegularExpression();
    m_reBlock->setPattern(sPattern);

    // line
    sPattern = "^\\s*(?:";
    sPattern += "Checksum =";
    sPattern += "|BaseInfo|NoSaveCTIWhenDisabled =1";
    sPattern += "|dbByte \"PublishToWeb\" =\"1\"";
    sPattern += "|PublishOption =1";
    sPattern += ")";
    m_reLine = new QRegularExpression();
    m_reLine->setPattern(sPattern);

}

SanitizeSetting::~SanitizeSetting()
{
    delete m_reBlock;
    delete m_reLine;
}

namespace
{
    void writeLine( QTextStream &stDesign, QTextStream &stModule, bool isCodeBehind, const QString &txt, const QString &lineEnd)
    {
        if ( !isCodeBehind )
            stDesign << txt << lineEnd;
        else
            stModule << txt << lineEnd;
    }
}

void SanitizeSetting::sanitize(QTextStream &streamSrc, QTextStream &streamDstDesign, QTextStream &streamDstModule, CodecInfo *codecDst)
{

    // TODO: remove trailing spaces

    m_blockData.clear();

    bool getLine = true;
    bool isCodeBehind = false;
    bool isReport = false;
    bool isReportPositionBegin = false;

    QString txt;
    QString sPattern;

    while (!streamSrc.atEnd())
    {
        //qDebug() << stIn.readLine();

        if ( getLine )
            txt = streamSrc.readLine();
        else
            getLine = true;

        QRegularExpressionMatch matchesLine;
        QRegularExpressionMatch matchesBlock;


        // Line
        matchesLine = m_reLine->match(txt);
        if ( matchesLine.hasMatch() )
        {
            QRegularExpression reIndent;
            reIndent.setPattern("^(\\s+)\\S");
            QRegularExpressionMatch matches = reIndent.match(txt);

            if ( !matches.hasMatch() )
                sPattern = "^";
            else
                sPattern = "^" + matches.captured(1);
            sPattern += "\\S";
            reIndent.setPattern(sPattern);

            while ( !streamSrc.atEnd() )
            {
                txt = streamSrc.readLine();
                if (reIndent.match(txt).hasMatch())
                    break;
            }
            getLine = false;
            if (isReport && isReportPositionBegin) { isReport = false; isReportPositionBegin = false; }
            goto end_of_while;
        }

        // Block
        matchesBlock = m_reBlock->match(txt);
        if ( matchesBlock.hasMatch() )
        {
            QString elementName = matchesBlock.captured(1);
            QRegularExpression reBinary;
            reBinary.setPattern("^\\s*0x(\\w+)(?: ,|)$");
            QString binaryStr = "";
            while ( !streamSrc.atEnd() )
            {
                txt = streamSrc.readLine();
                QRegularExpressionMatch matchesBinary = reBinary.match(txt);
                if ( matchesBinary.hasMatch() )
                    binaryStr += matchesBinary.captured(1);
                if ( txt.contains( "End" ))
                    break;

            }
            m_blockData[ elementName ] = binaryStr;
//            if ( elementName == "PrtDevMode" )
//            {
////                //qDebug() << binaryStr;
//                qDebug() << binaryStr.length(); // 3112
//            }

            if (isReport && isReportPositionBegin) { isReport = false; isReportPositionBegin = false; }
            goto end_of_while;
        }

        if ( txt.contains("Begin Report") )
        {
            isReport = true;
            isReportPositionBegin = false;
            writeLine(streamDstDesign, streamDstModule, isCodeBehind, txt, codecDst->lineEnd());
            goto end_of_while;
        }

        if ( isReport && (
                  txt.startsWith( "    Right =" ) ||
                  txt.startsWith( "    Bottom =" ) ||
                  txt.startsWith( "    Top =" ) ||
                  txt.startsWith( "    Left =" )
                  )
              )
        {
            isReportPositionBegin = true;
            goto end_of_while;
        }

        if ( txt.contains( "CodeBehindForm" ) )
        {
            isCodeBehind = true;
            writeLine(streamDstDesign, streamDstModule, isCodeBehind, txt, codecDst->lineEnd());
            if (isReport && isReportPositionBegin) { isReport = false; isReportPositionBegin = false; }
            goto end_of_while;
        }

        if ( !isCodeBehind && txt.isEmpty() )
        {
            goto end_of_while;
        }

        writeLine(streamDstDesign, streamDstModule, isCodeBehind, txt, codecDst->lineEnd());
        if (isReport && isReportPositionBegin) { isReport = false; isReportPositionBegin = false; }

end_of_while:
        ;
    }

}

