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

        int size;

        node *head;

    public:

        SortedList();
        ~SortedList();

        SortedList(const SortedList& sortedList);

        SortedList& operator=(const SortedList& sortedList);

        void insert(const T& t);

        // class Iterator;
        // Itereator begin() const;
        // Itereator end() const;

        // template <class T>
        // class SortedList<T>::Iterator
        // {
        // }

        class MethodNotImplementedYet{};
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

        node_pointer->next = node_pointer_new;
    }

    // template <class T>
    // void SortedList<T>::begin() const
    // {
    //     return Iterator(this, 0);
    // }

    // template <class T>
    // void SortedList<T>::end() const
    // {
    //     return Iterator(this, 0);
    // }
}

#endif //_SORTED_LIST
