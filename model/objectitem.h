#ifndef OBJECTITEM_H
#define OBJECTITEM_H

#include <QObject>
#include <QString>
#include <QDateTime>

namespace Model {
enum ObjectType
{
    Unknwon = 0,
    Unkown_OT = 0,
    TableDef,
    Query,
    Form,
    Report,
    Macro,
    Module,
    TableData,
    Relation,
    Reference
};

enum GitStatus
{
    GS_Unknown = 0,
    Unknown_GS = 0,
    Unmodified,
    Modified,
    Added,
    Deleted,
    Renamed,
    Copied,
    UpdatedButMerged,
    Untracked,
    Ignored
};

enum ObjectCheckState
{
    State_NoChange = 0,
    State_On = 1,
    State_Off = 2,
};

enum ObjectExistence
{
    OE_Unchecked = ObjectCheckState::State_NoChange,
    Unchecked_OE = ObjectCheckState::State_NoChange,
    Present = ObjectCheckState::State_On,
    Absent  = ObjectCheckState::State_Off,
};

enum ObjectDifference
{
    OD_Unchecked = ObjectCheckState::State_NoChange,
    Unchecked_OD = ObjectCheckState::State_NoChange,
    DifferentContents = ObjectCheckState::State_On,
    SameContents = ObjectCheckState::State_Off,
};

}

class ObjectItem : public QObject
{
    Q_OBJECT
public:
    ObjectItem(QObject *parent = 0);
    ObjectItem(ObjectItem *object, QObject *parent);

    /*!
       \brief name
       \return
     */
    QString name() const { return m_name; }
    void setName(const QString &name) { m_name = name; }

    /*!
       \brief createDate
       \return
     */
    QDateTime createDate() const { return m_createDate; }
    void setCreateDate(const QDateTime &dt) { m_createDate = dt; }

    /*!
       \brief updateDate
       \return
     */
    QDateTime updateDate() const { return m_updateDate; }
    void setUpdateDate(const QDateTime &dt) { m_updateDate = dt; }

    /*!
       \brief exportDate
       \return
     */
    QDateTime exportDate() const { return m_exportDate; }
    void setExportDate(const QDateTime &dt) { m_exportDate = dt; }

    /*!
       \brief isSelected
       \return
     */
    bool isSelected() const { return m_isSelected; }
    void setSelected(const bool selected) { m_isSelected = selected; }

    /*!
       \brief inProject
       \return
     */
    Model::ObjectExistence inProject() const { return m_inProject; }
    void setInProject(const Model::ObjectExistence existence) { m_inProject = existence; }

    /*!
       \brief inFileSystem
       \return
     */
    Model::ObjectExistence inFileSystem() const { return m_inFileSystem; }
    void setInFileSystem(const Model::ObjectExistence existence) { m_inFileSystem = existence; }

    /*!
       \brief isDifferent
       \return
     */
    Model::ObjectDifference isDifferent() const { return m_isDifferent; }
    void setDifferent(const Model::ObjectDifference different) { m_isDifferent = different; }

    /*!
       \brief hasData
       \return
     */
    bool hasData() const { return m_hasData; }
    void setHasData(bool has) { m_hasData = has; }


    /*!
       \brief objectType
       \return
     */
    Model::ObjectType objectType() const { return m_objectType; }
    void setObjectType(const Model::ObjectType objectType) { m_objectType = objectType; }

    /*!
       \brief gitStatusInIndex
       \return
     */
    Model::GitStatus gitStatusInIndex() const { return m_gitStatusInIndex; }
    void setGitStatusInIndex(const Model::GitStatus status) { m_gitStatusInIndex = status; }

    /*!
       \brief gitStatusInWorkTree
       \return
     */
    Model::GitStatus gitStatusInWorkTree() const { return m_gitStatusInWorkTree; }
    void setGitStatusInWorkTree(const Model::GitStatus status) { m_gitStatusInWorkTree = status; }


public:
    bool isModified() const;

protected:
    QString     m_name;
    QDateTime   m_createDate;
    QDateTime   m_updateDate;
    QDateTime   m_exportDate;
    bool        m_isSelected;
    bool        m_hasData;
    Model::ObjectExistence  m_inProject;
    Model::ObjectExistence  m_inFileSystem;
    Model::ObjectDifference m_isDifferent;
    Model::ObjectType       m_objectType;
    Model::GitStatus        m_gitStatusInIndex;
    Model::GitStatus        m_gitStatusInWorkTree;
};

#endif // OBJECTITEM_H
