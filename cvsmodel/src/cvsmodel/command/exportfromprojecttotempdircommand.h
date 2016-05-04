#ifndef EXPORTFROMPROJECTTOTEMPDIRCOMMAND_H
#define EXPORTFROMPROJECTTOTEMPDIRCOMMAND_H

#include "commandbase.h"

class ExportFromProjectToTempDirCommand : public CommandBase
{
    Q_OBJECT
public:
    explicit ExportFromProjectToTempDirCommand(QAxObject *application, QObject *parent = 0);

    virtual void execute(ObjectItems *allTargets);
};

#endif // EXPORTFROMPROJECTTOTEMPDIRCOMMAND_H
