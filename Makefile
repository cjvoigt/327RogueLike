all: main.o Dungeon.o Room.o
	gcc main.o Dungeon.o Room.o -o all

main.o: main.c Room.h Dungeon.h
	gcc -Wall -Werror -ggdb main.c -c

dungeon.o: Dungeon.c Dungeon.h Room.h
	gcc -Wall -Werror -ggdb dungeon.c -c

room.o: Room.h Room.c
	gcc -Wall -Werror -ggdb  room.c -c

clean:
	rm -f *.o fc