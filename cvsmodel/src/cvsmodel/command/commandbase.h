#ifndef COMMANDBASE_H
#define COMMANDBASE_H

#include <QObject>

class QAxObject;

class ObjectItems;

class CommandBase : public QObject
{
    Q_OBJECT
public:
    explicit CommandBase(QObject *parent = 0);

    void setApplication(QAxObject *application);
    virtual void execute(ObjectItems *allTargets);

signals:
    void progressStart(int type, int count);
    void progressEnd(int type);
    void progressChange(int type, int position);

public slots:

protected:
    QAxObject *m_application;
};

#endif // COMMANDBASE_H
