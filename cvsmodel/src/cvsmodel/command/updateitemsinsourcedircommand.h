#ifndef UPDATEITEMSINSOURCEDIRCOMMAND_H
#define UPDATEITEMSINSOURCEDIRCOMMAND_H

#include "commandbase.h"

class UpdateItemsInSourceDirCommand : public CommandBase
{
    Q_OBJECT
public:
    explicit UpdateItemsInSourceDirCommand(const int existence, ProjectContainer *project, QAxObject *application, QList<ObjectItem*> *items, QObject *parent = 0);

    virtual void execute(ObjectItemMap *allTargets);

private:
    int m_existence;
};

#endif // UPDATEITEMSINSOURCEDIRCOMMAND_H
