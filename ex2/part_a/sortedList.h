#ifndef _SORTED_LIST
#define _SORTED_LIST

#include <stdexcept>
#include <iostream>
#include <cassert>

using std::out_of_range;

namespace mtm
{
    template <class T>
    class SortedList
    {
    private:

        // ToDo: maybe write in different file ?
        // ToDo: consider replacing with struct
        // ToDo: encapsulate data members
        class node
        {
        public:
            T data;
            node *next;

            node(const T& t): data(t), next(NULL)
            {
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

        class const_iterator;

        void remove(const const_iterator& const_iterator);

        int length() const;

        template <class Condition>
        SortedList filter(const Condition& condition) const;

        template <class Apply>
        SortedList apply(const Apply& apply) const;

        const_iterator begin() const;
        const_iterator end() const;
    };

    // ToDo: maybe write in different file ?
    template <class T>
    class SortedList<T>::const_iterator
    {
    private:

        const SortedList<T> *sortedList;

        int index;

        const_iterator(const SortedList<T> *sortedList, int index);

        friend class SortedList<T>;

    public:

        ~const_iterator() = default;
        const_iterator(const const_iterator& const_iterator) = default;
        const_iterator& operator=(const const_iterator& const_iterator) = default;

        const T& operator*() const;

        const_iterator& operator++();
        // ToDo: there wasn't such operator in the requirements, maybe delete just before handing the assignment ?
        const_iterator operator++(int);

        bool operator==(const const_iterator& const_iterator) const;
    };

    template <class T>
    SortedList<T>::SortedList(): head(NULL), size(0)
    {
    }

    template <class T>
    SortedList<T>::~SortedList()
    {
        node* node_pointer = this->head;

        while (NULL != node_pointer)
        {
            node* temp = node_pointer->next;
            delete node_pointer;
            this->size--;
            node_pointer = temp;
        }
        assert(this->size == 0);

        this->head = NULL;
    }

    template <class T>
    SortedList<T>::SortedList(const SortedList& sortedList): head(NULL), size(0)
    {
        // ToDo: logic might be common among operator= and copy constructor

        if (NULL == sortedList.head)
        {
            return;
        }

        this->head = new node(sortedList.head->data);
        this->size++;

        node* node_list_new = this->head;
        node* node_list_original = sortedList.head->next;
        while (node_list_original != NULL)
        {
            // ToDo: add case for memory exception
            node_list_new->next = new node(node_list_original->data);
            this->size++;
            node_list_new = node_list_new->next;
            node_list_original = node_list_original->next;
        }
    }

    template <class T>
    SortedList<T>& SortedList<T>::operator=(const SortedList& sortedList)
    {
        if (this == &sortedList)
        {
            return *this;
        }

        node* node_list_new = this->head;
        while (node_list_new != NULL)
        {
            node* node_temp = node_list_new->next;
            delete node_list_new;
            this->size--;
            node_list_new = node_temp;
        }

        this->head = NULL;
        this->size = 0;

        // ToDo: logic might be common among operator= and copy constructor

        if (NULL == sortedList.head)
        {
            return *this;
        }

        this->head = new node(sortedList.head->data);
        this->size++;

        node_list_new = this->head;
        node* node_list_original = sortedList.head->next;
        while (NULL != node_list_original)
        {
            // ToDo: add case for memory exception
            node_list_new->next = new node(node_list_original->data);
            this->size++;
            node_list_new = node_list_new->next;
            node_list_original = node_list_original->next;
        }
        
        return *this;
    }

    template <class T>
    void SortedList<T>::insert(const T& t)
    {
        node* node_pointer_new = new node(t);

        if (NULL == this->head)
        {
            this->head = node_pointer_new;
            this->size++;
            return;
        }

        node** head_ref = &(this->head);

        if ((NULL == *head_ref) || !((*head_ref)->data < node_pointer_new->data))
        {
            node_pointer_new->next = *head_ref;
            *head_ref = node_pointer_new;
            this->size++;
        }
        else
        {
            node* current = *head_ref;

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
    void SortedList<T>::remove(const const_iterator& const_iterator)
    {
        // ToDo: should the iterator be checked for validity ?
        if (0 == const_iterator.index)
        {
            node* temp = this->head->next;
            delete this->head;
            this->size--;
            this->head = temp;
        }
        else
        {
            node* current_node = this->head;
            int index_temp = const_iterator.index;
            while (index_temp > 1)
            {
                current_node = current_node->next;
                index_temp--;
            }

            node* temp = current_node->next->next;
            delete current_node->next;
            this->size--;
            current_node->next = temp;
        }
    }

    template <class T>
    int SortedList<T>::length() const
    {
        return this->size;
    }

    template <class T>
    template <class Condition>
    SortedList<T> SortedList<T>::filter(const Condition& condition) const
    {
        SortedList<T> sortedList;

        for (typename SortedList<T>::const_iterator it = this->begin(); !(it == this->end()); ++it)
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
    SortedList<T> SortedList<T>::apply(const Apply& apply) const
    {
        SortedList<T> sortedList;

        for (typename SortedList<T>::const_iterator it = this->begin(); !(it == this->end()); ++it)
        {
            sortedList.insert(apply(*it));
        }

        return sortedList;
    }

    template <class T>
    // ToDo: is class and typename can be both indicated here meaning the same ?
    typename SortedList<T>::const_iterator SortedList<T>::begin() const
    {
        return const_iterator(this, 0);
    }

    template <class T>
    typename SortedList<T>::const_iterator SortedList<T>::end() const
    {
        return const_iterator(this, this->size);
    }

    template <class T>
    SortedList<T>::const_iterator::const_iterator(const SortedList<T> *sortedList, int index): sortedList(sortedList), index(index)
    {
    }

    template <class T>
    const T& SortedList<T>::const_iterator::operator*() const
    {
        node* node = sortedList->head;
        int index_temp = index;

        while (index_temp > 0)
        {
            // ToDo: what if this is NULL ? who should check that index is valid ?
            node = node->next;
            index_temp--;
        }

        return node->data;
    }

    template <class T>
    typename SortedList<T>::const_iterator& SortedList<T>::const_iterator::operator++()
    {
        if (this->index >= sortedList->size)
        {
            throw out_of_range("const_iterator has already reached last item in the list");
        }

        this->index++;

        return *this;
    }

    template <class T>
    typename SortedList<T>::const_iterator SortedList<T>::const_iterator::operator++(int)
    {
        if (this->index >= sortedList->size)
        {
            throw out_of_range("const_iterator has already reached last item in the list");
        }

        SortedList<T>::const_iterator temp = *this;

        this->index++;

        return temp;
    }

    template <class T>
    bool SortedList<T>::const_iterator::operator==(const const_iterator& const_iterator) const
    {
        return (this->sortedList == const_iterator.sortedList) && (this->index == const_iterator.index);
    }
}

#endif //_SORTED_LIST
