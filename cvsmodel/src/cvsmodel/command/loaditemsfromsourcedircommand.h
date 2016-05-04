#ifndef LOADITEMSFROMSOURCEDIRCOMMAND_H
#define LOADITEMSFROMSOURCEDIRCOMMAND_H

#include "commandbase.h"

class LoadItemsFromSourceDirCommand : public CommandBase
{
    Q_OBJECT
public:
    explicit LoadItemsFromSourceDirCommand(QAxObject *application, QList<ObjectItem*> *items, QObject *parent = 0);

    virtual void execute(ObjectItems *allTargets);
};

#endif // LOADITEMSFROMSOURCEDIRCOMMAND_H
