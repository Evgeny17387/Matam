#ifndef _SOLDIER_H
#define _SOLDIER_H

#include "Character.h"

namespace mtm
{
    class Soldier: public Character
    {
    private:

        int max_range_affect;
        units_t reduced_damage;

        static bool isOnTheSameLine(const GridPoint& coordinates_src, const GridPoint& coordinates_dst);

        void singleAttack(Character* defender, const GridPoint& coordinates_dst, const GridPoint& attack_coordinates);

    public:

        Soldier(Team team, units_t health, units_t ammo, units_t range, units_t power);

        ~Soldier() = default;
        Soldier(const Soldier& soldier) = default;
        Soldier& operator=(const Soldier& soldier) = default;

        Character* clone() const override;

        bool canAttack(Character* character, const GridPoint& coordinates_src, const GridPoint& coordinates_dst) const override;

        void attack(std::vector<std::vector<std::shared_ptr<Character>>>& board, const GridPoint& coordinates_dst) override;
    };
}

#endif // _SOLDIER_H
