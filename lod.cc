#include "simlib.h"
#include "lod.h"
#include "main.h"

void Lod::posunoutLod()
{
    AktualniKilometr += Trasy[TrasaId].Direction;   // Lod proplula dalsi kilometr
    CelkovyPocetKilometru++;                        // Na celem koridoru bylo lodma najeto dalsi kilometr
}

void Lod::handleKomora()
{
    int poziceKomory = -1;
    for (unsigned int i = 0; i < Trasy[TrasaId].N_KOMORY; i++)      // Cyklus kontrolujici, zda jde lod do plavebni komory
    {
        if (Trasy[TrasaId].KOMORY[i] == AktualniKilometr)
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
}

void Lod::handleTunel()
{
    int poziceTunelu = -1;
    for (unsigned int i = 0; i < Trasy[TrasaId].N_TUNELY; i++)      // Cyklus kontrolujici, zda jde lod do tunelu
    {
        if (Trasy[TrasaId].TUNELY[i] == AktualniKilometr)
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
        posunoutLod();
    }
}

void Lod::handlePlavba()
{
    Seize(Trasy[TrasaId].Kilometry[AktualniKilometr]);      // Lod pluje, obsazuje dalsi kilometr
    if (LodVyjela == false)                 // Tohle je za potrebi v pripade, ze lod chce vyplout priliz brzo, kdy neni ani prvni kilometr volny
    {
        PocLodiNaKoridoru++;                   // Na trase se nachazi o lod navic
        CelkovyPocLodi++;                   // Pribiva dalsi lod za celkovy cas simulace
        LodVyjela = true;                   // Lod vyjela
    }
    Wait(T_RYCHLOST_LODI);                  // Lod pluje obsazeny kilometr
    Release(Trasy[TrasaId].Kilometry[AktualniKilometr]);    // Lod opousti obsazeny kilometr, uvolnuje
    posunoutLod();                            // Na celem koridoru bylo lodma najeto dalsi kilometr
}

void Lod::Behavior() // popis chování lodi
{
    LodVyjela = false;
    
    if (Trasy[TrasaId].Direction == -1)
        AktualniKilometr = Trasy[TrasaId].N_KILOMETRY;
    else
        AktualniKilometr = 0;
    
    while ((Trasy[TrasaId].Direction == 1 && AktualniKilometr < Trasy[TrasaId].N_KILOMETRY) || (Trasy[TrasaId].Direction == -1 && AktualniKilometr > 0)) {
        handleKomora(); // Komory
        handleTunel();  // Tunely
        handlePlavba(); // Plavba
    }
    PocLodiNaKoridoru--;    // lod konci svou plavbu, vyklada svuj naklad
}

void Lod::init(int id)
{
    TrasaId = id;
    Activate();
}