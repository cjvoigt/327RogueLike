main: main.o Dungeon.o playercharacter.o binheap.o
	gcc -Wall -Werror  main.o Dungeon.o PlayerCharacter.o binheap.o -o main

main.o: main.c Room.h Dungeon.h
	gcc -Wall -Werror -ggdb main.c -c

dungeon.o: Dungeon.c Dungeon.h Room.h
	gcc -Wall -Werror -ggdb Dungeon.c -c

playercharacter.o: PlayerCharacter.h PlayerCharacter.c Dungeon.h Room.h binheap.h
	gcc -Wall -Werror -ggdb PlayerCharacter.c -c

binheap.o: binheap.h binheap.c
	gcc -Wall -Werror -ggdb binheap.c -c

clean:
	rm -f *.o main
