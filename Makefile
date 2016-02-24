main: main.o Dungeon.o PlayerCharacter.o binheap.o Monster.o
	gcc -Wall -Werror  main.o Dungeon.o PlayerCharacter.o binheap.o Monster.o -o main

main.o: main.c Dungeon.h
	gcc -ggdb main.c -c

dungeon.o: Dungeon.c Dungeon.h
	gcc -ggdb Dungeon.c -c

playercharacter.o: PlayerCharacter.h PlayerCharacter.c utils.h Dungeon.h
	gcc -ggdb PlayerCharacter.c -c

binheap.o: binheap.h binheap.c
	gcc -ggdb binheap.c -c

monster.o: Monster.h Monster.c utils.h Dungeon.c
	gcc -ggdb Monster.c -c

clean:
	rm -f *.o main
