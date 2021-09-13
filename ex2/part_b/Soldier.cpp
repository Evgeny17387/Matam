#include "Soldier.h"

using std::shared_ptr;

namespace mtm
{
    const units_t MOVE_RANGE = 3;

    const units_t RELOAD_AMMO = 3;

    const units_t AMMO_PER_ATTACK = 1;

    const units_t NO_DAMAGE = 0;

    const char SYMBOL = 'S';

    Soldier::Soldier(Team team, units_t health, units_t ammo, units_t range, units_t power):
        Character(team, health, MOVE_RANGE, RELOAD_AMMO, AMMO_PER_ATTACK, SYMBOL, ammo, range, power)
    {
        this->max_range_affect = divideToClosestUpperInt(this->range, 3);
        this->reduced_damage = divideToClosestUpperInt(this->power, 2);
    }

    Character* Soldier::clone() const
    {
        return new Soldier(*this);
    }

    bool Soldier::isOnTheSameLine(const GridPoint& coordinates_src, const GridPoint& coordinates_dst)
    {
        if ((coordinates_src.col != coordinates_dst.col) && (coordinates_src.row != coordinates_dst.row))
        {
            return false;
        }

        return true;
    }

    bool Soldier::canAttack(Character* character, const GridPoint& coordinates_src,
        const GridPoint& coordinates_dst) const
    {
        if (!isOnTheSameLine(coordinates_src, coordinates_dst))
        {
            return false;
        }

        return true;
    }

    void Soldier::singleAttack(Character* defender, const GridPoint& coordinates_dst,
        const GridPoint& attack_coordinates)
    {
        units_t impact = 0;

        if (defender->getTeam() == this->getTeam())
        {
            impact = NO_DAMAGE;
        }
        else
        {
            if (coordinates_dst == attack_coordinates)
            {
                impact = -this->power;
            }
            else
            {
                int attack_range = GridPoint::distance(coordinates_dst, attack_coordinates);

                if (attack_range > max_range_affect)
                {
                    impact = NO_DAMAGE;
                }
                else
                {
                    impact = -reduced_damage;
                }
            }
        }

        defender->updateHealth(impact);
    }

    void Soldier::attack(std::vector<std::vector<std::shared_ptr<Character>>>& board, const GridPoint& coordinates_dst)
    {
        this->ammo -= AMMO_PER_ATTACK;

        for (int row_diff = -this->max_range_affect; row_diff <= this->max_range_affect; row_diff++)
        {
            for (int col_diff = -this->max_range_affect; col_diff <= this->max_range_affect; col_diff++)
            {
                GridPoint attack_coordinates = coordinates_dst;
                attack_coordinates.row += row_diff;
                attack_coordinates.col += col_diff;

                if ((attack_coordinates.row < 0) ||
                    (attack_coordinates.col < 0) ||
                    (attack_coordinates.row >= (int)board[0].size()) ||
                    (attack_coordinates.col >= (int)board.size()))
                {
                    continue;
                }

                shared_ptr<Character> defender = board[attack_coordinates.col][attack_coordinates.row];

                if (defender == NULL)
                {
                    continue;
                }

                singleAttack(defender.get(), coordinates_dst, attack_coordinates);
            }
        }
    }
}
