//------------------------------------------------------------------------------
//
//
//------------------------------------------------------------------------------
// Liste des commandes
//------------------------------------------------------------------------------
//   Dx  : Change la vitesse par",
//   g   : Affiche les positions de la monture et son etat",
//   j   : Active/Desactive le joystick",
// j0,j1		
//   M   : Mode relatif/absolu",
//   m   : Affiche le mode",
//   n   : stop la monture",
//   p   : active/desactive affichage de la position
//   r   : Reset des valeurs courantes (asc. droite , declinaison)",
//          z1, z2, z3, z4",
//   S   : Active/Desactive le suivi rotation terre",
//   zx    : Coefficient vitesse joystick",
//         1x 1/5x 1/10x 1/20x",
//
//   Vxxx  : Change la viteese siderale",
//   exxx  : Change l exponentielle sur le joystick",
//
//   xxxx   : Emulation joystick en asc. droite de xxx",
//   yxxx   : Emulation joystick en declinaison de xxx",
//    
//   iaxxx   : Valeur courante en asc. droite de xxx degres",
//   idxxx   : Valeur courante en declinaison de xxx degres",
//
//
//-mode relatif
//   axxx    : Deplace la monture de xxx degres en asc. droite",
//   axxxp   : Deplace la monture de xxx pas    en asc. droite",
//
//-mode absolu
//   Axxx    : Deplace la monture de xxx degres en asc. droite",
//   Axxxp   : Deplace la monture de xxx pas    en asc. droite",
//    
//   dxxx    : Deplace la monture de xxx degres en declinaison",
//   dxxxp   : Deplace la monture de xxx pas    en declinaison",
//    
//   v       : Affiche la vitesse siderale",
//   vxxx    : Changement de la vitesse par defaut",
//    
//   sa      : Change sens en ascension droite",
//   sd      : Change le sens en declinaison",
//   sA      : Change sens en ascension droite du joystick ",
//   sD      : Change sens en declinaison du joystick",
//
//   ss      : Change le sens en suivi de rotation",
//------------------------------------------------------------------------------
//                               EQ-TEST.INO
//------------------------------------------------------------------------------
#define PROTO_ARDUINO   1
#include <EEPROM.h>
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
#define PAS_CHERCHE     150
<<<<<<< HEAD
#define VERSION         "1.0.4"
=======
#define VERSION         "1.0.6"
>>>>>>> f326a79 (Suppression des echanges non indispensable)
//------------------------------------------------------------------------------
#define EEPROM_MAGIC0	'#'
#define EEPROM_MAGIC1	'!'
#define EEPROM_VER		1
//------------------------------------------------------------------------------
#define EE_ADR_MAGIC0	0
#define EE_ADR_MAGIC1	1
#define EE_ADR_VER		2
#define EE_ADR_AD		3
#define EE_ADR_DC		4
#define EE_ADR_RETOUR   5
#define EE_ADR_SUIVI    6
//------------_------------------------------------------------------------------
//
// Mesure le 7/5/2018
//---------------------
// rho-bouvier et gama-bouvier
//   p1=199667          p2=236444        delta-pas = 36777
// dec1=30°22'19.3"   dec2=38°18'31.1"   delta-dec = 7.936611°
//
//------------------------------------------------------------------------------
// TODO:
//  Lors du suivi sideral il y a un décalage dans le temps
//  ------------------------------------------------------
// Mesure du 6/8/2018 à 0h46m (Aigre le galiment)
// ---------------------
//  Décalage angulaire mesuré par aladin via la serie de photos de la comete 21P/Giacobini-Zinner  
//  ~/Documents/astronomie/capture/2018-08-06/21P/
//
//
//  1)
//                        ------------
//    decalage mesuré de | 29.25" arc |
//                        ------------
//
//  2)
//    debut : 0h07m46s
//    fin   : 0h12m47s
//                                 ------
//    soit total du temps 5m 1s = | 301s |
//                                 ------
//  3)
//    ve = 0.09717608 "/s
//------------------------------------------------------------------------------
//float   convert =     400000.0;
//
//
//


//#define K_CONV          (1.0/86.5*convert)

#define DELTA_PAS       36777.0
//#define DELTA_PAS       36377.0     // correction manuelle
#define DELTA_DEG       7.936611

float   convert =       DELTA_PAS;
#define K_CONV          (convert/DELTA_DEG)
// K_CONV = 4633,84182493  pas par degree   
// precision := 0° 0' 0.77689"

#define DEG2PAS(__d)    (__d*K_CONV)
#define PAS2DEG(__p)    (__p/(K_CONV))

#define BAUDS           115200
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
A4988   drvDC( pinDC_MS1, pinDC_MS2, pinDC_MS3, pinDC_EN, pinDC_RST, pinDC_STEP, pinDC_DIR );
A4988   drvAD( pinAD_MS1, pinAD_MS2, pinAD_MS3, pinAD_EN, pinAD_RST, pinAD_STEP, pinAD_DIR );
//------------------------------------------------------------------------------
long    cptMili = 0;
long    cptStart = 0;
//------------------------------------------------------------------------------
long    cptDC = 0;
int     vitDC = -1;
long    cptAD = 0;
int     vitAD = -1;
int		uSelPas = 1;
//------------------------------------------------------------------------------
bool    bPrintPos = true   ;
bool    bJoy = false;
bool    bOk = false;
long    countAD = 0;
long    countDC = 0;
long    countADSideral = 0;
long    countDCSideral = 0;
int     defVit = 6;
float   zoom = 1.0;
//------------------------------------------------------------------------------
bool    bSuivi = false;
bool    bSensSideral = false;
float   vitSiderale = 0.0;
float   pasSideral  = 0.0;
float   pulseSideral = 0.0;
//------------------------------------------------------------------------------
bool    bCherche=false;
int     uCherche=0;
int     nbCherche=PAS_CHERCHE;
int     defVitCherche = 240;
//------------------------------------------------------------------------------
bool    bRattrapeJeu=false;
//------------------------------------------------------------------------------
bool    bRattrapage = false;
int     iRat = 0;
int     currentRat = 0;
float   pulseRat = 0.0;
//------------------------------------------------------------------------------
float   E = -3.0;      //exponentiel du joystick
long    timeMiliJoy = 0;
//------------------------------------------------------------------------------
bool	bSimJoy = false;
int		xSimJoy = 512;
int		ySimJoy = 512;
int		xJoy = 512;
int		yJoy = 512;
int     signeJoyAD = 1;
int     signeJoyDC = 1;
#define JOY_NONE      1
#define JOY_DC        2
#define JOY_AD        3

