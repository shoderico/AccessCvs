#include "excelprojectcontainer.h"

#include <QDebug>

#include "excellib/excellib.h"
#include "util/comptr.h"

#include "cvsmodel/processor/referenceprocessor.h"
#include "cvsmodel/processor/vbprojectprocessor.h"

#include "pch.hpp"

ExcelProjectContainer::ExcelProjectContainer(QObject *parent)
    : ProjectContainer(parent)
{
    m_DefaultTempPathName     = ".excelcvs";

    m_objectProcessorList.append( new ReferenceProcessor(this) );
    m_objectProcessorList.append( new VBProjectProcessor(this) );

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

