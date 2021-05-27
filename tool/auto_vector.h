#ifndef _AUTO_VECTOR_
#define _AUTO_VECTOR_
#include <vector>
#include "assert.h"
using namespace std;

template <class T>
class AutoVector
{
    //禁止拷贝构造和复制
    AutoVector(const AutoVector&);
    AutoVector& operator=(const AutoVector&);

public:
    typedef typename vector<T*>::const_iterator const_iterator;
    typedef typename vector<T*>::iterator iterator;

public:
    AutoVector()
    {
        _vec.clear();
    }
    ~AutoVector()
    {
        clear();
    }
    void push_back(T *pT)
    {
        _vec.push_back(pT);
    }
    unsigned int size()
    {
        return _vec.size();
    }
    void clear()
    {
        for (unsigned int i = 0; i < _vec.size(); ++i)
        {
            delete _vec[i];
        }
        _vec.clear();
    }
    T* operator[](unsigned int i)
    {
        assert(i < _vec.size());
        return _vec[i];
    }

    const_iterator begin() const
    {
        return _vec.begin();
    }

    const_iterator end() const
    {
        return _vec.end();
    }

    bool empty() const
    {
        return _vec.empty();
    }

    iterator erase(const_iterator iter)
    {
        delete *iter;
        return _vec.erase(iter);
    }
private:
    vector<T*> _vec;
};
#endif



