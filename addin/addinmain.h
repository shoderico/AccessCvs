#ifndef ADDINMAIN_H
#define ADDINMAIN_H

#include <QObject>
#include <QAxBindable>

class AddInMain : public QObject, public QAxBindable
{
    Q_OBJECT
public:
    explicit AddInMain(QObject *parent = 0);
    QAxAggregated *createAggregate();
};

#endif // ADDINMAIN_H
