#include <string>
#include <sstream>
#include <stdlib.h>

#include "lod.h"

Facility OdraTamKomory[8];
Facility OdraTamKilometry[98];
Facility OdraZpetKomory[8];
Facility OdraZpetKilometry[98];
Facility OdraTunely[2];

Facility DunajTamKomory[8];
Facility DunajTamKilometry[118];
Facility DunajZpetKomory[8];
Facility DunajZpetKilometry[118];
Facility DunajTunely[1];

Facility LabeTamKomory[16];
Facility LabeTamKilometry[154];
Facility LabeZpetKomory[16];
Facility LabeZpetKilometry[154];
Facility LabeTunely[6];

unsigned CelkovyPocLodi = 0;	// celkový počet lodí
unsigned PocLodiNaKoridoru = 0;                // aktuální počet lodí na trase
unsigned CelkovyPocetKilometru = 0;

double T_VYPLUTI_LODI = 24;                            // příchod lodi
double CENA_KONTEJNR = 50;                  // priblizna cena za naklad a vyklad jednoho kontejneru
double L_KAPACITA = 470;                    // kapacita lodi v poctu kontejneru
double L_MIN_KAPACITA = 200;                // minimalni zaplnena kapacita pro vypluti lode
double T_TRVANI_PORUCHY = 24*4;

Trasa Trasy[N_TRASY] = {
        {"OdraTam", 98, (unsigned int[]){9, 16,  53, 69, 74, 76, 82, 88}, 8, OdraTamKomory, OdraTamKilometry, (unsigned int[]){22, 34}, 2, OdraTunely, 1},
        {"OdraZpět", 98, (unsigned int[]){9, 16,  53, 69, 74, 76, 82, 88}, 8, OdraZpetKomory, OdraZpetKilometry, (unsigned int[]){22, 34}, 2, OdraTunely, -1},

        {"DunajTam", 118, (unsigned int[]){1, 12,  22,27, 54, 83, 92, 99}, 8, DunajTamKomory, DunajTamKilometry, (unsigned int[]){108}, 1, DunajTunely, 1},
        {"DunajZpet", 118, (unsigned int[]){1, 12,  22,27, 54, 83, 92, 99}, 8, DunajZpetKomory, DunajZpetKilometry, (unsigned int[]){108}, 1, DunajTunely, -1},

        {"LabeTam", 154, (unsigned int[]){44, 51, 61, 66, 68, 69, 70, 99, 114, 116, 118, 119, 123, 139, 153}, 16, LabeTamKomory, LabeTamKilometry, (unsigned int[]){52, 94, 97, 107, 111, 129}, 6, LabeTunely, 1},
        {"LabeZpet", 154, (unsigned int[]){44, 51, 61, 66, 68, 69, 70, 99, 114, 116, 118, 119, 123, 139, 153}, 16, LabeZpetKomory, LabeZpetKilometry, (unsigned int[]){52, 94, 97, 107, 111, 129}, 6, LabeTunely, -1},
    };

class Generator : public Event {
private:
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

class GeneratorPoruch : public Event {
private:
    void Behavior()
    {
        (new Lod)->init(TrasaId);
        Activate(Time + Exponential(T_));
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
