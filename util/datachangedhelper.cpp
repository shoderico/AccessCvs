#include "datachangedhelper.h"

DataChangedHelper::DataChangedHelper(int count)
    : m_count(count)
    , m_first(count)
    , m_last(-1)
{
}

void DataChangedHelper::changedAll()
{
    m_first = 0;
    m_last = m_count - 1;
}

void DataChangedHelper::changed(int row)
{
    if (m_first > row) m_first = row;
    if (m_last  < row) m_last  = row;
}

bool DataChangedHelper::isChanged() const
{
    return (m_first != m_count && m_last != -1);
}

int DataChangedHelper::first() const
{
    return m_first;
}

int DataChangedHelper::last() const
{
    return m_last;
}

