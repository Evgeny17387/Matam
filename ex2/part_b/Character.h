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

        // ToDo: refactor members since some of them already have get method, especially ammo_per_attack and symbol
        Team team;
        units_t ammo;
        units_t range;
        units_t power;
        units_t ammo_per_attack;
        char symbol;

        static int divideToClosestUpperInt(int number, int divider);

    public:

        Character(Team team, units_t health, units_t ammo, units_t range, units_t power, units_t move_range, units_t reload_ammo, units_t ammo_per_attack, char symbol);

        ~Character() = default;
        Character(const Character& character) = default;
        Character& operator=(const Character& character) = default;

        char getSymbol() const;

        int getMoveRange() const;

        void reload();

        Team getTeam() const;

        virtual bool isAttackInRange(const GridPoint& coordinates_src, const GridPoint& coordinates_dst) const;

        virtual bool isEnoughAmmo(Character* character) const;

        virtual bool canAttack(Character* character, const GridPoint& coordinates_src, const GridPoint& coordinates_dst) const = 0;

        virtual units_t getImpactRange() const = 0;

        virtual void chargeAttackAmmoCost(Character* defender) = 0;

        virtual units_t attack(Team defender_team, const GridPoint& coordinates_dst, const GridPoint& coordinates_attack) = 0;

        units_t getHealth() const;

        void updateHealth(units_t impact);

        virtual Character* clone() const = 0;
    };
}

#endif // _CHARACTER_H
