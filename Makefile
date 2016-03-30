main: main.o dungeon.o character.o player.o monster.o monsterparser.o binheap.o
	g++ -Wall -Werror *.o -o main -lncurses

main.o: main.cpp
	g++ -ggdb main.cpp -c

dungeon.o: dungeon.cpp dungeon.h
	g++ -ggdb dungeon.cpp -c

character.o: character.h character.cpp
	g++ -ggdb character.cpp -c

player.o: player.h player.cpp character.h  utils.h dungeon.h
	g++ -ggdb player.cpp -c

monster.o: monster.h monster.cpp utils.h dungeon.h
	g++ -ggdb monster.cpp -c

monsterparser.o: monsterparser.h monsterparser.cpp
	g++ -ggdb monsterparser.cpp -c

binheap.o: binheap.h binheap.cpp
	g++ -ggdb binheap.cpp -c

clean:
	rm -f *.o main
