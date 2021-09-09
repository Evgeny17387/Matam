#ifndef _MEDIC_H
#define _MEDIC_H

#include "Character.h"

namespace mtm
{
    class Medic: public Character
    {
    private:

    public:

        Medic(Team team, units_t health, units_t ammo, units_t range, units_t power);

        ~Medic();

        Medic(const Medic& medic);

        Medic& operator=(const Medic& medic);

        bool isAttackInRange(int range) const override;

        bool isEnoughAmmo(Team defender_team) const override;

        bool canAttack(bool is_destination_empty, bool is_destination_equals_source) const override;

        char getSymbol() const override;

        // ToDo: delete once not needed anymore
        class NotImplementedYet{};
    };
}

#endif // _MEDIC_H
