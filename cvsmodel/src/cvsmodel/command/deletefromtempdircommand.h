#ifndef DELETEFROMTEMPDIRCOMMAND_H
#define DELETEFROMTEMPDIRCOMMAND_H

#include "commandbase.h"

class DeleteFromTempDirCommand : public CommandBase
{
    Q_OBJECT
public:
    explicit DeleteFromTempDirCommand(QObject *parent = 0);

    virtual void execute(ObjectItems *allTargets);
};

#endif // DELETEFROMTEMPDIRCOMMAND_H
