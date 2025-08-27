CC := gcc
Cflags := -fsanitize=address
Ofiles := ARVADA.o ARVADA_helper.o while_oracle.o

arvada: $(Ofiles)
	$(CC) $(Cflags) $(Ofiles) -o arvada.out

debug: $(Ofiles)
	$(CC) $(Ofiles) -o arvada.out

ARVADA.o: ARVADA.c ARVADA.h ARVADA_helper.c while_oracle.c
	$(CC) -c ARVADA.c ARVADA_helper.c while_oracle.c

clean:
	rm arvada.out ARVADA.o ARVADA_helper.o while_oracle.o
