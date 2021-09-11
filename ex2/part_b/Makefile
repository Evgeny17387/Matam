CC = g++
OBJS1 = gameTest.o Game.o Exceptions.o Auxiliaries.o Character.o Soldier.o Medic.o Sniper.o
OBJS2 = partC_tester.o Game.o Exceptions.o Auxiliaries.o Character.o Soldier.o Medic.o Sniper.o
CFLAGS = -std=c++11 -Wall -Werror -pedantic-errors
EXEC = test

$(EXEC): $(OBJS2)
	$(CC) $(CFLAGS) $(OBJS2) -o $@

partC_tester.o: partC_tester.cpp Game.h Auxiliaries.h Character.h Exceptions.h
	$(CC) $(CFLAGS) -c $*.cpp

gameTest.o: gameTest.cpp Game.h Auxiliaries.h Character.h Exceptions.h
	$(CC) $(CFLAGS) -c $*.cpp

Game.o: Game.cpp Game.h Auxiliaries.h Character.h Exceptions.h Soldier.h Medic.h Sniper.h
	$(CC) $(CFLAGS) -c $*.cpp

Exceptions.o: Exceptions.cpp Exceptions.h
	$(CC) $(CFLAGS) -c $*.cpp

Auxiliaries.o: Auxiliaries.cpp Auxiliaries.h
	$(CC) $(CFLAGS) -c $*.cpp

Character.o: Character.cpp Character.h Auxiliaries.h
	$(CC) $(CFLAGS) -c $*.cpp

Soldier.o: Soldier.cpp Soldier.h Character.h Auxiliaries.h
	$(CC) $(CFLAGS) -c $*.cpp

Medic.o: Medic.cpp Medic.h Character.h Auxiliaries.h
	$(CC) $(CFLAGS) -c $*.cpp

Sniper.o: Sniper.cpp Sniper.h Character.h Auxiliaries.h
	$(CC) $(CFLAGS) -c $*.cpp

clean:
	rm -f $(OBJS2)
