#include "sanitizesetting.h"

#include <QTextStream>

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
    reBlock.setPattern(sPattern);

    // line
    sPattern = "^\\s*(?:";
    sPattern += "Checksum =";
    sPattern += "|BaseInfo|NoSaveCTIWhenDisabled =1";
    sPattern += "|dbByte \"PublishToWeb\" =\"1\"";
    sPattern += "|PublishOption =1";
    sPattern += ")";
    reLine.setPattern(sPattern);

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
        matchesLine = reLine.match(txt);
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
            goto end_of_while;
        }

        // Block
        matchesBlock = reBlock.match(txt);
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

            goto end_of_while;
        }

        if ( txt.contains("Begin Report") )
        {
            isReport = true;
            writeLine(streamDstDesign, streamDstModule, isCodeBehind, txt, codecDst->lineEnd());
            goto end_of_while;
        }

        if ( isReport && (
                  txt.contains( "    Right =" ) ||
                  txt.contains( "    Bottom =" ) ||
                  txt.contains( "    Top =" ) ||
                  txt.contains( "    Left =" )
                  )
              )
        {
            if ( txt.contains( "    Bottom =" ))
                isReport = false;
            goto end_of_while;
        }

        if ( txt.contains( "CodeBehindForm" ) )
        {
            isCodeBehind = true;
            writeLine(streamDstDesign, streamDstModule, isCodeBehind, txt, codecDst->lineEnd());
            goto end_of_while;
        }

        if ( !isCodeBehind && txt.isEmpty() )
        {
            goto end_of_while;
        }

        writeLine(streamDstDesign, streamDstModule, isCodeBehind, txt, codecDst->lineEnd());

end_of_while:
        ;
    }

}

