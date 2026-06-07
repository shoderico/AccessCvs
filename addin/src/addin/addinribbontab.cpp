#include "addinribbontab.h"

#include "comutil/comutil.h"

#include "addinfactory.h"
#include "addinutil/addincontroller.h"

#include <QDebug>

#include "pch.hpp"

AddInRibbonTab::AddInRibbonTab(QObject *parent)
    : QObject(parent)
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

