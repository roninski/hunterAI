/*
COMP1927 Assignment 2 - Fury of Dracula (Phase 1)
Luke Alderman, Charbel Antouny, Nathan Williams
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "game.h"
#include "cities.h"
#include "HunterView.h"
#include "Queue.h"

#define NONE 'N'
#define ROAD 'L'
#define RAIL 'T'
#define BOTH 'B'
#define SEA 'S'


typedef struct player {
    int health;
    PlayerID ID;
    LocationID curLocation;
    LocationID trail[TRAIL_SIZE];
    // note: trail[0] is where he was last turn, trail[1] the turn before that, etc.
} Player;

struct hunterView {
    int score;
    Round curRound;
    PlayerID currentPlayer;
    char *pastPlays;
    // Initialised to all 'N's
    // 'N' for no link, 'L' for land, 'T' for train, 'S' for Sea, 'B' for both Land and Train
    char map[NUM_MAP_LOCATIONS][NUM_MAP_LOCATIONS];
    Player players[NUM_PLAYERS]; //array of structs storing information about each player
    //playerMessage messages[];
};
    
void initialiseHunterView(HunterView HV);
void addLink(HunterView HV, LocationID L1, LocationID L2, char type);
int draculaAtSea(HunterView currentView);
int isSeaLocation(LocationID location);

void initialiseHunterView(HunterView HV) {
    // Variables
    int i, j;

    // initialise game
    HV->score = GAME_START_SCORE;
    HV->curRound=0;
    HV->currentPlayer = PLAYER_LORD_GODALMING;

    // initialise hunters
    for (i = 0; i < NUM_PLAYERS-1; i++) {
        HV->players[i].health = GAME_START_HUNTER_LIFE_POINTS;
        HV->players[i].ID = i;
        HV->players[i].curLocation = UNKNOWN_LOCATION;
        for (j = 0; j < TRAIL_SIZE; j++) {
            HV->players[i].trail[j] = UNKNOWN_LOCATION;
        }
    }

    // initialise dracula
    HV->players[PLAYER_DRACULA].health = GAME_START_BLOOD_POINTS;
    HV->players[PLAYER_DRACULA].ID = PLAYER_DRACULA;
    HV->players[PLAYER_DRACULA].curLocation = UNKNOWN_LOCATION;
    for (j = 0; j < TRAIL_SIZE; j++) {
        HV->players[PLAYER_DRACULA].trail[j] = UNKNOWN_LOCATION;
    }

    // initialise map
    for (i = 0; i < NUM_MAP_LOCATIONS; i++) {
        for (j = 0; j < NUM_MAP_LOCATIONS; j++) {
            HV->map[i][j] = NONE; 
        }
    }

    // Following code for addLink calls by Mohammad Ghasembeigi
    // https://www.openlearning.com/u/mohammadg/FuryOfDraculaMapConnections
    // ***START CODE FOR MAP CONNECTIONS*** //
    
    // ROAD Connections

    //ATHENS
    addLink(HV, ATHENS, VALONA, ROAD);

    //VALONA
    addLink(HV, VALONA, ATHENS, ROAD);
    addLink(HV, VALONA, SALONICA, ROAD);
    addLink(HV, VALONA, SOFIA, ROAD);
    addLink(HV, VALONA, SARAJEVO, ROAD);

    //SALONICA  
    addLink(HV, SALONICA, VALONA, ROAD);
    addLink(HV, SALONICA, SOFIA, ROAD);

    //SOFIA
    addLink(HV, SOFIA, VARNA, ROAD);
    addLink(HV, SOFIA, SALONICA, ROAD);
    addLink(HV, SOFIA, VALONA, ROAD);
    addLink(HV, SOFIA, SARAJEVO, ROAD);
    addLink(HV, SOFIA, BELGRADE, ROAD);
    addLink(HV, SOFIA, BUCHAREST, ROAD);

    //VARNA
    addLink(HV, VARNA, SOFIA, ROAD);
    addLink(HV, VARNA, CONSTANTA, ROAD);

    //CONSTANTA
    addLink(HV, CONSTANTA, VARNA, ROAD);
    addLink(HV, CONSTANTA, BUCHAREST, ROAD);
    addLink(HV, CONSTANTA, GALATZ, ROAD);

    //GALATZ
    addLink(HV, GALATZ, CONSTANTA, ROAD);
    addLink(HV, GALATZ, BUCHAREST, ROAD);
    addLink(HV, GALATZ, KLAUSENBURG, ROAD);
    addLink(HV, GALATZ, CASTLE_DRACULA, ROAD);

    //BUCHAREST
    addLink(HV, BUCHAREST, GALATZ, ROAD);
    addLink(HV, BUCHAREST, CONSTANTA, ROAD);
    addLink(HV, BUCHAREST, SOFIA, ROAD);
    addLink(HV, BUCHAREST, BELGRADE, ROAD);
    addLink(HV, BUCHAREST, KLAUSENBURG, ROAD);

    //BELGRADE
    addLink(HV, BELGRADE, SARAJEVO, ROAD);
    addLink(HV, BELGRADE, ST_JOSEPH_AND_ST_MARYS, ROAD);
    addLink(HV, BELGRADE, SZEGED, ROAD);
    addLink(HV, BELGRADE, KLAUSENBURG, ROAD);
    addLink(HV, BELGRADE, BUCHAREST, ROAD);
    addLink(HV, BELGRADE, SOFIA, ROAD);

    //SARAJEVO
    addLink(HV, SARAJEVO, VALONA, ROAD);
    addLink(HV, SARAJEVO, ZAGREB, ROAD);
    addLink(HV, SARAJEVO, ST_JOSEPH_AND_ST_MARYS, ROAD);
    addLink(HV, SARAJEVO, BELGRADE, ROAD);
    addLink(HV, SARAJEVO, SOFIA, ROAD);

    //ST_JOSEPH_AND_ST_MARYS
    addLink(HV, ST_JOSEPH_AND_ST_MARYS, SARAJEVO, ROAD);
    addLink(HV, ST_JOSEPH_AND_ST_MARYS, ZAGREB, ROAD);
    addLink(HV, ST_JOSEPH_AND_ST_MARYS, SZEGED, ROAD);
    addLink(HV, ST_JOSEPH_AND_ST_MARYS, BELGRADE, ROAD);

    //SZEGED
    addLink(HV, SZEGED, ST_JOSEPH_AND_ST_MARYS, ROAD);
    addLink(HV, SZEGED, ZAGREB, ROAD);
    addLink(HV, SZEGED, BUDAPEST, ROAD);
    addLink(HV, SZEGED, KLAUSENBURG, ROAD);
    addLink(HV, SZEGED, BELGRADE, ROAD);

    //KLAUSENBURG
    addLink(HV, KLAUSENBURG, BELGRADE, ROAD);
    addLink(HV, KLAUSENBURG, SZEGED, ROAD);
    addLink(HV, KLAUSENBURG, BUDAPEST, ROAD);
    addLink(HV, KLAUSENBURG, CASTLE_DRACULA, ROAD);
    addLink(HV, KLAUSENBURG, GALATZ, ROAD);
    addLink(HV, KLAUSENBURG, BUCHAREST, ROAD);

    //CASTLE_DRACULA
    addLink(HV, CASTLE_DRACULA, GALATZ, ROAD);
    addLink(HV, CASTLE_DRACULA, KLAUSENBURG, ROAD);

    //ZAGREB
    addLink(HV, ZAGREB, SARAJEVO, ROAD);
    addLink(HV, ZAGREB, MUNICH, ROAD);
    addLink(HV, ZAGREB, VIENNA, ROAD);
    addLink(HV, ZAGREB, BUDAPEST, ROAD);
    addLink(HV, ZAGREB, SZEGED, ROAD);
    addLink(HV, ZAGREB, ST_JOSEPH_AND_ST_MARYS, ROAD);

    //BUDAPEST
    addLink(HV, BUDAPEST, ZAGREB, ROAD);
    addLink(HV, BUDAPEST, VIENNA, ROAD);
    addLink(HV, BUDAPEST,KLAUSENBURG, ROAD);
    addLink(HV, BUDAPEST, SZEGED, ROAD);

    //VIENNA
    addLink(HV, VIENNA, ZAGREB, ROAD);
    addLink(HV, VIENNA, MUNICH, ROAD);
    addLink(HV, VIENNA, PRAGUE, ROAD);
    addLink(HV, VIENNA, BUDAPEST, ROAD);

    //PRAGUE 
    addLink(HV, PRAGUE, NUREMBURG, ROAD);
    addLink(HV, PRAGUE, BERLIN, ROAD);
    addLink(HV, PRAGUE, VIENNA, ROAD);
    
    //BERLIN
    addLink(HV, BERLIN, LEIPZIG, ROAD);
    addLink(HV, BERLIN, HAMBURG, ROAD);
    addLink(HV, BERLIN, PRAGUE, ROAD);

    //LEIPZIG
    addLink(HV, LEIPZIG, FRANKFURT, ROAD);
    addLink(HV, LEIPZIG, COLOGNE, ROAD);
    addLink(HV, LEIPZIG, HAMBURG, ROAD);
    addLink(HV, LEIPZIG, BERLIN, ROAD);
    addLink(HV, LEIPZIG, NUREMBURG, ROAD);

    //FRANKFURT
    addLink(HV, FRANKFURT, STRASBOURG, ROAD);
    addLink(HV, FRANKFURT, COLOGNE, ROAD);
    addLink(HV, FRANKFURT, LEIPZIG, ROAD);
    addLink(HV, FRANKFURT, NUREMBURG, ROAD);

    //NUREMBURG
    addLink(HV, NUREMBURG, STRASBOURG, ROAD);
    addLink(HV, NUREMBURG, FRANKFURT, ROAD);
    addLink(HV, NUREMBURG, LEIPZIG, ROAD);
    addLink(HV, NUREMBURG, PRAGUE, ROAD);
    addLink(HV, NUREMBURG, MUNICH, ROAD);

    //MUNICH
    addLink(HV, MUNICH, MILAN, ROAD);
    addLink(HV, MUNICH, ZURICH, ROAD);
    addLink(HV, MUNICH, STRASBOURG, ROAD);
    addLink(HV, MUNICH, NUREMBURG, ROAD);
    addLink(HV, MUNICH, VIENNA, ROAD);
    addLink(HV, MUNICH, ZAGREB, ROAD);
    addLink(HV, MUNICH, VENICE, ROAD);

    //VENICE
    addLink(HV, VENICE, FLORENCE, ROAD);
    addLink(HV, VENICE, GENOA, ROAD);
    addLink(HV, VENICE, MILAN, ROAD);
    addLink(HV, VENICE, MUNICH, ROAD);

    //FLORENCE
    addLink(HV, FLORENCE, GENOA, ROAD);
    addLink(HV, FLORENCE, VENICE, ROAD);
    addLink(HV, FLORENCE, ROME, ROAD);

    //ROME
    addLink(HV, ROME, FLORENCE, ROAD);
    addLink(HV, ROME, NAPLES, ROAD);
    addLink(HV, ROME, BARI, ROAD);

    //NAPLES
    addLink(HV, NAPLES, ROME, ROAD);
    addLink(HV, NAPLES, BARI, ROAD);

    //BARI
    addLink(HV, BARI, NAPLES, ROAD);
    addLink(HV, BARI, ROME, ROAD);

    //GENOA
    addLink(HV, GENOA, VENICE, ROAD);
    addLink(HV, GENOA, FLORENCE, ROAD);
    addLink(HV, GENOA, MILAN, ROAD);
    addLink(HV, GENOA, MARSEILLES, ROAD);

    //MILAN
    addLink(HV, MILAN, GENOA, ROAD);
    addLink(HV, MILAN, VENICE, ROAD);
    addLink(HV, MILAN, MARSEILLES, ROAD);
    addLink(HV, MILAN, ZURICH, ROAD);
    addLink(HV, MILAN, MUNICH, ROAD);

    //ZURICH
    addLink(HV, ZURICH, MILAN, ROAD);
    addLink(HV, ZURICH, MUNICH, ROAD);
    addLink(HV, ZURICH, GENEVA, ROAD);
    addLink(HV, ZURICH, MARSEILLES, ROAD);
    addLink(HV, ZURICH, STRASBOURG, ROAD);

    //STRASBOURG
    addLink(HV, STRASBOURG, ZURICH, ROAD);
    addLink(HV, STRASBOURG, MUNICH, ROAD);
    addLink(HV, STRASBOURG, GENEVA, ROAD);
    addLink(HV, STRASBOURG, NUREMBURG, ROAD);
    addLink(HV, STRASBOURG, FRANKFURT, ROAD);
    addLink(HV, STRASBOURG, COLOGNE, ROAD);
    addLink(HV, STRASBOURG, BRUSSELS, ROAD);
    addLink(HV, STRASBOURG, PARIS, ROAD);

    //COLOGNE
    addLink(HV, COLOGNE, STRASBOURG, ROAD);
    addLink(HV, COLOGNE, BRUSSELS, ROAD);
    addLink(HV, COLOGNE, FRANKFURT, ROAD);
    addLink(HV, COLOGNE, HAMBURG, ROAD);
    addLink(HV, COLOGNE, AMSTERDAM, ROAD);
    addLink(HV, COLOGNE, LEIPZIG, ROAD);

    //HAMBURG
    addLink(HV, HAMBURG, COLOGNE, ROAD);
    addLink(HV, HAMBURG, BERLIN, ROAD);
    addLink(HV, HAMBURG, LEIPZIG, ROAD);

    //AMSTERDAM
    addLink(HV, AMSTERDAM, COLOGNE, ROAD);
    addLink(HV, AMSTERDAM, BRUSSELS, ROAD);

    //BRUSSELS
    addLink(HV, BRUSSELS, AMSTERDAM, ROAD);
    addLink(HV, BRUSSELS, COLOGNE, ROAD);
    addLink(HV, BRUSSELS, STRASBOURG, ROAD);
    addLink(HV, BRUSSELS, PARIS, ROAD);
    addLink(HV, BRUSSELS, LE_HAVRE, ROAD);

    //GENEVA
    addLink(HV, GENEVA, ZURICH, ROAD);
    addLink(HV, GENEVA, STRASBOURG, ROAD);
    addLink(HV, GENEVA, PARIS, ROAD);
    addLink(HV, GENEVA, CLERMONT_FERRAND, ROAD);
    addLink(HV, GENEVA, MARSEILLES, ROAD);

    //MARSEILLES
    addLink(HV, MARSEILLES, GENEVA, ROAD);
    addLink(HV, MARSEILLES, ZURICH, ROAD);
    addLink(HV, MARSEILLES, MILAN, ROAD);
    addLink(HV, MARSEILLES, GENOA, ROAD);
    addLink(HV, MARSEILLES, CLERMONT_FERRAND, ROAD);
    addLink(HV, MARSEILLES, TOULOUSE, ROAD);

    //CLERMONT_FERRAND
    addLink(HV, CLERMONT_FERRAND, MARSEILLES, ROAD);
    addLink(HV, CLERMONT_FERRAND, GENEVA, ROAD);
    addLink(HV, CLERMONT_FERRAND, TOULOUSE, ROAD);
    addLink(HV, CLERMONT_FERRAND, PARIS, ROAD);
    addLink(HV, CLERMONT_FERRAND, BORDEAUX, ROAD);
    addLink(HV, CLERMONT_FERRAND, NANTES, ROAD);

    //PARIS
    addLink(HV, PARIS, STRASBOURG, ROAD);
    addLink(HV, PARIS, GENEVA, ROAD);
    addLink(HV, PARIS, BRUSSELS, ROAD);
    addLink(HV, PARIS, LE_HAVRE, ROAD);
    addLink(HV, PARIS, NANTES, ROAD);
    addLink(HV, PARIS, CLERMONT_FERRAND, ROAD);

    //LE_HAVRE
    addLink(HV, LE_HAVRE, PARIS, ROAD);
    addLink(HV, LE_HAVRE, BRUSSELS, ROAD);
    addLink(HV, LE_HAVRE, NANTES, ROAD);

    //NANTES
    addLink(HV, NANTES, LE_HAVRE, ROAD);
    addLink(HV, NANTES, PARIS, ROAD);
    addLink(HV, NANTES, BORDEAUX, ROAD);
    addLink(HV, NANTES, CLERMONT_FERRAND, ROAD);

    //BORDEAUX
    addLink(HV, BORDEAUX, NANTES, ROAD);
    addLink(HV, BORDEAUX, CLERMONT_FERRAND, ROAD);
    addLink(HV, BORDEAUX, TOULOUSE, ROAD);
    addLink(HV, BORDEAUX, SARAGOSSA, ROAD);

    //TOULOUSE
    addLink(HV, TOULOUSE, BORDEAUX, ROAD);
    addLink(HV, TOULOUSE, CLERMONT_FERRAND, ROAD);
    addLink(HV, TOULOUSE, MARSEILLES, ROAD);
    addLink(HV, TOULOUSE, BARCELONA, ROAD);
    addLink(HV, TOULOUSE, SARAGOSSA, ROAD);

    //SARAGOSSA
    addLink(HV, SARAGOSSA, TOULOUSE, ROAD);
    addLink(HV, SARAGOSSA, BARCELONA, ROAD);
    addLink(HV, SARAGOSSA, BORDEAUX, ROAD);
    addLink(HV, SARAGOSSA, ALICANTE, ROAD);
    addLink(HV, SARAGOSSA, MADRID, ROAD);
    addLink(HV, SARAGOSSA, SANTANDER, ROAD);

    //BARCELONA
    addLink(HV, BARCELONA, SARAGOSSA, ROAD);
    addLink(HV, BARCELONA, TOULOUSE, ROAD);

    //ALICANTE
    addLink(HV, ALICANTE, SARAGOSSA, ROAD);
    addLink(HV, ALICANTE, GRANADA, ROAD);
    addLink(HV, ALICANTE, MADRID, ROAD);

    //GRANADA
    addLink(HV, GRANADA, ALICANTE, ROAD);
    addLink(HV, GRANADA, MADRID, ROAD);
    addLink(HV, GRANADA, CADIZ, ROAD);

    //CADIZ
    addLink(HV, CADIZ, GRANADA, ROAD);
    addLink(HV, CADIZ, MADRID, ROAD);
    addLink(HV, CADIZ, LISBON, ROAD);

    //LISBON
    addLink(HV, LISBON, CADIZ, ROAD);
    addLink(HV, LISBON, MADRID, ROAD);
    addLink(HV, LISBON, SANTANDER, ROAD);

    //MADRID
    addLink(HV, MADRID, LISBON, ROAD);
    addLink(HV, MADRID, CADIZ, ROAD);
    addLink(HV, MADRID, GRANADA, ROAD);
    addLink(HV, MADRID, ALICANTE, ROAD);
    addLink(HV, MADRID, SANTANDER, ROAD);
    addLink(HV, MADRID, SARAGOSSA, ROAD);

    //SANTANDER
    addLink(HV, SANTANDER, MADRID, ROAD);
    addLink(HV, SANTANDER, LISBON, ROAD);
    addLink(HV, SANTANDER, SARAGOSSA, ROAD);

    //LONDON
    addLink(HV, LONDON, PLYMOUTH, ROAD);
    addLink(HV, LONDON, SWANSEA, ROAD);
    addLink(HV, LONDON, MANCHESTER, ROAD);

    //PLYMOUTH
    addLink(HV, PLYMOUTH, LONDON, ROAD);

    //SWANSEA
    addLink(HV, SWANSEA, LONDON, ROAD);
    addLink(HV, SWANSEA, LIVERPOOL, ROAD);

    //MANCHESTER
    addLink(HV, MANCHESTER, LIVERPOOL, ROAD);
    addLink(HV, MANCHESTER, EDINBURGH, ROAD);
    addLink(HV, MANCHESTER, LONDON, ROAD);

    //LIVERPOOL
    addLink(HV, LIVERPOOL, SWANSEA, ROAD);
    addLink(HV, LIVERPOOL, MANCHESTER, ROAD); // corrected from addLink(HV, LIVERPOOL, EDINBURGH, ROAD);

    //DUBLIN
    addLink(HV, DUBLIN, GALWAY, ROAD);

    //GALWAY
    addLink(HV, GALWAY, DUBLIN, ROAD);

    //EDINBURGH
    addLink(HV, EDINBURGH, MANCHESTER, ROAD);

    //### RAIL Connections ###

    //ALICANTE
    addLink(HV, ALICANTE, MADRID, RAIL);
    addLink(HV, ALICANTE, BARCELONA, RAIL);

    //MADRID
    addLink(HV, MADRID, ALICANTE, RAIL);
    addLink(HV, MADRID, LISBON, RAIL);
    addLink(HV, MADRID, SANTANDER, RAIL);
    addLink(HV, MADRID, SARAGOSSA, RAIL);

    //BARCELONA
    addLink(HV, BARCELONA, ALICANTE, RAIL);
    addLink(HV, BARCELONA, SARAGOSSA, RAIL);

    //LISBON
    addLink(HV, LISBON, MADRID, RAIL);


    //SANTANDER
    addLink(HV, SANTANDER, MADRID, RAIL);

    //SARAGOSSA
    addLink(HV, SARAGOSSA, MADRID, RAIL);
    addLink(HV, SARAGOSSA, BARCELONA, RAIL);
    addLink(HV, SARAGOSSA, BORDEAUX, RAIL);

    //BORDEAUX
    addLink(HV, BORDEAUX, SARAGOSSA, RAIL);
    addLink(HV, BORDEAUX, PARIS, RAIL);

    //LE_HAVRE
    addLink(HV, LE_HAVRE, PARIS, RAIL);
    
    //MARSEILLES
    addLink(HV, MARSEILLES, PARIS, RAIL);
    
    //PARIS
    addLink(HV, PARIS, BORDEAUX, RAIL);
    addLink(HV, PARIS, MARSEILLES, RAIL);
    addLink(HV, PARIS, LE_HAVRE, RAIL);
    addLink(HV, PARIS, BRUSSELS, RAIL);


    //BRUSSELS
    addLink(HV, BRUSSELS, COLOGNE, RAIL);
    addLink(HV, BRUSSELS, PARIS, RAIL);

    //COLOGNE
    addLink(HV, COLOGNE, BRUSSELS, RAIL);
    addLink(HV, COLOGNE, FRANKFURT, RAIL);

    //FRANKFURT
    addLink(HV, FRANKFURT, STRASBOURG, RAIL);
    addLink(HV, FRANKFURT, LEIPZIG, RAIL);
    addLink(HV, FRANKFURT, COLOGNE, RAIL);

    //STRASBOURG
    addLink(HV, STRASBOURG, FRANKFURT, RAIL);
    addLink(HV, STRASBOURG, ZURICH, RAIL);

    //ZURICH
    addLink(HV, ZURICH, MILAN, RAIL);
    addLink(HV, ZURICH, STRASBOURG, RAIL);

    //GENEVA
    addLink(HV, GENEVA, MILAN, RAIL);
    
    //GEONA
    addLink(HV, GENOA, MILAN, RAIL);
    
    //MILAN
    addLink(HV, MILAN, GENEVA, RAIL);
    addLink(HV, MILAN, FLORENCE, RAIL);
    addLink(HV, MILAN, ZURICH, RAIL);
    addLink(HV, MILAN, GENOA, RAIL);

    //FLORENCE
    addLink(HV, FLORENCE, ROME, RAIL);
    addLink(HV, FLORENCE, MILAN, RAIL);

    //ROME
    addLink(HV, ROME, FLORENCE, RAIL);
    addLink(HV, ROME, NAPLES, RAIL);

    //NAPLES
    addLink(HV, NAPLES, ROME, RAIL);
    addLink(HV, NAPLES, BARI, RAIL);

    //BARI
    addLink(HV, BARI, NAPLES, RAIL);

    //LEIPZIG
    addLink(HV, LEIPZIG, FRANKFURT, RAIL);
    addLink(HV, LEIPZIG, NUREMBURG, RAIL);
    addLink(HV, LEIPZIG, BERLIN, RAIL);

    //BERLIN
    addLink(HV, BERLIN, HAMBURG, RAIL);
    addLink(HV, BERLIN, PRAGUE, RAIL);
    addLink(HV, BERLIN, LEIPZIG, RAIL);

    //HAMBURG
    addLink(HV, HAMBURG, BERLIN, RAIL);
    
    //MUNICH
    addLink(HV, MUNICH, NUREMBURG, RAIL);
    
    //NUREMBURG
    addLink(HV, NUREMBURG, MUNICH, RAIL);
    addLink(HV, NUREMBURG, LEIPZIG, RAIL);

    //PRAGUE
    addLink(HV, PRAGUE, BERLIN, RAIL);
    addLink(HV, PRAGUE, VIENNA, RAIL);

    //VIENNA
    addLink(HV, VIENNA, PRAGUE, RAIL);
    addLink(HV, VIENNA, VENICE, RAIL);
    addLink(HV, VIENNA, BUDAPEST, RAIL);

    //VENICE
    addLink(HV, VENICE, VIENNA, RAIL);

    //BUDAPEST
    addLink(HV, BUDAPEST, VIENNA, RAIL);
    addLink(HV, BUDAPEST, SZEGED, RAIL);

    //SZEGED
    addLink(HV, SZEGED, BUDAPEST, RAIL);
    addLink(HV, SZEGED, BELGRADE, RAIL);
    addLink(HV, SZEGED, BUCHAREST, RAIL);

    //BELGRADE
    addLink(HV, BELGRADE, SZEGED, RAIL);
    addLink(HV, BELGRADE, SOFIA, RAIL);


    //SOFIA
    addLink(HV, SOFIA, BELGRADE, RAIL);
    addLink(HV, SOFIA, SALONICA, RAIL);
    addLink(HV, SOFIA, VARNA, RAIL);

    //SALONICA
    addLink(HV, SALONICA, SOFIA, RAIL);

    //VARNA
    addLink(HV, VARNA, SOFIA, RAIL);

    //BUCHAREST
    addLink(HV, BUCHAREST, SZEGED, RAIL);
    addLink(HV, BUCHAREST, GALATZ, RAIL);
    addLink(HV, BUCHAREST, CONSTANTA, RAIL);

    //GALATZ
    addLink(HV, GALATZ, BUCHAREST, RAIL);

    //CONSTANTA
    addLink(HV, CONSTANTA, BUCHAREST, RAIL);

    //LONDON
    addLink(HV, LONDON, SWANSEA, RAIL);
    addLink(HV, LONDON, MANCHESTER, RAIL);

    //SWANSEA
    addLink(HV, SWANSEA, LONDON, RAIL);

    //MANCHESTER
    addLink(HV, MANCHESTER, LONDON, RAIL);
    addLink(HV, MANCHESTER, LIVERPOOL, RAIL);
    addLink(HV, MANCHESTER, EDINBURGH, RAIL);

    //EDINBURGH
    addLink(HV, EDINBURGH, MANCHESTER, RAIL);
    
    //LIVERPOOL
    addLink(HV, LIVERPOOL, MANCHESTER, RAIL);
    
    //### SEA Connections ###

    //BLACK SEA
    addLink(HV, BLACK_SEA, IONIAN_SEA, SEA);
    addLink(HV, BLACK_SEA, VARNA, SEA);
    addLink(HV, BLACK_SEA, CONSTANTA, SEA);

    //IONIAN SEA
    addLink(HV, IONIAN_SEA, BLACK_SEA, SEA);
    addLink(HV, IONIAN_SEA, VALONA, SEA);
    addLink(HV, IONIAN_SEA, ATHENS, SEA);
    addLink(HV, IONIAN_SEA, SALONICA, SEA);
    addLink(HV, IONIAN_SEA, ADRIATIC_SEA, SEA);
    addLink(HV, IONIAN_SEA, TYRRHENIAN_SEA, SEA);

    //ADRIATIC SEA
    addLink(HV, ADRIATIC_SEA, VENICE, SEA);
    addLink(HV, ADRIATIC_SEA, BARI, SEA);
    addLink(HV, ADRIATIC_SEA, IONIAN_SEA, SEA);

    //TYRRHENIAN SEA
    addLink(HV, TYRRHENIAN_SEA, MEDITERRANEAN_SEA, SEA);
    addLink(HV, TYRRHENIAN_SEA, CAGLIARI, SEA);
    addLink(HV, TYRRHENIAN_SEA, GENOA, SEA);
    addLink(HV, TYRRHENIAN_SEA, ROME, SEA);
    addLink(HV, TYRRHENIAN_SEA, NAPLES, SEA);
    addLink(HV, TYRRHENIAN_SEA, IONIAN_SEA, SEA);

    //MEDITERRANEAN_SEA
    addLink(HV, MEDITERRANEAN_SEA, ATLANTIC_OCEAN, SEA);
    addLink(HV, MEDITERRANEAN_SEA, TYRRHENIAN_SEA, SEA);
    addLink(HV, MEDITERRANEAN_SEA, CAGLIARI, SEA);
    addLink(HV, MEDITERRANEAN_SEA, MARSEILLES, SEA);
    addLink(HV, MEDITERRANEAN_SEA, BARCELONA, SEA);
    addLink(HV, MEDITERRANEAN_SEA, ALICANTE, SEA);

    //BAY_OF_BISCAY
    addLink(HV, BAY_OF_BISCAY, ATLANTIC_OCEAN, SEA);
    addLink(HV, BAY_OF_BISCAY, BORDEAUX, SEA);
    addLink(HV, BAY_OF_BISCAY, SANTANDER, SEA);
    addLink(HV, BAY_OF_BISCAY, NANTES, SEA);

    //ATLANTIC_OCEAN
    addLink(HV, ATLANTIC_OCEAN, NORTH_SEA, SEA);
    addLink(HV, ATLANTIC_OCEAN, BAY_OF_BISCAY, SEA);
    addLink(HV, ATLANTIC_OCEAN, IRISH_SEA, SEA);
    addLink(HV, ATLANTIC_OCEAN, ENGLISH_CHANNEL, SEA);
    addLink(HV, ATLANTIC_OCEAN, MEDITERRANEAN_SEA, SEA);
    addLink(HV, ATLANTIC_OCEAN, CADIZ, SEA);
    addLink(HV, ATLANTIC_OCEAN, LISBON, SEA);
    addLink(HV, ATLANTIC_OCEAN, GALWAY, SEA);

    //ENGLISH_CHANNEL
    addLink(HV, ENGLISH_CHANNEL, ATLANTIC_OCEAN, SEA);
    addLink(HV, ENGLISH_CHANNEL, NORTH_SEA, SEA);
    addLink(HV, ENGLISH_CHANNEL, LE_HAVRE, SEA);
    addLink(HV, ENGLISH_CHANNEL, PLYMOUTH, SEA);
    addLink(HV, ENGLISH_CHANNEL, LONDON, SEA);

    //IRISH_SEA
    addLink(HV, IRISH_SEA, ATLANTIC_OCEAN, SEA);
    addLink(HV, IRISH_SEA, LIVERPOOL, SEA);
    addLink(HV, IRISH_SEA, SWANSEA, SEA);
    addLink(HV, IRISH_SEA, DUBLIN, SEA);

    //NORTH_SEA
    addLink(HV, NORTH_SEA, ATLANTIC_OCEAN, SEA);
    addLink(HV, NORTH_SEA, ENGLISH_CHANNEL, SEA);
    addLink(HV, NORTH_SEA, EDINBURGH, SEA);
    addLink(HV, NORTH_SEA, AMSTERDAM, SEA);
    addLink(HV, NORTH_SEA, HAMBURG, SEA);

    //### SEA->CITIES Connections ###

    addLink(HV, CONSTANTA, BLACK_SEA, SEA);
    addLink(HV, VARNA, BLACK_SEA, SEA);
    addLink(HV, SALONICA, IONIAN_SEA, SEA);
    addLink(HV, ATHENS, IONIAN_SEA, SEA);
    addLink(HV, VALONA, IONIAN_SEA, SEA);
    addLink(HV, BARI, ADRIATIC_SEA, SEA);
    addLink(HV, VENICE, ADRIATIC_SEA, SEA);
    addLink(HV, NAPLES, TYRRHENIAN_SEA, SEA);
    addLink(HV, ROME, TYRRHENIAN_SEA, SEA);
    addLink(HV, GENOA, TYRRHENIAN_SEA, SEA);
    addLink(HV, CAGLIARI, TYRRHENIAN_SEA, SEA);
    addLink(HV, CAGLIARI, MEDITERRANEAN_SEA, SEA);
    addLink(HV, MARSEILLES, MEDITERRANEAN_SEA, SEA);
    addLink(HV, BARCELONA, MEDITERRANEAN_SEA, SEA);
    addLink(HV, ALICANTE, MEDITERRANEAN_SEA, SEA);
    addLink(HV, CADIZ, ATLANTIC_OCEAN, SEA);
    addLink(HV, LISBON, ATLANTIC_OCEAN, SEA);
    addLink(HV, GALWAY, ATLANTIC_OCEAN, SEA);
    addLink(HV, SANTANDER, BAY_OF_BISCAY, SEA);
    addLink(HV, BORDEAUX, BAY_OF_BISCAY, SEA);
    addLink(HV, NANTES, BAY_OF_BISCAY, SEA);
    addLink(HV, LONDON, ENGLISH_CHANNEL, SEA);
    addLink(HV, PLYMOUTH, ENGLISH_CHANNEL, SEA);
    addLink(HV, LE_HAVRE, ENGLISH_CHANNEL, SEA);
    addLink(HV, SWANSEA, IRISH_SEA, SEA);
    addLink(HV, LIVERPOOL, IRISH_SEA, SEA);
    addLink(HV, DUBLIN, IRISH_SEA, SEA);
    addLink(HV, EDINBURGH, NORTH_SEA, SEA);
    addLink(HV, AMSTERDAM, NORTH_SEA, SEA);
    addLink(HV, HAMBURG, NORTH_SEA, SEA);

    // ***END CODE FOR MAP CONNECTIONS*** //

    
    /* // Our old connection code
    // From Top Left
    // Island 1
    addLink(HV, GALWAY, DUBLIN, ROAD);
    addLink(HV, EDINBURGH, MANCHESTER, BOTH);

    // Island 2
    addLink(HV, MANCHESTER, LIVERPOOL, BOTH);
    addLink(HV, MANCHESTER, LONDON, BOTH);
    addLink(HV, LIVERPOOL, SWANSEA, ROAD);
    addLink(HV, SWANSEA, LONDON, BOTH);
    addLink(HV, LONDON, PLYMOUTH, ROAD);

    // Madrid
    addLink(HV, MADRID, LISBON, BOTH);
    addLink(HV, MADRID, CADIZ, ROAD);
    addLink(HV, MADRID, GRANADA, ROAD);
    addLink(HV, MADRID, ALICANTE, BOTH);
    addLink(HV, MADRID, SARAGOSSA, BOTH);
    addLink(HV, MADRID, SANTANDER, BOTH);

    // Circle Around Madrid
    addLink(HV, LISBON, CADIZ, ROAD);
    addLink(HV, CADIZ, GRANADA, ROAD);
    addLink(HV, GRANADA, ALICANTE, ROAD);
    addLink(HV, ALICANTE, SARAGOSSA, ROAD);
    addLink(HV, SARAGOSSA, SANTANDER, ROAD);
    addLink(HV, SANTANDER, LISBON, ROAD);

    // Converge and connect Bordaux, Clermont Ferrand and Marseilles
    addLink(HV, ALICANTE, BARCELONA, BOTH);
    addLink(HV, SARAGOSSA, BARCELONA, BOTH);
    addLink(HV, BARCELONA, TOULOUSE, ROAD);
    addLink(HV, SARAGOSSA, TOULOUSE, ROAD);
    addLink(HV, SARAGOSSA, BORDEAUX, BOTH);
    addLink(HV, TOULOUSE, BORDEAUX, ROAD);
    addLink(HV, TOULOUSE, MARSEILLES, ROAD);
    addLink(HV, TOULOUSE, CLERMONT_FERRAND, ROAD);
    addLink(HV, CLERMONT_FERRAND, BORDEAUX, ROAD);
    addLink(HV, CLERMONT_FERRAND, MARSEILLES, ROAD);
    */
}

