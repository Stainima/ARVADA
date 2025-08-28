CC := gcc
Cflags := -fsanitize=address
Ofiles := ARVADA.o ARVADA_helper.o simple_while_oracle.o
Cfiles := ARVADA.c ARVADA.h ARVADA_helper.c simple_while_oracle.c

arvada: $(Ofiles)
	$(CC) $(Cflags) $(Ofiles) -o arvada.out

debug: $(Ofiles)
	$(CC) $(Ofiles) -o arvada.out

ARVADA.o: $(Cfiles)
	$(CC) -c $(Cfiles)

clean:
	rm arvada.out $(Ofiles)
