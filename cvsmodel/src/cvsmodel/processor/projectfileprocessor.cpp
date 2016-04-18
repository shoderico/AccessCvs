#include "projectfileprocessor.h"

#include <QDebug>
#include <QFile>
#include <QRegularExpression>
#include <QUuid>

#include "accesslib/accesslib.h"

#include "util/comptr.h"
#include "util/codecinfo.h"

#include "cvsmodel/projectsetting.h"
#include "cvsmodel/setting.h"

#include <windows.h>


ProjectFileProcessor::ProjectFileProcessor(ProjectSetting *parent)
    : ProjectLevelObjectProcessor(parent)
{
    m_objectName          = "AccessProject";

    m_objectType          = Model::ProjectFile;
    m_accessObjectType    = -1;
    m_objectPathName      = "";
    m_containerName       = "";

    m_tempFileExtension   = "accproj";
    m_designFileExtension = m_tempFileExtension;
    m_moduleFileExtension = "";
    m_existCheckExtension = m_tempFileExtension;
}

bool ProjectFileProcessor::exportFromProjectToTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    Q_UNUSED(objectName)

    deleteAllFileFromTempDir(m_objectName);

    QMap<QString, ProjectFileProperty*> propMap;
    loadProperties(propMap);

    // QSettings
    /*
    {
        QSettings settings( filePath(TempDir, TempFile, m_objectName), QSettings::IniFormat, this );
        settings.setIniCodec( m_codecForCvs->codec() );
        QStringList propNames( propMap.keys() );
        propNames.sort(Qt::CaseSensitive);
        foreach (const QString propName, propNames)
        {
            ProjectFileProperty *prop = propMap.value(propName);

            //[ANSI%20Query%20Mode]
            //Name=ANSI Query Mode
            //Type=4
            //Value=0
            settings.beginGroup(prop->Name);
            settings.setValue( "Name", prop->Name );
            if (prop->Type != -1 )
                settings.setValue( "Type", prop->Type );
            settings.setValue( "Value", prop->Value );
            settings.endGroup();

        }
    }
    */

    // Implement Own

    // Setting
    Setting setting( filePath(TempDir, TempFile, m_objectName), m_codecForCvs->codec(), m_codecForCvs->bom(), m_codecForCvs->lineEnd() );
    QStringList propNames( propMap.keys() );
    propNames.sort(Qt::CaseSensitive);
    SettingElement *propsElement = setting.append("Properties");
    foreach (const QString propName, propNames)
    {
        ProjectFileProperty *prop = propMap.value(propName);

        SettingElement *element = propsElement->append("Property");
        element->append("Name", prop->Name);
        if (prop->Type != -1)
            element->append("Type", prop->Type);
        element->append("Value", prop->Value);
        qDebug() << "export : " << prop->Name << " : " << prop->Value << " : " << prop->Type << " : QVariant.type() = " << prop->Value.type();
    }
    setting.save();

    return true;
}

