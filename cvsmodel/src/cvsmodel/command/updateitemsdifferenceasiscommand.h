#ifndef UPDATEITEMSDIFFERENCEASISCOMMAND_H
#define UPDATEITEMSDIFFERENCEASISCOMMAND_H

#include "commandbase.h"

class UpdateItemsDifferenceAsIsCommand : public CommandBase
{
    Q_OBJECT
public:
    explicit UpdateItemsDifferenceAsIsCommand(QAxObject *application, QList<ObjectItem*> *items, QObject *parent = 0);

    virtual void execute(ObjectItemMap *allTargets);
};

#endif // UPDATEITEMSDIFFERENCEASISCOMMAND_H
