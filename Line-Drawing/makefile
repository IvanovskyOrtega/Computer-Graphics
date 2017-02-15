a.out: main.o functions.o  
	gcc main.o functions.o 
main.o: main.c
	gcc -c main.c -Wall
functions.o: functions.c
	gcc -c functions.c -Wall
clean:
	rm main.o functions.o 
run: a.out
	./a.out

