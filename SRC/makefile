CC := gcc
Cflags := -fsanitize=address
Ofiles := ARVADA.o ARVADA_helper.o parse_tree_builder.o string_concatenate.o replacement_functions.o
Cfiles := ARVADA.c ARVADA.h ARVADA_helper.c parse_tree_builder.c string_concatenate.c replacement_functions.c

arvada: $(Ofiles)
	$(CC) $(Cflags) $(Ofiles) -o arvada.out

debug: $(Ofiles)
	$(CC) $(Ofiles) -o arvada.out

ARVADA.o: $(Cfiles)
	$(CC) -c $(Cfiles)

clean:
	rm arvada.out $(Ofiles)
