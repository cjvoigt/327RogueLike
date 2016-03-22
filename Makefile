main: main.o Dungeon.o PlayerCharacter.o binheap.o Monster.o
	gcc -Wall -Werror  main.o Dungeon.o PlayerCharacter.o binheap.o Monster.o -o main -lncurses

main.o: main.c Dungeon.h
	gcc -ggdb main.c -c

dungeon.o: Dungeon.c Dungeon.h
	gcc -ggdb Dungeon.c -c

character.o: character.h character.cpp
	g++ -ggdb character.cpp -c

player.o: player.h player.cpp utils.h Dungeon.h
	g++ -ggdb player.cpp -c

monster.o: monster.h monster.cpp utils.h Dungeon.c
	g++ -ggdb monster.cpp -c

binheap.o: binheap.h binheap.c
	gcc -ggdb binheap.c -c

monster.o: Monster.h Monster.c utils.h Dungeon.c
	gcc -ggdb Monster.c -c

clean:
	rm -f *.o main