void addLink(HunterView HV, LocationID L1, LocationID L2, char type) {
    char existingLink = HV->map[L1][L2];
    if (existingLink == NONE) {
        HV->map[L1][L2] = type;
    } else if ((type == ROAD) && (existingLink == RAIL)) {
        HV->map[L1][L2] = BOTH;
    } else if ((type == RAIL) && (existingLink == ROAD)) {
        HV->map[L1][L2] = BOTH;
    } else {
        // this assert will verify our addLink calls are correct and without duplicates
        assert(FALSE);
    }
}

// Parser - parses pastPlays into the hunterView form
HunterView newHunterView(char *pastPlays, playerMessage messages[]) {
    int i;
    HunterView HV = malloc(sizeof(struct hunterView));
    initialiseHunterView(HV);
    HV->pastPlays = pastPlays;

    char players[] = {'G', 'S', 'H', 'M', 'D'};

    int stringIndex = 0;
    PlayerID curPlayerID = -1;
    // int vampireOnMap = FALSE; // no longer used
    int teleportedThisTurn;
    int teleportedLastTurn[NUM_PLAYERS-1]; // used in the hunter's turn if block
    for (i = 0; i < NUM_PLAYERS-1; i++) {
        teleportedLastTurn[i] = FALSE;
    }
    
    char moveLocation[3];
    int moveLocationID;
    moveLocation[2] = '\0';
    while (1) {
        // since not sure yet if last move has space
        // note: for the first move pastPlays[stringIndex - 1] won't work, hence extra condition
        if (pastPlays[stringIndex] == '\0' || (stringIndex > 0 && pastPlays[stringIndex - 1] == '\0')) {
            break;
        }

        // Find who's turn it is
        for (i = 0; i < NUM_PLAYERS; i++) {
            if (pastPlays[stringIndex] == players[i]) {
                curPlayerID = i;
            }
        }

        moveLocation[0] = pastPlays[stringIndex+1];
        moveLocation[1] = pastPlays[stringIndex+2];
        moveLocationID = 0;
        while (strcmp(locToStr(moveLocationID), moveLocation)) {
            moveLocationID++;
        }
        
        for (i = TRAIL_SIZE-2; i >= 0; i--) {
            HV->players[curPlayerID].trail[i+1] = HV->players[curPlayerID].trail[i];
        }
        HV->players[curPlayerID].trail[0] = moveLocationID;
        HV->players[curPlayerID].curLocation = moveLocationID;

        // What you have now is the player who's turn it is as curPlayerID
        // along with their move location number (for the matrix) in moveLocationID
        // all you have to do is handle what happens to the players during the move
        // (editing the data in their struct to simulate their turn)

        if (curPlayerID != PLAYER_DRACULA) {
            // Hunter Move
            teleportedThisTurn = FALSE;
            for (i = stringIndex+3; i < stringIndex+7; i++) {
                if (pastPlays[i] == 'T') { // trap encountered
                    HV->players[curPlayerID].health -= LIFE_LOSS_TRAP_ENCOUNTER;
                /*} else if (pastPlays[i] == 'V') { // vampire encountered
                    vampireOnMap = FALSE;*/ // redundant
                } else if (pastPlays[i] == 'D') { // dracula confronted
                    HV->players[curPlayerID].health -= LIFE_LOSS_DRACULA_ENCOUNTER;
                    HV->players[PLAYER_DRACULA].health -= LIFE_LOSS_HUNTER_ENCOUNTER;
                }
                
                if (HV->players[curPlayerID].health <= 0) {
                    // if this has happened that means nothing will happen for the rest of the turn
                    HV->players[curPlayerID].curLocation = ST_JOSEPH_AND_ST_MARYS;
                    HV->players[curPlayerID].health = GAME_START_HUNTER_LIFE_POINTS;
                    HV->score -= SCORE_LOSS_HUNTER_HOSPITAL;
                    teleportedThisTurn = TRUE;
                }
            }
            // following if statement for dealing with REST may be incorrect
            // revise if any tests fail related to hunter resting
            if (HV->players[curPlayerID].curLocation == HV->players[curPlayerID].trail[1]
                    && !teleportedLastTurn[curPlayerID]) {
                HV->players[curPlayerID].health += LIFE_GAIN_REST;
                if (HV->players[curPlayerID].health > GAME_START_HUNTER_LIFE_POINTS) {
                    HV->players[curPlayerID].health = GAME_START_HUNTER_LIFE_POINTS;
                }
            }
            teleportedLastTurn[curPlayerID] = teleportedThisTurn;

        } else {
            // Dracula Move
            if (pastPlays[stringIndex+5] == 'V') {
                HV->score -= SCORE_LOSS_VAMPIRE_MATURES;
            }
            
            if (draculaAtSea(HV)) {
                HV->players[curPlayerID].health -= LIFE_LOSS_SEA;
            } else if (HV->players[curPlayerID].curLocation == CASTLE_DRACULA
                    || HV->players[curPlayerID].curLocation == TELEPORT) {
                HV->players[curPlayerID].health += LIFE_GAIN_CASTLE_DRACULA;
            }
            
            HV->score -= SCORE_LOSS_DRACULA_TURN;
            HV->curRound++;
        }
        stringIndex+=8; // skipping space that follows each 7-char move
    }

    // set current player id in HV
    curPlayerID++;
    if (curPlayerID == NUM_PLAYERS){
        curPlayerID = 0;
    }
    HV->currentPlayer = curPlayerID;

    return HV;
}

