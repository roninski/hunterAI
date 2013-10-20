all:
	gcc -Wall -Werror -O -c hunter.c HunterView.c Queue.c cities.c -ljansson

clean:
	rm -rf myPlayer
