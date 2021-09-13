#ifndef _SNIPER_H
#define _SNIPER_H

#include "Character.h"

namespace mtm
{
    class Sniper: public Character
    {
    private:

        int regular_shots_count;

    public:

        Sniper(Team team, units_t health, units_t ammo, units_t range,
            units_t power);

        ~Sniper() = default;
        Sniper(const Sniper& sniper) = default;
        Sniper& operator=(const Sniper& sniper) = default;

        Character* clone() const override;

        bool isAttackInRange(const GridPoint& coordinates_src,
            const GridPoint& coordinates_dst) const override;

        bool canAttack(Character* character, const GridPoint& coordinates_src,
            const GridPoint& coordinates_dst) const override;

        void attack(
            std::vector<std::vector<std::shared_ptr<Character>>>& board,
            const GridPoint& coordinates_dst) override;
    };
}

#endif // _SNIPER_H
