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
            case DifferentColumn: return tr("Different");
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
            case InProjectColumn:     return (int)item->inProject();
            case InFileSystemColumn:  return (int)item->inFileSystem();
            case CreateDateColumn:    return item->createDate();
            case UpdateDateColumn:    return item->updateDate();
            case ObjectTypeColumn:    return item->objectType();
            case DifferentColumn:    return item->isDifferent();
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

        // TODO: make icon manager
        switch (item->objectType()) {
            case Model::TableDef:   return QIcon( ":ui/images/table.png" );
            case Model::Query:      return QIcon( ":ui/images/table_multiple.png" );
            case Model::Form:       return QIcon( ":ui/images/application_form.png" );
            case Model::Report:     return QIcon( ":ui/images/report.png" );
            case Model::Macro:      return QIcon( ":ui/images/script.png" );
            case Model::Module:     return QIcon( ":ui/images/page.png" );

        default:
            break;
        }
        return QVariant();
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

void ObjectModel::prepareInit()
{
    //-------------------------------------------------------------------------------------------
    // `git init <directory>`

    //-------------------------------------------------------------------------------------------
    // ... prepareCommit()
    // ... executeCommit()

    //-------------------------------------------------------------------------------------------
    // ... preparePush()
    // ... executePush()
}

void ObjectModel::prepareClone()
{
    //-------------------------------------------------------------------------------------------
    // `git clone <repo-url> <directory>`

    //-------------------------------------------------------------------------------------------
    // ... openProject()    : done by user
    // ... createProject()  : done by user

    //-------------------------------------------------------------------------------------------
    // ... prepareMerge()
    // ... executeMerge()
}

void ObjectModel::prepareCommit()
{
    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //:prepare to determine target objects to reflect

    // access save & compile
    // `git checkout <branch>`              // make sure we are in proper branch


    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //:determine target objects to reflect from Project to FileSytem
    prepareExport();

    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //:confirm to reflect from Project to FileSystem


    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //:execute to reflect from Project to FileSystem
    executeExport();


    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //: and now, we are in git problem
    //

    //-------------------------------------------------------------------------------------------
    // `git status --porcelain`             // ... and show status of objects
    //      MADRCU?!

    //-------------------------------------------------------------------------------------------
    // `git diff`                           // ... and show diffs in external gui

    //-------------------------------------------------------------------------------------------
    //                                      // ... and select target objects ( extensions )
    // for unstaged files :
    //      `git add <file>`         : will be commit
    //      `git checkout -- <file>` : discard change in working directory
    // for staged files :
    //      `git reset HEAD <file>`  : unstage

    //-------------------------------------------------------------------------------------------
    //                                      // ... and show commit dialog
    // `git commit -m <commit message> ...` // ... and enter commit message and `git commit`


    //                                      // ... and if user changed from default selection, Merge will be required.
}

void ObjectModel::prepareMerge()
{
    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //:prepare to determine target objects to be imported

    // `git checkout <branch>`              // make sure we are in proper branch
    // `git pull`                           // pull recent updates from remote


    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //:determine target objects to be imported
    prepareImport();

    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //:confirm

    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //:execute
    executeImport();

    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //:and now we are in access problem
}




















bool ObjectModel::prepareExport()
{
    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //:determine target objects to reflect from Project to FileSytem

    // refresh model items
    loadFromProject();                          //                  : BLOCK :                   :
    loadFromFileSystem();                       //                  :       :                   :

    clearTempDir();

    // for InProjectOnly
    {
        // .
    }

    // for InFileSytemOnly
    {
        // .
    }

    // for InBoth
    {
        ObjectItems targets;
        getItems(&targets, InBoth);

        exportFromProjectToTempDir(&targets);   // InBoth           : BLOCK :                   :
        sanitizeTempDir(&targets);              // InBoth           :       :                   :
        compareTempDir(&targets);               // InBoth           :       :                   :
        //.
    }

    return true;
}

