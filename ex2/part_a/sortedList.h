#ifndef _SORTED_LIST
#define _SORTED_LIST

using namespace std;

namespace mtm
{
    template <class T>
    class SortedList
    {
    private:

        // ToDo: consider replacing with class
        struct node
        {
            T data;
            node *next;
        };

        node *head;

        int size;

    public:

        SortedList();
        ~SortedList();

        SortedList(const SortedList& sortedList);

        SortedList& operator=(const SortedList& sortedList);

        void insert(const T& t);

        class Iterator;
        Iterator begin() const;
        Iterator end() const;
        
        // ToDo: remove once not needed anymore
        class MethodNotImplementedYet{};
    };

    template <class T>
    class SortedList<T>::Iterator
    {
    private:

        const SortedList<T> *sortedList;

        Iterator(const SortedList<T> *sortedList);

        // ToDo: why needed ?
        friend class SortedList<T>;

    public:

        const T& operator*() const;

        Iterator& operator++();

        bool operator==(const Iterator& iterator) const;

    };

    template <class T>
    SortedList<T>::SortedList()
    {
        this->head = NULL;
        this->size = 0;
    }

    template <class T>
    SortedList<T>::~SortedList()
    {
        node *node_pointer = this->head;

        // ToDo: replace with itertor
        while (NULL != node_pointer)
        {
            node *temp = node_pointer->next;
            delete node_pointer;
            this->size--;
            node_pointer = temp;
        }

        this->head = NULL;
    }

    template <class T>
    SortedList<T>::SortedList(const SortedList& sortedList)
    {
        throw MethodNotImplementedYet();
    }

    template <class T>
    SortedList<T>& SortedList<T>::operator=(const SortedList& sortedList)
    {
        throw MethodNotImplementedYet();
    }

    template <class T>
    void SortedList<T>::insert(const T& t)
    {
        node *node_pointer_new = new node;
        node_pointer_new->next = NULL;
        node_pointer_new->data = t;

        if (NULL == this->head)
        {
            this->head = node_pointer_new;
            return;
        }

        // ToDo: implemented as sorted list
        node *node_pointer = this->head;
        while (NULL != node_pointer->next)
        {
            node_pointer = node_pointer->next;
        }

        this->size++;
        node_pointer->next = node_pointer_new;
    }

    template <class T>
    typename SortedList<T>::Iterator SortedList<T>::begin() const
    {
        return Iterator(this);
    }

    template <class T>
    typename SortedList<T>::Iterator SortedList<T>::end() const
    {
        return Iterator(this);
    }

    template <class T>
    SortedList<T>::Iterator::Iterator(const SortedList<T> *sortedList)
    {
        this->sortedList = sortedList;
    }

    template <class T>
    const T& SortedList<T>::Iterator::operator*() const
    {
        return sortedList->head->data;
    }

    template <class T>
    typename SortedList<T>::Iterator& SortedList<T>::Iterator::operator++()
    {
        return *this;
    }

    template <class T>
    bool SortedList<T>::Iterator::operator==(const Iterator& iterator) const
    {
        return true;
    }
}

#endif //_SORTED_LIST
