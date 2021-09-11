#ifndef _GAME_H
#define _GAME_H

#include "Auxiliaries.h"
#include "Character.h"

#include <vector>
#include <iostream>
#include <memory>

// ToDo: don't...
using std::ostream;
using std::shared_ptr;
using std::vector;

namespace mtm
{
    class Game
    {
    private:

        int height;
        int width;

        vector<vector<shared_ptr<Character>>> board;

        void verifyLegalCoordinates(const GridPoint& coordinates) const;
        void verifyCellNotEmpty(const GridPoint& coordinates) const;
        void verifyCellEmpty(const GridPoint& coordinates) const;

    public:

        Game(int height, int widht);

        ~Game();
        Game(const Game& game);
        Game& operator=(const Game& game);

        static shared_ptr<Character> makeCharacter(CharacterType type, Team team, units_t health, units_t ammo, units_t range, units_t power);

        void addCharacter(const GridPoint& coordinates, shared_ptr<Character> character);

        void move(const GridPoint& src_coordinates, const GridPoint& dst_coordinates);

        void reload(const GridPoint& coordinates);

        void attack(const GridPoint& src_coordinates, const GridPoint& dst_coordinates);

        bool isOver(Team* winningTeam=NULL) const;

        friend ostream& operator<<(ostream& os, const Game& game);
    };
}

#endif // _GAME_H
