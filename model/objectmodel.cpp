#include "objectmodel.h"

#include <QIcon>
#include <QDir>
#include <QTextCodec>
#include <QTextStream>

#include <QApplication>
#include <QThread>
#include <QDebug>
#include <QMessageBox>

#include "officelib/officelib.h"
#include "util/comptr.h"

#include "util/getcomobject.h"

#include "projectsetting.h"
#include "objectsetting.h"


ObjectModel::ObjectModel(QObject * parent)
    : QAbstractItemModel(parent)
    , m_application(0)
{
    m_mapItems.insert( Model::TableDef,  QMap< QString, ObjectItem* >() );
    m_mapItems.insert( Model::Query,  QMap< QString, ObjectItem* >() );
    m_mapItems.insert( Model::Form,   QMap< QString, ObjectItem* >() );
    m_mapItems.insert( Model::Report, QMap< QString, ObjectItem* >() );
    m_mapItems.insert( Model::Macro,  QMap< QString, ObjectItem* >() );
    m_mapItems.insert( Model::Module, QMap< QString, ObjectItem* >() );
}

int ObjectModel::columnCount(const QModelIndex &parent) const
{
    // name,objectType
    // inProject,inRepository
    // createDate,updateDate
    if (parent.isValid())
        return 0;
    return ColumnCount;
}

int ObjectModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_items.count();
}

QVariant ObjectModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ( role != Qt::DisplayRole )
        return QVariant();
    if ( orientation == Qt::Horizontal )
    {
        switch (section)
        {
        case NameColumn: return tr("Name");
        case InProjectColumn: return tr("InProject");
        case InFileSystemColumn: return tr("InRepository");
        case CreateDateColumn: return tr("CreateDate");
        case UpdateDateColumn: return tr("UpdateDate");
        case ObjectTypeColumn: return tr("ObjectType");
        }
    }

    return section + 1;
}

Qt::ItemFlags ObjectModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    if (index.isValid())
    {
        flags |= Qt::ItemIsSelectable
                | Qt::ItemIsEnabled
                | Qt::ItemIsUserCheckable
                ;

    }
    return flags;
}

QVariant ObjectModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() ||
            index.row() < 0 || index.row() >= m_items.count() ||
            index.column() < 0 || index.column() >= ColumnCount)
        return QVariant();

    const ObjectItem *item = m_items.at(index.row());

    if (role == Qt::SizeHintRole)
    {
    }
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        switch (index.column()) {
        case NameColumn:          return item->name();
        case InProjectColumn:     return item->inProject();
        case InFileSystemColumn:  return item->inFileSystem();
        case CreateDateColumn:    return item->createDate();
        case UpdateDateColumn:    return item->updateDate();
        case ObjectTypeColumn:    return item->objectType();
            break;
        default:
            break;
        }
    }

    if (role == Qt::TextAlignmentRole)
    {
        switch (index.column())
        {
        case NameColumn:  return static_cast<int>(Qt::AlignVCenter | Qt::AlignLeft);
        default: return static_cast<int>(Qt::AlignVCenter | Qt::AlignCenter);
        }
    }

    if (role == Qt::DecorationRole && index.column() == NameColumn)
    {
        // return QIcon
        QIcon iconTable( ":ui/images/table.png" );
        switch (item->objectType()) {
        case Model::TableDef: return QIcon( ":ui/images/table.png" );
        case Model::Query: return QIcon( ":ui/images/table_multiple.png" );
        case Model::Form: return QIcon( ":ui/images/application_form.png" );
        case Model::Report: return QIcon( ":ui/images/report.png" );
        case Model::Macro: return QIcon( ":ui/images/script.png" );
        case Model::Module: return QIcon( ":ui/images/page.png" );

            break;
        default:
            break;
        }
        return iconTable;
    }

    if (role == Qt::CheckStateRole)
    {
        switch (index.column())
        {
        case NameColumn: return static_cast<int>(item->isSelected() ? Qt::Checked : Qt::Unchecked);
        //case InProject: return static_cast<int>(item->inProject() ? Qt::Checked : Qt::Unchecked);
        //case InRepository: return static_cast<int>(item->inRepository() ? Qt::Checked : Qt::Unchecked);
        }
    }

    return QVariant();
}