// returns TRUE/FALSE
int draculaAtSea(HunterView currentView) {
    if (isSeaLocation(currentView->players[PLAYER_DRACULA].curLocation)) {
        return TRUE;
    }
    
    if (currentView->players[PLAYER_DRACULA].curLocation >= DOUBLE_BACK_1
            && currentView->players[PLAYER_DRACULA].curLocation <= DOUBLE_BACK_5) {
        int doubleBackSteps = currentView->players[PLAYER_DRACULA].curLocation - (DOUBLE_BACK_1 - 1);
        if (isSeaLocation(currentView->players[PLAYER_DRACULA].trail[doubleBackSteps])) {
            return TRUE;
        }
    }
    
    return FALSE;
}

// returns TRUE/FALSE
int isSeaLocation(LocationID location) {
    if (location == SEA_UNKNOWN || (location >= NORTH_SEA && location <= BLACK_SEA)) {
        return TRUE;
    }
    return FALSE;
}

// this function frees all memory previously allocated for the HunterView
// toBeDeleted. toBeDeleted should not be accessed after the call.
void disposeHunterView(HunterView toBeDeleted) {
    free(toBeDeleted);
}


Round getRound (HunterView currentView) {
    return currentView->curRound;
}


PlayerID getCurrentPlayer (HunterView currentView) {
    return currentView->currentPlayer;
}


