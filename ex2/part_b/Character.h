#ifndef _CHARACTER_H
#define _CHARACTER_H

#include "Auxiliaries.h"

namespace mtm
{
    class Character
    {
    private:

        CharacterType type;
        units_t health;
        units_t ammo;
        units_t range;
        units_t power;

    public:

        Character(CharacterType type, units_t health, units_t ammo, units_t range, units_t power);

        ~Character();

        Character(const Character& character);

        Character& operator=(const Character& character);
    };
}

#endif // _CHARACTER_H
