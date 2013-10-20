all: HunterView.h hunter.h
	gcc -g -o testHunter HunterView.c Queue.c cities.c testHunter.c hunter.c
clean:
	rm -rf testHunter
