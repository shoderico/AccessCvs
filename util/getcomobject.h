#ifndef GETCOMOBJECT_H
#define GETCOMOBJECT_H

#include <QString>
#include <QList>

class QAxObject;

class GetComObject
{
    QString m_targetExeName;
    QString m_targetWndClassName;
public:
    GetComObject(const QString &targetExeName = QString(), const QString &targetWindowClassName = QString());

    QString tartgetExeName() const;
    void setTargetExeName(const QString &targetExeName);

    QString targetWindowClassName() const;
    void setTargetWindowClassName(const QString &targetWindowClassName);

    QList< QAxObject *> getComObjects();

};

#endif // GETCOMOBJECT_H
