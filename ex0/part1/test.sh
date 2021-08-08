#!/bin/bash

clear

gcc -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG part1.c -o mtm_tot

./mtm_tot < test1.in > test1_private.out
./mtm_tot < test2.in > test2_private.out
./mtm_tot < test3.in > test3_private.out
./mtm_tot < test4.in > test4_private.out

diff test1.out test1_private.out
diff test2.out test2_private.out
diff test3.out test3_private.out
diff test4.out test4_private.out
