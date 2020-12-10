#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "r_cg_serial.h"
#include "sensor.h"
#include "hum.h"
#include "24cxx.h"
#include "irc.h"
#include "string.h"
#include "stdio.h"

static unsigned char cal_param_dot_bak;
static unsigned char cal_param_unit_bak;

              

    

static struct 
{ 
    unsigned char send_buf[128];
	unsigned char recv_buf[64];
	unsigned char rec_pos;
  
}
ircMisc=
{
	{0},
	{0},
	 0
    
};


unsigned char Check_Sum(unsigned char *Data,unsigned char Len)
{
	unsigned char Sum=0;
	unsigned char i=0;
	for(i=0;i<Len;i++)
	{
		Sum+=Data[i];
	}
	return Sum;
}

unsigned char Check_Sum_5A(unsigned char const *Data,unsigned char Len)
{
    unsigned char Sum=0x5A;
    unsigned char i=0;
    for(i=0;i<Len;i++)
    {
        Sum+=Data[i];
    }
    return Sum;
}

static unsigned long calc_sum(unsigned long const*buf,int len)
{
	int i=0;
	unsigned long sum=0xaa55aa55;
	for(i=0;i<len;i++)
	{
		sum+=buf[i];
	}
	return sum;
}

static unsigned int generateCRC(unsigned char *buffer, unsigned int messageLength)
{
	unsigned int crc = 0xFFFF;
	unsigned int crcHigh = 0;
	unsigned int crcLow = 0;
	int i, j = 0;

	for (i = 0;i < messageLength;i++)
	{
		crc ^= buffer[i];
		for (j = 8; j != 0; j--)
		{
			if ((crc & 0x0001) != 0)
			{
				crc >>= 1;
				crc ^= 0xA001;
			}
			else
			{
				crc >>= 1;
			}
		}
	}
	
	return crc;
}








