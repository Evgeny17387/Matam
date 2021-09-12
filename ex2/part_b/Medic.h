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

        ~Medic() = default;
        Medic(const Medic& medic) = default;
        Medic& operator=(const Medic& medic) = default;

        bool isEnoughAmmo(Character* character) const override;

        bool canAttack(Character* character, const GridPoint& coordinates_src, const GridPoint& coordinates_dst) const override;

        units_t getImpactRange() const override;

        void chargeAttackAmmoCost(Character* defender) override;

        units_t attack(Team defender_team, const GridPoint& coordinates_dst, const GridPoint& coordinates_attack) override;

        Character* clone() const override;
    };
}

#endif // _MEDIC_H
