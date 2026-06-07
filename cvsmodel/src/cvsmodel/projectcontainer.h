#ifndef PROJECTCONTAINER_H
#define PROJECTCONTAINER_H

#include "cvsmodel_global.h"
#include "cvsmodel_const.h"

#include <QObject>
#include <QMap>

class QAxObject;

class ObjectProcessor;
class ObjectItem;
class Setting;

class CVSMODEL_SHARED_EXPORT ProjectContainer : public QObject
{
    Q_OBJECT
public:
    explicit ProjectContainer(QObject *parent = 0);
    ~ProjectContainer();

    virtual void initialize(QAxObject* application);

    virtual bool isProjectOpened() const = 0;

    QString currentProjectFullName() const;
    QString currentProjectPath() const;
    QString projectPath() const;
    QString sourcePath() const;
    QString tempPath() const;

    ObjectProcessor* operator[](Model::ObjectType objectType);

    QAxObject *application() const;
    template <typename T> T *application() const
    {
        return dynamic_cast<T *>(m_application);
    }


    QList<Model::ObjectType> objectTypes() const;
    const QList<Model::ObjectType> &objectTypesForItemMap() const;

    void loadSetting();
    void saveSetting();

    void updateSetting(QList<ObjectItem*> *items);


public slots:
    void exception(int code, const QString & source, const QString & desc, const QString & help);

private:
    Setting *createSetting();

private:
    QAxObject *m_application;

protected:
//    QMap<Model::ObjectType, ObjectProcessor*> m_objectProcessorMap;
    QList<ObjectProcessor*> m_objectProcessorList;
    QList<Model::ObjectType> m_objectTypesForItemMap;

    QString m_currentProjectName;
    QString m_currentProjectFullName;
    QString m_currentProjectPath;
    QString m_projectPath;

    QString m_sourcePathName;
    QString m_tempPathName;
    QString m_settingFileName;

    QString m_DefaultSourcePathName;
    QString m_DefaultTempPathName;
    QString m_DefaultSettingFileName;
};

#endif // PROJECTCONTAINER_H