static unsigned char Pro_irc(unsigned char Cmd,unsigned char *buf)
{
	unsigned char i=0;
	unsigned int crc;
	unsigned char k=0;
	unsigned long temp=0;
	unsigned char VerifyResult;
	
	unsigned int DataId=((unsigned int)buf[2]<<8)+buf[3];
	if(Cmd==0x03)//Read
	{
		unsigned int DataNum=((unsigned int)buf[4]<<8)+buf[5];
		switch(DataId)
		{
			case 0:                //read addr
					ircMisc.send_buf[i++]=MD_DEVICE_ADDR;
					ircMisc.send_buf[i++]=Cmd;
					ircMisc.send_buf[i++]=2;//length
					ircMisc.send_buf[i++]=0;//device addr high
					ircMisc.send_buf[i++]=MD_DEVICE_ADDR;
					crc=generateCRC(ircMisc.send_buf, i);
					ircMisc.send_buf[i++]=crc;
					ircMisc.send_buf[i++]=crc>>8;
					break;
			case 4:                //density
					ircMisc.send_buf[i++]=MD_DEVICE_ADDR;
					ircMisc.send_buf[i++]=Cmd;
					ircMisc.send_buf[i++]=DataNum*2;//length
					ircMisc.send_buf[i++]=sensor_comps.current_density>>8;//
					ircMisc.send_buf[i++]=sensor_comps.current_density;
					if(DataNum>1)
					{
    					ircMisc.send_buf[i++]=sensor_comps.current_temp>>8;
    					ircMisc.send_buf[i++]=sensor_comps.current_temp;
					}
					crc=generateCRC(ircMisc.send_buf, i);
					ircMisc.send_buf[i++]=crc;
					ircMisc.send_buf[i++]=crc>>8;
					break;

			case 5:                //temp
					ircMisc.send_buf[i++]=MD_DEVICE_ADDR;
					ircMisc.send_buf[i++]=Cmd;
					ircMisc.send_buf[i++]=2;//length
					ircMisc.send_buf[i++]=sensor_comps.current_temp>>8;
					ircMisc.send_buf[i++]=sensor_comps.current_temp;
					crc=generateCRC(ircMisc.send_buf, i);
					ircMisc.send_buf[i++]=crc;
					ircMisc.send_buf[i++]=crc>>8;
					break;	
			case 6:                //density coe
					ircMisc.send_buf[i++]=MD_DEVICE_ADDR;
					ircMisc.send_buf[i++]=Cmd;
					ircMisc.send_buf[i++]=2;//length
					ircMisc.send_buf[i++]=MD_DENSITY_CORRECT_FACTOR>>8;
					ircMisc.send_buf[i++]=MD_DENSITY_CORRECT_FACTOR;
					crc=generateCRC(ircMisc.send_buf, i);
					ircMisc.send_buf[i++]=crc;
					ircMisc.send_buf[i++]=crc>>8;
					break;
			case 7:                //temp coe
			        ircMisc.send_buf[i++]=MD_DEVICE_ADDR;
					ircMisc.send_buf[i++]=Cmd;
					ircMisc.send_buf[i++]=2;//length
					ircMisc.send_buf[i++]=MD_TEMP_CORRECT_FACTOR>>8;
					ircMisc.send_buf[i++]=MD_TEMP_CORRECT_FACTOR;
					crc=generateCRC(ircMisc.send_buf, i);
					ircMisc.send_buf[i++]=crc;
					ircMisc.send_buf[i++]=crc>>8;
					break;
					
		   case 0x0d:  
		  
					ircMisc.send_buf[i++]=MD_DEVICE_ADDR;
					ircMisc.send_buf[i++]=Cmd;
					ircMisc.send_buf[i++]=4;//length
					ircMisc.send_buf[i++]=sensor_comps.signal_period>>24;
					ircMisc.send_buf[i++]=sensor_comps.signal_period>>16;
					ircMisc.send_buf[i++]=sensor_comps.signal_period>>8;
					ircMisc.send_buf[i++]=sensor_comps.signal_period;
					crc=generateCRC(ircMisc.send_buf, i);
					ircMisc.send_buf[i++]=crc;
					ircMisc.send_buf[i++]=crc>>8;
					break;
			
			default:       
					return 1;
			
		}
		ircComps.write(ircMisc.send_buf,i);
		return 8;
	}
	else if(Cmd==6)//Write
	{
       
	    switch(DataId)
	    {
            	   case 0:                //addr
				MD_DEVICE_ADDR=buf[5];
               			ircMisc.send_buf[i++]=MD_DEVICE_ADDR;
				ircMisc.send_buf[i++]=Cmd;
				ircMisc.send_buf[i++]=DataId>>8;
				ircMisc.send_buf[i++]=DataId;
				ircMisc.send_buf[i++]=0;
				ircMisc.send_buf[i++]=MD_DEVICE_ADDR;
				crc=generateCRC(ircMisc.send_buf, i);
				ircMisc.send_buf[i++]=crc;
				ircMisc.send_buf[i++]=crc>>8;
				sensor_comps.param_tab[sizeof(sensor_comps.param_tab)/sizeof(sensor_comps.param_tab[0])-1]= calc_sum(sensor_comps.param_tab,sizeof(sensor_comps.param_tab)/sizeof(sensor_comps.param_tab[0])-1);
	            		_24cxx_comps.write(MD_HUM_COMPS_PARAM_24CXX_ADDR_START,sensor_comps.param_tab,sizeof(sensor_comps.param_tab));
				break;
		   case 6:                //density coe
				
               	MD_DENSITY_CORRECT_FACTOR=((unsigned int)buf[4]<<8)+buf[5];	 
                ircMisc.send_buf[i++]=MD_DEVICE_ADDR;
				ircMisc.send_buf[i++]=Cmd;
				ircMisc.send_buf[i++]=DataId>>8;
				ircMisc.send_buf[i++]=DataId;
				ircMisc.send_buf[i++]=MD_DENSITY_CORRECT_FACTOR>>8;
				ircMisc.send_buf[i++]=MD_DENSITY_CORRECT_FACTOR;;
				crc=generateCRC(ircMisc.send_buf, i);
				ircMisc.send_buf[i++]=crc;
				ircMisc.send_buf[i++]=crc>>8;
				sensor_comps.param_tab[sizeof(sensor_comps.param_tab)/sizeof(sensor_comps.param_tab[0])-1]= calc_sum(sensor_comps.param_tab,sizeof(sensor_comps.param_tab)/sizeof(sensor_comps.param_tab[0])-1);
	            		_24cxx_comps.write(MD_HUM_COMPS_PARAM_24CXX_ADDR_START,sensor_comps.param_tab,sizeof(sensor_comps.param_tab));
				break;
				
			case 7:                //temp coe
				
               MD_TEMP_CORRECT_FACTOR=((unsigned int)buf[4]<<8)+buf[5];	
               ircMisc.send_buf[i++]=MD_DEVICE_ADDR;
				ircMisc.send_buf[i++]=Cmd;
				ircMisc.send_buf[i++]=DataId>>8;
				ircMisc.send_buf[i++]=DataId;
				ircMisc.send_buf[i++]=MD_TEMP_CORRECT_FACTOR>>8;
				ircMisc.send_buf[i++]=MD_TEMP_CORRECT_FACTOR;;
				crc=generateCRC(ircMisc.send_buf, i);
				ircMisc.send_buf[i++]=crc;
				ircMisc.send_buf[i++]=crc>>8;
				sensor_comps.param_tab[sizeof(sensor_comps.param_tab)/sizeof(sensor_comps.param_tab[0])-1]= calc_sum(sensor_comps.param_tab,sizeof(sensor_comps.param_tab)/sizeof(sensor_comps.param_tab[0])-1);
	            		_24cxx_comps.write(MD_HUM_COMPS_PARAM_24CXX_ADDR_START,sensor_comps.param_tab,sizeof(sensor_comps.param_tab));	
				break;
			default:       
					return 1;	
		}
		ircComps.write(ircMisc.send_buf,i);
		return 8;
		
	}
	else 
	{
		return 1;
	}
	return 1;
	
}
static unsigned char Check_irc_Com(unsigned char *Rec_Data,unsigned char Rec_Pos)
{
	
	if(Rec_Pos<2)
	{
		return 0;
	}
	if(Rec_Data[0]!=MD_DEVICE_ADDR&&Rec_Data[0]!=0)
	{
		return 1;
	}
	if(Rec_Data[1]!=0x03&&Rec_Data[1]!=0x06)
	{
		return 1;
	}
	if(Rec_Pos<8)
	{
		return 0;
	}
	if(((unsigned int)Rec_Data[7]<<8)+Rec_Data[6]!=generateCRC(Rec_Data,6))
	{
		return 1;
	}
	return Pro_irc(Rec_Data[1],Rec_Data);
}

static void Deal_irc(void)
{
	unsigned char err=0;
	if(!ircComps.sw._bit.busy)
 	do
	{
	    err=Check_irc_Com(ircMisc.recv_buf,ircMisc.rec_pos); 
		if(err>0)
		{
            DI();
            memcpy(ircMisc.recv_buf,ircMisc.recv_buf+err,ircMisc.rec_pos-=err);
			EI();
    	}
	}
	while (err>0);
}

static void  ircComps_task_handle(void)
{
   Deal_irc();
}

static void write_irc(unsigned char * const buf,unsigned int len)
{
    enable_irc_send();
    disable_irc_receive(); 
    ircComps.sw._bit.busy=1;
    R_UART0_Send(buf,len);
}

ircComps_t ircComps=
{
    ircMisc.recv_buf,
    &ircMisc.rec_pos,
    30,
    {0},
    write_irc,
    ircComps_task_handle
};


