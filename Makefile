CC = gcc
CFLAGS = -std=c99
WITHOUT_MAINO = funcs.o first_formating.o input_and_output.o extra_funcs.o

all: main.o $(WITHOUT_MAINO)
	$(CC) main.o $(WITHOUT_MAINO) -o cw $(CFLAGS)

main.o: main.c $(WITHOUT_MAINO) *.h
	$(CC) -c main.c $(CFLAGS)

input_and_output.o: input_and_output.c
	$(CC) -c input_and_output.c $(CFLAGS)

first_formating.o: first_formating.c
	$(CC) -c first_formating.c $(CFLAGS)

funcs.o: funcs.c
	$(CC) -c funcs.c $(CFLAGS)

extra_funcs.o: extra_funcs.c
	$(CC) -c extra_funcs.c $(CFLAGS)

clean:
	rm *.o cw

