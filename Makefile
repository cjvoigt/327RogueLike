all: main.o dungeon.o room.o
	gcc main.o dungeon.o room. -o all

main: main.c room.h dungeon.h
	gcc -Wall -Werror -ggdb main.c -c

dungeon: dungeon.c dungeon.h room.h
	gcc -Wall -Werror -ggdb dungeon.c -c

room: room.c room.h
	gcc -Wall -Werror -ggdb room.c -c

clean:
	rm -f *.o fc