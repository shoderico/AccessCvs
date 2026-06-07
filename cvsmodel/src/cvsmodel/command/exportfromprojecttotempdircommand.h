#ifndef EXPORTFROMPROJECTTOTEMPDIRCOMMAND_H
#define EXPORTFROMPROJECTTOTEMPDIRCOMMAND_H

#include "commandbase.h"

class ExportFromProjectToTempDirCommand : public CommandBase
{
    Q_OBJECT
public:
    explicit ExportFromProjectToTempDirCommand(ProjectContainer *project, QAxObject *application, QObject *parent = 0);

    virtual void execute(ObjectItemMap *allTargets);
};

#endif // EXPORTFROMPROJECTTOTEMPDIRCOMMAND_H
