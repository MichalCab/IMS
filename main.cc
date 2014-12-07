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
const double T_RYCHLOST_LODI = 60/17;                    // doba za kterou lod projde kilometr

double T_PRICH = 1;                         // příchod lodi
double CENA_KONTEJNR = 50;                  // priblizna cena za naklad a vyklad jednoho kontejneru
double L_KAPACITA = 470;                    // kapacita lodi v poctu kontejneru
double L_MIN_KAPACITA = 200;                // minimalni zaplnena kapacita pro vypluti lode

unsigned CelkovyPocLodi = 0;                // celkový počet lodí
unsigned PocLodiNaKoridoru = 0;                // aktuální počet lodí na trase
unsigned CelkovyPocetKilometru = 0;

const int N_TRASY = 10;
//Trasa Trasy[N_TRASY];
Facility PrerovOstravaKomory[4];
Facility PrerovOstravaKilometry[18888];
Facility OstravaPrerovKomory[4];
Facility OstravaPrerovKilometry[147];
Facility Tunely[];
Facility OstravaKozleKomory[4];
Facility OstravaKozleKilometry[53];
Facility KozleOstravaKomory[4];
Facility KozleOstravaKilometry[53];
Facility Tunely[];
Facility DunajHodoninKomory[4];
Facility DunajHodoninKilometry[98];
Facility HodoninDunajKomory[4];
Facility HodoninDunajKilometry[98];
Facility Tunely[];
Facility HodoninPrerovKunoviceKomory[4];
Facility HodoninPrerovKunoviceKilometry[90];
Facility KunovicePrerovHodoninKomory[4];
Facility KunovicePrerovHodoninKilometry[90];
Facility Tunely[];
Facility KunovicePardubiceKomory[4];
Facility KunovicePardubiceKilometry[120];
Facility PardubiceKunoviceKomory[4];
Facility PardubiceKunoviceKilometry[120];
Facility Tunely[];

Trasa Trasy[N_TRASY] = {
        {"PrerovOstrava", 98, (int[]){9, 16,  53, 69, 74, 76, 82, 88}, 8, PrerovOstravaKomory, PrerovOstravaKilometry, (int[]){22, 34}, 2, OdraTunely},
        {"PrerovOstrava", 98, (int[]){9, 16,  53, 69, 74, 76, 82, 88}, 8, PrerovOstravaKomory, PrerovOstravaKilometry, (int[]){22, 34}, 2, OdraTunely},

        {"OstravaKozle", 53, (int[]){40, 60,  90, 130}, 4, OstravaKozleKomory, OstravaKozleKilometry},
        {"KozleOstrava", 53, (int[]){40, 60,  90, 130}, 4, KozleOstravaKomory, KozleOstravaKilometry},

        {"DunajHodonin", 98, (int[]){40, 60,  90, 130}, 4, DunajHodoninKomory, DunajHodoninKilometry},
        {"HodoninDunaj", 98, (int[]){40, 60,  90, 130}, 4, HodoninDunajKomory, HodoninDunajKilometry},
        {"HodoninPrerovKunovice", 90, (int[]){40, 60,  90, 130}, 4, HodoninPrerovKunoviceKomory, HodoninPrerovKunoviceKilometry},
        {"KunovicePrerovHodonin", 90, (int[]){40, 60,  90, 130}, 4, KunovicePrerovHodoninKomory, KunovicePrerovHodoninKilometry},
        {"KunovicePardubice", 120, (int[]){40, 60,  90, 130}, 4, KunovicePardubiceKomory, KunovicePardubiceKilometry},
        {"PardubiceKunovice", 130, (int[]){40, 60,  90, 130}, 4, PardubiceKunoviceKomory, PardubiceKunoviceKilometry},
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
