#ifndef _SORTED_LIST
#define _SORTED_LIST

using namespace std;

namespace mtm
{
    template <class T>
    class SortedList
    {
    private:

        int size;

        T t;

    public:

        SortedList();
        ~SortedList();

        SortedList(const SortedList& sortedList);

        SortedList& operator=(const SortedList& sortedList);

        void insert(const T& t);

    };

    template <class T>
    SortedList<T>::SortedList()
    {
        this->size = 0;
    }

    template <class T>
    SortedList<T>::~SortedList()
    {
    }

    template <class T>
    SortedList<T>::SortedList(const SortedList& sortedList)
    {
    }

    template <class T>
    SortedList<T>& SortedList<T>::operator=(const SortedList& sortedList)
    {
    }

    template <class T>
    void SortedList<T>::insert(const T& t)
    {
        this->t = t;
    }
}

#endif //_SORTED_LIST