bool ObjectModel::executeExport()
{
    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //:execute to reflect from Project to FileSystem

    // TODO: targets must be selected only

    // for InProjectOnly
    {
        ObjectItems targets;
        getItems(&targets, InProjectOnly);
        exportFromProjectToTempDir(&targets);       // InProjectOnly    : BLOCK :                   :
        sanitizeTempDir(&targets);                  // InProjectOnly    :       :                   :
        copyFromTempDirToFileSystem(&targets);      // InProjectOnly    :       : Dirty FileSystem  : need one-more step? like confirm
    }

    // for InFileSytemOnly
    {
        ObjectItems targets;
        getItems(&targets, InFileSystemOnly);
        deleteFromFileSystem(&targets);             // InFileSystemOnly :       : Dirty FileSystem  : need one-more step? like confirm
    }

    // for InBoth
    {
        // for InBoth_Different
        {
            ObjectItems targets;
            getItems(&targets, InBoth_Different);
            copyFromTempDirToFileSystem(&targets);  // InBoth_Different :       : Dirty FileSystem  : need one-more step? like confirm
        }
    }

    return true;
}

bool ObjectModel::prepareImport()
{
    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //:determine target objects to be imported

    // refresh model items
    loadFromProject();                          //                  : BLOCK :               :
    loadFromFileSystem();                       //                  :       :               :

    clearTempDir();

    // for InProjectOnly
    {
        //.
    }

    // for InFileSytemOnly
    {
        //.
    }

    // for InBoth
    {
        ObjectItems targets;
        getItems(&targets, InBoth);

        exportFromProjectToTempDir(&targets);       // InBoth           : BLOCK :               :
        sanitizeTempDir(&targets);                  // InBoth           :       :               :
        compareTempDir(&targets);                   // InBoth           :       :               :

        // for InBoth_Different
        {
            //.
        }
    }

    return true;
}

bool ObjectModel::executeImport()
{
    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //:execute

    // TODO: targets must be selected only

    // for InProjectOnly
    {
        ObjectItems targets;
        getItems(&targets, InProjectOnly);
        deleteFromProject(&targets);                // InProjectOnly    : BLOCK : Dirty Project : need one-more step? like confirm
    }

    // for InFileSytemOnly
    {
        ObjectItems targets;
        getItems(&targets, InFileSystemOnly);
        copyFromFileSystemToTempDir(&targets);      // InFileSytemOnly  :       :               :
        desanitizeTempDir(&targets);                // InFileSytemOnly  :       :               :
        importFromTempDirToProject(&targets);       // InFileSystemOnly : BLOCK : Dirty Project : need one-more step? like confirm
    }

    // for InBoth
    {
        // for InBoth_Different
        {
            ObjectItems targets;
            getItems(&targets, InBoth_Different);
            copyFromFileSystemToTempDir(&targets);  // InBoth_Different :       :               :
            desanitizeTempDir(&targets);            // InBoth_Different :       :               :
            importFromTempDirToProject(&targets);   // InBoth_Different : BLOCK : Dirty Project : need one-more step? like confirm
        }
    }

    return true;
}

























