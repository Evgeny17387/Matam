#ifndef _CHARACTER_H
#define _CHARACTER_H

#include "Auxiliaries.h"

namespace mtm
{
    class Character
    {
    private:

        units_t health;

        units_t move_range;
        units_t reload_ammo;

    protected:

        Team team;
        units_t ammo;
        units_t range;
        units_t power;

        int divideToClosestUpperInt(int number, int divider) const;

        // ToDo: delete once not needed anymore
        class NotImplementedYet{};

    public:

        Character(Team team, units_t health, units_t ammo, units_t range, units_t power, units_t move_range, units_t reload);

        ~Character();

        Character(const Character& character);

        Character& operator=(const Character& character);

        virtual char getSymbol() const = 0;

        int getMoveRange() const;

        void reload();

        Team getTeam() const;

        virtual bool isAttackInRange(const GridPoint& coordinates_src, const GridPoint& coordinates_dst) const = 0;

        virtual bool isEnoughAmmo(Character* character) const = 0;

        virtual bool canAttack(Character* character, bool is_destination_equals_source) const = 0;

        virtual units_t getImpactRange() const = 0;

        virtual void chargeAttackAmmoCost(Character* defender) = 0;

        virtual units_t attack(Team defender_team, const GridPoint& coordinates_dst, const GridPoint& coordinates_attack) = 0;

        units_t getHealth() const;

        void updateHealth(units_t impact);

        virtual Character* clone() const = 0;
    };
}

#endif // _CHARACTER_H
