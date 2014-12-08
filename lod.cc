#include "lod.h"

void Lod::posunoutLod()
{
    AktualniKilometr += Trasy[TrasaId].Direction;   // Lod proplula dalsi kilometr
    TrasaKilometry[TrasaId]++;                      // Pripoctem hodnotu najetych kilometru na konkretni trase
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
    if (poziceKomory != -1)                 // pokud je na dan?m kilometru komora, vjede do n?
    {
        Seize(Trasy[TrasaId].Komory[poziceKomory]);        // Lod obsazuje plavebni komoru
        if ((int)(Random()*(double)PRAVDEPODOBNOST_PORUCHY) == 0)
        {
            Wait(Exponential(T_TRVANI_PORUCHY));
        }
        
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
    if (poziceTunelu != -1)                 // pokud je na dan?m kilometru komora, vjede do n?
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
        LodVyjela = true;                   // Lod vyjela
        PocetLodi[TrasaId]++;
        PocetZaseklychLodi[TrasaId]++;
    }
    Wait(T_RYCHLOST_LODI);                  // Lod pluje obsazeny kilometr
    Release(Trasy[TrasaId].Kilometry[AktualniKilometr]);    // Lod opousti obsazeny kilometr, uvolnuje
    posunoutLod();                            // Na celem koridoru bylo lodma najeto dalsi kilometr
}

void Lod::Behavior() // popis chov?n? lodi
{
    LodVyjela = false;
    
    if (Trasy[TrasaId].Direction == -1)
        AktualniKilometr = Trasy[TrasaId].N_KILOMETRY - 1;
    else
        AktualniKilometr = 0;
    
    while ((Trasy[TrasaId].Direction == 1 && AktualniKilometr < Trasy[TrasaId].N_KILOMETRY) || (Trasy[TrasaId].Direction == -1 && AktualniKilometr > 0)) {
        handleKomora(); // Komory
        handleTunel();  // Tunely
        handlePlavba(); // Plavba
    }
    
    PocetZaseklychLodi[TrasaId]--;
}

void Lod::init(int id)
{
    TrasaId = id;
    Activate();
}
