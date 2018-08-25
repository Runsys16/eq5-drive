//------------------------------------------------------------------------------
//                              evenement.h
//------------------------------------------------------------------------------
#ifndef EVENEMENT_H
#define EVENEMENT_H
//------------------------------------------------------------------------------
#include "Arduino.h"
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class Evenement{
    private:
        long            pasAD;
        long            pasDC;

        int             vitAD;
        int             vitDC;

        int             currentPasAD;
        int             currentPasDC;

        bool            bDecompte;
        bool            bPrintGOTO;
        
    public:
        // Liaison de la list chaînée
        //-----------------------------
        Evenement *     suivant;
        Evenement *     precedent;
        
        // Initialisation constructeur
        //-----------------------------
        Evenement(long, long, int, int, bool, bool);
        Evenement(long, long, int, int, bool);
        Evenement(bool);
        Evenement();
        
        // Acces aux membres de la classe
        //-----------------------------
        inline long     getPasAD()          {return pasAD;}
        inline long     getPasDC()          {return pasDC;}

        inline int      getVitAD()          {return vitAD;}
        inline int      getVitDC()          {return vitDC;}

        inline bool     getDecompte()       {return bDecompte;}
        inline bool     getPrintGOTO()      {return bPrintGOTO;}
        //-----------------------------
        inline void     setPasAD(long i)    {pasAD = i;}
        inline void     setPasDC(long i)    {pasDC = i;}

        inline void     setVitAD(int i)     {vitAD = i;}
        inline void     setVitDC(int i)     {vitDC = i;}

        inline void     setDecompte(bool b) {bDecompte = b;}
        inline void     setPrintGOTO(bool b){bPrintGOTO = b;}
};
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
#endif
