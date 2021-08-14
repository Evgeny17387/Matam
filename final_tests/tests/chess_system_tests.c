#include <stdlib.h>
#include "../chessSystem.h"
#include "../test_utilities.h"

/*The number of tests*/
#define NUMBER_TESTS 24


bool testChessCreation() {
    for (int i = 0; i < 5; i++)
    {
        ChessSystem chess = chessCreate();
	    chessDestroy(chess);
    }
	return true;
}

bool testChessDestruction() {
    chessDestroy(NULL);
	ChessSystem chess = chessCreate();
	chessDestroy(chess);
	return true;
}

bool testChessAddTournament() {
    ChessSystem chess = chessCreate();
    ASSERT_TEST(chessAddTournament(chess, 1, 4, "Toro  nto") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 2, 4, "Tel aviv") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 3, 4, "Germany") == CHESS_SUCCESS);
    chessDestroy(chess);
    return true;
}

bool testChessAddTournamentSingleFail() {
    ChessSystem chess = chessCreate();
    ASSERT_TEST(chessAddTournament(NULL, 1, 4, "London") == CHESS_NULL_ARGUMENT);

    ASSERT_TEST(chessAddTournament(chess, 1, 4, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 1, 6, "London") == CHESS_TOURNAMENT_ALREADY_EXISTS);
    
    ASSERT_TEST(chessAddTournament(chess, 2, 3, "Canada") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 2, 3, "Jerusalem") == CHESS_TOURNAMENT_ALREADY_EXISTS);

    ASSERT_TEST(chessAddTournament(chess, -3, 4, "London") == CHESS_INVALID_ID);

    
    ASSERT_TEST(chessAddTournament(chess, 10, 4, "tel aviv") == CHESS_INVALID_LOCATION);
    ASSERT_TEST(chessAddTournament(chess, 10, 4, "Tel Aviv") == CHESS_INVALID_LOCATION);
    ASSERT_TEST(chessAddTournament(chess, 10, 4, "Tel aviv@") == CHESS_INVALID_LOCATION);
    ASSERT_TEST(chessAddTournament(chess, 10, 4, "Tel aviv") == CHESS_SUCCESS);

    
    ASSERT_TEST(chessAddTournament(chess, 4, 0, "London") == CHESS_INVALID_MAX_GAMES);
    ASSERT_TEST(chessAddTournament(chess, 4, -1, "London") == CHESS_INVALID_MAX_GAMES);
    ASSERT_TEST(chessAddTournament(chess, 4, 4, "London") == CHESS_SUCCESS);

    chessDestroy(chess);
    return true;
}

bool testChessAddTournamentSeveralFails() {
    ChessSystem chess = chessCreate();
    ASSERT_TEST(chessAddTournament(NULL, 1, 4, "London") == CHESS_NULL_ARGUMENT);
    ASSERT_TEST(chessAddTournament(chess, 1, 4, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(NULL, 1, 4, "London") == CHESS_NULL_ARGUMENT);
    ASSERT_TEST(chessAddTournament(NULL, 2, -5, "London") == CHESS_NULL_ARGUMENT);
    ASSERT_TEST(chessAddTournament(NULL, 3, -5, "london") == CHESS_NULL_ARGUMENT);

    ASSERT_TEST(chessAddTournament(chess, 9, 4, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 9, 6, "London") == CHESS_TOURNAMENT_ALREADY_EXISTS);
    ASSERT_TEST(chessAddTournament(chess, 9, 6, "london") == CHESS_TOURNAMENT_ALREADY_EXISTS);
    ASSERT_TEST(chessAddTournament(chess, 9, -5, "London") == CHESS_TOURNAMENT_ALREADY_EXISTS);
    
    ASSERT_TEST(chessAddTournament(chess, 10, 4, "tel aviv") == CHESS_INVALID_LOCATION);
    ASSERT_TEST(chessAddTournament(chess, 10, -4, "tel aviv") == CHESS_INVALID_LOCATION);

    chessDestroy(chess);
    return true;
}

bool testChessAddGame() {
    ChessSystem chess = chessCreate();
    for (int i = 1; i < 10; i++) {
        ASSERT_TEST(chessAddTournament(chess, i, 4, "London") == CHESS_SUCCESS);
    }

    ASSERT_TEST(chessAddGame(chess, 1, 8, 9, FIRST_PLAYER, 4) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 8, 10, SECOND_PLAYER, 4) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 10, 9, DRAW, 4) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 4, 8, 9, FIRST_PLAYER, 4) == CHESS_SUCCESS);

    
    ASSERT_TEST(chessAddGame(chess, 1, 11, 9, DRAW, 0) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 12, 9, FIRST_PLAYER, 1000) == CHESS_SUCCESS);


    chessDestroy(chess);
    return true;
}

