#include <string>
#include <sstream>

#include "simlib.h"
#include "main.h"

using namespace std;

// definice konstant
const double T_KON = 365*24;    // kolik simulovanych hodin

const double T_PLNENI_KOMORY = 14/60;                   // doba plnění komory
const double T_VJEZD_A_VYJEZD_LODI_DO_Z_KOMORY  = 20/60;    // doba obsluhy lodě
const double T_OBSLUHY_KOMORY = T_PLNENI_KOMORY + T_VJEZD_A_VYJEZD_LODI_DO_Z_KOMORY;
const double T_RYCHLOST_LODI = 0.07;                    // doba za kterou lod projde kilometr

double T_PRICH = 1;                         // příchod lodi
double CENA_KONTEJNR = 50;                  // priblizna cena za naklad a vyklad jednoho kontejneru
double L_KAPACITA = 470;                    // kapacita lodi v poctu kontejneru
double L_MIN_KAPACITA = 200;                // minimalni zaplnena kapacita pro vypluti lode

unsigned CelkovyPocLodi = 0;                // celkový počet lodí
unsigned PocLodiNaKoridoru = 0;                // aktuální počet lodí na trase
unsigned CelkovyPocetKilometru = 0;

class Lod : public Process {
    unsigned int UjetychKilometru;
    int LodId;
    bool LodVyjela;
    void Behavior()       // popis chování lodi
    {
        LodVyjela = false;
        UjetychKilometru = 0;
        while (UjetychKilometru < N_KILOMETRY && 1)      // dokud má kam jet
        {
            int poziceKomory = -1;
            for (int i = 0; i < N_KOMORY; i++)      // Cyklus kontrolujici, zda jde lod do plavebni komory
            {
                if (KOMORY[i] == UjetychKilometru)
                {
                    poziceKomory = i;
                    break;
                }
            }
            if (poziceKomory != -1)                 // pokud je na daném kilometru komora, vjede do ní
            {
                Seize(Komora[poziceKomory]);        // Lod obsazuje plavebni komoru
                Wait(T_OBSLUHY_KOMORY);             // Lod ceka na dokonceni obsluhy plavebni komory aby mohla plout dal
                Release(Komora[poziceKomory]);      // Opousti plavebni komoru
            }
            Seize(Kilometr[UjetychKilometru]);      // Lod pluje, obsazuje dalsi kilometr
            if (LodVyjela == false)                 // Tohle je za potrebi v pripade, ze lod chce vyplout priliz brzo, kdy neni ani prvni kilometr volny
            {
                PocLodiNaKoridoru++;                   // Na trase se nachazi o lod navic
                CelkovyPocLodi++;                   // Pribiva dalsi lod za celkovy cas simulace
                LodId = CelkovyPocLodi;             // Identifikace cisla lodi na trase
                LodVyjela = true;                   // Lod vyjela
            }
            Wait(T_RYCHLOST_LODI);                  // Lod pluje obsazeny kilometr
            Release(Kilometr[UjetychKilometru]);    // Lod opousti obsazeny kilometr, uvolnuje
            
            UjetychKilometru++;                     // Lod proplula dalsi kilometr
            CelkovyPocetKilometru++;                // Na tomhle useku bylo lodi najeto dalsi kilometr
        }
        PocLodiNaKoridoru--;                        // loď konci svou plavbu, vyklada svuj naklad
    }
public:
    void init(int id)
    {
        LodId = id;
        Activate();
    }
};

class Generator : public Event {
    void Behavior() {
        (new Lod)->init(1);
        Activate(Time + Exponential(T_PRICH));
    }
};

int main() {
    T_PRICH = 24;
    Init(0, T_KON);
    
    (new Generator)->Activate();
    
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
