#include "accessdesignobjectprocessor.h"

#include <QDebug>
#include <QTextCodec>
#include <QFile>
#include <QTextStream>

#include "accesslib/accesslib.h"

#include "util/comptr.h"
#include "util/codecinfo.h"
#include "util/fileutil.h"

#include "cvsmodel/accessprojectcontainer.h"
#include "cvsmodel/sanitizer/accessdesignobjectsanitizer.h"
#include "cvsmodel/setting.h"

AccessDesignObjectProcessor::AccessDesignObjectProcessor(ProjectContainer *parent)
    : AccessObjectProcessor(parent)
    , m_sanitizer(new AccessDesignObjectSanitizer(this))
{
}

bool AccessDesignObjectProcessor::sanitizeTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    Q_UNUSED(objectName)

    // Target object types are Form, Report, Macro
    // Logic is very large and unstable.

    bool hasModule = !m_moduleFileExtension.isEmpty();

    // prepare files
    QFile fileSrc      ( filePath(TempDir, TempFile,   objectName) );
    QFile fileDstDesign( filePath(TempDir, DesignFile, objectName) );
    QFile fileDstModule;
    if (hasModule)
        fileDstModule.setFileName( filePath(TempDir, ModuleFile, objectName) );

    // delete existing files
    FileUtil::deleteFile( fileDstDesign.fileName() );
    if (hasModule)
        FileUtil::deleteFile( fileDstModule.fileName() );


    // open files
    fileSrc.open( QIODevice::ReadOnly );
    fileDstDesign.open( QIODevice::WriteOnly );
    if (hasModule)
        fileDstModule.open( QIODevice::WriteOnly );

    // codec
//    determineCodecForProject();

    // streams
    QTextStream streamSrc( &fileSrc );
    QTextStream streamDstDesign( &fileDstDesign );
    QTextStream streamDstModule;

    streamSrc.setDevice( &fileSrc );
    streamSrc.setCodec( m_codecForProject->codec() );
    // bom is auto

    streamDstDesign.setDevice( &fileDstDesign );
    streamDstDesign.setCodec( m_codecForCvs->codec() );
    streamDstDesign.setGenerateByteOrderMark( m_codecForCvs->bom() );

    if (hasModule)
    {
        streamDstModule.setDevice( &fileDstModule );
        streamDstModule.setCodec( m_codecForCvs->codec() );
        streamDstModule.setGenerateByteOrderMark( m_codecForCvs->bom() );
    }


    // sanitize
    AccessDesignObjectSanitizer sanitizer;
    sanitizer.sanitize( streamSrc, m_codecForProject, streamDstDesign, streamDstModule, m_codecForCvs );
    //m_sanitizer->sanitize( streamSrc, m_codecForProject, streamDstDesign, streamDstModule, m_codecForCvs );


    // post process for subclasses.
    afterSanitizeTempDir(object, objectName, &sanitizer);


    // close files
    fileSrc.close();
    fileDstDesign.close();
    if (hasModule)
        fileDstModule.close();


    // delete temp file
//    FileUtil::deleteFile( fileSrc.fileName() ); // keep original files

    // Check design data corruption for Unicode-Shift_JIS problem.
    // FIXME: this must be independent function and store results to ObjectItem
    //if ( m_codecForProject->codec()->name() ==  "???") // FIXME: check only for Shift_JIS.
    /*
    {
        QFile fileDstDesign( filePath(TempDir, DesignFile, objectName) );
        fileDstDesign.open( QIODevice::ReadOnly );
        QTextStream streamDstDesign( &fileDstDesign );
        QString line = "";
        int lineNum = 0;
        while (!streamDstDesign.atEnd())
        {
            ++lineNum;
            line = streamDstDesign.readLine();
            if (line.contains('?'))
            {
                qCritical() << QString("'?' found!  line :%1, content :%2, file :%3").arg(lineNum).arg(line).arg(fileDstDesign.fileName());
            }
        }
        fileDstDesign.close();
    }
    */

    return true;
}

bool AccessDesignObjectProcessor::desanitizeTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    Q_UNUSED(objectName)

    // Target object types are Form, Report, Macro
    // Logic is very large and unstable.

    // join design and module file into one.
    // and save to tempFile in codecForProject


    bool hasModule = !m_moduleFileExtension.isEmpty();

    // prepare files
    QFile fileSrc      ( filePath(TempDir, TempFile,   objectName) );
    QFile fileDstDesign( filePath(TempDir, DesignFile, objectName) );
    QFile fileDstModule;
    if (hasModule)
    {
        fileDstModule.setFileName( filePath(TempDir, ModuleFile, objectName) );

        // treat as no-module if module file is absent
        if (!fileDstModule.exists())
            hasModule = false;
    }

    // delete existing files
    FileUtil::deleteFile( fileSrc.fileName() );


    // open files
    fileSrc.open( QIODevice::WriteOnly );
    fileDstDesign.open( QIODevice::ReadOnly );
    if (hasModule)
        fileDstModule.open( QIODevice::ReadOnly );

    // codec
