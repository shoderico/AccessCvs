#ifndef APPSIGNALHANDLER_H
#define APPSIGNALHANDLER_H

#include <QObject>

namespace Access {
class Application;
}

class AppSignalHandler : public QObject
{
    Q_OBJECT
public:
    explicit AppSignalHandler(Access::Application *application, QObject *parent = 0);
    ~AppSignalHandler();

signals:

public slots:
    void exception(int code, const QString & source, const QString & desc, const QString & help);
    void	propertyChanged(const QString & name);
    void	signal(const QString & name, int argc, void * argv);

private:
    Access::Application *m_application;
    QWidget *m_parentWidget;

};

#endif // APPSIGNALHANDLER_H
