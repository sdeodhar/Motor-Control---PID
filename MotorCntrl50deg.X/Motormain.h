/*
Z +00000800
S +00002000
T +00000060
M +00000060
P +00002000
I +00000001
D +00000000
R +00000000
B +00000005
F +00132894
*/

/* 
 * File:   Motormain.h
 * Author: Admin
 *
 * Created on December 11, 2014, 2:01 PM
 */
#include <pic16f886.h>
#include <stdio.h>
#include <stdlib.h>
#include <htc.h>

#define int8u   unsigned char
#define int16u  unsigned int
#define int32u	unsigned long
#define int8s   signed   char
#define int16s	signed   int
#define int32s  signed   long

#define	MODE0		PORTBbits.RB4
#define	MODE1		PORTBbits.RB6
#define	MODE2		PORTBbits.RB7
#define	TMRGATE         PORTBbits.RB5
#define	STEP		PORTBbits.RB3
#define	MOTOR_DIR	PORTBbits.RB1
#define	ENB		PORTBbits.RB2
#define	DECAY		PORTBbits.RB0
#define	TTL_TX          PORTCbits.RC6
#define	TTL_RX          PORTCbits.RC7
#define	nSLEEP          PORTCbits.RC5

//#define	 FAULT
#define	HOME		PORTCbits.RC3
#define	C2INP		PORTAbits.RA2
#define	C2OUTP          PORTAbits.RA5
#define	TACHOFB         PORTCbits.RC0
#define	POTANG          PORTAbits.RA1
#define	SERIALDETECT	PORTAbits.RA6

#define MNULL       NULL

#define ZMAX        100000
#define ZMIN        0
#define SMAX        100000
#define SMIN        0
#define PMAX        10000000
#define PMIN        0
#define IMAX        100000
#define IMIN        0
#define DMAX        100000
#define DMIN        0
#define BMAX        100000
#define BMIN        0
#define DIMAX       1
#define DIMIN       0
#define TMAX        100000
#define TMIN        0
#define FMAX        1000000
#define FMIN        0
#define AMAX        180
#define AMIN        10

#define ZADDR       0//0x00
#define SADDR       4//0x04
#define TADDR       8//0x08
#define PADDR       12//0x0c
#define IADDR       16//0x14
#define DADDR       20//0x18
#define BADDR       24//0x1c
#define DIADDR      28//0x20
#define FADDR       32//0x24
#define AADDR       36

#define BCMD        9
#define RNGERR      10

//#define HALFSTEP    0
//#define QURTSTEP    1
#define MIC8STEP    2
//#define MIC16STEP   3
#define MIC32STEP   4
//#define MIC32STEPA  5
//#define MIC32STEPB  6

//#define MAX_TAB_ENTRIES 10
#define COMDSTRLEN      10
#define VAL1MS          06

//#define Ts        1000
//#define INITPIDU1 5000.0

const char strnw[2] = "\n";
//const char strnw[3] = "\n\r";
const char strtb = '\t';
/*
typedef struct
{   int8u NoStep;
    int8u StepTime;
}MotorTab;
*/
//const MotorTab HalfStep[5]={{4,250},{8,125},{168,63},{200,50},{240,40}};
//const MotorTab HalfStep[5]={{4,250},{8,250},{168,250},{200,250},{240,250}};
//MotorTab By32Step[9]={0};

//MotorTab ControlTable[10] ; //= {{MNULL,MNULL},{MNULL,MNULL},{MNULL,MNULL},{MNULL,MNULL},{MNULL,MNULL},{MNULL,MNULL},{MNULL,MNULL},{MNULL,MNULL},{MNULL,MNULL},};
//MotorTab *CurTabptr;

int32u Ti,Pb;
float  Td,e0,e1,e2,u,u1;
float  kc,ki,kd,k1,k2,k3;
float MaxStep;
int16u SetPoint,DeadBand;//,output

int16u Old_Capture,New_Capture,Cap_Diff[8];
int16s Cap_Diff1,e ;
int32u Rpm;

int8u MS100Flag,Capidx;
int8u Count4ms;
int8u flg,Crankflg;//,rbflg ;
int16u CurNoStep;
int8u  CurStepTime,CurStepDir,CurStepMode;
int16u CurTotNoStep;
int8u MotionComplete;//TableOver,//MoveMotorFlag,CurStepTimeFlag;
//int16u AbsPos;
int16u ADC_Count;
int32s Tacho_Reading;

int8u CLKVIZ,ANTICLKVIZ;
int32s CurVal;
int32u MaxVal,MinVal;
int8u  Command;

int32u ZCurVal,SCurVal,PCurVal,ICurVal,DCurVal,BCurVal,ACurVal;//,MCurVal
int32u NoTeeth,BoardFreq;
int8u  Commandstr[COMDSTRLEN];
int8u  PIDCycleNo;

extern void USARTinit(void);
extern void USARTWriteChar(char ch);
extern void USARTWriteString(const char *str);
extern void USARTWriteInt(int32s val);
extern int8u USARTGetString(void);
extern int8u getData(void);

extern int32u ReadEEP(int8u addr);
extern void Readeeprom(void);
extern int8u WriteEEP( int32u Data, int8u addr);
extern void Writeeeprom(void);

extern void Calc_PID_const(void);
extern void Calc_SetPoint(void);
extern void Calc_Err( int8u errno);
extern int8u Calc_PID_op( void );
extern void get_Actrpm(void);

extern void Get_ADC_Count(void);

extern void initports(void);
extern void initadc(void);
extern void inittimer1(void);
extern void inittimer2(void);
//extern void InitTabPtr(void);
extern void initall(void);

extern void SetMode(void);
extern void SetDir(void);
extern void SetTime(void);
extern void Assignnext(void);
extern int8u MoveMotor(void);
extern void Make_Table(int16u TotNoStep);
