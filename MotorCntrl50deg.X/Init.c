#include "Motormain.h"

void initports(void)
{
    TRISA   =  0b01000110;
    TRISB   =  0b00100000;
    TRISC   =  0b10000011;
    ANSEL   =  0b010;
    ANSELH  =  0;
}

void initadc(void)
{
    ADCON0 = 0b00000101; 	// Select ADC channel. ,Set result format right justified.
    //ADON   = 0;
    ADCON1 = 0b10000000;	//Set conversion clock fosc/64.Select internal reference of VCC/2048 mV.
}

void inittimer1(void)
{
    T1CON = 0b00000101;		// 00 = 1:1 Pre scale Value, channel 0;
    //T1GSS = 1;			//   Timer1 gate source is T1G
    TMR1L = 0; TMR1H = 0;
    TMR1ON = 1;
//    rbflg = 1;

    CCP2CON = 0b00000100;
    CCP2IF = 0;
    CCP2IE = 1;

}

void inittimer2(void)
{
    T2CON = 0b00000101;		// post scale 1:1, pre scale 1:1// focs/4 = 1MHz so 100mic sec foe TMR2 = 155
    TMR2ON = 1;                 
    TMR2IE = 1;// TMR2IF
    TMR2 = 0;
    PR2 = 0xff;
    PEIE = 1;
    GIE = 1;
}

void inittimer0(void)
{
    T0CS = 0;  //0 = Internal instruction cycle clock (FOSC/4)
    PSA = 0;
    OPTION_REG = 0b0011;  //pre scal 1:16 --> 4msinterrupt
    T0SE = 1;
    T0IE = 1;
}

void initall(void)
{
    initadc();
    Readeeprom();
 
    inittimer0(); //USARTWriteString("Timer 0 init done\n\r");
    inittimer1(); //USARTWriteString("Timer 1 init done\n\r");
    inittimer2(); //USARTWriteString("Timer 2 init done\n\r");

    Calc_PID_const();
    PIDCycleNo = 0;
    MotionComplete = 1;

    MaxStep = ACurVal * 18;
    Crankflg = 0;
    Capidx = 0;

    INTCON |= 0b11001000;
    PIE1   |= 0b00000010;
}
