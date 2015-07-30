#ifndef OBJECTMODEL_H
#define OBJECTMODEL_H

#include <QAbstractItemModel>

#include "objectitem.h"

namespace Access {
class Application;
}

class ObjectModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    ObjectModel(QObject * parent = 0);

    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant data(const QModelIndex &index, int role) const;

    // TODO: implement
    // index()
    // parent()
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;

    enum Column
    {
        NameColumn = 0,
        InProjectColumn,
        InFileSystemColumn,
        CreateDateColumn,
        UpdateDateColumn,
        ObjectTypeColumn,
        ColumnCount
    };

    // interface for ui
    void setApplication(Access::Application *application);


    // Step-1 :
    void loadFromProject();     //                                                      : BLOCK
    void loadFromFileSystem();  //                                                      :

    // Step-2 :
    void exportToTempDir();     // InBoth           , InProjectOnly ,                   : BLOCK
    void sanitizeTempDir();     // InBoth           , InProjectOnly ,                   :
    void checkDiffInTempDir();  // InBoth           ,               ,                   :


    // Step-3 Export
    void copyToFileSystem();    // InBoth:DiffOnly  , InProjectOnly ,                   :
    void deleteFromFileSystem();//                  ,               , InFileSystemOnly  :


    // Step-3 Import
    void copyToTempDir();       // InBoth:DiffOnly  ,               , InFileSystemOnly  :
    void desanitizeTempDir();   // InBoth:DiffOnly  ,               , InFileSystemOnly  :

    // Step-4 Import
    void importFromTempDir();   // InBoth:DiffOnly  ,               , InFileSystemOnly  : BLOCK
    void deleteFromProject();   //                  , InProjectOnly ,                   : BLOCK


public:
    struct ProcessData
    {
        int count;
        int position;
    };
    struct SubProcessData
    {
        int count;
        int position;
    };


signals:
    void processStart(ObjectModel::ProcessData processData);
    void processEnd(ObjectModel::ProcessData processData);
    void subProcessStart(ObjectModel::ProcessData processData, ObjectModel::SubProcessData subProcessData);
    void subProcessEnd(ObjectModel::ProcessData processData, ObjectModel::SubProcessData subProcessData);
    void subProcessProgess(ObjectModel::ProcessData processData, ObjectModel::SubProcessData subProcessData);

private:
    QList<ObjectItem*> m_items;
    QMap< Model::ObjectType, QMap< QString, ObjectItem* > > m_mapItems;
    Access::Application *m_application;

    void addItem( ObjectItem* item);
};

Q_DECLARE_METATYPE(ObjectModel::ProcessData);
Q_DECLARE_METATYPE(ObjectModel::SubProcessData);

#endif // OBJECTMODEL_H
