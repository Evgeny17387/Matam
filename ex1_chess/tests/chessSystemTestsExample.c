#include <stdlib.h>
#include "../chessSystem.h"
#include "../test_utilities.h"

/*The number of tests*/
#define NUMBER_TESTS 20

#define ASSERT_TRUE ASSERT_TEST
#define ASSERT_EQ(x, y) ASSERT_TEST((x) == (y))
#define ASSERT_NE(x, y) ASSERT_TEST((x) != (y))
#define ASSERT_FALSE(x) ASSERT_TEST(!(x))

int compareFile(FILE* fPtr1, FILE* fPtr2)
{
    // return 0;

    char ch1, ch2;
    do
    {
        // Input character from both files
        ch1 = fgetc(fPtr1);
        ch2 = fgetc(fPtr2);
        // If characters are not same then return -1
        if (ch1 != ch2)
            return -1;
    } while (ch1 != EOF && ch2 != EOF);

    /* If both files have reached end */
    if (ch1 == EOF && ch2 == EOF)
        return 0;
    else
        return -1;
}

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

    FILE* f1 = fopen("./tests/player_levels_your_output.txt", "r");
    FILE* f2 = fopen("./tests/player_levels_expected_output.txt", "r");
    ASSERT_TEST(compareFile(f1, f2) == 0);
    fclose(f1);
    fclose(f2);

    f1 = fopen("./tests/tournament_statistics_your_output.txt", "r");
    f2 = fopen("./tests/tournament_statistics_expected_output.txt", "r");
    ASSERT_TEST(compareFile(f1, f2) == 0);
    fclose(f1);
    fclose(f2);

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
    FILE* file1 = fopen("./tests/your_output/player_levels_remove_tournment_1.txt", "w");
    ASSERT_TEST(chessAddTournament(chess_system, 1, 1, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess_system, 1, 1, 2, FIRST_PLAYER, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemoveTournament(chess_system, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessSavePlayersLevels(chess_system, file1) == CHESS_SUCCESS);
    chessDestroy(chess_system);
    fclose(file1);
    file1 = fopen("./tests/your_output/player_levels_remove_tournment_1.txt", "r");
    FILE* file2 = fopen("./tests/expected_output/player_levels_remove_tournment_1.txt", "r");
    ASSERT_TEST(compareFile(file1, file2) == 0);
    fclose(file1);
    fclose(file2);

    chess_system = chessCreate();
    file1 = fopen("./tests/your_output/player_levels_remove_tournment_2.txt", "w");
    ASSERT_TEST(chessAddTournament(chess_system, 1, 1, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess_system, 1, 1, 2, FIRST_PLAYER, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess_system, 2, 1, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess_system, 2, 1, 2, SECOND_PLAYER, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemoveTournament(chess_system, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessSavePlayersLevels(chess_system, file1) == CHESS_SUCCESS);
    chessDestroy(chess_system);
    fclose(file1);
    file1 = fopen("./tests/your_output/player_levels_remove_tournment_2.txt", "r");
    file2 = fopen("./tests/expected_output/player_levels_remove_tournment_2.txt", "r");
    ASSERT_TEST(compareFile(file1, file2) == 0);
    fclose(file1);
    fclose(file2);

    chess_system = chessCreate();
    file1 = fopen("./tests/your_output/player_levels_remove_tournment_3.txt", "w");
    ASSERT_TEST(chessAddTournament(chess_system, 1, 2, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess_system, 1, 1, 2, FIRST_PLAYER, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess_system, 1, 1, 3, FIRST_PLAYER, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemovePlayer(chess_system, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemoveTournament(chess_system, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessSavePlayersLevels(chess_system, file1) == CHESS_SUCCESS);
    chessDestroy(chess_system);
    fclose(file1);
    file1 = fopen("./tests/your_output/player_levels_remove_tournment_3.txt", "r");
    file2 = fopen("./tests/expected_output/player_levels_remove_tournment_3.txt", "r");
    ASSERT_TEST(compareFile(file1, file2) == 0);
    fclose(file1);
    fclose(file2);

    chess_system = chessCreate();
    file1 = fopen("./tests/your_output/player_levels_remove_tournment_4.txt", "w");
    ASSERT_TEST(chessAddTournament(chess_system, 1, 2, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess_system, 1, 1, 2, FIRST_PLAYER, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess_system, 1, 1, 3, FIRST_PLAYER, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess_system, 2, 2, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess_system, 2, 1, 4, FIRST_PLAYER, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess_system, 2, 1, 5, FIRST_PLAYER, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemovePlayer(chess_system, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemoveTournament(chess_system, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessSavePlayersLevels(chess_system, file1) == CHESS_SUCCESS);
    chessDestroy(chess_system);
    fclose(file1);
    file1 = fopen("./tests/your_output/player_levels_remove_tournment_4.txt", "r");
    file2 = fopen("./tests/expected_output/player_levels_remove_tournment_4.txt", "r");
    ASSERT_TEST(compareFile(file1, file2) == 0);
    fclose(file1);
    fclose(file2);

    return true;
}

bool testChessDestroy_maaroof() {
    chessDestroy(NULL);

    ChessSystem sys1 = chessCreate();
    chessDestroy(sys1);

    ChessSystem sys2 = chessCreate();
    ASSERT_TEST(chessAddTournament(sys2, 1, 1, "Location one") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(sys2, 2, 1, "Location two") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(sys2, 3, 1, "Location three") == CHESS_SUCCESS);
    chessDestroy(sys2);

    ChessSystem sys3 = chessCreate();
    ASSERT_TEST(chessAddTournament(sys3, 1, 1, "Location one") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(sys3, 2, 1, "Location two") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(sys3, 3, 1, "Location three") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(sys3, 1, 01, 02, FIRST_PLAYER, 5) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(sys3, 2, 01, 02, FIRST_PLAYER, 5) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(sys3, 3, 01, 02, FIRST_PLAYER, 5) == CHESS_SUCCESS);
    chessDestroy(sys3);

    return true;
}

bool testChessAddTournamentAndEndTournament_maaroof() {
    ChessSystem sys1 = chessCreate();
    ASSERT_TEST(chessAddTournament(sys1, 1, 1, NULL) == CHESS_NULL_ARGUMENT);
    ASSERT_TEST(chessAddTournament(NULL, 1, 1, "Location one") == CHESS_NULL_ARGUMENT);

    ASSERT_TEST(chessAddTournament(sys1, 1, 1, "Location one") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(sys1, 2, 1, "Location two") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(sys1, 3, 1, "Location three") == CHESS_SUCCESS);

    ASSERT_TEST(chessAddTournament(sys1, 1, 1, "Location one") == CHESS_TOURNAMENT_ALREADY_EXISTS);
    ASSERT_TEST(chessAddTournament(sys1, 1, 1, "invalid location") == CHESS_TOURNAMENT_ALREADY_EXISTS);
    ASSERT_TEST(chessAddTournament(sys1, 4, -1, "invalid location") == CHESS_INVALID_LOCATION);
    ASSERT_TEST(chessAddTournament(sys1, 4, -1, "London") == CHESS_INVALID_MAX_GAMES);

    ASSERT_TEST(chessAddTournament(sys1, -2, -1, "invalid location") == CHESS_INVALID_ID);
    ASSERT_TEST(chessAddTournament(sys1, 2, -1, "invalid location") == CHESS_TOURNAMENT_ALREADY_EXISTS);
    ASSERT_TEST(chessAddTournament(sys1, 5, 1, "invalid location") == CHESS_INVALID_LOCATION);
    ASSERT_TEST(chessAddTournament(sys1, 5, 1, " Invalid location") == CHESS_INVALID_LOCATION);
    ASSERT_TEST(chessAddTournament(sys1, 10, 1, "L") == CHESS_SUCCESS);
    
    int player_1 = 1;
    int player_2 = 2;
    ASSERT_TEST(chessAddGame(sys1, 1, player_1, player_2, FIRST_PLAYER, 10) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(sys1, 2, player_1, player_2, SECOND_PLAYER, 10) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(sys1, 3, player_1, player_2, SECOND_PLAYER, 10) == CHESS_SUCCESS);

    ASSERT_TEST(chessEndTournament(sys1, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessEndTournament(sys1, 2) == CHESS_SUCCESS);
    ASSERT_TEST(chessEndTournament(sys1, 3) == CHESS_SUCCESS);
    // ASSERT_TEST(chessEndTournament(sys1, 3) == CHESS_SUCCESS); // ToDo: Shouldn't pass since this tournment was already been ended
    ASSERT_TEST(chessEndTournament(sys1, 4) == CHESS_TOURNAMENT_NOT_EXIST);
    ASSERT_TEST(chessEndTournament(sys1, -1) == CHESS_INVALID_ID);

   
    ASSERT_TEST(chessAddGame(sys1, 1, 4, 5, FIRST_PLAYER, 10) == CHESS_TOURNAMENT_ENDED);
    ASSERT_TEST(chessSaveTournamentStatistics(sys1, "your_output/test_add_tournament_end_tournament_your_output.txt") == CHESS_SUCCESS);
    FILE* f1 = fopen("your_output/test_add_tournament_end_tournament_your_output.txt", "r");
    FILE* f2 = fopen("expected_output/test_add_tournament_end_tournament_expected_output.txt", "r");
    ASSERT_TEST(compareFile(f1, f2) == 0);
    fclose(f1);
    fclose(f2);

    ChessSystem sys2 = chessCreate();
    ASSERT_TEST(chessAddTournament(sys2, 1, 1, "Location one") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(sys2, 2, 1, "Location two") == CHESS_SUCCESS);
    ASSERT_TEST(chessSaveTournamentStatistics(sys2, "document.txt") == CHESS_NO_TOURNAMENTS_ENDED);

    chessDestroy(sys2);
    chessDestroy(sys1);
    return true;
}

bool testChessAddGame_maaroof()
{
    ChessSystem sys1 = chessCreate();
    ASSERT_TEST(chessAddTournament(sys1, 1, 1, "Location one") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(sys1, 2, 1, "Location two") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(sys1, 3, 1, "Location three") == CHESS_SUCCESS);
    
    int player_1 = 1;
    int player_2 = 2;
    int player_3 = 3;
    int player_4 = 4;

    ASSERT_TEST(chessAddGame(sys1, 1, player_1, player_2, FIRST_PLAYER, 10) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(sys1, 1, player_1, player_2, FIRST_PLAYER, 10) == CHESS_GAME_ALREADY_EXISTS);
    ASSERT_TEST(chessAddGame(sys1, 1, player_2, player_1, FIRST_PLAYER, 10) == CHESS_GAME_ALREADY_EXISTS);
    ASSERT_TEST(chessAddGame(sys1, 2, player_1, player_2, FIRST_PLAYER, 10) == CHESS_SUCCESS);

    ASSERT_TEST(chessAddGame(sys1, 1, player_3, player_3, FIRST_PLAYER, 10) == CHESS_INVALID_ID);
    ASSERT_TEST(chessAddGame(sys1, 1, player_3, player_4, FIRST_PLAYER, -1) == CHESS_INVALID_PLAY_TIME);
    //not sure about ZERO as play time
    //ASSERT_TEST(chessAddGame(sys1, 1, player_1, player_2, FIRST_PLAYER, 0) == CHESS_INVALID_PLAY_TIME);
    ASSERT_TEST(chessAddGame(sys1, -1, player_1, player_2, FIRST_PLAYER, -1) == CHESS_INVALID_ID);
    ASSERT_TEST(chessAddGame(sys1, 1, -8, player_2, FIRST_PLAYER, -1) == CHESS_INVALID_ID);

    ASSERT_TEST(chessAddGame(sys1, 1, player_1, player_3, SECOND_PLAYER, 10) == CHESS_EXCEEDED_GAMES);
    ASSERT_TEST(chessAddGame(sys1, 1, player_3, player_2, FIRST_PLAYER, 10) == CHESS_EXCEEDED_GAMES);
    ASSERT_TEST(chessAddGame(sys1, 1, player_3, player_1, FIRST_PLAYER, 10) == CHESS_EXCEEDED_GAMES);
    ASSERT_TEST(chessAddGame(sys1, 1, player_1, player_4, SECOND_PLAYER, 10) == CHESS_EXCEEDED_GAMES);
    ASSERT_TEST(chessAddGame(sys1, 3, player_1, player_2, FIRST_PLAYER, 10) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(sys1, 4, player_1, player_2, FIRST_PLAYER, 10) == CHESS_TOURNAMENT_NOT_EXIST);

    ASSERT_TEST(chessAddTournament(sys1, 4, 2, "Location four") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(sys1, 4, player_1, player_2, FIRST_PLAYER, 10) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(sys1, 4, player_3, player_2, SECOND_PLAYER, 10) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(sys1, 4, player_3, player_2, FIRST_PLAYER, 10) == CHESS_GAME_ALREADY_EXISTS);
    ASSERT_TEST(chessAddGame(sys1, 4, player_4, player_2, FIRST_PLAYER, 10) == CHESS_EXCEEDED_GAMES);
    
    ASSERT_TEST(chessAddGame(sys1, 4, player_3, player_4, FIRST_PLAYER, 10) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemovePlayer(sys1,player_3) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(sys1, 4, player_3, player_4, FIRST_PLAYER, 10) == CHESS_SUCCESS);

    chessDestroy(sys1);
    return true;
}

bool testRemoveTournament_maaroof() 
{
    ChessSystem sys1 = chessCreate();
    ASSERT_TEST(chessAddTournament(sys1, 1, 3, "Location one") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(sys1, 2, 3, "Location two") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(sys1, 3, 3, "Location three") == CHESS_SUCCESS);
    ASSERT_TEST(chessRemoveTournament(sys1, 3) == CHESS_SUCCESS);

    int player_1 = 1;
    int player_2 = 2;
    int player_3 = 3;
    //int player_4 = 4;

    ASSERT_TEST(chessAddGame(sys1, 3, player_1, player_2, FIRST_PLAYER, 10) == CHESS_TOURNAMENT_NOT_EXIST);
    ASSERT_TEST(chessAddGame(sys1, 2, player_1, player_2, FIRST_PLAYER, 10) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(sys1, 2, player_3, player_2, FIRST_PLAYER, 10) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemoveTournament(sys1, 2) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(sys1, 2, player_1, player_2, FIRST_PLAYER, 10) == CHESS_TOURNAMENT_NOT_EXIST);
    ASSERT_TEST(chessAddGame(sys1, 2, -1, player_2, FIRST_PLAYER, 10) == CHESS_INVALID_ID);
    ASSERT_TEST(chessAddGame(sys1, -1, player_1, player_2, FIRST_PLAYER, -10) == CHESS_INVALID_ID);

    ASSERT_TEST(chessAddGame(sys1, 1, player_1, player_2, FIRST_PLAYER, 10) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(sys1, 1, player_3, player_2, FIRST_PLAYER, 10) == CHESS_SUCCESS);

    ASSERT_TEST(chessSaveTournamentStatistics(sys1,"your_output/test_tournament_remove_your_output.txt") == CHESS_NO_TOURNAMENTS_ENDED);

    ASSERT_TEST(chessEndTournament(sys1, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessSaveTournamentStatistics(sys1, "your_output/test_tournament_remove_your_output.txt") == CHESS_SUCCESS);
    FILE* f1 = fopen("your_output/test_tournament_remove_your_output.txt", "r");
    FILE* f2 = fopen("expected_output/test_tournament_remove_expected_output.txt", "r");
    ASSERT_TEST(compareFile(f1, f2) == 0); // ToDo: originaly there was a bug in maroof file, it was 2, but it should be 1
    fclose(f1);
    fclose(f2);

    chessDestroy(sys1);
    return true;
}

bool testChessRemovePlayer_maaroof() {
    ChessSystem sys1 = chessCreate();
    ASSERT_TEST(chessAddTournament(sys1, 1, 3, "Location one") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(sys1, 2, 3, "Location two") == CHESS_SUCCESS);
    
    int player_1 = 1;
    int player_2 = 2;
    int player_3 = 3;
    //int player_4 = 4;

    ASSERT_TEST(chessAddGame(sys1, 1, player_1, player_2, FIRST_PLAYER, 6) == CHESS_SUCCESS); //tour 1: add 1,2
    ASSERT_TEST(chessAddGame(sys1, 1, player_3, player_2, FIRST_PLAYER, 4) == CHESS_SUCCESS); //tour 1: add 3,2
    ASSERT_TEST(chessAddGame(sys1, 2, player_1, player_2, FIRST_PLAYER, 10) == CHESS_SUCCESS);//tour 2: add 1,2
    
    FILE* file_levels = fopen("your_output/player_levels_all.txt", "w");
    ASSERT_TEST(chessSavePlayersLevels(sys1, file_levels) == CHESS_SUCCESS);
    fclose(file_levels);

    FILE* f1 = fopen("your_output/player_levels_all.txt", "r");
    FILE* f2 = fopen("expected_output/player_levels_all.txt", "r"); // ToDo: maroof hasn't arenged the player from highest point to lowest
    ASSERT_TEST(compareFile(f1, f2) == 0);
    fclose(f1);
    fclose(f2);

    ASSERT_TEST(chessRemovePlayer(sys1, player_1) == CHESS_SUCCESS);//remove 1
    ASSERT_TEST(chessEndTournament(sys1, 1) == CHESS_SUCCESS);//end 1
    ASSERT_TEST(chessEndTournament(sys1, 2) == CHESS_SUCCESS);//end 2
    
    FILE* file_levels_2 = fopen("your_output/remove_player_1.txt", "w");
    ASSERT_TEST(chessSavePlayersLevels(sys1, file_levels_2) == CHESS_SUCCESS);//print
    ASSERT_TEST(chessRemovePlayer(sys1, player_1) == CHESS_PLAYER_NOT_EXIST);
    fclose(file_levels_2);

    f1 = fopen("your_output/remove_player_1.txt", "r");
    f2 = fopen("expected_output/remove_player_1.txt", "r");
    ASSERT_TEST(compareFile(f1, f2) == 0);
    fclose(f1);
    fclose(f2);


    chessDestroy(sys1);
    return true;
}

bool testChessRemovePlayer_2_maaroof() {
    ChessSystem sys1 = chessCreate();
    ASSERT_TEST(chessAddTournament(sys1, 1, 3, "Location one") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(sys1, 2, 3, "Location two") == CHESS_SUCCESS);

    int player_1 = 1;
    int player_2 = 2;

    ASSERT_TEST(chessAddGame(sys1, 1, player_1, player_2, FIRST_PLAYER, 6) == CHESS_SUCCESS); //tour 1: add 1,2
    
    FILE* file1 = fopen("your_output/before_remove.txt", "w");
    ASSERT_TEST(chessSavePlayersLevels(sys1, file1) == CHESS_SUCCESS);
    fclose(file1);
    
    FILE* f1 = fopen("your_output/before_remove.txt", "r");
    FILE* f2 = fopen("expected_output/before_remove.txt", "r");
    ASSERT_TEST(compareFile(f1, f2) == 0);
    fclose(f1);
    fclose(f2);

    ASSERT_TEST(chessRemovePlayer(sys1, player_1) == CHESS_SUCCESS);//remove 1

    file1 = fopen("your_output/after_remove.txt", "w");
    ASSERT_TEST(chessSavePlayersLevels(sys1, file1) == CHESS_SUCCESS);
    fclose(file1);

    f1 = fopen("your_output/after_remove.txt", "r");
    f2 = fopen("expected_output/after_remove.txt", "r");
    ASSERT_TEST(compareFile(f1, f2) == 0);
    fclose(f1);
    fclose(f2);

    ASSERT_TEST(chessAddGame(sys1, 1, player_1, player_2, FIRST_PLAYER, 6) == CHESS_SUCCESS); //tour 1: add 1,2
    
    file1 = fopen("your_output/add_same_game_after_remove.txt", "w");
    ASSERT_TEST(chessSavePlayersLevels(sys1, file1) == CHESS_SUCCESS);
    fclose(file1);

    f1 = fopen("your_output/add_same_game_after_remove.txt", "r");
    f2 = fopen("expected_output/add_same_game_after_remove.txt", "r"); // Note: originaly maaroof had a bug here
    ASSERT_TEST(compareFile(f1, f2) == 0);
    fclose(f1);
    fclose(f2);

    chessDestroy(sys1);
    return true;
}

bool testAvgGameTime_maaroof() 
{
    ChessSystem sys1 = chessCreate();
    ASSERT_TEST(chessAddTournament(sys1, 1, 3, "Location one") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(sys1, 2, 3, "Location two") == CHESS_SUCCESS);

    int player_1 = 1;
    int player_2 = 2;
    int player_3 = 3;
    int player_4 = 4;

    ChessResult* chess_result = malloc(sizeof(*chess_result));
    ASSERT_TEST(chessAddGame(sys1, 1, player_1, player_2, FIRST_PLAYER, 6) == CHESS_SUCCESS); //tour 1: add 1,2
    double avg1 = chessCalculateAveragePlayTime(sys1, player_1, chess_result);
    ASSERT_TEST(*chess_result == CHESS_SUCCESS);
    double avg2 = chessCalculateAveragePlayTime(sys1, player_2, chess_result);
    ASSERT_TEST(*chess_result == CHESS_SUCCESS);
    ASSERT_TEST(avg1 == avg2);
    ASSERT_TEST(avg1 == 6);
    
    ASSERT_TEST(chessAddGame(sys1, 1, player_1, player_3, FIRST_PLAYER, 14) == CHESS_SUCCESS);
    avg1 = chessCalculateAveragePlayTime(sys1, player_1, chess_result);
    ASSERT_TEST(*chess_result == CHESS_SUCCESS);
    ASSERT_TEST(avg1 == 10);

    avg1 = chessCalculateAveragePlayTime(sys1, player_3, chess_result);
    ASSERT_TEST(*chess_result == CHESS_SUCCESS);
    ASSERT_TEST(avg1 == 14);

    ASSERT_TEST(chessAddGame(sys1, 2, player_1, player_3, FIRST_PLAYER, 20) == CHESS_SUCCESS);
    avg1 = chessCalculateAveragePlayTime(sys1, player_1, chess_result);
    ASSERT_TEST(*chess_result == CHESS_SUCCESS);
    ASSERT_TEST(avg1 == 40.0/3.0);

    avg1 = chessCalculateAveragePlayTime(sys1, player_3, chess_result);
    ASSERT_TEST(*chess_result == CHESS_SUCCESS);
    ASSERT_TEST(avg1 == 17);

    chessCalculateAveragePlayTime(sys1, player_4, chess_result);
    ASSERT_TEST(*chess_result == CHESS_PLAYER_NOT_EXIST);

    chessCalculateAveragePlayTime(sys1, -1, chess_result);
    ASSERT_TEST(*chess_result == CHESS_INVALID_ID);

    chessCalculateAveragePlayTime(NULL, player_4, chess_result);
    ASSERT_TEST(*chess_result == CHESS_NULL_ARGUMENT);

    chessCalculateAveragePlayTime(NULL, -1, chess_result);
    ASSERT_TEST(*chess_result == CHESS_NULL_ARGUMENT);

    ASSERT_TEST(chessRemovePlayer(sys1, player_1) == CHESS_SUCCESS);
    chessCalculateAveragePlayTime(sys1, player_1, chess_result);
    ASSERT_TEST(*chess_result == CHESS_PLAYER_NOT_EXIST);

    avg1 = chessCalculateAveragePlayTime(sys1, player_3, chess_result);
    ASSERT_TEST(*chess_result == CHESS_SUCCESS);
    ASSERT_TEST(avg1 == 17);

    free(chess_result);
    chessDestroy(sys1);
    return true;
}

bool testSavePlayerLevelsAndTournamentStatistics_maaroof() 
{
    ChessSystem sys1 = chessCreate();
    ASSERT_TEST(chessAddTournament(sys1, 1, 3, "Location one") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(sys1, 2, 3, "Location two") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(sys1, 3, 3, "Location three") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(sys1, 4, 3, "Location four") == CHESS_SUCCESS);

    int player_1 = 1;
    int player_2 = 2;
    //int player_3 = 3;
    //int player_4 = 4;
    //int player_5 = 5;

    char* fileName = "";
   // FILE* file;

    ASSERT_TEST(chessAddGame(sys1, 4, player_1, player_2, FIRST_PLAYER, 6) == CHESS_SUCCESS);
    ASSERT_TEST(chessEndTournament(sys1, 4) == CHESS_SUCCESS);
    fileName = "your_output/tournament_statistics_1.txt";
    ASSERT_TEST(chessSaveTournamentStatistics(sys1, fileName) == CHESS_SUCCESS);
    
    FILE* f1 = fopen("your_output/tournament_statistics_1.txt", "r");
    FILE* f2 = fopen("expected_output/tournament_statistics_1.txt", "r");
    ASSERT_TEST(compareFile(f1, f2) == 0);
    fclose(f1);
    fclose(f2);

    ASSERT_TEST(chessAddGame(sys1, 3, player_1, player_2, FIRST_PLAYER, 6) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemovePlayer(sys1, 1));
    ASSERT_TEST(chessEndTournament(sys1, 3) == CHESS_SUCCESS);
    fileName = "your_output/tournament_statistics_2.txt"; // Note: originally maaroof had here a bug, he didn't count the players who already been removed
    ASSERT_TEST(chessSaveTournamentStatistics(sys1, fileName) == CHESS_SUCCESS);

    f1 = fopen("your_output/tournament_statistics_2.txt", "r");
    f2 = fopen("expected_output/tournament_statistics_2.txt", "r");
    ASSERT_TEST(compareFile(f1, f2) == 0);
    fclose(f1);
    fclose(f2);

    ASSERT_TEST(chessAddGame(sys1, 2, player_1, player_2, FIRST_PLAYER, 6) == CHESS_SUCCESS);
    ASSERT_TEST(chessEndTournament(sys1, 2) == CHESS_SUCCESS);
    fileName = "your_output/tournament_statistics_3.txt";
    ASSERT_TEST(chessSaveTournamentStatistics(sys1, fileName) == CHESS_SUCCESS);

    f1 = fopen("your_output/tournament_statistics_3.txt", "r");
    f2 = fopen("expected_output/tournament_statistics_3.txt", "r");
    ASSERT_TEST(compareFile(f1, f2) == 0);
    fclose(f1);
    fclose(f2);

    chessDestroy(sys1);
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
    ASSERT_TEST(chessAddTournament(chess, 9, 1000, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 5, 1000, "Tel aviv") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 44, 3, "New Mexico") == CHESS_INVALID_LOCATION);
    ASSERT_TEST(chessAddTournament(chess, 3, 1000, "Ibiza") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 9, 5, "Morocco") == CHESS_TOURNAMENT_ALREADY_EXISTS);
    ASSERT_TEST(chessAddTournament(chess, 12, 6, "") == CHESS_INVALID_LOCATION);
    ASSERT_TEST(chessAddTournament(chess, 12, 1000, "Napoli") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 3, 10, "Haifa") == CHESS_TOURNAMENT_ALREADY_EXISTS);
    ASSERT_TEST(chessAddTournament(chess, 14, 1000, "Eilat") == CHESS_SUCCESS);
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

bool testChessCreateDestroy_nadav()
{
    chessDestroy(NULL);

    ChessSystem chess = chessCreate();
    ASSERT_NE(chess, NULL);
    chessDestroy(chess);

    return true;
}

bool testChessPlayers_nadav()
{
    ChessSystem chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessRemovePlayer(NULL, 1), CHESS_NULL_ARGUMENT);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessRemovePlayer(chess, 0), CHESS_INVALID_ID);
    ASSERT_EQ(chessRemovePlayer(chess, -1), CHESS_INVALID_ID);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessRemovePlayer(chess, 1), CHESS_PLAYER_NOT_EXIST);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessRemovePlayer(chess, 1), CHESS_SUCCESS);
    ASSERT_EQ(chessRemovePlayer(chess, 1), CHESS_PLAYER_NOT_EXIST);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, SECOND_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessRemovePlayer(chess, 2), CHESS_SUCCESS);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 1), CHESS_SUCCESS);
    ASSERT_EQ(chessRemovePlayer(NULL, 1), CHESS_NULL_ARGUMENT);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 1), CHESS_SUCCESS);
    ASSERT_EQ(chessRemovePlayer(NULL, 0), CHESS_NULL_ARGUMENT);
    ASSERT_EQ(chessRemovePlayer(chess, 0), CHESS_INVALID_ID);
    ASSERT_EQ(chessRemovePlayer(chess, 3), CHESS_PLAYER_NOT_EXIST);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ChessResult result;
    ASSERT_EQ(chessCalculateAveragePlayTime(NULL, 1, &result), -1);
    ASSERT_EQ(result, CHESS_NULL_ARGUMENT);
    ASSERT_EQ(chessCalculateAveragePlayTime(chess, 1, NULL), -1);
    ASSERT_EQ(chessCalculateAveragePlayTime(chess, 0, &result), -1);
    ASSERT_EQ(result, CHESS_INVALID_ID);
    ASSERT_EQ(chessCalculateAveragePlayTime(chess, -1, &result), -1);
    ASSERT_EQ(result, CHESS_INVALID_ID);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessCalculateAveragePlayTime(chess, 1, &result), -1);
    ASSERT_EQ(result, CHESS_PLAYER_NOT_EXIST);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 1), CHESS_SUCCESS);
    ASSERT_EQ(chessCalculateAveragePlayTime(chess, 1, &result), 60);
    ASSERT_EQ(result, CHESS_SUCCESS);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 90), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 3, FIRST_PLAYER, 110), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 4, FIRST_PLAYER, 130), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 2, 3, FIRST_PLAYER, 30), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 1), CHESS_SUCCESS);
    ASSERT_EQ(chessCalculateAveragePlayTime(chess, 1, &result), 110);
    ASSERT_EQ(result, CHESS_SUCCESS);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 90), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 3, FIRST_PLAYER, 110), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 4, FIRST_PLAYER, 130), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 2, 3, FIRST_PLAYER, 30), CHESS_SUCCESS);
    ASSERT_EQ(chessCalculateAveragePlayTime(chess, 1, &result), 110);
    ASSERT_EQ(result, CHESS_SUCCESS);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddTournament(chess, 2, 5, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 109), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 3, FIRST_PLAYER, 120), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 1, 3, FIRST_PLAYER, 150), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 2, 5, FIRST_PLAYER, 100), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 4, FIRST_PLAYER, 130), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 1, 5, FIRST_PLAYER, 10), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 2, 3, FIRST_PLAYER, 30), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 1), CHESS_SUCCESS);
    ASSERT_EQ(chessCalculateAveragePlayTime(chess, 1, &result), 103.8);
    ASSERT_EQ(result, CHESS_SUCCESS);
    chessDestroy(chess);

    return true;
}