void ObjectModel::getItems(ObjectItems *pItems, ObjectModel::ItemsType itemsType) const
{
    foreach ( ObjectItem *item, m_items )
    {
        ObjectItem *toBeInserted = NULL;
        switch (itemsType)
        {
            case AllItems:
            {
                toBeInserted = item;
                break;
            }
            case InBoth:
            {
                if ( item->inProject()    == Model::Present &&
                     item->inFileSystem() == Model::Present )
                    toBeInserted = item;
                break;
            }
            case InBoth_Different:
            {
                if ( item->inProject()    == Model::Present &&
                     item->inFileSystem() == Model::Present &&
                     item->isDifferent()  == Model::DifferentContents )
                    toBeInserted = item;
                break;
            }
            case InProjectOnly:
            {
                if ( item->inProject()    == Model::Present &&
                     item->inFileSystem() == Model::Absent )
                    toBeInserted = item;
                break;
            }
            case InFileSystemOnly:
            {
                if ( item->inProject()    == Model::Absent &&
                     item->inFileSystem() == Model::Present )
                    toBeInserted = item;
                break;
            }
        }

        if (toBeInserted)
        {
            (*pItems)[ toBeInserted->objectType() ][ toBeInserted->name() ] = toBeInserted;
        }
    }


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
                items << os->createItemFromProject(tableDef.ptr(), this);
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
                items << os->createItemFromProject(queryDef.ptr(), this);
            }
        }
    }

    //------------------------------------------------------------------------------------------
    // Form, Report, Macro, Module
    if ( setting.isMDB() )
    {
        QList<Model::ObjectType> objectTypes;
        objectTypes << Model::Form
                    << Model::Report
                    << Model::Macro
                    << Model::Module
                       ;

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
                    items << os->createItemFromProject(document.ptr(), this);
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
                items << os->createItemFromProject(object.ptr(), this);
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
                items << os->createItemFromProject(object.ptr(), this);
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
                items << os->createItemFromProject(object.ptr(), this);
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
                items << os->createItemFromProject(object.ptr(), this);
            }
        }
    }

    //------------------------------------------------------------------------------------------
    // Reference
    {
        os = setting[ Model::Reference ];
        items << os->createItemFromProject(NULL, this);
    }



    //------------------------------------------------------------------------------------------
    // reset items
    beginRemoveRows( QModelIndex(), 0, m_items.count() - 1 );
    {
        qDeleteAll(m_items);
        m_items.clear();

        foreach (Model::ObjectType t, m_mapItems.keys() )
            m_mapItems[t].clear();
    }
    endRemoveRows();

    beginInsertRows( QModelIndex(), 0, items.count() - 1);
    {
        foreach( ObjectItem *item, items)
            addItem( item );
    }
    endInsertRows();
}

void ObjectModel::loadFromFileSystem()
{
    // load items from local file system.


    beginResetModel();
    {
        foreach ( ObjectItem *item, m_items )
            item->setInFileSystem( Model::Absent );
    }
    endResetModel();

    ComPtr<Access::CurrentProject> currentProject = m_application->CurrentProject();
    if (!currentProject.is())
        return;

    ProjectSetting setting(this);
    ObjectSetting *os;
    setting.initialize(m_application);

    QDir sourceDir( setting.sourcePath() );
    if (!sourceDir.exists())
        return;

    QList<ObjectItem *> items;

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
    //------------------------------------------------------------------------------------------
    // Reference
    {
        QList<Model::ObjectType> objectTypes;
        objectTypes << Model::TableDef
                    << Model::Query
                    << Model::Form
                    << Model::Report
                    << Model::Macro
                    << Model::Module
                    << Model::Reference
                       ;

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
                    items << os->createItemFromFileSystem(fileInfo, this);
                }
            }
        }
    }

    //------------------------------------------------------------------------------------------
    // TableData
    //------------------------------------------------------------------------------------------
    // Relation



    // update / insert items
    beginResetModel();
    {
        foreach ( ObjectItem *item, items )
            addItem( item );
    }
    endResetModel();
}


