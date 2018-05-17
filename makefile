CC=gcc
CFLAGS=-I
fun: main.c
	$(CC) -o fun main.c -F/Library/Frameworks -framework SDL2 -framework SDL2_Image 
