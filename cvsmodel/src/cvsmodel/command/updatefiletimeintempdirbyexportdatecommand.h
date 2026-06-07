#ifndef UPDATEFILETIMEINTEMPDIRBYEXPORTDATECOMMAND_H
#define UPDATEFILETIMEINTEMPDIRBYEXPORTDATECOMMAND_H

#include "commandbase.h"

class UpdateFileTimeInTempDirByExportDateCommand : public CommandBase
{
    Q_OBJECT
public:
    explicit UpdateFileTimeInTempDirByExportDateCommand(const int differenceTypes, ProjectContainer *project, QAxObject *application, QList<ObjectItem*> *items, QObject *parent = 0);

    virtual void execute(ObjectItemMap *allTargets);

    void setDifferenceTypes(const int differenceTypes);

private:
    int m_differenceTypes;
};

#endif // UPDATEFILETIMEINTEMPDIRBYEXPORTDATECOMMAND_H
