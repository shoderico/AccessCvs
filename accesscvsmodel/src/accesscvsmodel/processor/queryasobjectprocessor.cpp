#include "queryasobjectprocessor.h"

#include <QDebug>

#include "accesslib/accesslib.h"

#include "cvsmodel/projectcontainer.h"

#include "pch.hpp"


//=============================================================================
// QueryAsObjectProcessor
// NOTE: this class is export/import by SaveAsText/LoadFromText.
//       this class is implemented to solve the problem that
//       "join-on criterias are always different between import and export".
//       for example,
//           if   import as 'SELECT ... FROM A INNER JOIN B ON ( A.x = B.x ) AND (A.y = B.y)' ... statement(N)
//           then export as 'SELECT ... FROM A INNER JOIN B ON ( A.y = B.y ) AND (A.x = B.x)' ... statement(M)
//               ( swapped the criterias after ON )
//           and then import as (M) again, then export as (N)...
//        this problem cannot be solved simply.
//        we set it as Known Issue.
//
QueryAsObjectProcessor::QueryAsObjectProcessor(ProjectContainer *parent)
    : QueryAsSqlProcessor(parent)
{
}

bool QueryAsObjectProcessor::exportFromProjectToTempDir(QAxObject *object, const QString &objectName)
{
    deleteAllFileFromTempDir(objectName);

    Q_UNUSED(object)
    {
        m_projectContainer->application<Access::Application>()->SaveAsText( (Access::AcObjectType)m_accessObjectType, objectName, filePath(TempDir, TempFile, objectName) );
        return true;
    }
}

bool QueryAsObjectProcessor::importFromTempDirToProject(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    {
        m_projectContainer->application<Access::Application>()->LoadFromText( (Access::AcObjectType)m_accessObjectType, objectName, filePath(TempDir, TempFile, objectName) );

        return true;
    }
    return true;
}
