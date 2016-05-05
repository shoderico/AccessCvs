#ifndef ACCESSDESIGNOBJECTPROCESSOR_H
#define ACCESSDESIGNOBJECTPROCESSOR_H

#include "cvsmodel/cvsmodel_global.h"
#include "accessobjectprocessor.h"

class ProjectContainer;
class AccessDesignObjectSanitizer;

class QAxObject;

class AccessDesignObjectProcessor : public AccessObjectProcessor
{
public:
    explicit AccessDesignObjectProcessor(ProjectContainer *parent);
    virtual bool        sanitizeTempDir(QAxObject* object, const QString &objectName);
    virtual bool        desanitizeTempDir(QAxObject* object, const QString &objectName);
    virtual void determineCodecForProject();

protected:
    virtual bool afterSanitizeTempDir(QAxObject* object, const QString &objectName, AccessDesignObjectSanitizer *sanitizer);
    AccessDesignObjectSanitizer *m_sanitizer;
};


#endif // ACCESSDESIGNOBJECTPROCESSOR_H
