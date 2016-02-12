//#include <pic16f886.h>
#include "Motormain.h"

void USARTinit(void)
{
    TRISCbits.TRISC7 = 1;// RC7 = RX out
    TRISCbits.TRISC6 = 0;// RC6 = TX in
    TXSTAbits.BRGH = 1;//0;
    BRG16 = 0;
    SYNC = 0;

    TXSTAbits.TX9 = 0;        // select 8 data bits
    TXSTAbits.TXEN= 1;     // enable transmit
    RCSTAbits.CREN = 1;
    RCSTAbits.SPEN= 1;       // serial port is enabled
    //RCSTAbits.RX9=0;        // select 8 data bits
    SPBRG  = 25;//69 ;      //9600 at 4MHz
    SPBRGH = 0;
    //BAUDCTL = 0;//Baud rate =  FOSC / [ 16 ( n + 1 ) ]
}

void USARTWriteChar(char ch)
{
  while(!TXSTAbits.TRMT);
  TXREG = ch;
}

void USARTWriteString(const char *str)
{
  while(*str!='\0')
  {
      USARTWriteChar(*str);
      str++;
  }
}

void USARTWriteInt(int32s val)
{
    char str[8]={'0','0','0','0','0','0','0','0'};
    int i = 7;
    //Handle negative integers
    if( val < 0 )
    {
        USARTWriteChar('-');   //Write Negative sign
        val = val * -1;     //convert to positive
    }
    else
    {
        USARTWriteChar('+');
    }
    while(val)
    {
        str[i]=(val % 10)+'0';
        val = val / 10;
        i--;
        if ( i == 0 ) break ;
    }
    for( i = 0 ; i < 8 ; i++ )
    {
        USARTWriteChar(str[i]);
    }
}

int8u UARTGetChar(void)
{
    RCIE = 1;
    RCEN = 1;
    int8u temp;
    while( !RCIF );
    temp = RCREG;
    // Clear error flag
    RCSTAbits.OERR = 0;
    RCIF = 0; //RCIE = 0;
    RCEN = 0;
    return temp;
}

int8u USARTGetString(void)
{
    RCEN = 1;
    int8u charcur = '0';
    int8u i=0;
    charcur = UARTGetChar();

    while((charcur != '\n')&&(charcur != '\r'))
    {
        if( i < COMDSTRLEN-1 )
        {
            Commandstr[i] = charcur;
            i++;
        }
        charcur = UARTGetChar();
    }
    Commandstr[i] = '\0';
    RCEN = 0;
}

void DispallCur(void)
{
   Readeeprom();
   //USARTWriteString(" \tValue\t\t  \tRange"); USARTWriteString("\n\r\n\r");
   USARTWriteChar('Z'); USARTWriteInt(ZCurVal); //USARTWriteString(strtb);
   //USARTWriteInt(ZMIN);USARTWriteString("\tTO\t");USARTWriteInt(ZMAX);
   USARTWriteString(strnw);
   USARTWriteChar('S'); USARTWriteInt(SCurVal); //USARTWriteString(strtb);
   //USARTWriteInt(SMIN);USARTWriteString("\tTO\t");USARTWriteInt(SMAX);
   USARTWriteString(strnw);
   USARTWriteChar('T'); USARTWriteInt(NoTeeth); //USARTWriteString(strtb);
   //USARTWriteInt(TMIN);USARTWriteString("\tTO\t");USARTWriteInt(TMAX);
   USARTWriteString(strnw);
   //USARTWriteString("M"); USARTWriteInt(MCurVal); //USARTWriteString(strtb);
   //USARTWriteInt(MMIN);USARTWriteString("\tTO\t");USARTWriteInt(MMAX);
   //USARTWriteString(strnw);
   USARTWriteChar('P'); USARTWriteInt(PCurVal); //USARTWriteString(strtb);
   //USARTWriteInt(PMIN);USARTWriteString("\tTO\t");USARTWriteInt(PMAX);
   USARTWriteString(strnw);
   USARTWriteChar('I'); USARTWriteInt(ICurVal); //USARTWriteString(strtb);
   //USARTWriteInt(IMIN);USARTWriteString("\tTO\t");USARTWriteInt(IMAX);
   USARTWriteString(strnw);
   USARTWriteChar('D'); USARTWriteInt(DCurVal); //USARTWriteString(strtb);
   //USARTWriteInt(DMIN);USARTWriteString("\tTO\t");USARTWriteInt(DMAX);
   USARTWriteString(strnw);
   USARTWriteChar('R'); USARTWriteInt(CLKVIZ); //USARTWriteString(strtb);
   //USARTWriteInt(DMIN);USARTWriteString("\tTO\t");USARTWriteInt(DMAX);
   USARTWriteString(strnw);
   USARTWriteChar('B'); USARTWriteInt(BCurVal); //USARTWriteString(strtb);
   //USARTWriteInt(DMIN);USARTWriteString("\tTO\t");USARTWriteInt(DMAX);
   USARTWriteString(strnw);
   USARTWriteChar('F'); USARTWriteInt(BoardFreq);
   USARTWriteString(strnw);
   USARTWriteChar('A'); USARTWriteInt(ACurVal); //USARTWriteString(strtb);
   //USARTWriteInt(AMIN);USARTWriteString("\tTO\t");USARTWriteInt(AMAX);
   USARTWriteString(strnw);
   //USARTWriteString(strnw);
}

