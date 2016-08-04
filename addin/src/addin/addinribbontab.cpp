#include "addinribbontab.h"

#include "comutil/comutil.h"

#include "addinfactory.h"
#include "addinutil/addincontroller.h"
#include "windowwidgetmanager.h"

#include <QDebug>

#include "pch.hpp"

AddInRibbonTab::AddInRibbonTab(AddInFactory *factory, QObject *parent)
    : QObject(parent)
    , m_winWidgetManager(0)
    , m_factory(factory)
{

}


void AddInRibbonTab::appendController(AddInController *controller)
{
    m_controllers.append( controller );
}

void AddInRibbonTab::setRibbonTabId(const QString &tabId)
{
    m_ribbonTabId = tabId;
}

void AddInRibbonTab::setRibbonTabLabel(const QString &tabLabel)
{
    m_ribbonTabLabel = tabLabel;
}

void AddInRibbonTab::onConnectionEvent()
{
    m_winWidgetManager = new WindowWidgetManager(m_factory->applicationHwnd(), this);

    foreach (AddInController *c, m_controllers)
        c->initialize(m_factory->application(), m_winWidgetManager->widget());
}

void AddInRibbonTab::onDisconnectionEvent()
{
    //qDeleteAll(m_controllers); // controller is owned by multiple ribbonTabs.
    m_controllers.clear();

    if (m_winWidgetManager)
    {
        delete m_winWidgetManager;
        m_winWidgetManager = 0;
    }
}

QString AddInRibbonTab::ribbomXml()
{
    QString content;
    content += QString("<tab id=\"%1\" label=\"%2\">").arg(m_ribbonTabId).arg(m_ribbonTabLabel);

    foreach (AddInController *c, m_controllers)
        content += c->ribbonXml();

    content += "</tab>";
    qDebug() << content;

    return content;
}

IPictureDisp *AddInRibbonTab::buttonImage(const QString &controlId)
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

bool AddInRibbonTab::onButtonClicked(const QString &controlId)
{

    foreach (AddInController *c, m_controllers)
        if ( c->handleButtonClick(controlId) )
            return true;

    return false;
}

