#include "Exceptions.h"

#include <vector>
#include <iostream>

using std::ostream;

using namespace mtm;

const char* IllegalTarget::what() const noexcept
{
    return "A game related error has occurred: IllegalTarget\n";
};

const char* IllegalArgument::what() const noexcept
{
    return "A game related error has occurred: IllegalArgument\n";
};

const char* IllegalCell::what() const noexcept
{
    return "A game related error has occurred: IllegalCell\n";
};

const char* CellOccupied::what() const noexcept
{
    return "A game related error has occurred: CellOccupied\n";
};

const char* CellEmpty::what() const noexcept
{
    return "A game related error has occurred: CellEmpty\n";
};

const char* MoveTooFar::what() const noexcept
{
    return "A game related error has occurred: MoveTooFar\n";
};

const char* OutOfRange::what() const noexcept
{
    return "A game related error has occurred: OutOfRange\n";
};

const char* OutOfAmmo::what() const noexcept
{
    return "A game related error has occurred: OutOfAmmo\n";
};
