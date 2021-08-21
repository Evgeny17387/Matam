#include <stdlib.h>
#include "../chessSystem.h"
#include "../test_utilities.h"

/*The number of tests*/
#define NUMBER_TESTS 7

bool testChessAddTournament() {
    ChessSystem chess = chessCreate();
    ASSERT_TEST(chessAddTournament(chess, 1, 4, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 2, 5, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 1, 10, "Paris") == CHESS_TOURNAMENT_ALREADY_EXISTS);

    chessDestroy(chess);
    return true;
}

bool testChessRemoveTournament() {
    ChessSystem chess = chessCreate();
    ASSERT_TEST(chessAddTournament(chess, 1, 4, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 2, 4, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessRemoveTournament(chess, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemoveTournament(chess, 1) == CHESS_TOURNAMENT_NOT_EXIST);
    ASSERT_TEST(chessAddTournament(chess, 1, 4, "Paris") == CHESS_SUCCESS);

    chessDestroy(chess);
    return true;
}

bool testChessAddGame(){
    ChessSystem chess = chessCreate();
    ASSERT_TEST(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 2000) == CHESS_TOURNAMENT_NOT_EXIST);
    ASSERT_TEST(chessAddTournament(chess, 1, 4, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 2000) == CHESS_SUCCESS);

    chessDestroy(chess);
    return true;
}

bool testChessPrintLevelsAndTournamentStatistics(){
    FILE* file_levels = fopen("./tests/player_levels_your_output.txt", "w");
    if(!file_levels){
        printf("test failed to open file, make sure you have the folder tests in the same folder with chessSystem"
               "excutable file and you have write permissions for the file /tests/player_levels_your_output.txt");
        return false;
    }
    ChessSystem chess = chessCreate();
    ASSERT_TEST(chessAddTournament(chess, 1, 4, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 2000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 1, 3, FIRST_PLAYER, 3000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 3, 2, SECOND_PLAYER, 3000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 4, 1, SECOND_PLAYER, 1000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 2, 4, FIRST_PLAYER, 3500) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 3, 4, DRAW, 400) == CHESS_SUCCESS);
    ASSERT_TEST(chessEndTournament(chess, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessSavePlayersLevels(chess, file_levels) == CHESS_SUCCESS);
    ASSERT_TEST(chessSaveTournamentStatistics(chess, "./tests/tournament_statistics_your_output.txt") == CHESS_SUCCESS);
    chessDestroy(chess);
    fclose(file_levels);
    return true;
}

bool testBadArguments()
{
    ChessSystem chess_system = chessCreate();

    ASSERT_TEST(chessAddTournament(NULL, 1, 1, "London") == CHESS_NULL_ARGUMENT);
    ASSERT_TEST(chessAddTournament(chess_system, 1, 1, NULL) == CHESS_NULL_ARGUMENT);
    ASSERT_TEST(chessAddTournament(chess_system, 0, 1, "London") == CHESS_INVALID_ID);
    ASSERT_TEST(chessAddTournament(chess_system, 1, 0, "London") == CHESS_INVALID_MAX_GAMES);
    ASSERT_TEST(chessAddTournament(chess_system, 1, 1, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess_system, 1, 1, "London") == CHESS_TOURNAMENT_ALREADY_EXISTS);

    ASSERT_TEST(chessAddGame(NULL, 1, 1, 2, FIRST_PLAYER, 1) == CHESS_NULL_ARGUMENT);
    ASSERT_TEST(chessAddGame(chess_system, 0, 1, 2, FIRST_PLAYER, 1) == CHESS_INVALID_ID);
    ASSERT_TEST(chessAddGame(chess_system, 1, 0, 2, FIRST_PLAYER, 1) == CHESS_INVALID_ID);
    ASSERT_TEST(chessAddGame(chess_system, 1, 1, 0, FIRST_PLAYER, 1) == CHESS_INVALID_ID);
    ASSERT_TEST(chessAddGame(chess_system, 1, 1, 1, FIRST_PLAYER, 1) == CHESS_INVALID_ID);
    ASSERT_TEST(chessAddGame(chess_system, 1, 1, 2, 3, 1) == CHESS_INVALID_ID);
    ASSERT_TEST(chessAddGame(chess_system, 1, 1, 2, FIRST_PLAYER, -1) == CHESS_INVALID_PLAY_TIME);

    ASSERT_TEST(chessAddGame(chess_system, 1, 1, 2, FIRST_PLAYER, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess_system, 1, 1, 2, FIRST_PLAYER, 1) == CHESS_GAME_ALREADY_EXISTS);
    ASSERT_TEST(chessAddGame(chess_system, 1, 1, 3, FIRST_PLAYER, 1) == CHESS_EXCEEDED_GAMES);
    ASSERT_TEST(chessAddGame(chess_system, 1, 2, 3, FIRST_PLAYER, 1) == CHESS_EXCEEDED_GAMES);

    ASSERT_TEST(chessRemoveTournament(NULL, 1) == CHESS_NULL_ARGUMENT);
    ASSERT_TEST(chessRemoveTournament(chess_system, 0) == CHESS_INVALID_ID);
    ASSERT_TEST(chessRemoveTournament(chess_system, 2) == CHESS_TOURNAMENT_NOT_EXIST);

    chessDestroy(chess_system);

    return true;
}

bool testPrivate()
{
    ChessSystem chess_system = chessCreate();

    ASSERT_TEST(chessAddTournament(chess_system, 1, 1, "London") == CHESS_SUCCESS);

    ASSERT_TEST(chessAddGame(chess_system, 1, 1, 2, FIRST_PLAYER, 1) == CHESS_SUCCESS);

    chessDestroy(chess_system);

    return true;
}


static bool updatePlayerInLoop(ChessSystem chess, int tour, int player_id, int _win, int _lose, int _draw, int time)
{
    int start_from = 123456;
    int win1 = _win/2;
    int win2 = _win - win1;
    int lose1 = _lose/2;
    int lose2 = _lose - lose1;
    int draw1 = _draw/2;
    int draw2 = _draw - draw1;
    for (int i = start_from; i < start_from + win1; i++)
    {
        ASSERT_TEST(chessAddGame(chess, tour, player_id, i, FIRST_PLAYER, time) == CHESS_SUCCESS);
    }
    for (int i = start_from + win1; i <start_from + win1 + lose1; i++)
    {
        ASSERT_TEST(chessAddGame(chess, tour, player_id, i, SECOND_PLAYER, time) == CHESS_SUCCESS);
    }
    for (int i = start_from + win1 + lose1; i < start_from + win1 + lose1 + draw1; i++)
    {
        ASSERT_TEST(chessAddGame(chess, tour, player_id, i, DRAW, time ) == CHESS_SUCCESS);
    }

    start_from *= 2;
    for (int i = start_from + win2; i > start_from; i--)
    {
        ASSERT_TEST(chessAddGame(chess, tour, i, player_id, SECOND_PLAYER, time ) == CHESS_SUCCESS);
    }

    for (int i = start_from + win2 + lose2; i > start_from + win2 ; i--)
    {
        ASSERT_TEST(chessAddGame(chess, tour, i, player_id, FIRST_PLAYER, time ) == CHESS_SUCCESS);
    }

    for (int i = start_from + win2 + lose2 + draw2; i > start_from + win2 + lose2; i--)
    {
        ASSERT_TEST(chessAddGame(chess, tour, i, player_id, DRAW, time ) == CHESS_SUCCESS);
    }

    return true;
}

bool testAndStatsLevels()
{
    FILE* file_levels = fopen("./tests/myExpectedTestLevel.txt", "w");
    if(!file_levels)
    {
        printf("test failed to open file, make sure you have the folder tests in the same folder with chessSystem"
               "excutable file and you have write permissions for the file /tests/myExpectedTestLevel.txt");
    }
    ChessSystem chess = chessCreate();
    ASSERT_TEST(chessEndTournament(chess, 9) == CHESS_TOURNAMENT_NOT_EXIST);
    ASSERT_TEST(chessAddTournament(chess, 9, 2, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 5, 3, "Tel aviv") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 44, 3, "New Mexico") == CHESS_INVALID_LOCATION);
    ASSERT_TEST(chessAddTournament(chess, 3, 1, "Ibiza") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 9, 5, "Morocco") == CHESS_TOURNAMENT_ALREADY_EXISTS);
    ASSERT_TEST(chessAddTournament(chess, 12, 6, "") == CHESS_INVALID_LOCATION);
    ASSERT_TEST(chessAddTournament(chess, 12, 9, "Napoli") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 3, 10, "Haifa") == CHESS_TOURNAMENT_ALREADY_EXISTS);
    ASSERT_TEST(chessAddTournament(chess, 14, 11, "Eilat") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 65, 15, "New-York") == CHESS_INVALID_LOCATION);
    ASSERT_TEST(chessRemoveTournament(chess, 14) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemoveTournament(chess, 14) == CHESS_TOURNAMENT_NOT_EXIST);

    int tour = 9;
    ASSERT_TEST(chessAddGame(chess, tour, 1, 2, FIRST_PLAYER, 5000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, tour, 2, 1, FIRST_PLAYER, 2000) == CHESS_GAME_ALREADY_EXISTS);
    ASSERT_TEST(chessAddGame(chess, tour, 1, 2, FIRST_PLAYER, 3000) == CHESS_GAME_ALREADY_EXISTS);
    ASSERT_TEST(chessAddGame(chess, tour, 3, 2, SECOND_PLAYER, 7500) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, tour, 4, 1, SECOND_PLAYER, 1000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, tour, 2, 4, FIRST_PLAYER, 3500) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, tour, 3, 4, DRAW, 400) == CHESS_SUCCESS);
    int t= 15;
    updatePlayerInLoop(chess, tour,99999,	45,	0,	0,++t);
    updatePlayerInLoop(chess, tour,88888,	4,	0,	82,++t);
    updatePlayerInLoop(chess, tour,77777,	40,	0,	0,++t);
    updatePlayerInLoop(chess, tour,2222,	18,	5,	18,++t);
    updatePlayerInLoop(chess, tour,7777,	13,	8,	15,++t);
    updatePlayerInLoop(chess, tour,5555,	11,	5,	11,++t);
    updatePlayerInLoop(chess, tour,9999,	0,	18,	19,++t);
    updatePlayerInLoop(chess, tour,4444,	5,	1,	1,++t);
    updatePlayerInLoop(chess, tour,3333,	4,	15,	3,--t);
    updatePlayerInLoop(chess, tour,6666,	2,	4,	5,--t);
    updatePlayerInLoop(chess, tour,1111,	0,	12,	3,--t);
    ASSERT_TEST(chessEndTournament(chess, tour) == CHESS_SUCCESS);
    ASSERT_TEST(chessEndTournament(chess, tour) == CHESS_TOURNAMENT_ENDED);

    tour = 5;
    updatePlayerInLoop(chess, tour,99999,	89,	60,	54,t);
    updatePlayerInLoop(chess, tour,77777,	61,	5	,54,t);
    updatePlayerInLoop(chess, tour,4444,	37,	52	,50,t);
    updatePlayerInLoop(chess, tour,2222,	15,	10,	90,t);
    updatePlayerInLoop(chess, tour,88888,	37,	51,	46,t);
    updatePlayerInLoop(chess, tour,5555,	45,	75,	76,t);
    updatePlayerInLoop(chess, tour,7777,	57,	73,	74,t);
    updatePlayerInLoop(chess, tour,6666,	66,	73,	72,t);
    updatePlayerInLoop(chess, tour,9999,	79,	78,	59,t);
    updatePlayerInLoop(chess, tour,3333,	45,	41,	87,t);
    updatePlayerInLoop(chess, tour,1111,    24,	18,	69,t);


    tour = 3;
    t=17;
    ASSERT_TEST(chessAddTournament(chess, tour, 1, "Ibiza") == CHESS_TOURNAMENT_ALREADY_EXISTS);
    updatePlayerInLoop(chess, tour,99999,	35,	46	,24,t);
    updatePlayerInLoop(chess, tour,77777,	94,	46	,58,t);
    updatePlayerInLoop(chess, tour,4444,	22,	7	,69,t);
    updatePlayerInLoop(chess, tour,2222,	1,	39,	8,t);
    updatePlayerInLoop(chess, tour,88888,	53,	77,	47,t);
    updatePlayerInLoop(chess, tour,5555,	89,	48,	82,t);
    updatePlayerInLoop(chess, tour,7777	,55,	21,	29,t);
    updatePlayerInLoop(chess, tour,6666	,90	,30,	95,t);
    updatePlayerInLoop(chess, tour,9999,	31,	33,	14,t);
    updatePlayerInLoop(chess, tour,3333,	66,	38,	14,t);
    updatePlayerInLoop(chess, tour,1111,	20,	32,	1,t);
    ASSERT_TEST(chessEndTournament(chess, tour) == CHESS_SUCCESS);
    ASSERT_TEST(chessEndTournament(chess, tour) == CHESS_TOURNAMENT_ENDED);

    tour = 12;  
    ASSERT_TEST(chessAddGame(chess, tour, 9999999, 1111111, FIRST_PLAYER, 5000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, tour, 9999999, 1111112, SECOND_PLAYER, 5000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, tour, 1111113, 9999999, FIRST_PLAYER, 5000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, tour, 1111117, 9999999, FIRST_PLAYER, 5000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, tour, 93, 456, FIRST_PLAYER, 2000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, tour, 1456, 18, FIRST_PLAYER, 3000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, tour, 1156, 2, SECOND_PLAYER, 7500) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, tour, -17, 7, SECOND_PLAYER, 1000) == CHESS_INVALID_ID);
    ASSERT_TEST(chessAddGame(chess, -15, 13517, 156, SECOND_PLAYER, 1000) == CHESS_INVALID_ID);
    ASSERT_TEST(chessAddGame(chess, tour, 15, -1, FIRST_PLAYER, 3500) == CHESS_INVALID_ID);
    ASSERT_TEST(chessEndTournament(chess, tour) == CHESS_SUCCESS);
    ASSERT_TEST(chessSavePlayersLevels(chess, file_levels) == CHESS_SUCCESS);
    ASSERT_TEST(chessSaveTournamentStatistics(chess, "./tests/myExpectedTestStats.txt") == CHESS_SUCCESS);

    chessDestroy(chess);
    fclose(file_levels);
    return true;    
}

