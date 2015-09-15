#ifndef HELPCONTROLLER_H
#define HELPCONTROLLER_H

#include "helpcontroller_global.h"

#include "addin/addincontroller.h"

class HELPCONTROLLER_SHARED_EXPORT HelpController : public QObject, public AddInController
{
    Q_OBJECT
    Q_INTERFACES(AddInController)
public:
    explicit HelpController(QObject *parent = 0);
    ~HelpController();

    void initialize(Access::Application *application, QWidget *parentWidget);
    QString ribbonXml();
    bool imagePath(const QString &controlId, QString &imagePath, QSize &size);
    bool handleButtonClick(const QString &controlId);

private slots:
    void about();

private:
    Access::Application *m_application;
    QWidget *m_parentWidget;
};

#endif // HELPCONTROLLER_H
