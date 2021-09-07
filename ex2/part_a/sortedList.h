#ifndef _SORTED_LIST
#define _SORTED_LIST

using std::out_of_range;

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

        int index;

        Iterator(const SortedList<T> *sortedList, int index);

        // ToDo: is there a difference putting it here or in public section ?
        friend class SortedList<T>;

    public:

        ~Iterator();

        Iterator(const Iterator& iterator);

        Iterator& operator=(const Iterator& iterator);

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
        this->head = NULL;
        this->size = 0;

        if (NULL == sortedList.head)
        {
            return;
        }

        node *node_list_original = sortedList.head;

        node *node_new = new node;
        node_new->next = NULL;
        node_new->data = node_list_original->data;

        this->head = node_new;

        this->size++;

        node *node_list_new = this->head;
        node_list_original = node_list_original->next;

        while (NULL != node_list_original)
        {
            node *node_new = new node;
            node_new->next = NULL;
            node_new->data = node_list_original->data;

            node_list_new->next = node_new;
            node_list_new = node_list_new->next;

            this->size++;

            node_list_original = node_list_original->next;
        }
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
        return Iterator(this, 0);
    }

    template <class T>
    typename SortedList<T>::Iterator SortedList<T>::end() const
    {
        return Iterator(this, this->size);
    }

    template <class T>
    SortedList<T>::Iterator::Iterator(const SortedList<T> *sortedList, int index)
    {
        this->sortedList = sortedList;
        this->index = index;
    }

    template <class T>
    SortedList<T>::Iterator::~Iterator()
    {
    }

    template <class T>
    SortedList<T>::Iterator::Iterator(const Iterator& iterator)
    {
        this->sortedList = iterator.sortedList;
        this->index = iterator.index;
    }

    template <class T>
    //ToDo: why typename needed ?
    typename SortedList<T>::Iterator& SortedList<T>::Iterator::operator=(const Iterator& iterator)
    {
        this->sortedList = iterator.sortedList;
        this->index = iterator.index;

        return *this;
    }

    template <class T>
    const T& SortedList<T>::Iterator::operator*() const
    {
        // ToDo: what if this is NULL ? who should check that index is valid ?
        // ToDo: maybe implement this logic in Iterator Constructor
        node *node = sortedList->head;
        int index_temp = index;

        while (index_temp > 0)
        {
            node = node->next;
            index_temp--;
        }

        return node->data;
    }

    template <class T>
    typename SortedList<T>::Iterator& SortedList<T>::Iterator::operator++()
    {
        // ToDo: should it be -1 or this way ?
        if (this->index >= sortedList->size)
        {
            // ToDo: should add here some log ?
            throw out_of_range("");
        }

        this->index++;

        return *this;
    }

    template <class T>
    bool SortedList<T>::Iterator::operator==(const Iterator& iterator) const
    {
        return this->index == iterator.index;
    }
}

#endif //_SORTED_LIST
