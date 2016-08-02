#include "cvsaddinimpl.h"

//#include <QResource>
//#include <QFile>
//#include <QTextStream>

#include "comutil/comutil.h"
#include "addin/addincontroller.h"

#include "managers/windowwidgetmanager.h"

#include "cvscontroller/cvscontroller.h"
#include "gitcontroller/gitcontroller.h"
#include "accessutilcontroller/accessutilcontroller.h"

CvsAddInImpl::CvsAddInImpl(AddInFactory *factory, QObject *parent)
    : AddInImpl(factory, parent)
    , m_winWidgetManager(0)
{
}

void CvsAddInImpl::appendController(AddInController *controller)
{
    m_controllers.append( controller );
}

void CvsAddInImpl::onConnectionEvent()
{
    Q_INIT_RESOURCE(resource);

    m_winWidgetManager = new WindowWidgetManager(application(), this);

    m_controllers.append( new CvsController(this) );
    m_controllers.append( new GitController(this) );
    m_controllers.append( new AccessUtilController(this) );

    foreach (AddInController *c, m_controllers)
        c->initialize(application(), m_winWidgetManager->widget());
}

void CvsAddInImpl::onDisconnectionEvent()
{
    qDeleteAll(m_controllers);
    m_controllers.clear();

    if (m_winWidgetManager)
    {
        delete m_winWidgetManager;
        m_winWidgetManager = 0;
    }

    Q_CLEANUP_RESOURCE(resource);
}

QString CvsAddInImpl::ribbomXml()
{
    QString content;
    content += "<customUI xmlns=\"http://schemas.microsoft.com/office/2006/01/customui\">";
    content += "<ribbon>";
    content += "<tabs>";
    content += "<tab id=\"AccessCvs\" label=\"AccessCvs\">";

    foreach (AddInController *c, m_controllers)
        content += c->ribbonXml();

    content += "</tab></tabs></ribbon></customUI>";

    return content;
}

IPictureDisp *CvsAddInImpl::buttonImage(const QString &controlId)
{
    // determine icon image path and size
    QString imagePath = "";
    QSize size(16,16);

    foreach (AddInController *c, m_controllers)
        if( c->imagePath(controlId, imagePath, size) )
            break;

    if ( imagePath.isEmpty() )
        return NULL;

    // load picutre
    return ComUtil::loadPictureFromSvg( imagePath, size );
}

HRESULT CvsAddInImpl::onButtonClicked(const QString &controlId)
{

    foreach (AddInController *c, m_controllers)
        if ( c->handleButtonClick(controlId) )
            break;

    return S_OK;
}

