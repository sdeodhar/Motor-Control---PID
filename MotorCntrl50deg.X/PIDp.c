#include "Motormain.h"

void Calc_PID_const(void)
{    // Td , Ti to be Specified in terms of Ts
    float Td1, Ti1;

    Td1 = Td/100.0;
    Ti1 = Ti/100.0;

    kc = 10000.0/Pb;
    k1 = kc + (kc/Ti1) + (kc*Td1) ;
    k2 = -(kc +(2.0*kc*Td1 )) ;
    k3 = kc*Td1 ;
    u = u1 = 900.0;//0.0;
    //u1 = 0.0 ; //INITPIDU1;
}

void Calc_SetPoint(void)
{
    int32u temp;
    Get_ADC_Count();
    temp = ADC_Count;
    temp *= (SCurVal - ZCurVal);
    temp /= 1023;
    temp += ZCurVal;
    SetPoint = temp;
    //USARTWriteString("St");
    //USARTWriteInt(SetPoint);USARTWriteChar(' ');
    USARTWriteChar('W');USARTWriteInt(SetPoint);USARTWriteString(strnw);

    DeadBand = ( BCurVal * SetPoint ) / 1000L;
}

void get_Actrpm(void)
{
    //int32u Rpm;
    //Tacho_Reading =  New_Capture - Old_Capture;
    //if(Tacho_Reading < 0) Tacho_Reading += 0x10000;
    //USARTWriteString("Old_Capture:");USARTWriteInt(Old_Capture);USARTWriteString("\n\r");
    //USARTWriteString("New_Capture:");USARTWriteInt(New_Capture);USARTWriteString("\n\r");
    //USARTWriteString("tacho fb:");USARTWriteInt(Tacho_Reading);USARTWriteString("\n\r");
    //Rpm = 7452L * CCurVal / Tacho_Reading;
    Rpm = ( BoardFreq * NoTeeth ) / Tacho_Reading;
    if( ( Rpm > ( SetPoint - DeadBand ) ) && ( Rpm < (SetPoint + DeadBand ) ) ) Rpm = SetPoint;

    if(Rpm > 100) Crankflg = 1;

    Tacho_Reading = 0x7fffffff;
    //USARTWriteString("Rpm");
    //USARTWriteInt(Rpm);USARTWriteChar(' ');
    USARTWriteChar('X');USARTWriteInt(Rpm);USARTWriteString(strnw);

}

void Calc_Err( int8u errno)
{
    //int32u ActRpm;
    Calc_SetPoint();
    get_Actrpm();
    //int32u ActRpm = CCurVal / PER8CNT;
    //Tacho_Reading = TMR1L + (unsigned int)(TMR1H << 8);
    //ActRpm = Tacho_Reading / PER8CNT;
    e = SetPoint - Rpm;
   // USARTWriteString("e:");USARTWriteInt(e);USARTWriteChar(' ');
    switch(errno)
    {
        case 0: e0 = e; PIDCycleNo++; break;
        case 1: e1 = e0; e0 = e; PIDCycleNo++; break;
        case 2: e2 = e1 ;e1 = e0 ; e0 = e; break;
    }
}

int8u Calc_PID_op( void )
{
    int16s NoStepOut,angle;
    /*if(PIDCycleNo < 2)
    {
        Calc_Err(PIDCycleNo);
        return;
    }
    Calc_Err(PIDCycleNo);
*/
    float diff;
        //u = (u1 + (k1*e0) + (k2*e1) + (k3*e2));
        //check limits
        //if(u > 1080.0) u = 1080.0;
        //if(u < 0.0 )    u = 0.0;
        if( Rpm > 100 )
            diff = ( k1 * e0 ) + ( k2 * e1 ) + ( k3 *e2 ) ;
        else
            diff = -48.0 ;

        if ( diff > 48.0 ) diff = 48.0 ;
        if ( diff < -48.0 ) diff = -48.0 ;

//        if ( ( u1 + diff ) > 1170.0 ) diff = 1170.0 - u1 ;
//        if ( ( u1 + diff ) < -90.0  ) diff = -90.0 - u1 ;
          if ( ( u1 + diff ) > (MaxStep + 90) ) diff = MaxStep + 90 - u1 ;
          if ( ( u1 + diff ) < -90.0  )  diff = -90.0 - u1 ;
        u = u1 + diff ;
        //output = u;//1 * u;
        NoStepOut = diff ; //u - u1;
        //if ( NoStepOut > 500 ) { NoStepOut = 500 ; u = u1 + 500 ; }
        //if ( NoStepOut < -500 ) { NoStepOut = -500 ; u = u1 - 500 ; }
        //if ( NoStepOut > 30 ) { NoStepOut /= 4 ; CurStepMode = MIC8STEP ; }
        //else if ( NoStepOut < -30 ) { NoStepOut /= 4 ; CurStepMode = MIC8STEP ; }

        //else
            CurStepMode = MIC32STEP ;
        u1 = u;

        //NoStepOut = 5000 - output;

        if( NoStepOut < 0 ) { CurStepDir = ANTICLKVIZ; NoStepOut = 0 - NoStepOut;}
        else { CurStepDir = CLKVIZ;}
        CurTotNoStep = NoStepOut ;
  //set motor motion parameters(
        //CurTotNoStep = NoStepOut / StepConst;//(NoStepOut * 4)/25;

        //if( CurTotNoStep > 500 ) CurTotNoStep = 500 ;

        //USARTWriteString("Stp:");
        //USARTWriteInt(CurTotNoStep);USARTWriteChar(' ');
        //USARTWriteInt(output);USARTWriteChar(' ');
        //if( CurStepDir )USARTWriteChar('F'); else USARTWriteChar('R');
        //if( CurStepMode == MIC8STEP )USARTWriteChar('8');else USARTWriteString("32");
//USARTWriteChar(' ');
        angle = u/18;// * 5.625;
        if(angle < 0 ) angle = 0;

        USARTWriteChar('Y');USARTWriteInt(angle);USARTWriteString(strnw);
        //USARTWriteInt(angle);
        //USARTWriteString(strnw);
//CurTotNoStep = 29;//just 
        //if(CurTotNoStep > 32) CurStepMode = HALFSTEP;
        //else CurStepMode = MIC32STEP;
        
        Make_Table( CurTotNoStep );
      //  USARTWriteString("Step count:"); USARTWriteInt(CurTotNoStep); USARTWriteString("\n\r");
}
