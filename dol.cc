#include <string>
#include <sstream>
#include <iterator>
#include <iostream>     // std::cout
//#include <algorithm>    // std::find
#include <vector>       // std::vector

#include "simlib.h"

// time of simulation:  T_KON - stop
const double T_KON = 365*24;   // hodin

// definice konstant
const double T_PLNENI_KOMORY  = 14/60;   // doba plnění komory
const double T_VJEZD_LODI_DO_KOMORY  = 10/60;   // doba obsluhy lodě
const double T_OBSLUHY = T_PLNENI_KOMORY + T_VJEZD_LODI_DO_KOMORY;
const double T_PRICH = 1; // příchod lodi
const double T_RYCHLOST_LODI = 0.3;

const int N_KILOMETRY = 150;

const int KOMORY[] = {40, 60,  90, 130}; // na jakych kilometrech jsou komory
const int N_KOMORY = (sizeof(KOMORY) / sizeof(const int));

// deklarace globalnich objektu
Facility Komora[N_KOMORY];           // komora
Facility Kilometr[N_KILOMETRY];

unsigned CelkovyPocLodi = 0; // celkový počet lodí
unsigned PocLodiNaTrase = 0;    // aktuální počet lodí na trase
unsigned CelkovyPocetKilometru = 0;

Histogram Tabulka ("Čekací doby lodi",0,0.15,20);

using namespace std;

class Lod : public Process {  // 
  double Vyjezd;               // čas vjezdu lodě do systému
  unsigned int UjetychKilometru;
  int LodId;
  void Behavior() // popis chování lodi
  {
    Vyjezd = Time;
    PocLodiNaTrase++;
    CelkovyPocLodi++;
	LodId = CelkovyPocLodi;
    while (UjetychKilometru < N_KILOMETRY) // dokud má kam jet
    {
        int poziceKomory = -1;
        for (int i = 0; i < N_KOMORY; i++)
        {
            if (KOMORY[i] == UjetychKilometru)
            {
                poziceKomory = i;
                break;
            }
        }
        if (poziceKomory != -1)// pokud je na daném kilometru komora, vjede do ní
        {
            Seize(Komora[poziceKomory]);
            Wait(T_OBSLUHY);
            Release(Komora[poziceKomory]);
        }
        Seize(Kilometr[UjetychKilometru]);
        Wait(T_RYCHLOST_LODI);
        Release(Kilometr[UjetychKilometru]);
		UjetychKilometru++;
		CelkovyPocetKilometru++;
    }
    PocLodiNaTrase--; // loď odplouvá ze systéme
    Tabulka(Time - Vyjezd);    // záznam o výjezdu
  }
};

class Generator : public Event {        //
  void Behavior() {                     //
    (new Lod)->Activate();         //
    Activate(Time + Exponential(T_PRICH));
  }
};

int main() {
    //SetOutput("dol.out");    // output redirection
    for(int i = 0; i < N_KOMORY; i++)
    {
        std::stringstream nazevKomory;
        nazevKomory << "Komora " << i;
        std::string str = nazevKomory.str();
        Komora[i].SetName(str.c_str());
    }
    Init(0, T_KON);         // initialize, non-zero initial time
    Tabulka.Clear();            // we start at non-zero time
    (new Generator)->Activate();        // generator lodí
    Print(" DOL --- simulace průjezdu nákladních lodí\n");
    Run();                      // simulation
    Print("\n Systémem nestihlo projet %u lodí, které by chtěly \n\n", PocLodiNaTrase);
    Print("\n Systémem prošlo %u lodí \n\n", CelkovyPocLodi);
    Print("\n Najeto kilometru celkem %u \n\n", CelkovyPocetKilometru);
    Tabulka.Output();
    SIMLIB_statistics.Output(); // print run statistics
}

//
