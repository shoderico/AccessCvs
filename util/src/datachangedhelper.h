#ifndef DATACHANGEDHELPER_H
#define DATACHANGEDHELPER_H

#include "util_global.h"

#include <QReadWriteLock>

class UTILSHARED_EXPORT DataChangedHelper
{
public:
    DataChangedHelper(int count);

    void changedAll();
    void changed(int row);

    bool isChanged() const;

    int first() const;
    int last() const;

private:
    int m_count;
    int m_first;
    int m_last;
    mutable QReadWriteLock m_lock;
};

#endif // DATACHANGEDHELPER_H
