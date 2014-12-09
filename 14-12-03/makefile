CC = gcc

calc: stackcalc_short.c longnumber.c stack.c slist.c
	$(CC) -o launch stackcalc_short.c longnumber.c stack.c slist.c

calc_s: stackcalc_short.c stack.c slist.c
	$(CC) -o launch stackcalc_short.c stack.c slist.c

longnum: longnum_tester.c longnumber.c slist.c
	$(CC) -o launch longnum_tester.c longnumber.c slist.c

slist: slist_tester.c slist.c
	$(CC) -o launch slist_tester.c slist.c

clean:
	$(RM) launch
