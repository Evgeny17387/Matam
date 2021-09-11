CC = g++
OBJS = main.o examDetails.o
CFLAGS = -std=c++11 -Wall -Werror -pedantic-errors -g
EXEC = test

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

main.o: main.cpp sortedList.h testClass.h
	$(CC) $(CFLAGS) -c $*.cpp

examDetails.o: examDetails.cpp examDetails.h
	$(CC) $(CFLAGS) -c $*.cpp

clean:
	rm -f $(OBJS)
