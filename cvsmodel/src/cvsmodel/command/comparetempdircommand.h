#ifndef COMPARETEMPDIRCOMMAND_H
#define COMPARETEMPDIRCOMMAND_H

#include "commandbase.h"

class CompareTempDirCommand : public CommandBase
{
    Q_OBJECT
public:
    explicit CompareTempDirCommand(QObject *parent = 0);

    virtual void execute(ObjectItems *allTargets);
};

#endif // COMPARETEMPDIRCOMMAND_H
