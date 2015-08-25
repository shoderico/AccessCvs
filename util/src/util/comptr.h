#ifndef COMPTR_H
#define COMPTR_H

#include "util_global.h"

template <typename T>
class UTILSHARED_EXPORT ComPtr
{
public:
    ComPtr(T* ptr) : m_ptr(ptr){}
    ComPtr() : m_ptr(0) {}
    ~ComPtr() {
        clear();
    }
    T* operator ->() {
        return m_ptr;
    }
    ComPtr& operator=(T* ptr) {
        set(ptr);
        return *this;
    }
    void set(T* ptr) {
        clear();
        m_ptr = ptr;
    }
    void clear() {
        if (m_ptr)
        {
            m_ptr->clear();
            delete m_ptr;
            m_ptr = 0;
        }
    }

    bool is() { return (m_ptr != 0); }
    T* ptr() const { return m_ptr; }

private:
    T* m_ptr;
};



#endif // COMPTR_H
