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
        const char* what() const noexcept override;
    };

    class IllegalTarget: public Exception
    {
    public:
        const char* what() const noexcept override;
    };

    class IllegalCell: public Exception
    {
    public:
        const char* what() const noexcept override;
    };

    class CellOccupied: public Exception
    {
    public:
        const char* what() const noexcept override;
    };

    class CellEmpty: public Exception
    {
    public:
        const char* what() const noexcept override;
    };

    class MoveTooFar: public Exception
    {
    public:
        const char* what() const noexcept override;
    };

    class OutOfRange: public Exception
    {
    public:
        const char* what() const noexcept override;
    };

    class OutOfAmmo: public Exception
    {
    public:
        const char* what() const noexcept override;
    };
}

#endif // _EXCEPTIONS_H
