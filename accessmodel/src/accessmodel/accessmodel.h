#ifndef ACCESSMODEL_H
#define ACCESSMODEL_H

#include "accessmodel_global.h"

#include <QObject>

namespace Access {
class Application;
} // namespace Access

class ACCESSMODEL_SHARED_EXPORT AccModel
{

public:
    AccModel(){}

    Access::Application *createAccessApplication(QObject *parent = 0);
    void quitAndDeleteApplication(Access::Application *application);

    quint64 getAccessThreadId(Access::Application *application);

    bool decompile(const QString &fileName, quint64 threadIdForAttachInput);
    bool compactRepair(Access::Application *application, const QString &fileName, const int repeatCount = 1);
    bool openCurrentDatabase(Access::Application *application, const QString &fileName);

    // TODO: clearConnection() for ADP
    // TODO: removeLinkTables() for MDB
    // TODO: clearWorkTables() for MDB
};

#endif // ACCESSMODEL_H
