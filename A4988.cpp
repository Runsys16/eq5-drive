//------------------------------------------------------------------------------
//                                  A4988.cpp
//------------------------------------------------------------------------------
#include "A4988.h"
//------------------------------------------------------------------------------
// Port mapping
//------------------------------------------------------------------------------
A4988::A4988(   int _MS1, int _MS2, int _MS3,
                int _EN, int _RST, int _STEP, int _DIR )   {
    MS1 = _MS1;
    MS2 = _MS2;
    MS3 = _MS3;
    EN  = _EN;
    RST = _RST;
    STEP = _STEP;
    DIR = _DIR;
    bRot = true;
    
    cpt = 0;
    jeu = 0;
}
//------------------------------------------------------------------------------
// Initialise les ports
//------------------------------------------------------------------------------
bool    A4988::setup(void)  {
    pinMode( MS1,  OUTPUT );
    pinMode( MS2,  OUTPUT );
    pinMode( MS3,  OUTPUT );
    pinMode( EN,   OUTPUT );
    pinMode( RST,  OUTPUT );
    pinMode( STEP, OUTPUT );
    pinMode( DIR,  OUTPUT );
    
    digitalWrite( DIR   , HIGH); // Direction avant
    digitalWrite( STEP  , LOW);  // Initialisation de la broche step
    bStep = false;

    enable();
    digitalWrite( RST, HIGH );   // logique inversée
    
    cpt = 0;
    jeu = 0;
}
//------------------------------------------------------------------------------
// Change le pas
//------------------------------------------------------------------------------
void A4988::selPas16(){
    digitalWrite( MS1   , HIGH);
    digitalWrite( MS2   , HIGH);
    digitalWrite( MS3   , HIGH);
}
//------------------------------------------------------------------------------
// Change le pas
//------------------------------------------------------------------------------
void A4988::selPas8(){
    digitalWrite( MS1   , HIGH);
    digitalWrite( MS2   , HIGH);
    digitalWrite( MS3   , LOW);
}
//------------------------------------------------------------------------------
// Change le pas
//------------------------------------------------------------------------------
void A4988::selPas4(){
    digitalWrite( MS1   , LOW);
    digitalWrite( MS2   , HIGH);
    digitalWrite( MS3   , LOW);
}
//------------------------------------------------------------------------------
// Change le pas
//------------------------------------------------------------------------------
void A4988::selPas2(){
    digitalWrite( MS1   , HIGH);
    digitalWrite( MS2   , LOW);
    digitalWrite( MS3   , LOW);
}
//------------------------------------------------------------------------------
// Change le pas
//------------------------------------------------------------------------------
void A4988::selPas(){
    digitalWrite( MS1   , LOW);
    digitalWrite( MS2   , LOW);
    digitalWrite( MS3   , LOW);
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void    A4988::setSens(bool b)     { 
    // On tient compte du jeu de la vis sans fin
    if ( bSens != b )   {
        cpt = cpt + (bSens ? +jeu : -jeu ); 
    }
    //--------------------------------------------
    bSens = b;
    if (bSens)      {
        if ( bRot )     digitalWrite(DIR, HIGH);
        else            digitalWrite(DIR, LOW);
    }
    else    {
        if ( bRot )     digitalWrite(DIR, LOW);
        else            digitalWrite(DIR, HIGH);
    }
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void    A4988::step()     { 
    if( bStep )    {
        digitalWrite(STEP, LOW);
        bStep = false;
    }
    else {
        digitalWrite(STEP, HIGH);
        bStep = true;
    }
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void    A4988::step(bool b)     { 
    step();
    if( b && bStep )    {
        if (bSens )         cpt++;
        else                cpt--;
    }
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void    A4988::step(int delay, int nb)     { 
    for( int i=0; i<nb; i++ )   {
        digitalWrite(STEP, HIGH);
        delayMicroseconds(delay);
        digitalWrite(STEP, LOW);
        delayMicroseconds(delay);
        bStep = false;
    }
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void    A4988::stop()     { 
    digitalWrite(STEP, LOW);
    bStep = false;
}

