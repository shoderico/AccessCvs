#ifndef ADDINMAIN_H
#define ADDINMAIN_H

#include <QObject>
#include <QAxBindable>

class ITypeInfo;

class AddInMain : public QObject, public QAxBindable
{
    Q_OBJECT
    //Q_CLASSINFO("ClassID",     "{85842016-1eb7-4e60-ae2d-a473360251a8}")
    //Q_CLASSINFO("InterfaceID", "{f3341da4-35a3-4b45-af98-ca5dac0930c4}")
    //Q_CLASSINFO("EventsID",    "{4296ce0f-1bd1-495e-ae1a-f7f10f5ccbee}")
    //Q_CLASSINFO("Version", "1.0")
    //Q_CLASSINFO("ToSuperClass", "AddInMain")
    //Q_CLASSINFO("Creatable", "yes")
    //Q_CLASSINFO("Insertable", "yes")
    //Q_CLASSINFO("Aggregatable", "no")

public:
    explicit AddInMain(ITypeInfo *pTypeInfo, QObject *parent = 0);

    QAxAggregated *createAggregate();

private:
    ITypeInfo *m_pTypeInfo;
};

#endif // ADDINMAIN_H
