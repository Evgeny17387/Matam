//
// Created by SKryem on 28/08/2021.
//

#ifndef MAIN_CPP_SORTEDLIST_H
#define MAIN_CPP_SORTEDLIST_H
 
#include "iostream"
#include "DynamicArray.h"
#include <stdexcept>
namespace mtm {

template<typename T>

  class SortedList {
    DynArray<T*> array;  // can not make an array for <T> because we can't assume it has a parameterless constructor,
                         // we can make an array for pointers though. DynArray's size is unlimited.
    int size;

    static void  swap(T*& a, T*& b)
    {
        T* temp=a;
        a=b;
        b=temp;
    }

    static int bubble(DynArray<T*>& arr,  int length);

    static void bubbleSort(DynArray<T*>& arr,  int length) // used to keep the array sorted
    {
        int not_sorted = 1;
        while ((length > 1) && not_sorted) {
            not_sorted = bubble(arr, length--);
        }
    }

  public:
    class const_iterator;

    SortedList()
    {
        this->array=DynArray<T*>();
        size=0;
    }

    ~SortedList()
    {
      for(int i=0;i<this->size;i++)
      {
        delete array[i];
      }
    }

    SortedList (const SortedList& src);
    SortedList& operator=(const SortedList& src);

    bool operator==(const SortedList& list2) const
    {
        return this==&list2;
    }

    void insert(const T& element);

    void remove(const const_iterator& itr) ;

    int length() const
    {
        return this->size;
    }

    const_iterator begin() const
    {
        return const_iterator(this,0);
    }

    const_iterator end() const
    {
      return const_iterator(this,size);
    }


template <class S>      
    SortedList filter( S predicate) const
    {
        SortedList new_list;

        for(int i=0;i<this->size;i++)
        {
            T element=*array[i];
            if(predicate(element))
            {
            new_list.insert(element);
            }
        }
        return new_list;
    }
/*****************************************************************/
template <class S>
      SortedList apply( S func) const
    {
        SortedList new_list;
        for(int i=0;i<this->size;i++)
        {
            new_list.insert(func(*array[i]));
        }
        return new_list;
        }

      };
/****************************************************************************/
/****************************    Implementaion       ************************/
/****************************************************************************/

template<typename T>
    int SortedList<T>::bubble(DynArray<T *> &arr, int length) 
    {
        int i, swapped = 0;
        for (i = 1; i < length; ++i)
            if (*arr[i] < *arr[i - 1]) { // COMPARE ACTUAL ELEMENTS, NOT POINTERS. use * to dereference the pointer and
                // get the actual element<T> (do not forget that our array contains pointers to
                // the elements <T*> and not the elements themselves).
                swap(arr[i], arr[i - 1]);
                swapped = 1;
            }
        return swapped;
    }
/****************************************************************************/

template<typename T>
    SortedList<T>::SortedList(const SortedList<T> &src):size(src.size) 
    {

        this->array = DynArray<T *>();
        for (int i = 0; i < src.size; i++) {
            T *new_element;
            try {
                new_element = new T(*src.array[i]);// make a copy of the element
            }
            catch (const std::bad_alloc &e) // allocation failed, free what's been allocated
            {
                for (int k = 0; k < i; k++)
                {
                    delete array[k];
                }
                throw std::bad_alloc();
            }

            this->array.insert(i, new_element);
        }

    }
/****************************************************************************/

    template<typename T>
    SortedList<T> &SortedList<T>::operator=(const SortedList<T> &src) 
    // in case of allocation failure, *this is unchanged( maintains original value)
    {
        if (*this == src) 
        {
            return *this;
        }
        auto temp_array = this->array; // backup the elements in case of allocation failure
        this->array = DynArray<T *>();
        for (int j = 0; j < src.size; j++) 
        {
            T *new_element;
            try {
                new_element = new T(*src.array[j]);// make a copy of the element
            }
            catch (const std::bad_alloc &e) 
            {
                for (int k = 0; k < j; k++) 
                {
                    delete array[k]; // free allocated copies
                }
                this->array = temp_array; // restore old elements
                throw std::bad_alloc();
            }
            this->array.insert(j, new_element);
        }
        // only free old memory if the assignment succeeded.
        for (int i = 0; i < this->size; i++) {
            delete temp_array[i];
        }
        this->size = src.size;
        return *this;
    }
/****************************************************************************/

    template<typename T>
    void SortedList<T>::insert(const T &element) 
    {
        T *new_data = new T(element);
        array.insert(size, new_data); // insert expands the array if needed.
        size++;
        bubbleSort(this->array, this->size);
    }
/****************************************************************************/

    template<typename T>
    void SortedList<T>::remove(const const_iterator &itr) 
    {
        int index = itr.index;
        if (index >= size) 
        {
            throw std::out_of_range("trying to remove an out of range iterator");
        }
        delete array[index];
        array[index] = array[size - 1];

        size--;
        bubbleSort(this->array, this->size);
    }
    /***********************************************************************************************************/
    /***********************************************************************************************************/
    /*************************************          Iterator                ************************************/
    /***********************************************************************************************************/
    /***********************************************************************************************************/
    template<typename T>
    class SortedList<T>::const_iterator {

        const SortedList<T> *sorted_list;
        int index;

        const_iterator(const SortedList<T> *list, const int index) : sorted_list(list), index(index) {};

        friend class SortedList<T>;

    public:


        const_iterator(const const_iterator &src) = default;

        ~const_iterator() = default;

        const_iterator &operator=(const const_iterator &src) = default;


        const_iterator &operator++() 
        {
            if (this->index >= this->sorted_list->size) 
            {
                throw std::out_of_range("trying to increment iterator makes it out of range");
            }
            this->index++;
            return *this;
        }

        const_iterator operator++(int) 
        {
            const_iterator temp(*this);
            ++*this;
            return temp;
        }

        bool operator==(const const_iterator &itr2) const 
        {
            return (itr2.index == this->index) && (itr2.sorted_list == this->sorted_list);
        }


        const T &operator*() const 
        {

            if (index >= sorted_list->size) 
            {
                throw std::out_of_range("trying to dereference an out of range iterator");
            }
            return *sorted_list->array[index];
        }

    };
}//namespace
#endif //MAIN_CPP_SORTEDLIST_H
