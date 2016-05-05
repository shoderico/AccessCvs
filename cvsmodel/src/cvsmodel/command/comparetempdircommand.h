#ifndef COMPARETEMPDIRCOMMAND_H
#define COMPARETEMPDIRCOMMAND_H

#include "commandbase.h"

class CompareTempDirCommand : public CommandBase
{
    Q_OBJECT
public:
    explicit CompareTempDirCommand(ProjectContainer *project, QAxObject *application, QList<ObjectItem*> *items, QObject *parent = 0);

    virtual void execute(ObjectItemMap *allTargets);
};

#endif // COMPARETEMPDIRCOMMAND_H