void ObjectModel::exportFromProjectToTempDir(ObjectItems *allTargets)
{
    // export to temp directory
    //      for objects existing in both Project and FileSystem
    //      for objects existing in ProjectOnly
    // without sanitizing and any extra processes.

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
    // TableDef
    if ( setting.isMDB() )
    {
        time.start();

        os = setting[ Model::TableDef ];
        os->mkdirTempObjectPath();

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
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
            os->exportFromProjectToTempDir(tableDef.ptr(), objectName);

        }
        emit subProcessEnd(processData, { nCount, nCount } );
        qDebug() << "TableDefs : " << nCount << " : " << time.elapsed();
    }

    //------------------------------------------------------------------------------------------
    // Query
    if ( setting.isMDB() )
    {
        time.start();

        os = setting[ Model::Query ];
        os->mkdirTempObjectPath();

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();

        int nCount = objectNames.count(); int nPos = 0;
        emit subProcessStart(processData, { nCount, 0 } );

        // TODO: universal iteration
        ComPtr<DAO::QueryDefs> queryDefs = currentDb->QueryDefs();

        foreach ( QString objectName, objectNames )
        {
            emit subProcessProgess(processData,  { nCount, ++nPos } );
            QApplication::processEvents();

            //------------------------------------------------------------------------------------------
            // Export Query as SQL
            ComPtr<DAO::QueryDef> queryDef = queryDefs->Item( objectName );
            os->exportFromProjectToTempDir(queryDef.ptr(), objectName);
        }
        emit subProcessEnd(processData, { nCount, nCount } );
        qDebug() << "Queries : " << nCount << " : " << time.elapsed();
    }


    //------------------------------------------------------------------------------------------
    // Form, Report, Macro, Module
    //------------------------------------------------------------------------------------------
    // Reference
    if ( setting.isMDB() || setting.isADP() )
    {
        QList<Model::ObjectType> objectTypes;
        objectTypes << Model::Form
                    << Model::Report
                    << Model::Macro
                    << Model::Module
                    << Model::Reference
                       ;

        foreach ( Model::ObjectType objectType, objectTypes )
        {
            time.start();

            os = setting[ objectType ];
            os->mkdirTempObjectPath();

            QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
            QStringList objectNames = targets.keys();

            int nCount = objectNames.count(); int nPos = 0;
            emit subProcessStart(processData, { nCount, 0 } );

            foreach ( QString objectName, objectNames )
            {
                emit subProcessProgess(processData,  { nCount, ++nPos } );
                QApplication::processEvents();

                //------------------------------------------------------------------------------------------
                // Export Object as Text
                os->exportFromProjectToTempDir(NULL, objectName);
            }
            emit subProcessEnd(processData, { nCount, nCount } );
            qDebug() << os->objectPathName() << " : " << nCount << " : " << time.elapsed();
        }
    }

    //------------------------------------------------------------------------------------------
    // TableData
    if ( setting.isMDB() )
    {
        // TODO: implement tabledata export

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
        // TODO: implement relation export
    }


    emit processEnd(processData);
    qDebug() << "DONE : " << timeTotal.elapsed() ;
}

void ObjectModel::importFromTempDirToProject(ObjectItems *allTargets)
{
    QTime time;
    QTime timeTotal;
    timeTotal.start();
    time.start();

    ProjectSetting setting(this);
    ObjectSetting *os;

    setting.initialize(m_application);


    // TableDef
    // TableData
    // Relation
    {
        // TODO: implement import object
    }
    // Query
    if (setting.isMDB())
    {
        os = setting[ Model::Query ];
        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();

        int nCount = objectNames.count(); int nPos = 0;
        Q_UNUSED(nCount)
        Q_UNUSED(nPos)

        ComPtr<DAO::Database> currentDb = m_application->CurrentDb();
        ComPtr<DAO::QueryDefs> queryDefs = currentDb->QueryDefs();

        foreach (QString objectName, objectNames)
        {
            ObjectItem *item = targets[ objectName ];

            if (item->inProject() == Model::Present)
            {
                ComPtr<DAO::QueryDef> queryDef = queryDefs->Item( objectName );
                os->importFromTempDirToProject(queryDef.ptr(), objectName);
            }
            else
            {
                os->importFromTempDirToProject(NULL, objectName);
            }
        }
    }

    // Form. Report, Macro, Module, Rerefence
    {
        QList<Model::ObjectType> objectTypes;
        objectTypes << Model::Form
                    << Model::Report
                    << Model::Macro
                    << Model::Module
                    << Model::Reference
                    ;

        foreach (Model::ObjectType objectType, objectTypes)
        {
            os = setting[ objectType ];

            QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
            QStringList objectNames = targets.keys();

            int nCount = objectNames.count(); int nPos = 0;
            Q_UNUSED(nCount)
            Q_UNUSED(nPos)

            foreach (QString objectName, objectNames)
            {
                os->importFromTempDirToProject(NULL, objectName);
            }
        }
    }
}

