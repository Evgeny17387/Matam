#ifndef _CHARACTER_H
#define _CHARACTER_H

#include "Auxiliaries.h"

#include <vector>
#include <memory>

namespace mtm
{
    class Character
    {
    private:

        Team team;
        units_t health;
        units_t move_range;
        units_t reload_ammo;
        units_t ammo_per_attack;
        char symbol;

    protected:

        // ToDo: refactor members after final version
        units_t ammo;
        units_t range;
        units_t power;

        static int divideToClosestUpperInt(int number, int divider);

    public:

        Character(Team team, units_t health, units_t move_range, units_t reload_ammo, units_t ammo_per_attack, char symbol, units_t ammo, units_t range, units_t power);

        ~Character() = default;
        Character(const Character& character) = default;
        Character& operator=(const Character& character) = default;

        char getSymbol() const;

        int getMoveRange() const;

        Team getTeam() const;

        units_t getHealth() const;
        void updateHealth(units_t impact);

        void reload();

        virtual bool isAttackInRange(const GridPoint& coordinates_src, const GridPoint& coordinates_dst) const;
        virtual bool isEnoughAmmo(Character* character) const;

        virtual Character* clone() const = 0;

        virtual bool canAttack(Character* character, const GridPoint& coordinates_src, const GridPoint& coordinates_dst) const = 0;

        virtual void attack(std::vector<std::vector<std::shared_ptr<Character>>>& board, const GridPoint& coordinates_dst) = 0;
    };
}

#endif // _CHARACTER_H
