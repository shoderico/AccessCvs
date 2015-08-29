#include "gitcontroller.h"

#include "cvsmodel/projectsetting.h"
#include "util/fileutil.h"
#include "qgit2.h"
#include "addinutil/addinutil.h"

#include <QMessageBox>
#include <QPointer>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QRegularExpression>


GitController::GitController(QObject *parent)
    : QObject(parent)
    , m_application(0)
    , m_parentWidget(0)
{
}


void GitController::initialize(Access::Application *application, QWidget *parentWidget)
{
    m_application = application;
    m_parentWidget = parentWidget;
}


QString GitController::ribbonXml()
{
    QString content =
        "<group id=\"GitGroup\" "
            "label=\"Git\"> "
            "<button id=\"GitInitButton\" "
            "   size=\"normal\" "
            "   label=\"Initialize Repository\" "
            "   onAction=\"ButtonClicked\" "
            "   getImage=\"GetButtonImage\" "
            "   /> "
            "<button id=\"GitIgnoreButton\" "
            "  size=\"normal\" "
            "  label=\"Update .gitignore\" "
            "  onAction=\"ButtonClicked\" "
            "  getImage=\"GetButtonImage\" "
            "  /> "
        "</group>"
            ;
    return content;
//            <!--
//            <button id="GitManageRemotesButton"
//               size="normal"
//               label="Manage Remotes"
//               onAction="ButtonClicked"
//               getImage="GetButtonImage"
//               />
//           <button id="GitPullButton"
//              size="normal"
//              label="Pull"
//              onAction="ButtonClicked"
//              getImage="GetButtonImage"
//              />
//           -->
}

bool GitController::imagePath(const QString &controlId, QString &imagePath, QSize &size)
{
    if ( controlId == "GitInitButton")
    {
        imagePath = ":/images/git-init.svg";
        size = AddInUtil::ribbonIconSize(AddInUtil::Small);
    }
    else if ( controlId == "GitIgnoreButton")
    {
        imagePath = ":/images/git-update-gitignore.svg";
        size = AddInUtil::ribbonIconSize(AddInUtil::Small);
    }
    else
        return false;

    return true;
}

bool GitController::handleButtonClick(const QString &controlId)
{
    if (controlId == "GitInitButton")
        init();
    else if (controlId == "GitIgnoreButton")
        gitIgnore();
    else if (controlId == "GitManageRemotesButton")
        manageRemotes();
    else if (controlId == "GitPullButton")
        pull();
    else
        return false;
    return true;
}


void GitController::init()
{
    QScopedPointer<ProjectSetting> setting( createSetting() );
    if (!checkProjectOpened( setting.data() )) return;
    if (!checkRepositoryNotInitialized( setting.data() )) return;

    // git init setting.projectPath()
    QString repoPath = setting->projectPath();
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
        QMessageBox::information(0, tr(""), tr("git init succeessfully!"));
    }
}

