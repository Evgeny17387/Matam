#include "Medic.h"

using namespace mtm;

const units_t MOVE_RANGE = 5;

Medic::Medic(Team team, units_t health, units_t ammo, units_t range, units_t power): Character(team, health, ammo, range, power, MOVE_RANGE)
{
}

Medic::~Medic()
{
    // ToDo: how to implement ?
}

Medic::Medic(const Medic& medic): Character(medic)
{
}

Medic& Medic::operator=(const Medic& medic)
{
    // ToDo: how to implement ?
    throw NotImplementedYet();
    return *this;
}

char Medic::GetSymbol() const
{
    // Should it be implemented this way ?
    return this->team == POWERLIFTERS ? 'M' : 'm';
}
