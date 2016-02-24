main: main.o Dungeon.o playercharacter.o binheap.o
	gcc -Wall -Werror  main.o Dungeon.o PlayerCharacter.o binheap.o monster.o utils.o -o main

main.o: main.c Room.h Dungeon.h
	gcc -Wall -Werror -ggdb main.c -c

dungeon.o: Dungeon.c Dungeon.h Room.h
	gcc -Wall -Werror -ggdb Dungeon.c -c

playercharacter.o: PlayerCharacter.h PlayerCharacter.c utils.h Dungeon.h
	gcc -Wall -Werror -ggdb PlayerCharacter.c -c

binheap.o: binheap.h binheap.c
	gcc -Wall -Werror -ggdb binheap.c -c

monster.o: Monster.h Monster.c utils.h Dungeon.c
	gcc -Wall -Werror -ggdb Monster.c -c

utils.o: utils.h
	gcc -Wall -Werror -ggdb utils.h -c

clean:
	rm -f *.o main
