#ifndef ODBCTABLEPROCESSOR_H
#define ODBCTABLEPROCESSOR_H

#include "tableobjectprocessor.h"

#include "cvsmodel/sanitizer/tabledefsanitizer.h"
#include "cvsmodel/sanitizer/tabledatasanitizer.h"

class ObjectItem;

class OdbcTableProcessor : public TableObjectProcessor
{
public:
    explicit OdbcTableProcessor(ProjectContainer *parent);

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

private:
    bool m_toBeManaged;

    TableDefSanitizer *m_tableDefSanitizer;
    TableDataSanitizer *m_tableDataSanitizer;

    // ODBC property export/import helpers
    bool exportOdbcTable(QAxObject *object, const QString &objectName);
    bool importOdbcTable(const QString &objectName);

    // ODBC-specific sanitize/desanitize helpers
    bool sanitizeOdbcProperty(const QString &objectName);
    bool desanitizeOdbcProperty(const QString &objectName);

};

#endif // ODBCTABLEPROCESSOR_H
