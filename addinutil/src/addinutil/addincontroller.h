#ifndef ADDINCONTROLLER_H
#define ADDINCONTROLLER_H

#include <QtPlugin>

class QAxObject;

class AddInController
{
public:
    virtual ~AddInController(){}

    virtual void initialize(QAxObject *application, QWidget *parentWidget) = 0;

    virtual QString ribbonXml() = 0;
    virtual bool imagePath(const QString &controlId, QString &imagePath, QSize &size) = 0;
    virtual bool handleButtonClick(const QString &controlId) = 0;
};

#define AddInController_iid "com.shoderitz.addin.AddInController"

Q_DECLARE_INTERFACE(AddInController, AddInController_iid)

#endif // ADDINCONTROLLER_H

