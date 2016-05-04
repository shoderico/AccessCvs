#ifndef UPDATEITEMSCREATEUPDATEDATEFROMPROJECTCOMMAND_H
#define UPDATEITEMSCREATEUPDATEDATEFROMPROJECTCOMMAND_H

#include "commandbase.h"

class UpdateItemsCreateUpdateDateFromProjectCommand : public CommandBase
{
    Q_OBJECT
public:
    explicit UpdateItemsCreateUpdateDateFromProjectCommand(QAxObject *application, QList<ObjectItem*> *items, QObject *parent = 0);

    virtual void execute(ObjectItemMap *allTargets);
};

#endif // UPDATEITEMSCREATEUPDATEDATEFROMPROJECTCOMMAND_H
