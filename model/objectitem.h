#ifndef OBJECTITEM_H
#define OBJECTITEM_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QFileInfo>

namespace Model {
enum ObjectType
{
    Unknwon,
    Table,
    Query,
    Form,
    Report,
    Macro,
    Module,
    TableData,
    Relation,
    Reference
};

}

namespace Access {
class AccessObject;
}

namespace DAO {
class TableDef;
class QueryDef;
class Document;
}

class ObjectItem : public QObject
{
    Q_OBJECT
public:
    ObjectItem(QObject *parent = 0);

    QString name() const { return m_name; }
    void setName(const QString &name) { m_name = name; }

    QDateTime createDate() const { return m_createDate; }
    void setCreateDate(const QDateTime &dt) { m_createDate = dt; }

    QDateTime updateDate() const { return m_updateDate; }
    void setUpdateDate(const QDateTime &dt) { m_updateDate = dt; }

    bool inProject() const { return m_inProject; }
    void setInProject(const bool in) { m_inProject = in; }

    bool inFileSystem() const { return m_inFileSystem; }
    void setInFileSystem(const bool in) { m_inFileSystem = in; }

    Model::ObjectType objectType() const { return m_objectType; }
    void setObjectType(const Model::ObjectType objectType) { m_objectType = objectType; }

    bool isSelected() const { return m_isSelected; }
    void setSelected(const bool selected) { m_isSelected = selected; }


    static ObjectItem *fromTableDef(DAO::TableDef* tableDef, QObject *parent = 0);
    static ObjectItem *fromQueryDef(DAO::QueryDef* queryDef, QObject *parent = 0);
    static ObjectItem *fromAccessObject(Model::ObjectType objectType, Access::AccessObject* accessObject, QObject *parent = 0);
    static ObjectItem *fromDAODocument(Model::ObjectType objectType, DAO::Document* daoDocument, QObject *parent = 0);
    static ObjectItem *fromFileInfo(Model::ObjectType objectType, QFileInfo &fileInfo, QObject *parent = 0);

protected:
    QString m_name;
    QDateTime m_createDate;
    QDateTime m_updateDate;
    bool m_inProject;
    bool m_inFileSystem;
    Model::ObjectType m_objectType;
    bool m_isSelected;
};

#endif // OBJECTITEM_H