/*The functions for the tests should be added here*/
bool (*tests[]) (void) = {
        testChessAddTournament,
        testChessRemoveTournament,
        testChessAddGame,
        testChessPrintLevelsAndTournamentStatistics,
        testBadArguments,
        testPrivate,
        testAndStatsLevels
};

/*The names of the test functions should be added here*/
const char* testNames[] = {
        "testChessAddTournament",
        "testChessRemoveTournament",
        "testChessAddGame",
        "testChessPrintLevelsAndTournamentStatistics",
        "testBadArguments",
        "testPrivate",
        "levelAndStatsTest"
};

int main(int argc, char *argv[]) {
    if (argc == 1) {
        for (int test_idx = 0; test_idx < NUMBER_TESTS; test_idx++) {
            RUN_TEST(tests[test_idx], testNames[test_idx]);
        }
        return 0;
    }
    if (argc != 2) {
        fprintf(stdout, "Usage: chessSystem <test index>\n");
        return 0;
    }

    int test_idx = strtol(argv[1], NULL, 10);
    if (test_idx < 1 || test_idx > NUMBER_TESTS) {
        fprintf(stderr, "Invalid test index %d\n", test_idx);
        return 0;
    }

    RUN_TEST(tests[test_idx - 1], testNames[test_idx - 1]);
    return 0;
}
