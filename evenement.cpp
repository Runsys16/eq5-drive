//------------------------------------------------------------------------------
//                                  Evenement.cpp
//------------------------------------------------------------------------------
#ifndef EVENEMENT_CPP
#define EVENEMENT_CPP
#endif
//------------------------------------------------------------------------------
#include "evenement.h"
//------------------------------------------------------------------------------
//
//  Constructeur
//
//------------------------------------------------------------------------------
Evenement::Evenement(long a, long d, int va, int vd, bool b, bool bp)   {
    pasAD           = a;
    pasDC           = d;
    
    vitAD           = va;
    vitDC           = vd;
    
    bDecompte       = b;
    bPrintGOTO      = bp;
    
    currentPasAD    = 0;
    currentPasDC    = 0;
    
    suivant   = NULL;
    precedent = NULL;
    
}
//------------------------------------------------------------------------------
Evenement::Evenement(long a, long d, int va, int vd, bool b)   {
    pasAD           = a;
    pasDC           = d;
    
    vitAD           = va;
    vitDC           = vd;
    
    bDecompte       = b;
    bPrintGOTO      = true;
    
    currentPasAD    = 0;
    currentPasDC    = 0;
    
    suivant   = NULL;
    precedent = NULL;
    
}
//------------------------------------------------------------------------------
Evenement::Evenement()   {
    pasAD           = 0;
    pasDC           = 0;
    
    vitAD           = -1;
    vitDC           = -1;
    
    bDecompte       = true;
    bPrintGOTO      = true;
    
    currentPasAD    = 0;
    currentPasDC    = 0;
    
    suivant   = NULL;
    precedent = NULL;
    
}
//------------------------------------------------------------------------------
Evenement::Evenement(bool b)   {
    pasAD           = 0;
    pasDC           = 0;
    
    vitAD           = -1;
    vitDC           = -1;
    
    bDecompte       = b;
    bPrintGOTO      = true;
    
    currentPasAD    = 0;
    currentPasDC    = 0;
    
    suivant   = NULL;
    precedent = NULL;
    
}

