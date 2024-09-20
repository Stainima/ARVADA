
arvada: ARVADA.o
	gcc ARVADA.o -o arvada

ARVADA.o: ARVADA.c ARVADA.h
	gcc -c ARVADA.c

clean:
	rm arvada ARVADA.o
