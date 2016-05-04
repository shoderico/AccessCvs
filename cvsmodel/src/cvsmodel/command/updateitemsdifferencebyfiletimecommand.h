#ifndef UPDATEITEMSDIFFERENCEBYFILETIMECOMMAND_H
#define UPDATEITEMSDIFFERENCEBYFILETIMECOMMAND_H

#include "commandbase.h"

class UpdateItemsDifferenceByFileTimeCommand : public CommandBase
{
    Q_OBJECT
public:
    explicit UpdateItemsDifferenceByFileTimeCommand(QAxObject *application, QList<ObjectItem*> *items, QObject *parent = 0);

    virtual void execute(ObjectItemMap *allTargets);
};

#endif // UPDATEITEMSDIFFERENCEBYFILETIMECOMMAND_H
