CC = gcc

calc: stackcalc.c longnumber.c stack.c slist.c
	$(CC) -o launch.l stackcalc.c longnumber.c stack.c slist.c

calc_s: stackcalc_short.c stack.c slist.c
	$(CC) -o launch.l stackcalc_short.c stack.c slist.c

longnum: longnum_tester.c longnumber.c slist.c
	$(CC) -o launch.l longnum_tester.c longnumber.c slist.c

slist: slist_tester.c slist.c
	$(CC) -o launch.l slist_tester.c slist.c

clean:
	$(RM) launch.l
