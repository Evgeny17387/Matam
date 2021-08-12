TEST_NAMES=dot1 dot2 dot3 dot4 dot5 dot6 dot7 dot8 dot9 dot10
TEST_SUBJECT=mtm_tot

COMP_FLAG=-std=c99 -Wall -pedantic-errors -Werror -DNDEBUG
CC=gcc

test_all: clean_test_res mtm_tot $(TEST_NAMES) 
	echo "all tests success!"

dot%: test%R.out check_diff_in_test_%
	echo "finished test"
 
test%R.out: test%.in
	./$(TEST_SUBJECT) < $^ > $@

check_diff_in_test_%: test%R.out test%.out
	diff $^

mtm_tot: part1.c
	$(CC) $(COMP_FLAG) $^ -o $@

clean_test_res:
	touch test___R.out
	rm test*R.out

clean: 
	rm mtm_tot test*R.out
