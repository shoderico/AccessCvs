#include "sanitizesetting.h"

#include <QDebug>
#include <QRegularExpression>

#include "util/codecinfo.h"
#include <QTextCodec>
#include <QTextDecoder>
#include <QTextEncoder>
#include <QByteArray>


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

    // single-line
    // FIXME: remove ItemSuffix
    // FIXME: remove TabIndex from Report
    sPattern = "^\\s*(?:";
    sPattern += "Checksum =";
    sPattern += "|NoSaveCTIWhenDisabled =1";
    sPattern += "|dbByte \"PublishToWeb\" =\"1\"";
    sPattern += "|PublishOption =1";
    sPattern += "|OverlapFlags =";
    sPattern += "|LayoutCachedLeft =";
    sPattern += "|LayoutCachedTop =";
    sPattern += "|LayoutCachedWidth =";
    sPattern += "|LayoutCachedHeight =";
    sPattern += ")";
    m_reSingleLine = new QRegularExpression();
    m_reSingleLine->setPattern(sPattern);

    // multi-line
    sPattern = "^\\s*(?:";
    sPattern += "BaseInfo";
    sPattern += ")";
    m_reMultiLine = new QRegularExpression();
    m_reMultiLine->setPattern(sPattern);

    // picture-data
    sPattern = "^\\s*(?:";
    sPattern += "PictureData";
    sPattern += ")";
    m_rePictureData = new QRegularExpression();
    m_rePictureData->setPattern(sPattern);
    //
    sPattern = "^(\\s*)0x";       //1
    sPattern += "([0-9a-z]{16})"; //2
    sPattern += "([0-9a-z]{8})";  //3
    sPattern += "([0-9a-z]{8})";  //4
    sPattern += "([0-9a-z]{8})";  //5
    sPattern += "([0-9a-z]{8})";  //6
    sPattern += "([0-9a-z]{16})"; //7
    sPattern += " ,$";
    m_rePictureDataFirstLine = new QRegularExpression();
    m_rePictureDataFirstLine->setPattern(sPattern);



    // text decoder
    m_codecInfoSrc = NULL;
    m_deviceSrc = NULL;
}

SanitizeSetting::~SanitizeSetting()
{
    delete m_reBlock;
    delete m_reSingleLine;
    delete m_reMultiLine;
}

