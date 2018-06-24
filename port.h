//------------------------------------------------------------------------------
//
//                  port.h
//
//------------------------------------------------------------------------------
#ifdef PROTO_ARDUINO
    #define pinDC_EN        4
    #define pinDC_MS1      13   
    #define pinDC_MS2      12   
    #define pinDC_MS3      11
    #define pinDC_RST      10
    #define pinDC_STEP      9
    #define pinDC_DIR       8
    //-----------------------------------------------------------------------------
    #define pinAD_EN       56
    #define pinAD_MS1      57   
    #define pinAD_MS2      58   
    #define pinAD_MS3      59
    #define pinAD_RST       7
    #define pinAD_STEP      6
    #define pinAD_DIR       5
#else
    #define pinDC_EN        A9
    #define pinDC_MS1      A10 
    #define pinDC_MS2      A11   
    #define pinDC_MS3      A12
    #define pinDC_RST      A13
    #define pinDC_STEP     A14
    #define pinDC_DIR      A15
    //-----------------------------------------------------------------------------
    #define pinAD_EN         7
    #define pinAD_MS1        8
    #define pinAD_MS2        9
    #define pinAD_MS3       10
    #define pinAD_RST       11
    #define pinAD_STEP      12
    #define pinAD_DIR       13
#endif
//-----------------------------------------------------------------------------
#define pinBtn         53
//-----------------------------------------------------------------------------
#define pinJoyX        A0
#define pinJoyY        A1

