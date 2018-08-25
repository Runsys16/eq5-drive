//------------------------------------------------------------------------------
//                              evenementList.h
//------------------------------------------------------------------------------
#ifndef EVENEMENT_LIST_H
#define EVENEMENT_LIST_H
//------------------------------------------------------------------------------
#include "Arduino.h"
#include "evenement.h"
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class EvenementList{
    private:
        Evenement*      list;
        int             nb;
    public:
        // Initialisation constructeur
        //-----------------------------
        EvenementList();
        // Ajoute cet objet au debut de la list
        void                reset();
        // Ajoute cet objet au debut de la list
        void                addfirst(Evenement* pEvt);
        void                addend(Evenement* pEvt);
        // Supprime cet objet la list
        void                sup(Evenement* pEvt);
        Evenement*          getLast();
        
        inline Evenement*   getCurrent()        {return list;}
        inline int          getNb()             {return nb;}
};
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
#ifdef EVENEMENT_LIST_CPP
    EvenementList   listEvenement;
#else
    extern EvenementList    listEvenement;
#endif
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
#endif
