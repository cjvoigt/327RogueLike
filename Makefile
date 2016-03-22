main: main.o Dungeon.o character.o player.o monster.o binheap.o
	g++ -Wall -Werror  main.o Dungeon.o PlayerCharacter.o binheap.o Monster.o -o main -lncurses

main.o: main.c
	gcc -ggdb main.c -c

dungeon.o: Dungeon.c Dungeon.h
	gcc -ggdb Dungeon.c -c

character.o: character.h character.c
	g++ -ggdb character.cpp -c

player.o: player.h player.cpp utils.h Dungeon.h
	g++ -ggdb player.cpp -c

monster.o: monster.h monster.cpp utils.h Dungeon.c
	g++ -ggdb monster.cpp -c

binheap.o: binheap.h binheap.c
	gcc -ggdb binheap.c -c

clean:
	rm -f *.o main
