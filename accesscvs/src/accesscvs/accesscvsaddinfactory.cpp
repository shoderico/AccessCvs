#include "accesscvsaddinfactory.h"

#include "util/logfile.h"
#include "addin/addinaggregated.h"
#include "addin/addinribbontab.h"
#include "accesslib/accesslib.h"

#include "accesscvsaddincontroller/accesscvsaddincontroller.h"
#include "accessaddincontroller/accessaddincontroller.h"
#include "helpaddincontroller/helpaddincontroller.h"

#include "resource/resource.h"

#include <QDebug>

#include "pch.hpp"
#if defined(_DEBUG) && defined(_MSC_VER) && defined(DETECT_MEMORY_LEAKS)

// http://stackoverflow.com/questions/6825376/how-to-detect-memory-leaks-in-qtcreator-on-windows

_CRT_REPORT_HOOK prevHook;

int customReportHook(int /* reportType */, char* message, int* /* returnValue */) {
  // This function is called several times for each memory leak.
  // Each time a part of the error message is supplied.
  // This holds number of subsequent detail messages after
  // a leak was reported
  const int numFollowupDebugMsgParts = 2;
  static bool ignoreMessage = false;
  static int debugMsgPartsCount = 0;

  // check if the memory leak reporting starts
  if ((strncmp(message,"Detected memory leaks!\n", 10) == 0)
    || ignoreMessage)
  {
    // check if the memory leak reporting ends
    if (strncmp(message,"Object dump complete.\n", 10) == 0)
    {
      _CrtSetReportHook(prevHook);
      ignoreMessage = false;
    } else
      ignoreMessage = true;

    // something from our own code?
    if(strstr(message, ".cpp") == NULL)
    {
      if(debugMsgPartsCount++ < numFollowupDebugMsgParts)
        // give it back to _CrtDbgReport() to be printed to the console
        return FALSE;
      else
        return TRUE;  // ignore it
    } else
    {
      debugMsgPartsCount = 0;
      // give it back to _CrtDbgReport() to be printed to the console
      return FALSE;
    }
  } else
    // give it back to _CrtDbgReport() to be printed to the console
    return FALSE;
}
#endif

static const char LibraryID[]     = "{27e3bd9e-2ee3-41ba-a69d-61f510fda820}";
static const char ApplicationID[] = "{18bf0f9a-c557-4324-b5d8-f4077561a87e}";
static const char ClassID[]       = "{85842016-1eb7-4e60-ae2d-a473360251a8}";
static const char InterfaceID[]   = "{f3341da4-35a3-4b45-af98-ca5dac0930c4}";
static const char EventsID[]      = "{4296ce0f-1bd1-495e-ae1a-f7f10f5ccbee}";
static const char ClassName[]     = "AddInMain";

AccessCvsAddInFactory::AccessCvsAddInFactory(const QUuid &app, const QUuid &lib)
    : AddInFactory(app, lib)
    , m_application(0)
{

#if defined(_DEBUG) && defined(_MSC_VER) && defined(DETECT_MEMORY_LEAKS)
    _CrtSetDbgFlag( _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG )| _CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
    prevHook = _CrtSetReportHook(customReportHook);
#endif

    setRegistryRoot( QLatin1String("HKEY_CURRENT_USER\\Software") );
    setRegistryPath( QLatin1String("\\Microsoft\\Office\\Access\\Addins") );

    setClassId( ClassID );
    setInterfaceId( InterfaceID );
    setEventsId( EventsID );
    setClassName( ClassName );

    // FIXME: proper friendlyName and description
    setLoadBehavior( 3 );
    setCommandLineSafe( 0 );
    setFriendlyName( tr("FriendlyName") );
    setDescription( tr("Description"));

    // log qDebug() output
    (void)new LogFile( serverDirPath() + "\\log", "log_access_", false );
    qInstallMessageHandler(LogFile::MessageOutput);
}

AccessCvsAddInFactory::~AccessCvsAddInFactory()
{
    delete LogFile::instance();
}

QAxAggregated *AccessCvsAddInFactory::createAggregate(QObject *parent)
{
    // aggregated
    AddInAggregated *aggregated = new AddInAggregated(this, parent);
    aggregated->loadTypeLib( QAxFactory::serverFilePath() );

    // controllers
    AccessCvsAddinController *accessCvsAddinController = new AccessCvsAddinController(aggregated);
    AccessAddinController *accessAddinController = new AccessAddinController(aggregated);
    HelpAddinController *helpAddinController = new HelpAddinController(aggregated);

    // ribbon tabs
    AddInRibbonTab *addInRibbonTab = new AddInRibbonTab(this, aggregated);
    addInRibbonTab->setRibbonTabId("AccessCvs");
    addInRibbonTab->setRibbonTabLabel("AccessCvs");
    addInRibbonTab->appendController( accessCvsAddinController );
    addInRibbonTab->appendController( accessAddinController );
    addInRibbonTab->appendController( helpAddinController );
    aggregated->appendRibbonTab(addInRibbonTab);
    m_addInRibbonTab = addInRibbonTab;

    // test code for 1 controller owned by multiple tabs.
    AddInRibbonTab *addInRibbonTab2 = new AddInRibbonTab(this, aggregated);
    addInRibbonTab2->setRibbonTabId("AccessCvs2");
    addInRibbonTab2->setRibbonTabLabel("AccessCvs");
    addInRibbonTab2->appendController( helpAddinController );
    aggregated->appendRibbonTab(addInRibbonTab2);

    return aggregated;
}

void AccessCvsAddInFactory::setApplication(IDispatch *application)
{
    Access::_Application *_application = new Access::_Application(application/*, this*/);
    m_application = new Access::Application(_application);
}

void AccessCvsAddInFactory::releaseApplication()
{
    if (m_application)
    {
        delete m_application;
        m_application = NULL;
    }
}

QAxObject *AccessCvsAddInFactory::application() const
{
    return m_application;
}

int AccessCvsAddInFactory::applicationHwnd()
{
    return m_application->hWndAccessApp();
}

void AccessCvsAddInFactory::onBeforeConnectionEvent()
{
    Resource res;
    res.init();
}

void AccessCvsAddInFactory::onAfterConnectionEvent()
{
    // show manual dialog if ribbon ui is not supported
    QString accessVer = m_application->SysCmd( Access::acSysCmdAccessVer ).toString();
    if ( accessVer < "12.0" )
    {
        m_addInRibbonTab->onButtonClicked( QString("StandardManualButton") );
    }
}

void AccessCvsAddInFactory::onAfterDisconnectionEvent()
{
    Resource res;
    res.cleanup();
}

// onAddInImplConnection : init Resource

// onAddInImplDisconnection : release Resource

QAXFACTORY_EXPORT(AccessCvsAddInFactory, LibraryID, ApplicationID)
