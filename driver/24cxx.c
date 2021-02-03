/*****************************************************************************/
#include "r_cg_macrodriver.h"
#include "24cxx.h"


#define  I2CVCC     P1.3    // 
#define  I2CWP      P1.3=
#define  I2CSCL     P1.4     //
#define  I2CSDA     P1.5    //
#define  TRISSDA    PM1.5    //
#define  _TRUE  0
#define  _FALSE 1
#define  _RtFail {I2CVCC=0;I2CWP=0;I2CSCL = 0;return _FALSE;}
#define  _RtSucc {return _TRUE;}

static void DelayMs(unsigned short TimeMs)
{
	unsigned short j;
	unsigned short  i;
	for(j=0;j<TimeMs*10;j++){		
		i=97*3;	//100us at 24Mhz
		while(i>0) 
		{	
			i--;
		}
	}
}
/****************************************************************************/
//	Module:	static void I2CDelay(void)
//	Arg:
//	Ret:
/***************************************************************************/
static void I2CDelay(void)	
{	unsigned char i;
	i=7*3;	//10us at 24Mhz
	while(i>0)
	{
		i--;
	}
}
/****************************************************************************/
//	Module:	static void I2CDelay(void)
//	Arg:
//	Ret:   I2Cstart:when SCL is high, SDA high to low
/***************************************************************************/ 
static void I2CStart(void)
{
	I2CSCL = 1;
	TRISSDA = 1;  //输入态输出1
	I2CDelay(); 
	I2CSDA = 0;
	TRISSDA = 0;  //输出态输出0
	I2CDelay();
	I2CSCL = 0;
}
/****************************************************************************/
//	Module:	static void I2CStop(void)
//	Arg:
//	Ret:   I2Cstop:when SCL is high, SDA low to high
/***************************************************************************/ 

static void I2CStop(void)
{
	TRISSDA = 0;
	I2CSDA = 0;
	I2CDelay(); 
	I2CSCL = 1; 
	I2CDelay(); 
	TRISSDA = 1;  //把SDA引脚置为输入态,前提该引脚必须接上拉电阻
	I2CDelay();
	TRISSDA = 1;
}
 
/****************************************************************************/
//	Module:	static void I2CACK(void)
//	Arg:
//	Ret:   SDA keep low,SCL low->high->low
/***************************************************************************/
static void I2CACK(void)
{
	I2CSDA = 0;
	TRISSDA = 0;
	I2CDelay(); 
	I2CSCL   = 1;
	I2CDelay(); 
	I2CSCL   = 0;
	I2CDelay(); 
}
/****************************************************************************/
static void I2CNOACK(void)
{
	TRISSDA = 1;
	I2CDelay(); 
	I2CSCL = 1;
	I2CDelay(); 
	I2CSCL = 0;
	I2CDelay(); 
}
/***************************************************************************/
static unsigned char  I2CByteTX(unsigned char byte)
{
	unsigned char loop = 8;
  
	while(loop--){
		if(byte & 0x80)  I2CSDA = 1; //获取高位状态
		else{
			I2CSDA = 0;
			TRISSDA = 0;
		}
		I2CDelay();
		I2CSCL  = 1;
		I2CDelay();
		I2CSCL  = 0;
		byte  <<= 1;
	} 
	//等待ACK
	TRISSDA = 1;    //输入态
	I2CDelay();
	I2CSCL  = 1;
	I2CDelay();
	if(I2CSDA == 1){
		I2CSCL = 1;
		return _FALSE;
	}
	I2CSCL = 0;
	I2CDelay();
	return _TRUE;
}
/***************************************************************************/
static unsigned char  I2CByteRX(void)
{
	unsigned char byte;
	unsigned char loop = 8;
	
	TRISSDA = 1;
	do{
		byte <<= 1;
		I2CSCL = 1;
		I2CDelay();
		if(I2CSDA == 1) byte |= 0x01;
		I2CSCL = 0;
		I2CDelay();
		loop--;
	}while(loop > 0);
	return(byte);
}
/***************************************************************************/

/****************************************************************************/
/*	Module:	uint8 write_eeprom(uint16 addr, uint8 *src, uint8 n,uint8 eeprom)*/
/*	Arg:   addr     *src   n eeprom						*/
/*	Ret:  _TRUE OR  _FALSE							*/
/***************************************************************************/
static unsigned char write_eeprom(unsigned int addr,  void const * const buf, unsigned int n)
{
	unsigned char w_wait,e2page,SlaveAddr;	
	unsigned char const *src=(unsigned char *)buf;
	e2page = 16;
	
	I2CVCC = 1;
	I2CDelay();
	I2CWP=0; 
	
reStart:
	SlaveAddr = 0xA0 + (unsigned char)((addr & 0x0700)>>7);
	for(w_wait=0; w_wait<4; w_wait++){
		I2CStart();
		if(_TRUE == I2CByteTX(SlaveAddr))break;
		I2CStop();
		I2CStop();
		DelayMs(1);
	}	
	if(w_wait >= 4) _RtFail;
	if(_FALSE == I2CByteTX((unsigned char)(addr%0x100))) _RtFail; 
	while(n--)
	{
		if(_FALSE == I2CByteTX(*src++)) _RtFail; 
		addr++;
		if((addr % e2page) == 0)break;
	}	
	I2CStop();
	DelayMs(5);
	if(n != 0xffff)	goto reStart;
	
	I2CDelay();
	I2CVCC=0;
	I2CWP=0;
	I2CSCL = 0;	//power down eeprom
	
	_RtSucc;
}
/****************************************************************************
*	Module:	uint8 read_eeprom(uint8 *dest, uint16 addr, uint8 n,uint8 eeprom)
*	Arg:    dest  要写入的内存地址     addr	需要读取的EEPROM地址    n读取数量
*	Ret:   从EEPROM(MC24LC256)中读取一串数放入内存中     
                                                        
***************************************************************************/
static unsigned char read_eeprom(unsigned int addr, void * const buf, unsigned int n)
{
	unsigned char w_wait,SlaveAddr;
	unsigned char *dest=(unsigned char *)buf;
	I2CVCC = 1;I2CDelay(); 	//power up eeprom
	
	SlaveAddr = 0xA0 + (unsigned char)((addr & 0x0700)>>7);	
	for(w_wait=0; w_wait<4; w_wait++){
		I2CStart();
		if(_TRUE == I2CByteTX(SlaveAddr))break;
		I2CStop();
		I2CStop();
		DelayMs(1);
	}
	
	if(w_wait >= 4) _RtFail;
	if(_FALSE == I2CByteTX((unsigned char)(addr%0x100))) _RtFail;
	I2CStart();
	if(_FALSE == I2CByteTX((SlaveAddr+1))) _RtFail;
	while(n--)
	{
		*dest++	= I2CByteRX();
		if(n != 0)
		{
			I2CACK();
		}
		addr++;
	}
	I2CNOACK();
	I2CStop();	
	DelayMs(1);
	I2CDelay();I2CVCC=0;I2CSCL = 0;	//power down eeprom
	_RtSucc;
}
/***************************************************************************/
//No more
/***************************************************************************/


_24cxx_comps_t _24cxx_comps=
{
	"e2pom r/w driver",//char desc[32];
	&_24cxx_comps,//struct _24CXX_COMPONENTS *this;
	write_eeprom,//unsigned char (*const write)(unsigned int addr,  void const * const buf, unsigned int n);
	read_eeprom//unsigned char   (*const read )(unsigned int addr,  void * const buf, unsigned int n);
};