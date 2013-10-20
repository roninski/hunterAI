#include <stdlib.h>
#include <assert.h>
#include "cities.h"

// this has NUM_LOCATIONS elements in it
static char * cities[] = { "AL", "AM", "AT", "BA", "BI", "BE", "BR", "BO", "BU", "BC", 
                           "BD", "CA", "CG", "CD", "CF", "CO", "CN", "DU", "ED", "FL", 
                           "FR", "GA", "GW", "GE", "GO", "GR", "HA", "JM", "KL", "LE", 
                           "LI", "LS", "LV", "LO", "MA", "MN", "MR", "MI", "MU", "NA",
                           "NP", "NU", "PA", "PL", "PR", "RO", "SA", "SN", "SR", "SJ",
                           "SO", "ST", "SW", "SZ", "TO", "VA", "VR", "VE", "VI", "ZA",
                           "ZU",
                           "NS", "EC", "IS", "AO", "BB", "MS", "TS", "IO", "AS", "BS",
                           "C?", "S?", "HI", "D1", "D2", "D3", "D4", "D5", "TP"};

char *locToStr(LocationID location) {
	assert(location >= 0 && location < NUM_LOCATIONS);
	return cities[location];
}