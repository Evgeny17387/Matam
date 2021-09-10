#ifndef _TEST_CLASS_H
#define _TEST_CLASS_H

//#include <stdbool.h>
#include <stdio.h>

namespace mtm
{
    class TestClass
    {
    private:
        //int x;
        double y;

        TestClass(int x, double y);
        
    public:
        int x;
        TestClass(const TestClass& test_class) = default;
        friend TestClass externalConstructor(int x, double y);
        ~TestClass() = default;
        bool operator<(const TestClass& test) const;
        void print() const;
        friend bool externalEqualVerification(TestClass c1, TestClass c2);
    };

    TestClass::TestClass(int x, double y)
    {
        this->x = x;
        this->y = y;
    }

    bool TestClass::operator<(const TestClass& test) const
    {
        if (x < test.x)
        {
            return true;
        }

        if (x > test.x)
        {
            return false;
        }

        return y < test.y;
    }

    void TestClass::print() const
    {
        printf("int: %d  ,  double: %lf \n", x, y);
    }


    bool externalEqualVerification(TestClass c1, TestClass c2)
    {
        if (c1.x == c2.x && c1.y == c2.y)
        {
            return true;
        }
        return false;
    }

    TestClass externalConstructor(int x, double y)
    {
        return TestClass(x, y);
    }

}




#endif