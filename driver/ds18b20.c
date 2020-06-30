#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "r_cg_serial.h"
#include "ht1621x.h"
#include "sensor.h"
#include "ds18b20.h"
#include "ad54x0.h"
#include "r_cg_adc.h"
#include "r_cg_timer.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "24cxx.h"

#define MD_DQ             P0.1
#define MD_DQ_PULL_UP     PU0.1=1
#define MD_DQ_CFG_OUT     PM0.1=0
#define MD_DQ_CFG_IN      PM0.1=1

static void udelay(unsigned long delay)
{
	while(delay>0)
	{
		NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
		delay--;
	}
}
static MD_DS18B20_EXECUTION_STATUS reset(void)  
{  
    int retval = 0;  
  
    MD_DQ_PULL_UP;// s3c2410_gpio_pullup(DQ, 0);  
    MD_DQ_CFG_IN;//s3c2410_gpio_cfgpin(DQ, CFG_OUT);  
    
  
    //MD_DQ=1;//s3c2410_gpio_setpin(DQ, 1);  
    udelay(4);  
    MD_DQ=0;//s3c2410_gpio_setpin(DQ, 0); // 拉低ds18b20总线，复位ds18b20  
    MD_DQ_CFG_OUT;
    udelay(550);                // 保持复位电平500us  t>480us 
  
    MD_DQ_CFG_IN;//s3c2410_gpio_setpin(DQ, 1); // 释放ds18b20总线   
    udelay(70);  //15us<t<60us
  
    // 若复位成功，ds18b20发出存在脉冲（低电平，持续60~240us）   
    //s3c2410_gpio_cfgpin(DQ, CFG_IN);  
    retval = MD_DQ; //    retval = s3c2410_gpio_getpin(DQ); 
  
    udelay(250);
    
    // MD_DQ_CFG_OUT;// s3c2410_gpio_cfgpin(DQ, CFG_OUT);  
    //s3c2410_gpio_pullup(DQ, 0);  
   // s3c2410_gpio_setpin(DQ, 1); // 释放总线   
  
    return retval;  
} 

static void write_byte(unsigned char data)  
{  
    int i = 0;  
  
   // MD_DQ_CFG_OUT;// s3c2410_gpio_cfgpin(DQ, CFG_OUT);  
    //s3c2410_gpio_pullup(DQ, 1);  
  
    for (i = 0; i < 8; i++)  
    {  
        // 总线从高拉至低电平时，就产生写时隙   
        MD_DQ_CFG_IN;//s3c2410_gpio_setpin(DQ, 1);  
        udelay(4);  
        MD_DQ=0;//s3c2410_gpio_setpin(DQ, 0);
	MD_DQ_CFG_OUT;
	udelay(4);//////////t<15
        if(data&0x01)//s3c2410_gpio_setpin(DQ, data & 0x01); 
	{
		MD_DQ_CFG_IN;
	}
	else
	{
		MD_DQ=0;
	        MD_DQ_CFG_OUT;// s3c2410_gpio_cfgpin(DQ, CFG_OUT);  
	}
        udelay(60);  
        data >>= 1;  
    }  
    MD_DQ_CFG_IN;//s3c2410_gpio_setpin(DQ, 1); // 重新释放ds18b20总线   
}  

static unsigned char read_byte(void)  
{  
    int i;  
    unsigned char data = 0;  
  // 总线从高拉至低，只需维持低电平17ts，再把总线拉高，就产生读时隙   
  //  s3c2410_gpio_cfgpin(DQ, CFG_OUT);  
   // s3c2410_gpio_pullup(DQ, 0); 
 
    for (i = 0; i < 8; i++)  
    {  
        
        MD_DQ_CFG_IN;//s3c2410_gpio_setpin(DQ, 1);  
        udelay(4);  
        MD_DQ=0;//s3c2410_gpio_setpin(DQ, 0);
	MD_DQ_CFG_OUT;
        udelay(4);  
        MD_DQ_CFG_IN;// s3c2410_gpio_setpin(DQ, 1);  
        udelay(6);  
        data >>= 1;  
    //    s3c2410_gpio_cfgpin(DQ, CFG_IN);  
        if(MD_DQ)//if (s3c2410_gpio_getpin(DQ)) 
	{
            data |= 0x80; 
	}
        udelay(10);  //50
    }  
//    s3c2410_gpio_cfgpin(DQ, CFG_OUT);  
  //  s3c2410_gpio_pullup(DQ, 0);  
     MD_DQ_CFG_IN;//s3c2410_gpio_setpin(DQ, 1); // 释放ds18b20总线   
    return data;  
} 

static unsigned char check_crc(void const *p,unsigned char len)
{
	unsigned  char crc = 0, i, j;
	unsigned char *dat=(unsigned char *)p;
	for (i = 0; i <len ; i++)
	{
		crc = crc ^ dat[i];
		for (j = 0; j < 8; j++)
		{
			if (crc & 0x01) 
			{
				crc = (crc >> 1) ^ 0x8C;
			}
			else 
			{
				crc >>= 1;
			}
		}
	}
	return crc;
}

static MD_DS18B20_EXECUTION_STATUS read_ram(void *const buf,unsigned int len)
{
	unsigned  int  i=0;
	unsigned  char temp[9]={1,2};
	unsigned  char *Buf=(unsigned char *)buf;
	if(reset())
	{
		return 1;
	}
	write_byte(0xcc);// Skip ROM command 
	write_byte(0xbe);// read ram command
	for(i=0;i<9;i++)
	{
		temp[i]=read_byte();
	}
	if(temp[8]==check_crc(temp,8))
	{
		for(i=0;i<len;i++)
		{
			Buf[i]=temp[i];
		}
		return 0;
	}
	else
	{
		return 1;
	}
}

static void write_ram(void const * buf, unsigned int len)
{
	unsigned  int  i=0;
	unsigned  char temp[9];
	unsigned  char const *Buf=(unsigned char *)buf;
	reset();
	write_byte(0xcc);// Skip ROM command 
	write_byte(0x4e);// read ram command
	for(i=0;i<len;i++)
	{
		write_byte(Buf[i]);
	}
}

static void start_temp_convert(void)
{
	reset();
	write_byte(0xcc);// Skip ROM command 
	write_byte(0x44);// start 18b20 temp convert
}

static int get_temp_valve(void)
{
	int temp;
	unsigned char buf[2];
	if(!read_ram(buf,sizeof(buf)))
	{
		temp=((int)buf[1]<<8)+buf[0];
	}
	else 
	{
		temp=-1000;
	}
	return temp;
}
	

ds18b20_comps_t ds18b20_comps=
{
	"ds18b20 driver",//char desc[32];
	&ds18b20_comps,//struct _DS18B20_COMPONENTS  *this;
	reset,//MD_DS18B20_EXECUTION_STATUS (*const reset)(void);
	read_ram,//MD_DS18B20_EXECUTION_STATUS (*const read_ram )(void * const  buf, unsigned int len);
	write_ram,//void                        (*const write_ram)(void const * buf, unsigned int len);
	start_temp_convert,//void(*const start_temp_convert)(void);
	get_temp_valve//int (*const get_temp_valve)  (void);
};
