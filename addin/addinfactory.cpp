#include "addinfactory.h"

#include "addinmain.h"
#include "logfile.h"
#include <QDebug>

#include <QSettings>
#include <QFileInfo>


static const char LibraryID[]     = "{27e3bd9e-2ee3-41ba-a69d-61f510fda820}";
static const char ApplicationID[] = "{18bf0f9a-c557-4324-b5d8-f4077561a87e}";
static const char ClassID[]       = "{85842016-1eb7-4e60-ae2d-a473360251a8}";
static const char InterfaceID[]   = "{f3341da4-35a3-4b45-af98-ca5dac0930c4}";
static const char EventsID[]      = "{4296ce0f-1bd1-495e-ae1a-f7f10f5ccbee}";
static const char ClassName[]     = "AddInMain";


AddInFactory::AddInFactory(const QUuid &app, const QUuid &lib)
    : QAxFactory(app, lib)
    , m_className( QLatin1String( ClassName ) )
//  , m_registryRoot( QLatin1String("HKEY_LOCAL_MACHINE\\SOFTWARE") )
    , m_registryRoot( QLatin1String("HKEY_CURRENT_USER\\Software") )
    , m_registryPath( QLatin1String("\\Microsoft\\Office\\Access\\Addins") )
{
    (void)new LogFile( serverDirPath() + "\\log", "log_", false );
    qInstallMessageHandler(LogFile::MessageOutput);
}

AddInFactory::~AddInFactory()
{
    delete LogFile::instance();
}

QStringList AddInFactory::featureList() const
{
    QStringList list;
    list << m_className;
    return list;
}

const QMetaObject *AddInFactory::metaObject(const QString &key) const
{
    if (key == m_className)
        return &AddInMain::staticMetaObject;
    return 0;
}

QObject *AddInFactory::createObject(const QString &key)
{
    if (key == m_className)
        return  new AddInMain(this);
    return 0;
}

QUuid AddInFactory::classID(const QString &key) const
{
    if (key == m_className)
        return QUuid( ClassID );
    return QUuid();
}

QUuid AddInFactory::interfaceID(const QString &key) const
{
    if (key == m_className)
        return QUuid( InterfaceID );
    return QUuid();
}

QUuid AddInFactory::eventsID(const QString &key) const
{
    if (key == m_className)
        return QUuid( EventsID );
    return QUuid();
}

void AddInFactory::registerClass(const QString &key, QSettings *settings) const
{
    Q_UNUSED(settings)

    registerClassInternal(key, ws32bit);
    //if ( is64bit() ) registerClassInternal(key, ws64bit);
}

void AddInFactory::unregisterClass(const QString &key, QSettings *settings) const
{
    Q_UNUSED(settings)

    unregisterClassInternal(key, ws32bit);
    //if ( is64bit() ) unregisterClassInternal(key, ws64bit);
}

QString AddInFactory::progID() const
{
    return QFileInfo( serverFilePath() ).baseName() + "." + m_className;
}

bool AddInFactory::is64bit() const
{
    QScopedPointer<QSettings> stRoot( new QSettings(m_registryRoot, QSettings::NativeFormat));
    return stRoot->contains("Wow6432Node");
}

QString AddInFactory::registryPath(AddInFactory::WordSize ws) const
{
    QString path = m_registryRoot;
    if (ws == ws64bit)
        path += "\\Wow6432Node";
    path += m_registryPath;
    return path;
}

void AddInFactory::registerClassInternal(const QString &key, AddInFactory::WordSize ws) const
{
    //[HKEY_CURRENT_USER\Software\Microsoft\Office\Outlook\Addins\SampleAddIn.AddInIFace]
    //"FriendlyName"="Sample Add-in"
    //"Description"="Sample Outlook Add-In"
    //"LoadBehavior"=dword:00000008

    if ( key == m_className )
    {
        QScopedPointer<QSettings> st( new QSettings( registryPath(ws) , QSettings::NativeFormat));
        st->setValue("/" + progID() + "/LoadBehavior",      3);
        st->setValue("/" + progID() + "/CommandLineSafe",   0);
        st->setValue("/" + progID() + "/FriendlyName",      tr("FriendlyName")); // FIXME: proper FriendlyName
        st->setValue("/" + progID() + "/Description",       tr("Description"));  // FIXME: proper Description
    }
}

void AddInFactory::unregisterClassInternal(const QString &key, AddInFactory::WordSize ws) const
{
    if ( key == m_className )
    {
        QScopedPointer<QSettings> st( new QSettings( registryPath(ws) , QSettings::NativeFormat));
        st->remove("/" + progID());
    }
}

QAXFACTORY_EXPORT(AddInFactory, LibraryID, ApplicationID)
