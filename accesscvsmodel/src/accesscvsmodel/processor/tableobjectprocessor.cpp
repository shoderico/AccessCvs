#include "tableobjectprocessor.h"

#include <QDebug>
#include <QTextCodec>

#include "accesslib/accesslib.h"

#include "util/fileutil.h"
#include "util/codecinfo.h"

#include "cvsmodel/objectitem.h"
#include "accesscvsmodel/accessprojectcontainer.h"

#include "pch.hpp"

TableObjectProcessor::TableObjectProcessor(ProjectContainer *parent)
    : ObjectProcessor(parent)
{

}

bool TableObjectProcessor::isTargetObject(QAxObject *object) const
{
    DAO::TableDef *tableDef = dynamic_cast<DAO::TableDef*>(object);
    if (tableDef)
    {
        QString name = tableDef->Name();
        return !name.startsWith("MSys")
                && !name.startsWith("~")
                ;
    }
    return false;
}

ObjectItem *TableObjectProcessor::createItemFromProject(QAxObject *object, QObject *parent)
{
    ObjectItem *item = new ObjectItem(parent);

    DAO::TableDef *tableDef = dynamic_cast<DAO::TableDef*>(object);
    if (tableDef)
    {
        item->setObjectType( m_objectType );
        item->setSelectObjectType( m_selectObjectType );
        item->setIconPath( m_iconPath );
        item->setName( tableDef->Name() );
        item->setInProject( Model::Present );
        item->setCreateDate( tableDef->DateCreated().toDateTime() );
        item->setUpdateDate( tableDef->LastUpdated().toDateTime() );
        item->setExportDate( FileUtil::fileTime( filePath(TempDir, TempFile, item->name()) ) );
    }

    return item;
}

bool TableObjectProcessor::prepareItemCollection()
{
    if (!projectContainer<AccessProjectContainer>()->isMDB())
        return false;

    ComPtr<DAO::Database> currentDb = m_projectContainer->application<Access::Application>()->CurrentDb();
    if (!currentDb.is())
        return false;

    m_tableDefs.set( currentDb->TableDefs() );
    return m_tableDefs.is();
}

int TableObjectProcessor::itemCount()
{
    if (!m_tableDefs.is())
        return 0;
    return m_tableDefs->Count();
}

QAxObject *TableObjectProcessor::itemUnsafePtr(const QVariant &index)
{
    if (!m_tableDefs.is())
        return 0;
    return m_tableDefs->Item(index);
}

void TableObjectProcessor::determineCodecForProject()
{
    if (!m_codecForProject)
    {
        m_codecForProject = new CodecInfo(this);
        m_codecForProject->setCodec( QTextCodec::codecForName("UTF-8") );
        m_codecForProject->setBom( false );
        m_codecForProject->setLineEnd("\r\n");
    }
}

