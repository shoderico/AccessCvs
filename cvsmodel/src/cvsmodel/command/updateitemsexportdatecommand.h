#ifndef UPDATEITEMSEXPORTDATECOMMAND_H
#define UPDATEITEMSEXPORTDATECOMMAND_H

#include "commandbase.h"

#include <QDateTime>

class UpdateItemsExportDateCommand : public CommandBase
{
    Q_OBJECT
public:
    explicit UpdateItemsExportDateCommand(const QDateTime &exportDate, const int differenceTypes, ProjectContainer *project, QAxObject *application, QList<ObjectItem*> *items, QObject *parent = 0);

    virtual void execute(ObjectItemMap *allTargets);

    void setExportDate(const QDateTime &exportDate);
    void setDifferenceTypes(const int differenceTypes);

private:
    QDateTime m_exportDate;
    int m_differenceTypes;
};

#endif // UPDATEITEMSEXPORTDATECOMMAND_H
