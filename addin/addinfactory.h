#ifndef ADDINFACTORY_H
#define ADDINFACTORY_H

#include <QAxFactory>

class AddInFactory : public QAxFactory
{
public:
    AddInFactory(const QUuid &app, const QUuid &lib);

    QStringList featureList() const;
    const QMetaObject *metaObject(const QString &key) const;
    QObject *createObject(const QString &key);
    QUuid classID(const QString &key) const;
    QUuid interfaceID(const QString &key) const;
    QUuid eventsID(const QString &key) const;

    void registerClass(const QString &key, QSettings *settings) const;
    void unregisterClass(const QString &key, QSettings *settings) const;

private:
    QString m_className;
    QString m_regisryPath;
    QString progID() const;
};


#endif // ADDINFACTORY_H
