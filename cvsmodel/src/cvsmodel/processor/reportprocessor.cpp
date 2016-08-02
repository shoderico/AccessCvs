#include "reportprocessor.h"

#include <QDebug>
#include <QFile>

#include "accesslib/accesslib.h"

#include "util/comptr.h"
#include "util/codecinfo.h"
#include "util/fileutil.h"

#include "cvsmodel/accessprojectcontainer.h"
#include "cvsmodel/sanitizer/accessdesignobjectsanitizer.h"
#include "util/setting.h"

#include <windows.h>
#include <wingdi.h>

#include "pch.hpp"

ReportProcessor::ReportProcessor(ProjectContainer *parent)
    : AccessDesignObjectProcessor(parent)
{
    m_objectType          = Model::Report;
    m_selectObjectType    = Model::ReportObjectType;
    m_accessObjectType    = Access::acReport;
    m_objectPathName      = "reports";
    m_containerName       = "Reports";
    m_iconPath            = ":/images/report.png";
    m_uiText              = "Reports";

    m_tempFileExtension   = "tmp";
    m_designFileExtension = "rpt";
    m_moduleFileExtension = "bas";
    m_existCheckExtension = m_designFileExtension;

    m_reportPropFileExtension = "rpp";
}

bool ReportProcessor::prepareItemCollection()
{
    if (!projectContainer<AccessProjectContainer>()->isADP() && !projectContainer<AccessProjectContainer>()->isMDB())
        return AccessDesignObjectProcessor::prepareItemCollection();

    ComPtr<Access::CurrentProject> currentProject = m_projectContainer->application<Access::Application>()->CurrentProject();
    m_objects.set( currentProject->AllReports() );

    return m_objects.is();
}

int ReportProcessor::itemCount()
{
    if (!projectContainer<AccessProjectContainer>()->isADP() && !projectContainer<AccessProjectContainer>()->isMDB())
        return AccessDesignObjectProcessor::itemCount();

    if (!m_objects.is())
        return 0;
    return m_objects->Count();
}

QAxObject *ReportProcessor::itemUnsafePtr(const QVariant &index)
{
    if (!projectContainer<AccessProjectContainer>()->isADP() && !projectContainer<AccessProjectContainer>()->isMDB())
        return AccessDesignObjectProcessor::itemUnsafePtr(index);

    if (!m_objects.is())
        return 0;
    return m_objects->Item(index);
}

