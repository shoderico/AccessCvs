#ifndef ADDINRIBBONTAB_H
#define ADDINRIBBONTAB_H

#include "addin_global.h"

#include <QObject>
#include <OCIdl.h>

class WindowWidgetManager;
class AddInController;
class AddInFactory;

class ADDIN_SHARED_EXPORT AddInRibbonTab : public QObject
{
    Q_OBJECT
public:
    explicit AddInRibbonTab(QObject *parent = 0);

    void appendController(AddInController *controller);

    void setRibbonTabId(const QString &tabId);
    void setRibbonTabLabel(const QString &tabLabel);

    virtual bool onButtonClicked(const QString &controlId);
    virtual QString ribbomXml();
    virtual IPictureDisp *buttonImage(const QString &controlId);

private:
    QList<AddInController *> m_controllers;

    QString m_ribbonTabId;
    QString m_ribbonTabLabel;
};

#endif // ADDINRIBBONTAB_H
