#ifndef SANITIZETEMPDIRCOMMAND_H
#define SANITIZETEMPDIRCOMMAND_H

#include "commandbase.h"

class SanitizeTempDirCommand : public CommandBase
{
    Q_OBJECT
public:
    explicit SanitizeTempDirCommand(QAxObject *application, QObject *parent = 0);

    virtual void execute(ObjectItemMap *allTargets);
};

#endif // SANITIZETEMPDIRCOMMAND_H
