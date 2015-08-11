#ifndef PROJECTSETTING_H
#define PROJECTSETTING_H

#include <QObject>
#include <QMap>

#include "objectitem.h"
//#include "util/comptr.h"

class ObjectSetting;
//#include "objectsetting.h"

namespace Access {
class Application;
}

class ProjectSetting : public QObject
{
    Q_OBJECT
public:
    explicit ProjectSetting(QObject *parent = 0);
    ~ProjectSetting();

    void initialize(Access::Application* application);

    bool isMDB() const;
    bool isADP() const;

    QString sourcePath() const;
    QString tempPath() const;

    ObjectSetting* operator[](Model::ObjectType objectType);

    Access::Application *application() const;

    QList<Model::ObjectType> objectTypes() const;

    void loadSettings();
    void saveSettings();

public slots:
    void exception(int code, const QString & source, const QString & desc, const QString & help);

private:
    QMap<Model::ObjectType, ObjectSetting*> m_objectSettings;
    Access::Application *m_application;
    int m_projectType;
    QString m_projectPath;

    QString m_sourcePathName;
    QString m_tempPathName;
};

#endif // PROJECTSETTING_H
