#include "gitmanager.h"

#include "model/projectsetting.h"
#include "util/fileutil.h"
#include "qgit2.h"

#include <QMessageBox>
#include <QPointer>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>


GitManager::GitManager(Access::Application *application, QObject *parent)
    : QObject(parent)
    , m_application(application)
{
}

void GitManager::init()
{
    ProjectSetting setting(this);
    setting.initialize(m_application);
    if (!setting.isProjectOpened())
    {
        QMessageBox::information(0, tr(""), tr("no project is opened!"));
        return;
    }

    if ( QDir( setting.projectPath() + "\\.git" ).exists() )
    {
        QMessageBox::information(0, tr(""), tr("already initialized!"));
        return;
    }

    // git init setting.projectPath()
    QString repoPath = setting.projectPath();
    QPointer<LibQGit2::Repository> repo;
    repo = new LibQGit2::Repository();
    bool succeed = false;
    try
    {
        repo->init(repoPath, false);
        succeed = true;
    }
    catch (const LibQGit2::Exception& ex)
    {
        QMessageBox::warning(0, tr(""), ex.what());
    }

    if (succeed)
    {
        QMessageBox::information(0, tr(""), tr("git init succeed!"));
    }
}

void GitManager::gitIgnore()
{
    // create or update .gitignore

    QStringList toBeIgnored;
    toBeIgnored << ".accesscvs";
    toBeIgnored << "*.mdb";
    toBeIgnored << "*.ldb";
    toBeIgnored << "*.accdb";
    toBeIgnored << "*.laccdb";
    toBeIgnored << "*.adp";

    ProjectSetting setting(this);
    setting.initialize(m_application);
    if (!setting.isProjectOpened())
    {
        QMessageBox::information(0, tr(""), tr("no project is opened!"));
        return;
    }

    QFile file( setting.projectPath() + "\\.gitignore" );
    if (file.exists())
    {
        file.open( QIODevice::ReadOnly );
        QTextStream stream( &file );
        QTextCodec *codec = stream.codec();
        bool bom = stream.generateByteOrderMark();
        QString allContents = stream.readAll();
        file.close();

        QString endOfLine = "\n";
        if ( allContents.contains("\r\n") )
            endOfLine = "\r\n";
        QStringList contents = allContents.split( endOfLine );

        foreach (const QString &ignore, toBeIgnored )
        {
            if ( !contents.contains( ignore ))
                contents.append( ignore );
        }

        QFile fileDst( file.fileName() );
        fileDst.open( QIODevice::WriteOnly );
        QTextStream streamDst( &fileDst );
        streamDst.setCodec( codec );
        streamDst.setGenerateByteOrderMark( bom );

        streamDst << contents.join( endOfLine );
        fileDst.close();
        QMessageBox::information(0, tr(""), tr(".gitignore updated succeessfully!"));
    }
    else
    {
        file.open( QIODevice::WriteOnly );
        QTextStream stream( &file );
        stream.setCodec( QTextCodec::codecForName("UTF-8") );
        stream.setGenerateByteOrderMark( false );

        foreach( const QString &ignore, toBeIgnored )
            stream << ignore << "\r\n";

        file.close();
        QMessageBox::information(0, tr(""), tr(".gitignore created succeessfully!"));
    }
}

bool GitManager::isSupportedSsh() const
{
    return (git_libgit2_features() & GIT_FEATURE_SSH);
}

