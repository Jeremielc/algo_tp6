all : L_ADJ queue perso_queue main
	gcc main.o L_ADJ.o queue.o perso_queue.o -std=c99 -Wall -o output
	rm ./*.o
	
main : sources/main.c
	gcc -c sources/main.c -std=c99 -Wall
	
L_ADJ : sources/L_ADJ.c
	gcc -c sources/L_ADJ.c -std=c99 -Wall

queue : sources/queue.c
	gcc -c sources/queue.c -std=c99 -Wall

perso_queue : sources/perso_queue.c
	gcc -c sources/perso_queue.c -std=c99 -Wall