#include "objectsetting.h"

#include <QDir>

#include <QTextCodec>
#include <QFile>
#include <QTextStream>

#include "officelib/officelib.h"

#include "projectsetting.h"

ObjectSetting::ObjectSetting(ProjectSetting *parent)
    : QObject(parent)
    , m_projectSetting(parent)
{
}


bool ObjectSetting::isTargetObject(QAxObject *object) const
{
    // TODO:
    //   use polymorphism to solve this problem.

    DAO::TableDef *tableDef = dynamic_cast<DAO::TableDef*>(object);
    if (tableDef)
    {
        return !tableDef->Name().startsWith("MSys") && tableDef->Connect().isEmpty();
    }

    DAO::QueryDef *queryDef = dynamic_cast<DAO::QueryDef*>(object);
    if (queryDef)
    {
        return !queryDef->Name().startsWith("~") && queryDef->Connect().isEmpty();
    }

    DAO::Document *document = dynamic_cast<DAO::Document*>(object);
    if (document)
    {
        return !document->Name().startsWith("~");
    }

    Access::AccessObject *accObject = dynamic_cast<Access::AccessObject*>(object);
    if (accObject)
    {
        return !accObject->Name().startsWith("~");
    }

    return false;
}


QString ObjectSetting::tempFileInTempPath(const QString &objectName)
{
    return tempObjectPath() + "\\" + objectName + "." + m_tempFileExtension;
}


QString ObjectSetting::designFileInTempPath(const QString &objectName)
{
    return tempObjectPath() + "\\" + objectName + "." + m_designFileExtension;
}


QString ObjectSetting::moduleFileInTempPath(const QString &objectName)
{
    return tempObjectPath() + "\\" + objectName + "." + m_moduleFileExtension;
}


QString ObjectSetting::sourceObjectPath() const
{
    QString path = m_projectSetting->sourcePath();
    if ( !m_objectPathName.isEmpty() )
        path += "\\" + m_objectPathName;
    return path;
}

QString ObjectSetting::tempObjectPath() const
{
    QString path = m_projectSetting->tempPath();
    if ( !m_objectPathName.isEmpty() )
        path += "\\" + m_objectPathName;
    return path;
}


void ObjectSetting::mkdirTempObjectPath()
{
    QDir dir( tempObjectPath() );
    dir.mkpath("");
}


void ObjectSetting::mkdirSourceObjectPath()
{
    QDir dir( sourceObjectPath() );
    dir.mkpath("");
}


void ObjectSetting::saveToFile(const QString &contents, const QString &filePath)
{
    // TODO:
    //   move another place

    bool withBOM = false;
    QTextCodec *codecDst = QTextCodec::codecForName("UTF-8");

    QFile fileDst( filePath );
    if ( fileDst.exists() )
        fileDst.remove();

    fileDst.open(QIODevice::WriteOnly);

    QTextStream stDst( &fileDst );
    stDst.setCodec( codecDst );
    stDst.setGenerateByteOrderMark( withBOM );

    stDst << contents;

    fileDst.close();
}



TableDefSetting::TableDefSetting(ProjectSetting *parent)
    : ObjectSetting(parent)
{
    m_objectType          = Model::TableDef;
    m_accessObjectType    = Access::acTable;
    m_objectPathName      = "tabledefs";
    m_containerName       = "";

    m_tempFileExtension   = "tmp";
    m_designFileExtension = "xml";
    m_moduleFileExtension = "";
    m_existCheckExtension = "xml";
}


TableDataSetting::TableDataSetting(ProjectSetting *parent)
    : ObjectSetting(parent)
{
    m_objectType          = Model::TableData;
    m_accessObjectType    = Access::acTable;
    m_objectPathName      = "tabledatas";
    m_containerName       = "";

    m_tempFileExtension   = "tmp";
    m_designFileExtension = "csv";
    m_moduleFileExtension = "";
    m_existCheckExtension = "csv";
}


RelationSetting::RelationSetting(ProjectSetting *parent)
    : ObjectSetting(parent)
{
    m_objectType          = Model::Relation;
    m_accessObjectType    = -1;
    m_objectPathName      = "relations";
    m_containerName       = "";

    m_tempFileExtension   = "tmp";
    m_designFileExtension = "rel";
    m_moduleFileExtension = "";
    m_existCheckExtension = "rel";
}


QuerySetting::QuerySetting(ProjectSetting *parent)
    : ObjectSetting(parent)
{
    m_objectType          = Model::Query;
    m_accessObjectType    = Access::acQuery;
    m_objectPathName      = "queries";
    m_containerName       = "";

    m_tempFileExtension   = "tmp";
    m_designFileExtension = "sql";
    m_moduleFileExtension = "";
    m_existCheckExtension = "sql";
}


FormSetting::FormSetting(ProjectSetting *parent)
    : ObjectSetting(parent)
{
    m_objectType          = Model::Form;
    m_accessObjectType    = Access::acForm;
    m_objectPathName      = "forms";
    m_containerName       = "Forms";

    m_tempFileExtension   = "tmp";
    m_designFileExtension = "frm";
    m_moduleFileExtension = "bas";
    m_existCheckExtension = "frm";
}


ReportSetting::ReportSetting(ProjectSetting *parent)
    : ObjectSetting(parent)
{
    m_objectType          = Model::Report;
    m_accessObjectType    = Access::acReport;
    m_objectPathName      = "reports";
    m_containerName       = "Reports";

    m_tempFileExtension   = "tmp";
    m_designFileExtension = "rpt";
    m_moduleFileExtension = "bas";
    m_existCheckExtension = "rpt";
}


MacroSetting::MacroSetting(ProjectSetting *parent)
    : ObjectSetting(parent)
{
    m_objectType          = Model::Macro;
    m_accessObjectType    = Access::acMacro;
    m_objectPathName      = "macros";
    m_containerName       = "Scripts";

    m_tempFileExtension   = "tmp";
    m_designFileExtension = "mcr";
    m_moduleFileExtension = "";
    m_existCheckExtension = "mcr";
}


ModuleSetting::ModuleSetting(ProjectSetting *parent)
    : ObjectSetting(parent)
{
    m_objectType          = Model::Module;
    m_accessObjectType    = Access::acModule;
    m_objectPathName      = "modules";
    m_containerName       = "Modules";

    m_tempFileExtension   = "tmp";
    m_designFileExtension = "";
    m_moduleFileExtension = "bas";
    m_existCheckExtension = "bas";
}


ReferenceSetting::ReferenceSetting(ProjectSetting *parent)
    : ObjectSetting(parent)
{
    m_objectType          = Model::Reference;
    m_accessObjectType    = -1;
    m_objectPathName      = "";
    m_containerName       = "";

    m_tempFileExtension   = "tmp";
    m_designFileExtension = "csv";
    m_moduleFileExtension = "";
    m_existCheckExtension = "csv";
}