void GitController::gitIgnore()
{
    // create or update .gitignore

    QStringList toBeIgnored;
    toBeIgnored << ".accesscvs";
    toBeIgnored << "*.mdb";
    toBeIgnored << "*.ldb";
    toBeIgnored << "*.accdb";
    toBeIgnored << "*.laccdb";
    toBeIgnored << "*.adp";

    QScopedPointer<ProjectSetting> setting( createSetting() );
    if (!checkProjectOpened( setting.data() )) return;

    QFile file( setting->projectPath() + "\\.gitignore" );
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

bool GitController::isSupportedSsh() const
{
    return (git_libgit2_features() & GIT_FEATURE_SSH);
}

void GitController::manageRemotes()
{
    QScopedPointer<ProjectSetting> setting( createSetting() );
    if (!checkProjectOpened(setting.data())) return;
    if (!checkRepositoryInitialized(setting.data())) return;

    QPointer<LibQGit2::Repository> repo;
    repo = new LibQGit2::Repository();
    repo->open( setting->projectPath() );

    git_strarray remotes;
    if (0 > git_remote_list( &remotes,   repo->data() ) )
        QMessageBox::information(0,tr(""), tr("git_remote_list failed"));

    QStringList list;
    for (size_t i = 0 ; i < remotes.count ; ++i)
        list << QString( remotes.strings[i] );
    git_strarray_free( &remotes );

    QMessageBox::information(0, "", tr("remote count = %1\n%2").arg(list.count()).arg( list.join( "\n" ) ) );

    delete repo;
}
int cred_acquire_cb(git_cred **out,
        const char * url,
        const char * username_from_url,
        unsigned int allowed_types,
        void *payload)
{
    Q_UNUSED(url)
    Q_UNUSED(username_from_url)
    Q_UNUSED(allowed_types)
    Q_UNUSED(payload)

    char username[128] = {0};
    char password[128] = {0};

    return git_cred_userpass_plaintext_new(out, username, password);
}

void GitController::pull()
{
    if (!isSupportedSsh())
    {
        QMessageBox::information(0, "", tr("ssh is NOT supported"));
        return;
    }

    QScopedPointer<ProjectSetting> setting( createSetting() );
    if (!checkProjectOpened(setting.data())) return;
    if (!checkRepositoryInitialized(setting.data())) return;

    QScopedPointer<LibQGit2::Repository> repo( new LibQGit2::Repository() );
    repo->open( setting->projectPath() );

    LibQGit2::Reference headRef = repo->head();
    LibQGit2::Reference head = headRef.resolve();
    QString headFullName = head.name();

    git_reference *upstream = NULL;
    git_branch_upstream( &upstream, headRef.data() );

    QString upstreamFullName = git_reference_name( upstream );

    git_reference_free(upstream);

    // headName     = refs/heads/           feature/***
    // upstreamName = refs/remotes/ origin/ feature/***

    QMessageBox::information(0, "", QString("headFullName name = %1, upstreamFullName = %2").arg( headFullName ).arg( upstreamFullName ) );

    QString headName = headFullName.replace( QRegularExpression("^refs/heads/"), QString() );
    QString upstreamName = upstreamFullName.replace( QRegularExpression("^refs/remotes/"), QString() ).replace( QRegularExpression("/"+headName+"$"), QString() );

    QMessageBox::information(0, "", QString("headName name = %1, upstreamName = %2").arg( headName ).arg( upstreamName ) );

//    QString remote;
//    QString head;
//    LibQGit2::Signature signature;
//    QString message;
//    repo->fetch( remote, head, signature, message );

    QString privateKeyPath = "C:\\Users\\shoichiro\\.ssh\\id_rsa";
    QString publicKeyPath  = "C:\\Users\\shoichiro\\.ssh\\id_rsa.pub";

    git_remote *remote;
    git_remote_lookup( &remote, repo->data(), "origin" );
    QString remoteUrl = git_remote_url( remote );
    QMessageBox::information(0,"",remoteUrl);


    LibQGit2::Signature signature;
    //LibQGit2::Credentials;
//    LibQGit2::Remote rem;

    git_remote_callbacks callbacks = GIT_REMOTE_CALLBACKS_INIT;
    callbacks.credentials = cred_acquire_cb;
    git_remote_set_callbacks( remote, &callbacks );
    const git_transfer_progress *stats = git_remote_stats(remote);
    git_remote_connect(remote, GIT_DIRECTION_FETCH);
    git_remote_download(remote, NULL);

    if (stats->local_objects > 0)
    {

    }
    else
    {

    }
    git_remote_disconnect(remote);
    git_remote_free(remote);


    bool succeed = false;
    try
    {
        //repo->setRemoteCredentials(upstreamName, LibQGit2::Credentials::ssh(privateKeyPath, publicKeyPath, "git"));
        repo->fetch(upstreamName, headName);
        succeed = true;
    }
    catch (const LibQGit2::Exception& ex)
    {
        QMessageBox::warning(0, tr(""), ex.what());
    }

    if (!succeed) return;

    QMessageBox::information(0, "", tr("fetch succeed!") );
//    LibQGit2::OId headOId = head.target();
//    LibQGit2::Tree headTree = repo->lookupTree( &headOId );

//    git_branch_upstream( &upstream, headRef.data() );

//    LibQGit2::Reference upstreamRef( upstream );
//    LibQGit2::OId upstreamOId = upstreamRef.target();
//    LibQGit2::Tree upstreamTree = repo->lookupTree( &upstreamOId );

//    LibQGit2::Index index = repo->mergeTrees( &headTree, &upstreamTree);
//    if (index.hasConflicts())
//        ;


}

bool GitController::checkProjectOpened(const ProjectSetting *setting)
{
    if (!setting->isProjectOpened())
    {
        QMessageBox::information(0, tr(""), tr("no project is opened!"));
        return false;
    }
    return true;
}

bool GitController::checkRepositoryInitialized(const ProjectSetting *setting)
{
    if ( !QDir( setting->projectPath() + "\\.git" ).exists() )
    {
        QMessageBox::information(0, tr(""), tr("project has not been initialized yet! git init first."));
        return false;
    }
    return true;
}

bool GitController::checkRepositoryNotInitialized(const ProjectSetting *setting)
{
    if ( QDir( setting->projectPath() + "\\.git" ).exists() )
    {
        QMessageBox::information(0, tr(""), tr("already initialized!"));
        return false;
    }
    return true;
}

ProjectSetting *GitController::createSetting()
{
    ProjectSetting *setting = new ProjectSetting(this);
    setting->initialize(m_application);
    return setting;
}


