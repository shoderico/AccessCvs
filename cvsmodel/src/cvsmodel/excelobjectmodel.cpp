#include "excelobjectmodel.h"

#include "excellib/excellib.h"
#include "util/comptr.h"

#include "excelprojectcontainer.h"

#include <QDebug>

ExcelObjectModel::ExcelObjectModel(QObject *parent)
    : ObjectModel(parent)
{
    ProjectContainer *projectContainer = new ExcelProjectContainer(this);

    init(projectContainer);
}

void ExcelObjectModel::setApplication(QAxObject *application)
{
    ObjectModel::setApplication(application);
    m_application = static_cast<Excel::Application*>(application);
}

bool ExcelObjectModel::checkProjectState()
{
    ComPtr<Excel::Workbook> workbook  = m_application->ActiveWorkbook();
    if ( !workbook.is() )
    {
        qDebug() << "ActiveWorkbook is null";
        return false;
    }
    int projectType = workbook->FileFormat();
    // TODO : determine by projectType

    return true;
}

