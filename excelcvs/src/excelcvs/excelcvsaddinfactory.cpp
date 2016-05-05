#include "excelcvsaddinfactory.h"

#include "util/logfile.h"
#include "addin/addincontrollerimpl.h"
#include "excellib/excellib.h"

#include "cvscontroller/excelcvscontroller.h"
//#include "acccontroller/acccontroller.h"
#include "helpcontroller/helpcontroller.h"

static const char LibraryID[]     = "{91A0CABD-B244-47D0-B23F-9CA3A55FE638}";
static const char ApplicationID[] = "{AC2609B7-EF6C-4A93-A5C2-1226C0A2D422}";
static const char ClassID[]       = "{9025340A-966D-444E-818C-114FC0D5E981}";
static const char InterfaceID[]   = "{03237AC1-68D2-42CF-BDD7-C0FCD15400D6}";
static const char EventsID[]      = "{A9B2EA45-722D-4E55-8897-5ED67ADD6D62}";
static const char ClassName[]     = "ExcelAddInMain";

ExcelCvsAddInFactory::ExcelCvsAddInFactory(const QUuid &app, const QUuid &lib)
    : AddInFactory(app, lib)
    , m_application(0)
{
    setRegistryRoot( QLatin1String("HKEY_CURRENT_USER\\Software") );
    setRegistryPath( QLatin1String("\\Microsoft\\Office\\Excel\\Addins") );

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
    (void)new LogFile( serverDirPath() + "\\log", "log_excel_", false );
    qInstallMessageHandler(LogFile::MessageOutput);
}

ExcelCvsAddInFactory::~ExcelCvsAddInFactory()
{
    delete LogFile::instance();
}

QAxAggregated *ExcelCvsAddInFactory::createAggregate(QObject *parent)
{
    AddInControllerImpl *addInImpl = new AddInControllerImpl(this, parent);
    addInImpl->setRibbonTabId("ExcelCvs");
    addInImpl->setRibbonTabLabel("ExcelCvs");
    addInImpl->appendController( new ExcelCvsController(this) );
//    addInImpl->appendController( new AccController(this) );
    addInImpl->appendController( new HelpController(this) );
    return addInImpl;
}

void ExcelCvsAddInFactory::setApplication(IDispatch *application)
{
    Excel::_Application *_application = new Excel::_Application(application/*, this*/);
    m_application = new Excel::Application(_application);
}

void ExcelCvsAddInFactory::releaseApplication()
{
    if (m_application)
    {
        delete m_application;
        m_application = NULL;
    }
}

QAxObject *ExcelCvsAddInFactory::application() const
{
    return m_application;
}

int ExcelCvsAddInFactory::applicationHwnd()
{
    return m_application->Hwnd();
}

void ExcelCvsAddInFactory::onBeforeConnectionEvent()
{
    Q_INIT_RESOURCE(resource);
}

void ExcelCvsAddInFactory::onAfterDisconnectionEvent()
{
    Q_CLEANUP_RESOURCE(resource);
}

// onAddInImplConnection : init Resource

// onAddInImplDisconnection : release Resource

QAXFACTORY_EXPORT(ExcelCvsAddInFactory, LibraryID, ApplicationID)
