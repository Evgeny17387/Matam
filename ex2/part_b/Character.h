#ifndef _CHARACTER_H
#define _CHARACTER_H

#include "Auxiliaries.h"

namespace mtm
{
    class Character
    {
    private:

        units_t move_range;

        units_t health;
        units_t ammo;
        units_t range;
        units_t power;

    protected:

        Team team;

    public:

        Character(Team team, units_t health, units_t ammo, units_t range, units_t power, units_t move_range);

        ~Character();

        Character(const Character& character);

        Character& operator=(const Character& character);

        int GetMoveRange() const;

        // ToDo: should be pure virtual once create character in game.cpp is resolved for default
        virtual char GetSymbol() const;
    };
}

#endif // _CHARACTER_H
