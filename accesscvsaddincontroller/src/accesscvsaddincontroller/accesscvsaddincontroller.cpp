#include "accesscvsaddincontroller.h"

#include "cvsmodel/accessobjectmodel.h"
#include "cvsmodel/accessobjectproxymodel.h"
#include "cvsaddincontroller/view/maindialog.h"

#include "pch.hpp"

AccessCvsAddinController::AccessCvsAddinController(QObject *parent)
    : CvsAddinController(parent)
{

}

void AccessCvsAddinController::init()
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

    CvsAddinController::init();
}

