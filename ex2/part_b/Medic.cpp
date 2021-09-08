#include "Medic.h"

using namespace mtm;

Medic::Medic(CharacterType type, units_t health, units_t ammo, units_t range, units_t power): Character(type, health, ammo, range, power)
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
