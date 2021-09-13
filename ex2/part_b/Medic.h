#ifndef _MEDIC_H
#define _MEDIC_H

#include "Character.h"

namespace mtm
{
    class Medic: public Character
    {
    private:

    public:

        Medic(Team team, units_t health, units_t ammo, units_t range,
            units_t power);

        ~Medic() = default;
        Medic(const Medic& medic) = default;
        Medic& operator=(const Medic& medic) = default;

        Character* clone() const override;

        bool isEnoughAmmo(Character* character) const override;

        bool canAttack(Character* character, const GridPoint& coordinates_src,
            const GridPoint& coordinates_dst) const override;

        void attack(
            std::vector<std::vector<std::shared_ptr<Character>>>& board,
            const GridPoint& coordinates_dst) override;
    };
}

#endif // _MEDIC_H
