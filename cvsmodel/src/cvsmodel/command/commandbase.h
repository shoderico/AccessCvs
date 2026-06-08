#ifndef COMMANDBASE_H
#define COMMANDBASE_H

#include <QObject>

class QAxObject;

class ObjectItem;
class ObjectItemMap;
class DataChangedHelper;
class ProjectContainer;

class CommandBase : public QObject
{
    Q_OBJECT
public:
    explicit CommandBase(ProjectContainer *project, QObject *parent = 0);
    explicit CommandBase(ProjectContainer *project, QAxObject *application, QObject *parent = 0);
    explicit CommandBase(ProjectContainer *project, QAxObject *application, QList<ObjectItem*> *items, QObject *parent = 0);

    void setApplication(QAxObject *application);
    void setItems(QList<ObjectItem*> *items);
    virtual void execute(ObjectItemMap *allTargets);

signals:
    void progressStart(int type, int count);
    void progressEnd(int type);
    void progressChange(int type, int position);

    void currentObjectTypeChanged(int objectType);

    void dataChanged(int rowStart, int rowEnd, int columnStart, int columnEnd);

public slots:

private:
    void assign();

protected:
    ProjectContainer *m_project;
    QAxObject *m_application;
    QList<ObjectItem*> *m_items;
};


struct FunctionObjectBase
{
    FunctionObjectBase(DataChangedHelper *dataChangedHelper, QList<ObjectItem*> *items);

    typedef void result_type;

    void registerChanged(ObjectItem* item);

    DataChangedHelper *m_dataChangedHelper;
    QList<ObjectItem*> *m_items;
};



#endif // COMMANDBASE_H