bool testChessAddGameSingleFail() {
    ChessSystem chess = chessCreate();

    for (int i = 1; i < 10; i++) {
        ASSERT_TEST(chessAddTournament(chess, i, 4, "London") == CHESS_SUCCESS);
    }

    ASSERT_TEST(chessAddGame(NULL, 1, 8, 9, FIRST_PLAYER, 4) == CHESS_NULL_ARGUMENT);

    ASSERT_TEST(chessAddGame(chess, 1, -8, 9, FIRST_PLAYER, 4) == CHESS_INVALID_ID);
    ASSERT_TEST(chessAddGame(chess, 1, 8, -9, FIRST_PLAYER, 4) == CHESS_INVALID_ID);
    //ASSERT_TEST(chessAddGame(chess, 1, 8, 8, FIRST_PLAYER, 4) == CHESS_INVALID_ID);

    ASSERT_TEST(chessAddGame(chess, 1, 8, 9, FIRST_PLAYER, 4) == CHESS_SUCCESS);

    ASSERT_TEST(chessAddGame(chess, 10, 8, 9, FIRST_PLAYER, 4) == CHESS_TOURNAMENT_NOT_EXIST);

    ASSERT_TEST(chessAddGame(chess, 5, 8, 9, FIRST_PLAYER, 4) == CHESS_SUCCESS);
    chessEndTournament(chess, 5);
    ASSERT_TEST(chessAddGame(chess, 5, 5, 9, FIRST_PLAYER, 4) == CHESS_TOURNAMENT_ENDED);

    ASSERT_TEST(chessAddGame(chess, 1, 8, 9, FIRST_PLAYER, 4) == CHESS_GAME_ALREADY_EXISTS);
    ASSERT_TEST(chessAddGame(chess, 1, 9, 8, FIRST_PLAYER, 4) == CHESS_GAME_ALREADY_EXISTS);

    ASSERT_TEST(chessAddGame(chess, 1, 10, 8, FIRST_PLAYER, -1) == CHESS_INVALID_PLAY_TIME);
    ASSERT_TEST(chessAddGame(chess, 1, 10, 8, FIRST_PLAYER, 0) == CHESS_SUCCESS);

    ASSERT_TEST(chessAddGame(chess, 1, 2, 8, FIRST_PLAYER, 0) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 8, 3, FIRST_PLAYER, 0) == CHESS_SUCCESS);
    //ASSERT_TEST(chessAddGame(chess, 1, 8, 5, FIRST_PLAYER, 0) == CHESS_EXCEEDED_GAMES);

    chessDestroy(chess);
    return true;
}