int32s str_to_num( int8u *String )
{	int8u i ;
	int32s Num = 0 ;
	for( i = 0 ; String[ i ] != '\0' ; i++ )
	{
		//if( ( String[ i ] != '/' ) && ( String[ i ] != ':' ) && ( String[ i ] != '.' ) && ( String[ i ] != '-' ) )
                if((String[ i ] >= '0')&&(String[ i ] <= '9'))
		{
			Num *= 10 ;
			Num += String[ i ] - '0' ;
		}
	}
	if( String[ 0 ] == '-' ) Num *= -1 ;
	return Num ;
}

void getrangeval(void)
{
    switch(Command)
    {
        case 'Z': MaxVal = ZMAX; MinVal = ZMIN;   break;
        case 'S': MaxVal = SMAX; MinVal = SMIN;   break;
        case 'T': MaxVal = TMAX; MinVal = TMIN;   break;
       // case 'M': MaxVal = MMAX; MinVal = MMIN;   break;
        case 'P': MaxVal = PMAX; MinVal = PMIN;   break;
        case 'I': MaxVal = IMAX; MinVal = IMIN;   break;
        case 'D': MaxVal = DMAX; MinVal = DMIN;   break;
        case 'B': MaxVal = BMAX; MinVal = BMIN;   break;
        case 'R': MaxVal = DIMAX; MinVal = DIMIN;   break;
        case 'Q': break;
        case 'F': MaxVal = FMAX; MinVal = FMIN;   break;
        case 'A': MaxVal = AMAX; MinVal = AMIN;   break;
        default: Command = BCMD;
    }
}

int8u ChkRange(int32u val)
{
    if( val > MaxVal || val < MinVal) return 1;
    else return 0;
}

void StoreCurVal(int32u val)
{
    switch(Command)
    {
        case 'Z': ZCurVal = val;   break;
        case 'S': SCurVal = val;   break;
        case 'T': NoTeeth = val;   break;
        //case 'M': MCurVal = val;   break;
        case 'P': PCurVal = val;   break;
        case 'I': ICurVal = val;   break;
        case 'D': DCurVal = val;   break;
        case 'B': BCurVal = val;   break;
        case 'R': CLKVIZ  = val;   break;
        case 'Q': break;
        case 'F': BoardFreq = val;   break;
        case 'A': ACurVal = val;   break;
    }
    Writeeeprom();
}

int8u getData(void)
{
    int8u i;
    USARTGetString();
    Command = Commandstr[0];
    //USARTWriteString("Cmd:");USARTWriteChar(Command);USARTWriteString(strnw);

    for( i = 0 ; i < COMDSTRLEN-1 ; i++)
    {
        Commandstr[i] = Commandstr[i+1];
    }

    CurVal = str_to_num(Commandstr);
    //USARTWriteString("Value:");USARTWriteInt(CurVal);USARTWriteString(strnw);

    getrangeval();
    if(Command == BCMD)
    {
        USARTWriteChar('C');//USARTWriteString("BCMD");
        USARTWriteString(strnw);
        return(BCMD);
        //USARTWriteString("BCMD");USARTWriteString(strnw); return(BCMD);
    }
    //    If BADCOMMAND	Command = BMCD;	Outtext ? BCMD
    //   Else Separate Next Chars up to \n or\t into CurVal;
    else if(Command == 'Q')  DispallCur();//Print all parameters
    else if(ChkRange(CurVal))//  If CurVal out of range
    {
         USARTWriteChar('E');//USARTWriteString("RNGERR");
         USARTWriteString(strnw);
         return(RNGERR);
    }
    else
    {
        StoreCurVal(CurVal);//Store 	CurVal in appropriate Variable;  //If acceptable
        //USARTWriteChar(Command); USARTWriteString(Commandstr); USARTWriteString("\n\r");///USARTWriteChar('\n');
        return(Command);
    }
}
