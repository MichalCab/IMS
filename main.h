struct trasa 
{
    const char *NazovTrasy;
    int N_KILOMETRY;                          // celkem kilometru na trase
    int *KOMORY;              // na jakych kilometrech jsou komory
    int N_KOMORY;  // pocet komor
    Facility *Komora;                      // komory
    Facility *Kilometr;                 // kilometry
};

    // char NazovTrasy = "Trasa1";
    // int N_KILOMETRY = 150;                          // celkem kilometru na trase
    // int KOMORY[] = {40, 60,  90, 130};              // na jakych kilometrech jsou komory
    // int N_KOMORY = (sizeof(KOMORY) / sizeof(int));  // pocet komor
    // Facility Komora[N_KOMORY];                      // komory
    // Facility Kilometr[N_KILOMETRY];                 // kilometry

Facility K1[4];
Facility K2[150];
    
struct trasa Trasy[1] = {
    {"Trasa 1", 150, (int[]){40, 60,  90, 130}, 4, K1, K2}
};