#include "Medic.h"

using namespace mtm;

Medic::Medic(units_t health, units_t ammo, units_t range, units_t power): Character(health, ammo, range, power)
{
}

Medic::~Medic()
{
}

Medic::Medic(const Medic& medic): Character(medic)
{
}

Medic& Medic::operator=(const Medic& medic)
{
    // ToDo: how to implement ?
    return *this;
}
