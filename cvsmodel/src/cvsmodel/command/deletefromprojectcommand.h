#ifndef DELETEFROMPROJECTCOMMAND_H
#define DELETEFROMPROJECTCOMMAND_H

#include "commandbase.h"

class DeleteFromProjectCommand : public CommandBase
{
    Q_OBJECT
public:
    explicit DeleteFromProjectCommand(QAxObject *application, QObject *parent = 0);

    virtual void execute(ObjectItemMap *allTargets);
};

#endif // DELETEFROMPROJECTCOMMAND_H