void SanitizeSetting::sanitize(QTextStream &streamSrc, CodecInfo *codecSrc, QTextStream &streamDstDesign, QTextStream &streamDstModule, CodecInfo *codecDst)
{

    // TODO: remove trailing spaces

    m_blockData.clear();

    m_deviceSrc = streamSrc.device();
    m_codecInfoSrc = codecSrc;
    m_streamSrc = &streamSrc;

    bool getLine = true;
    bool isCodeBehind = false;
    bool isReport = false;
    bool isReportPositionBegin = false;

    QString txt;
    QString sPattern;

    while ( !atEnd() )
    {
        //qDebug() << stIn.readLine();

        if ( getLine )
            txt = readLine();
        else
            getLine = true;

        QRegularExpressionMatch matchesSingleLine;
        QRegularExpressionMatch matchesMultiLine;
        QRegularExpressionMatch matchesBlock;
        QRegularExpressionMatch matchesPictureData;
        QRegularExpressionMatch matchesPictureDataFirstLine;


        // SingleLine
        matchesSingleLine = m_reSingleLine->match(txt);
        if ( matchesSingleLine.hasMatch() )
        {
            if (isReport && isReportPositionBegin) { isReport = false; isReportPositionBegin = false; }
            goto end_of_while;
        }

        // MultiLine
        matchesMultiLine = m_reMultiLine->match(txt);
        if ( matchesMultiLine.hasMatch() )
        {
            QRegularExpression reIndentSame;
            QRegularExpression reIndentLess;
            reIndentSame.setPattern("^(\\s+)\\S");
            QRegularExpressionMatch matches = reIndentSame.match(txt);

            if ( !matches.hasMatch() )
            {
                sPattern = "^\\S";
                reIndentSame.setPattern(sPattern);
                reIndentLess.setPattern(sPattern);
            }
            else
            {
                sPattern = "^" + matches.captured(1) + "\\S";
                reIndentSame.setPattern(sPattern);

                sPattern = "^" + matches.captured(1).left( matches.captured(1).length() - 4 ) + "\\S";
                reIndentLess.setPattern(sPattern);
            }

            while ( !atEnd() )
            {
                txt = readLine();
                if (reIndentSame.match(txt).hasMatch() || reIndentLess.match(txt).hasMatch())
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
            while ( !atEnd() )
            {
                txt = readLine();
                QRegularExpressionMatch matchesBinary = reBinary.match(txt);
                if ( matchesBinary.hasMatch() )
                    binaryStr += matchesBinary.captured(1);
                if ( txt.contains( "End" ))
                    break;

            }
            m_blockData[ elementName ] = binaryStr;
//            if ( elementName == "PrtDevMode" )
//            {
//                qDebug() << "PrtDevMode" << binaryStr.length(); // 3112
//                qDebug() << binaryStr;
//            }

            if (isReport && isReportPositionBegin) { isReport = false; isReportPositionBegin = false; }
            goto end_of_while;
        }

        // picture-data
        // http://www.lebans.com/imagecontroltoclipboard.htm
        matchesPictureData = m_rePictureData->match(txt);
        if ( matchesPictureData.hasMatch() )
        {
            writeLine(streamDstDesign, streamDstModule, isCodeBehind, txt, codecDst->lineEnd());

            txt = readLine();
            matchesPictureDataFirstLine = m_rePictureDataFirstLine->match(txt);

            QString indent = matchesPictureDataFirstLine.captured(1);
            QString cfData = matchesPictureDataFirstLine.captured(2);
            QString mfh1   = matchesPictureDataFirstLine.captured(3);
            QString mfh2   = matchesPictureDataFirstLine.captured(4);
            QString mfh3   = matchesPictureDataFirstLine.captured(5);
            QString mfh4   = matchesPictureDataFirstLine.captured(6);
            QString rest   = matchesPictureDataFirstLine.captured(7);

            if (cfData.startsWith("03"))
            {
                // CF_METAFILEPICT = 3
                // sanitize 2 - 4 bytes to 00 ( unused )
                cfData = "0300000000000000";
                // sanitize METAFILEHEADER::hMetaFile ( always changed )
                mfh4   = "00000000";
            }
            else if (cfData.startsWith("0e"))
            {
                // CF_ENHMETAFILE = 14
                // sanitize 2 - 4 bytes to 00 ( unused )
                cfData = "0e00000000000000";
            }

            txt = indent + "0x" + cfData + mfh1 + mfh2 + mfh3 + mfh4 + rest + " ,";
            writeLine(streamDstDesign, streamDstModule, isCodeBehind, txt, codecDst->lineEnd());


            if (isReport && isReportPositionBegin) { isReport = false; isReportPositionBegin = false; }
            goto end_of_while;
        }

        if ( txt.contains("Begin Report") || txt.contains("Begin Form") )
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

QByteArray SanitizeSetting::blockData(const QString &elementName)
{
    QByteArray data;
    if ( m_blockData.contains(elementName) )
    {
        QString dataStr = m_blockData[ elementName ];
        data = QByteArray::fromHex( dataStr.toLatin1() );
    }
    return data;
}

QString SanitizeSetting::readLine()
{

    QString line;
    if (m_codecInfoSrc->codec()->name() == "Shift_JIS")
    {
        QByteArray lineData = m_deviceSrc->readLine();
        line = QString::fromLocal8Bit( lineData );
    }
    else
    {
//        line = lineData;
        line = m_streamSrc->readLine();
    }

    if (line.endsWith( m_codecInfoSrc->lineEnd() ))
        line.chop( m_codecInfoSrc->lineEnd().length() );
    return line;
}

void SanitizeSetting::writeLine( QTextStream &stDesign, QTextStream &stModule, bool isCodeBehind, QString &txt, const QString &lineEnd)
{
    if ( !isCodeBehind )
        stDesign << txt << lineEnd;
    else
    {
        // remove trailing spaces
        while ( txt.size() > 0 && txt.at( txt.size() - 1 ).isSpace() )
            txt.chop( 1 );
        stModule << txt << lineEnd;
    }
}

bool SanitizeSetting::atEnd()
{
    if (m_codecInfoSrc->codec()->name() == "Shift_JIS")
        return m_deviceSrc->atEnd();
    else
        return m_streamSrc->atEnd();
}

