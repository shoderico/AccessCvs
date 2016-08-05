#include "excelcvsaddinfactory.h"

#include "util/logfile.h"
#include "addin/addinaggregated.h"
#include "addin/addinribbontab.h"
#include "addin/windowwidgetmanager.h"
#include "excellib/excellib.h"

#include "excelcvsaddincontroller/excelcvsaddincontroller.h"
//#include "accesscontroller/accesscontroller.h"
#include "helpaddincontroller/helpaddincontroller.h"

#include "resource/resource.h"

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
    (void)new LogFile( serverDirPath() + "\\log", "log_excel_", false, this );
    qInstallMessageHandler(LogFile::MessageOutput);
}

ExcelCvsAddInFactory::~ExcelCvsAddInFactory()
{
    // delete by QObject
    // delete LogFile::instance();
}

QAxAggregated *ExcelCvsAddInFactory::createAggregate(QObject *parent)
{
    // aggregated
    AddInAggregated *aggregated = new AddInAggregated(parent);
    aggregated->loadTypeLib( QAxFactory::serverFilePath() );
    connect( aggregated, SIGNAL(addInConnection(IDispatch*)), this, SLOT(onAddInConnection(IDispatch*)) );
    connect( aggregated, SIGNAL(addInDisconnection()),        this, SLOT(onAddInDisconnection()) );

    // controllers
    ExcelCvsAddinController *excelCvsAddinController = new ExcelCvsAddinController(aggregated);
    HelpAddinController *helpAddinController = new HelpAddinController(aggregated);
    connect( this, SIGNAL(addInConnection(QAxObject*,QWidget*)), excelCvsAddinController, SLOT(initialize(QAxObject*,QWidget*)) );
    connect( this, SIGNAL(addInConnection(QAxObject*,QWidget*)), helpAddinController,     SLOT(initialize(QAxObject*,QWidget*)) );

    // ribbon tabs
    AddInRibbonTab *addInRibbonTab = new AddInRibbonTab(aggregated);
    addInRibbonTab->setRibbonTabId("ExcelCvs");
    addInRibbonTab->setRibbonTabLabel("ExcelCvs");
    addInRibbonTab->appendController( excelCvsAddinController );
    addInRibbonTab->appendController( helpAddinController );
    aggregated->appendRibbonTab(addInRibbonTab);


    return aggregated;
}

QAxObject *ExcelCvsAddInFactory::application() const
{
    return m_application;
}

void ExcelCvsAddInFactory::onAddInConnection(IDispatch *application)
{
    // create application wrapper
    Excel::_Application *_application = new Excel::_Application(application/*, this*/);
    m_application = new Excel::Application(_application);

    // initialize resource
    Resource res;
    res.init();

    // create root widget
    m_winWidgetManager = new WindowWidgetManager(m_application->Hwnd(), this);

    // initialize all controllers
    emit addInConnection(m_application, m_winWidgetManager->widget());
}

void ExcelCvsAddInFactory::onAddInDisconnection()
{
    // finalize all controllers

    // delete root widget
    delete m_winWidgetManager;
    m_winWidgetManager = 0;

    // cleanup resource
    Resource res;
    res.cleanup();

    // release application
    if (m_application)
    {
        delete m_application;
        m_application = NULL;
    }
}

QAXFACTORY_EXPORT(ExcelCvsAddInFactory, LibraryID, ApplicationID)
