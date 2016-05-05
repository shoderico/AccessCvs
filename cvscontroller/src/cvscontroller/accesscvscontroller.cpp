#include "accesscvscontroller.h"

#include "cvsmodel/accessobjectmodel.h"
#include "cvsmodel/accessobjectproxymodel.h"

AccessCvsController::AccessCvsController(QObject *parent)
    : CvsController(parent)
{

}

void AccessCvsController::init()
{
    if (!m_model)
    {
        m_model = new AccessObjectModel(this);
        m_model->setApplication(m_application);
    }

    if (!m_proxyModel)
    {
        m_proxyModel = new AccessObjectProxyModel(this);
        m_proxyModel->setSourceModel(m_model);
    }

    CvsController::init();
}

