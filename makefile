CC := gcc
Cflags := -fsanitize=address
arvada: ARVADA.o ARVADA_helper.o
	$(CC) $(Cflags) ARVADA_helper.o ARVADA.o -o arvada

ARVADA.o: ARVADA.c ARVADA.h ARVADA_helper.c
	$(CC) -c ARVADA.c

clean:
	rm arvada ARVADA.o ARVADA_helper.o
