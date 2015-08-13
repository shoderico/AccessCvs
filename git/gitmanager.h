#ifndef GITMANAGER_H
#define GITMANAGER_H

#include <QObject>

class IDispatch;

class GitManager : public QObject
{
    Q_OBJECT
public:
    explicit GitManager(IDispatch *application, QObject *parent = 0);

    void init();
    void gitIgnore();

signals:
public slots:

private:
    IDispatch *m_applicationIDisp;
    IDispatch *m_addInInstIDisp;
};

#endif // GITMANAGER_H
