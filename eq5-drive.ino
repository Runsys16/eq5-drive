//------------------------------------------------------------------------------
//                               EQ-TEST.INO
//------------------------------------------------------------------------------
#ifndef EQ5_DRIVE_CPP
#define EQ5_DRIVE_CPP
#endif
//------------------------------------------------------------------------------
#include "eq5-drive.h"
//------------------------------------------------------------------------------
//	Routine d'interruption
//	----------------------
//
//	TIMER3
//	pour le calcul du temps (1ms)
//
//	!!! Attention : (TRES IMPORTANT) les librairies ont été modifiées pour utiliser la nouvelle gestion du temps
//	La variable cpt est utilisé pour USB  voir fichier lib/print.cpp
//------------------------------------------------------------------------------
ISR(TIMER3_COMPA_vect)										// timer compare interrupt service routine
{
    cptMili++;
    if ( cptMili < 0 )      cptMili = 0;    

    Evenement*  evt;
    evt = listEvenement.getCurrent();
    
    if ( evt == NULL) return;


	if ( evt->getVitDC() != -1 ) {
        if ( !drvDC.getStep() ) {
            if ( cptDC >= (evt->getVitDC()-4) ) {
                cptDC = 0;
                drvDC.step(evt->getDecompte());
            }
        }
        else {
            if ( cptDC >= 4 ) {
                cptDC = 0;
                drvDC.step(evt->getDecompte());
            }
        }
        cptDC++;												    //  incremente le var.compteur de temps
    }

	if ( evt->getVitAD() != -1 ) {
	    if ( !drvAD.getStep() ) {
	        if ( cptAD >= (evt->getVitAD()-4) ) {
	            cptAD = 0;
	            drvAD.step(evt->getDecompte());
	        }
        }
        else {
            if ( cptAD >= 4 ) {
                cptAD = 0;
                drvAD.step(evt->getDecompte());
            }
        }
	    cptAD++;												    //  incremente le var.compteur de temps
	}
}
//------------------------------------------------------------------------------
//	Initialisation du TIMER 3
//	-------------------------
//	Voir doc constructeur (atmega datasheet p 223)
//	
//	Le compteur est utilisé pour caculer le temps
//	Frequence timer : 16.000.000 / 256 / TIMER3  = 1008,064516129 Hz   (TIMER3 = 62)
//	
//	TCNT3 est initialiser à une valeur bidon pour le desynchroniser du TIMER1
//
//  0.1ms   100us
//------------------------------------------------------------------------------
void setupTimer3() {
	TCCR3A = 0;
	TCCR3B = 0;
	TCNT3  = 200;

	OCR3A = TIMER3;										// compare match register 16.000.000/16000 = 1 kHz (1 miliseconde)
	TCCR3B |= (1 << WGM32);								// CTC mode
	TCCR3B |= (1 << CS30);								// no prescaler 
	TIMSK3 |= (1 << OCIE3A);							// enable timer compare interrupt
}
//------------------------------------------------------------------------------
//
// temps sideral moyen 23h56m4s
// = 23.934444444 h = 86164.0s
//
//------------------------------------------------------------------------------
void setup() {
    Serial.begin(BAUDS);

    pinMode( pinBtn,   INPUT_PULLUP );

    drvDC.setup();
    drvAD.setup();

    drvDC.setSens(false);
    drvAD.setSens(false);
    
    drvDC.selPas2();
    drvAD.selPas2();


    pinMode( pinJoyX, INPUT);
    pinMode( pinJoyY, INPUT);
    
    cli();
    setupTimer3();
    sei();
    
    vitAD = -1;
    vitDC = -1;

    //drvDC.inverseRot();
    drvDC.normalRot();

    //drvAD.inverseRot();
    drvAD.normalRot();
    
    
    vitSiderale = (360.0/86164.1)-1.9*(0.09717608/3600);
    vitSiderale = 360.0/86164.1;
    pasSideral  = (10000.0/DEG2PAS(vitSiderale));
    
    // Test sur stellarium pasSideral = 2212
    
    //pasSideral = 10.0 * pasSideral;
  
    //pasSideral = 2212;
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
float dc2deg( String s )   {
    int n = s.indexOf(':', 0 );
    float f[3];
    String st = s;

    for( int i=0; i<3; i++ )    {
        f[i] = st.toFloat();
        n = st.indexOf(':', 0 );
        st = &st[n+1];
    }
    float deg = f[0] + 1/60.0*f[1] + 1/3600.0*f[2];
    return deg;
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
float ad2deg( String s )   {
    int n = s.indexOf(':', 0 );
    float f[3];
    String st = s;

    for( int i=0; i<3; i++ )    {
        f[i] = st.toFloat();
        //Serial.print( st );
        //Serial.print( "-" );
        //Serial.println( f[i], DEC );
        n = st.indexOf(':', 0 );
        st = &st[n+1];
    }
    float deg = 1.0/24.0*360.0*(f[0] + 1.0/60.0*f[1] + 1.0/3600.0*f[2]);
    return deg;
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
long ad2pas( String s )   {
    float f = ad2deg( s);
    long ret = DEG2PAS(f);
    return ret;
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
long dc2pas( String s )   {
    float f = dc2deg( s);
    long ret = DEG2PAS(f);
    return ret;
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
char * pasToAd( float f0 )   {
    //String ret;
    float f = f0;
    char s[16];
    char sign = ' ';
    
    if ( f<0 )  {
        sign = '-';
        f = -f;
    }

    //Serial.println( f, DEC );    
    f = PAS2DEG(f); 
    //Serial.println( f, DEC );    
    f = f/360.0*24.0;
    int h = (int)f;
    
    f = f - (float)h;
    f = f * 60.0;
    //Serial.println( f, DEC );    
    int m = (int)f;

    f = f - (float)m;
    f = f * 60.0;
    //Serial.println( f, DEC );    
    int se = (int)f;

    f = f - (float)se;
    f = f * 100.0;
    //Serial.println( f, DEC );    
    int mi = (int)f;

    sprintf( s, "%c%02dh%02dm%02d.%02ds", sign, h, m, se, mi );
    
    return s;
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
char * pasToDc( float f )   {
    String ret;
    char s[18];
    char sign = ' ';
    
    if ( f<0 )  {
        sign = '-';
        f = -f;
    }

    //Serial.println( f, DEC );    
    f = PAS2DEG(f); 
    //Serial.println( f, DEC );    
    //f = f/360.0*24.0;
    int h = (int)f;
    
    f = f - (float)h;
    f = f * 60.0;
    //Serial.println( f, DEC );    
    int m = (int)f;

    f = f - (float)m;
    f = f * 60.0;
    //Serial.println( f, DEC );    
    int se = (int)f;

    f = f - (float)se;
    f = f * 100.0;
    //Serial.println( f, DEC );    
    int mi = (int)f;

    sprintf( s, "%c%02d°%02d\"%02d.%02d\'", sign, h, m, se, mi );
    
    return s;
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int computeVit(int x)   {
    float f = 550-x;
    float v = 10*(exp((f)/128)-exp(0.0));
    if      (v >500.0 )             v = -1.0;
    else if (v <6.0 )               v = zoom*6.0;
    else {
        v= zoom * v;
    }
    /*
    Serial.print( f, DEC );
    Serial.print( " : " );
    Serial.print( vit, DEC );
    Serial.print( " : " );
    Serial.println( v, DEC );
    */


    return (int)v;
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void rattrapeJeuDC(int sign)    {
    int i = sign * 220;
    countDC = drvDC.getCount() + i;
    bRattrapeJeu = true;
    vitDC = 6;
    return;
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void rattrapeJeuAD(int sign)    {
    int i = sign * 320;
    countAD = drvAD.getCount() + i;
    bRattrapeJeu = true;
    vitAD = 6;
    return;
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void computeJoyDCold(int x)    {
    bool bSens;
    lastJoy = JOY_DC;

    x = signeJoyDC * (x-512);

    if ( x>=0 )  {
        bSens = true;
        drvDC.setSens(true);
        if ( bSens != bOldSensDC )  {
            rattrapeJeuDC(1);
            bOldSensDC = true;
            return;
        }
    }
    else {
        bSens = false;
        drvDC.setSens(false);
        x = -x;
        if ( bSens != bOldSensDC )  {
            rattrapeJeuDC(-1);
            bOldSensDC = false;
            return;
        }
    }

    vitDC = computeVit(x);
    if ( vitDC == -1 )  {
        drvDC.stop();
        //cptDC = 0;
    }
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void computeJoyADold(int y)    {
    bool bSens;
    lastJoy = JOY_AD;
    
    y = signeJoyAD * (y-512);

    if ( y>=0 )  {
        bSens = true;
        drvAD.setSens(true);
        if ( bSens != bOldSensAD )  {
            rattrapeJeuAD(1);
            bOldSensAD = true;
            return;
        }
    }
    else {
        bSens = false;
        drvAD.setSens(false);
        y = -y;
        if ( bSens != bOldSensAD )  {
            rattrapeJeuAD(-1);
            bOldSensAD = false;
            return;
        }
    }

    vitAD = computeVit(y);
    if ( vitAD == -1 )  {
        drvAD.stop();
        //cptAD = 0;
    }
    //else
    //   Serial.println( vitAD, DEC );
    //Serial.println( vitAD, DEC );
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void computeJoyAD(int y)    {
    bool        bSens;
    Evenement*  pEvt;
    long        sign = 1;
    long        currentPas;
    long        ciblePas;
    
    
    lastJoy = JOY_AD;
    currentPas = drvAD.getCount();
    
    pEvt = listEvenement.getCurrent();
    
    if ( pEvt == NULL )  {
        pEvt = new Evenement();
        listEvenement.addfirst(pEvt);
    }
    
    y = signeJoyAD * (y-512);

    if ( y>=0 )     sign = 1;
    else            sign = -1;
    
    if ( !bRelatif )         ciblePas = sign * 10000 + currentPas;
    else                    ciblePas = sign * 10000;

    cptStart = cptMili;
    
    if ( y<0 )      y = -y;
    vitAD = computeVit(y);
    if ( vitAD == -1 )  {
        drvAD.stop();
    }

    pEvt->setPasAD( ciblePas );
    pEvt->setVitAD( vitAD );

    /*
    Serial.print("Asc. droite = ");
    Serial.print(ciblePas, DEC );
    Serial.print(" (");
    Serial.print( PAS2DEG(ciblePas), DEC );
    Serial.print( "°), vitAD = ");
    Serial.print( vitAD, DEC );
    Serial.println( "" );
    */

}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void computeJoyDC(int y)    {
    bool        bSens;
    Evenement*  pEvt;
    long        sign = 1;
    long        currentPas;
    long        ciblePas;
    
    
    lastJoy = JOY_DC;
    currentPas = drvDC.getCount();
    
    pEvt = listEvenement.getCurrent();
    
    if ( pEvt == NULL )  {
        pEvt = new Evenement();
        listEvenement.addfirst(pEvt);
    }
    
    y = signeJoyDC * (y-512);

    if ( y>=0 )     sign = 1;
    else            sign = -1;
    
    if ( !bRelatif )         ciblePas = sign * 10000 + currentPas;
    else                    ciblePas = sign * 10000;

    cptStart = cptMili;
    
    if ( y<0 )      y = -y;
    vitDC = computeVit(y);
    if ( vitDC == -1 )  {
        drvDC.stop();
    }

    pEvt->setPasDC( ciblePas );
    pEvt->setVitDC( vitDC );

    /*
    Serial.print("Declinaison = ");
    Serial.print(ciblePas, DEC );
    Serial.print(" (");
    Serial.print( PAS2DEG(ciblePas), DEC );
    Serial.print( "°), vitDC = ");
    Serial.print( vitDC, DEC );
    Serial.println( "" );
    */
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void loopJostick()    {
    int x = analogRead(pinJoyX);
    int y = analogRead(pinJoyY);
    
    if ( x<(512-JOY_ZONE_NEUTRE) || x>(512+JOY_ZONE_NEUTRE) )     computeJoyDC(x);
    if ( y<(512-JOY_ZONE_NEUTRE) || y>(512+JOY_ZONE_NEUTRE) )     computeJoyAD(y);
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void emuleJoy(int x, int y)    {
    if ( x<0 || x>=1024 )       return;
    if ( y<0 || y>=1024 )       return;
    
    if ( x<(512-JOY_ZONE_NEUTRE) || x>(512+JOY_ZONE_NEUTRE) )     computeJoyDC(x);
    if ( y<(512-JOY_ZONE_NEUTRE) || y>(512+JOY_ZONE_NEUTRE) )     computeJoyAD(y);
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
long getNum( String s )  {
    return s.toInt();
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
float getFloat( String s )  {
    return s.toFloat();
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
long getDegOrPas( A4988* pDrv, String s )  {
    float   f = getFloat(s);
    int     l = s.length();
    long    ret;

    if ( pDrv ) {
        if ( f>=0 )         pDrv->setSens(true);
        else                pDrv->setSens(false);
    }

    if (s[l-1]!='p')        f = f*K_CONV;
    
    ret = round(f);
    return ret;
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
long deg2pas( String s )  {
    float   f = getFloat(s);
    int     l = s.length();
    long    ret;

    if (s[l-1]!='p')        f = f*K_CONV;
    
    ret = round(f);
    return ret;
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void setSens( A4988* pDrv, long l )  {

    if ( l>=0 )         pDrv->setSens(true);
    else                pDrv->setSens(false);
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void computeSens( A4988* pDrv, long l )  {
    if ( pDrv ) {
        if ( l>=0 )         pDrv->setSens(true);
        else                pDrv->setSens(false);
    }
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void printRotJoyAD()  {
    Serial.print("Rotation  Joystick Asc Droite : " );
    if (signeJoyAD == 1)     Serial.println("normal");
    else                    Serial.println("inverse");
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void printRotJoyDC()  {
    Serial.print("Rotation Joystick Declinaison : " );
    if (signeJoyDC == 1)     Serial.println("normal");
    else                    Serial.println("inverse");
}

//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void printInfoOuiNon(bool b)  {
    if (b)          Serial.print("OUI");
    else            Serial.print("NON");
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void printInfoOnOff(bool b)  {
    if (b)          Serial.print("on");
    else            Serial.print("off");
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void printInfoTime()  {
    float f, c;
    float fh, fm, fs;
    int   i, h, m, s;
    Serial.print("Temps de fonctionnement : " );
    c = cptMili / 10000.0 / 3600.0;
    fh = c;
    h = fh;
    Serial.print( h, DEC );
    Serial.print("h " );

    c = fh - (float)h;
    fm = c * 60.0;
    m = fm;
    Serial.print( m, DEC );
    Serial.print("m " );

    c = fm - (float)m;
    fs = c * 60.0;
    s = fs;
    Serial.print( s, DEC );
    Serial.print("s" );

    Serial.println("");
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void printInfoVitsseSiderale()  {
    Serial.print("V. siderale : " );
    Serial.print( vitSiderale, DEC );
    Serial.print(" °/s,   Pas sideral : " );
    Serial.print( pasSideral, DEC );
    Serial.print(" tip/pas" );
    Serial.println("");
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void printInfoPosition()  {
    long cad, cdc;
    long dest_ad, dest_dc;

    cad = drvAD.getCount();
    cdc = drvDC.getCount();

    Evenement *     pEvt = listEvenement.getCurrent();
    
    if ( pEvt != NULL)      {
        dest_ad = pEvt->getPasAD();
        dest_dc = pEvt->getPasDC();
    }
    else    {
        dest_dc = 0;
        dest_ad = 0;
    }



    Serial.println("==================================");
    Serial.print( "Asc Dr : " );
    Serial.print( pasToAd(cad) );
    Serial.print( " , " );
    Serial.print( cad, DEC );
    Serial.print( " (sens " );
    Serial.print( drvAD.getSens(), DEC );

    Serial.print( ") => " );
    Serial.print( " dest = " );
    Serial.print( dest_ad, DEC );
    Serial.println("");

    Serial.print( "Declin : " );
    Serial.print( pasToDc(cdc) );
    Serial.print( " , " );
    Serial.print( cdc, DEC );
    Serial.print( " (sens " );
    Serial.print( drvDC.getSens(), DEC );

    Serial.print( ") => " );
    Serial.print( " dest = " );
    Serial.print( dest_dc, DEC );
    Serial.println( "" );
    
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void printInfoEvt()  {
    Evenement* pEvt;

    Serial.print( "Nb Evenement : " );
    Serial.print( listEvenement.getNb(), DEC );
    Serial.println( "" );
    
    pEvt = listEvenement.getCurrent();
    while( pEvt != NULL )   {
        Serial.print("AD : ");
        Serial.print(pEvt->getPasAD(), DEC );
        Serial.print(", DEC : ");
        Serial.print(pEvt->getPasDC(), DEC );
        Serial.println("");
        
        pEvt = pEvt->suivant;
    }
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void printInfo()  {
    printInfoPosition();

    Serial.println("==================================");
    printInfoEvt();

    Serial.println("==================================");
    Serial.print("Convertion : " );
    Serial.print( DEG2PAS(1), DEC );
    Serial.print( " pas/°  ,  " );
    Serial.print( PAS2DEG(1)*3600.0, DEC );
    Serial.print(" arcsec/pas ");
    Serial.println("");

    printInfoVitsseSiderale();

    Serial.println("==================================");
    Serial.print("Rotation Declinaison : " );
    if (drvDC.getRot())     Serial.println("normal");
    else                    Serial.println("inverse");
    Serial.print("Rotation  Asc Droite : " );
    if (drvAD.getRot())     Serial.println("normal");
    else                    Serial.println("inverse");

    printRotJoyAD();
    printRotJoyDC();
    
    Serial.println("==================================");
    Serial.print( "Rotation terre " );
    printInfoOuiNon( bSuivi );
    Serial.println("");

    Serial.print( "Retour Stellarium " );
    printInfoOnOff( bPrintPos );
    Serial.println("");

    Serial.print( "Joystick " );
    printInfoOnOff( bJoy );
    Serial.println("");

    printInfoTime();
    Serial.println("==================================");
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void changeJoy()  {
    static long cptMiliDbl = -1;
    bJoy = !bJoy;
    Serial.print("Change joy ...");
    if ( bJoy )     Serial.println( "OK" );
    else            Serial.println( "NOK" );
    bRattrapeJeu = false;
    vitAD = -1;
    vitDC = -1;
    countAD = 0;
    countDC = 0;

    //-----------------------------------
    // Gestion du double click    
    long duree = cptMili - cptMiliDbl;

    if ( duree>100 && duree<3000 )    {
        Serial.println( "dbl click");
        bJoy  = true;
        if ( lastJoy == JOY_DC )    {
            signeJoyDC *= -1;
            printRotJoyDC();
        }
        else
        if ( lastJoy == JOY_AD )    {
            signeJoyAD *= -1;
            printRotJoyAD();
        }
    }
    cptMiliDbl = cptMili;

    //-------------------------------------------------------------
    // Si on quitte le joystick effacement des evenements en cours 
    if ( !bJoy )    {
        listEvenement.reset();
    }   
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void printMode()  {
    Serial.print("Mode ");
    if ( bRelatif )         Serial.println("relatif !");
    else                    Serial.println("absolu !");
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void changeMode()  {
    bRelatif = !bRelatif;
    Serial.print("Nouveau mode : ");
    if ( bRelatif )         Serial.println("relatif !");
    else                    Serial.println("absolu !");
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void loopBtn() {
    int val;

    btnMili++;
    if ( btnMili < 100 )      return;
    
    btnMili = 0;
    val = digitalRead( pinBtn );
    if ( (val == HIGH) && bOk)   {
        changeJoy();
        if ( bJoy )         bStop = true;
        bOk = false;
    }
    if (val == LOW)     bOk = true;
}
//-----------------------------------------------------------------------------
//
// Envoi la position de la monture 5x par seconde
//
//------------------------------------------------------------------------------
void printPosition(void)    {
    //if ( vitAD == -1 && vitDC == -1 )       return;

    if ( cptPosition < cptMili )    {
        long cad, cdc;
        cptPosition = cptMili + 2000.0;//10000.0;
        cad = drvAD.getCount();
        cdc = drvDC.getCount();
        Serial.print("-a");
        Serial.println( PAS2DEG(cad), DEC );
        Serial.print("-d");
        Serial.println( PAS2DEG(cdc), DEC );
    }

}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void sensEvenement(Evenement * pEvt) {
    if (pEvt==NULL )  return;

    if (pEvt->getVitAD() != -1)             setSens(&drvAD, pEvt->getPasAD()-drvAD.getCount());        
    if (pEvt->getVitDC() != -1)             setSens(&drvDC, pEvt->getPasDC()-drvDC.getCount());
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void computePositionSuivant() {
    Evenement *     pEvt = listEvenement.getCurrent();
    
    listEvenement.sup(pEvt);
    pEvt = listEvenement.getCurrent();
    
    sensEvenement(pEvt);
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void printGOTOend(Evenement* pEvt) {
    if (!pEvt->getPrintGOTO())       return;
    float t;
    
    t = (cptMili - cptStart)/10000.0;
    
    Serial.print("GOTO OK in ");
    Serial.print( t, DEC);
    Serial.println("s ");

    cptStart = cptMili;
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void computePosition() {

    Evenement *     pEvt = listEvenement.getCurrent();
    
    if ( pEvt == NULL)      {
        bStop = true;
        return;
    }
    
    if (bStop)   {
        sensEvenement(pEvt);
    }

    if ( pEvt->getVitDC() != -1 )    {
        bStop = false;

         if (   ( drvDC.getSens() && (drvDC.getCount()+countDCSideral) >= (pEvt->getPasDC()) )
             || (!drvDC.getSens() && (drvDC.getCount()+countDCSideral) <= (pEvt->getPasDC()) ) )
        {
            pEvt->setVitDC(-1);
            
            if ( (pEvt->getVitAD() == -1) )      {
                printGOTOend(pEvt);
                computePositionSuivant();
            }
        }
    }
    if ( pEvt->getVitAD() != -1 )    {
        bStop = false;

        if (   ( drvAD.getSens() && (drvAD.getCount()+countADSideral) >= (pEvt->getPasAD()) )
            || (!drvAD.getSens() && (drvAD.getCount()+countADSideral) <= (pEvt->getPasAD()) ) )
        {
            pEvt->setVitAD(-1);

            if ( (pEvt->getVitDC() == -1) )      {
                printGOTOend(pEvt);
                computePositionSuivant();
            }
            
        }
    }
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void printSuivant( long i, long count)   {
        Serial.print("Evt n° ");
        Serial.print(i, DEC);
        Serial.print("  count : ");
        Serial.print(count, DEC);
        Serial.println("");
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void chercheSuivant(int nb)   {
    long ca;
    long cd;
    long PAS = 250;
    long sign = 1;
    Evenement* pEvt;
    volatile long count;
    int  vit;
    
    ca = drvAD.getCount();
    cd = drvDC.getCount();
    
    vit = defVit * 20;
    
    if (nb< 2 )         nb = 2;
    
    nb++;
        
    for( long i=1; i<nb; i++ )    {
        count = sign * i * PAS;

        ca += count;
        setSens( &drvAD, count );
        pEvt = new Evenement(ca,0, vit, -1, true);        
        listEvenement.addend(pEvt);
        //printSuivant(i*2-1, ca);

        cd += count;
        setSens( &drvDC, count );
        pEvt = new Evenement(0, cd, -1, vit, true);        
        listEvenement.addend(pEvt);
        //printSuivant(i*2, cd);

        sign = sign * -1;
    }
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void computeSuivi()  {
    if ( pulseSideral >= pasSideral )   {
        pulseSideral = pulseSideral - pasSideral;
        
        long countAD = drvAD.getCount() + (bSensSideral ? 1 : -1);
        Evenement* pEvt = new Evenement(countAD, 0, defVit, -1, true, false);
        //Serial.println(countAD, DEC);
        listEvenement.addfirst(pEvt);
        sensEvenement(pEvt);
    }
    long avance = 0;
    if ( cptMiliSuivi!=0)   {
        avance = cptMili - cptMiliSuivi;
        cptMiliSuivi = cptMili;
    }
    else    {
        cptMiliSuivi = cptMili;
    }
    pulseSideral += avance;
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void computeCible( String s, A4988 * pDrv )   {
    long i;
    
    if (bJoy)   changeJoy();
    
    i = deg2pas( s );
    
    if ( i != 0 )  {
        Evenement * pEvt;
        pEvt = listEvenement.getLast();
        if ( !bCmdMultiple )    {
            pEvt = new Evenement();
            listEvenement.addend(pEvt);
        }

        long currentPas, ciblePas;
        currentPas = pDrv->getCount();
        
        if ( bRelatif )         ciblePas = i + currentPas;
        else                    ciblePas = i;

        if ( pDrv == &drvAD )   {
            pEvt->setPasAD( ciblePas );
            pEvt->setVitAD( defVit );
        }
        else if ( pDrv == &drvDC )  {
            pEvt->setPasDC( ciblePas );
            pEvt->setVitDC( defVit );
        }

        cptStart = cptMili;
        
        Serial.print("Asc. droite : ");
        Serial.print(ciblePas, DEC );
        Serial.print( ", ");
        Serial.println( PAS2DEG(ciblePas), DEC );
    }
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void decodeCmd( String s)  {
    char cmd = s[0];
    char tt;
    long i, cad, cdc;
    float f;
    
    switch( cmd )   {
    case 'j':
        changeJoy();
        break;
    case 'x':
        i = getNum(&s[1]);
        emuleJoy( i, 512);
        break;
    case 'y':
        i = getNum(&s[1]);
        emuleJoy( 512, i);
        break;
    case 'g':
        printInfo();
        break;
    case 'd':
        computeCible( &s[1], &drvDC );
        break;
    case 'a':
        computeCible( &s[1], &drvAD );
        break;
    case 'm':
        printMode();
        break;
    case 'M':
        changeMode();
        break;
    case 'n':
        listEvenement.reset();
        Serial.println("--STOP--");
        break;
    case 'i' :
        tt = s[1];
        if ( tt == 'a' )   {
            i = deg2pas( &s[2] );
            drvAD.setCount( i );
            Serial.print("Init Asc. Droite : ");
            //Serial.print( pasToAd(i) );
            //Serial.print( " --- " );
            Serial.println(i, DEC);
        }
        else  if ( tt == 'd' )   {
            i = deg2pas( &s[2] );
            drvDC.setCount( i );
            Serial.print("Init Declinaison : ");
            Serial.println(i, DEC);
        }
        else    {
            Serial.println("Erreur");
        }
        break;
    case 'r':
        drvDC.resetCount();
        drvAD.resetCount();
        Serial.println("Reset valeur courante");
        break;
    case 'v':
        i = getNum(&s[1]);
        pasSideral = i;
        printInfoVitsseSiderale();
        break;
        if ( i < 0 )        i = -i;
        if ( i > 4000 )     i = 4000;
        if ( i>=6 )         defVit = i;
        
        if ( i != 0 )   {        
            Serial.print("Vitesse : " );
            Serial.print( defVit, DEC );
            Serial.println("");
        }
        else    {
            Serial.print("V. siderale : " );
            Serial.print( vitSiderale, DEC );
            Serial.print(" pas sideral : " );
            Serial.print( pasSideral, DEC );
            Serial.println("");
        }
        break;
    case 'c':
        i = getNum(&s[1]);
        if ( i > 5000 )        {
            convert = (float)i;
        }        
        Serial.print("Convertion (pas / deg) : " );
        Serial.print( i, DEC );
        Serial.println("");
        break;
    case 'p':
        bPrintPos = !bPrintPos;
        Serial.print("PrintPos : ");
        if ( bPrintPos )        Serial.println( "OUI" );
        else                    Serial.println( "NON" );
        break;
    case 's':
        tt = s[1];
        if ( tt == 'a' )   {
            drvAD.changeRot();
            Serial.println("As droite sens: ");
        }
        else  if ( tt == 'd' )   {
            drvDC.changeRot();
            Serial.println("Declinaison sens: ");
        }
        else  if ( tt == 's' )   {
            bSensSideral = !bSensSideral;
            Serial.print("Rotation siderale ");
            if ( bSensSideral )         Serial.println("horaire");
            else                        Serial.println("antihoraire");
        }
        else  if ( tt == 'A' )   {
            signeJoyAD *= -1;
            printRotJoyAD();
        }
        else  if ( tt == 'D' )   {
            signeJoyDC *= -1;
            printRotJoyDC();
        }
        else    {
            Serial.println("Erreur");
        }
        break;
    case 'S':
        bSuivi = !bSuivi;
        Serial.print("Suivi rotation terre : ");
        if ( bSuivi )       Serial.println( "OUI" );
        else                Serial.println( "NON" );

        if ( bSuivi )       cptMiliSuivi = 0;
        break;
    case 'C':
        i = getNum(&s[1]);
        chercheSuivant(i);
        break;
    case 'z':
        i = getNum(&s[1]);
        if ( i == 1 )        {
            Serial.print("1x OK !!" );
            Serial.println("");
            zoom = 1.0;
        }        
        else if ( i == 2 )        {
            Serial.print("1/5x OK !!" );
            Serial.println("");
            zoom = 5.0;
        }        
        else if ( i == 3 )        {
            Serial.print("1/10x OK !!" );
            Serial.println("");
            zoom = 10.0;
        }        
        else if ( i == 4 )        {
            Serial.print("1/20x OK !!" );
            Serial.println("");
            zoom = 20.0;
        }        
        break;
    }
}

//-----------------------------------------------------------------------------
//
//
//
//------------------------------------------------------------------------------
void readCommand()  {
    String s = Serial.readString();
    s[s.length()-1] = 0;

    bCmdMultiple = false;
    while( 1 )  {
        int n = s.indexOf(';', 0 );
        if ( n == -1)   {
            decodeCmd(s);
            break;
        }
        s[n] = 0;
        decodeCmd(s);
        s = &s[n+1];
        bCmdMultiple = true;
    }
} 
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//long c0 = 0;
void loop() {
    if ( Serial.available() )       readCommand();
    
    if ( bJoy && !bRattrapeJeu )    loopJostick();
    else                            computePosition();

    loopBtn();
    
    if (bPrintPos)                  printPosition();
    if (bSuivi)                     computeSuivi();
}

