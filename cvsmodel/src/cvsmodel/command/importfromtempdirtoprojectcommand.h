#ifndef IMPORTFROMTEMPDIRTOPROJECTCOMMAND_H
#define IMPORTFROMTEMPDIRTOPROJECTCOMMAND_H

#include "commandbase.h"

class ImportFromTempDirToProjectCommand : public CommandBase
{
    Q_OBJECT
public:
    explicit ImportFromTempDirToProjectCommand(ProjectContainer *project, QAxObject *application, QObject *parent = 0);

    virtual void execute(ObjectItemMap *allTargets);
};

#endif // IMPORTFROMTEMPDIRTOPROJECTCOMMAND_H
