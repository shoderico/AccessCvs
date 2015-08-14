#ifndef GITMANAGER_H
#define GITMANAGER_H

#include <QObject>

class IDispatch;
namespace Access {
class Application;
} // namespace Access

class GitManager : public QObject
{
    Q_OBJECT
public:
    explicit GitManager(Access::Application *application, QObject *parent = 0);

    void init();
    void gitIgnore();
    bool isSupportedSsh() const;

signals:
public slots:

private:
    Access::Application *m_application;
};

#endif // GITMANAGER_H
