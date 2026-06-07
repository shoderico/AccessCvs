#include "referenceprocessor.h"

#include <QDebug>
#include <QDir>
#include <QFile>

#include "officelib/officelib.h"

#include "util/comptr.h"
#include "util/codecinfo.h"

#include "cvsmodel/projectcontainer.h"
#include "util/setting.h"

#include "pch.hpp"


ReferenceProcessor::ReferenceProcessor(ProjectContainer *parent)
    : VBEProcessor(parent)
{
    m_objectName          = "Reference";

    m_objectType          = Model::Reference;
    m_selectObjectType    = Model::ReferenceObjectType;
    m_accessObjectType    = -1;
    m_objectPathName      = "";
    m_containerName       = "";
    m_iconPath            = ":/images/page_link.png";
    m_uiText              = "Reference";

    m_tempFileExtension   = "ref";
    m_designFileExtension = m_tempFileExtension;
    m_moduleFileExtension = "";
    m_existCheckExtension = m_tempFileExtension;
}

bool ReferenceProcessor::exportFromProjectToTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    Q_UNUSED(objectName)

    deleteAllFileFromTempDir(m_objectName);

    ComPtr<VBIDE::VBProject> vbProject = currentVBProject();
    if ( !vbProject.is() )
    {
        qDebug() << "ReferenceProcessor::exportFromProjectToTempDir : vbProject is NULL ";
        return true;
    }
    ComPtr<VBIDE::References> references = vbProject->References();
    if ( !references.is() )
    {
        qDebug() << "ReferenceProcessor::exportFromProjectToTempDir : references is NULL ";
        return true;
    }
    int nCount = references->Count();

    // QSettings
    /*
    QSettings settings( filePath(TempDir, TempFile, m_objectName), QSettings::IniFormat, this );
    settings.setIniCodec( m_codecForCvs->codec() );

    settings.beginWriteArray("Reference");
    for ( int i = 1 ; i <= nCount ; ++i )
    {
        ComPtr<Access::Reference> reference = references->Item( i );

        settings.setArrayIndex( i-1 );

        QString name     = reference->Name();
        bool    builtIn  = reference->BuiltIn();
        QString guid     = reference->Guid();
        int     major    = reference->Major();
        int     minor    = reference->Minor();
        QString fullPath = reference->FullPath();

        if ( !guid.isEmpty() )
            fullPath = "";

        settings.setValue( "BuiltIn",  builtIn );
        settings.setValue( "Name",     name );
        settings.setValue( "Guid",     guid );
        settings.setValue( "Major",    major );
        settings.setValue( "Minor",    minor );
        settings.setValue( "FullPath", fullPath );

    }
    settings.endArray();
    */

    // Implement Own
    /*
    QFile file( filePath(TempDir, TempFile, m_objectName) );
    file.open(QIODevice::WriteOnly);
    QTextStream stream( &file );
    stream.setGenerateByteOrderMark( m_codecForCvs->bom() );
    stream.setCodec( m_codecForCvs->codec() );
    QString lineEnd = m_codecForCvs->lineEnd();

    for ( int i = 1 ; i <= nCount ; ++i )
    {
        ComPtr<Access::Reference> reference = references->Item( i );

        QString name     = reference->Name();
        bool    builtIn  = reference->BuiltIn();
        QString guid     = reference->Guid();
        int     major    = reference->Major();
        int     minor    = reference->Minor();
        QString fullPath = reference->FullPath();

        if ( !guid.isEmpty() )
            fullPath = "";

        stream << "Begin Reference" << lineEnd;

        stream << "    " << "BuildIn"   << " =" << (builtIn ? "true" : "false") << lineEnd;
        stream << "    " << "Name"      << " =" << name                         << lineEnd;
        stream << "    " << "Guid"      << " =" << guid                         << lineEnd;
        stream << "    " << "Major"     << " =" << QString::number(major)       << lineEnd;
        stream << "    " << "Minor"     << " =" << QString::number(minor)       << lineEnd;
        stream << "    " << "FullPath"  << " =" << fullPath                     << lineEnd;

        stream << "End" << lineEnd;
    }

    file.close();
    */

    // Setting
    Setting setting(filePath(TempDir, TempFile, m_objectName), m_codecForCvs->codec(), m_codecForCvs->bom(), m_codecForCvs->lineEnd());
    SettingElement *refsElement = setting.append("References");
    for ( int i = 1 ; i <= nCount ; ++i )
    {
        ComPtr<VBIDE::Reference> reference = references->Item( i );

        QString name     = reference->Name();
        bool    builtIn  = reference->BuiltIn();
        QString guid     = reference->Guid();
        int     major    = reference->Major();
        int     minor    = reference->Minor();
        QString fullPath = reference->FullPath();

        if ( !guid.isEmpty() )
            fullPath = "";

        // project file reference : make it relative path
        if ( !fullPath.isEmpty() )
        {
            if ( fullPath.endsWith( ".accdb", Qt::CaseInsensitive )
                 || fullPath.endsWith( ".mdb", Qt::CaseInsensitive )
                 || fullPath.endsWith( ".adp", Qt::CaseInsensitive )
                 )
            {
                QDir dir( m_projectContainer->currentProjectPath() );

                fullPath = dir.relativeFilePath( fullPath );
            }
        }

        SettingElement *element = refsElement->append("Reference");
        element->append( "BuiltIn",     builtIn );
        element->append( "Name",        name );
        element->append( "Guid",        guid );
        element->append( "Major",       major );
        element->append( "Minor",       minor );
        element->append( "FullPath",    fullPath );
    }
    setting.save();

    return true;
}

