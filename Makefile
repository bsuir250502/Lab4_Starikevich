all:
	gcc -Wall -o lab4.bin lab4.c mylib.c -lm && ./lab4.bin
clean: 
	rm lab4.bin