bool testChessAddGameSeveralFails() {
    ChessSystem chess = chessCreate();
    for (int i = 1; i < 10; i++) {
        ASSERT_TEST(chessAddTournament(chess, i, 4, "London") == CHESS_SUCCESS);
    }

    ASSERT_TEST(chessAddGame(NULL, 1, 8, 9, FIRST_PLAYER, 4) == CHESS_NULL_ARGUMENT);
    ASSERT_TEST(chessAddGame(NULL, 1, -8, 9, FIRST_PLAYER, 4) == CHESS_NULL_ARGUMENT);
    ASSERT_TEST(chessAddGame(NULL, 1, 8, -9, FIRST_PLAYER, 4) == CHESS_NULL_ARGUMENT);
    ASSERT_TEST(chessAddGame(NULL, 1, 8, 8, FIRST_PLAYER, 4) == CHESS_NULL_ARGUMENT);
    ASSERT_TEST(chessAddGame(NULL, 11, 8, 9, FIRST_PLAYER, 4) == CHESS_NULL_ARGUMENT);
    ASSERT_TEST(chessAddGame(chess, 5, 8, 9, FIRST_PLAYER, 4) == CHESS_SUCCESS);
    chessEndTournament(chess, 5);
    ASSERT_TEST(chessAddGame(NULL, 5, 8, 9, FIRST_PLAYER, 4) == CHESS_NULL_ARGUMENT);
    ASSERT_TEST(chessAddGame(NULL, 5, 8, 9, FIRST_PLAYER, 4) == CHESS_NULL_ARGUMENT);
    ASSERT_TEST(chessAddGame(NULL, 1, 8, 9, FIRST_PLAYER, -4) == CHESS_NULL_ARGUMENT);

    ASSERT_TEST(chessAddGame(chess, 11, -8, 9, FIRST_PLAYER, 4) == CHESS_INVALID_ID);
    ASSERT_TEST(chessAddGame(chess, 11, 8, -9, FIRST_PLAYER, 4) == CHESS_INVALID_ID);
    ASSERT_TEST(chessAddGame(chess, 6, 8, 9, FIRST_PLAYER, 4) == CHESS_SUCCESS);
    //ASSERT_TEST(chessAddGame(chess, 11, 8, 8, FIRST_PLAYER, 4) == CHESS_INVALID_ID);
    chessEndTournament(chess, 6);
    ASSERT_TEST(chessAddGame(chess, 6, -8, 9, FIRST_PLAYER, 4) == CHESS_INVALID_ID);
    ASSERT_TEST(chessAddGame(chess, 6, 8, -9, FIRST_PLAYER, 4) == CHESS_INVALID_ID);
    //ASSERT_TEST(chessAddGame(chess, 6, 8, 8, FIRST_PLAYER, 4) == CHESS_INVALID_ID);

    ASSERT_TEST(chessAddGame(chess, 2, -8, 9, FIRST_PLAYER, -4) == CHESS_INVALID_ID);
    ASSERT_TEST(chessAddGame(chess, 2, 8, -9, FIRST_PLAYER, -4) == CHESS_INVALID_ID);
    //ASSERT_TEST(chessAddGame(chess, 2, 8, 8, FIRST_PLAYER, -4) == CHESS_INVALID_ID);

    ASSERT_TEST(chessAddGame(chess, 11, 8, 9, FIRST_PLAYER, -4) == CHESS_TOURNAMENT_NOT_EXIST);
    //ASSERT_TEST(chessAddGame(chess, 11, 8, 8, FIRST_PLAYER, 4) == CHESS_INVALID_ID);

    ASSERT_TEST(chessAddGame(chess, 1, 8, 9, FIRST_PLAYER, 4) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 8, 1, FIRST_PLAYER, 4) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 8, 2, FIRST_PLAYER, 4) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 8, 3, FIRST_PLAYER, 4) == CHESS_SUCCESS);
    chessEndTournament(chess, 1);
    ASSERT_TEST(chessAddGame(chess, 1, 8, 9, FIRST_PLAYER, 4) == CHESS_TOURNAMENT_ENDED);
    ASSERT_TEST(chessAddGame(chess, 1, 8, 9, FIRST_PLAYER, -4) == CHESS_TOURNAMENT_ENDED);

    ASSERT_TEST(chessAddGame(chess, 8, 8, 9, FIRST_PLAYER, 4) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 8, 8, 1, FIRST_PLAYER, 4) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 8, 8, 2, FIRST_PLAYER, 4) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 8, 8, 3, FIRST_PLAYER, 4) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 8, 8, 4, FIRST_PLAYER, -4) == CHESS_INVALID_PLAY_TIME);

    chessDestroy(chess);
    return true;
}

bool testChessRemoveTournament() {
    ChessSystem chess = chessCreate();
    for (int i = 1; i < 10; i++) {
        ASSERT_TEST(chessAddTournament(chess, i, 4, "London") == CHESS_SUCCESS);
    }

    ASSERT_TEST(chessAddGame(chess, 2, 8, 9, FIRST_PLAYER, 4) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemoveTournament(chess, 2) == CHESS_SUCCESS);


    chessDestroy(chess);
    return true;
}

bool testChessRemoveTournamentSingleFail() {
    ChessSystem chess = chessCreate();

    for (int i = 1; i < 10; i++) {
        ASSERT_TEST(chessAddTournament(chess, i, 4, "London") == CHESS_SUCCESS);
    }

    ASSERT_TEST(chessAddGame(chess, 2, 8, 9, FIRST_PLAYER, 4) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemoveTournament(NULL, 2) == CHESS_NULL_ARGUMENT);
    ASSERT_TEST(chessRemoveTournament(chess, 11) == CHESS_TOURNAMENT_NOT_EXIST);
    ASSERT_TEST(chessEndTournament(chess, 2) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemoveTournament(chess, 2) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemoveTournament(chess, 2) == CHESS_TOURNAMENT_NOT_EXIST);
    //ASSERT_TEST(chessAddGame(chess, 1, 8, 5, FIRST_PLAYER, 0) == CHESS_EXCEEDED_GAMES);

    chessDestroy(chess);
    return true;
}