QModelIndex ObjectModel::index(int row, int column, const QModelIndex &parent) const
{
    if (row < 0 || column < 0 || column >= ColumnCount || row >= m_items.count() ||
            parent.isValid())
        return QModelIndex();

    ObjectItem *item = m_items.at(row);
    return createIndex(row, column, item);
}

QModelIndex ObjectModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}

void ObjectModel::setApplication(Access::Application *application)
{
    m_application = application;
}


void ObjectModel::loadFromProject()
{

    QList<ObjectItem*> items;

    ComPtr<DAO::Database> currentDb = m_application->CurrentDb();
    ComPtr<Access::CurrentProject> currentProject = m_application->CurrentProject();

    ProjectSetting setting(this);
    ObjectSetting *os;

    setting.initialize(m_application);


    //------------------------------------------------------------------------------------------
    // TableDef
    if ( setting.isMDB() )
    {
        os = setting[ Model::TableDef ];

        ComPtr<DAO::TableDefs> tableDefs = currentDb->TableDefs();
        for ( int i = 0 ; i < tableDefs->Count() ; ++i )
        {
            ComPtr<DAO::TableDef> tableDef = tableDefs->Item(i);
            if ( os->isTargetObject( tableDef.ptr() ) )
            {
                items << ObjectItem::fromTableDef(tableDef.ptr(), this);
            }
        }
    }
    //------------------------------------------------------------------------------------------
    // TableData
    //------------------------------------------------------------------------------------------
    // Relation

    //------------------------------------------------------------------------------------------
    // Query
    if ( setting.isMDB() )
    {
        os = setting[ Model::Query ];

        ComPtr<DAO::QueryDefs> queryDefs = currentDb->QueryDefs();
        for ( int i = 0 ; i < queryDefs->Count() ; ++i )
        {
            ComPtr<DAO::QueryDef> queryDef = queryDefs->Item(i);
            if ( os->isTargetObject( queryDef.ptr() ) )
            {
                items << ObjectItem::fromQueryDef(queryDef.ptr(), this);
            }
        }
    }

    //------------------------------------------------------------------------------------------
    // Form, Report, Macro, Module
    if ( setting.isMDB() )
    {
        QList<Model::ObjectType> objectTypes;
        objectTypes << Model::Form << Model::Report << Model::Macro << Model::Module;

        ComPtr<DAO::Containers> containers = currentDb->Containers();

        foreach ( Model::ObjectType objectType, objectTypes )
        {
            os = setting[ objectType ];

            ComPtr<DAO::Container> container = containers->Item( os->containerName() );
            ComPtr<DAO::Documents> documents = container->Documents();
            for (int i = 0 ; i < documents->Count() ; ++i )
            {
                ComPtr<DAO::Document> document = documents->Item(i);
                if ( os->isTargetObject( document.ptr() ) )
                {
                    items << ObjectItem::fromDAODocument(objectType, document.ptr(), this);
                }
            }
        }
    }
    //------------------------------------------------------------------------------------------
    // Form
    if ( setting.isADP() )
    {
        os = setting[ Model::Form ];
        ComPtr<Access::AllForms> objects = currentProject->AllForms();
        for ( int i = 0 ; i < objects->Count() ; ++i )
        {
            ComPtr<Access::AccessObject> object = objects->Item( i );
            if ( os->isTargetObject( object.ptr() ) )
            {
                items << ObjectItem::fromAccessObject(os->objectType(), object.ptr(), this);
            }
        }
    }
    //------------------------------------------------------------------------------------------
    // Report
    if ( setting.isADP() )
    {
        os = setting[ Model::Report ];
        ComPtr<Access::AllReports> objects = currentProject->AllReports();
        for ( int i = 0 ; i < objects->Count() ; ++i )
        {
            ComPtr<Access::AccessObject> object = objects->Item( i );
            if ( os->isTargetObject( object.ptr() ) )
            {
                items << ObjectItem::fromAccessObject(os->objectType(), object.ptr(), this);
            }
        }
    }
    //------------------------------------------------------------------------------------------
    // Macro
    if ( setting.isADP() )
    {
        os = setting[ Model::Macro ];
        ComPtr<Access::AllMacros> objects = currentProject->AllMacros();
        for ( int i = 0 ; i < objects->Count() ; ++i )
        {
            ComPtr<Access::AccessObject> object = objects->Item( i );
            if ( os->isTargetObject( object.ptr() ) )
            {
                items << ObjectItem::fromAccessObject(os->objectType(), object.ptr(), this);
            }
        }
    }
    //------------------------------------------------------------------------------------------
    // Module
    if ( setting.isADP() )
    {
        os = setting[ Model::Module ];
        ComPtr<Access::AllModules> objects = currentProject->AllModules();
        for ( int i = 0 ; i < objects->Count() ; ++i )
        {
            ComPtr<Access::AccessObject> object = objects->Item( i );
            if ( os->isTargetObject( object.ptr() ) )
            {
                items << ObjectItem::fromAccessObject(os->objectType(), object.ptr(), this);
            }
        }
    }

    //------------------------------------------------------------------------------------------
    // Reference



    //------------------------------------------------------------------------------------------
    // reset items
    int first  = 0;
    int last = std::max<int>( m_items.count() - 1 , items.count() - 1 );
    if ( last < 0)
        last = 0;
    beginInsertRows( QModelIndex(), first, last);
    qDeleteAll(m_items);
    m_items.clear();
    foreach( ObjectItem *item, items)
        addItem( item );
    endInsertRows();
}