//    determineCodecForProject();

    // streams
    QTextStream streamSrc( &fileSrc );
    QTextStream streamDstDesign( &fileDstDesign );
    QTextStream streamDstModule;

    streamSrc.setDevice( &fileSrc );
    streamSrc.setCodec( m_codecForProject->codec() );
    streamSrc.setGenerateByteOrderMark( m_codecForProject->bom() );

    streamDstDesign.setDevice( &fileDstDesign );
    streamDstDesign.setCodec( m_codecForCvs->codec() );
    streamDstDesign.setGenerateByteOrderMark( m_codecForCvs->bom() );

    if (hasModule)
    {
        streamDstModule.setDevice( &fileDstModule );
        streamDstModule.setCodec( m_codecForCvs->codec() );
        streamDstModule.setGenerateByteOrderMark( m_codecForCvs->bom() );
    }


    // de-sanitize : just join two files
    {
        if (m_codecForProject->codec()->name() == "Shift_JIS")
        {
            while (!streamDstDesign.atEnd())
            {
                QString line = streamDstDesign.readLine();
                line += m_codecForProject->lineEnd();

                QByteArray lineData = line.toLocal8Bit();
                fileSrc.write( lineData );
            }
        }
        else
        {
            while (!streamDstDesign.atEnd())
            {
                QString line = streamDstDesign.readLine();
                streamSrc << line << m_codecForProject->lineEnd();
            }
        }
    }
    if (hasModule)
    {
        if (m_codecForProject->codec()->name() == "Shift_JIS")
        {
            while (!streamDstModule.atEnd())
            {
                QString line = streamDstModule.readLine();
                line += m_codecForProject->lineEnd();
                QByteArray lineData = line.toLocal8Bit();
                fileSrc.write( lineData );
            }
        }
        else
        {
            while (!streamDstModule.atEnd())
                streamSrc << streamDstModule.readLine() << m_codecForProject->lineEnd();
        }
    }


    // TODO: add post process for subclasses.


    // close files
    fileSrc.close();
    fileDstDesign.close();
    if (hasModule)
        fileDstModule.close();


    // delete files
//    FileUtil::deleteFile( fileDstDesign.fileName() ); // keep original files
//    FileUtil::deleteFile( fileDstModule.fileName() ); // keep original files

    return true;
}

void AccessDesignObjectProcessor::determineCodecForProject()
{
    if (!m_codecForProject)
    {
        bool isUcs2 = true;

        if (false)
        {
            // https://github.com/timabell/msaccess-vcs-integration/blob/master/MSAccess-VCS/VCS_File.bas

            // In this way, we have to export at least one object from project.

            QString objectName = "????";
            QFile fileCodec ( filePath(TempDir, TempFile, objectName) );
            fileCodec.open( QIODevice::ReadOnly );
            QByteArray header = fileCodec.read( 2 );
            fileCodec.close();

            if ( header.at(0) == '\xFF' && header.at(1) == '\xFE' )
                isUcs2 = true;
            else
                isUcs2 = false;
        }



        if (true)
        {
            // TODO: we need some research this logic is right

            if ( projectContainer<AccessProjectContainer>()->isADP())
            {
                isUcs2 = false;
            }
            else
            {
                QString accessVer = m_projectContainer->application<Access::Application>()->SysCmd( Access::acSysCmdAccessVer ).toString();

                // Access 2000
                if ( accessVer == "9.0" )
                {
                    isUcs2 = false;
                }
                else
                {
                    ComPtr<Access::CurrentProject> currentProject = m_projectContainer->application<Access::Application>()->CurrentProject();
                    int fileFormat = currentProject->FileFormat();
                    switch (fileFormat)
                    {
                        case Access::acFileFormatAccess2:       isUcs2 = false; break;
                        case Access::acFileFormatAccess95:      isUcs2 = false; break;
                        case Access::acFileFormatAccess97:      isUcs2 = false; break;
                        case Access::acFileFormatAccess2000:    isUcs2 = false; break;
                        case Access::acFileFormatAccess2002:    isUcs2 = false; break;
                        case Access::acFileFormatAccess2007:    isUcs2 = true;  break;
                        default:                                isUcs2 = true;  break;
                    }
                }
            }
        }

        m_codecForProject = new CodecInfo(this);
        if ( isUcs2 )
        {
            m_codecForProject->setCodec( QTextCodec::codecForName( "UTF-16LE" ) );
            m_codecForProject->setBom( true );
        }
        else
        {
            m_codecForProject->setCodec( QTextCodec::codecForName( "Shift_JIS" ) );
            m_codecForProject->setBom( false );
        }

        m_codecForProject->setLineEnd( "\r\n" );
    }
}

bool AccessDesignObjectProcessor::afterSanitizeTempDir(QAxObject *object, const QString &objectName, AccessDesignObjectSanitizer *sanitizer)
{
    Q_UNUSED(object)
    Q_UNUSED(objectName)
    Q_UNUSED(sanitizer)
    return true;
}
