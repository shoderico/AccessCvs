#include "resource.h"

#include "pch.hpp"

Resource::Resource(QObject *parent) : QObject(parent)
{

}

void Resource::init()
{
    Q_INIT_RESOURCE(resource);
}

void Resource::cleanup()
{
    Q_CLEANUP_RESOURCE(resource);
}