bool testChessRemoveTournamentSeveralFails() {
    ChessSystem chess = chessCreate();
    for (int i = 1; i < 10; i++) {
        ASSERT_TEST(chessAddTournament(chess, i, 4, "London") == CHESS_SUCCESS);
    }

    ASSERT_TEST(chessAddGame(chess, 2, 8, 9, FIRST_PLAYER, 4) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemoveTournament(NULL, 11) == CHESS_NULL_ARGUMENT);
    ASSERT_TEST(chessEndTournament(chess, 2) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemoveTournament(chess, 2) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemoveTournament(NULL, 2) == CHESS_NULL_ARGUMENT);

    chessDestroy(chess);
    return true;
}

bool testChessRemovePlayer() {
    ChessSystem chess = chessCreate();
    for (int i = 1; i < 10; i++) {
        ASSERT_TEST(chessAddTournament(chess, i, 4, "London") == CHESS_SUCCESS);
    }

    ASSERT_TEST(chessAddGame(chess, 2, 8, 9, FIRST_PLAYER, 4) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemovePlayer(chess, 8) == CHESS_SUCCESS);


    chessDestroy(chess);
    return true;
}

bool testChessRemovePlayerSingleFail() {
    ChessSystem chess = chessCreate();

    for (int i = 1; i < 10; i++) {
        ASSERT_TEST(chessAddTournament(chess, i, 4, "London") == CHESS_SUCCESS);
    }

    ASSERT_TEST(chessAddGame(chess, 2, 8, 9, FIRST_PLAYER, 4) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemovePlayer(NULL, 8) == CHESS_NULL_ARGUMENT);
    ASSERT_TEST(chessRemovePlayer(chess, 10) == CHESS_PLAYER_NOT_EXIST);
    //ASSERT_TEST(chessAddGame(chess, 1, 8, 5, FIRST_PLAYER, 0) == CHESS_EXCEEDED_GAMES);

    chessDestroy(chess);
    return true;
}

bool testChessRemovePlayerSeveralFails() {
    ChessSystem chess = chessCreate();
    for (int i = 1; i < 10; i++) {
        ASSERT_TEST(chessAddTournament(chess, i, 4, "London") == CHESS_SUCCESS);
    }

    ASSERT_TEST(chessAddGame(chess, 2, 8, 9, FIRST_PLAYER, 4) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemovePlayer(NULL, 8) == CHESS_NULL_ARGUMENT);
    ASSERT_TEST(chessRemovePlayer(NULL, 10) == CHESS_NULL_ARGUMENT);
    ASSERT_TEST(chessRemovePlayer(NULL, 0) == CHESS_NULL_ARGUMENT);

    chessDestroy(chess);
    return true;
}

bool testChessEndTournament() {
    ChessSystem chess = chessCreate();
    for (int i = 1; i < 10; i++) {
        ASSERT_TEST(chessAddTournament(chess, i, 4, "London") == CHESS_SUCCESS);
    }

    ASSERT_TEST(chessAddGame(chess, 2, 8, 9, FIRST_PLAYER, 4) == CHESS_SUCCESS);
    ASSERT_TEST(chessEndTournament(chess, 2) == CHESS_SUCCESS);


    chessDestroy(chess);
    return true;
}

bool testChessEndTournamentSingleFail() {
    ChessSystem chess = chessCreate();

    for (int i = 1; i < 10; i++) {
        ASSERT_TEST(chessAddTournament(chess, i, 4, "London") == CHESS_SUCCESS);
    }

    ASSERT_TEST(chessAddGame(chess, 2, 8, 9, FIRST_PLAYER, 4) == CHESS_SUCCESS);
    ASSERT_TEST(chessEndTournament(NULL, 2) == CHESS_NULL_ARGUMENT);
    ASSERT_TEST(chessEndTournament(chess, -7) == CHESS_INVALID_ID);
    
    ASSERT_TEST(chessEndTournament(chess, 11) == CHESS_TOURNAMENT_NOT_EXIST);

    ASSERT_TEST(chessEndTournament(chess, 2) == CHESS_SUCCESS);
    ASSERT_TEST(chessEndTournament(chess, 2) == CHESS_TOURNAMENT_ENDED);
    //ASSERT_TEST(chessAddGame(chess, 1, 8, 5, FIRST_PLAYER, 0) == CHESS_EXCEEDED_GAMES);

    chessDestroy(chess);
    return true;
}

