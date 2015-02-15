CC = gcc

calc: stackcalc.c longnumber.c stack.c slist.c
	$(CC) -o launch.l stackcalc.c longnumber.c stack.c slist.c -g -pg

calc_s: stackcalc_short.c stack.c slist.c
	$(CC) -o launch.l stackcalc_short.c stack.c slist.c -g -pg

longnum: longnum_tester.c longnumber.c slist.c
	$(CC) -o launch.l longnum_tester.c longnumber.c slist.c -g -pg

slist: slist_tester.c slist.c
	$(CC) -o launch.l slist_tester.c slist.c -g -pg

clean:
	$(RM) launch.l
