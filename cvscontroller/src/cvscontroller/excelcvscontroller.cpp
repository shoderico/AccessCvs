#include "excelcvscontroller.h"

#include "cvsmodel/excelobjectmodel.h"
#include "cvsmodel/excelobjectproxymodel.h"
#include "view/maindialog.h"

#include "pch.hpp"

ExcelCvsController::ExcelCvsController(QObject *parent)
    : CvsController(parent)
{

}

void ExcelCvsController::init()
{
    if (!m_model)
    {
        m_model = new ExcelObjectModel(this);
        m_model->setApplication(m_application);
    }

    if (!m_proxyModel)
    {
        m_proxyModel = new ExcelObjectProxyModel(this);
        m_proxyModel->setSourceModel(m_model);
    }

    if (!m_mainDlg)
    {
        m_mainDlg = new MainDialog( m_model, m_proxyModel, m_parentWidget );
    }

    CvsController::init();
}

