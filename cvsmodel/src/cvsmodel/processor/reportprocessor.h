#ifndef REPORTPROCESSOR_H
#define REPORTPROCESSOR_H

#include "cvsmodel/cvsmodel_global.h"
#include "accessdesignobjectprocessor.h"

#include <QReadWriteLock>

#include "util/comptr.h"

class ProjectContainer;
class AccessDesignObjectSanitizer;

class QAxObject;
class QVariant;

namespace Access {
class AllReports;
}

class ReportProcessor : public AccessDesignObjectProcessor
{
public:
    explicit ReportProcessor(ProjectContainer *parent);
    virtual bool        prepareItemCollection();
    virtual int         itemCount();
    virtual QAxObject  *itemUnsafePtr(const QVariant &index);
    virtual bool        afterImportFromTempDirToProject(QAxObject* object, const QString &objectName);
protected:
    virtual bool afterSanitizeTempDir(QAxObject *object, const QString &objectName, AccessDesignObjectSanitizer *sanitizer);
    ComPtr<Access::AllReports> m_objects;
    QReadWriteLock m_lock;
};


#endif // REPORTPROCESSOR_H
