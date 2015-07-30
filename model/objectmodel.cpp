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


ObjectModel::ObjectModel(QObject * parent)
    : QAbstractItemModel(parent)
    , m_application(0)
{
    m_mapItems.insert( Model::Table,  QMap< QString, ObjectItem* >() );
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
        case Model::Table: return QIcon( ":ui/images/table.png" );
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
    int projectType = -1;

    if ( currentProject.is() )
        projectType = currentProject->ProjectType();

    //------------------------------------------------------------------------------------------
    // TableDef
    if ( projectType == Access::acMDB )
    {
        ComPtr<DAO::TableDefs> tableDefs = currentDb->TableDefs();
        for (int i = 0 ; i < tableDefs->Count() ; ++i )
        {
            ComPtr<DAO::TableDef> tableDef = tableDefs->Item(i);
            if ( !tableDef->Name().startsWith("MSys") && tableDef->Connect().isEmpty() )
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
    if ( projectType == Access::acMDB )
    {
        ComPtr<DAO::QueryDefs> queryDefs = currentDb->QueryDefs();
        for (int i = 0 ; i < queryDefs->Count() ; ++i )
        {
            ComPtr<DAO::QueryDef> queryDef = queryDefs->Item(i);
            if ( !queryDef->Name().startsWith("~") && queryDef->Connect().isEmpty() )
            {
                items << ObjectItem::fromQueryDef(queryDef.ptr(), this);
            }
        }
    }

    //------------------------------------------------------------------------------------------
    // Form, Report, Macro, Module
    if ( projectType == Access::acMDB )
    {

        ComPtr<DAO::Containers> containers = currentDb->Containers();

        QStringList containerNames;
        containerNames << "Forms" << "Reports" << "Scripts" << "Modules";
        foreach ( QString containerName, containerNames )
        {
            ComPtr<DAO::Container> container = containers->Item( containerName );
            ComPtr<DAO::Documents> documents = container->Documents();
            for (int i = 0 ; i < documents->Count() ; ++i )
            {
                ComPtr<DAO::Document> document = documents->Item(i);
                if ( !document->Name().startsWith("~") )
                {
                    Model::ObjectType objectType = Model::Unknwon;
                    if ( containerName == "Forms" )
                    {
                        objectType = Model::Form;
                    }
                    else if ( containerName == "Reports" )
                    {
                        objectType = Model::Report;
                    }
                    else if ( containerName == "Scripts" )
                    {
                        objectType = Model::Macro;
                    }
                    else if ( containerName == "Modules" )
                    {
                        objectType = Model::Module;
                    }
                    items << ObjectItem::fromDAODocument(objectType, document.ptr(), this);
                }
            }
        }
    }
    //------------------------------------------------------------------------------------------
    // Form
    if ( projectType == Access::acADP )
    {
        ComPtr<Access::AllForms> objects = currentProject->AllForms();
        for ( int i = 0 ; i < objects->Count() ; ++i )
        {
            ComPtr<Access::AccessObject> object = objects->Item( i );
            if ( !object->Name().startsWith("~") )
            {
                items << ObjectItem::fromAccessObject(Model::Form, object.ptr(), this);
            }
        }
    }
    //------------------------------------------------------------------------------------------
    // Report
    if ( projectType == Access::acADP )
    {
        ComPtr<Access::AllReports> objects = currentProject->AllReports();
        for ( int i = 0 ; i < objects->Count() ; ++i )
        {
            ComPtr<Access::AccessObject> object = objects->Item( i );
            if ( !object->Name().startsWith("~") )
            {
                items << ObjectItem::fromAccessObject(Model::Report, object.ptr(), this);
            }
        }
    }
    //------------------------------------------------------------------------------------------
    // Macro
    if ( projectType == Access::acADP )
    {
        ComPtr<Access::AllMacros> objects = currentProject->AllMacros();
        for ( int i = 0 ; i < objects->Count() ; ++i )
        {
            ComPtr<Access::AccessObject> object = objects->Item( i );
            if ( !object->Name().startsWith("~") )
            {
                items << ObjectItem::fromAccessObject(Model::Macro, object.ptr(), this);
            }
        }
    }
    //------------------------------------------------------------------------------------------
    // Module
    if ( projectType == Access::acADP )
    {
        ComPtr<Access::AllModules> objects = currentProject->AllModules();
        for ( int i = 0 ; i < objects->Count() ; ++i )
        {
            ComPtr<Access::AccessObject> object = objects->Item( i );
            if ( !object->Name().startsWith("~") )
            {
                items << ObjectItem::fromAccessObject(Model::Module, object.ptr(), this);
            }
        }
    }

    //------------------------------------------------------------------------------------------
    // Reference

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

    QDir sourceDir;
    sourceDir.setPath( currentProject->Path() + "\\source2" );

    if (!sourceDir.exists())
        return;

    //------------------------------------------------------------------------------------------
    // TableDef
    {
        QDir objectDir( sourceDir.absolutePath() + "\\tabledefs" );
        if (objectDir.exists())
        {
            objectDir.setNameFilters( (QStringList() << ("*" + QString(".xml"))) );
            QFileInfoList files = objectDir.entryInfoList( QDir::Files );
            foreach( QFileInfo fi, files)
            {
                addItem( ObjectItem::fromFileInfo( Model::Table, fi, this) );
            }
        }
    }
    //------------------------------------------------------------------------------------------
    // TableData
    //------------------------------------------------------------------------------------------
    // Relation

    //------------------------------------------------------------------------------------------
    // Query
    {
        QDir objectDir( sourceDir.absolutePath() + "\\queries" );
        if (objectDir.exists())
        {
            objectDir.setNameFilters( (QStringList() << ("*" + QString(".sql"))) );
            QFileInfoList files = objectDir.entryInfoList( QDir::Files );
            foreach( QFileInfo fi, files)
            {
                addItem( ObjectItem::fromFileInfo( Model::Query, fi, this) );
            }
        }
    }

    //------------------------------------------------------------------------------------------
    // Form
    {
        QDir objectDir( sourceDir.absolutePath() + "\\forms" );
        if (objectDir.exists())
        {
            objectDir.setNameFilters( (QStringList() << ("*" + QString(".frm"))) );
            QFileInfoList files = objectDir.entryInfoList( QDir::Files );
            foreach( QFileInfo fi, files)
            {
                addItem( ObjectItem::fromFileInfo( Model::Form, fi, this) );
            }
        }
    }

    //------------------------------------------------------------------------------------------
    // Report
    {
        QDir objectDir( sourceDir.absolutePath() + "\\reports" );
        if (objectDir.exists())
        {
            objectDir.setNameFilters( (QStringList() << ("*" + QString(".rpt"))) );
            QFileInfoList files = objectDir.entryInfoList( QDir::Files );
            foreach( QFileInfo fi, files)
            {
                addItem( ObjectItem::fromFileInfo( Model::Report, fi, this) );
            }
        }
    }

    //------------------------------------------------------------------------------------------
    // Macro
    {
        QDir objectDir( sourceDir.absolutePath() + "\\macros" );
        if (objectDir.exists())
        {
            objectDir.setNameFilters( (QStringList() << ("*" + QString(".mcr"))) );
            QFileInfoList files = objectDir.entryInfoList( QDir::Files );
            foreach( QFileInfo fi, files)
            {
                addItem( ObjectItem::fromFileInfo( Model::Macro, fi, this) );
            }
        }
    }

    //------------------------------------------------------------------------------------------
    // Module
    {
        QDir objectDir( sourceDir.absolutePath() + "\\modules" );
        if (objectDir.exists())
        {
            objectDir.setNameFilters( (QStringList() << ("*" + QString(".bas"))) );
            QFileInfoList files = objectDir.entryInfoList( QDir::Files );
            foreach( QFileInfo fi, files)
            {
                addItem( ObjectItem::fromFileInfo( Model::Module, fi, this) );
            }
        }
    }

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


    QTime time;
    QTime timeTotal;

    timeTotal.start();
    time.start();

    ProcessData processData;// = {0};
    SubProcessData subProcessData;// = {0};
    Q_UNUSED(subProcessData);

    emit processStart(processData);

    time.start();
    QString appVersion = m_application->Version();
    QString appName = m_application->Name();
    ComPtr<Access::CurrentProject> currentProject  = m_application->CurrentProject();
    ComPtr<DAO::Database> currentDb = m_application->CurrentDb();

    if ( !currentProject.is() )
    {
        QMessageBox::information(0, QString(""), QString("currentProject is null"));
        return;
    }
    int projectType         = currentProject->ProjectType();
    QString projectFullName = currentProject->FullName();
    QString projectPath     = currentProject->Path();

    if ( projectType == Access::acMDB && !currentDb.is() )
    {
        QMessageBox::information(0, QString(""), QString("projectType is null"));
        return;
    }

    qDebug() << appName << appVersion;
    qDebug() << projectFullName << projectType;
    qDebug() << "Preparation : " << time.elapsed();


    QDir tempSourceDir( currentProject->Path() + "\\source_temp"  );
    tempSourceDir.rmdir("");
    tempSourceDir.mkpath("");

    struct ObjectSetting
    {
        int objectType;
        QDir sourceDir;
        QString objectPathName;
        QString containerName;
        QString tempFileExtension;
        QString designFileExtension;
        QString moduleFileExtension;

        QString tempFile(const QString &objectName) {
            return objectPath() + "\\" + objectName + "." + tempFileExtension;
        }
        QString designFile(const QString &objectName) {
            return objectPath() + "\\" + objectName + "." + designFileExtension;
        }
        void mkdirObjectPath() {
            sourceDir.mkpath( objectPathName );
        }
        void saveToFile(const QString &contents, const QString &filePath) {
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
        QString objectPath() {
            QString path = sourceDir.absolutePath();
            if ( !objectPathName.isEmpty() )
                path += "\\" + objectPathName;
            return path;
        }
    };

    ObjectSetting objectSettings[] = {
        { Access::acTable,  tempSourceDir, "tabledefs", "",        "tmp","xml","" },
        { Access::acQuery,  tempSourceDir, "queries",   "",        "tmp","sql","" },
        { Access::acForm,   tempSourceDir, "forms",     "Forms",   "tmp","frm","bas" },
        { Access::acReport, tempSourceDir, "reports",   "Reports", "tmp","rpt","bas" },
        { Access::acMacro,  tempSourceDir, "macros",    "Scripts", "tmp","mcr","" },
        { Access::acModule, tempSourceDir, "modules",   "Modules", "tmp","bas","" },
        { -1,               tempSourceDir, "",          "",        "tmp","csv","" }
    };
    ObjectSetting *os;


    //------------------------------------------------------------------------------------------
    // TableDef
    time.start();
    if ( projectType == Access::acMDB )
    {
        os = &objectSettings[ 0 ];

        os->mkdirObjectPath();

        ComPtr<DAO::TableDefs> tableDefs = currentDb->TableDefs();
        int nCount = tableDefs->Count();

        emit subProcessStart(processData, { nCount, 0 } );

        for ( int i = 0 ; i < nCount ; ++i )
        {
            emit subProcessProgess(processData,  { nCount, i + 1 } );

            ComPtr<DAO::TableDef> tableDef = tableDefs->Item(i);
            QString objectName   = tableDef->Name();
            QString connect      = tableDef->Connect();
            if ( !objectName.startsWith("~") && !objectName.startsWith("MSys"))
            {
                if (connect.isEmpty())
                {
                    // Local Table

                    // Xml Export

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

                    //pTableDef->Indexes()->Item(0)


                }
                else
                {
                    // TODO:
                    // Link Table
                }
            }
            QApplication::processEvents();
        }

        emit subProcessEnd(processData, { nCount, nCount } );
        qDebug() << "TableDefs : " << nCount << " : " << time.elapsed();
    }
    //------------------------------------------------------------------------------------------
    // TableData
    if ( projectType == Access::acMDB )
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
    if ( projectType == Access::acMDB )
    {
        // TODO:
    }


    //------------------------------------------------------------------------------------------
    // Query
    time.start();
    if ( projectType == Access::acMDB )
    {
        os = &objectSettings[ 1 ];

        os->mkdirObjectPath();

        ComPtr<DAO::QueryDefs> queryDefs = currentDb->QueryDefs();
        int nCount = queryDefs->Count();
        emit subProcessStart(processData, { nCount, 0 } );

        for ( int i = 0 ; i < nCount ; ++i )
        {
            emit subProcessProgess(processData,  { nCount, i + 1 } );
            ComPtr<DAO::QueryDef> queryDef = queryDefs->Item(i);
            QString objectName = queryDef->Name();
            if ( !objectName.startsWith("~"))
            {
                // Access Export
                if (false)
                {
                    //pApplication->dynamicCall("SaveAsText(const int,const QString&,const QString&)", os->objectType, objectName, os->tempFile(objectName) );
                    //sanitizer.localToUtf8( os->tempFile(objectName), os->designFile(objectName));
                }

                // Export as SQL
                if (true)
                {
                    QString sql = queryDef->SQL();
                    os->saveToFile( sql, os->designFile(objectName) );
                }
            }
            QApplication::processEvents();
        }
        emit subProcessEnd(processData, { nCount, nCount } );
        qDebug() << "Queries : " << nCount << " : " << time.elapsed();
    }


    //------------------------------------------------------------------------------------------
    // Form, Report, Macro, Module
    if ( projectType == Access::acMDB )
    {
        ComPtr<DAO::Containers> containers = currentDb->Containers();

        time.start();
        foreach ( int index, (QList<int>() << 2 << 3 << 4 << 5 ) )
        {
            os = &objectSettings[ index ];

            os->mkdirObjectPath();

            ComPtr<DAO::Container> container = containers->Item( os->containerName );
            ComPtr<DAO::Documents> documents = container->Documents();

            int nCount = documents->Count();
            emit subProcessStart(processData, { nCount, 0 } );

            for ( int i = 0 ; i < nCount ; ++i )
            {
                emit subProcessProgess(processData,  { nCount, i + 1 } );
                ComPtr<DAO::Document> document = documents->Item(i);

                QString objectName = document->Name();
                if ( !objectName.startsWith("~") )
                {
                    m_application->SaveAsText( (Access::AcObjectType)os->objectType, objectName, os->tempFile(objectName) );
                }
                QApplication::processEvents();
            }
            emit subProcessEnd(processData, { nCount, nCount } );
            qDebug() << os->objectPathName << " : " << nCount << " : " << time.elapsed();
        }

    }
    //------------------------------------------------------------------------------------------
    // Form, Report, Macro, Module
    // TODO:
    if ( projectType == Access::acADP )
    {

    }

    //------------------------------------------------------------------------------------------
    // Reference
    time.start();
    {
        QString contents;
        contents = "";

        ComPtr<Access::References> references = m_application->References();
        int nCount = references->Count();
        emit subProcessStart(processData, { nCount, 0 } );

        for ( int i = 1 ; i <= nCount ; ++i )
        {
            emit subProcessProgess(processData,  { nCount, i + 1 } );
            ComPtr<Access::Reference> reference = references->Item(i);

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
            QApplication::processEvents();
        }

        // write contents to file
        {
            os = &objectSettings[ 6 ];

            QString objectName = "references";
            os->saveToFile(contents, os->designFile(objectName));
        }

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

