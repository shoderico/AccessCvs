#ifndef DELETEFROMSOURCEDIRCOMMAND_H
#define DELETEFROMSOURCEDIRCOMMAND_H

#include "commandbase.h"

class DeleteFromSourceDirCommand : public CommandBase
{
    Q_OBJECT
public:
    explicit DeleteFromSourceDirCommand(ProjectContainer *project, QAxObject *application, QObject *parent = 0);

    virtual void execute(ObjectItemMap *allTargets);
};

#endif // DELETEFROMSOURCEDIRCOMMAND_H
