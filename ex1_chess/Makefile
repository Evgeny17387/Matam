CC = gcc
OBJS = chessSystemTestsExample.o chessSystem.o tournament.o game.o player.o utilities.o
CFLAGS = -std=c99 -Wall -Werror -pedantic-errors -DNDEBUG
EXEC = chessSystemTestsExample

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ -L. -lmap

chessSystemTestsExample.o: chessSystemTestsExample.c chessSystem.h test_utilities.h

chessSystem.o: chessSystem.c chessSystem.h map.h utilities.h tournament.h player.h game.h

tournament.o: tournament.c tournament.h chessSystem.h map.h game.h utilities.h player.h

game.o: game.c game.h chessSystem.h map.h player.h

player.o: player.c player.h map.h

utilities.o: utilities.c utilities.h map.h
