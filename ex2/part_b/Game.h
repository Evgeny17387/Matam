#ifndef _GAME_H
#define _GAME_H

#include "Auxiliaries.h"
#include "Character.h"

#include <vector>
#include <iostream>

using std::ostream;

namespace mtm
{
    class Game
    {
    private:

        int height;
        int width;

        std::vector<std::vector<Character*>> board;

    public:
        Game(int height, int widht);

        ~Game();

        Game(const Game& game);

        Game& operator=(const Game& game);

        static Character* makeCharacter(CharacterType type, Team team, units_t health, units_t ammo, units_t range, units_t power);

        void addCharacter(const GridPoint& coordinates, Character *character);

        void move(const GridPoint& src_coordinates, const GridPoint& dst_coordinates);

        friend ostream& operator<<(ostream& os, const Game& game);

        class IllegalArgument{};
        class IllegalCell{};
        class CellOccupied{};
        class CellEmpty{};
        class MoveTooFar{};

        class NotImplementedYet{};
    };
}

#endif // _GAME_H
