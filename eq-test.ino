//------------------------------------------------------------------------------
//                               EQ-TEST.INO
//------------------------------------------------------------------------------
#include "port.h"
#include "A4988.h"
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
#define DELAY           400
#define NB              100
#define TIMER3          16000
#define TIMER3          1600        //0.1ms
//------------------------------------------------------------------------------
//
// Mesure le 7/5/2018
//---------------------
// rho-bouvier et gama-bouvier
//   p1=199667          p2=236444        delta-pas = 36777
// dec1=30°22'19.3"   dec2=38°18'31.1"   delta-dec = 7.936611°
//
//------------------------------------------------------------------------------
//float   convert =     400000.0;
//#define K_CONV          (1.0/86.5*convert)

float   convert =       36777.0;
#define K_CONV          (1.0/7.936611*convert)

#define DEG2PAS(d)      (d*K_CONV)
#define PAS2DEG(d)      (d/(K_CONV))
#define BAUDS           115200
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
A4988   drvDC( pinDC_MS1, pinDC_MS2, pinDC_MS3, pinDC_EN, pinDC_RST, pinDC_STEP, pinDC_DIR );
A4988   drvAD( pinAD_MS1, pinAD_MS2, pinAD_MS3, pinAD_EN, pinAD_RST, pinAD_STEP, pinAD_DIR );
//------------------------------------------------------------------------------
long    cptMili = 0;
long    btnMili = 0;
long    cptStart = 0;
//------------------------------------------------------------------------------
long    cptDC = 0;
int     vitDC = -1;
long    cptAD = 0;
int     vitAD = -1;
//------------------------------------------------------------------------------
bool    bPrintPos = false;
bool    bJoy = false;
bool    bOk = false;
bool    bRelatif = false;
long    countAD = 0;
long    countDC = 0;
long    countADSideral = 0;
long    countDCSideral = 0;
int     defVit = 6;
//------------------------------------------------------------------------------
bool    bSuivi = false;
bool    bSensSideral = false;
float   vitSiderale = 0.0;
float   pasSideral  = 0.0;
float   pulseSideral = 0.0;
//------------------------------------------------------------------------------
int     signeJoyAD = 1;
int     signeJoyDC = 1;
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
	if ( vitDC != -1 ) {
        if ( !drvDC.getStep() ) {
            if ( cptDC >= vitDC ) {
                cptDC = 0;
                drvDC.step();
            }
        }
        else {
            if ( cptDC >= 4 ) {
                cptDC = 0;
                drvDC.step();
            }
        }
        cptDC++;												    //  incremente le var.compteur de temps
    }

	if ( vitAD != -1 ) {
	    if ( !drvAD.getStep() ) {
	        if ( cptAD >= vitAD ) {
	            cptAD = 0;
	            drvAD.step();
	        }
        }
        else {
            if ( cptAD >= 4 ) {
                cptAD = 0;
                drvAD.step();
            }
        }
	    cptAD++;												    //  incremente le var.compteur de temps
	}
	//   Suivi rotation la terre
	else if ( bSuivi )   {
	    drvAD.setSens(bSensSideral);
	    if ( !drvAD.getStep() ) {
	        if ( pulseSideral >= (pasSideral) ) {
	            pulseSideral = pulseSideral - pasSideral;
	            //cptAD = 0;
	            drvAD.step();
	        }
        }
        else {
            if ( pulseSideral >= 4.0 ) {
                //cptAD = 0;
                drvAD.step();
                if ( drvAD.getSens() )      drvAD.decStep();
                else                        drvAD.incStep();
            }
        }
	    pulseSideral += 1.0;												    //  incremente le var.compteur de temps
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

    pinMode( pinBut0,  INPUT_PULLUP );
    pinMode( pinBut1,  INPUT_PULLUP );

    pinMode( pinBtn,   INPUT_PULLUP );

    drvDC.setup();
    drvAD.setup();

    drvDC.setSens(true);
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

    drvDC.inverseRot();
    //drvDC.normalRot();
    drvAD.normalRot();
    
    
    vitSiderale = DEG2PAS(360.0/86164.1);
    pasSideral  = 10000.0 / vitSiderale;
    //pasSideral = 10;
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
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void DCmoveTo( float deg )  {
        //countDC = drvDC.getCount() + DegtoPas(deg);
        vitDC = defVit;
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void loopTestJoy()  {
    int x = analogRead(pinJoyX);
    int y = analogRead(pinJoyY);

    x = x/100 - 5;
    y = y/100 - 5;

    if ( x>=0 )  {
        drvDC.setSens(true);
    }
    else {
        drvDC.setSens(false);
        x = -x;
    }

    switch( x )   {
        case 5:
            drvDC.step(DELAY, NB);
            break;
        case 4:
            drvDC.step((float)(DELAY*1.5), NB);
            break;
        case 3:
            drvDC.step(DELAY*2, NB);
            break;
        case 2:
            drvDC.step((float)(DELAY*2.5), NB);
            break;
        case 1:
            drvDC.step(DELAY*3, NB);
            break;
    }

    if ( y>=0 )  {
        drvAD.setSens(false);
    }
    else {
        drvAD.setSens(true);
        y = -y;
    }

    
    switch( y )   {
    
        case 5:
            drvAD.step(DELAY, NB);
            break;
        case 4:
            drvAD.step((float)(DELAY*1.5), NB);
            break;
        case 3:
            drvAD.step((float)(DELAY*2), NB);
            break;
        case 2:
            drvAD.step((float)(DELAY*2.5), NB);
            break;
        case 1:
            drvAD.step(DELAY*3, NB);
            break;
    }
    /*
    float d = (float)DELAY;
    d = (5.0-(float)y)*d;
    
    drvAD.step((int)d, NB);
    */
    /*
    Serial.print( x, DC );
    Serial.print( " " );
    Serial.println( y, DC );
    */
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int computeVit(int x)   {
    int vit;
    if ( x <20 )                 vit = -1;
    else if ( x <50 )            vit = 200;
    else if ( x <100 )           vit = 94;
    else if ( x <150 )           vit = 57;
    else if ( x <200 )           vit = 39;
    else if ( x <250 )           vit = 28;
    else if ( x <300 )           vit = 20;
    else if ( x <350 )           vit = 15;
    else if ( x <400 )           vit = 11;
    else if ( x <450 )           vit = 8;
    else if ( x <500 )           vit = 7;
    else if ( x <550 )           vit = 6;
    return vit;
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void computeDC()    {
    int x = analogRead(pinJoyX);

    x = signeJoyDC * (x-512);

    if ( x>=0 )  {
        drvDC.setSens(true);
    }
    else {
        drvDC.setSens(false);
        x = -x;
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
void computeAD()    {
    int y = analogRead(pinJoyY);
    
    y = signeJoyAD * (y-512);

    if ( y>=0 )  {
        drvAD.setSens(true);
    }
    else {
        drvAD.setSens(false);
        y = -y;
    }

    vitAD = computeVit(y);
    if ( vitAD == -1 )  {
        drvAD.stop();
        //cptAD = 0;
    }
    //Serial.println( cptAD, DEC );
    //Serial.println( vitAD, DEC );
}
//-----------------------------------------------------------------------------
//
// Aller retour en Y en moins de 200ms
//
//------------------------------------------------------------------------------
long startMili = -1;
void cmdJoy()    {
    int y = analogRead(pinJoyY);
    if ( startMili == -1 && y>1000 )    {
        startMili = cptMili;
    }

    if ( startMili != -1 && y<20 )    {
        startMili = cptMili - startMili;
        //Serial.print( cptMili - startMili, DEC );
        //Serial.println( "" );
        if ( startMili < 1800 )       {
            bJoy = !bJoy;
            Serial.println( "cmdJoy() ..." );
        }    
        startMili = -1;
    }
    //Serial.println( y, DEC );
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void loopTestTimerDC()    {
    computeDC();
    computeAD();
    
    /*
    //Serial.print( analogRead(pinJoyY), DEC );
    Serial.print( "  " );
    Serial.print( vitDC, DEC );
    Serial.print( "  " );
    Serial.print( vitAD, DEC );
    Serial.print( "" );
    */    
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
void printInfo()  {
    long cad, cdc;

    cad = drvAD.getCount();
    cdc = drvDC.getCount();
    Serial.print( " Asc D : " );
    Serial.print( pasToAd(cad) );
    Serial.print( " , " );
    Serial.print( cad, DEC );
    Serial.print( " -" );
    Serial.print( drvDC.getSens(), DEC );
    Serial.print( "- Decl : " );
    Serial.print( pasToDc(cdc) );
    Serial.print( " , " );
    Serial.print( cdc, DEC );
    Serial.print( " -" );
    Serial.print( drvAD.getSens(), DEC );

    cad = countAD;
    cdc = countDC;
    Serial.print( "- COUNT " );
    Serial.print( " a.d=" );
    Serial.print( cad, DEC );
    Serial.print( " dec=" );
    Serial.print( cdc, DEC );
    Serial.println( "" );
    Serial.print("Convertion (pas / deg) : " );
    Serial.print( convert, DEC );
    Serial.print("   pas/° ");
    Serial.print( K_CONV, DEC );
    Serial.println("");

    Serial.print("Pas sideral : ");
    Serial.print( pasSideral, DEC );
    Serial.println("");

    Serial.print("Pulse sideral : ");
    Serial.print( pulseSideral, DEC );
    Serial.println("");

    Serial.print("Rotation Declinaison : " );
    if (drvDC.getRot())     Serial.println("normal");
    else                    Serial.println("inverse");
    Serial.print("Rotation  Asc Droite : " );
    if (drvAD.getRot())     Serial.println("normal");
    else                    Serial.println("inverse");

    printRotJoyAD();
    printRotJoyDC();
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void changeJoy()  {
    bJoy = !bJoy;
    Serial.print("Change joy ...");
    if ( bJoy )     Serial.println( "OK" );
    else            Serial.println( "NOK" );
    vitAD = -1;
    vitDC = -1;
    countAD = 0;
    countDC = 0;
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
// 400000 pas = 86.5 deg
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
    case 'g':
        printInfo();
        break;
    case 'd':
        if (bJoy)   changeJoy();
        //i = getDegOrPas(&drvDC, &s[1]);
        i = deg2pas( &s[1] );
        if ( i != 0 )  {
            if ( bRelatif ) {
                countDC = drvDC.getCount() + i;
                setSens( &drvDC, i );
            }
            else    {
                countDC = i;
                setSens( &drvDC, (i - drvDC.getCount()) );
            }
            vitDC = defVit;
            cptStart = cptMili;
            Serial.print("Declinaison : ");
            Serial.print(countDC, DEC );
            Serial.print( ", ");
            Serial.println( PAS2DEG(countDC), DEC );
        }
        break;
    case 'a':
        if (bJoy)   changeJoy();
        i = deg2pas( &s[1] );
        if ( i != 0 )  {
            if ( bRelatif ) {
                countAD = drvAD.getCount() + i;
                setSens( &drvAD, i );
            }
            else    {
                countAD = i;
                setSens( &drvAD, (i - drvAD.getCount()) );
            }
            vitAD = defVit;
            cptStart = cptMili;
            Serial.print("Asc. droite : ");
            Serial.print(countAD, DEC );
            Serial.print( ", ");
            Serial.println( PAS2DEG(countAD), DEC );
        }
        break;
    case 'A':
        i = ad2pas( &s[1]);
        Serial.println( i, DEC );
        f = ad2deg( &s[1]);
        Serial.println( f, DEC );
        if ( bRelatif ) {
            countAD = drvAD.getCount() + i;
            setSens( &drvAD, i );
        }
        else    {
            countAD = i;
            setSens( &drvAD, (i - drvAD.getCount()) );
        }
        vitAD = defVit;
        cptStart = cptMili;
        break;
    case 'D':
        i = dc2pas( &s[1]);
        Serial.println( i, DEC );
        f = dc2deg( &s[1]);
        Serial.println( f, DEC );
        if ( bRelatif ) {
            countDC = drvDC.getCount() + i;
            setSens( &drvDC, i );
        }
        else    {
            countDC = i;
            setSens( &drvDC, (i - drvDC.getCount()) );
        }
        vitDC = defVit;
        cptStart = cptMili;
        break;
/*
    case 'A':
        i = getDegOrPas(NULL, &s[1]);
        if ( i!=0 ) {
            countAD = drvAD.getCount() + i;
            countDC = drvDC.getCount() + i;
            vitAD = defVit;
            vitDC = defVit;
            Serial.print("Asc. droite + Declinaison : ");
            Serial.println( i, DEC );
        }
        break;
*/
    case 'm':
        printMode();
        break;
    case 'M':
        changeMode();
        break;
    case 'n':
        vitAD = -1;
        vitDC = -1;
        countAD = 0;
        countDC = 0;
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
        if ( bPrintPos )       Serial.println( "OUI" );
        else                Serial.println( "NON" );
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
        break;
    }
}
//-----------------------------------------------------------------------------
// 400000 pas = 86.5 deg
//------------------------------------------------------------------------------
void readCommand()  {
    String s = Serial.readString();
    s[s.length()-1] = 0;
    while( 1 )  {
        int n = s.indexOf(';', 0 );
        if ( n == -1)   {
            decodeCmd(s);
            break;
        }
        s[n] = 0;
        decodeCmd(s);
        s = &s[n+1];
    }
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
        bOk = false;
    }
    if (val == LOW)     bOk = true;
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
long cptPosition = 0;
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
//long c0 = 0;
void computePosition() {
    float t;

    if ( vitDC != -1 )    {
         if (   ( drvDC.getSens() && drvDC.getCount() >= (countDC + countDCSideral) )
             || (!drvDC.getSens() && drvDC.getCount() <= (countDC + countDCSideral) ) )
        {
            vitDC = -1;
            countDC = 0;
            
            if ( (vitAD == -1) )      {
                t = (cptMili - cptStart)/10000.0;
                Serial.print("GOTO OK in ");
                Serial.print( t, DEC);
                Serial.println("s ");
            }
            
        }
    }
    if ( vitAD != -1 )    {
        if (   ( drvAD.getSens() && drvAD.getCount() >= (countAD + countADSideral) )
            || (!drvAD.getSens() && drvAD.getCount() <= (countAD + countADSideral) ) )
        {
            vitAD = -1;
            countAD = 0;
            
            if ( (vitDC == -1) )      {
                t = (cptMili - cptStart)/10000.0;
                Serial.print("GOTO OK in ");
                Serial.print( t, DEC );
                Serial.println("s ");
            }
            
        }
    }
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//long c0 = 0;
void loop() {
    if ( Serial.available() )       readCommand();

    if ( bJoy )                     loopTestTimerDC();
    else                            computePosition();

    loopBtn();
    if (bPrintPos)                  printPosition();
}

