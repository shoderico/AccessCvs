#include "addinfactory.h"

#include "addinbindable.h"

#include <QDebug>
#include <QSettings>
#include <QFileInfo>

#include "pch.hpp"

extern QAxFactory *qax_instantiate();

AddInFactory::AddInFactory(const QUuid &app, const QUuid &lib)
    : QAxFactory(app, lib)
    , m_loadBehavior(3)
    , m_commandLineSafe(0)
{
}

AddInFactory::~AddInFactory()
{
    qDebug() << "destructor";
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
        return &AddInBindable::staticMetaObject;
    return 0;
}

QObject *AddInFactory::createObject(const QString &key)
{
    if (key == m_className)
        return  new AddInBindable(this, this);
    return 0;
}

QUuid AddInFactory::classID(const QString &key) const
{
    if (key == m_className)
        return QUuid( m_classId );
    return QUuid();
}

QUuid AddInFactory::interfaceID(const QString &key) const
{
    if (key == m_className)
        return QUuid( m_interfaceId );
    return QUuid();
}

QUuid AddInFactory::eventsID(const QString &key) const
{
    if (key == m_className)
        return QUuid( m_eventsId );
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

QAxAggregated *AddInFactory::createAggregate(QObject *parent)
{
    Q_UNUSED(parent)
    return NULL;
}

void AddInFactory::setApplication(IDispatch *application)
{
    Q_UNUSED(application)
}

void AddInFactory::releaseApplication()
{

}

QAxObject *AddInFactory::application() const
{
    return NULL;
}

int AddInFactory::applicationHwnd()
{
    return 0;
}

void AddInFactory::onBeforeConnectionEvent()
{

}

void AddInFactory::onAfterConnectionEvent()
{

}

void AddInFactory::onAfterDisconnectionEvent()
{

}

void AddInFactory::setRegistryRoot(const QString &registryRoot)
{
    m_registryRoot = registryRoot;
}

void AddInFactory::setRegistryPath(const QString &registryPath)
{
    m_registryPath = registryPath;
}

void AddInFactory::setClassName(const QString &className)
{
    m_className = className;
}

void AddInFactory::setClassId(const QString &classId)
{
    m_classId = classId;
}

void AddInFactory::setInterfaceId(const QString &interfaceId)
{
    m_interfaceId = interfaceId;
}

void AddInFactory::setEventsId(const QString &eventsId)
{
    m_eventsId = eventsId;
}

void AddInFactory::setLoadBehavior(const int loadBehavior)
{
    m_loadBehavior = loadBehavior;
}

void AddInFactory::setCommandLineSafe(const int commandLineSafe)
{
    m_commandLineSafe = commandLineSafe;
}

void AddInFactory::setFriendlyName(const QString &friendlyName)
{
    m_friendlyName = friendlyName;
}

void AddInFactory::setDescription(const QString &description)
{
    m_description = description;
}

void AddInFactory::setServerFilePath(const QString &serverFilePath)
{
    m_serverFilePath = serverFilePath;
}

QString AddInFactory::progID() const
{
    return QFileInfo( m_serverFilePath ).baseName() + "." + m_className;
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
        st->setValue("/" + progID() + "/LoadBehavior",      m_loadBehavior);
        st->setValue("/" + progID() + "/CommandLineSafe",   m_commandLineSafe);
        st->setValue("/" + progID() + "/FriendlyName",      m_friendlyName);
        st->setValue("/" + progID() + "/Description",       m_description);
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

QAXFACTORY_EXPORT(AddInFactory, "DummyLibraryID", "DummyApplicationID")