void ObjectModel::loadFromFileSystem()
{
    // load items from local file system.

    beginResetModel();

    foreach ( ObjectItem *item, m_items )
    {
        item->setInFileSystem(false);
    }

    ComPtr<Access::CurrentProject> currentProject = m_application->CurrentProject();
    if (!currentProject.is())
        return;

    ProjectSetting setting(this);
    ObjectSetting *os;
    setting.initialize(m_application);


    QDir sourceDir( setting.sourcePath() );

    if (!sourceDir.exists())
        return;

    //------------------------------------------------------------------------------------------
    // TableDef
    //------------------------------------------------------------------------------------------
    // Query
    //------------------------------------------------------------------------------------------
    // Form
    //------------------------------------------------------------------------------------------
    // Report
    //------------------------------------------------------------------------------------------
    // Macro
    //------------------------------------------------------------------------------------------
    // Module
    {
        QList<Model::ObjectType> objectTypes;
        objectTypes << Model::TableDef << Model::Query << Model::Form << Model::Report << Model::Macro << Model::Module;

        foreach ( Model::ObjectType objectType, objectTypes )
        {
            os = setting[ objectType ];

            QDir objectDir( os->sourceObjectPath() );
            if (objectDir.exists())
            {
                objectDir.setNameFilters( (QStringList() << ("*." + os->existCheckExtension() ) ) );
                QFileInfoList fileInfos = objectDir.entryInfoList( QDir::Files );
                foreach ( QFileInfo fileInfo, fileInfos )
                {
                    addItem( ObjectItem::fromFileInfo( os->objectType(), fileInfo, this) );
                }
            }
        }
    }

    //------------------------------------------------------------------------------------------
    // TableData
    //------------------------------------------------------------------------------------------
    // Relation
    //------------------------------------------------------------------------------------------
    // Reference



    endResetModel();

}

/*
QCryptographicHash hash( QCryptographicHash::Sha1 );
QFile file( fileName );

if ( file.open( QIODevice::ReadOnly ) ) {
    hash.addData( file.readAll() );
} else {
    // Handle "cannot open file" error
}

// Retrieve the SHA1 signature of the file
QByteArray sig = hash.result();
*/

