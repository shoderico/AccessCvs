#include "excelprojectcontainer.h"

#include <QDebug>

#include "excellib/excellib.h"
#include "util/comptr.h"

#include "processor/referenceprocessor.h"
#include "processor/vbprojectprocessor.h"

ExcelProjectContainer::ExcelProjectContainer(QObject *parent)
    : ProjectContainer(parent)
{
    m_DefaultTempPathName     = ".excelcvs";

    m_objectProcessors.insert( Model::Reference,  new ReferenceProcessor(this) );
    m_objectProcessors.insert( Model::VBProject,  new VBProjectProcessor(this) );
}

void ExcelProjectContainer::initialize(QAxObject *application)
{
    m_application = static_cast<Excel::Application*>(application);

    m_currentProjectName = "";
    m_currentProjectPath = "";
    m_currentProjectFullName = "";
    m_fileFormat = -1;

    ComPtr<Excel::Workbook> workbook = m_application->ActiveWorkbook();

    if (workbook.is())
    {
        m_currentProjectName        = workbook->Name();
        m_currentProjectPath        = workbook->Path();
        m_currentProjectFullName    = workbook->FullName();
        m_fileFormat                = workbook->FileFormat();
    }

    ProjectContainer::initialize(application);
}

bool ExcelProjectContainer::isProjectOpened() const
{
    return m_fileFormat != -1;
}

