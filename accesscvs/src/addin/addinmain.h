#ifndef ADDINMAIN_H
#define ADDINMAIN_H

#include <QObject>
#include <QAxBindable>

class AddInFactory;

class AddInMain : public QObject, public QAxBindable
{
    Q_OBJECT
public:
    explicit AddInMain(AddInFactory *factory, QObject *parent = 0);
    QAxAggregated *createAggregate();
private:
    AddInFactory *m_factory;
};

#endif // ADDINMAIN_H
