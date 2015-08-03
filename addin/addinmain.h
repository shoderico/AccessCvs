#ifndef ADDINMAIN_H
#define ADDINMAIN_H

#include <QObject>
#include <QAxBindable>

class ITypeInfo;

class AddInMain : public QObject, public QAxBindable
{
    Q_OBJECT
public:
    explicit AddInMain(ITypeInfo *pTypeInfo, QObject *parent = 0);

    QAxAggregated *createAggregate();

private:
    ITypeInfo *m_pTypeInfo;
};

#endif // ADDINMAIN_H
