#include <string>
#include <sstream>
#include <stdlib.h>

#include "simlib.h"
#include "main.h"
#include "lod.h"

class Generator : public Event {
    int TrasaId;
    void Behavior() 
    {
        (new Lod)->init(TrasaId);
        Activate(Time + Exponential(T_VYPLUTI_LODI));
    }
public:
    void init(int TrasaId)
    {
        TrasaId = TrasaId;
        Activate();
    }
};

int main(int argc, char *argv[]) {
    if (argc == 2)
        T_VYPLUTI_LODI = strtod(argv[1], NULL);
    
    Init(0, T_KON);
    
    for (int i = 0; i < N_TRASY; i++)
        (new Generator)->init(i);
        
    Print(" DOL --- simulace průjezdu nákladních lodí\n");
    
    Run();
    
    int zarobek = 0;
    for (unsigned int i = 0; i < CelkovyPocLodi; i++) {
        int kontejneru = L_MIN_KAPACITA + Exponential(L_KAPACITA - L_MIN_KAPACITA);
        zarobek += kontejneru * CENA_KONTEJNR;
    }
    
    Print("\n Systémem nestihlo projet %u lodí, které by chtěly \n\n", PocLodiNaKoridoru);
    Print("\n Systémem prošlo %u lodí \n\n", CelkovyPocLodi);
    Print("\n Najeto kilometru celkem %u \n\n", CelkovyPocetKilometru);
    Print("\n Trasa vydelala priblizne %d \n\n", zarobek);
    SIMLIB_statistics.Output(); // print run statistics
}

//
