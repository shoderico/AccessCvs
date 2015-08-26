#ifndef ADDINFACTORY_H
#define ADDINFACTORY_H

#include "addin_global.h"

#include <QAxFactory>

class QAxAggregated;

class ADDINSHARED_EXPORT AddInFactory : public QAxFactory
{
    Q_OBJECT
public:
    AddInFactory(const QUuid &app, const QUuid &lib);
    ~AddInFactory();

    QStringList featureList() const;
    const QMetaObject *metaObject(const QString &key) const;
    QObject *createObject(const QString &key);
    QUuid classID(const QString &key) const;
    QUuid interfaceID(const QString &key) const;
    QUuid eventsID(const QString &key) const;

    void registerClass(const QString &key, QSettings *settings) const;
    void unregisterClass(const QString &key, QSettings *settings) const;

    virtual QAxAggregated *createAggregate(QObject *parent = 0) = 0;

protected:
    void setClassName(const QString &className);
    void setClassId(const QString &classId);
    void setInterfaceId(const QString &interfaceId);
    void setEventsId(const QString &eventsId);
    void setFriendlyName(const QString &friendlyName);
    void setDescription(const QString &description);

private:
    QString m_className;
    QString m_classId;
    QString m_interfaceId;
    QString m_eventsId;

    QString m_friendlyName;
    QString m_description;

    QString m_registryRoot;
    QString m_registryPath;
    QString progID() const;

    enum WordSize { ws32bit, ws64bit };
    bool is64bit() const;
    QString registryPath(WordSize ws) const;
    void registerClassInternal(const QString &key, WordSize ws) const;
    void unregisterClassInternal(const QString &key, WordSize ws) const;
};


#endif // ADDINFACTORY_H
