#include "Motormain.h"

void SetMode(void)
{
    //CurStepMode = MIC32STEP;//for test
    //switch(CurStepMode)
    //{
        //case HALFSTEP : MODE2 = 0; MODE1 = 0; MODE0 = 1; break;//001  <- M2 M1 M0
        //case QURTSTEP : MODE2 = 0; MODE1 = 1; MODE0 = 0; break;//010
        //case MIC8STEP : MODE2 = 0; MODE1 = 1; MODE0 = 1; break;//011
        //case MIC16STEP: MODE2 = 1; MODE1 = 0; MODE0 = 0; break;//100
        //case MIC32STEP: MODE2 = 1; MODE1 = 0; MODE0 = 1; break;//101
        //case MIC32STEPA:MODE2 = 1; MODE1 = 1; MODE0 = 0; break;//110
        //case MIC32STEPB:MODE2 = 1; MODE1 = 1; MODE0 = 1; break;//111
    //}
    if ( CurStepMode == MIC8STEP )
    {
        MODE2 = 0 ; MODE1 = 1 ; 
    }
    else
    {
        MODE2 = 1 ; MODE1 = 0 ;
    }
    MODE0 = 1 ;
}

void SetDir(void)
{
   MOTOR_DIR = CurStepDir;
}

void SetTime(void)
{
    PR2 = CurStepTime;//TMR2 compare value = CurStepTime;
    TMR2 = 0;//155;
    TMR2IE = 1;
    TMR2ON = 1;
}

void Make_Table(int16u TotNoStep)
{
    CurNoStep = TotNoStep << 1;
    CurStepTime = 200;//250;
    SetMode();
    SetDir();
    SetTime();
    MotionComplete = 0;
    /*int8s i;
    int8u j=0,k;
    
    for( i = TotNoStep/2; i > 0; i-=ControlTable[j-1].NoStep )
    {
        if( i > HalfStep[j].NoStep)  ControlTable[j].NoStep = HalfStep[j].NoStep;
        else ControlTable[j].NoStep = i;

        ControlTable[j].StepTime = HalfStep[j].StepTime;
        j++;
    }
    k = j-1;
    for( i = TotNoStep/2; i > 0; i-=ControlTable[j-1].NoStep )
    {
        ControlTable[j].NoStep = ControlTable[k].NoStep;
        ControlTable[j].StepTime = ControlTable[k].StepTime;
        j++;k--;
    }
    ControlTable[j].NoStep = MNULL;
    ControlTable[j].StepTime = MNULL;
    Assignnext(); MotionComplete = 0;*/
}

/*void Assignnext(void)
{
    if ( CurNoStep = CurTabptr->NoStep )//Read next CurNoSteps;
    {
        CurStepTime = CurTabptr->StepTime;//Fill TMR2 with next CurStepTime from table;
        SetMode();
        SetDir();
        SetTime();
        MotionComplete = 0;
    }
    else
        MotionComplete = 1 ;
}*/

int8u MoveMotor(void)
{/*
	if((CurNoStep != 0))
        {
            if(flg) {flg = 0; STEP = 0;}
            else {flg = 1; STEP = 1;}
            CurNoStep--;
             return 0;
        }
	if(CurNoStep == 0)
        {
            CurTabptr++ ;
            Assignnext();
        }
	if(MotionComplete)
        {
               InitTabPtr();
        }
   */
        if((CurNoStep != 0))
        {
            if(flg) {flg = 0; STEP = 0;}
            else {flg = 1; STEP = 1;}
            CurNoStep--;
            if(CurNoStep == 0) MotionComplete = 1;
         }
}
