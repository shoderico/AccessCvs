#ifndef UPDATEITEMSINPROJECTCOMMAND_H
#define UPDATEITEMSINPROJECTCOMMAND_H

#include "commandbase.h"

class UpdateItemsInProjectCommand : public CommandBase
{
    Q_OBJECT
public:
    explicit UpdateItemsInProjectCommand(const int existence, QAxObject *application, QList<ObjectItem*> *items, QObject *parent = 0);

    virtual void execute(ObjectItems *allTargets);

private:
    int m_existence;
};

#endif // UPDATEITEMSINPROJECTCOMMAND_H
