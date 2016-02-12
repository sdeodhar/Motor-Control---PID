/* 
 * File:   motormain.c
 * Created on December 9, 2014, 10:57 AM
 */
#include "Motormain.h"

__CONFIG(LVP_OFF & DEBUG_OFF & FOSC_INTRC_NOCLKOUT & WDTE_OFF & PWRTE_ON & MCLRE_OFF & CP_ON & CPD_OFF & BOREN_ON & IESO_OFF & FCMEN_OFF);
__CONFIG(WRT_OFF & BOR4V_BOR21V );

void Get_ADC_Count(void)
{
    ADIF = 0;
    ADON = 1;           //Enable ADC module.

    asm("NOP");asm("NOP");
    asm("NOP");asm("NOP");
    asm("NOP");asm("NOP");
    //Delay(2);           //Wait for 11.5 Tad (acquisition time).

    GO = 1;		//Set GO/DONE bit.
  //  while (!GO){}	//Poll the GO/DONE bit.
    while(!ADIF){}
    ADIF = 0;
    ADON = 0;
    ADC_Count = ADRESH;
    ADC_Count <<= 8 ;
    ADC_Count += ADRESL;// Read result into ADC_Count.
    ADC_Count = ADC_Count & 0x03ff;
    ADIF = 0;
   // USARTWriteString("ADC");USARTWriteInt(ADC_Count);USARTWriteString(strnw);
}

//void Move_Motor_to_Zero(void){}
//void Move_Motor_to_Half(void){}

void main(void)
{
    OSCCON = 0b01100001;  //4 MHz
    OSCTUNE= 0;
    //INTCON = 0;
    //Count1micros = 0;
    Count4ms = 0;
    //Delay(2);
    initports();
    USARTinit();
//USARTWriteString("Serial init\n\rchecking for config enable...\n\r");
    while(SERIALDETECT == 1)
    {
        //USARTWriteString("\n\rEnter Config\n\r");USARTWriteString(strnw);
        //USARTWriteString("\n\rEnter ");
        getData();
    }
    ENB = 0;
    nSLEEP = 1;
    DECAY = 1;
    initall();
    int i;
    for( i = 0 ; i < 4 ; i++)
    {
        CurTotNoStep = 270; CurStepDir = ANTICLKVIZ; CurStepMode = MIC32STEP ;

        Make_Table( CurTotNoStep );
        while(!MotionComplete);
    }

    while(Crankflg == 0)
    {
        get_Actrpm();
        Calc_SetPoint();
        USARTWriteChar('N');USARTWriteString(strnw);
    }

    for( i = 0 ; i < 4 ; i++)
    {
        CurTotNoStep = 225; CurStepDir = CLKVIZ; //CurStepMode = MIC32STEP ;
        Make_Table( CurTotNoStep );
        while(!MotionComplete);
    }
        //MoveMotor();
    while(1)
    {
        if(MS100Flag)
        {
            MS100Flag = 0;
            Calc_Err(PIDCycleNo);
            
            if((PIDCycleNo >= 2) /*&& Crankflg*/ )
            {
                Calc_PID_op();
            }//USARTWriteString("100 ms done \n\r");
            /*else
            {
                USARTWriteChar('N');USARTWriteString(strnw);
                //USARTWriteString("no crank\n\r");
            }*/
        }
    }
}

void interrupt isrroutine(void)
{
  int8u idx;

  if (T0IF && T0IE)
  {
      TMR0 = VAL1MS;// 1MHz -> 4 interrupts = 1ms; prescale 1:16 so interrupt --> 4ms
      Count4ms++;
      if(Count4ms == 25)
      {
          MS100Flag = 1;
          Count4ms =0;
      }
      T0IF = 0;
  }

  if (CCP2IE && CCP2IF)
  {
      Old_Capture = New_Capture;
      New_Capture = CCPR2L + (int16u)(CCPR2H << 8);

      Cap_Diff1 = New_Capture - Old_Capture;
      if(Cap_Diff1 < 0) Cap_Diff1 += 0x10000;
      Cap_Diff[ Capidx ] = Cap_Diff1 ;
      if(Capidx < 7)Capidx++;
      else Capidx = 0;

      Tacho_Reading = 0 ;
      for( idx = 0 ; idx < 8 ; idx++ )
      {
            Tacho_Reading +=  Cap_Diff[idx];
      }

      Tacho_Reading = Tacho_Reading >> 3 ;

      CCP2IF = 0;
  }
  
  if (TMR2IF && TMR2IE)
  {
     	TMR2IF = 0;

        TMR2 = 0;

        MoveMotor();
        TMR2IE = 1;
        if(MotionComplete == 1)
        {
            STEP = 0;
            TMR2 = 0x00;				// Reset Timer1
            TMR2ON  = 0;                   // Restart Timer1
        }
        TMR2IF = 0;//TMR2ON = 0;
  }
}