int getScore(HunterView currentView) {
    return currentView->score;
}


int getHealth(HunterView currentView, PlayerID player) {
    return currentView->players[player].health;
}


LocationID getLocation(HunterView currentView, PlayerID player) {
    return currentView->players[player].curLocation;
}


void getHistory(HunterView currentView, PlayerID player, LocationID trail[TRAIL_SIZE]) {
    int i;
    for (i = 0; i < TRAIL_SIZE; i++) {
        trail[i] = currentView->players[player].trail[i];
    }
}

// road rail and sea are flags, TRUE or FALSE (const)
LocationID * connectedLocations(HunterView currentView, int * numLocations, LocationID from, 
                              PlayerID player, Round round, int road, int rail, int sea) {
	assert(from >= 0);
    int i, j;
    int isConnected[NUM_MAP_LOCATIONS];
    int numIsConnected = 0;
    int railTravelLength = (round + player) % 4;
    
    for (i = 0; i < NUM_MAP_LOCATIONS; i++) {
        // long if statement which essentially says:
        // if want road, and connected by road,
        // or want rail, and allowed to use rail, and connected by rail,
        // or want sea, and connected by sea.
        if ((road && (currentView->map[from][i] == ROAD || currentView->map[from][i] == BOTH))
                || (rail && player != PLAYER_DRACULA && railTravelLength > 0 &&
                     (currentView->map[from][i] == RAIL || currentView->map[from][i] == BOTH))
                || (sea && currentView->map[from][i] == SEA)) {
            isConnected[i] = TRUE;
            numIsConnected++;
        } else {
            isConnected[i] = FALSE;
        }
    }
    
    // adds "from" here to avoid being overwritten by above code
    // doing this saves putting extra checks in above code
    if (!isConnected[from]) {
        isConnected[from] = TRUE;
        numIsConnected++;
    }
    
    // BFS for finding rail travel connections
    // if railTravelLength is 1, already checked above to avoid unnecessary BFS
    if (rail && player != PLAYER_DRACULA && railTravelLength > 1) {
        Queue q = newQueue();
        int distance[NUM_MAP_LOCATIONS];
        for (i = 0; i < NUM_MAP_LOCATIONS; i++) {
            distance[i] = -1;
        }
        distance[from] = 0;
        QueueJoin(q, from);
        while (!QueueIsEmpty(q)) {
            LocationID l = QueueLeave(q);
            if (distance[l] >= railTravelLength) {
                break; // already searched deep enough
            }
            for (i = 0; i < NUM_MAP_LOCATIONS; i++) {
                if (distance[i] == -1 && (currentView->map[l][i] == RAIL || currentView->map[l][i] == BOTH)) {
                    distance[i] = 1 + distance[l];
                    QueueJoin(q, i);
                }
            }
        }
        dropQueue(q);
        for (i = 0; i < NUM_MAP_LOCATIONS; i++) {
            if (distance[i] >= 0 && distance[i] <= railTravelLength && !isConnected[i]) {
                isConnected[i] = TRUE;
                numIsConnected++;
            }
        }
    }
    
    // Copy any reached location into new array to return
    LocationID * connections = malloc(sizeof(LocationID) * numIsConnected);
    *numLocations = numIsConnected;
    i = 0;
    for (j = 0; j < NUM_MAP_LOCATIONS; j++) {
        if (isConnected[j]) {
            connections[i] = j;
            i++;
        }
    }
    assert(i == numIsConnected); // will fail if numIsConnected was miscalculated
    return connections;
}