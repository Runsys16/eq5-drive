//------------------------------------------------------------------------------
//                                  Evenement.cpp
//------------------------------------------------------------------------------
#ifndef EVENEMENT_LIST_CPP
#define EVENEMENT_LIST_CPP
#endif
//------------------------------------------------------------------------------
#include "evenementList.h"
//------------------------------------------------------------------------------
//
//  Constructeur
//
//------------------------------------------------------------------------------
EvenementList::EvenementList()   {
    nb = 0;
    list = NULL;
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void EvenementList::addfirst(Evenement * pEvt)   {
    pEvt->precedent = NULL;

    if ( list != NULL )     list->precedent = pEvt;
    pEvt->suivant = list;

    list = pEvt;
    nb++;
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void EvenementList::addend(Evenement * pEvt)   {
    if ( list == NULL )     { 
        addfirst(pEvt);
        return;
    }
    
    Evenement *     current = list;
    
    while ( current->suivant != NULL )  {
        current = current->suivant;
    }
    
    current->suivant = pEvt;
    pEvt->suivant = NULL;
    pEvt->precedent = current;
    nb++;
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void EvenementList::sup(Evenement * pEvt)   {
    if ( pEvt->precedent != NULL )  pEvt->precedent->suivant = pEvt->suivant;
    if ( pEvt->suivant != NULL )    pEvt->suivant->precedent = pEvt->precedent;
    if ( list == pEvt )             list = pEvt->suivant;
    
    delete pEvt;
    
    nb--;
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void EvenementList::reset()   {
    while( list != NULL )   {
        Evenement * pEvt = getCurrent();
        sup(pEvt);
    }
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
Evenement* EvenementList::getLast()   {
    Evenement *     current = list;

    if ( current == NULL )       return NULL;
    
    while( current->suivant != NULL )   {
        current = current->suivant;
    }
    
    return current;
}