bool testChessEndTournamentSeveralFails() {
    ChessSystem chess = chessCreate();
    for (int i = 1; i < 10; i++) {
        ASSERT_TEST(chessAddTournament(chess, i, 4, "London") == CHESS_SUCCESS);
    }

    ASSERT_TEST(chessAddGame(chess, 2, 8, 9, FIRST_PLAYER, 4) == CHESS_SUCCESS);
    ASSERT_TEST(chessEndTournament(NULL, 2) == CHESS_NULL_ARGUMENT);
    ASSERT_TEST(chessEndTournament(NULL, -7) == CHESS_NULL_ARGUMENT);
    ASSERT_TEST(chessEndTournament(NULL, 11) == CHESS_NULL_ARGUMENT);
    ASSERT_TEST(chessEndTournament(NULL, 2) == CHESS_NULL_ARGUMENT);
    //ASSERT_TEST(chessAddGame(chess, 1, 8, 5, FIRST_PLAYER, 0) == CHESS_E
    chessDestroy(chess);
    return true;
}



bool testChessSavePlayersLevels (){
    FILE* file_levels = fopen("./player_levels_your_output_1.txt", "w");
    if(!file_levels){
        printf("test failed to open file, make sure"
               " you have write permissions for the file /player_levels_your_output_1.txt");
    }
    ChessSystem chess = chessCreate();
    ASSERT_TEST(chessAddTournament(chess, 1, 4, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 2, 4, "Chicago") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 3, 4, "New york") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 2000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 1, 3, FIRST_PLAYER, 3000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 3, 2, SECOND_PLAYER, 3000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 4, 1, SECOND_PLAYER, 1000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 2, 4, FIRST_PLAYER, 3500) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 3, 4, DRAW, 400) == CHESS_SUCCESS);
    ASSERT_TEST(chessEndTournament(chess, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 1, 2, SECOND_PLAYER, 2000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 1, 3, FIRST_PLAYER, 3000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 3, 2, DRAW, 3000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 4, 1, FIRST_PLAYER, 1000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 2, 4, FIRST_PLAYER, 3500) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 3, 4, DRAW, 400) == CHESS_SUCCESS);
    ASSERT_TEST(chessEndTournament(chess, 2) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 3, 1, 2, SECOND_PLAYER, 2000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 3, 1, 3, FIRST_PLAYER, 3000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 3, 3, 2, DRAW, 3000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 3, 4, 1, FIRST_PLAYER, 1000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 3, 2, 4, FIRST_PLAYER, 3500) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 3, 3, 4, DRAW, 400) == CHESS_SUCCESS);
    ASSERT_TEST(chessEndTournament(chess, 3) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemoveTournament(chess, 2) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemovePlayer(chess, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessSavePlayersLevels(chess, file_levels) == CHESS_SUCCESS);
    fclose(file_levels);
    chessDestroy(chess);
    return true;
}

bool testChessSavePlayersLevelSingleFail (){
    ChessSystem chess = chessCreate();
    //ASSERT_TEST(chessSavePlayersLevels(NULL, NULL) == CHESS_NULL_ARGUMENT);
    FILE* file_levels = fopen("./1.txt", "w");
    if(!file_levels){
        printf("test failed to open file, make sure"
               "you have write permissions for the file /player_levels_your_output_1.txt");
    }
    //ASSERT_TEST(chessSavePlayersLevels(NULL, file_levels) == CHESS_NULL_ARGUMENT);
    fclose(file_levels);
    chessDestroy(chess);
    return true;
}


