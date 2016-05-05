#ifndef UPDATEITEMSDIFFERENCECOMMAND_H
#define UPDATEITEMSDIFFERENCECOMMAND_H

#include "commandbase.h"

class UpdateItemsDifferenceCommand : public CommandBase
{
    Q_OBJECT
public:
    explicit UpdateItemsDifferenceCommand(const int difference, ProjectContainer *project, QAxObject *application, QList<ObjectItem*> *items, QObject *parent = 0);

    virtual void execute(ObjectItemMap *allTargets);

private:
    int m_difference;
};

#endif // UPDATEITEMSDIFFERENCECOMMAND_H
