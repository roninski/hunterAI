#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "game.h"
#include "HunterView.h"
#include "hunter.h"
#include "cities.h"

void headTowards(HunterView gameState, LocationID to, LocationID from, PlayerID player, int round, playerMessage message);

void decideMove(HunterView gameState) {
    srand(time(NULL));
    
    int currRound = getRound(gameState);
    PlayerID currPlayer = getCurrentPlayer(gameState);
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
                headTowards(gameState, KLAUSENBURG, currLoc, currPlayer, currRound, "Head towards KLAUSENBURG");
            }
        } else { // hunters 1, 2, 3 move randomly until draculas trail is found
            adjLocations = connectedLocations(gameState, &adjLocsSize, currLoc, currPlayer, currRound, 1, 1, 1);
            registerBestPlay(cities[adjLocations[rand() % adjLocsSize]], "Move randomly via road, rail, or sea.");
        }
        // TODO if we're in draculas trail keep following him
    }
}

// Does a BFS, finds shortest path to location, and registers move
void headTowards(HunterView gameState, LocationID to, LocationID from, PlayerID player, int round, playerMessage message) {
    int i;
    int done = FALSE;
    Queue q = newQueue();
    LocationID st[NUM_MAP_LOCATIONS]; // used to find path
    int distance[NUM_MAP_LOCATIONS]; // used to calculate round numbers
    for (i = 0; i < NUM_MAP_LOCATIONS; i++) {
        st[i] = -1;
        distance[i] = -1;
    }
    int adjLocsSize;
    LocationID *adjLocations;
    
    st[from] = from;
    distance[from] = 0;
    QueueJoin(q, from);
    while (!QueueIsEmpty(q)) {
        LocationID l = QueueLeave(q);
        adjLocations = connectedLocations(gameState, &adjLocsSize, l, player, round + distance[l], 1, 1, 1);
        for (i = 0; i < adjLocsSize; i++) {
            if (adjLocations[i] == to) { 
                st[adjLocations[i]] = l;
                done = TRUE;
                break;
            } else if (st[adjLocations[i]] == -1) {
                st[adjLocations[i]] = l;
                distance[adjLocations[i]] = 1 + distance[l];
                QueueJoin(q, adjLocations[i]);
            }
        }
        if (done) {
            break;
        }
    }
    dropQueue(q);
    if (done) { // this means path was found, which should always be the case, but putting it anyway
        LocationID prev = st[to];
        while (st[prev] != from) {
            prev = st[prev];
        }
        // so now st[prev] == from
        // so our next move is prev
        registerBestPlay(cities[prev], message);
    }
}