bool testChessSanity_nadav()
{
    ChessSystem chess = chessCreate();
    ASSERT_NE(chess, NULL);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 3, FIRST_PLAYER, 80), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 1), CHESS_SUCCESS);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessRemoveTournament(chess, 1), CHESS_SUCCESS);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 1), CHESS_SUCCESS);
    ChessResult result;
    ASSERT_EQ(chessCalculateAveragePlayTime(chess, 1, &result), 60);
    ASSERT_EQ(result, CHESS_SUCCESS);
    chessDestroy(chess);
    
    return true;
}

bool testChessModification_nadav()
{
    ChessSystem chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(NULL, 1, 1, "A"), CHESS_NULL_ARGUMENT);
    ASSERT_EQ(chessAddTournament(chess, 1, 1, NULL), CHESS_NULL_ARGUMENT);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 0, 1, "A"), CHESS_INVALID_ID);
    ASSERT_EQ(chessAddTournament(chess, -1, 1, "A"), CHESS_INVALID_ID);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_EQ(chessAddTournament(chess, 1, 0, "A"), CHESS_INVALID_MAX_GAMES);
    ASSERT_EQ(chessAddTournament(chess, 1, -1, "A"), CHESS_INVALID_MAX_GAMES);
    ASSERT_EQ(chessAddTournament(chess, 1, 1, "A"), CHESS_SUCCESS);
    ASSERT_NE(chess, NULL);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 1, ""), CHESS_INVALID_LOCATION);
    ASSERT_EQ(chessAddTournament(chess, 1, 1, "abc"), CHESS_INVALID_LOCATION);
    ASSERT_EQ(chessAddTournament(chess, 1, 1, "AbC"), CHESS_INVALID_LOCATION);
    ASSERT_EQ(chessAddTournament(chess, 1, 1, "abC"), CHESS_INVALID_LOCATION);
    ASSERT_EQ(chessAddTournament(chess, 1, 1, "Abc"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddTournament(chess, 2, 1, "Abc def"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddTournament(chess, 3, 1, "Abc Def"), CHESS_INVALID_LOCATION);
    ASSERT_EQ(chessAddTournament(chess, 3, 1, "Abc!"), CHESS_INVALID_LOCATION);
    ASSERT_EQ(chessAddTournament(chess, 3, 1, "A!bc"), CHESS_INVALID_LOCATION);
    ASSERT_EQ(chessAddTournament(chess, 3, 1, "Abc !"), CHESS_INVALID_LOCATION);
    ASSERT_EQ(chessAddTournament(chess, 3, 1, "Abc !de"), CHESS_INVALID_LOCATION);
    ASSERT_EQ(chessAddTournament(chess, 3, 1, "Abc ! de"), CHESS_INVALID_LOCATION);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 1, "Abc"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddTournament(chess, 1, 1, "Abc"), CHESS_TOURNAMENT_ALREADY_EXISTS);
    ASSERT_EQ(chessAddTournament(chess, 1, 2, "Abc"), CHESS_TOURNAMENT_ALREADY_EXISTS);
    ASSERT_EQ(chessAddTournament(chess, 1, 1, "Abcd"), CHESS_TOURNAMENT_ALREADY_EXISTS);
    ASSERT_EQ(chessAddTournament(chess, 1, 2, "Abcd"), CHESS_TOURNAMENT_ALREADY_EXISTS);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 1, "Abc"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 1), CHESS_SUCCESS);
    ASSERT_EQ(chessAddTournament(chess, 1, 1, "Abc"), CHESS_TOURNAMENT_ALREADY_EXISTS);
    ASSERT_EQ(chessAddTournament(chess, 1, 2, "Abc"), CHESS_TOURNAMENT_ALREADY_EXISTS);
    ASSERT_EQ(chessAddTournament(chess, 1, 1, "Abcd"), CHESS_TOURNAMENT_ALREADY_EXISTS);
    ASSERT_EQ(chessAddTournament(chess, 1, 2, "Abcd"), CHESS_TOURNAMENT_ALREADY_EXISTS);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 10, 1, "Abc"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddTournament(NULL, 0, 0, NULL), CHESS_NULL_ARGUMENT);
    ASSERT_EQ(chessAddTournament(chess, 0, 0, "abC"), CHESS_INVALID_ID);
    ASSERT_EQ(chessAddTournament(chess, 10, 0, "abC"), CHESS_TOURNAMENT_ALREADY_EXISTS);
    ASSERT_EQ(chessAddTournament(chess, 1, 0, "abC"), CHESS_INVALID_LOCATION);
    ASSERT_EQ(chessAddTournament(chess, 1, 0, "Abc"), CHESS_INVALID_MAX_GAMES);
    ASSERT_EQ(chessAddTournament(chess, 1, 1, "Abc"), CHESS_SUCCESS);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessEndTournament(NULL, 1), CHESS_NULL_ARGUMENT);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessEndTournament(chess, 1), CHESS_TOURNAMENT_NOT_EXIST);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessEndTournament(chess, 0), CHESS_INVALID_ID);
    ASSERT_EQ(chessEndTournament(chess, -1), CHESS_INVALID_ID);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 1), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 1), CHESS_TOURNAMENT_ENDED);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 1), CHESS_NO_GAMES);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 1), CHESS_SUCCESS);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 2, 3, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 3, 4, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 1), CHESS_SUCCESS);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 10, 1, "Abc"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 10, 1, 2, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 10), CHESS_SUCCESS);
    ASSERT_EQ(chessAddTournament(chess, 11, 1, "Abc"), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(NULL, 0), CHESS_NULL_ARGUMENT);
    ASSERT_EQ(chessEndTournament(chess, 0), CHESS_INVALID_ID);
    ASSERT_EQ(chessEndTournament(chess, 1), CHESS_TOURNAMENT_NOT_EXIST);
    ASSERT_EQ(chessEndTournament(chess, 10), CHESS_TOURNAMENT_ENDED);
    ASSERT_EQ(chessEndTournament(chess, 11), CHESS_NO_GAMES);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessRemoveTournament(NULL, 1), CHESS_NULL_ARGUMENT);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessRemoveTournament(chess, 0), CHESS_INVALID_ID);
    ASSERT_EQ(chessRemoveTournament(chess, -1), CHESS_INVALID_ID);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessRemoveTournament(chess, 1), CHESS_TOURNAMENT_NOT_EXIST);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessRemoveTournament(chess, 1), CHESS_SUCCESS);
    ASSERT_EQ(chessRemoveTournament(chess, 1), CHESS_TOURNAMENT_NOT_EXIST);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 2, 3, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessRemoveTournament(chess, 1), CHESS_SUCCESS);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 2, 3, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 1), CHESS_SUCCESS);
    ASSERT_EQ(chessRemoveTournament(chess, 1), CHESS_SUCCESS);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessRemoveTournament(NULL, 0), CHESS_NULL_ARGUMENT);
    ASSERT_EQ(chessRemoveTournament(chess, 0), CHESS_INVALID_ID);
    ASSERT_EQ(chessRemoveTournament(chess, 2), CHESS_TOURNAMENT_NOT_EXIST);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 2, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 0, 1, 2, FIRST_PLAYER, 60), CHESS_INVALID_ID);
    ASSERT_EQ(chessAddGame(chess, -1, 1, 2, FIRST_PLAYER, 60), CHESS_INVALID_ID);
    ASSERT_EQ(chessAddGame(chess, 1, -1, 2, FIRST_PLAYER, 60), CHESS_INVALID_ID);
    ASSERT_EQ(chessAddGame(chess, 1, 0, 2, FIRST_PLAYER, 60), CHESS_INVALID_ID);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 0, FIRST_PLAYER, 60), CHESS_INVALID_ID);
    ASSERT_EQ(chessAddGame(chess, 1, 1, -2, FIRST_PLAYER, 60), CHESS_INVALID_ID);
    ASSERT_EQ(chessAddGame(chess, 1, 0, 0, FIRST_PLAYER, 60), CHESS_INVALID_ID);
    ASSERT_EQ(chessAddGame(chess, 1, -1, -2, FIRST_PLAYER, 60), CHESS_INVALID_ID);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 2, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 1, 2, FIRST_PLAYER, 60), CHESS_TOURNAMENT_NOT_EXIST);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 2, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 1), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 2, 3, FIRST_PLAYER, 60), CHESS_TOURNAMENT_ENDED);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 2, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 60), CHESS_GAME_ALREADY_EXISTS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, SECOND_PLAYER, 60), CHESS_GAME_ALREADY_EXISTS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 120), CHESS_GAME_ALREADY_EXISTS);
    ASSERT_EQ(chessAddGame(chess, 1, 2, 1, FIRST_PLAYER, 60), CHESS_GAME_ALREADY_EXISTS);
    ASSERT_EQ(chessAddGame(chess, 1, 2, 1, DRAW, 120), CHESS_GAME_ALREADY_EXISTS);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 2, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, -1), CHESS_INVALID_PLAY_TIME);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, -5), CHESS_INVALID_PLAY_TIME);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 0), CHESS_SUCCESS);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 2, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 3, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 4, FIRST_PLAYER, 60), CHESS_EXCEEDED_GAMES);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 1, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessAddTournament(chess, 2, 2, "London"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 1, 2, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 2), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(NULL, 0, -1, -1, FIRST_PLAYER, -1), CHESS_NULL_ARGUMENT);
    ASSERT_EQ(chessAddGame(chess, 0, -1, -1, FIRST_PLAYER, -1), CHESS_INVALID_ID);
    ASSERT_EQ(chessAddGame(chess, 3, 1, 2, FIRST_PLAYER, -1), CHESS_TOURNAMENT_NOT_EXIST);
    ASSERT_EQ(chessAddGame(chess, 2, 1, 2, FIRST_PLAYER, -1), CHESS_TOURNAMENT_ENDED);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, -1), CHESS_GAME_ALREADY_EXISTS);
    ASSERT_EQ(chessAddGame(chess, 1, 2, 3, FIRST_PLAYER, -1), CHESS_INVALID_PLAY_TIME);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 4, FIRST_PLAYER, 60), CHESS_EXCEEDED_GAMES);
    chessDestroy(chess);

    return true;
}

