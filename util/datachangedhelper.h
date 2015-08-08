#ifndef DATACHANGEDHELPER_H
#define DATACHANGEDHELPER_H


class DataChangedHelper
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
};

#endif // DATACHANGEDHELPER_H
