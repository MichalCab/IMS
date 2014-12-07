typedef struct 
{
    const char *NazovTrasy;
    unsigned int N_KILOMETRY;                          // celkem kilometru na trase
    unsigned int *KOMORY;              // na jakych kilometrech jsou komory
    unsigned int N_KOMORY;  // pocet komor
    Facility *Komory;                      // komory
    Facility *Kilometry;                 // kilometry
    unsigned int *TUNELY;
    unsigned int N_TUNELY;
    Facility *Tunely;
    int Direction;
} Trasa;

// definice konstant
const double T_KON = 365*24;    // kolik simulovanych hodin
const double T_PLNENI_KOMORY = 14/60;                   // doba plnění komory
const double T_VJEZD_A_VYJEZD_LODI_DO_Z_KOMORY  = 20/60;    // doba obsluhy lodě
const double T_OBSLUHY_KOMORY = T_PLNENI_KOMORY + T_VJEZD_A_VYJEZD_LODI_DO_Z_KOMORY;
const double T_PRUJEZD_TUNELEM = (60/10)/60;
const double T_RYCHLOST_LODI = (60/17)/60;                    // doba za kterou lod projde kilometr

double T_VYPLUTI_LODI = 24;                            // příchod lodi
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
        {"OdraTam", 98, (unsigned int[]){9, 16,  53, 69, 74, 76, 82, 88}, 8, OdraTamKomory, OdraTamKilometry, (unsigned int[]){22, 34}, 2, OdraTunely, 1},
        {"OdraZpět", 98, (unsigned int[]){9, 16,  53, 69, 74, 76, 82, 88}, 8, OdraZpetKomory, OdraZpetKilometry, (unsigned int[]){22, 34}, 2, OdraTunely, -1},

        {"DunajTam", 118, (unsigned int[]){1, 12,  22,27, 54, 83, 92, 99}, 8, DunajTamKomory, DunajTamKilometry, (unsigned int[]){108}, 1, DunajTunely, 1},
        {"DunajZpet", 118, (unsigned int[]){1, 12,  22,27, 54, 83, 92, 99}, 8, DunajZpetKomory, DunajZpetKilometry, (unsigned int[]){108}, 1, DunajTunely, -1},

        {"LabeTam", 154, (unsigned int[]){44, 51, 61, 66, 68, 69, 70, 99, 114, 116, 118, 119, 123, 139, 153}, 16, LabeTamKomory, LabeTamKilometry, (unsigned int[]){52, 94, 97, 107, 111, 129}, 6, LabeTunely, 1},
        {"LabeZpet", 154, (unsigned int[]){44, 51, 61, 66, 68, 69, 70, 99, 114, 116, 118, 119, 123, 139, 153}, 16, LabeZpetKomory, LabeZpetKilometry, (unsigned int[]){52, 94, 97, 107, 111, 129}, 6, LabeTunely, -1},
    };