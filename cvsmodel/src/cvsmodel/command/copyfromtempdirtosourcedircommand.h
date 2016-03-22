#ifndef COPYFROMTEMPDIRTOSOURCEDIRCOMMAND_H
#define COPYFROMTEMPDIRTOSOURCEDIRCOMMAND_H

#include "commandbase.h"

class CopyFromTempDirToSourceDirCommand : public CommandBase
{
    Q_OBJECT
public:
    explicit CopyFromTempDirToSourceDirCommand(QObject *parent = 0);

    virtual void execute(ObjectItems *allTargets);
};

#endif // COPYFROMTEMPDIRTOSOURCEDIRCOMMAND_H
