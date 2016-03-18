#include "commandbase.h"

CommandBase::CommandBase(QObject *parent) : QObject(parent)
{

}

void CommandBase::setApplication(QAxObject *application)
{
    m_application = application;
}

void CommandBase::execute(ObjectItems *allTargets)
{
    Q_UNUSED(allTargets)
}

