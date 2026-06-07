#ifndef HELPADDINCONTROLLER_H
#define HELPADDINCONTROLLER_H

#include "helpaddincontroller_global.h"

#include "addinutil/addincontroller.h"

class HELPADDINCONTROLLER_SHARED_EXPORT HelpAddinController : public QObject, public AddInController
{
    Q_OBJECT
    Q_INTERFACES(AddInController)
public:
    explicit HelpAddinController(QObject *parent = 0);
    ~HelpAddinController();

    void initialize(QAxObject *application, QWidget *parentWidget);
    QString ribbonXml();
    bool imagePath(const QString &controlId, QString &imagePath, QSize &size);
    bool handleButtonClick(const QString &controlId);

private slots:
    void about();

private:
    QAxObject *m_application;
    QWidget *m_parentWidget;
};

#endif // HELPADDINCONTROLLER_H
