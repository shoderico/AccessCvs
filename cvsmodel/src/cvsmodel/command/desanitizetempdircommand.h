#ifndef DESANITIZETEMPDIRCOMMAND_H
#define DESANITIZETEMPDIRCOMMAND_H

#include "commandbase.h"

class DesanitizeTempDirCommand : public CommandBase
{
    Q_OBJECT
public:
    DesanitizeTempDirCommand(QAxObject *application, QObject *parent = 0);

    virtual void execute(ObjectItems *allTargets);
};

#endif // DESANITIZETEMPDIRCOMMAND_H
