all: main.o dungeon.o room.o
	gcc main.o dungeon.o room. -o all

main.o: main.c room.h dungeon.h
	gcc -Wall -Werror -ggdb main.c -c

dungeon.o: dungeon.c dungeon.h room.h
	gcc -Wall -Werror -ggdb dungeon.c -c

room.o: room.h room.c
	gcc -Wall -Werror -ggdb room.c -c

clean:
	rm -f *.o fc