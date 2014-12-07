#include <string>
#include <sstream>

#include "simlib.h"

using namespace std;

// definice konstant
const double T_KON = 365*24;    // kolik simulovanych hodin

const double T_PLNENI_KOMORY = 14/60;                   // doba plnění komory
const double T_VJEZD_A_VYJEZD_LODI_DO_Z_KOMORY  = 20/60;    // doba obsluhy lodě
const double T_OBSLUHY_KOMORY = T_PLNENI_KOMORY + T_VJEZD_A_VYJEZD_LODI_DO_Z_KOMORY;
const double T_RYCHLOST_LODI = 0.07;                    // doba za kterou lod projde kilometr
const int N_KILOMETRY = 150;                            // celkem kilometru na trase

const int KOMORY[] = {40, 60,  90, 130};                // na jakych kilometrech jsou komory
const int N_KOMORY = (sizeof(KOMORY) / sizeof(int));    // pocet komor


// deklarace globalnich objektu
Facility Komora[N_KOMORY];                  // komory
Facility Kilometr[N_KILOMETRY];             // kilometry

double T_PRICH = 1;                         // příchod lodi
double CENA_KONTEJNR = 50;                  // priblizna cena za naklad a vyklad jednoho kontejneru
double L_KAPACITA = 470;                    // kapacita lodi v poctu kontejneru
double L_MIN_KAPACITA = 200;                // minimalni zaplnena kapacita pro vypluti lode

unsigned CelkovyPocLodi = 0;                // celkový počet lodí
unsigned PocLodiNaTrase = 0;                // aktuální počet lodí na trase
unsigned CelkovyPocetKilometru = 0;

class Lod : public Process {
  unsigned int UjetychKilometru;
  int LodId;
  bool LodVyjela;
  void Behavior()       // popis chování lodi
  {
    LodVyjela = false;
    UjetychKilometru = 0;
    while (UjetychKilometru < N_KILOMETRY)      // dokud má kam jet
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
            PocLodiNaTrase++;                   // Na trase se nachazi o lod navic
            CelkovyPocLodi++;                   // Pribiva dalsi lod za celkovy cas simulace
            LodId = CelkovyPocLodi;             // Identifikace cisla lodi na trase
            LodVyjela = true;                   // Lod vyjela
        }
        Wait(T_RYCHLOST_LODI);                  // Lod pluje obsazeny kilometr
        Release(Kilometr[UjetychKilometru]);    // Lod opousti obsazeny kilometr, uvolnuje
        
        UjetychKilometru++;                     // Lod proplula dalsi kilometr
        CelkovyPocetKilometru++;                // Na tomhle useku bylo lodi najeto dalsi kilometr
    }
    PocLodiNaTrase--;                           // loď konci svou plavbu, vyklada svuj naklad
  }
};

class Generator : public Event {
  void Behavior() {
    (new Lod)->Activate();
    Activate(Time + Exponential(T_PRICH));
  }
};

int main() {
    T_PRICH = 5;
    // for(int i = 0; i < N_KOMORY; i++)
    // {
        // std::stringstream nazevKomory;
        // nazevKomory << "Komora " << i;
        // std::string str = nazevKomory.str();
        // Komora[i].SetName(str.c_str());
    // }
    Init(0, T_KON);         // initialize, non-zero initial time
    (new Generator)->Activate();        // generator lodí
    Print(" DOL --- simulace průjezdu nákladních lodí\n");
    Run();                      // simulation
    Print("\n Systémem nestihlo projet %u lodí, které by chtěly \n\n", PocLodiNaTrase);
    Print("\n Systémem prošlo %u lodí \n\n", CelkovyPocLodi);
    Print("\n Najeto kilometru celkem %u \n\n", CelkovyPocetKilometru);
    int zarobek = 0;
    for (int i = 0; i < CelkovyPocLodi; i++) {
        int kontejneru = L_MIN_KAPACITA + Exponential(L_KAPACITA - L_MIN_KAPACITA);
        zarobek += kontejneru * CENA_KONTEJNR;
    }
    Print("\n Trasa vydelala priblizne %d \n\n", zarobek);
    SIMLIB_statistics.Output(); // print run statistics
}

//
