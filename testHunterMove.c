// Test file I used to investigate an issue where
// our AI got disqualified in the testing round

// Feel free to modify the pastPlays string in the
// newHunterView call to test whatever situation you want

#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "hunter.h"
#include "HunterView.h"

int main(int argc, char *argv[]) {
    HunterView hv = newHunterView("GKL.... SLO.... HMA.... MMI.... DC?.V..", NULL);
    decideMove(hv);
    return EXIT_SUCCESS;
}

void registerBestPlay(char *play, playerMessage message) {
    printf("Registered move: %s\nMessage: %s\n\n", play, message);
}