bool ProjectFileProcessor::importFromTempDirToProject(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    Q_UNUSED(objectName)

    if ( !QFile( filePath(TempDir, TempFile, m_objectName) ).exists())
        return true;

    // properties in tempdir
    QMap<QString, ProjectFileProperty*> propMapTempDir;
    // QSettings
    /*
    {
        QSettings settings( filePath(TempDir, TempFile, m_objectName), QSettings::IniFormat, this );
        settings.setIniCodec( m_codecForCvs->codec() );
        QStringList propNames = settings.childGroups();
        foreach ( const QString propName, propNames )
        {
            settings.beginGroup(propName);
            propMapTempDir.insert( propName, new ProjectFileProperty(propName, settings.value( "Type", -1 ).toInt(), settings.value("Value")) );
            settings.endGroup();
        }
    }
    */
    // Implement Own

    // Setting
    Setting setting( filePath(TempDir, TempFile, m_objectName), m_codecForCvs->codec(), m_codecForCvs->bom(), m_codecForCvs->lineEnd() );
    if (!setting.load())
        return false;
    SettingElement *propsElement = setting.at(0)->toElement();
    Q_ASSERT(propsElement != NULL);
    Q_ASSERT(propsElement->name() == "Properties");
    for ( int i = 0 ; i < propsElement->count() ; ++i )
    {
        SettingElement *element = propsElement->at(i)->toElement();
        Q_ASSERT(element != NULL);
        Q_ASSERT(element->name() == "Property");
        QString propName = element->value("Name").toString();
        int propType = element->value("Type", -1).toInt();
        QVariant propValue = element->value("Value");
        switch (propType) {
            case QMetaType::Int:
//            case QVariant::Int: // obsolete
                propValue = propValue.toInt();
                break;
            case QMetaType::QString:
//            case QVariant::String: // obsolete
                propValue = propValue.toString();
                break;
            default:
                propValue = propValue.toString();
                break;
        }
        propMapTempDir.insert( propName, new ProjectFileProperty(propName, propType, propValue) );

    }



    // properties in project
    QMap<QString, ProjectFileProperty*> propMapProject;
    loadProperties(propMapProject);

    QList<ProjectFileProperty*> inBoth;
    QList<ProjectFileProperty*> inProjectOnly;
    QList<ProjectFileProperty*> inTempDirOnly;

    // both exists
    foreach ( const QString &propName, propMapTempDir.keys() )
        if ( propMapProject.contains( propName ))
            inBoth << propMapTempDir.value(propName);

    // exists in project only
    foreach ( const QString &propName, propMapProject.keys() )
        if ( !propMapTempDir.contains( propName ))
            inProjectOnly << propMapProject.value(propName);

    // exists in tempdir only
    foreach ( const QString &propName, propMapTempDir.keys() )
        if ( !propMapProject.contains( propName ))
            inTempDirOnly << propMapTempDir.value(propName);


    if (m_projectSetting->isMDB())
    {
        ComPtr<DAO::Database> currentDb = m_projectSetting->application()->CurrentDb();
        ComPtr<DAO::Properties> props = currentDb->Properties();

        // both exists : update
        foreach ( ProjectFileProperty *prop , inBoth )
        {
            ComPtr<DAO::Property> p = props->Item( prop->Name );
            p->SetValue( prop->Value );
            qDebug() << "update : " << prop->Name << " : " << prop->Value << " : " << prop->Type;
        }

        // exists in project only : remove
        foreach ( ProjectFileProperty *prop , inProjectOnly )
        {
            props->Delete( prop->Name );
            qDebug() << "delete : " << prop->Name << " : " << prop->Value << " : " << prop->Type;
        }

        // exists in tempdir only : insert
        foreach ( ProjectFileProperty *prop , inTempDirOnly )
        {
            ComPtr<DAO::Property> p = currentDb->CreateProperty( prop->Name, prop->Type, prop->Value );
            qDebug() << "insert : " << prop->Name << " : " << prop->Value << " : " << prop->Type;
            IDispatch *idisp = 0;
            p->queryInterface( QUuid(IID_IDispatch), (void**)&idisp);
            if (idisp)
            {
                props->Append( idisp );
                idisp->Release();
            }
        }
    }
    else if (m_projectSetting->isADP())
    {
        ComPtr<Access::CurrentProject> currentProject = m_projectSetting->application()->CurrentProject();
        ComPtr<Access::AccessObjectProperties> props = currentProject->Properties();

        // both exists : update
        foreach ( ProjectFileProperty *prop , inBoth )
        {
            ComPtr<Access::AccessObjectProperty> p = props->Item( prop->Name );
            p->SetValue( prop->Value );
            qDebug() << "update : " << prop->Name << " : " << prop->Value << " : " << prop->Type;
        }

        // exists in project only : remove
        foreach ( ProjectFileProperty *prop , inProjectOnly )
        {
            props->Remove( prop->Name );
            qDebug() << "delete : " << prop->Name << " : " << prop->Value << " : " << prop->Type;
        }

        // exists in tempdir only : insert
        foreach ( ProjectFileProperty *prop , inTempDirOnly )
        {
            props->Add( prop->Name, prop->Value );
            qDebug() << "insert : " << prop->Name << " : " << prop->Value << " : " << prop->Type;
        }
    }

    return true;
}

void ProjectFileProcessor::loadProperties(QMap<QString, ProjectFileProcessor::ProjectFileProperty *> &propMap)
{
    QString pattern;
    pattern  = "^(";
    // DAO.Property : Read-Only
    pattern += "CollatingOrder";
    pattern += "|Connect";
    pattern += "|Connection";
    pattern += "|DesignMasterID";
    pattern += "|Name";
    pattern += "|QueryTimeout";
    pattern += "|RecordsAffected";
    pattern += "|ReplicaID";
    pattern += "|Transactions";
    pattern += "|Updatable";
    pattern += "|Version";
    // Application Properties : Read-Only
    pattern += "|AccessVersion";
    pattern += "|Build";
    pattern += "|ProjVer";
    pattern += "|HasOfflineLists";
    pattern += "|NavPane .*";
    pattern += "";
    pattern += ")$";
    QRegularExpression regExp;
    regExp.setPattern( pattern );

    if ( m_projectSetting->isMDB() )
    {
        ComPtr<DAO::Database> currentDb = m_projectSetting->application()->CurrentDb();
        ComPtr<DAO::Properties> props = currentDb->Properties();
        for ( int i = 0 ; i < props->Count() ; ++i )
        {
            ComPtr<DAO::Property> prop = props->Item(i);
            if (!regExp.match(prop->Name()).hasMatch())
            {
                // write Name, Type, Value
                propMap.insert( prop->Name(), new ProjectFileProperty(prop->Name(), prop->Type(), prop->Value()) );
            }
        }
    }
    else if (m_projectSetting->isADP())
    {
        ComPtr<Access::CurrentProject> currentProject = m_projectSetting->application()->CurrentProject();
        ComPtr<Access::AccessObjectProperties> props = currentProject->Properties();

        for ( int i = 0 ; i < props->Count() ; ++i )
        {
            ComPtr<Access::AccessObjectProperty> prop = props->Item(i);
            if (!regExp.match(prop->Name()).hasMatch())
            {
                propMap.insert( prop->Name(), new ProjectFileProperty(prop->Name(), prop->Value().type(), prop->Value()));
            }
        }
    }
}