void ObjectModel::exportToTempDir()
{
    // export to temp directory
    //      for objects existing in both Project and FileSystem
    //      for objects existing in ProjectOnly
    // without sanitizing and any extra processes.

    // we know the target object types and names.
    QMap< Model::ObjectType, QMap<QString, ObjectItem*> > allTargets;

    // TODO: build up allTargets from m_items


    QTime time;
    QTime timeTotal;

    timeTotal.start();
    time.start();

    ProjectSetting setting(this);
    ObjectSetting *os;

    setting.initialize(m_application);



    ProcessData processData;// = {0};
    SubProcessData subProcessData;// = {0};
    Q_UNUSED(subProcessData);

    emit processStart(processData);


    ComPtr<Access::CurrentProject> currentProject  = m_application->CurrentProject();
    ComPtr<DAO::Database> currentDb = m_application->CurrentDb();
    if ( !currentProject.is() )
    {
        QMessageBox::information(0, QString(""), QString("currentProject is null"));
        return;
    }
    if ( setting.isMDB() && !currentDb.is() )
    {
        QMessageBox::information(0, QString(""), QString("projectType is null"));
        return;
    }


    //------------------------------------------------------------------------------------------
    // clear temp path
    QDir tempSourceDir( setting.tempPath() );
    tempSourceDir.rmdir("");
    tempSourceDir.mkpath("");



    //------------------------------------------------------------------------------------------
    // TableDef
    if ( setting.isMDB() )
    {
        time.start();

        os = setting[ Model::TableDef ];
        os->mkdirTempObjectPath();

        QMap<QString, ObjectItem*> targets = allTargets[ os->objectType() ];
        QStringList objectNames = targets.keys();

        int nCount = objectNames.count(); int nPos = 0;
        emit subProcessStart(processData, { nCount, 0 } );

        ComPtr<DAO::TableDefs> tableDefs = currentDb->TableDefs();
        foreach ( QString objectName, objectNames )
        {
            emit subProcessProgess(processData,  { nCount, ++nPos } );
            QApplication::processEvents();

            //------------------------------------------------------------------------------------------
            // Export Local Table
            ComPtr<DAO::TableDef> tableDef = tableDefs->Item( objectName );
            // this makes force termination...
            //m_application->ExportXML(
            //            Access::acExportTable
            //            ,objectName
            //            ,QString() // DataTarget
            //            ,os->tempFile(objectName) // SchemaTarget
            //            ,QString() //PresentationTarget
            //            ,QString() //ImageTarget
            //            ,Access::acUTF16 //Encoding
            //            ,Access::acExportAllTableAndFieldProperties //OtherFlags
            //            ,QString() //WhereCondition
            //            ,QVariant()//AdditionalData
            //            );
        }
        emit subProcessEnd(processData, { nCount, nCount } );
        qDebug() << "TableDefs : " << nCount << " : " << time.elapsed();
    }
    //------------------------------------------------------------------------------------------
    // TableData
    if ( setting.isMDB() )
    {
        // TODO:
        // determine target table names
        // import
            // build select sql with order by
            // write data as tab-delimited csv with UCS2
        // export
            // read one by one..

    }
    //------------------------------------------------------------------------------------------
    // Relation
    if ( setting.isMDB() )
    {
        // TODO:
    }


    //------------------------------------------------------------------------------------------
    // Query
    if ( setting.isMDB() )
    {
        time.start();

        os = setting[ Model::Query ];
        os->mkdirTempObjectPath();

        QMap<QString, ObjectItem*> targets = allTargets[ os->objectType() ];
        QStringList objectNames = targets.keys();

        int nCount = objectNames.count(); int nPos = 0;
        emit subProcessStart(processData, { nCount, 0 } );

        ComPtr<DAO::QueryDefs> queryDefs = currentDb->QueryDefs();
        foreach ( QString objectName, objectNames )
        {
            emit subProcessProgess(processData,  { nCount, ++nPos } );
            QApplication::processEvents();

            //------------------------------------------------------------------------------------------
            // Export Query as SQL
            ComPtr<DAO::QueryDef> queryDef = queryDefs->Item( objectName );
            QString sql = queryDef->SQL();
            os->saveToFile( sql, os->designFileInTempPath(objectName) );
        }
        emit subProcessEnd(processData, { nCount, nCount } );
        qDebug() << "Queries : " << nCount << " : " << time.elapsed();
    }


    //------------------------------------------------------------------------------------------
    // Form, Report, Macro, Module
    if ( setting.isMDB() || setting.isADP() )
    {
        QList<Model::ObjectType> objectTypes;
        objectTypes << Model::Form << Model::Report << Model::Macro << Model::Module;

        foreach ( Model::ObjectType objectType, objectTypes )
        {
            time.start();

            os = setting[ objectType ];
            os->mkdirTempObjectPath();

            QMap<QString, ObjectItem*> targets = allTargets[ os->objectType() ];
            QStringList objectNames = targets.keys();

            int nCount = objectNames.count(); int nPos = 0;
            emit subProcessStart(processData, { nCount, 0 } );

            foreach ( QString objectName, objectNames )
            {
                emit subProcessProgess(processData,  { nCount, ++nPos } );
                QApplication::processEvents();

                //------------------------------------------------------------------------------------------
                // Export Object as Text
                m_application->SaveAsText( (Access::AcObjectType)os->accessObjectType(), objectName, os->tempFileInTempPath(objectName) );
            }
            emit subProcessEnd(processData, { nCount, nCount } );
            qDebug() << os->objectPathName() << " : " << nCount << " : " << time.elapsed();
        }
    }

    //------------------------------------------------------------------------------------------
    // Reference
    {
        time.start();

        os = setting[ Model::Reference ];
        os->mkdirTempObjectPath();

        ComPtr<Access::References> references = m_application->References();

        int nCount = references->Count(); int nPos = 0;
        emit subProcessStart(processData, { nCount, 0 } );

        //------------------------------------------------------------------------------------------
        // Export References as Text
        QString contents = "";
        for ( int i = 1 ; i <= nCount ; ++i )
        {
            emit subProcessProgess(processData,  { nCount, ++nPos } );
            QApplication::processEvents();

            ComPtr<Access::Reference> reference = references->Item( i );
            bool refBuiltIn = reference->BuiltIn();
            //if ( !refBuiltIn )
            {
                QString refName = reference->Name();
                QString refGuid = reference->Guid();

                if ( refGuid.length() > 0 )
                {
                    QString refMajor = QString().setNum( reference->Major() );
                    QString refMinor = QString().setNum( reference->Minor() );
                    contents += refName;
                    contents += "," + (refBuiltIn ? QString("1") : QString("0"));
                    contents += "," + refGuid;
                    contents += "," + refMajor;
                    contents += "," + refMinor;
                    contents += "\r\n";
                }
                else
                {
                    // references of types mdb,accdb,mde etc don't have a GUID
                    QString fullPath = reference->FullPath();
                    contents += refName;
                    contents += "," + (refBuiltIn ? QString("1") : QString("0"));
                    contents += "," + fullPath;
                    contents += "\r\n";
                }
            }
        }
        // write contents to file
        QString objectName = "references";
        os->saveToFile(contents, os->designFileInTempPath(objectName));

        emit subProcessEnd(processData, { nCount, nCount } );
        qDebug() << "References : " << nCount << " : " << time.elapsed();
    }

    emit processEnd(processData);
    qDebug() << "DONE : " << timeTotal.elapsed() ;
}


void ObjectModel::addItem(ObjectItem *item)
{
    if ( m_mapItems[ item->objectType() ].contains( item->name() ) )
    {
        // already exists.
        // so we must merge.
        ObjectItem *itemInList = m_mapItems[ item->objectType() ].value( item->name() );
        itemInList->setInProject( itemInList->inProject() || item->inProject() );
        itemInList->setInFileSystem(itemInList->inFileSystem() || item->inFileSystem() );
        itemInList->setCreateDate(   item      ->createDate().isValid() ? item      ->createDate()
                                 : ( itemInList->createDate().isValid() ? itemInList->createDate()
                                 : QDateTime() ) );
        itemInList->setUpdateDate(   item      ->updateDate().isValid() ? item      ->updateDate()
                                 : ( itemInList->updateDate().isValid() ? itemInList->updateDate()
                                 : QDateTime() ) );
        // and delete 'item' object.
        delete item;
    }
    else
    {
        m_items << item;
        m_mapItems[ item->objectType() ].insert( item->name(), item );
    }
}