void ObjectModel::copyFromTempDirToFileSystem(ObjectItems *allTargets)
{
    QTime time;
    QTime timeTotal;
    timeTotal.start();
    time.start();

    ProjectSetting setting(this);
    ObjectSetting *os;

    setting.initialize(m_application);

    QList<Model::ObjectType> objectTypes;
    objectTypes << Model::TableDef
                << Model::Query
                << Model::Form
                << Model::Report
                << Model::Macro
                << Model::Module
                << Model::Reference
                ;

    foreach (Model::ObjectType objectType, objectTypes)
    {
        os = setting[ objectType ];
        os->mkdirSourceObjectPath();

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();

        int nCount = objectNames.count(); int nPos = 0;
        Q_UNUSED(nCount)
        Q_UNUSED(nPos)

        foreach (QString objectName, objectNames)
        {
            os->copyFromTempDirToFileSystem(objectName);
        }
    }
}

void ObjectModel::copyFromFileSystemToTempDir(ObjectItems *allTargets)
{
    QTime time;
    QTime timeTotal;
    timeTotal.start();
    time.start();

    ProjectSetting setting(this);
    ObjectSetting *os;

    setting.initialize(m_application);

    QList<Model::ObjectType> objectTypes;
    objectTypes << Model::TableDef
                << Model::Query
                << Model::Form
                << Model::Report
                << Model::Macro
                << Model::Module
                << Model::Reference
                ;

    foreach (Model::ObjectType objectType, objectTypes)
    {
        os = setting[ objectType ];

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();

        int nCount = objectNames.count(); int nPos = 0;
        Q_UNUSED(nCount)
        Q_UNUSED(nPos)

        foreach (QString objectName, objectNames)
        {
            os->copyFromFileSystemToTempDir(objectName);
        }
    }

}

void ObjectModel::sanitizeTempDir(ObjectItems *allTargets)
{
    QTime time;
    QTime timeTotal;
    timeTotal.start();
    time.start();

    ProjectSetting setting(this);
    ObjectSetting *os;

    setting.initialize(m_application);

    QList<Model::ObjectType> objectTypes;
    objectTypes
                << Model::TableDef
                << Model::Query
                << Model::Form
                << Model::Report
                << Model::Macro
                << Model::Module
                << Model::Reference
                ;

    foreach (Model::ObjectType objectType, objectTypes)
    {
        os = setting[ objectType ];

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();

        int nCount = objectNames.count(); int nPos = 0;
        Q_UNUSED(nCount)
        Q_UNUSED(nPos)

        foreach (QString objectName, objectNames)
        {
            os->sanitizeTempDir(NULL, objectName);
        }
    }
}

void ObjectModel::desanitizeTempDir(ObjectItems *allTargets)
{
    QTime time;
    QTime timeTotal;
    timeTotal.start();
    time.start();

    ProjectSetting setting(this);
    ObjectSetting *os;

    setting.initialize(m_application);

    QList<Model::ObjectType> objectTypes;
    objectTypes << Model::TableDef
                << Model::Query
                << Model::Form
                << Model::Report
                << Model::Macro
                << Model::Module
                << Model::Reference
                ;

    foreach (Model::ObjectType objectType, objectTypes)
    {
        os = setting[ objectType ];

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();

        int nCount = objectNames.count(); int nPos = 0;
        Q_UNUSED(nCount)
        Q_UNUSED(nPos)

        foreach (QString objectName, objectNames)
        {
            os->desanitizeTempDir(NULL, objectName);
        }
    }

}

