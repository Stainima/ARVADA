CC := gcc
arvada: ARVADA.o
	$(CC) ARVADA.o -o arvada

ARVADA.o: ARVADA.c ARVADA.h
	$(CC) -c ARVADA.c

clean:
	rm arvada ARVADA.o
