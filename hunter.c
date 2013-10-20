#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "game.h"
#include "HunterView.h"
#include "hunter.h"
#include "cities.h"


void decideMove (HunterView gameState) {
    srand(time(NULL));
    
    int currRound = getRound(gameState);
    int currPlayer = getCurrentPlayer(gameState);
    LocationID currLoc = getLocation(gameState, currPlayer);
    
    int adjLocsSize;
    LocationID *adjLocations;

    if (currRound == 0) { // pick starting locations
        if (currPlayer == 0) {
            registerBestPlay(cities[KLAUSENBURG], "Start at KLAUSENBURG");
        } else if (currPlayer == 1) {
            registerBestPlay(cities[LONDON], "Start at LONDON");
        } else if (currPlayer == 2) {
            registerBestPlay(cities[MADRID], "Start at MADRID");
        } else {
            registerBestPlay(cities[MILAN], "Start at MILAN");
        }
    } else {
        if (currPlayer == 0) { // hunter 0 camps outside castle dracula
            if (currLoc == GALATZ) {
                registerBestPlay(cities[KLAUSENBURG], "Move to KLAUSENBURG");
            } else if (currLoc == KLAUSENBURG) {
                registerBestPlay(cities[GALATZ], "Move to GALATZ");
            } else {
                registerBestPlay(cities[currLoc], "Rest");
                // TODO head back to KLAUSENBURG
            }
        } else { // hunters 1, 2, 3 move randomly until draculas trail is found
            adjLocations = connectedLocations(gameState, &adjLocsSize, currLoc, currPlayer, currRound, 1, 1, 1);
            registerBestPlay(cities[adjLocations[rand() % adjLocsSize]], "Move randomly via road, rail, or sea.");
        }
        // TODO if we're in draculas trail keep following him
    }
}

