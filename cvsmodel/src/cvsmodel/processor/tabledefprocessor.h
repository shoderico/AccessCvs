#ifndef TABLEDEFPROCESSOR_H
#define TABLEDEFPROCESSOR_H

#include "cvsmodel/cvsmodel_global.h"
#include "tableobjectprocessor.h"

class ProjectContainer;
class TableDefSanitizer;
class TableDataSanitizer;
class ObjectItem;

class QAxObject;
class QStringList;
class QFileInfo;
class QVariant;


class TableDefProcessor : public TableObjectProcessor
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

    virtual void loadSetting(Setting *projectSetting);
    virtual void saveSetting(Setting *projectSetting);

    virtual void updateSetting(QList<ObjectItem*> *items);

protected:
    QStringList m_tableDataTargets;
    TableDefSanitizer *m_tableDefSanitizer;
    TableDataSanitizer *m_tableDataSanitizer;
};

#endif // TABLEDEFPROCESSOR_H
