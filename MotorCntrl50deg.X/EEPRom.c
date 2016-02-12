#include "Motormain.h"

int8u RdByte( int8u addr )
{
    int8u Data ;

    EEADR = addr ;
    EECON1 = 0x01 ;
    EEPGD = 0 ;
    RD = 1 ;
    while ( RD == 1 ) {}
    Data = EEDAT ;
    RD = 0 ;
    return Data ;
}

int32u ReadEEP(int8u addr)
{
    int32u DataH = 0 ;
    int8u i ;

    for ( i = 0 ; i < 4 ; i++ )
    {
        DataH <<= 8 ;
        DataH |= RdByte( addr ) ;
        addr++ ;
    }
    return DataH ;
}

void Readeeprom(void)
{    //read all ZCurVal,SCurVal,CCurVal,MCurVal,PCurVal,ICurVal,DCurVal;
     ZCurVal = ReadEEP(ZADDR);
     SCurVal = ReadEEP(SADDR);
     NoTeeth = ReadEEP(TADDR);
     //MCurVal = ReadEEP(MADDR);
     PCurVal = ReadEEP(PADDR);
     ICurVal = ReadEEP(IADDR);
     DCurVal = ReadEEP(DADDR);
     BCurVal= ReadEEP(BADDR);
     CLKVIZ = ReadEEP(DIADDR);
     BoardFreq =ReadEEP(FADDR);
     ACurVal = ReadEEP(AADDR);

     if( CLKVIZ == 1 ) ANTICLKVIZ = 0;
     else ANTICLKVIZ = 1;

     Ti = ICurVal;
     Td = DCurVal;
     Pb = PCurVal;
}

int8u WrByte( int8u addr , int8u data )
{
    EEADR = addr ;
    EEDAT = data ;

    EECON1 = 0x00 ;
    WREN=1;							//write enable set
    GIE=0;
    EECON2=0x55;
    EECON2=0xAA;
    WR=1;							//set WR register to write
    GIE=1;
    WREN=0;							//write enable cleared
    while( WR == 1 ) {}
    if(EEIF==1)					//poll write operation
    {
	EEIF=0;					//clear the write flag
	return 1;				//return 1 if write completed
    }
    else
    {
	return 0;				//else return 0
    }

}

int8u WriteEEP( int32u Data, int8u addr)
{
    int8u i , status = 1 ;
    addr += 3 ;
    for ( i = 0 ; ( i < 4 ) && ( status ); i++ )
    {
        status = WrByte( addr , Data & 0xff ) ;
        addr-- ;
        Data >>= 8 ;
    }

    return status;
}


void Writeeeprom(void)
{    //switch write ZCurVal,SCurVal,CCurVal,MCurVal,PCurVal,ICurVal,DCurVal;
    int8u retval;
    switch(Command)
    {
        case 'Z': retval = WriteEEP(ZCurVal,ZADDR);   break;
        case 'S': retval = WriteEEP(SCurVal,SADDR);   break;
        case 'T': retval = WriteEEP(NoTeeth,TADDR);   break;
        //case 'M': retval = WriteEEP(MCurVal,MADDR);   break;
        case 'P': retval = WriteEEP(PCurVal,PADDR);   break;
        case 'I': retval = WriteEEP(ICurVal,IADDR);   break;
        case 'D': retval = WriteEEP(DCurVal,DADDR);   break;
        case 'Q': break;
        case 'B': retval = WriteEEP(BCurVal,BADDR);   break;
        case 'R': retval = WriteEEP(CLKVIZ,DIADDR);   break;
        case 'F': retval = WriteEEP(BoardFreq,FADDR);   break;
        case 'A': retval = WriteEEP(ACurVal,AADDR);   break;
    }
    if(retval)
    {
        USARTWriteChar('G');USARTWriteString(strnw);
        //USARTWriteString("Success");USARTWriteString(strnw);
    }
    else
    {
        USARTWriteChar('H');USARTWriteString(strnw);
        //USARTWriteString("Fail");USARTWriteString(strnw);
    }
}
