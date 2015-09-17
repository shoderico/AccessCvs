#include "addincontrollerimpl.h"

#include "comutil/comutil.h"

#include "addinutil/addincontroller.h"
#include "windowwidgetmanager.h"

AddInControllerImpl::AddInControllerImpl(AddInFactory *factory, QObject *parent)
    : AddInImpl(factory, parent)
    , m_winWidgetManager(0)
{

}


void AddInControllerImpl::appendController(AddInController *controller)
{
    m_controllers.append( controller );
}

void AddInControllerImpl::onConnectionEvent()
{
    m_winWidgetManager = new WindowWidgetManager(application(), this);

    foreach (AddInController *c, m_controllers)
        c->initialize(application(), m_winWidgetManager->widget());
}

void AddInControllerImpl::onDisconnectionEvent()
{
    qDeleteAll(m_controllers);
    m_controllers.clear();

    if (m_winWidgetManager)
    {
        delete m_winWidgetManager;
        m_winWidgetManager = 0;
    }
}

QString AddInControllerImpl::ribbomXml()
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

IPictureDisp *AddInControllerImpl::buttonImage(const QString &controlId)
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

HRESULT AddInControllerImpl::onButtonClicked(const QString &controlId)
{

    foreach (AddInController *c, m_controllers)
        if ( c->handleButtonClick(controlId) )
            break;

    return S_OK;
}

