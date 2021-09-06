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

        T temp;

    public:

        SortedList();

    };

    template <class T>
    SortedList<T>::SortedList()
    {
        this->size = 0;
        
        this->temp = 0;
    }
}

#endif //_SORTED_LIST
