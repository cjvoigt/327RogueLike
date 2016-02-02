main: main.o Dungeon.o
	gcc main.o Dungeon.o -o main

main.o: main.c Room.h Dungeon.h
	gcc -Wall -Werror -ggdb main.c -c

dungeon.o: Dungeon.c Dungeon.h Room.h
	gcc -Wall -Werror -ggdb dungeon.c -c

clean:
	rm -f *.o main
