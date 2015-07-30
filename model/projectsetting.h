#ifndef PROJECTSETTING_H
#define PROJECTSETTING_H

#include <QObject>
#include <QMap>

#include "objectitem.h"

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

    void initialize(const Access::Application* application);

    bool isMDB() const;
    bool isADP() const;

    QString sourcePath() const;
    QString tempPath() const;

    ObjectSetting* operator[](Model::ObjectType objectType);

private:
    QMap<Model::ObjectType, ObjectSetting*> m_objectSettings;
    int m_projectType;
    QString m_projectPath;

    QString m_sourcePathName;
    QString m_tempPathName;
};

#endif // PROJECTSETTING_H
