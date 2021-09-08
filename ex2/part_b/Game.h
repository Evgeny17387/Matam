#ifndef _GAME_H
#define _GAME_H

#include "Auxiliaries.h"
#include "Character.h"

namespace mtm
{
    class Game
    {
    private:

        int height;
        int widht;

    public:
        Game(int height, int widht);

        ~Game();

        Game(const Game& game);

        Game& operator=(const Game& game);

        static Character makeCharacter(CharacterType type, Team team, units_t health, units_t ammo, units_t range, units_t power);

        void addCharacter(const GridPoint& coordinates, Character character);

        class IllegalArgument{};
    };
}

#endif // _GAME_H
