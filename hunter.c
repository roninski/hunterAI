#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "game.h"
#include "HunterView.h"
#include "hunter.h"
#include "cities.h"
#include "Queue.h"

void randomMove(LocationID *adjLocations, int adjLocsSize, LocationID currLoc, LocationID lastMove);
int adjArrayContains(LocationID location, LocationID *adjLocations, int adjLocsSize);
LocationID draculaLocation(HunterView hv, int *turnsAgo);
//int dracTrailContains(HunterView hv, LocationID location);
void headTowards(HunterView hv, LocationID to, LocationID from, PlayerID player, int round, playerMessage message);

void decideMove(HunterView gameState) {
    int i;
    srand(time(NULL));
    
    int currRound = getRound(gameState);
    PlayerID currPlayer = getCurrentPlayer(gameState);
    LocationID currLoc = getLocation(gameState, currPlayer);
    LocationID ourTrail[TRAIL_SIZE];
    getHistory(gameState, currPlayer, ourTrail);
    
    int adjLocsSize;
    LocationID *adjLocations;

    int trailLocationAge;
    LocationID trailLocation = draculaLocation(gameState, &trailLocationAge);

    // this gives the time since we were at trailLocation
    int dracPosInHunterTrail = TRAIL_SIZE;
    for (i = 0; i < TRAIL_SIZE; i++) {
        if (ourTrail[i] == trailLocation) {
            dracPosInHunterTrail = i;
            break;
        }
    }
    
    if (currRound == 0) { // pick starting locations
        if (currPlayer == 0) {
            registerBestPlay(locToStr(KLAUSENBURG), "Start at KLAUSENBURG");
        } else if (currPlayer == 1) {
            registerBestPlay(locToStr(LONDON), "Start at LONDON");
        } else if (currPlayer == 2) {
            registerBestPlay(locToStr(MADRID), "Start at MADRID");
        } else {
            registerBestPlay(locToStr(MILAN), "Start at MILAN");
        }
    } else { // for subsequent rounds
        adjLocations = connectedLocations(gameState, &adjLocsSize, currLoc, currPlayer, currRound, 1, 1, 1);
        
        // some common sense moves that override anything fancy
        if (trailLocationAge == 0 && adjArrayContains(trailLocation, adjLocations, adjLocsSize)) {
            registerBestPlay(locToStr(trailLocation), "Go to draculas current location");
            return;
        } else if (trailLocation == UNKNOWN_LOCATION && getHealth(gameState, currPlayer) < GAME_START_HUNTER_LIFE_POINTS) {
            registerBestPlay(locToStr(currLoc), "Rest to regain health");
            return;
        }
        
        if (currPlayer == 0) { // hunter 0 camps outside castle dracula
            if (currLoc == GALATZ) {
                registerBestPlay(locToStr(KLAUSENBURG), "Move to KLAUSENBURG");
            } else if (currLoc == KLAUSENBURG) {
                registerBestPlay(locToStr(GALATZ), "Move to GALATZ");
            } else {
                registerBestPlay(locToStr(currLoc), "Rest"); // should be overwritten by move below
                headTowards(gameState, KLAUSENBURG, currLoc, currPlayer, currRound, "Head towards KLAUSENBURG");
            }
            
            // if we're in or one step away from draculas trail then follow him
            /*if (dracTrailContains(gameState, currLoc) || adjArrayContains(trailLocation, adjLocations, adjLocsSize)) {
                if (trailLocationAge < dracPosInHunterTrail) {
                    // if dracula has been there since we were last there, head there
                    headTowards(gameState, trailLocation, currLoc, currPlayer, currRound, "Following draculas trail");
                } else {
                    // if we've been there since dracula's been there, he's moved, so we guess where
                    randomMove(adjLocations, adjLocsSize, currLoc, ourTrail[1]);
                }
                
            }*/
        } else { // hunters 1, 2, 3 move randomly until draculas trail is found
            // register random move first
            randomMove(adjLocations, adjLocsSize, currLoc, ourTrail[1]);
            // but then if we know draculas trail then follow him
            if (trailLocation != UNKNOWN_LOCATION && trailLocationAge < dracPosInHunterTrail) {
                // Note: if trailLocationAge >= dracPosInHunterTrail then he's not still here
                headTowards(gameState, trailLocation, currLoc, currPlayer, currRound, "Following draculas trail");
            }
            // TODO maybe find a smarter way to follow his trail :/
        }
    }
}

// Here to avoid code dupliation
void randomMove(LocationID *adjLocations, int adjLocsSize, LocationID currLoc, LocationID lastMove) {
    LocationID possibleMove = adjLocations[rand() % adjLocsSize];
    while (possibleMove == currLoc || (possibleMove == lastMove && adjLocsSize > 2)) {
        possibleMove = adjLocations[rand() % adjLocsSize];
    }
    registerBestPlay(locToStr(possibleMove), "Move randomly via road, rail, or sea.");
}

// returns TRUE or FALSE showing whether location is a valid move
// this function is just here to make code neater
int adjArrayContains(LocationID location, LocationID *adjLocations, int adjLocsSize) {
    int i;
    for (i = 0; i < adjLocsSize; i++) {
        if (adjLocations[i] == location) {
            return TRUE;
        }
    }
    return FALSE;
}

// Returns the most recent location in dracula's trail
// Stores the number of turns since he was there in turnsAgo
// turnsAgo would be 0 if he is still there
LocationID draculaLocation(HunterView hv, int *turnsAgo) {
    LocationID trail[TRAIL_SIZE];
    getHistory(hv, PLAYER_DRACULA, trail);

    int i;
    for (i = 0; i < TRAIL_SIZE; i++) {
        if (trail[i] >= 0 && trail[i] < NUM_MAP_LOCATIONS) {
            *turnsAgo = i;
            return trail[i];
        } else if (trail[i] >= DOUBLE_BACK_1 && trail[i] <= DOUBLE_BACK_5) {
            // If he's doubled back to a position still in the trail
            // That position is his most recent known location
            int dbTrailPosition = i + (trail[i] - (DOUBLE_BACK_1 - 1)); // i + doubleBackSteps
            if (dbTrailPosition < TRAIL_SIZE
                    && trail[dbTrailPosition] >= 0 && trail[dbTrailPosition] < NUM_MAP_LOCATIONS) {
                *turnsAgo = i;
                return trail[dbTrailPosition];
            }
        }
    }

    *turnsAgo = -1;
    return UNKNOWN_LOCATION;
}

// returns TRUE or FALSE showing whether the given location is in dracula's trail
// this function is also just here to make code neater
/*int dracTrailContains(HunterView hv, LocationID location) {
    int i;
    LocationID trail[TRAIL_SIZE];
    getHistory(hv, PLAYER_DRACULA, trail);
    for (i = 0; i < TRAIL_SIZE; i++) {
        if (trail[i] == location) {
            return TRUE;
        }
    }
    return FALSE;
}*/

// Does a BFS, finds shortest path to location, and registers move
void headTowards(HunterView hv, LocationID to, LocationID from, PlayerID player, int round, playerMessage message) {
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
        adjLocations = connectedLocations(hv, &adjLocsSize, l, player, round + distance[l], 1, 1, 1);
        for (i = 0; i < adjLocsSize; i++) {
            if (adjLocations[i] == to) { // we're done now
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
        registerBestPlay(locToStr(prev), message);
    }
}

