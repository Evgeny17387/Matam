#ifndef _CHARACTER_H
#define _CHARACTER_H

#include "Auxiliaries.h"

namespace mtm
{
    class Character
    {
    private:

        units_t health;
        units_t range;

        units_t move_range;
        units_t reload_ammo;

    protected:

        Team team;
        units_t ammo;
        units_t power;

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

        bool isAttackInRange(int attack_range) const;

        units_t getHealth() const;

        void updateHealth(units_t impact);

        virtual bool isEnoughAmmo(Team defender_team) const = 0;

        virtual bool canAttack(bool is_destination_empty, bool is_destination_equals_source) const = 0;

        virtual units_t attack(Team defender_team) = 0;
    };
}

#endif // _CHARACTER_H
