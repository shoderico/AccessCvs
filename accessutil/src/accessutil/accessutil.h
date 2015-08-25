#ifndef ACCESSUTIL_H
#define ACCESSUTIL_H

#include "accessutil_global.h"

namespace Access {
class Application;
} // namespace Access

class ACCESSUTILSHARED_EXPORT AccessUtil
{

public:
    AccessUtil(){}

    bool decompile(const QString &fileName, quint64 threadIdForAttachInput);
    bool compactRepair(const QString &fileName, const int repeatCount = 1);
    bool openCurrentDatabase(Access::Application *application, const QString &fileName);
    quint64 getAccessThreadId(Access::Application *application);

    // TODO: clearConnection() for ADP
    // TODO: removeLinkTables() for MDB
    // TODO: clearWorkTables() for MDB
};

#endif // ACCESSUTIL_H
