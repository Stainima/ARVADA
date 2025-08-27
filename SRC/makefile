CC := gcc
Cflags := -fsanitize=address

arvada: ARVADA.o ARVADA_helper.o
	$(CC) $(Cflags) ARVADA_helper.o ARVADA.o -o arvada.out -ljson-c

debug: ARVADA.o ARVADA_helper.o
	$(CC) ARVADA_helper.o ARVADA.o -o arvada.out -ljson.c

ARVADA.o: ARVADA.c ARVADA.h ARVADA_helper.c
	$(CC) -c ARVADA.c ARVADA_helper.c

clean:
	rm arvada.out ARVADA.o ARVADA_helper.o
