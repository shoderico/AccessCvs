#ifndef CVSADDINIMPL_H
#define CVSADDINIMPL_H

#include "addin/addinimpl.h"

class ActionManager;
class GitManager;
class AccessUtilManager;
class WindowWidgetManager;

class CvsAddInImpl : public AddInImpl
{
    Q_OBJECT
public:
    explicit CvsAddInImpl(QObject *parent = 0);

protected:
    virtual void onConnectionEvent();
    virtual void onDisconnectionEvent();
    virtual QString ribbomXml();
    virtual IPictureDisp *buttonImage(const QString &controlId);
    virtual HRESULT onButtonClicked(const QString &controlId);

private:
    ActionManager       *m_actionManager;
    GitManager          *m_gitManager;
    AccessUtilManager   *m_accessUtilManager;
    WindowWidgetManager *m_winWidgetManager;
};

#endif // CVSADDINIMPL_H