char    lastJoy=JOY_NONE;
bool    stateJoy = false;

#define JEU_JOY         25
//------------------------------------------------------------------------------
void chercheSuivant();
//------------------------------------------------------------------------------
typedef union   f_bool
{
	uint16_t	u;

	struct  	{
		bool bRotAD			: 1;
		bool bRotDC			: 1;
		bool bRotJoyAD		: 1;
		bool bRotJoyDC		: 1;
		bool bRotSiderale	: 1;
		bool bRetStellar	: 1;
		bool bJoy			: 1;
		int	 zero			: 9;
	}b;
};
f_bool		B;
//------------------------------------------------------------------------------
//	Routine d'interruption
//	----------------------
//
//	TIMER3
//	pour le calcul du temps (1ms)
//
//	!!! Attention : (TRES IMPORTANT) les librairies ont été modifiées pour utiliser la nouvelle gestion du temps
//	La variable cpt est utilisé pour USB  voir fichier lib/print.cpp
//-----------------------------------------------problem-------------------------------
ISR(TIMER3_COMPA_vect)										// timer compare interrupt service routine
{
    cptMili++;
    if ( cptMili < 0 )      cptMili = 0;    

	if ( vitDC != -1 || vitAD != -1 ) {
	    if ( !bJoy )    {
	        pulseRat += 1.0;												    //  incremente le var.compteur de temps
            if ( pulseRat >= (pasSideral) ) {
                pulseRat = pulseRat - pasSideral;
                iRat++;
            }
	    }
	}
	//---------------------------------------------------------------------------------
	//
	// DECLINAISON		DC
	//
	//---------------------------------------------------------------------------------
	if ( vitDC != -1 ) {
        if ( !drvDC.getStep() ) {
            if ( cptDC >= (vitDC*uSelPas) ) {
                cptDC = 0;
                drvDC.step(true);
            }
        }
        else {
            if ( cptDC >= (4*uSelPas) ) {
                cptDC = 0;
                drvDC.step(true);
            }
        }
        cptDC++;												    //  incremente le var.compteur de temps
    }
	//---------------------------------------------------------------------------------
	//
	// ASCENTION DROITE   AD
	//
	//---------------------------------------------------------------------------------
	if ( vitAD != -1 ) {
	    if ( !drvAD.getStep() ) {
	        if ( cptAD >= (vitAD*uSelPas) ) {
	            cptAD = 0;
	            drvAD.step(true);
	        }
        }
        else {
            if ( cptAD >= (4*uSelPas) ) {
                cptAD = 0;
                drvAD.step(true);
            }
        }
	    cptAD++;												    //  incremente le var.compteur de temps
	}
	//---------------------------------------------------------------------------------
	//
	//   SUIVI ROTATION TERRE OU RATRAPAGE
	//
	//---------------------------------------------------------------------------------
	else
	if ( bSuivi || bRattrapage )   {
	    //-----------------------------------
	    if ( bRattrapage )   {
	        drvAD.setSens(bSensSideral);
	        if ( !drvAD.getStep() ) {
	            if ( pulseRat >= (defVit+4) ) {
	                pulseRat = 0;
	                drvAD.step(false);
	                currentRat++;
	                if ( currentRat >= iRat )   {
	                    currentRat = 0;
	                    iRat = 0;
	                    bRattrapage = false;
	                }
	            }
            }
            else {
                if ( pulseRat >= 4.0 ) {
                    drvAD.step(false);
                }
            }
	        pulseRat++;												    //  incremente le var.compteur de temps
	    }
	    //-----------------------------------
	    else    {
	        drvAD.setSens(bSensSideral);
	        if ( !drvAD.getStep() ) {
	            if ( pulseSideral >= (pasSideral) ) {
	                pulseSideral = pulseSideral - pasSideral;
	                drvAD.step(false);
	            }
            }
            else {
                if ( pulseSideral >= 4.0 ) {
                    drvAD.step(false);
                }
            }
	        pulseSideral += 1.0;												    //  incremente le var.compteur de temps
        }
	    //-----------------------------------
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
// Ecriture EEPROM EEPROM
// 
//
//------------------------------------------------------------------------------
void writeEEPROM( int adr, char val)
{
	EEPROM.update(adr, val);
	
    Serial.print( "Ecriture EEPROM " );
    Serial.print( adr, HEX );
    Serial.print( ":" );
    Serial.print( val, DEC );
    Serial.println( "" );
}
//------------------------------------------------------------------------------
//
// Ecriture par defaut EEPROM
// 
//
//------------------------------------------------------------------------------
void writeDefaultEEPROM()
{
	EEPROM.write( EE_ADR_MAGIC0, 	EEPROM_MAGIC0 );
	EEPROM.write (EE_ADR_MAGIC1, 	EEPROM_MAGIC1 );
	EEPROM.write( EE_ADR_VER, 		EEPROM_VER );
	EEPROM.write( EE_ADR_AD, 		0 );
	EEPROM.write( EE_ADR_DC, 		0 );
	EEPROM.write( EE_ADR_RETOUR, 	0 );
	EEPROM.write( EE_ADR_SUIVI, 	0 );

    Serial.println( "Init EEPROM" );
}
//------------------------------------------------------------------------------
//
// Read EEPROM
// 
//
//------------------------------------------------------------------------------
void readEEPROM()
{
	char MAGIC0, MAGIC1, VER;

	MAGIC0 	= EEPROM[EE_ADR_MAGIC0];
	MAGIC1 	= EEPROM[EE_ADR_MAGIC1];
	VER		= EEPROM[EE_ADR_VER];

	if 	( 	(MAGIC0 != EEPROM_MAGIC0)
		 || (MAGIC1 != EEPROM_MAGIC1) 
		 || (VER    != EEPROM_VER) 
		)
	{
		writeDefaultEEPROM();
		return;
	}
	
	drvAD.setRot( (EEPROM[EE_ADR_AD]==0) ? false: true );
	drvDC.setRot( (EEPROM[EE_ADR_DC]==0) ? false: true );
	bPrintPos =   (EEPROM[EE_ADR_RETOUR]==0) ? false: true;
	bSuivi    =   (EEPROM[EE_ADR_SUIVI]==0) ? false: true;

	B.b.bRotAD  		= drvAD.getRot();
	B.b.bRotDC 			= drvDC.getRot();
	B.b.bRotSiderale	= bSuivi;
	B.b.bRetStellar		= bPrintPos;
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
    
    /*
    drvDC.selPas2();
    drvAD.selPas2();
	*/

    pinMode( pinJoyX, INPUT);
    pinMode( pinJoyY, INPUT);
    
    cli();
    setupTimer3();
    sei();
    
    vitAD = -1;
    vitDC = -1;

    drvDC.normalRot();
    drvAD.normalRot();
        
    vitSiderale = (360.0/86164.1)-1.9*(0.09717608/3600);
    pasSideral  = 10000.0 / (DEG2PAS(vitSiderale));
  
	drvDC.selPas2();
	drvAD.selPas2();
	
	/*
	defVit = 30	;
	*/
    readEEPROM();
	if ( signeJoyAD == 1 )		B.b.bRotJoyAD = true;
	else						B.b.bRotJoyAD = false;
	if ( signeJoyDC == 1 )		B.b.bRotJoyDC = true;
	else						B.b.bRotJoyDC = false;
    
    B.b.bJoy = bJoy;
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

    sprintf( s, "%c%02dd%02d\'%02d.%02d\"", sign, h, m, se, mi );
    
    return s;
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void DCmoveTo( float deg )  {
        //countDC = drvDC.getCount() + DegtoPas(deg);
        vitDC = defVit;
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
int computeVit2(int x)   {
//#define AFF
    #ifdef AFF
    static long timeJoy = 0;

    timeJoy += cptMili - timeMiliJoy;
    timeMiliJoy = cptMili;
    #endif

    if ( x <JEU_JOY )         return -1;

    #define MAX_VIT 150.0
    #define MIN_VIT 6.0

    float X = x;
    X = X / 512.0;
    float Y = (exp(E*X)-1) / (exp(E)-1);
    float a = (MAX_VIT - MIN_VIT)/(0.0-1.0);
    float b = MAX_VIT;
    float y = a * Y + b;


    #ifdef AFF
    if ( timeJoy >= 10000 )
    {
        timeJoy = 0;
        Serial.print( "vitesse Joy : " );
        Serial.print( x, DEC );
        Serial.print( "=>" );
        Serial.println( y, DEC );
    }
    #endif

    return (int)y;
#undef AFF
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void rattrapeJeuDC(int sign)    {
/*
    Serial.print("Rattrapage de jeu ");
    if ( sign == 1 )        Serial.println("true");
    else                    Serial.println("false");
    */
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
    /*
    Serial.print("Rattrapage de jeu ");
    if ( sign == 1 )        Serial.println("true");
    else                    Serial.println("false");
    */
    int i = sign * 320;
    countAD = drvAD.getCount() + i;
    bRattrapeJeu = true;
    vitAD = 6;
    return;
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
bool bOldSensDC = true;
bool bOldSensAD = true;
void computeJoyDC(int x)    {
    bool bSens;

    x = signeJoyDC * (x-512);

    if ( x>=0 )  {
        bSens = true;
        drvDC.setSens(true);
        if ( bSens != bOldSensDC )  {
            //rattrapeJeuDC(1);
            bOldSensDC = true;
            return;
        }
    }
    else {
        bSens = false;
        drvDC.setSens(false);
        x = -x;
        if ( bSens != bOldSensDC )  {
            //rattrapeJeuDC(-1);
            bOldSensDC = false;
            return;
        }
    }

    vitDC = computeVit2(x);
    
    if ( vitDC == -1 )  {
        drvDC.stop();
        //cptDC = 0;
    }
    else
    {
        if ( vitAD==-1 )        lastJoy = JOY_DC;
	    vitDC *= zoom;
    }
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void computeJoyAD(int y)    {
    bool bSens;
    
    y = signeJoyAD * (y-512);

    if ( y>=0 )  {
        bSens = true;
        drvAD.setSens(true);
        if ( bSens != bOldSensAD )  {
            //rattrapeJeuAD(1);
            bOldSensAD = true;
            return;
        }
    }
    else {
        bSens = false;
        drvAD.setSens(false);
        y = -y;
        if ( bSens != bOldSensAD )  {
            //rattrapeJeuAD(-1);
            bOldSensAD = false;
            return;
        }
    }

    vitAD = computeVit2(y);

    if ( vitAD == -1 )  {
        drvAD.stop();
        //cptAD = 0;
    }
    else
    {
        if ( vitDC==-1 )    lastJoy = JOY_AD;
		vitAD *= zoom;
    }
    
    //else
    //   Serial.println( vitAD, DEC );
    //Serial.println( vitAD, DEC );
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
volatile long lTimeJoy = 0;
volatile long lTimeAnalogPrint = 0;
         long lTimeAnalog_old = 0;
//------------------------------------------------------------------------------
void loopReadJoystick()    {
//#define DEBUG
    long elapsedTime = cptMili - lTimeAnalog_old;
       
    lTimeJoy         += elapsedTime;
    lTimeAnalogPrint += elapsedTime;
    lTimeAnalog_old = cptMili;
    
    if ( lTimeJoy < 2000 )   return;
    lTimeJoy = 0;

    #ifdef DEBUG
    Serial.print( cptMili, DEC );
    Serial.println( " time" );
    #endif

	int x;
	int y;
	if ( bSimJoy )	{
		xJoy = x = xSimJoy;
		yJoy = y = ySimJoy;
	}
	else	{
		xJoy = x = analogRead(pinJoyX);
		yJoy = y = analogRead(pinJoyY);
	}



    #ifdef DEBUG
    Serial.print( "read " );
    Serial.print( cptMili, DEC );
    Serial.print( " x=" );
    Serial.print( x, DEC );
    Serial.print( " y=" );
    Serial.println( y, DEC );
    #endif    

    computeJoyDC(x);
    computeJoyAD(y);

    #ifdef DEBUG
    Serial.print( cptMili, DEC );
    Serial.println( " comp" );
    #endif

//#define DEBUG
    #ifdef DEBUG
    if ( lTimeAnalogPrint > 10000L )
    {
        if ( vitAD != -1 || vitDC != -1 )
        {
            Serial.print( lTimeAnalogPrint, DEC );
            Serial.print( " - JoyXY (" );
            Serial.print( x, DEC );
            Serial.print( ", " );
            Serial.print( y, DEC );
            Serial.print( ") (" );
            Serial.print( vitDC, DEC );
            Serial.print( ", " );
            Serial.print( vitAD, DEC );
            Serial.println( ")" );
            lTimeAnalogPrint = 0;
        }
    }    
    #endif
    //if ( x<(512-JEU_JOY) || x>(512+JEU_JOY) )     computeJoyDC(x);
    //if ( y<(512-JEU_JOY) || y>(512+JEU_JOY) )     computeJoyAD(y);
#undef DEBUG    
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

    if (s[l-1]!='p')        f = DEG2PAS(f);
    
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
    if (signeJoyAD == 1)	Serial.println("normal");
    else                    Serial.println("inverse");
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void printRotJoyDC()  {
    Serial.print("Rotation Joystick Declinaison : " );
    if (signeJoyDC == 1)	Serial.println("normal");
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
    Serial.print("T : " );
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
    Serial.print("deg/s,   Pas sideral : " );
    Serial.print( pasSideral, DEC );
    Serial.println("");
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void printInfo()  {
    long cad, cdc;
    long dest_ad, dest_dc;

    cad = drvAD.getCount();
    cdc = drvDC.getCount();
    dest_ad = countAD;
    dest_dc = countDC;

    Serial.println("= START");
    Serial.print( "V : " );
    Serial.println( VERSION );

    Serial.print( 'W' );
    Serial.write( (uint16_t)B.u );
    Serial.println( "" );

    printInfoTime();
    
    
    Serial.println("= STOP");
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void changeJoy( bool b)  {
    
<<<<<<< HEAD
    if ( bJoy && (yJoy > 1000) )	{
		signeJoyAD *= -1;
	    Serial.println("Change signe AD ");
		return;
    }
    if ( bJoy && (yJoy < 10) )	{
		signeJoyAD *= -1;
	    Serial.println("Change signe AD ...");
		return;
    }
    if ( bJoy && (xJoy > 1000) )	{
		signeJoyDC *= -1;
	    Serial.println("Change signe DC ");
		return;
    }
    if ( bJoy && (xJoy < 10) )	{
		signeJoyDC *= -1;
	    Serial.println("Change signe DC ...");
		return;
    }

    bJoy = b;
=======
    if ( bJoy && ((yJoy > 1000) || (yJoy < 10))	)
    {
		signeJoyAD *= -1;
		if ( signeJoyAD == 1 )		B.b.bRotJoyAD = true;
		else						B.b.bRotJoyAD = false;
	    Serial.println("Change signe AD ");
		return;
    }
    if ( bJoy && ((xJoy > 1000) || (xJoy < 10)) )
    {
		signeJoyDC *= -1;
		if ( signeJoyDC == 1 )		B.b.bRotJoyDC = true;
		else						B.b.bRotJoyDC = false;
	    Serial.println("Change signe DC ");
		return;
    }

    B.b.bJoy = bJoy = b;
>>>>>>> f326a79 (Suppression des echanges non indispensable)
    Serial.print("Change bJoy = ");
    
    if ( bJoy )     Serial.println( "OK" );
    else            Serial.println( "NOK" );
    
    if ( !bJoy )	bSimJoy = false;
    
    bRattrapeJeu = false;
    vitAD = -1;
    vitDC = -1;
    countAD = 0;
    countDC = 0;
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void changeJoy()  {
    changeJoy( !bJoy );
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void changeRotAD()  {
    drvAD.changeRot();
    Serial.print("As droite sens : ");
    if ( drvAD.getRot() )
    {
    	Serial.println("normal");
    	writeEEPROM(EE_ADR_AD, 1);
<<<<<<< HEAD
=======
    	B.b.bRotAD = true;
>>>>>>> f326a79 (Suppression des echanges non indispensable)
    }
    else
    {
    	Serial.println("inverse");
    	writeEEPROM(EE_ADR_AD, 0);
<<<<<<< HEAD
=======
    	B.b.bRotAD = false;
>>>>>>> f326a79 (Suppression des echanges non indispensable)
    }

}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void changeRotDC()  {
    drvDC.changeRot();
    Serial.print("Declinaison sens : ");
    if ( drvDC.getRot() )
    {
    	Serial.println("normal");
    	writeEEPROM(EE_ADR_DC, 1);
<<<<<<< HEAD
=======
    	B.b.bRotDC = true;
>>>>>>> f326a79 (Suppression des echanges non indispensable)
    }
    else
    {
    	Serial.println("inverse");
    	writeEEPROM(EE_ADR_DC, 0);
<<<<<<< HEAD
=======
    	B.b.bRotDC = false;
>>>>>>> f326a79 (Suppression des echanges non indispensable)
    }
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
    //-------------------------------------------------------------------------
    // Changement AD en relatif
    //-------------------------------------------------------------------------
    case 'a':
        if (bJoy)   changeJoy();
        i = deg2pas( &s[1] );

        Serial.print( "AD relatif : " );
        Serial.println( s );
        if ( i != 0 )  {
            countAD = drvAD.getCount() + i;
            setSens( &drvAD, i );

            vitAD = defVit;
            cptStart = cptMili;
            Serial.print("AD : ");
            Serial.print(countAD, DEC );
            Serial.print( "  (");
            Serial.print( PAS2DEG(countAD), DEC );
            Serial.println( ")" );
            iRat = 0;
            pulseRat = 0.0;
        }
        break;
    //-------------------------------------------------------------------------
    // Changement AD en absolu
    //-------------------------------------------------------------------------
    case 'A':
        if (bJoy)   changeJoy();
        i = deg2pas( &s[1] );
        Serial.print( "AD absolu : " );
        Serial.println( s );
        if ( i != 0 )  {
            countAD = i;
            long c = drvAD.getCount();
            if ( i > c )  {
              if ( (i-c)>DEG2PAS(180.0) ) {
                drvAD.setCount(  drvAD.getCount() + DEG2PAS(360.0)  );
                Serial.println( "Correction..." );
              }
            }

            if ( (i-c)<DEG2PAS(-180.0) ) {
              drvAD.setCount(  drvAD.getCount() - DEG2PAS(360.0)  );
              Serial.println( "Correction..." );
            }
            //if ( c > DEG2PAS(180) )   Serial.println( "[*** Superieur ***]") ;

            setSens( &drvAD, (countAD - drvAD.getCount()) );

            vitAD = defVit;
            cptStart = cptMili;
            Serial.print("AD : ");
            Serial.print(countAD, DEC );
            Serial.print( "  (");
            Serial.print( PAS2DEG(countAD), DEC );
            Serial.println( ")" );
            iRat = 0;
            pulseRat = 0.0;
        }
        break;
    //-------------------------------------------------------------------------
    // modifie/affiche la valeur de conversion pas<=>degré
    //-------------------------------------------------------------------------
    case 'c':
        i = getNum(&s[1]);
        if ( i > 20000 )        {
            convert = (float)i;
        }        
        Serial.print("Convertion (pas / deg) : " );
        Serial.print( i, DEC );
        Serial.println("");
        break;
    //-------------------------------------------------------------------------
    // Changement DEC en relatif
    //-------------------------------------------------------------------------
    case 'd':
        if (bJoy)   changeJoy();
        //i = getDegOrPas(&drvDC, &s[1]);
        i = deg2pas( &s[1] );
        Serial.print( "DC relatif : " );
        Serial.println( s );
        if ( i != 0 )  {
            countDC = drvDC.getCount() + i;
            setSens( &drvDC, i );

            vitDC = defVit;
            cptStart = cptMili;
            Serial.print("DC : ");
            Serial.print(countDC, DEC );
            Serial.print( "  (");
            Serial.print( PAS2DEG(countDC), DEC );
            Serial.println( ")" );
            iRat = 0;
            pulseRat = 0.0;
        }
        break;
    //-------------------------------------------------------------------------
    // Changement DEC en absolu
    //-------------------------------------------------------------------------
    case 'D':
        if (bJoy)   changeJoy();
        //i = getDegOrPas(&drvDC, &s[1]);
        i = deg2pas( &s[1] );
        Serial.print( "DC absolu : " );
        Serial.println( s );
        if ( i != 0 )  {
            countDC = i;
            setSens( &drvDC, (i - drvDC.getCount()) );

            vitDC = defVit;
            cptStart = cptMili;
            Serial.print("DC : ");
            Serial.print(countDC, DEC );
            Serial.print( "  (");
            Serial.print( PAS2DEG(countDC), DEC );
            Serial.println( ")" );
            iRat = 0;
            pulseRat = 0.0;
        }
        break;
    //-------------------------------------------------------------------------
    // modifie la valeur de l'expo joystick
    //-------------------------------------------------------------------------
    case 'e':
        f = getFloat(&s[1]);
        if ( i < -100.0 )       break;
        if ( i >  100.0 )       break;
        E = f / 10.0;
        Serial.print( "Exp joystick : " );
        Serial.print( f, DEC );
        Serial.println("\%");
        break;
    //-------------------------------------------------------------------------
    // Initialise position monture
    //-------------------------------------------------------------------------
    case 'i' :
        tt = s[1];
        if ( tt == 'a' )   {
            i = deg2pas( &s[2] );
            drvAD.setCount( i );
            Serial.print("Init Asc. Droite : ");
            //Serial.print( pasToAd(i) );
            //Serial.print( " --- " );
            Serial.print(i, DEC);
            Serial.print( "  (" );
            Serial.print( PAS2DEG(i), DEC );
            Serial.println( ")" );
        }
        else  if ( tt == 'd' )   {
            i = deg2pas( &s[2] );
            drvDC.setCount( i );
            Serial.print("Init Declinaison : ");
            Serial.print(i, DEC);
            Serial.print( "  (" );
            Serial.print( PAS2DEG(i), DEC );
            Serial.println( ")" );
        }
        else    {
            Serial.println("Erreur");
        }
        break;
    //-------------------------------------------------------------------------
    // Joystick
<<<<<<< HEAD
=======
    //-------------------------------------------------------------------------
    case 'j':
        if ( s[1] == 0 )        changeJoy();
        else
        if ( s[1] == '1' )      changeJoy(true);
        else
        if ( s[1] == '0' )      changeJoy(false);
        break;
    //-------------------------------------------------------------------------
    // Affiche les infos
    //-------------------------------------------------------------------------
    case 'g':
        printInfo();
        break;
    //-------------------------------------------------------------------------
    // ARRET D'URGENCE
    //-------------------------------------------------------------------------
    case 'n':
        vitAD = -1;
        vitDC = -1;
        countAD = 0;
        countDC = 0;
        bCherche = false;
        Serial.println("--STOP--");
        break;
    //-------------------------------------------------------------------------
    // Autorise/interdi l'affichage printPos
    //-------------------------------------------------------------------------
    case 'p':
        bPrintPos = !bPrintPos;
        B.b.bRetStellar = bPrintPos;
        Serial.print("PrintPos : ");
        if ( bPrintPos )
        {
        	Serial.println( "OUI" );
        	writeEEPROM( EE_ADR_RETOUR, 1 );
        }
        else
        {
        	Serial.println( "NON" );
        	writeEEPROM( EE_ADR_RETOUR, 0 );
        }
        
        
        break;
    //-------------------------------------------------------------------------
    // Reset valeur pas 
>>>>>>> f326a79 (Suppression des echanges non indispensable)
    //-------------------------------------------------------------------------
    case 'j':
        if ( s[1] == 0 )        changeJoy();
        else
        if ( s[1] == '1' )      changeJoy(true);
        else
        if ( s[1] == '0' )      changeJoy(false);
        break;
    //-------------------------------------------------------------------------
<<<<<<< HEAD
    // Affiche les infos
=======
    // Change le sens des deplacements asc droite (sa), declinaison (sd), rotation siderale (ss)
    // et joystick (sA, sD)
    // commande s[a,d,s,A,D]
    //-------------------------------------------------------------------------
    case 's':
        tt = s[1];
        if ( tt == 'a' )   {
        	changeRotAD();
        }
        else  if ( tt == 'd' )   {
			changeRotDC();
        }
        else  if ( tt == 's' )   {
            bSensSideral = !bSensSideral;
            Serial.print("Rotation siderale ");
            if ( bSensSideral )         Serial.println("horaire");
            else                        Serial.println("antihoraire");
        }
        else  if ( tt == 'A' )   {
            signeJoyAD *= -1;
			if ( signeJoyAD == 1 )		B.b.bRotJoyAD = true;
			else						B.b.bRotJoyAD = false;
            printRotJoyAD();
        }
        else  if ( tt == 'D' )   {
            signeJoyDC *= -1;
			if ( signeJoyDC == 1 )		B.b.bRotJoyDC = true;
			else						B.b.bRotJoyDC = false;
            printRotJoyDC();
        }
        else    {
            Serial.println("Erreur");
        }
        break;
    //-------------------------------------------------------------------------
    // Autorise/interdi le suivi de rotation de la terre (rotation siderale)
    //-------------------------------------------------------------------------
    case 'S':
        bSuivi = !bSuivi;
        B.b.bRotSiderale = bSuivi;
        Serial.print("Suivi rotation terre : ");

        if ( bSuivi )
        {
        	Serial.println( "OUI" );
        	writeEEPROM( EE_ADR_SUIVI, 1 );
        }
        else
        {
        	Serial.println( "NON" );
        	writeEEPROM( EE_ADR_SUIVI, 0 );
        }

        break;
    //-------------------------------------------------------------------------
    // Affiche ou modifie la vitesse de rotation de la terre
>>>>>>> f326a79 (Suppression des echanges non indispensable)
    //-------------------------------------------------------------------------
    case 'g':
        printInfo();
        break;
    //-------------------------------------------------------------------------
    // ARRET D'URGENCE
    //-------------------------------------------------------------------------
<<<<<<< HEAD
    case 'n':
        vitAD = -1;
        vitDC = -1;
        countAD = 0;
        countDC = 0;
        bCherche = false;
        Serial.println("--STOP--");
=======
    case 'V':
        f = getFloat(&s[1]);
        if ( i < 0.0 )        break;
        if ( i > 4000.0 )     break;
        pasSideral = f;
        Serial.print( pasSideral, DEC );
        Serial.println("");
        break;
    //-------------------------------------------------------------------------
    // Simulation du joystick
    //-------------------------------------------------------------------------
    case 'x':
        i = getNum(&s[1]);
        bSimJoy = true;
        if ( i>100)			i = 100;
        if ( i<-100)		i = -100;
        xSimJoy = i * 5 + 512;

        /*
        Serial.print("x_joy=" );
        Serial.print(xSimJoy,DEC);
        Serial.println("");
        */
>>>>>>> f326a79 (Suppression des echanges non indispensable)
        break;
    //-------------------------------------------------------------------------
    // Simulation du joystick
    //-------------------------------------------------------------------------
<<<<<<< HEAD
    case 'p':
        bPrintPos = !bPrintPos;
        Serial.print("PrintPos : ");
        if ( bPrintPos )
        {
        	Serial.println( "OUI" );
        	writeEEPROM( EE_ADR_RETOUR, 1 );
        }
        else
        {
        	Serial.println( "NON" );
        	writeEEPROM( EE_ADR_RETOUR, 0 );
        }
        
        
        break;
    //-------------------------------------------------------------------------
    // Reset valeur pas 
    //-------------------------------------------------------------------------
    case 'r':
        drvDC.resetCount();
        drvAD.resetCount();
        Serial.println("Reset valeur courante");
        break;
    //-------------------------------------------------------------------------
    // Change le sens des deplacements asc droite (sa), declinaison (sd), rotation siderale (ss)
    // et joystick (sA, sD)
    // commande s[a,d,s,A,D]
    //-------------------------------------------------------------------------
    case 's':
        tt = s[1];
        if ( tt == 'a' )   {
        	changeRotAD();
        }
        else  if ( tt == 'd' )   {
			changeRotDC();
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
=======
    case 'y':
        i = getNum(&s[1]);
        bSimJoy = true;
        if ( i>100)			i = 100;
        if ( i<-100)		i = -100;
        ySimJoy = i * 5 + 512;

        /*
        Serial.print("y_joy=" );
        Serial.print(ySimJoy,DEC);
        Serial.println("");
        */
>>>>>>> f326a79 (Suppression des echanges non indispensable)
        break;
    //-------------------------------------------------------------------------
    // Vitesse de deplacement du joystick
    //-------------------------------------------------------------------------
<<<<<<< HEAD
    case 'S':
        bSuivi = !bSuivi;
        Serial.print("Suivi rotation terre : ");

        if ( bSuivi )
        {
        	Serial.println( "OUI" );
        	writeEEPROM( EE_ADR_SUIVI, 1 );
        }
        else
        {
        	Serial.println( "NON" );
        	writeEEPROM( EE_ADR_SUIVI, 0 );
        }

        break;
    //-------------------------------------------------------------------------
    // Affiche ou modifie la vitesse de rotation de la terre
    //-------------------------------------------------------------------------
    case 'v':
        i = getNum(&s[1]);
        if ( i < 0 )        i = -i;
        if ( i > 4000 )     i = 4000;
        if ( i>=6 )         defVit = i;
        
        if ( i != 0 )   {        
            Serial.print("defVit : " );
            Serial.print( defVit, DEC );
            Serial.println("");
        }
        else    {
            Serial.print("defVit : " );
            Serial.print( defVit, DEC );
            Serial.print(" V. siderale : " );
            Serial.print( vitSiderale, DEC );
            Serial.print(" pas sideral : " );
            Serial.print( pasSideral, DEC );
            Serial.println("");
        }
        break;
    //-------------------------------------------------------------------------
    // Affiche ou modifie de la variable pasSideral
    //-------------------------------------------------------------------------
    case 'V':
        f = getFloat(&s[1]);
        if ( i < 0.0 )        break;
        if ( i > 4000.0 )     break;
        pasSideral = f;
        Serial.print( pasSideral, DEC );
        Serial.println("");
        break;
    //-------------------------------------------------------------------------
    // Simulation du joystick
    //-------------------------------------------------------------------------
    case 'x':
        i = getNum(&s[1]);
        bSimJoy = true;
        if ( i>100)			i = 100;
        if ( i<-100)		i = -100;
        xSimJoy = i * 5 + 512;

        /*
        Serial.print("x_joy=" );
        Serial.print(xSimJoy,DEC);
        Serial.println("");
        */
        break;
    //-------------------------------------------------------------------------
    // Simulation du joystick
    //-------------------------------------------------------------------------
    case 'y':
        i = getNum(&s[1]);
        bSimJoy = true;
        if ( i>100)			i = 100;
        if ( i<-100)		i = -100;
        ySimJoy = i * 5 + 512;

        /*
        Serial.print("y_joy=" );
        Serial.print(ySimJoy,DEC);
        Serial.println("");
        */
        break;
    //-------------------------------------------------------------------------
    // Vitesse de deplacement du joystick
    //-------------------------------------------------------------------------
=======
>>>>>>> f326a79 (Suppression des echanges non indispensable)
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
        else if ( i == 5 )        {
            Serial.print("1/40x OK !!" );
            Serial.println("");
            zoom = 400.0;
        }
        else if ( i == 6 )        {
            Serial.print("1/80x OK !!" );
            Serial.println("");
            zoom = 80.0;
        }
        else if ( i == 100 )        {
            Serial.println("SelPas");
			drvDC.selPas2();
			drvAD.selPas2();
			uSelPas = 1;
        }
        else if ( i == 101 )        {
            Serial.println("SelPas2");
			drvDC.selPas2();
			drvAD.selPas2();
			uSelPas = 2;
        }
        else if ( i == 102 )        {
            Serial.println("SelPas4");
			drvDC.selPas4();
			drvAD.selPas4();
			uSelPas = 4;
        }
        else if ( i == 103 )        {
            Serial.println("SelPas8");
			drvDC.selPas8();
			drvAD.selPas8();
			uSelPas = 8;
        }
        else if ( i == 104 )        {
            Serial.println("SelPas16");
			drvDC.selPas16();
			drvAD.selPas16();
			uSelPas = 16;
		}
        else {
            Serial.print("VitAD : ");
            Serial.print(vitAD, DEC);
            Serial.print("   VitDC : ");
            Serial.print(vitDC, DEC);
            Serial.println("");
        }
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
long startMiliBtn = 0L;
void loopBtn() {
//#define DEBUG
    int val;

    static long lTimeButtonDelay = 0;
    static long lTimeButton = 0;
    static long lTimeButtonStartDblc = 0;
    static bool bChangeJoy = false;
    
    lTimeButton          +=  cptMili - startMiliBtn;
    lTimeButtonStartDblc +=  cptMili - startMiliBtn;
    lTimeButtonDelay     +=  cptMili - startMiliBtn;
    
    startMiliBtn = cptMili;
	//--------------------------------------    
    // Anti-rebond
    if ( lTimeButtonDelay < 200 )       return;
    lTimeButtonDelay = 0;

    if ( bChangeJoy && lTimeButtonStartDblc>1000 )
    {
        bChangeJoy = false;
        changeJoy();
        return;
    }

    val = digitalRead( pinBtn );
    
    //if ( !stateJoy && val==LOW )
   
    
    if ( !stateJoy && val==LOW )
    {
        #ifdef DEBUG
        Serial.print( lTimeButton );
        Serial.print( " : bouton " );
        Serial.print( val );
        Serial.println();
        #endif

        if ( lTimeButtonStartDblc < 4000 )
        {
            bChangeJoy = false;
            lTimeButtonStartDblc = 0;
            //if ( lastJoy == JOY_AD )    { signeJoyAD *= -1; printRotJoyAD(); }
            //if ( lastJoy == JOY_DC )    { signeJoyDC *= -1; printRotJoyDC(); }
            changeRotDC();
            Serial.println( "dbl click" );
            bJoy = true;
            B.b.bJoy = bJoy;
        }
        else
        {
            lTimeButtonStartDblc = 0;
            bChangeJoy = true;            //lTimeButtonStartDblc = 0;
            bOk = true;
        }
    }

    if (val == LOW)     stateJoy = true;
    else                stateJoy = false;

//undef DEBUG    
}
//-----------------------------------------------------------------------------
//
// Envoi la position de la monture 10x par seconde
//
//------------------------------------------------------------------------------
long cptPosition = 0;
void printPosition(void)    {
    //if ( vitAD == -1 && vitDC == -1 )       return;

    if ( cptPosition < cptMili )    {
        long cad, cdc;
        cptPosition = cptMili + 500.0;//10000.0;
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
         if (   ( drvDC.getSens() && (drvDC.getCount()+countDCSideral) >= (countDC) )
             || (!drvDC.getSens() && (drvDC.getCount()+countDCSideral) <= (countDC) ) )
        {
            vitDC = -1;
            countDC = 0;
            
            if ( (vitAD == -1) )      {
                t = (cptMili - cptStart)/10000.0;
                if ( !bRattrapeJeu )    {
                    Serial.print("GOTO OK in ");
                    Serial.print( t, DEC);
                    Serial.println("s ");
                    
                    Serial.print("Rattrapage rotation ");
                    Serial.print( iRat, DEC);
                    Serial.println(" pas ");
                    bRattrapage = true;
                    currentRat = 0;
                }
                bRattrapeJeu = false;
            }
            
        }
    }
    if ( vitAD != -1 )    {
        if (   ( drvAD.getSens() && (drvAD.getCount()+countADSideral) >= (countAD) )
            || (!drvAD.getSens() && (drvAD.getCount()+countADSideral) <= (countAD) ) )
        {
            vitAD = -1;
            countAD = 0;
            
            if ( (vitDC == -1) )      {
                t = (cptMili - cptStart)/10000.0;
                if ( !bRattrapeJeu )    {
                    Serial.print("GOTO OK in ");
                    Serial.print( t, DEC );
                    Serial.println("s ");
                    
                    Serial.print("Rattrapage rotation ");
                    Serial.print( iRat, DEC);
                    Serial.println(" pas ");

                    if ( !bJoy )    bRattrapage = true;
                    currentRat = 0;
                }
                bRattrapeJeu = false;
            }
            
        }
    }
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void chercheSuivant()   {
    if ( vitDC!=-1 || vitAD!=-1 )       return;
    
    uCherche++;
    uCherche = uCherche%4;
    
    nbCherche += PAS_CHERCHE;

    switch( uCherche )  {
        case 0:
            vitAD = defVitCherche;
            countAD = drvAD.getCount() + nbCherche;
            setSens( &drvAD, (countAD - drvAD.getCount()) );
            break;
        case 1:
            vitDC = defVitCherche;
            countDC = drvDC.getCount() + nbCherche;
            setSens( &drvDC, (countDC - drvDC.getCount()) );
            break;
        case 2:
            vitAD = defVitCherche;
            countAD = drvAD.getCount() - nbCherche;
            setSens( &drvAD, (countAD - drvAD.getCount()) );
            break;
        case 3:
            vitDC = defVitCherche;
            countDC = drvDC.getCount() - nbCherche;
            setSens( &drvDC, (countDC - drvDC.getCount()) );
            break;
    }
    Serial.println( uCherche, DEC );
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//long c0 = 0;
void loop() {
    if ( Serial.available() )       readCommand();

    if ( bCherche )                 chercheSuivant();

    if ( bJoy && !bRattrapeJeu )    loopReadJoystick();
    else                            computePosition();

    loopBtn();
    if (bPrintPos)                  printPosition();
}

