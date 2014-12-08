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

double T_VYPLUTI_LODI = 6;                  // příchod lodi
double CENA_KONTEJNR = 50;                  // priblizna cena za naklad a vyklad jednoho kontejneru
double L_KAPACITA = 250;                    // kapacita lodi v poctu kontejneru
double L_MIN_KAPACITA = 192;                // minimalni zaplnena kapacita pro vypluti lode
double PRAVDEPODOBNOST_PORUCHY = 1/0.001;
double T_TRVANI_PORUCHY = 24*4;

unsigned int TrasaKilometry[N_TRASY] = {0,};
unsigned int PocetLodi[N_TRASY] = {0,};
unsigned int PocetZaseklychLodi[N_TRASY] = {0,};

Trasa Trasy[N_TRASY] = {
        {"OdraTam", 98, (unsigned int[]){9, 16, 53, 69, 74, 76, 82, 88}, 8, OdraTamKomory, OdraTamKilometry, (unsigned int[]){22, 34}, 2, OdraTunely, 1},
        {"OdraZpět", 98, (unsigned int[]){9, 16, 53, 69, 74, 76, 82, 88}, 8, OdraZpetKomory, OdraZpetKilometry, (unsigned int[]){22, 34}, 2, OdraTunely, -1},

        {"DunajTam", 118, (unsigned int[]){1, 12, 22, 27, 54, 83, 92, 99}, 8, DunajTamKomory, DunajTamKilometry, (unsigned int[]){108}, 1, DunajTunely, 1},
        {"DunajZpet", 118, (unsigned int[]){1, 12, 22, 27, 54, 83, 92, 99}, 8, DunajZpetKomory, DunajZpetKilometry, (unsigned int[]){108}, 1, DunajTunely, -1},

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
    void init(int trasaId)
    {
        TrasaId = trasaId;
        Activate();
    }
};

int main(int argc, char *argv[]) {
    if (argc == 2)
        T_VYPLUTI_LODI = strtod(argv[1], NULL);
    
    Init(0, T_KON);
    
    for (int i = 0; i < N_TRASY; i++)
        (new Generator)->init(i);
        
    Print("\n DOL --- simulace koridoru Dunaj-Odra-Labe\n");
    
    Run();
    
    int zarobek[N_TRASY];
    int zarobekCelkem = 0;
    unsigned int CelkovyPocLodi = 0;
    unsigned int CelkovyPocetKilometru = 0;
    unsigned int CelkovyPocetZaseklychLodi = 0;
    
    for (int i = 0; i < N_TRASY; i+=2){
        zarobek[i] = 0;
        for (unsigned int k = 0; k < PocetLodi[i]; k++) {
            unsigned kontejneru = L_MIN_KAPACITA + Exponential(L_KAPACITA - L_MIN_KAPACITA);
            zarobek[i] += kontejneru * (unsigned)CENA_KONTEJNR;
        }
        
        Print("\n---------- TRASA %s <=> %s ---------- ", Trasy[i].NazovTrasy, Trasy[i+1].NazovTrasy);
        Print("\n   Trasou prošlo lodí:               %u", (PocetLodi[i] + PocetLodi[i+1]));
        Print("\n   Na trase zustalo lodí:            %u", (PocetZaseklychLodi[i] + PocetZaseklychLodi[i+1]));
        Print("\n   Lode najeli celkem kilometru:     %u", (TrasaKilometry[i] + TrasaKilometry[i+1]));
        Print("\n   Trasa vydelala priblizne:         %u", zarobek[i]);
        
        zarobekCelkem += zarobek[i] + zarobek[i+1];
        CelkovyPocLodi += (PocetLodi[i] + PocetLodi[i+1]);
        CelkovyPocetKilometru += (TrasaKilometry[i] + TrasaKilometry[i+1]);
        Print("\n------------------------------------------------- ");
    }
    
    Print("\n----------------- CELKOVE CISLA ----------------- ");
    Print("\n   Trasou prošlo lodí:               %u", CelkovyPocLodi);
    Print("\n   Na trase zustalo lodí:            %u", CelkovyPocetZaseklychLodi);
    Print("\n   Lode najeli celkem kilometru:     %u", CelkovyPocetKilometru);
    Print("\n   Koridor vydelal priblizne:        %u", zarobekCelkem);
    Print("\n------------------------------------------------- \n");
    
    SIMLIB_statistics.Output(); // print run statistics
}

//
