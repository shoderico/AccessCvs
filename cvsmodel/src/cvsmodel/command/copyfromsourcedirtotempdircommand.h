#ifndef COPYFROMSOURCEDIRTOTEMPDIRCOMMAND_H
#define COPYFROMSOURCEDIRTOTEMPDIRCOMMAND_H

#include "commandbase.h"

class CopyFromSourceDirToTempDirCommand : public CommandBase
{
    Q_OBJECT
public:
    explicit CopyFromSourceDirToTempDirCommand(QObject *parent = 0);

    virtual void execute(ObjectItems *allTargets);
};

#endif // COPYFROMSOURCEDIRTOTEMPDIRCOMMAND_H