bool ReportProcessor::afterImportFromTempDirToProject(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)

    // if report-prop-file doesn't exist, return true (it's normal)
    if ( !QFile( filePath( TempDir, ReportPropFile, objectName ) ).exists() )
        return true;

    // QSettings : low performance
    /*
    // open report-prop-file
    QSettings settings( filePath( TempDir, ReportPropFile, objectName ), QSettings::IniFormat, this );
    settings.setIniCodec( m_codecForCvs->codec() );

    // PrtDevMode
    settings.beginGroup("PrtDevMode");
    {
        bool hasPrtDevMode = settings.value("hasPrtDevMode", false).toBool();

        if ( hasPrtDevMode )
        {
            // open target report in design view
            ComPtr<Access::DoCmd> doCmd = m_projectSetting->application()->DoCmd();
            doCmd->OpenReport( objectName, Access::acViewDesign );
            ComPtr<Access::Reports> reports = m_projectSetting->application()->Reports();
            ComPtr<Access::Report> report = reports->Item( objectName );

            // retreive PrtDevMode
            QByteArray prtDevModeDataSrc = report->PrtDevMode().toByteArray();

            // consturct new PrtDevMode
            DEVMODEA dm;
            mempcpy( &dm, (const void*)prtDevModeDataSrc.constData(), sizeof(dm) );
            dm.dmOrientation = settings.value("dmOrientation", DMORIENT_PORTRAIT).toInt();
            dm.dmPaperSize   = settings.value("dmPaperSize",   DMPAPER_A4).toInt();
            dm.dmPaperLength = settings.value("dmPaperLength", 0).toInt(); // FIXME: need meaningful default value
            dm.dmPaperWidth  = settings.value("dmPaperWidth",  0).toInt(); // FIXME: need meaningful default value
            dm.dmScale       = settings.value("dmScale",       100).toInt();
            dm.dmColor       = settings.value("dmColor",       DMCOLOR_COLOR).toInt();

            QByteArray prtDevModeDataDst( (const char *)&dm, sizeof(dm) );

            // set new PrtDevMode to report
            report->SetPrtDevMode( prtDevModeDataDst );

            // save report
            doCmd->Close( Access::acReport, objectName, Access::acSaveYes );
        }
    }
    settings.endGroup();
    */

    // Implement own : faster
    /*
    QFile file( filePath( TempDir, ReportPropFile, objectName ) );
    file.open( QIODevice::ReadOnly / *| QFile::Text* / );
    QTextStream stream( &file );
    stream.setCodec( m_codecForCvs->codec() );

    while (!stream.atEnd())
    {
        QString txt = stream.readLine();

        if (txt == "[PrtDevMode]")
        {
            QString hasPrtDevMode = stream.readLine();
            if (hasPrtDevMode.endsWith("true"))
            {
                DEVMODEA dmTemp;
                // order is important !
                dmTemp.dmOrientation = stream.readLine().remove( 0, QString("dmOrientation=")  .length() ).toShort();
                dmTemp.dmPaperSize   = stream.readLine().remove( 0, QString("dmPaperSize=")    .length() ).toShort();
                dmTemp.dmPaperLength = stream.readLine().remove( 0, QString("dmPaperLength=")  .length() ).toShort();
                dmTemp.dmPaperWidth  = stream.readLine().remove( 0, QString("dmPaperWidth=")   .length() ).toShort();
                dmTemp.dmScale       = stream.readLine().remove( 0, QString("dmScale=")        .length() ).toShort();
                dmTemp.dmColor       = stream.readLine().remove( 0, QString("dmColor=")        .length() ).toShort();

                // open target report in design view
                ComPtr<Access::DoCmd> doCmd = m_projectSetting->application()->DoCmd();
                doCmd->OpenReport( objectName, Access::acViewDesign );
                ComPtr<Access::Reports> reports = m_projectSetting->application()->Reports();
                ComPtr<Access::Report> report = reports->Item( objectName );

                // retreive PrtDevMode
                QByteArray prtDevModeDataSrc = report->PrtDevMode().toByteArray();

                // consturct new PrtDevMode
                DEVMODEA dm;
                mempcpy( &dm, (const void*)prtDevModeDataSrc.constData(), sizeof(dm) );
                dm.dmOrientation = dmTemp.dmOrientation;
                dm.dmPaperSize   = dmTemp.dmPaperSize;
                dm.dmPaperLength = dmTemp.dmPaperLength;
                dm.dmPaperWidth  = dmTemp.dmPaperWidth;
                dm.dmScale       = dmTemp.dmScale;
                dm.dmColor       = dmTemp.dmColor;

                QByteArray prtDevModeDataDst( (const char *)&dm, sizeof(dm) );

                // set new PrtDevMode to report
                report->SetPrtDevMode( prtDevModeDataDst );

                // save report
                doCmd->Close( Access::acReport, objectName, Access::acSaveYes );

            }
        }
    }

    file.close();
    */

    // Setting
    Setting setting( filePath( TempDir, ReportPropFile, objectName ), m_codecForCvs->codec(), m_codecForCvs->bom(), m_codecForCvs->lineEnd() );
    if (!setting.load())
        return false;
    SettingElement *element = setting.at(0)->toElement();
    Q_ASSERT(element != NULL);
    Q_ASSERT(element->name() == "PrtDevMode");
    if (element->value("HasPrtDevMode", false).toBool())
    {
        DEVMODEA dmTemp;
        dmTemp.dmOrientation = element->value("dmOrientation")  .toString().toShort();
        dmTemp.dmPaperSize   = element->value("dmPaperSize")    .toString().toShort();
        dmTemp.dmPaperLength = element->value("dmPaperLength")  .toString().toShort();
        dmTemp.dmPaperWidth  = element->value("dmPaperWidth")   .toString().toShort();
        dmTemp.dmScale       = element->value("dmScale")        .toString().toShort();
        dmTemp.dmColor       = element->value("dmColor")        .toString().toShort();

        // open target report in design view
        ComPtr<Access::DoCmd> doCmd = m_projectContainer->application<Access::Application>()->DoCmd();
        doCmd->OpenReport( objectName, Access::acViewDesign );
        ComPtr<Access::Reports> reports = m_projectContainer->application<Access::Application>()->Reports();
        ComPtr<Access::Report> report = reports->Item( objectName );

        // retreive PrtDevMode
        QByteArray prtDevModeDataSrc = report->PrtDevMode().toByteArray();

        // consturct new PrtDevMode
        DEVMODEA dm;
        memcpy( &dm, (const void*)prtDevModeDataSrc.constData(), sizeof(dm) );
        dm.dmOrientation = dmTemp.dmOrientation;
        dm.dmPaperSize   = dmTemp.dmPaperSize;
        dm.dmPaperLength = dmTemp.dmPaperLength;
        dm.dmPaperWidth  = dmTemp.dmPaperWidth;
        dm.dmScale       = dmTemp.dmScale;
        dm.dmColor       = dmTemp.dmColor;

        QByteArray prtDevModeDataDst( (const char *)&dm, sizeof(dm) );

        // set new PrtDevMode to report
        report->SetPrtDevMode( prtDevModeDataDst );

        // save report
        doCmd->Close( Access::acReport, objectName, Access::acSaveYes );
    }

    return true;
}

