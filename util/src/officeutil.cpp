#include "officeutil.h"

#include <QSettings>
#include <QScopedPointer>


QString OfficeUtil::getExePath(const QString &progId)
{
    // https://support.microsoft.com/ja-jp/kb/240794
    QString path = QString("HKEY_LOCAL_MACHINE\\SOFTWARE\\Classes").arg(progId);
    QScopedPointer<QSettings> settings( new QSettings( path, QSettings::NativeFormat) );
    QString clsid = settings->value( QString("%1/CLSID/Default").arg(progId) ).toString();
    return settings->value( QString("CLSID/%1/LocalServer32/Default").arg(clsid) ).toString();
}
