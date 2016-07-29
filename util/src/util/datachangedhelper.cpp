#include "datachangedhelper.h"

#include "pch.hpp"

DataChangedHelper::DataChangedHelper(int count)
    : m_count(count)
    , m_first(count)
    , m_last(-1)
{
}

void DataChangedHelper::changedAll()
{
    QWriteLocker locker(&m_lock);
    m_first = 0;
    m_last = m_count - 1;
}

void DataChangedHelper::changed(int row)
{
    QWriteLocker locker(&m_lock);
    if (m_first > row) m_first = row;
    if (m_last  < row) m_last  = row;
}

bool DataChangedHelper::isChanged() const
{
    QReadLocker locker(&m_lock);
    return (m_first != m_count && m_last != -1);
}

int DataChangedHelper::first() const
{
    QReadLocker locker(&m_lock);
    return m_first;
}

int DataChangedHelper::last() const
{
    QReadLocker locker(&m_lock);
    return m_last;
}

