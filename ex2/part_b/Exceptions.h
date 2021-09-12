#ifndef _EXCEPTIONS_H
#define _EXCEPTIONS_H

#include <exception>

namespace mtm
{
    class Exception: public std::exception
    {
    public:
        virtual const char* what() const noexcept = 0;
    };

    class IllegalArgument: public Exception
    {
    public:
        const char* what() const noexcept override
        {
            return "A game related error has occurred: IllegalArgument";
        }
    };

    class IllegalTarget: public Exception
    {
    public:
        const char* what() const noexcept override
        {
            return "A game related error has occurred: IllegalTarget";
        }
    };

    class IllegalCell: public Exception
    {
    public:
        const char* what() const noexcept override
        {
            return "A game related error has occurred: IllegalCell";
        }
    };

    class CellOccupied: public Exception
    {
    public:
        const char* what() const noexcept override
        {
            return "A game related error has occurred: CellOccupied";
        }
    };

    class CellEmpty: public Exception
    {
    public:
        const char* what() const noexcept override
        {
            return "A game related error has occurred: CellEmpty";
        }
    };

    class MoveTooFar: public Exception
    {
    public:
        const char* what() const noexcept override
        {
            return "A game related error has occurred: MoveTooFar";
        }
    };

    class OutOfRange: public Exception
    {
    public:
        const char* what() const noexcept override
        {
            return "A game related error has occurred: OutOfRange";
        }
    };

    class OutOfAmmo: public Exception
    {
    public:
        const char* what() const noexcept override
        {
            return "A game related error has occurred: OutOfAmmo";
        }
    };
}

#endif // _EXCEPTIONS_H
