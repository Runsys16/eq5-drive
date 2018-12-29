//------------------------------------------------------------------------------
//                              eq5-drive.h
//------------------------------------------------------------------------------
#ifndef EQ5_DRIVE_H
#define EQ5_DRIVE_H
//------------------------------------------------------------------------------
#define PROTO_ARDUINO   1
#include "port.h"
#include "A4988.h"
#include "evenement.h"
#include "evenementList.h"
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
#define DELAY           400
#define NB              100
#define TIMER3          16000
#define TIMER3          1600        //0.1ms
//------------------------------------------------------------------------------
#define PAS_CHERCHE     1500
#define JOY_ZONE_NEUTRE 25

//------------------------------------------------------------------------------
#define JOY_NONE      1
#define JOY_DC        2
#define JOY_AD        3
//------------------------------------------------------------------------------
// Premiere mesure
//-----------------aa
//  400000 pas = 86.5 deg
//
// Mesure le 7/5/2018
//---------------------
// rho-bouvier et gama-bouvier
//   p1=199667          p2=236444        delta-pas = 36777
// dec1=30°22'19.3"   dec2=38°18'31.1"   delta-dec = 7.936611°
//
// resolution : 4633,84182493 pas/° <=> 0,776893156 arcsec/pas <=> 0,000215804 °/pas
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
//
// Nouvelle mesure sur stellarium 25-08-2018
//---------------------------------
//  29.11 arcsec   en 15m38.98s = 938.98
// ve = 0.03100172527636 arcsec/s
// il faut accelere le deplacement
//------------------------------------------------------------------------------
#define DELTA_PAS       36777.0
//#define DELTA_PAS       36377.0     // correction manuelle
#define DELTA_DEG       7.936611

#define K_CONV          (convert/DELTA_DEG)

#define DEG2PAS(__d)    (__d*K_CONV)
#define PAS2DEG(__p)    (__p/(K_CONV))
#define BAUDS           115200
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
#ifdef EQ5_DRIVE_CPP
//------------------------------------------------------------------------------
A4988   drvDC( pinDC_MS1, pinDC_MS2, pinDC_MS3, pinDC_EN, pinDC_RST, pinDC_STEP, pinDC_DIR );
A4988   drvAD( pinAD_MS1, pinAD_MS2, pinAD_MS3, pinAD_EN, pinAD_RST, pinAD_STEP, pinAD_DIR );
//------------------------------------------------------------------------------
float   convert =       DELTA_PAS;
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
bool    bPrintPos = false   ;
bool    bJoy = false;
bool    bOk = false;
bool    bRelatif = false;
long    countAD = 0;
long    countDC = 0;
long    countADSideral = 0;
long    countDCSideral = 0;
int     defVit = 10;
float   zoom = 1.0;
//------------------------------------------------------------------------------
bool    bSuivi = false;
long    cptMiliSuivi = 0;
//------------------------------------------------------------------------------
bool    bSensSideral = false;
float   vitSiderale = 0.0;
float   pasSideral  = 0.0;
float   pulseSideral = 0.0;
//------------------------------------------------------------------------------
int     signeJoyAD = -1;
int     signeJoyDC = 1;
//------------------------------------------------------------------------------
bool    bCherche=false;
int     uCherche=0;
int     nbCherche=PAS_CHERCHE;
//------------------------------------------------------------------------------
bool    bRattrapeJeu=false;
//------------------------------------------------------------------------------
int     lastJoy=JOY_NONE;
//------------------------------------------------------------------------------
bool    bCmdMultiple = false;
bool    bStop = true;
//------------------------------------------------------------------------------
bool bOldSensDC = true;
bool bOldSensAD = true;
long startMili = -1;
//------------------------------------------------------------------------------
long cptPosition = 0;
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
#else
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
extern A4988   drvDC;
extern A4988   drvAD;
//------------------------------------------------------------------------------
extern float   convert;
//------------------------------------------------------------------------------
extern long    cptMili;
extern long    btnMili;
extern long    cptStart;
//------------------------------------------------------------------------------
extern long    cptDC;
extern long    cptAD;
//------------------------------------------------------------------------------
extern bool    bPrintPos;
extern bool    bJoy;
extern bool    bOk;
extern bool    bRelatif;
extern int     defVit;
extern float   zoom;
//------------------------------------------------------------------------------
extern bool    bSuivi;
extern bool    bSensSideral;
extern float   vitSiderale;
extern float   pasSideral;
extern float   pulseSideral;
//------------------------------------------------------------------------------
extern int     signeJoyAD;
extern int     signeJoyDC;
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
extern bool    bCmdMultiple;
//------------------------------------------------------------------------------
extern bool     bOldSensDC;
extern bool     bOldSensAD;
extern long     startMili;
//------------------------------------------------------------------------------
extern long     cptPosition;
//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void        chercheSuivant();
void        setupTimer3();
void        setup();
float       dc2deg( String s );
float       ad2deg( String s );
long        ad2pas( String s );
long        dc2pas( String s );

void        strDegToAd( String s, float f );
void        strPasToAd( String s, float f );
char *      pasToAd( float f );
void        strDegToDc( String s, float f );
void        strPasToDc( String s, float f );
char *      pasToDc( float f );

void        rattrapeJeuDC(int sign);
void        rattrapeJeuAD(int sign);
void        computeJoyDC(int x);
void        computeJoyAD(int y);
void        loopJostick();
long        getNum( String s );
float       getFloat( String s );
long        getDegOrPas( A4988* pDrv, String s );
long        deg2pas( String s );
void        setSens( A4988* pDrv, long l );
void        computeSens( A4988* pDrv, long l );
void        printRotJoyAD();
void        printRotJoyDC();
void        printInfoOuiNon(bool b);
void        printInfoOnOff(bool b);
void        printInfoTime();
void        printInfoVitsseSiderale();
void        printInfoPosition();
void        printInfo();
void        changeJoy();
void        printMode();
void        changeMode();
void        loopBtn();
void        printPosition(void);
void        sensEvenement(Evenement * pEvt);
void        computePositionSuivant();
void        printGOTOend(Evenement*);
void        computePosition();
void        chercheSuivant(int);
void        computeSuivi();
void        decodeCmd( String );
void        readCommand();
void        loop();


#endif
