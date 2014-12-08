#include "simlib.h"

#ifndef _IFCE_H_INCLUDED
#define _IFCE_H_INCLUDED

class Lod : public Process {
private:
    unsigned int AktualniKilometr;
    bool LodVyjela;
    int TrasaId;
    
    void posunoutLod();
    void handleKomora();
    void handleTunel();
    void handlePlavba();
    void Behavior();
    
public:
    void init(int id);
};


typedef struct
{
    const char *NazovTrasy;
    unsigned int N_KILOMETRY;   // celkem kilometru na trase
    unsigned int *KOMORY;       // na jakych kilometrech jsou komory
    unsigned int N_KOMORY;      // pocet komor
    Facility *Komory;           // komory
    Facility *Kilometry;        // kilometry
    unsigned int *TUNELY;
    unsigned int N_TUNELY;
    Facility *Tunely;
    int Direction;
} Trasa;

// definice konstant
const double T_KON = 365*24;                // kolik simulovanych hodin
const double T_PLNENI_KOMORY = 14/60;       // doba plnění komory
const double T_VJEZD_A_VYJEZD_LODI_DO_Z_KOMORY  = 20/60;    // doba obsluhy lodě
const double T_OBSLUHY_KOMORY = T_PLNENI_KOMORY + T_VJEZD_A_VYJEZD_LODI_DO_Z_KOMORY;
const double T_PRUJEZD_TUNELEM = (60/10)/60;
const double T_RYCHLOST_LODI = (60/17)/60;  // doba za kterou lod projde kilometr

extern double T_VYPLUTI_LODI;               // příchod lodi
extern double PRAVDEPODOBNOST_PORUCHY;      // příchod lodi
extern double T_TRVANI_PORUCHY;
extern double CENA_KONTEJNR;                // priblizna cena za naklad a vyklad jednoho kontejneru
extern double L_KAPACITA;                   // kapacita lodi v poctu kontejneru
extern double L_MIN_KAPACITA;

const int N_TRASY = 6;

extern Facility OdraTamKomory[8];
extern Facility OdraTamKilometry[98];
extern Facility OdraZpetKomory[8];
extern Facility OdraZpetKilometry[98];
extern Facility OdraTunely[2];

extern Facility DunajTamKomory[8];
extern Facility DunajTamKilometry[118];
extern Facility DunajZpetKomory[8];
extern Facility DunajZpetKilometry[118];
extern Facility DunajTunely[1];

extern Facility LabeTamKomory[16];
extern Facility LabeTamKilometry[154];
extern Facility LabeZpetKomory[16];
extern Facility LabeZpetKilometry[154];
extern Facility LabeTunely[6];

extern Trasa Trasy[N_TRASY];

extern unsigned TrasaKilometry[N_TRASY];
extern unsigned PocetZaseklychLodi[N_TRASY];
extern unsigned PocetLodi[N_TRASY];

#endif