bool testChessSaveTournamentStatistics (){
    ChessSystem chess = chessCreate();
    ASSERT_TEST(chessAddTournament(chess, 1, 4, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 2, 4, "Chicago") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 3, 4, "New york") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 600, 4, "Rome") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 324, 4, "Paris") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 2000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 1, 3, FIRST_PLAYER, 3000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 3, 2, SECOND_PLAYER, 3000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 4, 1, SECOND_PLAYER, 1000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 2, 4, FIRST_PLAYER, 3500) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 3, 4, DRAW, 400) == CHESS_SUCCESS);
    ASSERT_TEST(chessEndTournament(chess, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 1, 2, SECOND_PLAYER, 2000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 1, 3, FIRST_PLAYER, 3000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 3, 2, DRAW, 3000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 4, 1, FIRST_PLAYER, 1000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 2, 4, FIRST_PLAYER, 3500) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 3, 4, DRAW, 400) == CHESS_SUCCESS);
    ASSERT_TEST(chessEndTournament(chess, 2) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 3, 1, 2, SECOND_PLAYER, 2000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 3, 1, 3, FIRST_PLAYER, 3000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 3, 3, 2, DRAW, 3000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 3, 4, 1, FIRST_PLAYER, 1000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 3, 2, 4, FIRST_PLAYER, 3500) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 3, 3, 4, DRAW, 400) == CHESS_SUCCESS);
    ASSERT_TEST(chessEndTournament(chess, 3) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 600, 9, 8, SECOND_PLAYER, 7) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 600, 7, 8, FIRST_PLAYER, 900) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 600, 9, 1, DRAW, 8) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 600, 10, 1, FIRST_PLAYER, 453) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 600, 8, 1, FIRST_PLAYER, 5544) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 600, 8, 10, DRAW, 65545) == CHESS_SUCCESS);
        
    ASSERT_TEST(chessAddGame(chess, 324, 1, 2, DRAW, 2000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 324, 1, 3, FIRST_PLAYER, 3000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 324, 3, 2, DRAW, 3000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 324, 4, 1, FIRST_PLAYER, 1000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 324, 2, 4, SECOND_PLAYER, 3500) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 324, 3, 4, DRAW, 400) == CHESS_SUCCESS);
    ASSERT_TEST(chessEndTournament(chess, 600) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemoveTournament(chess, 2) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemovePlayer(chess, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemovePlayer(chess, 9) == CHESS_SUCCESS);
    ASSERT_TEST(chessSaveTournamentStatistics(chess, "./tournament_statistics_your_output_1.txt") == CHESS_SUCCESS);
    chessDestroy(chess);
    return true;
}

bool testChessSaveTournamentStatisticSingleFail (){
    ChessSystem chess = chessCreate();
    ASSERT_TEST(chessAddTournament(chess, 1, 4, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 2, 4, "Chicago") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 3, 4, "New york") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 600, 4, "Rome") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 324, 4, "Paris") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 2000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 1, 3, FIRST_PLAYER, 3000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 3, 2, SECOND_PLAYER, 3000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 4, 1, SECOND_PLAYER, 1000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 2, 4, FIRST_PLAYER, 3500) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 3, 4, DRAW, 400) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 1, 2, SECOND_PLAYER, 2000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 1, 3, FIRST_PLAYER, 3000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 3, 2, DRAW, 3000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 4, 1, FIRST_PLAYER, 1000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 2, 4, FIRST_PLAYER, 3500) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 3, 4, DRAW, 400) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 3, 1, 2, SECOND_PLAYER, 2000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 3, 1, 3, FIRST_PLAYER, 3000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 3, 3, 2, DRAW, 3000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 3, 4, 1, FIRST_PLAYER, 1000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 3, 2, 4, FIRST_PLAYER, 3500) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 3, 3, 4, DRAW, 400) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 600, 9, 8, SECOND_PLAYER, 7) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 600, 7, 8, FIRST_PLAYER, 900) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 600, 9, 1, DRAW, 8) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 600, 10, 1, FIRST_PLAYER, 453) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 600, 8, 1, FIRST_PLAYER, 5544) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 600, 8, 10, DRAW, 65545) == CHESS_SUCCESS);
        
    ASSERT_TEST(chessAddGame(chess, 324, 1, 2, DRAW, 2000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 324, 1, 3, FIRST_PLAYER, 3000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 324, 3, 2, DRAW, 3000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 324, 4, 1, FIRST_PLAYER, 1000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 324, 2, 4, SECOND_PLAYER, 3500) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 324, 3, 4, DRAW, 400) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemoveTournament(chess, 2) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemovePlayer(chess, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemovePlayer(chess, 9) == CHESS_SUCCESS);
    ASSERT_TEST(chessSaveTournamentStatistics(NULL, "./1.txt") == CHESS_NULL_ARGUMENT);
    ASSERT_TEST(chessSaveTournamentStatistics(chess, "./1.txt") == CHESS_NO_TOURNAMENTS_ENDED);
    chessDestroy(chess);
    return true;
}

