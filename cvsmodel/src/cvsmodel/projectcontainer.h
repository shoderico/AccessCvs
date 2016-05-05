#ifndef PROJECTCONTAINER_H
#define PROJECTCONTAINER_H

#include "cvsmodel_global.h"
#include "cvsmodel_const.h"

#include <QObject>
#include <QMap>

class QAxObject;
class QSettings;

class ObjectProcessor;

namespace Access {
class Application;
}

class CVSMODEL_SHARED_EXPORT ProjectContainer : public QObject
{
    Q_OBJECT
public:
    explicit ProjectContainer(QObject *parent = 0);
    ~ProjectContainer();

    void initialize(QAxObject* application);

    bool isMDB() const;
    bool isADP() const;
    bool isProjectOpened() const;

    QString sourcePath() const;
    QString tempPath() const;

    ObjectProcessor* operator[](Model::ObjectType objectType);

    Access::Application *application() const;

    QList<Model::ObjectType> objectTypes() const;

    void loadSettings();
    void saveSettings();

public slots:
    void exception(int code, const QString & source, const QString & desc, const QString & help);

private:
    QSettings *createSettings();
    QString settingsFilePath() const;

private:
    QMap<Model::ObjectType, ObjectProcessor*> m_objectProcessors;
    Access::Application *m_application;
    int m_projectType;
    QString m_projectPath;

    QString m_sourcePathName;
    QString m_tempPathName;

    QString m_settingsFileName;
};

#endif // PROJECTCONTAINER_H
