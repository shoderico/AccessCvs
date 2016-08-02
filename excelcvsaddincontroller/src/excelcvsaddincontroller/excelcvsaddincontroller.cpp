#include "excelcvsaddincontroller.h"

#include "cvsmodel/excelobjectmodel.h"
#include "cvsmodel/excelobjectproxymodel.h"
#include "cvsaddincontroller/view/maindialog.h"

#include "pch.hpp"

ExcelCvsAddinController::ExcelCvsAddinController(QObject *parent)
    : CvsAddinController(parent)
{

}

void ExcelCvsAddinController::init()
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

    CvsAddinController::init();
}

