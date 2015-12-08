#include "appsignalhandler.h"

#include "accesslib/accesslib.h"

#include <QMessageBox>


AppSignalHandler::AppSignalHandler(Access::Application *application, QObject *parent)
    : QObject(parent)
    , m_application(application)
    , m_parentWidget(0)
{
    bool c;
    c = connect( m_application, SIGNAL(exception(int,QString,QString,QString)), this, SLOT(exception(int,QString,QString,QString)) );
    if (!c) QMessageBox::information(m_parentWidget, "", "connect exception failed");
    c = connect( m_application, SIGNAL(propertyChanged(QString)), this, SLOT(propertyChanged(QString)) );
    if (!c) QMessageBox::information(m_parentWidget, "", "connect propertyChanged failed");
    c = connect( m_application, SIGNAL(signal(QString,int,void*)), this, SLOT(signal(QString,int,void*)) );
    if (!c) QMessageBox::information(m_parentWidget, "", "connect signal failed");
}

AppSignalHandler::~AppSignalHandler()
{
    if (m_application)
    {
        disconnect( m_application, SIGNAL(exception(int,QString,QString,QString)), this, SLOT(exception(int,QString,QString,QString)) );
        disconnect( m_application, SIGNAL(propertyChanged(QString)), this, SLOT(propertyChanged(QString)) );
        disconnect( m_application, SIGNAL(signal(QString,int,void*)), this, SLOT(signal(QString,int,void*)) );
    }
}

void AppSignalHandler::exception(int code, const QString &source, const QString &desc, const QString &help)
{
    QMessageBox::information(m_parentWidget, "", QString().setNum(code) + "\n" + source + "\n" + desc + "\n" + help );
}

void AppSignalHandler::propertyChanged(const QString &name)
{
    QMessageBox::information(m_parentWidget, "", QString("propertyChanged ") + name);
}

void AppSignalHandler::signal(const QString &name, int argc, void *argv)
{
    Q_UNUSED(argc) Q_UNUSED(argv)
    QMessageBox::information(m_parentWidget, "", QString("propertyChanged ") + name);
}