bool testChessAddTournamentExample() {
	ChessSystem chess = chessCreate();
	ASSERT_TEST(chessAddTournament(chess, 1, 4, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 2, 5, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 1, 10, "Paris") == CHESS_TOURNAMENT_ALREADY_EXISTS);

	chessDestroy(chess);
	return true;
}

bool testChessRemoveTournamentExample() {
    ChessSystem chess = chessCreate();
    ASSERT_TEST(chessAddTournament(chess, 1, 4, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 2, 4, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessRemoveTournament(chess, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemoveTournament(chess, 1) == CHESS_TOURNAMENT_NOT_EXIST);
    ASSERT_TEST(chessAddTournament(chess, 1, 4, "Paris") == CHESS_SUCCESS);

    chessDestroy(chess);
    return true;
}

bool testChessAddGameExample(){
    ChessSystem chess = chessCreate();
    ASSERT_TEST(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 2000) == CHESS_TOURNAMENT_NOT_EXIST);
    ASSERT_TEST(chessAddTournament(chess, 1, 4, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 2000) == CHESS_SUCCESS);

    chessDestroy(chess);
    return true;
}

bool testChessPrintLevelsAndTournamentStatisticsExample(){
    FILE* file_levels = fopen("./player_levels_your_output.txt", "w");
    if(!file_levels){
        printf("test failed to open file"
               ". make sure you have write permissions for the file /player_levels_your_output.txt");
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
    ASSERT_TEST(chessSaveTournamentStatistics(chess, "./tournament_statistics_your_output.txt") == CHESS_SUCCESS);
    chessDestroy(chess);
    return true;
}


/*The functions for the tests should be added here*/
bool (*tests[]) (void) = {
                      testChessCreation,
                      testChessDestruction,
                      testChessAddTournament,
                      testChessAddTournamentSingleFail,
                      testChessAddTournamentSeveralFails,
                      testChessAddGame,
                      testChessAddGameSingleFail,
                      testChessAddGameSeveralFails,
                      testChessRemoveTournament,
                      testChessRemoveTournamentSingleFail,
                      testChessRemoveTournamentSeveralFails,
                      testChessRemovePlayer,
                      testChessRemovePlayerSingleFail,
                      testChessRemovePlayerSeveralFails,
                      testChessEndTournament,
                      testChessEndTournamentSingleFail,
                      testChessEndTournamentSeveralFails,
                      testChessSavePlayersLevels,
                      testChessSavePlayersLevelSingleFail,
                      testChessSaveTournamentStatistics,
                      testChessSaveTournamentStatisticSingleFail,
                      testChessAddTournamentExample,
                      testChessRemoveTournamentExample,
                      testChessAddGameExample,
                      testChessPrintLevelsAndTournamentStatisticsExample
};

/*The names of the test functions should be added here*/
const char* testNames[] = {
                           "testChessCreation",
                           "testChessDestruction",
                           "testChessAddTournament",  
                            "testChessAddTournamentSingleFail",
                            "testChessAddTournamentSeveralFails",
                            "testChessAddGame",
                           "testChessAddGameSingleFail",
                           "testChessAddGameSeveralFails",
                           "testChessRemoveTournament",
                           "testChessRemoveTournamentSingleFail",
                           "testChessRemoveTournamentSeveralFails",
                           "testChessRemovePlayer",
                           "testChessRemovePlayerSingleFail",
                           "testChessRemovePlayerSeveralFails",
                           "testChessEndTournament",
                           "testChessEndTournamentSingleFail",
                           "testChessEndTournamentSeveralFails",
                           "testChessSavePlayersLevels",
                           "testChessSavePlayersLevelSingleFail",
                           "testChessSaveTournamentStatistics",
                           "testChessSaveTournamentStatisticSingleFail",
                           "testChessAddTournamentExample",
                           "testChessRemoveTournamentExample",
                           "testChessAddGameExample",
                           "testChessPrintLevelsAndTournamentStatisticsExample"
                           
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
