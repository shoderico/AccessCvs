#ifndef DELETEFROMSOURCEDIRCOMMAND_H
#define DELETEFROMSOURCEDIRCOMMAND_H

#include "commandbase.h"

class DeleteFromSourceDirCommand : public CommandBase
{
    Q_OBJECT
public:
    explicit DeleteFromSourceDirCommand(QObject *parent = 0);

    virtual void execute(ObjectItems *allTargets);
};

#endif // DELETEFROMSOURCEDIRCOMMAND_H
