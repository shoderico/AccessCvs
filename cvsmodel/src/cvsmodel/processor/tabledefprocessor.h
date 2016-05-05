#ifndef TABLEDEFPROCESSOR_H
#define TABLEDEFPROCESSOR_H

#include "cvsmodel/cvsmodel_global.h"
#include "objectprocessor.h"

#include "util/comptr.h"

class ProjectContainer;
class TableDefSanitizer;
class TableDataSanitizer;
class ObjectItem;

class QAxObject;
class QSettings;
class QStringList;
class QFileInfo;
class QVariant;

namespace DAO {
class TableDefs;
}


class TableDefProcessor : public ObjectProcessor
{
public:
    explicit TableDefProcessor(ProjectContainer *parent);
    virtual bool        isTargetObject(QAxObject *object) const;
    virtual ObjectItem *createItemFromProject(QAxObject* object, QObject *parent = 0);
    virtual ObjectItem *createItemFromSourceDir(QFileInfo &fileInfo, QObject *parent = 0);
    virtual bool        exportFromProjectToTempDir(QAxObject* object, const QString &objectName);
    virtual bool        importFromTempDirToProject(QAxObject* object, const QString &objectName);
    virtual bool        sanitizeTempDir(QAxObject* object, const QString &objectName);
    virtual bool        desanitizeTempDir(QAxObject* object, const QString &objectName);

    virtual bool        prepareItemCollection();
    virtual int         itemCount();
    virtual QAxObject  *itemUnsafePtr(const QVariant &index);

    virtual void loadSettings(QSettings *settings);
    virtual void saveSettings(QSettings *settings);

    void setTableDataTargets(QStringList *newTargets);
    virtual void determineCodecForProject();
protected:
    ComPtr<DAO::TableDefs> m_tableDefs;
    QStringList m_tableDataTargets;
    TableDefSanitizer *m_tableDefSanitizer;
    TableDataSanitizer *m_tableDataSanitizer;
};

//class TableDataSetting : public ObjectSetting
//{
//public:
//    explicit TableDataSetting(ProjectSetting *parent);
//};



#endif // TABLEDEFPROCESSOR_H
