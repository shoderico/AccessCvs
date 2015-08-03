#include "addinfactory.h"

#include "addinmain.h"
#include "ribboncallback.h"

#include <QSettings>
#include <QFileInfo>
#include <QMessageBox>
//#include <QSysInfo>

AddInFactory::AddInFactory(const QUuid &app, const QUuid &lib)
    : QAxFactory(app, lib)
    , m_className( QLatin1String("AddInMain") )
//  , m_registryRoot( QLatin1String("HKEY_LOCAL_MACHINE\\SOFTWARE") )
    , m_registryRoot( QLatin1String("HKEY_CURRENT_USER\\Software") )
    , m_registryPath( QLatin1String("\\Microsoft\\Office\\Access\\Addins") )
{
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
    {
        HRESULT hr;
        ITypeLib *pTypeLib = NULL;
        ITypeInfo *pTypeInfo = NULL;
        QString typeLibResourcePath = serverFilePath() + "\\2";
        hr = LoadTypeLib( reinterpret_cast<const OLECHAR *>( typeLibResourcePath.utf16() ), &pTypeLib);
        if ( SUCCEEDED( hr ) )
        {
            hr = pTypeLib->GetTypeInfoOfGuid( IID_IRibbonCallback, &pTypeInfo );
            if ( !SUCCEEDED( hr ) )
                QMessageBox::information(0, QString(""), QString("AddInFactory::createObject  GetTypeInfoOfGuid failed"));

            pTypeLib->Release();
        }
        else
            QMessageBox::information(0, QString(""), QString("AddInFactory::createObject  LoadTypeLib failed"));

        AddInMain *object = new AddInMain(pTypeInfo, 0);

        return object;
    }
    return 0;
}

QUuid AddInFactory::classID(const QString &key) const
{
    if (key == m_className)
        return QUuid( "{85842016-1eb7-4e60-ae2d-a473360251a8}" );
    return QUuid();
}

QUuid AddInFactory::interfaceID(const QString &key) const
{
    if (key == m_className)
        return QUuid( "{f3341da4-35a3-4b45-af98-ca5dac0930c4}" );
    return QUuid();
}

QUuid AddInFactory::eventsID(const QString &key) const
{
    if (key == m_className)
        return QUuid( "{4296ce0f-1bd1-495e-ae1a-f7f10f5ccbee}" );
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
        st->setValue("/" + progID() + "/FriendlyName",      QString("FriendlyName"));
        st->setValue("/" + progID() + "/Description",       QString("Description"));
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

QAXFACTORY_EXPORT(AddInFactory, "{27e3bd9e-2ee3-41ba-a69d-61f510fda820}", "{18bf0f9a-c557-4324-b5d8-f4077561a87e}")
