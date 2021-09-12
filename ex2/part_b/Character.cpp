#include "Character.h"
#include <ctype.h>

namespace mtm
{
    Character::Character(Team team, units_t health, units_t move_range, units_t reload_ammo, units_t ammo_per_attack, char symbol, units_t ammo, units_t range, units_t power):
        team(team), health(health), move_range(move_range), reload_ammo(reload_ammo), ammo_per_attack(ammo_per_attack), symbol(symbol), ammo(ammo), range(range), power(power)
    {
    }

    char Character::getSymbol() const
    {
        return this->team == POWERLIFTERS ? this->symbol : tolower(this->symbol);
    }

    int Character::getMoveRange() const
    {
        return this->move_range;
    }

    void Character::reload()
    {
        this->ammo += this->reload_ammo;
    }

    Team Character::getTeam() const
    {
        return this->team;
    }

    bool Character::isAttackInRange(const GridPoint& coordinates_src, const GridPoint& coordinates_dst) const
    {
        int attack_range = GridPoint::distance(coordinates_src, coordinates_dst);

        if (attack_range > this->range)
        {
            return false;
        }

        return true;
    }


    bool Character::isEnoughAmmo(Character* character) const
    {
        if (this->ammo < this->ammo_per_attack)
        {
            return false;
        }

        return true;
    }

    void Character::chargeAttackAmmoCost(Character* defender)
    {
        this->ammo -= this->ammo_per_attack;
    }

    units_t Character::getHealth() const
    {
        return this->health;
    }

    void Character::updateHealth(units_t impact)
    {
        this->health += impact;
    }

    int Character::divideToClosestUpperInt(int number, int divider)
    {
        return number % divider == 0 ? number / divider : number / divider + 1;
    }
}
