#ifndef _SORTED_LIST
#define _SORTED_LIST

using std::out_of_range;

namespace mtm
{
    template <class T>
    class SortedList
    {
    private:

        // ToDo: consider replacing with struct
        // ToDo: encapsulate data members
        class node
        {
        public:
            T data;
            node *next;

            node(const T& t): data(t)
            {
                this->next = NULL;
            }
        };

        node *head;

        int size;

    public:

        SortedList();
        ~SortedList();

        SortedList(const SortedList& sortedList);

        SortedList& operator=(const SortedList& sortedList);

        void insert(const T& t);

        // ToDo: what does it mean to indicate here "class Iterator" ?
        class Iterator;

        void remove(const Iterator& iterator);

        int length() const;

        template <class Condition>
        SortedList filter(Condition condition) const;

        template <class Apply>
        SortedList apply(Apply apply) const;

        Iterator begin() const;
        Iterator end() const;
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

        node *node_new = new node(node_list_original->data);

        this->head = node_new;

        this->size++;

        node *node_list_new = this->head;
        node_list_original = node_list_original->next;

        while (NULL != node_list_original)
        {
            node *node_new = new node(node_list_original->data);

            node_list_new->next = node_new;
            node_list_new = node_list_new->next;

            this->size++;

            node_list_original = node_list_original->next;
        }
    }

    template <class T>
    SortedList<T>& SortedList<T>::operator=(const SortedList& sortedList)
    {
        this->head = NULL;
        this->size = 0;

        // ToDo: logic might be common among all operator= and copy constructor
        if (NULL == sortedList.head)
        {
            *this;
        }

        node *node_list_original = sortedList.head;

        node *node_new = new node(node_list_original->data);

        this->head = node_new;

        this->size++;

        node *node_list_new = this->head;
        node_list_original = node_list_original->next;

        // ToDo: might use just iterator and insert instead
        while (NULL != node_list_original)
        {
            node *node_new = new node(node_list_original->data);

            node_list_new->next = node_new;
            node_list_new = node_list_new->next;

            this->size++;

            node_list_original = node_list_original->next;
        }
        
        return *this;
    }

    template <class T>
    void SortedList<T>::insert(const T& t)
    {
        node *node_pointer_new = new node(t);

        if (NULL == this->head)
        {
            this->head = node_pointer_new;
            this->size++;
            return;
        }

        // ToDo: implemented as sorted list
        // node *node_pointer = this->head;
        // while (NULL != node_pointer->next)
        // {
        //     node_pointer = node_pointer->next;
        // }

        // this->size++;
        // node_pointer->next = node_pointer_new;

        node **head_ref = &(this->head);

        if ((NULL == *head_ref) || !((*head_ref)->data < node_pointer_new->data))
        {
            node_pointer_new->next = *head_ref;
            *head_ref = node_pointer_new;
            this->size++;
        }
        else
        {
            node *current = *head_ref;

            while ((current->next != NULL) && (current->next->data < node_pointer_new->data))
            {
                current = current->next;
            }

            node_pointer_new->next = current->next;
            current->next = node_pointer_new;
            this->size++;
        }
    }

    template <class T>
    void SortedList<T>::remove(const Iterator& iterator)
    {
        // ToDo: check if out of range \ null
        if (0 == iterator.index)
        {
            node *next = this->head->next;
            delete this->head;
            this->size--;
            this->head = next;
        }
        else
        {
            node *current_node = this->head;
            int index_temp = iterator.index;
            while (index_temp > 1)
            {
                current_node = current_node->next;
                index_temp--;
            }

            node *next = current_node->next->next;
            delete current_node->next;
            this->size--;
            current_node->next = next;
        }
    }

    template <class T>
    int SortedList<T>::length() const
    {
        return this->size;
    }

    template <class T>
    template <class Condition>
    SortedList<T> SortedList<T>::filter(Condition condition) const
    {
        SortedList<T> sortedList;

        for (typename SortedList<T>::Iterator it = this->begin(); !(it == this->end()); ++it)
        {
            if (condition(*it))
            {
                sortedList.insert(*it);
            }
        }

        return sortedList;
    }

    template <class T>
    template <class Apply>
    SortedList<T> SortedList<T>::apply(Apply apply) const
    {
        SortedList<T> sortedList;

        for (typename SortedList<T>::Iterator it = this->begin(); !(it == this->end()); ++it)
        {
            sortedList.insert(apply(*it));
        }

        // ToDo: should it be sorted again ? in case the new list is not sorted

        return sortedList;
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