bool testChessStatistics_nadav()
{
    ChessSystem chess = chessCreate();
    ASSERT_NE(chess, NULL);
    FILE* file = fopen("./nadav/your_outs/testChessStatistics_nadav", "w");
    ASSERT_NE(file, NULL);
    ASSERT_EQ(chessSavePlayersLevels(NULL, NULL), CHESS_NULL_ARGUMENT);
    ASSERT_EQ(chessSavePlayersLevels(NULL, file), CHESS_NULL_ARGUMENT);
    ASSERT_EQ(chessSavePlayersLevels(chess, NULL), CHESS_NULL_ARGUMENT);
    fclose(file);
    chessDestroy(chess);

    // ToDo: check how to implement check if print to file failed
    // chess = chessCreate();
    // ASSERT_NE(chess, NULL);
    // file = fopen("./nadav/your_outs/testChessStatistics_nadav.txt", "r");
    // ASSERT_NE(file, NULL);
    // ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    // ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 60), CHESS_SUCCESS);
    // ASSERT_EQ(chessEndTournament(chess, 1), CHESS_SUCCESS);
    // ASSERT_EQ(chessSavePlayersLevels(chess, file), CHESS_SAVE_FAILURE);
    // fclose(file);
    // chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    file = fopen("./nadav/your_outs/testChessStatistics_nadav", "r");
    ASSERT_NE(file, NULL);
    ASSERT_EQ(chessSavePlayersLevels(chess, file), CHESS_SUCCESS);
    fclose(file);
    chessDestroy(chess);

    // ToDo: decide if need, if so how to implement
    // chess = chessCreate();
    // ASSERT_NE(chess, NULL);
    // file = fopen("./nadav/your_outs/testChessStatistics_nadav.txt", "r");
    // ASSERT_EQ(chessSavePlayersLevels(chess, file), CHESS_SUCCESS);
    // fclose(file);
    // ASSERT_EQ(readFile(file_name).size(), 0);
    // chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    file = fopen("./nadav/your_outs/one_game.levels", "w");
    ASSERT_NE(file, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 1), CHESS_SUCCESS);
    ASSERT_EQ(chessSavePlayersLevels(chess, file), CHESS_SUCCESS);
    fclose(file);
    FILE* f1 = fopen("./nadav/expected_outs/one_game.levels", "r");
    FILE* f2 = fopen("./nadav/your_outs/one_game.levels", "r");
    ASSERT_TEST(compareFile(f1, f2) == 0);
    fclose(f1);
    fclose(f2);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    file = fopen("./nadav/your_outs/multi_game.levels", "w");
    ASSERT_NE(file, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 2, 3, FIRST_PLAYER, 20), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 3, SECOND_PLAYER, 50), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 4, DRAW, 50), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 1), CHESS_SUCCESS);
    ASSERT_EQ(chessSavePlayersLevels(chess, file), CHESS_SUCCESS);
    fclose(file);
    f1 = fopen("./nadav/expected_outs/multi_game.levels", "r");
    f2 = fopen("./nadav/your_outs/multi_game.levels", "r");
    ASSERT_TEST(compareFile(f1, f2) == 0);
    fclose(f1);
    fclose(f2);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    file = fopen("./nadav/your_outs/multi_tournament.levels", "w");
    ASSERT_NE(file, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 2, 3, FIRST_PLAYER, 20), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 3, SECOND_PLAYER, 50), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 4, DRAW, 50), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 1), CHESS_SUCCESS);
    ASSERT_EQ(chessAddTournament(chess, 2, 5, "London"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 3, 4, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 2, 3, DRAW, 300), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 5, 4, SECOND_PLAYER, 220), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 2), CHESS_SUCCESS);
    ASSERT_EQ(chessSavePlayersLevels(chess, file), CHESS_SUCCESS);
    fclose(file);
    f1 = fopen("./nadav/expected_outs/multi_tournament.levels", "r");
    f2 = fopen("./nadav/your_outs/multi_tournament.levels", "r");
    ASSERT_TEST(compareFile(f1, f2) == 0);
    fclose(f1);
    fclose(f2);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    file = fopen("./nadav/your_outs/remove_player_at_end.levels", "w");
    ASSERT_NE(file, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 2, 3, FIRST_PLAYER, 20), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 3, SECOND_PLAYER, 50), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 4, DRAW, 50), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 1), CHESS_SUCCESS);
    ASSERT_EQ(chessAddTournament(chess, 2, 5, "London"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 3, 4, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 2, 3, DRAW, 300), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 5, 4, SECOND_PLAYER, 220), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 2), CHESS_SUCCESS);
    ASSERT_EQ(chessRemovePlayer(chess, 2), CHESS_SUCCESS);
    ASSERT_EQ(chessSavePlayersLevels(chess, file), CHESS_SUCCESS);
    fclose(file);
    f1 = fopen("./nadav/expected_outs/remove_player_at_end.levels", "r");
    f2 = fopen("./nadav/your_outs/remove_player_at_end.levels", "r");
    ASSERT_TEST(compareFile(f1, f2) == 0);
    fclose(f1);
    fclose(f2);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    file = fopen("./nadav/your_outs/remove_player_mid_tournament.levels", "w");
    ASSERT_NE(file, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 2, 3, FIRST_PLAYER, 20), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 3, SECOND_PLAYER, 50), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 4, DRAW, 50), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 1), CHESS_SUCCESS);
    ASSERT_EQ(chessAddTournament(chess, 2, 5, "London"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 3, 4, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 2, 3, DRAW, 300), CHESS_SUCCESS);
    ASSERT_EQ(chessRemovePlayer(chess, 2), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 5, 4, SECOND_PLAYER, 220), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 2, 5, SECOND_PLAYER, 300), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 2), CHESS_SUCCESS);
    ASSERT_EQ(chessSavePlayersLevels(chess, file), CHESS_SUCCESS);
    fclose(file);
    f1 = fopen("./nadav/expected_outs/remove_player_mid_tournament.levels", "r");
    f2 = fopen("./nadav/your_outs/remove_player_mid_tournament.levels", "r");
    ASSERT_TEST(compareFile(f1, f2) == 0);
    fclose(f1);
    fclose(f2);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    file = fopen("./nadav/your_outs/remove_player_after_first.levels", "w");
    ASSERT_NE(file, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 2, 3, FIRST_PLAYER, 20), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 3, SECOND_PLAYER, 50), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 4, DRAW, 50), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 1), CHESS_SUCCESS);
    ASSERT_EQ(chessRemovePlayer(chess, 2), CHESS_SUCCESS);
    ASSERT_EQ(chessAddTournament(chess, 2, 5, "London"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 3, 4, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 2, 3, DRAW, 300), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 5, 4, SECOND_PLAYER, 220), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 2, 5, SECOND_PLAYER, 300), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 2), CHESS_SUCCESS);
    ASSERT_EQ(chessSavePlayersLevels(chess, file), CHESS_SUCCESS);
    fclose(file);
    f1 = fopen("./nadav/expected_outs/remove_player_after_first.levels", "r");
    f2 = fopen("./nadav/your_outs/remove_player_after_first.levels", "r");
    ASSERT_TEST(compareFile(f1, f2) == 0);
    fclose(f1);
    fclose(f2);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    file = fopen("./nadav/your_outs/remove_second_player_winner.levels", "w");
    ASSERT_NE(file, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 2, 3, FIRST_PLAYER, 20), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 3, SECOND_PLAYER, 50), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 4, DRAW, 50), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 1), CHESS_SUCCESS);
    ASSERT_EQ(chessAddTournament(chess, 2, 5, "London"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 3, 4, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 3, 2, SECOND_PLAYER, 300), CHESS_SUCCESS);
    ASSERT_EQ(chessRemovePlayer(chess, 2), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 5, 4, SECOND_PLAYER, 220), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 2, 5, SECOND_PLAYER, 300), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 2), CHESS_SUCCESS);
    ASSERT_EQ(chessSavePlayersLevels(chess, file), CHESS_SUCCESS);
    fclose(file);
    f1 = fopen("./nadav/expected_outs/remove_second_player_winner.levels", "r");
    f2 = fopen("./nadav/your_outs/remove_second_player_winner.levels", "r");
    ASSERT_TEST(compareFile(f1, f2) == 0);
    fclose(f1);
    fclose(f2);
    chessDestroy(chess);

    // ToDo: check how to implement check if print to file failed
    // chess = chessCreate();
    // ASSERT_NE(chess, NULL);
    // file = fopen("./nadav/your_outs/testChessStatistics_nadav", "r");
    // ASSERT_NE(file, NULL);
    // ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    // ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 60), CHESS_SUCCESS);
    // ASSERT_EQ(chessEndTournament(chess, 1), CHESS_SUCCESS);
    // ASSERT_EQ(chessSavePlayersLevels(NULL, file), CHESS_NULL_ARGUMENT);
    // ASSERT_EQ(chessSavePlayersLevels(chess, file), CHESS_SAVE_FAILURE);
    // fclose(file);
    // chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessSaveTournamentStatistics(NULL, NULL), CHESS_NULL_ARGUMENT);
    ASSERT_EQ(chessSaveTournamentStatistics(chess, NULL), CHESS_NULL_ARGUMENT);
    ASSERT_EQ(chessSaveTournamentStatistics(NULL, (char *)"test.stats"), CHESS_NULL_ARGUMENT);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessSaveTournamentStatistics(chess, (char *)"test.stats"), CHESS_NO_TOURNAMENTS_ENDED);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 1), CHESS_SUCCESS);
    ASSERT_EQ(chessSaveTournamentStatistics(chess, (char *)"this/path/does/not/exist/test.stats"), CHESS_SAVE_FAILURE);
    chessDestroy(chess);

    // ToDo: decide if need, if so how to implement
