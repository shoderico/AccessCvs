#ifndef ADDINMAIN_H
#define ADDINMAIN_H

#include "addin_global.h"

#include <QObject>
#include <QAxBindable>

class AddInFactory;

class ADDINSHARED_EXPORT AddInMain : public QObject, public QAxBindable
{
    Q_OBJECT
public:
    explicit AddInMain(AddInFactory *factory, QObject *parent = 0);
    QAxAggregated *createAggregate();
private:
    AddInFactory *m_factory;
};

#endif // ADDINMAIN_H
