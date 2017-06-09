a.out: main.o bresenham.o vertex.o drawing.o
	gcc main.o bresenham.o vertex.o drawing.o -lm -Wall
main.o: main.c
	gcc -c main.c -Wall -lm
bresenham.o: bresenham.c
	gcc -c bresenham.c -Wall -lm
vertex.o: vertex.c
	gcc -c vertex.c -Wall -lm
drawing.o: drawing.c
	gcc -c drawing.c -Wall -lm
clean:
	rm main.o bresenham.o vertex.o drawing.o a.out
run:
	./a.out
indent:
	indent main.c bresenham.c vertex.c drawing.c bresenham.h vertex.h drawing.h
