#ifndef ADDINCONTROLLER_H
#define ADDINCONTROLLER_H

#include <QtPlugin>

namespace Access {
class Application;
}

class AddInController
{
public:
    virtual ~AddInController(){}

    virtual void initialize(Access::Application *application, QWidget *parentWidget);

    virtual QString ribbonXml() = 0;
    virtual QString imagePath() = 0;
    virtual bool handleButtonClick(const QString &controlId) = 0;
};

#define AddInController_iid "com.shoderitz.addin.AddInController"

Q_DECLARE_INTERFACE(AddInController, AddInController_iid)

#endif // ADDINCONTROLLER_H

