#ifndef UPDATEFILETIMEINTEMPDIRCOMMAND_H
#define UPDATEFILETIMEINTEMPDIRCOMMAND_H

#include "commandbase.h"

#include <QDateTime>

class UpdateFileTimeInTempDirCommand : public CommandBase
{
    Q_OBJECT
public:
    explicit UpdateFileTimeInTempDirCommand(const QDateTime &fileTime, const int differenceTypes, ProjectContainer *project, QAxObject *application, QObject *parent = 0);

    virtual void execute(ObjectItemMap *allTargets);

private:
    QDateTime m_fileTime;
    int m_differenceTypes;
};

#endif // UPDATEFILETIMEINTEMPDIRCOMMAND_H
