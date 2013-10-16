/**
 * Compiles with:
 *  gcc -Wall -Werror -o testHunter game.c hunter.c HunterView.c 
 * 
 * (And any additional files you wrote for HunterView implementation)
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "game.h"
#include "cities.h"
#include "HunterView.h"
#include "hunter.h"

//includes '\0'
#define MOVE_SIZE 3

static char latestPlay[MOVE_SIZE] = "";

void testInitialMoves(void);
void testMovesRound1(void);
void testAll(void);

int main( int argc, char *argv[] ) {
    testAll();
    return EXIT_SUCCESS;
}

void testAll(void){
    testInitialMoves();
    testMovesRound1();
}

int moveIn(char * locs[], int size){
    int i;
    for(i=0; i< size; i++){
        if(strcmp(latestPlay,locs[i]) == 0) return 1;
    }
    return 0;
}

void testMovesRound1(void){
      HunterView gameState;
     
    {
       playerMessage messages[7] = {""};
       printf("Test: Round 1 hunter 2 moves from AT\n");
       gameState = newHunterView("GGW.... SGA.... HAT.... MMA.... DBO.V.. GGW.... SKL....",messages);
       decideMove( gameState );
       disposeHunterView( gameState );
       char * possibleLocations[2] = {"VA","AT"};
       assert(moveIn(possibleLocations,2));
       printf("Test passed\n");
   }

}

void testInitialMoves(void){
   HunterView gameState;

   {
       playerMessage messages[] = {};
       printf("Test 1: Round 0 hunter 0 in GW\n");   
       gameState = newHunterView("",messages);
       decideMove( gameState );
       disposeHunterView( gameState );   
       assert(strcmp(latestPlay,"GW") == 0);
       printf("Test passed\n");
   }   

   {
       playerMessage messages[] = {""};                                                        
       printf("Test 2: Round 0 hunter 1 in GA\n");                 
       gameState = newHunterView("GGW....",messages);
       decideMove( gameState );
       disposeHunterView( gameState );
       assert(strcmp(latestPlay,"GA") == 0);
       printf("Test passed\n");
   }

    {
       playerMessage messages[2] = {""};
       printf("Test 3: Round 0 hunter 2 in AT\n");
       gameState = newHunterView("GGW.... SGA....",messages);
       decideMove( gameState );
       disposeHunterView( gameState );
       assert(strcmp(latestPlay,"AT") == 0);
       printf("Test passed\n");
   }
   
    {
       playerMessage messages[3] = {""};
       printf("Test 4: Round 0 hunter 3 in MA\n");
       gameState = newHunterView("GGW.... SGA.... HAT....",messages);
       decideMove( gameState );
       disposeHunterView( gameState );
       assert(strcmp(latestPlay,"MA") == 0);
       printf("Test passed\n");
   }
}

// Saves characters from play (and appends a terminator)
// and saves characters from message (and appends a terminator)
void registerBestPlay ( char *play, playerMessage message ) {
   strncpy( latestPlay, play, MOVE_SIZE-1 );
   latestPlay[MOVE_SIZE-1] = '\0';
}


