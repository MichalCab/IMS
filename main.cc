#include <string>
#include <sstream>

#include "main.h"

using namespace std;



// char NazovTrasy = "Trasa1";
// int N_KILOMETRY = 150;                          // celkem kilometru na trase
// int KOMORY[] = {40, 60,  90, 130};              // na jakych kilometrech jsou komory
// int N_KOMORY = (sizeof(KOMORY) / sizeof(int));  // pocet komor
// Facility Komora[N_KOMORY];                      // komory
// Facility Kilometr[N_KILOMETRY];                 // kilometry

// definice konstant
const double T_KON = 365*24;    // kolik simulovanych hodin

const double T_PLNENI_KOMORY = 14/60;                   // doba plnění komory
const double T_VJEZD_A_VYJEZD_LODI_DO_Z_KOMORY  = 20/60;    // doba obsluhy lodě
const double T_OBSLUHY_KOMORY = T_PLNENI_KOMORY + T_VJEZD_A_VYJEZD_LODI_DO_Z_KOMORY;
const double T_PRUJEZD_TUNELEM = (60/10)/60;
const double T_RYCHLOST_LODI = (60/17)/60;                    // doba za kterou lod projde kilometr

double T_PRICH = 1;                         // příchod lodi
double CENA_KONTEJNR = 50;                  // priblizna cena za naklad a vyklad jednoho kontejneru
double L_KAPACITA = 470;                    // kapacita lodi v poctu kontejneru
double L_MIN_KAPACITA = 200;                // minimalni zaplnena kapacita pro vypluti lode

unsigned CelkovyPocLodi = 0;                // celkový počet lodí
unsigned PocLodiNaKoridoru = 0;                // aktuální počet lodí na trase
unsigned CelkovyPocetKilometru = 0;

const int N_TRASY = 6;

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

Trasa Trasy[N_TRASY] = {
        {"OdraTam", 98, (int[]){9, 16,  53, 69, 74, 76, 82, 88}, 8, OdraTamKomory, OdraTamKilometry, (int[]){22, 34}, 2, OdraTunely},
        {"OdraZpět", 98, (int[]){9, 16,  53, 69, 74, 76, 82, 88}, 8, OdraZpetKomory, OdraZpetKilometry, (int[]){22, 34}, 2, OdraTunely},

        {"DunajTam", 118, (int[]){1, 12,  22,27, 54, 83, 92, 99}, 8, DunajTamKomory, DunajTamKilometry}, (int[]){108}, 1, DunajTunely},
        {"DunajZpet", 118, (int[]){1, 12,  22,27, 54, 83, 92, 99}, 8, DunajZpetKomory, DunajZpetKilometry}, (int[]){108}, 1, DunajTunely},

        {"LabeTam", 154, (int[]){44, 51, 61, 66, 68, 69, 70, 99, 114, 116, 118, 119, 123, 139, 153}, 16, LabeTamKomory, LabeTamKilometry}, (int[]){52, 94, 97, 107, 111, 129}, 6, LabeTunely},
        {"LabeZpet", 154, (int[]){44, 51, 61, 66, 68, 69, 70, 99, 114, 116, 118, 119, 123, 139, 153}, 16, LabeZpetKomory, LabeZpetKilometry}, (int[]){52, 94, 97, 107, 111, 129}, 6, LabeTunely},
    };

class Lod : public Process {
    unsigned int UjetychKilometru;
    bool LodVyjela;
    int TrasaId;
    
    void Behavior()       // popis chování lodi
    {
        LodVyjela = false;
        UjetychKilometru = 0;
        while (UjetychKilometru < Trasy[TrasaId].N_KILOMETRY)      // dokud má kam jet
        {
            // KOMORY
            int poziceKomory = -1;
            for (int i = 0; i < Trasy[TrasaId].N_KOMORY; i++)      // Cyklus kontrolujici, zda jde lod do plavebni komory
            {
                if (Trasy[TrasaId].KOMORY[i] == UjetychKilometru)
                {
                    poziceKomory = i;
                    break;
                }
            }
            if (poziceKomory != -1)                 // pokud je na daném kilometru komora, vjede do ní
            {
                Seize(Trasy[TrasaId].Komory[poziceKomory]);        // Lod obsazuje plavebni komoru
                Wait(T_OBSLUHY_KOMORY);             // Lod ceka na dokonceni obsluhy plavebni komory aby mohla plout dal
                Release(Trasy[TrasaId].Komory[poziceKomory]);      // Opousti plavebni komoru
            }
    
            // TUNELY
            int poziceTunelu = -1;
            for (int i = 0; i < Trasy[TrasaId].N_TUNELY; i++)      // Cyklus kontrolujici, zda jde lod do tunelu
            {
                if (Trasy[TrasaId].TUNELY[i] == UjetychKilometru)
                {
                    poziceTunelu = i;
                    break;
                }
            }
            if (poziceTunelu != -1)                 // pokud je na daném kilometru komora, vjede do ní
            {
                Seize(Trasy[TrasaId].Tunely[poziceTunelu]);        // Lod obsazuje plavebni komoru
                Wait(T_PRUJEZD_TUNELEM);             // Lod ceka na dokonceni obsluhy plavebni komory aby mohla plout dal
                Release(Trasy[TrasaId].Tunely[poziceTunelu]);      // Opousti plavebni komoru
                UjetychKilometru++;
                CelkovyPocetKilometru++;
            }

            // JIZDA
            Seize(Trasy[TrasaId].Kilometry[UjetychKilometru]);      // Lod pluje, obsazuje dalsi kilometr
            if (LodVyjela == false)                 // Tohle je za potrebi v pripade, ze lod chce vyplout priliz brzo, kdy neni ani prvni kilometr volny
            {
                PocLodiNaKoridoru++;                   // Na trase se nachazi o lod navic
                CelkovyPocLodi++;                   // Pribiva dalsi lod za celkovy cas simulace
                LodVyjela = true;                   // Lod vyjela
            }
            Wait(T_RYCHLOST_LODI);                  // Lod pluje obsazeny kilometr
            Release(Trasy[TrasaId].Kilometry[UjetychKilometru]);    // Lod opousti obsazeny kilometr, uvolnuje
            
            UjetychKilometru++;                     // Lod proplula dalsi kilometr
            CelkovyPocetKilometru++;                // Na tomhle useku bylo lodi najeto dalsi kilometr
        }
        PocLodiNaKoridoru--;                        // loď konci svou plavbu, vyklada svuj naklad
    }
public:
    void init(int id)
    {
        TrasaId = id;
        Activate();
    }
};

class Generator : public Event {
    int TrasaId;
    void Behavior() 
    {
        (new Lod)->init(TrasaId);
        Activate(Time + Exponential(T_PRICH));
    }
public:
    void init(int TrasaId)
    {
        TrasaId = TrasaId;
        Activate();
    }
};

int main() {
    //Trasa t1 = {"Trasa 1", 150, (int[]){40, 60,  90, 130}, 4, Komory, Kilometry};
    
    //Trasa t2 = {"Trasa 2", 150, (int[]){40, 60,  90, 130}, 4, Komory, Kilometry};
    //Trasy[0] = t1;
    //Trasy[1] = t2;

    
    Init(0, T_KON);
    //for (int i = 0; i < N_TRASY; i++)
    //{   
    (new Generator)->init(0);
    //}   
    Print(" DOL --- simulace průjezdu nákladních lodí\n");
    Run();
    
    int zarobek = 0;
    for (int i = 0; i < CelkovyPocLodi; i++) {
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
