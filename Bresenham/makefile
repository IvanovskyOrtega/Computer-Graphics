a.out: main.o bresenham.o  
	gcc main.o bresenham.o 
main.o: main.c
	gcc -c main.c -Wall
bresenham.o: bresenham.c
	gcc -c bresenham.c -Wall
clean:
	rm main.o bresenham.o 
run: a.out
	./a.out

