#include "simlib.h"

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