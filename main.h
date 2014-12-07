#include <string>
#include <sstream>
#include <stdlib.h>

#include "simlib.h"

typedef struct 
{
    const char *NazovTrasy;
    int N_KILOMETRY;                          // celkem kilometru na trase
    int *KOMORY;              // na jakych kilometrech jsou komory
    int N_KOMORY;  // pocet komor
    Facility *Komory;                      // komory
    Facility *Kilometry;                 // kilometry
    int *TUNELY;
    int N_TUNELY;
    Facility *Tunely;
    int Direction;
} Trasa;

