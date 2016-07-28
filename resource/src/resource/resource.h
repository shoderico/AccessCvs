#ifndef RESOURCE_H
#define RESOURCE_H

#include "resource_global.h"
#include <QObject>

class RESOURCE_SHARED_EXPORT Resource : public QObject
{
    Q_OBJECT
public:
    explicit Resource(QObject *parent = 0);

    void init();
    void cleanup();

signals:

public slots:
};

#endif // RESOURCE_H
