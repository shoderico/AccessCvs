#include "excelcvscontroller.h"

#include "cvsmodel/excelobjectmodel.h"
#include "cvsmodel/excelobjectproxymodel.h"

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

    CvsController::init();
}