bool ReferenceProcessor::importFromTempDirToProject(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    Q_UNUSED(objectName)

    if ( !QFile( filePath(TempDir, TempFile, m_objectName) ).exists())
        return true;

    ComPtr<VBIDE::VBProject> vbProject = currentVBProject();
    ComPtr<VBIDE::References> references = vbProject->References();

    // clear current references
    {
        int nCount = references->Count();
        for ( int i = nCount ; i >= 1 ; --i )
        {

            ComPtr<VBIDE::Reference> reference = references->Item( i );
            bool refBuiltIn = reference->BuiltIn();
            if (!refBuiltIn)
            {
                references->Remove( reference.ptr() );
            }
        }
    }


    // QSettings
    /*
    QSettings settings( filePath(TempDir, TempFile, m_objectName), QSettings::IniFormat, this );
    settings.setIniCodec( m_codecForCvs->codec() );

    // add references
    int nCount = settings.beginReadArray("Reference");
    for ( int i = 1 ; i <= nCount ; ++i )
    {
        settings.setArrayIndex( i-1 );

        QString name     = settings.value("Name").toString();   Q_UNUSED(name)
        bool    builtIn  = settings.value("BuiltIn").toBool();
        QString guid     = settings.value("Guid").toString();
        int     major    = settings.value("Major").toInt();
        int     minor    = settings.value("Minor").toInt();
        QString fullPath = settings.value("FullPath").toString();

        if ( !builtIn )
        {
            if ( !guid.isEmpty() )
            {
                ComPtr<Access::Reference> reference = references->AddFromGuid(guid, major, minor);
                Q_UNUSED(reference)
                // NOTE: error 32813 may occur : The reference is already present in the access project
            }
            else
            {
                ComPtr<Access::Reference> reference = references->AddFromFile(fullPath);
                Q_UNUSED(reference)
            }
        }
    }
    settings.endArray();
    */

    // Implement Own
    /*
    QFile file( filePath(TempDir, TempFile, m_objectName) );
    file.open(QIODevice::ReadOnly);
    QTextStream stream( &file );
    stream.setGenerateByteOrderMark( m_codecForCvs->bom() );
    stream.setCodec( m_codecForCvs->codec() );

    QRegularExpression regExpBegin;
    regExpBegin.setPattern("^\\s*Begin (.*)$");
    QRegularExpression regExpEnd;
    regExpEnd.setPattern("^\\s*End$");
    QRegularExpression regExpKeyValue;
    regExpKeyValue.setPattern("^\\s*([^=]+) =(.*)$");

    while (!stream.atEnd())
    {
        QString line = stream.readLine();

        QRegularExpressionMatch matchBegin = regExpBegin.match(line);
        if (matchBegin.hasMatch())
        {
            QString element = matchBegin.captured(1);
            if (element == "Reference")
            {
                QString name     ;
                bool    builtIn  = true;
                QString guid     ;
                int     major    = 0;
                int     minor    = 0;
                QString fullPath ;

                while (!stream.atEnd())
                {
                    line = stream.readLine();

                    if (regExpEnd.match(line).hasMatch())
                        break;

                    QRegularExpressionMatch matchKeyValue = regExpKeyValue.match(line);
                    if (matchKeyValue.hasMatch())
                    {
                        QString key = matchKeyValue.captured(1);
                        QString value = matchKeyValue.captured(2);

                        if (key == "Name")
                            name = value;
                        else if (key == "BuiltIn")
                            builtIn = ( (value == "true") ? true : false );
                        else if (key == "Guid")
                            guid = value;
                        else if (key == "Major")
                            major = value.toInt();
                        else if (key == "Minor")
                            minor = value.toInt();
                        else if (key == "FullPath")
                            fullPath = value;
                    }
                }

                if ( !builtIn )
                {
                    if ( !guid.isEmpty() )
                    {
                        ComPtr<Access::Reference> reference = references->AddFromGuid(guid, major, minor);
                        Q_UNUSED(reference)
                        // NOTE: error 32813 may occur : The reference is already present in the access project
                    }
                    else
                    {
                        ComPtr<Access::Reference> reference = references->AddFromFile(fullPath);
                        Q_UNUSED(reference)
                    }
                }

            }
        }
    }

    file.close();
    */

    // Setting
    Setting setting(filePath(TempDir, TempFile, m_objectName), m_codecForCvs->codec(), m_codecForCvs->bom(), m_codecForCvs->lineEnd());
    if (!setting.load())
        return false;
    SettingElement *refsElement = setting.at(0)->toElement();
    Q_ASSERT(refsElement != NULL);
    Q_ASSERT(refsElement->name() == "References");
    for ( int i = 0 ; i < refsElement->count() ; ++i )
    {
        SettingElement *element = refsElement->at(i)->toElement();
        Q_ASSERT(element != NULL);
        Q_ASSERT(element->name() == "Reference");

        QString name     = element->value("Name"            ).toString();
        bool    builtIn  = element->value("BuiltIn" , true  ).toBool();
        QString guid     = element->value("Guid"            ).toString();
        int     major    = element->value("Major"   , 0     ).toInt();
        int     minor    = element->value("Minor"   , 0     ).toInt();
        QString fullPath = element->value("FullPath"        ).toString();

        if ( !builtIn )
        {
            if ( !guid.isEmpty() )
            {
                ComPtr<VBIDE::Reference> reference = references->AddFromGuid(guid, major, minor);
                Q_UNUSED(reference)
                // NOTE: error 32813 may occur : The reference is already present in the access project
            }
            else
            {
                // project file : relative path to absolute path
                if ( fullPath.endsWith( ".accdb", Qt::CaseInsensitive )
                     || fullPath.endsWith( ".mdb", Qt::CaseInsensitive )
                     || fullPath.endsWith( ".adp", Qt::CaseInsensitive )
                     )
                {
                    QDir dir( m_projectContainer->currentProjectPath() );
                    fullPath = QDir::cleanPath( dir.filePath( fullPath ) ).replace(QString('/'),QString('\\'));
                }

                ComPtr<VBIDE::Reference> reference = references->AddFromFile(fullPath);
                Q_UNUSED(reference)
            }
        }
    }

    return true;
}