//     chess = chessCreate();
//     ASSERT_NE(chess, NULL);
//     ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
//     ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 60), CHESS_SUCCESS);
//     ASSERT_EQ(chessEndTournament(chess, 1), CHESS_SUCCESS);
// #ifdef MSVC
//     ASSERT_EQ(chessSaveTournamentStatistics(chess, (char *)"C:/test.stats"), CHESS_SAVE_FAILURE);
// #else
//     ASSERT_EQ(chessSaveTournamentStatistics(chess, (char *)"/test.stats"), CHESS_SAVE_FAILURE);
// #endif
//     chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 3, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 2, 3, FIRST_PLAYER, 20), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, SECOND_PLAYER, 50), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 1), CHESS_SUCCESS);
    ASSERT_EQ(chessSaveTournamentStatistics(chess, "./nadav/your_outs/one_tournament.stats"), CHESS_SUCCESS);
    f1 = fopen("./nadav/expected_outs/one_tournament.stats", "r");
    f2 = fopen("./nadav/your_outs/one_tournament.stats", "r");
    ASSERT_TEST(compareFile(f1, f2) == 0);
    fclose(f1);
    fclose(f2);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 3, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 2, 3, FIRST_PLAYER, 20), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, SECOND_PLAYER, 50), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 1), CHESS_SUCCESS);
    ASSERT_EQ(chessAddTournament(chess, 2, 5, "London"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 3, 4, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 2, 3, DRAW, 300), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 5, 4, SECOND_PLAYER, 220), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 2), CHESS_SUCCESS);
    ASSERT_EQ(chessSaveTournamentStatistics(chess, "./nadav/your_outs/multi_tournament.stats"), CHESS_SUCCESS);
    f1 = fopen("./nadav/expected_outs/multi_tournament.stats", "r");
    f2 = fopen("./nadav/your_outs/multi_tournament.stats", "r");
    ASSERT_TEST(compareFile(f1, f2) == 0);
    fclose(f1);
    fclose(f2);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 3, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 2, 3, FIRST_PLAYER, 20), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, SECOND_PLAYER, 50), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 1), CHESS_SUCCESS);
    ASSERT_EQ(chessAddTournament(chess, 2, 5, "London"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 3, 4, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 2, 3, DRAW, 300), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 5, 4, SECOND_PLAYER, 220), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 2), CHESS_SUCCESS);
    ASSERT_EQ(chessRemoveTournament(chess, 1), CHESS_SUCCESS);
    ASSERT_EQ(chessSaveTournamentStatistics(chess, "./nadav/your_outs/remove_tournament.stats"), CHESS_SUCCESS);
    f1 = fopen("./nadav/expected_outs/remove_tournament.stats", "r");
    f2 = fopen("./nadav/your_outs/remove_tournament.stats", "r");
    ASSERT_TEST(compareFile(f1, f2) == 0);
    fclose(f1);
    fclose(f2);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 3, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 2, 3, FIRST_PLAYER, 20), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, SECOND_PLAYER, 50), CHESS_SUCCESS);
    ASSERT_EQ(chessAddTournament(chess, 2, 5, "London"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 3, 4, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 2, 3, DRAW, 300), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 5, 4, SECOND_PLAYER, 220), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 2), CHESS_SUCCESS);
    ASSERT_EQ(chessRemoveTournament(chess, 1), CHESS_SUCCESS);
    ASSERT_EQ(chessSaveTournamentStatistics(chess, "./nadav/your_outs/open_tournament.stats"), CHESS_SUCCESS);
    f1 = fopen("./nadav/expected_outs/open_tournament.stats", "r");
    f2 = fopen("./nadav/your_outs/open_tournament.stats", "r");
    ASSERT_TEST(compareFile(f1, f2) == 0);
    fclose(f1);
    fclose(f2);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 3, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 2, 3, FIRST_PLAYER, 20), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, SECOND_PLAYER, 50), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 1), CHESS_SUCCESS);
    ASSERT_EQ(chessAddTournament(chess, 2, 5, "London"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 3, 4, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 2, 3, DRAW, 300), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 5, 4, SECOND_PLAYER, 220), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 2), CHESS_SUCCESS);
    ASSERT_EQ(chessRemovePlayer(chess, 1), CHESS_SUCCESS);
    ASSERT_EQ(chessSaveTournamentStatistics(chess, "./nadav/your_outs/remove_player_at_end.stats"), CHESS_SUCCESS);
    f1 = fopen("./nadav/expected_outs/remove_player_at_end.stats", "r");
    f2 = fopen("./nadav/your_outs/remove_player_at_end.stats", "r");
    ASSERT_TEST(compareFile(f1, f2) == 0);
    fclose(f1);
    fclose(f2);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 3, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 2, 3, DRAW, 20), CHESS_SUCCESS);
    ASSERT_EQ(chessRemovePlayer(chess, 1), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, DRAW, 50), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 1), CHESS_SUCCESS);
    ASSERT_EQ(chessAddTournament(chess, 2, 5, "London"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 3, 4, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 2, 3, DRAW, 300), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 5, 4, SECOND_PLAYER, 220), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 2), CHESS_SUCCESS);
    ASSERT_EQ(chessSaveTournamentStatistics(chess, "./nadav/your_outs/remove_player_at_mid.stats"), CHESS_SUCCESS);
    f1 = fopen("./nadav/expected_outs/remove_player_at_mid.stats", "r");
    f2 = fopen("./nadav/your_outs/remove_player_at_mid.stats", "r");
    ASSERT_TEST(compareFile(f1, f2) == 0);
    fclose(f1);
    fclose(f2);
    chessDestroy(chess);

    chess = chessCreate();
    ASSERT_NE(chess, NULL);
    ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 3, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 2, 3, FIRST_PLAYER, 20), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 1, 1, 2, SECOND_PLAYER, 50), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 1), CHESS_SUCCESS);
    ASSERT_EQ(chessRemovePlayer(chess, 2), CHESS_SUCCESS);
    ASSERT_EQ(chessAddTournament(chess, 2, 5, "London"), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 3, 4, FIRST_PLAYER, 60), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 2, 3, DRAW, 300), CHESS_SUCCESS);
    ASSERT_EQ(chessAddGame(chess, 2, 5, 4, SECOND_PLAYER, 220), CHESS_SUCCESS);
    ASSERT_EQ(chessEndTournament(chess, 2), CHESS_SUCCESS);
    ASSERT_EQ(chessSaveTournamentStatistics(chess, "./nadav/your_outs/remove_winner.stats"), CHESS_SUCCESS);
    f1 = fopen("./nadav/expected_outs/remove_winner.stats", "r");
    f2 = fopen("./nadav/your_outs/remove_winner.stats", "r");
    ASSERT_TEST(compareFile(f1, f2) == 0);
    fclose(f1);
    fclose(f2);
    chessDestroy(chess);

    // ToDo: decide if need, if so how to implement
    // chess = chessCreate();
    // ASSERT_NE(chess, NULL);
    // ASSERT_EQ(chessAddTournament(chess, 1, 5, "Paris"), CHESS_SUCCESS);
    // ASSERT_EQ(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 60), CHESS_SUCCESS);
    // ASSERT_EQ(chessSaveTournamentStatistics(NULL, (char *)"this/path/does/not/exist/test.stats"), CHESS_NULL_ARGUMENT);
    // ASSERT_EQ(chessSaveTournamentStatistics(chess, (char *)"this/path/does/not/exist/test.stats"), CHESS_NO_TOURNAMENTS_ENDED);
    // ASSERT_EQ(chessEndTournament(chess, 1), CHESS_SUCCESS);
    // ASSERT_EQ(chessSaveTournamentStatistics(chess, (char *)"this/path/does/not/exist/test.stats"), CHESS_SAVE_FAILURE);
    // chessDestroy(chess);

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
        testChessDestroy_maaroof,
        testChessAddTournamentAndEndTournament_maaroof,
        testChessAddGame_maaroof,
        testRemoveTournament_maaroof,
        testChessRemovePlayer_maaroof,
        testChessRemovePlayer_2_maaroof,
        testAvgGameTime_maaroof,
        testSavePlayerLevelsAndTournamentStatistics_maaroof,
        testAndStatsLevels,
        testChessCreateDestroy_nadav,
        testChessPlayers_nadav,
        testChessSanity_nadav,
        testChessModification_nadav,
        testChessStatistics_nadav
};

/*The names of the test functions should be added here*/
const char* testNames[] = {
        "testChessAddTournament",
        "testChessRemoveTournament",
        "testChessAddGame",
        "testChessPrintLevelsAndTournamentStatistics",
        "testBadArguments",
        "testPrivate",
        "testChessDestroy_maaroof",
        "testChessAddTournamentAndEndTournament_maaroof",
        "testChessAddGame_maaroof",
        "testRemoveTournament_maaroof",
        "testChessRemovePlayer_maaroof",
        "testChessRemovePlayer_2_maaroof",
        "testAvgGameTime_maaroof",
        "testSavePlayerLevelsAndTournamentStatistics_maaroof",
        "testAndStatsLevels",
        "testChessCreateDestroy_nadav",
        "testChessPlayers_nadav",
        "testChessSanity_nadav",
        "testChessModification_nadav",
        "testChessStatistics_nadav"
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