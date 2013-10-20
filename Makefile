all: HunterView.h hunter.h
	gcc -g -o testHunter HunterView.c Queue.c testHunter.c hunter.c
clean:
	rm -rf testHunter
