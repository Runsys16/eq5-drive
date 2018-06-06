//------------------------------------------------------------------------------
//                              A4988.h
//------------------------------------------------------------------------------
#ifndef A4988_H
#define A4988_H
//------------------------------------------------------------------------------
#include "Arduino.h"
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class A4988{
    private:
        int MS1, MS2, MS3;
        int EN, RST, STEP, DIR;

        bool    bSens;
        bool    bStep;
        bool    bRot;

        long    cpt;
        long    jeu;
    public:
        // Initialisation
        // constructeur
        A4988(int, int, int, int, int, int, int);
        // selection du mode de pas 
        void            selPas16();
        void            selPas8();
        void            selPas4();
        void            selPas2();
        void            selPas();
        // Setup
        bool            setup(void); 
        // rotation normal ou inverse
        inline void     changeRot(void)     { bRot = !bRot; }
        inline bool     getRot(void)        { return bRot; }
        inline void     inverseRot(void)    { bRot = false; }
        inline void     normalRot(void)     { bRot = true; }
        // choix du sens de rotation
        inline bool     getSens()           { return bSens; }
        void            setSens(bool);
        // Active/desactive le moteur
        inline void     enable(void)        { digitalWrite(EN,LOW); }
        inline void     disable(void)       { digitalWrite(EN,HIGH); }
        // compteur de pas 
        void            resetCount()        { cpt = 0; }
        inline long     getCount()          { return cpt; }
        inline void     setCount(long c)    { cpt = c; }
        // hysteresis (rattrapage dujeu)
        inline long     getJeu()            { return jeu; }
        inline void     setJeu(long c)      { jeu = c; }
        // Front sur la broche step du driver
        // Modificaiton du compteur cpt
        void            step();
        void            step(bool);
        void            step(int, int);
        bool            getStep()           { return bStep; }
        void            setStep(long l)     { cpt = l; }
        void            incStep()           { cpt++; }
        void            decStep()           { cpt--; }
        // Arret
        void            stop();
};

#endif
