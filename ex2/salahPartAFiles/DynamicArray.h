//
// Created by SKryem on 09/06/2021.
//

#ifndef HW2WET_DYNAMICARRAY_H
#define HW2WET_DYNAMICARRAY_H

namespace mtm{
//
//
const int EXPAND_RATE =2 ;

#include <iostream>
    template<typename T>
    class DynArray{ // variable sized array of unlimited capacity.
    private:
        T* array;
        int len;

        void expand()
        {
            int new_len=this->len *EXPAND_RATE;
            T* temp=new T[new_len];
            for ( int i=0;i<len;i++)
                temp[i]=this->array[i];

            delete[] this->array;
            this->array=temp;
            this->len=new_len;
        }

    public:

        DynArray(): len(1){
            this->array= new T[1];
        }
        explicit DynArray(const int len):len(len){
            this->array=new T[len];
        };
        DynArray(const DynArray& src):len(src.len){

            this->array=new T[len];
            for(int j=0;j<len;j++)
                array[j]=src.array[j];
        };
        DynArray& operator=(const DynArray& src)
        {
            if(this==&src)
                return *this;
            this->len=src.len;
            delete[] this->array;

            this->array=new T[len];
            for(int j=0;j<len;j++)
                array[j]=src.array[j];
            return *this;
        };

        T& operator[](const int index)  // use insert if you want to insert values safely
        {
            return array[index];
        };
        const T& operator[](const int index) const // use insert if you want to insert values safely
        {
            return array[index];
        };

        void insert(const int index, const T& element )
        {
            if(index<0) return;

            if(index>=this->len) {
                this->expand();
                this->array[index]=element;
                return;
            }
            this->array[index]=element;
        }


        ~DynArray(){
            delete[] array;
        };

        int getMaxLen() const
        {
            return this->len;
        };

    };
} //namespace
#endif //HW2WET_DYNAMICARRAY_H