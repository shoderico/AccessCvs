#include "excelobjectmodel.h"

#include "excellib/excellib.h"
#include "util/comptr.h"

#include "excelprojectcontainer.h"

#include <QDebug>

ExcelObjectModel::ExcelObjectModel(QObject *parent)
    : ObjectModel(parent)
{
    QList<Model::ObjectType> objectTypes;
//    objectTypes
//            << Model::TableDef
//            << Model::Query
//            << Model::Form
//            << Model::Report
//            << Model::Macro
//            << Model::Module
//           ;

    ProjectContainer *project = new ExcelProjectContainer(this);

    init(objectTypes, project);
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

