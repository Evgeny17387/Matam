#ifndef _MEDIC_H
#define _MEDIC_H

#include "Character.h"

namespace mtm
{
    class Medic: public Character
    {
    private:

    public:

        Medic(CharacterType type, units_t health, units_t ammo, units_t range, units_t power);

        ~Medic();

        Medic(const Medic& medic);

        Medic& operator=(const Medic& medic);

        // ToDo: delete once not needed anymore
        class NotImplementedYet{};
    };
}

#endif // _MEDIC_H
