all: main.o funcs.o first_formating.o input_and_output.o
	gcc *.o -o cw -std=c99
main.o: main.c funcs.o first_formating.o input_and_output.o *.h
	gcc -c main.c -std=c99
input_and_output.o :input_and_output.c
	gcc -c input_and_output.c -std=c99
first_formating.o: first_formating.c 
	gcc -c first_formating.c -std=c99
funcs.o: funcs.c 
	gcc -c funcs.c -std=c99
clean:
	rm *.o cw