bool ReportProcessor::afterSanitizeTempDir(QAxObject *object, const QString &objectName, AccessDesignObjectSanitizer *sanitizer)
{
    Q_UNUSED(object);

    // report properties

    FileUtil::deleteFile( filePath( TempDir, ReportPropFile, objectName ) );

    // QSettings : low performance
    /*
    // open report-prop-file
    QSettings settings( filePath( TempDir, ReportPropFile, objectName ), QSettings::IniFormat, 0 );
    settings.setIniCodec( m_codecForCvs->codec() );

    // PrtDevMode
    settings.beginGroup("PrtDevMode");
    {
        QByteArray prtDevModeData = sanitizer->blockData( "PrtDevMode" );
        if ( prtDevModeData.size() > 0 )
        {
            const void *pprtDevModeData = (const void*)prtDevModeData.constData();
            const DEVMODEA *pdm = static_cast<const DEVMODEA*>(pprtDevModeData);

            settings.setValue( "hasPrtDevMode", true );
            settings.setValue( "dmOrientation", pdm->dmOrientation );
            settings.setValue( "dmPaperSize",   pdm->dmPaperSize );
            settings.setValue( "dmPaperLength", pdm->dmPaperLength );
            settings.setValue( "dmPaperWidth",  pdm->dmPaperWidth );
            settings.setValue( "dmScale",       pdm->dmScale );
            settings.setValue( "dmColor",       pdm->dmColor );

        }
        else
        {
            settings.setValue( "hasPrtDevMode", false );
        }
    }
    settings.endGroup();
    */


    // Implement own
    /*
    QFile file( filePath( TempDir, ReportPropFile, objectName ) );
    file.open( QIODevice::WriteOnly / *| QFile::Text* / );
    QTextStream stream( &file );
    stream.setCodec( m_codecForCvs->codec() );
    stream.setGenerateByteOrderMark( m_codecForCvs->bom() );

    stream << "[PrtDevMode]" << m_codecForCvs->lineEnd();
    {
        QByteArray prtDevModeData = sanitizer->blockData( "PrtDevMode" );
        if ( prtDevModeData.size() > 0 )
        {
            const void *pprtDevModeData = (const void*)prtDevModeData.constData();
            const DEVMODEA *pdm = static_cast<const DEVMODEA*>(pprtDevModeData);

            stream << "hasPrtDevMode=true" << m_codecForCvs->lineEnd();
            // order is important !
            stream << QString("dmOrientation=%1")   .arg( pdm->dmOrientation )  << m_codecForCvs->lineEnd();
            stream << QString("dmPaperSize=%1")     .arg( pdm->dmPaperSize )    << m_codecForCvs->lineEnd();
            stream << QString("dmPaperLength=%1")   .arg( pdm->dmPaperLength )  << m_codecForCvs->lineEnd();
            stream << QString("dmPaperWidth=%1")    .arg( pdm->dmPaperWidth )   << m_codecForCvs->lineEnd();
            stream << QString("dmScale=%1")         .arg( pdm->dmScale )        << m_codecForCvs->lineEnd();
            stream << QString("dmColor=%1")         .arg( pdm->dmColor )        << m_codecForCvs->lineEnd();

        }
        else
        {
            stream << "hasPrtDevMode=false" << m_codecForCvs->lineEnd();
        }
    }


    file.close();
    */

    // Setting
    Setting setting( filePath( TempDir, ReportPropFile, objectName ), m_codecForCvs->codec(), m_codecForCvs->bom(), m_codecForCvs->lineEnd() );
    SettingElement *element = setting.append("PrtDevMode");
    QByteArray prtDevModeData = sanitizer->blockData( "PrtDevMode" );
    if (prtDevModeData.size() > 0)
    {
        const void *pprtDevModeData = (const void*)prtDevModeData.constData();
        const DEVMODEA *pdm = static_cast<const DEVMODEA*>(pprtDevModeData);

        element->append("HasPrtDevMode", true);

        element->append("dmOrientation",    pdm->dmOrientation);
        element->append("dmPaperSize",      pdm->dmPaperSize);
        element->append("dmPaperLength",    pdm->dmPaperLength);
        element->append("dmPaperWidth",     pdm->dmPaperWidth);
        element->append("dmScale",          pdm->dmScale);
        element->append("dmColor",          pdm->dmColor);


    }
    else
    {
        element->append("HasPrtDevMode", false);
    }
    setting.save();


    return true;
}