void ObjectModel::compareTempDir(ObjectItems *allTargets)
{
    QTime time;
    QTime timeTotal;
    timeTotal.start();
    time.start();

    ProjectSetting setting(this);
    ObjectSetting *os;

    setting.initialize(m_application);

    QList<Model::ObjectType> objectTypes;
    objectTypes << Model::TableDef
                << Model::Query
                << Model::Form
                << Model::Report
                << Model::Macro
                << Model::Module
                << Model::Reference
                ;

    foreach (Model::ObjectType objectType, objectTypes)
    {
        os = setting[ objectType ];

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();

        int nCount = objectNames.count(); int nPos = 0;
        Q_UNUSED(nCount)
        Q_UNUSED(nPos)

        foreach (QString objectName, objectNames)
        {
            bool isDifferent = false;
            os->compareTempDir(objectName, &isDifferent);

            ObjectItem *item = targets[ objectName ];
            if (isDifferent == true && item->isDifferent() != Model::DifferentContents )
                item->setDifferent( Model::DifferentContents );
            else if (isDifferent == false && item->isDifferent() != Model::SameContents )
                item->setDifferent( Model::SameContents );

        }
    }


    // update items
    emit dataChanged( createIndex(0, DifferentColumn), createIndex( m_items.count()-1, DifferentColumn ) );

}

void ObjectModel::deleteFromFileSystem(ObjectItems *allTargets)
{
    QTime time;
    QTime timeTotal;
    timeTotal.start();
    time.start();

    ProjectSetting setting(this);
    ObjectSetting *os;

    setting.initialize(m_application);

    QList<Model::ObjectType> objectTypes;
    objectTypes << Model::TableDef
                << Model::Query
                << Model::Form
                << Model::Report
                << Model::Macro
                << Model::Module
                << Model::Reference
                ;

    foreach (Model::ObjectType objectType, objectTypes)
    {
        os = setting[ objectType ];

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();

        int nCount = objectNames.count(); int nPos = 0;
        Q_UNUSED(nCount)
        Q_UNUSED(nPos)

        foreach (QString objectName, objectNames)
        {
            os->deleteFromFileSystem(objectName);
        }
    }
}

void ObjectModel::deleteFromProject(ObjectItems *allTargets)
{
    QTime time;
    QTime timeTotal;
    timeTotal.start();
    time.start();

    ProjectSetting setting(this);
    ObjectSetting *os;

    setting.initialize(m_application);

    QList<Model::ObjectType> objectTypes;
    objectTypes << Model::TableDef
                << Model::Query
                << Model::Form
                << Model::Report
                << Model::Macro
                << Model::Module
                ;

    foreach (Model::ObjectType objectType, objectTypes)
    {
        os = setting[ objectType ];

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();

        int nCount = objectNames.count(); int nPos = 0;
        Q_UNUSED(nCount)
        Q_UNUSED(nPos)

        foreach (QString objectName, objectNames)
        {
            os->deleteFromProject(objectName);
        }
    }
}


















void ObjectModel::addItem(ObjectItem *item)
{

    if ( m_mapItems[ item->objectType() ].contains( item->name() ) )
    {
        // already exists.
        // so we must merge.
        ObjectItem *itemInList = m_mapItems[ item->objectType() ].value( item->name() );

        //TODO: object existent merge logic must be function.

        itemInList->setInProject(
                    (itemInList->inProject() == Model::Present || item->inProject() == Model::Present) ? Model::Present :
                    (itemInList->inProject() == Model::Absent  || item->inProject() == Model::Absent ) ? Model::Absent  :
                                                                                                         Model::OE_Unchecked );
        itemInList->setInFileSystem(
                    (itemInList->inFileSystem() == Model::Present || item->inFileSystem() == Model::Present) ? Model::Present :
                    (itemInList->inFileSystem() == Model::Absent  || item->inFileSystem() == Model::Absent ) ? Model::Absent  :
                                                                                                               Model::OE_Unchecked );

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

void ObjectModel::clearTempDir()
{
    ProjectSetting setting(this);
    setting.initialize(m_application);

    QDir tempDir( setting.tempPath() );
    if (tempDir.exists())
    {
        tempDir.removeRecursively();
        tempDir.mkpath(".");
    }
}

