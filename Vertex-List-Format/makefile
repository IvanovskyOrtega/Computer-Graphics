a.out: main.o vertex.o
	gcc main.o vertex.o
main.o: main.c
	gcc -c main.c -Wall
vertex.o: vertex.c
	gcc -c vertex.c -Wall
clean:
	rm main.o vertex.o a.out vlf.txt elf.txt flf.txt
indent:
	indent main.c
	indent vertex.c
	indent vertex.h
run: a.out
	./a.out
