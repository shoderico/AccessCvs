#include "accesscvscontroller.h"

#include "cvsmodel/accessobjectmodel.h"
#include "cvsmodel/accessobjectproxymodel.h"
#include "view/maindialog.h"

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

    if (!m_mainDlg)
    {
        m_mainDlg = new MainDialog( m_model, m_proxyModel, m_parentWidget );
    }

    CvsController::init();
}

