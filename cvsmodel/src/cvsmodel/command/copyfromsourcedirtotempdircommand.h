#ifndef COPYFROMSOURCEDIRTOTEMPDIRCOMMAND_H
#define COPYFROMSOURCEDIRTOTEMPDIRCOMMAND_H

#include "commandbase.h"

class CopyFromSourceDirToTempDirCommand : public CommandBase
{
    Q_OBJECT
public:
    explicit CopyFromSourceDirToTempDirCommand(ProjectContainer *project, QAxObject *application, QObject *parent = 0);

    virtual void execute(ObjectItemMap *allTargets);
};

#endif // COPYFROMSOURCEDIRTOTEMPDIRCOMMAND_H
