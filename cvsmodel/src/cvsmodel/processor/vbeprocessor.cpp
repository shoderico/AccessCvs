#include "vbeprocessor.h"

#include <QDebug>

#include "officelib/officelib.h"

#include "util/comptr.h"
#include "cvsmodel/projectcontainer.h"

#include <QUuid>
#include <windows.h>

VBEProcessor::VBEProcessor(ProjectContainer *parent)
    : ProjectLevelObjectProcessor(parent)
{
}

VBIDE::VBProject *VBEProcessor::currentVBProject()
{
    QString fileName = m_projectContainer->currentProjectFullName();

    // retreive VBE object dynamically
    QAxObject *vbePtr = m_projectContainer->application()->querySubObject("VBE()");
    if ( !vbePtr )
    {
        qDebug() << "VBEProcessor::currentVBProject : vbePtr is null";
        return NULL;
    }
    // retreive IDispatch from pointer
    IDispatch *idisp = 0;
    vbePtr->queryInterface( QUuid(IID_IDispatch), (void**)&idisp);
    if ( !idisp )
    {
        qDebug() << "VBEProcessor::currentVBProject : idisp is null";
        return NULL;
    }
    // and create real object using IDispatch
    ComPtr<VBIDE::VBE> vbe = new VBIDE::VBE(idisp);
    if ( !vbe.is() )
    {
        qDebug() << "VBEProcessor::currentVBProject : vbe is null";
        return NULL;
    }

    //

    ComPtr<VBIDE::VBProjects> vbProjects = vbe->VBProjects();
    if ( !vbProjects.is() )
    {
        qDebug() << "VBEProcessor::currentVBProject : vbProjects is null";
        return NULL;
    }

    for ( int i = 1 ; i <= vbProjects->Count(); ++i )
    {
        ComPtr<VBIDE::VBProject> vbp = vbProjects->Item(i);
        if ( !vbp.is() )
        {
            qDebug() << "VBEProcessor::currentVBProject : vbp is null";
            return NULL;
        }
        if (vbp->FileName() == fileName)
        {
            return vbProjects->Item(i);
        }
    }
    return NULL;
}

