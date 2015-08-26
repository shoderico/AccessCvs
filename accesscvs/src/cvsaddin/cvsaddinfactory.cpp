#include "cvsaddinfactory.h"

#include "cvsaddinimpl.h"

static const char LibraryID[]     = "{27e3bd9e-2ee3-41ba-a69d-61f510fda820}";
static const char ApplicationID[] = "{18bf0f9a-c557-4324-b5d8-f4077561a87e}";
static const char ClassID[]       = "{85842016-1eb7-4e60-ae2d-a473360251a8}";
static const char InterfaceID[]   = "{f3341da4-35a3-4b45-af98-ca5dac0930c4}";
static const char EventsID[]      = "{4296ce0f-1bd1-495e-ae1a-f7f10f5ccbee}";
static const char ClassName[]     = "AddInMain";

CvsAddInFactory::CvsAddInFactory(const QUuid &app, const QUuid &lib)
    : AddInFactory(app, lib)
{
    setClassId( ClassID );
    setInterfaceId( InterfaceID );
    setEventsId( EventsID );
    setClassName( ClassName );

    // FIXME: proper friendlyName and description
    setFriendlyName( tr("FriendlyName") );
    setDescription( tr("Description"));
}

QAxAggregated *CvsAddInFactory::createAggregate(QObject *parent)
{
    return new CvsAddInImpl(parent);
}

QAXFACTORY_EXPORT(CvsAddInFactory, LibraryID, ApplicationID)
