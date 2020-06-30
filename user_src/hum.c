#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "r_cg_serial.h"
#include "hum.h"
#include "ht1621x.h"
#include "sensor.h"
#include "r_cg_adc.h"
#include "r_cg_timer.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
          //key_io

#define   MD_K1  P13.7
#define   MD_K2  P12.0
#define   MD_K3  P12.1
#define   MD_KEY_MASK  (int)0x07
#define   MD_KEY_DATA  ((MD_K1<<2)+(MD_K2<<1)+MD_K3)
#define   MD_S_KEY     0x03//011
#define   MD_M_KEY     0x05//101
#define   MD_J_KEY     0x06//110



#define   MD_HIDE_DISP                  (sizeof(hum_comps.SEG_TAB)-1)

				//  D10  H   L  
static void  MD_DISPLAY_WRITE_D1_0(unsigned char a)   
{
	hum_comps.dis_buf[5]=(hum_comps.SEG_TAB[a]>>4);
	hum_comps.dis_buf[4]=(hum_comps.SEG_TAB[a]&0x0f);
	ht1621x_comps.write(_10_PIN_LCD_SEG_05,hum_comps.dis_buf[5]);
	ht1621x_comps.write(_09_PIN_LCD_SEG_04,hum_comps.dis_buf[4]);
}
static void  MD_DISPLAY_WRITE_D1_1(unsigned char a) //  D11  H   L
{
	hum_comps.dis_buf[3]=(hum_comps.SEG_TAB[a]>>4);
	hum_comps.dis_buf[2]=(hum_comps.SEG_TAB[a]&0x0f);
	if(hum_comps.dot1_pos==1) hum_comps.dis_buf[3]|=0x08;
	ht1621x_comps.write(_08_PIN_LCD_SEG_03,hum_comps.dis_buf[3]);
	ht1621x_comps.write(_07_PIN_LCD_SEG_02,hum_comps.dis_buf[2]);
}
static void  MD_DISPLAY_WRITE_D1_2(unsigned char a)	//  D12  H   L 
{
	hum_comps.dis_buf[1]=(hum_comps.SEG_TAB[a]>>4);
	hum_comps.dis_buf[0]=(hum_comps.SEG_TAB[a]&0x0f);
	if(hum_comps.dot1_pos==2) hum_comps.dis_buf[1]|=0x08;
	ht1621x_comps.write(_06_PIN_LCD_SEG_01,hum_comps.dis_buf[1]);
	ht1621x_comps.write(_05_PIN_LCD_SEG_00,hum_comps.dis_buf[0]);
}
static void MD_DISPLAY_WRITE_D0_0(unsigned char a)	//  D00  H   L 
{
	hum_comps.dis_buf[17]=(hum_comps.SEG_TAB[a]>>4);
	hum_comps.dis_buf[16]=(hum_comps.SEG_TAB[a]&0x0f);
	ht1621x_comps.write(_22_PIN_LCD_SEG_17,hum_comps.dis_buf[17]);
	ht1621x_comps.write(_21_PIN_LCD_SEG_16,hum_comps.dis_buf[16]);
}
static void MD_DISPLAY_WRITE_D0_1(unsigned char a)//  D01  H   L 
{
	hum_comps.dis_buf[15]=(hum_comps.SEG_TAB[a]>>4);
	hum_comps.dis_buf[14]=(hum_comps.SEG_TAB[a]&0x0f);
	if(hum_comps.dot0_pos==1) hum_comps.dis_buf[15]|=0x08;
	ht1621x_comps.write(_20_PIN_LCD_SEG_15,hum_comps.dis_buf[15]);
	ht1621x_comps.write(_19_PIN_LCD_SEG_14,hum_comps.dis_buf[14]);
}
static void MD_DISPLAY_WRITE_D0_2(unsigned char a)	//  D02  H   L 
{
	hum_comps.dis_buf[13]=(hum_comps.SEG_TAB[a]>>4);
	hum_comps.dis_buf[12]=(hum_comps.SEG_TAB[a]&0x0f);
	if(hum_comps.dot0_pos==2) hum_comps.dis_buf[13]|=0x08;
	ht1621x_comps.write(_18_PIN_LCD_SEG_13,hum_comps.dis_buf[13]);
	ht1621x_comps.write(_17_PIN_LCD_SEG_12,hum_comps.dis_buf[12]);
}
static void MD_DISPLAY_WRITE_D0_3(unsigned char a)	//  D03  H   L 
{
	hum_comps.dis_buf[11]=(hum_comps.SEG_TAB[a]>>4);
	hum_comps.dis_buf[10]=(hum_comps.SEG_TAB[a]&0x0f);
	if(hum_comps.dot0_pos==3) hum_comps.dis_buf[11]|=0x08;
	ht1621x_comps.write(_16_PIN_LCD_SEG_11,hum_comps.dis_buf[11]);
	ht1621x_comps.write(_15_PIN_LCD_SEG_10,hum_comps.dis_buf[10]);
}
static void MD_DISPLAY_WRITE_D0_4(unsigned char a)//  D04  H   L  
{
	hum_comps.dis_buf[9]=(hum_comps.SEG_TAB[a]>>4);
	hum_comps.dis_buf[8]=(hum_comps.SEG_TAB[a]&0x0f);
	if(hum_comps.dot0_pos==4) hum_comps.dis_buf[9]|=0x08;
	ht1621x_comps.write(_14_PIN_LCD_SEG_09,hum_comps.dis_buf[9]);
	ht1621x_comps.write(_13_PIN_LCD_SEG_08,hum_comps.dis_buf[8]);
}
 static void display_line0_data()//display 0th line data
{
  	((hum_comps.cursor==4)&&(hum_comps.dis_oper_mark&MD_OPER_CUR_MARK))?((hum_comps.dis_oper_mark&MD_OPER_DIS_MARK)?MD_DISPLAY_WRITE_D0_4(hum_comps.dig0_4):MD_DISPLAY_WRITE_D0_4(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D0_4(hum_comps.dig0_4);
	((hum_comps.cursor==3)&&(hum_comps.dis_oper_mark&MD_OPER_CUR_MARK))?((hum_comps.dis_oper_mark&MD_OPER_DIS_MARK)?MD_DISPLAY_WRITE_D0_3(hum_comps.dig0_3):MD_DISPLAY_WRITE_D0_3(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D0_3(hum_comps.dig0_3);
	((hum_comps.cursor==2)&&(hum_comps.dis_oper_mark&MD_OPER_CUR_MARK))?((hum_comps.dis_oper_mark&MD_OPER_DIS_MARK)?MD_DISPLAY_WRITE_D0_2(hum_comps.dig0_2):MD_DISPLAY_WRITE_D0_2(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D0_2(hum_comps.dig0_2);
	((hum_comps.cursor==1)&&(hum_comps.dis_oper_mark&MD_OPER_CUR_MARK))?((hum_comps.dis_oper_mark&MD_OPER_DIS_MARK)?MD_DISPLAY_WRITE_D0_1(hum_comps.dig0_1):MD_DISPLAY_WRITE_D0_1(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D0_1(hum_comps.dig0_1);
	((hum_comps.cursor==0)&&(hum_comps.dis_oper_mark&MD_OPER_CUR_MARK))?((hum_comps.dis_oper_mark&MD_OPER_DIS_MARK)?MD_DISPLAY_WRITE_D0_0(hum_comps.dig0_0):MD_DISPLAY_WRITE_D0_0(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D0_0(hum_comps.dig0_0);

}
static void display_line1_data()//display 1th line data
{
	MD_DISPLAY_WRITE_D1_2(hum_comps.dig1_2);
	MD_DISPLAY_WRITE_D1_1(hum_comps.dig1_1);
	MD_DISPLAY_WRITE_D1_0(hum_comps.dig1_0);
}
//return low level key_valve
static int get_key(hum_comps_t *const this )
{
	static unsigned long key_delay_count=0;
        static int           current_key=MD_KEY_MASK;
	int                  key_valve;
	this->key_type=EM_NO_KEY;
	key_valve=(MD_KEY_DATA&MD_KEY_MASK);
	if(key_valve!=current_key)
	{
		current_key=key_valve;
		key_delay_count=1002;
		key_valve=MD_KEY_MASK;
	}
	else
	{
		if(key_delay_count>0)
		{
			key_delay_count--;
		}
		if(key_delay_count==1000)
		{
			if((key_valve&MD_KEY_MASK)!=MD_KEY_MASK)
			{
				this->key_type=EM_SHORT_KEY;
				this->key=key_valve;
			}
		}
		else if(key_delay_count==2)
		{
			if((key_valve&MD_KEY_MASK)!=MD_KEY_MASK)
			{
				this->key_type=EM_LONG_KEY;
				this->key=key_valve;
			}
		}
		else
		{
			key_valve=MD_KEY_MASK;
		}
	}
	return key_valve;
}







/*********************************start  key function************************************************************/

static void on_s_key()
{
	mode_comps[hum_comps.current_mode].on_s_key();
}
static void on_m_key()
{
	mode_comps[hum_comps.current_mode].on_m_key();
}
static void on_j_key()
{
	mode_comps[hum_comps.current_mode].on_j_key();
}
static void on_long_s_key()
{
	mode_comps[hum_comps.current_mode].on_long_s_key();
}
static void on_long_m_key()
{
	mode_comps[hum_comps.current_mode].on_long_m_key();
}
static void on_long_s_and_j_key()
{
	mode_comps[hum_comps.current_mode].on_long_s_and_j_key();
}


static void normal_mode_on_s_key()
{
	
}
static void normal_mode_on_m_key()
{
	mode_comps[hum_comps.current_mode].dis_option++;
	mode_comps[hum_comps.current_mode].dis_option%=4;
	hum_comps.dis_oper_mark|=MD_OPER_REFRESH_DATA_MARK;
}
static void normal_mode_on_j_key()
{
	
}
static void normal_mode_on_long_m_key()
{
	hum_comps.current_mode=EM_PWD_MODE;
	hum_comps.dis_oper_mark|=MD_OPER_CUR_MARK;
	hum_comps.dis_oper_mark|=MD_OPER_DIS_MARK;
	hum_comps.dig0_0=0;
	hum_comps.dig0_1=0;
	hum_comps.dig0_2=0;
	hum_comps.dig0_3=0;
	hum_comps.dig0_4=0;
	hum_comps.cursor=4;
	hum_comps.dot0_pos=0;
	hum_comps.cursor_count=0;
	
	hum_comps.dig1_0=MD_HIDE_DISP;
	hum_comps.dig1_1=MD_HIDE_DISP;
	hum_comps.dig1_2=MD_HIDE_DISP;
	hum_comps.dot1_pos=0;
	HID_S1_S2_S3_S4;
	display_line0_data();
	display_line1_data();
	
}
static void normal_mode_on_long_s_and_j_key()
{
	hum_comps.current_mode=EM_DEBUG_MODE;
	hum_comps.dis_oper_mark|=MD_OPER_REFRESH_FREQ_MARK;
	hum_comps.dis_oper_mark|=MD_OPER_REFRESH_TEMP_MARK;
}
static void debug_mode_on_s_key()
{
	
}
static void debug_mode_on_m_key()
{
	
}
static void debug_mode_on_j_key()
{
	
}
static void debug_mode_on_long_m_key()
{


}
static void debug_mode_on_long_s_and_j_key()
{
	hum_comps.current_mode=EM_NORMAL_MODE;
	hum_comps.dis_oper_mark|=MD_OPER_REFRESH_DATA_MARK;
	hum_comps.dis_oper_mark|=MD_OPER_REFRESH_TEMP_MARK;
}

static void pwd_mode_on_s_key()
{
	hum_comps.cursor+=4;
	hum_comps.cursor%=5;//4->3->2...>0
}
static void pwd_mode_on_m_key()
{
	unsigned long pwd=(unsigned long)hum_comps.dig0_4*10000+(unsigned long)hum_comps.dig0_3*1000+(unsigned long)hum_comps.dig0_2*100+hum_comps.dig0_1*10+hum_comps.dig0_0;
	if(pwd==57481)
	{
		hum_comps.current_mode=EM_QUERY_MODE;
		hum_comps.dis_oper_mark|=MD_OPER_REFRESH_PARAM_MARK;
		hum_comps.dis_oper_mark|=MD_OPER_REFRESH_OPTION_MARK;
		hum_comps.dis_oper_mark&=~MD_OPER_CUR_MARK;
		mode_comps[hum_comps.current_mode].dis_option=0;
		hum_comps.accessible_data=6;
	}
	else if(pwd==11111)
	{
		hum_comps.current_mode=EM_QUERY_MODE;
		hum_comps.dis_oper_mark|=MD_OPER_REFRESH_PARAM_MARK;
		hum_comps.dis_oper_mark|=MD_OPER_REFRESH_OPTION_MARK;
		hum_comps.dis_oper_mark&=~MD_OPER_CUR_MARK;
		mode_comps[hum_comps.current_mode].dis_option=3;
		hum_comps.accessible_data=7;
	}
	else
	{
		NOP();
	}
}
static void pwd_mode_on_j_key()
{
	*(&hum_comps.dig0_0+hum_comps.cursor)=(*(&hum_comps.dig0_0+hum_comps.cursor)+1)%10;
}
static void pwd_mode_on_long_m_key()
{
	hum_comps.current_mode=EM_NORMAL_MODE;
	hum_comps.dis_oper_mark|=MD_OPER_REFRESH_DATA_MARK;
	hum_comps.dis_oper_mark|=MD_OPER_REFRESH_TEMP_MARK;
	hum_comps.dis_oper_mark&=~MD_OPER_CUR_MARK;
}
static void pwd_mode_on_long_s_and_j_key()
{
	
}


static void query_mode_on_s_key()
{
	hum_comps.current_mode=EM_MODIFY_MODE;
	mode_comps[hum_comps.current_mode].dis_option=mode_comps[EM_QUERY_MODE].dis_option;
	hum_comps.dis_oper_mark|=MD_OPER_CUR_MARK;
	hum_comps.dis_oper_mark|=MD_OPER_DIS_MARK;
	if(mode_comps[hum_comps.current_mode].dis_option==5)
	{
	     hum_comps.cursor=0;
	}
	else
	{
	    hum_comps.cursor=4;
	}
	hum_comps.cursor_count=0;
}
static void query_mode_on_m_key()
{
	if(hum_comps.accessible_data==6)
	{
		mode_comps[hum_comps.current_mode].dis_option++;
		mode_comps[hum_comps.current_mode].dis_option%=3;
		
	}
	else if(hum_comps.accessible_data==7)
	{
		mode_comps[hum_comps.current_mode].dis_option++;
		mode_comps[hum_comps.current_mode].dis_option%=9;
		if(!mode_comps[hum_comps.current_mode].dis_option)
		{
		    mode_comps[hum_comps.current_mode].dis_option=3;
		}
	}
	hum_comps.dis_oper_mark|=MD_OPER_REFRESH_PARAM_MARK;
	hum_comps.dis_oper_mark|=MD_OPER_REFRESH_OPTION_MARK;
}
static void query_mode_on_j_key()
{
	
}
static void query_mode_on_long_m_key()
{
	hum_comps.current_mode=EM_NORMAL_MODE;
	hum_comps.dis_oper_mark|=MD_OPER_REFRESH_DATA_MARK;
	hum_comps.dis_oper_mark|=MD_OPER_REFRESH_TEMP_MARK;
	hum_comps.dis_oper_mark&=~MD_OPER_CUR_MARK;
}
static void query_mode_on_long_s_and_j_key()
{
	

}


static void modify_mode_on_s_key()
{
    if(mode_comps[hum_comps.current_mode].dis_option!=5)
    {
        hum_comps.cursor+=4;
	    hum_comps.cursor%=5;//4->3->2...>0
    }
}
static void modify_mode_on_m_key()
{
	unsigned int save_data_en=0;
	unsigned long num=(unsigned long)hum_comps.dig0_4*10000+(unsigned long)hum_comps.dig0_3*1000+
								(unsigned long)hum_comps.dig0_2*100+
									       hum_comps.dig0_1*10+           
								               hum_comps.dig0_0;        
								           
								                         
	if(mode_comps[hum_comps.current_mode].dis_option<3)
	{
		if(sensor_comps.sw&MD_DENSITY_STB)
		{
			save_data_en=1;
			sensor_comps.period_tab[mode_comps[hum_comps.current_mode].dis_option]=sensor_comps.signal_period;
			sensor_comps.temp_tab  [mode_comps[hum_comps.current_mode].dis_option]=sensor_comps.current_temp;
		}
	}
	else if(mode_comps[hum_comps.current_mode].dis_option==3)//lower range
	{
		if(num<10000)
		{
			save_data_en=1;
		}
	}
	else if(mode_comps[hum_comps.current_mode].dis_option==4)//up range
	{
		if((num>10000)&&(num<=30000))
		{
			save_data_en=1;
		}
	}
	else if(mode_comps[hum_comps.current_mode].dis_option==5)//current dir [0,1]
	{
        {
            num%=10;
			save_data_en=1;
		}
	}
	else if(mode_comps[hum_comps.current_mode].dis_option<9)//current dir
	{
		if((num>5000)&&(num<15000))
		{
			save_data_en=1;
		}
	}
	
	if(save_data_en==1)
	{
		
		sensor_comps.param_tab[mode_comps[hum_comps.current_mode].dis_option]=num;
		sensor_comps.save_param();
		hum_comps.current_mode=EM_QUERY_MODE;
		HID_S5_STB;
		hum_comps.dis_oper_mark|=MD_OPER_REFRESH_PARAM_MARK;
		hum_comps.dis_oper_mark&=~MD_OPER_CUR_MARK;
	}
}
static void modify_mode_on_j_key()
{
    if(mode_comps[hum_comps.current_mode].dis_option==5)
    {
        *(&hum_comps.dig0_0+hum_comps.cursor)=(*(&hum_comps.dig0_0+hum_comps.cursor)+1)%2;
    }
    else
    {
	   *(&hum_comps.dig0_0+hum_comps.cursor)=(*(&hum_comps.dig0_0+hum_comps.cursor)+1)%10;
	}
}

static void modify_mode_on_long_s_key()
{
	hum_comps.current_mode=EM_QUERY_MODE;
	HID_S5_STB;
	hum_comps.dis_oper_mark|=MD_OPER_REFRESH_PARAM_MARK;
	hum_comps.dis_oper_mark&=~MD_OPER_CUR_MARK;
}
static void modify_mode_on_long_m_key()
{
	hum_comps.current_mode=EM_NORMAL_MODE;
	hum_comps.dis_oper_mark|=MD_OPER_REFRESH_DATA_MARK;
	hum_comps.dis_oper_mark|=MD_OPER_REFRESH_TEMP_MARK;
	hum_comps.dis_oper_mark&=~MD_OPER_CUR_MARK;
	HID_S5_STB;
}
static void modify_mode_on_long_s_and_j_key()
{
	
}
///////////////////////END KEY FUNCTION////////////////////


//////////////START DISPLAY///////////////////////////////

void display_temp()//refresh temperature
{
	unsigned char dot_pos=0;
	long num=sensor_comps.current_temp;
	DIS_S4_C;
	switch(num<0)
	{
		case 1: num=-num;
			if(num<100)
			{
				hum_comps.dig1_0=num%10;
				hum_comps.dig1_1=num/10%10;
				hum_comps.dot1_pos=1;//                                                                
				hum_comps.dig1_2=MD_HIDE_DISP-1;//dis -  
			}
			else if(num<1000)
			{
				hum_comps.dig1_0=num/10%10;
				hum_comps.dig1_1=num/100%10;
				hum_comps.dot1_pos=0;
				hum_comps.dig1_2=MD_HIDE_DISP-1;//dis -  
			}
			else
			{
				hum_comps.dig1_0=MD_HIDE_DISP;
				hum_comps.dig1_1=MD_HIDE_DISP;
				hum_comps.dig1_2=MD_HIDE_DISP-1;// Overrange display - _ _
			}
			break;
		
		case 0:
			if(num<100)
			{
				hum_comps.dig1_0=num%10;
				hum_comps.dig1_1=num/10%10;
				hum_comps.dot1_pos=1;//P2    
				hum_comps.dig1_2=MD_HIDE_DISP;
			}
			else if(num<1000)
			{
				hum_comps.dig1_0=num%10;
				hum_comps.dig1_1=num/10%10;
				hum_comps.dot1_pos=1;//P2     
				hum_comps.dig1_2=num/100%10;
			}
			else if(num<10000)
			{
				hum_comps.dig1_0=num/10%10;
				hum_comps.dot1_pos=0;//  
				hum_comps.dig1_1=num/100%10;
				hum_comps.dig1_2=num/1000%10;
			}
			else   // Overrange display 1_ _
			{
				hum_comps.dig1_0=MD_HIDE_DISP;
				hum_comps.dig1_1=MD_HIDE_DISP;
				hum_comps.dig1_2=1;             
			}
			break;
		default:
			break;
	}
	display_line1_data();//display 1 line data
	
	
}
static void hide_zero(unsigned char reserved_num)//Number of digits reserved
{
	int i=0;
	unsigned char  *buf=&hum_comps.dig0_4;
	for(i=0;i<5-reserved_num;i++)
	{
		if(*(buf-i)==0)
		{
			*(buf-i)=MD_HIDE_DISP;
		}
		else
		{
			break;	
		}
	}
}

static void normal_mode_display(unsigned char opt)
{
    	if(opt<2)
    	{
	    	if(hum_comps.dis_oper_mark&MD_OPER_REFRESH_DATA_MARK)
	    	{
                long num =sensor_comps.current_density;
                if(opt==1)
                {
                    #if(MD_DEVICE==MD_CONSICY)
        	    		num=(num/10000.*193.798-189)*100;
        	    		if(num<0)
        	    		{
        	    		    num=0;
        	    		}
        	    		else if(num>10000)
        	    		{
                            num=10000;
        	    		}
                    #endif
	    		}
	    		
	    		hum_comps.dig0_0=num%10;
	    		hum_comps.dig0_1=num/10%10;
	    		hum_comps.dig0_2=num/100%10;
	    		hum_comps.dig0_3=num/1000%10;
	    		hum_comps.dig0_4=num/10000%10;
	    		switch(opt)
	    		{
	    			case 0:  
	    			    hide_zero(5);
	    				hum_comps.dot0_pos=4;// 0.000
	    				display_line0_data();    // g/cm^3 0.000
	    				DIS_S1_G_CM_3; 
                        break;
                      
	    			case 1: 
	    			  #if(MD_DEVICE==MD_DENSITY) 
	    				hide_zero(2);//reserved 2 digits 
	    				hum_comps.dot0_pos=1;// 0.0  // kg/m^3 0.0
                        DIS_S2_KG_M_3; 
                      #elif(MD_DEVICE==MD_CONSICY)
                        hide_zero(3);//reserved 2 digits 
	    				hum_comps.dot0_pos=2;// 0.00  // %
                        DIS_S3_PPM;
	    			  #endif
	    			  display_line0_data();
	    			    break;	
	    		}
	    		hum_comps.dis_oper_mark&=~MD_OPER_REFRESH_DATA_MARK;
	    	}
	}
	else if(opt==2)
	{
	        unsigned long num=MD_DEVICE_ADDR;
	    	hum_comps.dig0_0=num%10;
	        hum_comps.dig0_1=num/10%10;
	        hum_comps.dig0_2=num/100%10;
	        hum_comps.dig0_3=MD_HIDE_DISP;
	        hum_comps.dig0_4=0x0a;
	        hum_comps.dot0_pos=0;
	        display_line0_data();
	        HID_S1_S2_S3;

	}
	else if(opt==3)
	{
	        
	    	hum_comps.dig0_0=2;
	        hum_comps.dig0_1=0;
	        hum_comps.dig0_2=MD_HIDE_DISP-1;
	        hum_comps.dig0_3=0x0f;
	        hum_comps.dig0_4=MD_HIDE_DISP;
	        hum_comps.dot0_pos=1;
	        display_line0_data();
	        HID_S1_S2_S3;

	}
	if(hum_comps.dis_oper_mark&MD_OPER_REFRESH_TEMP_MARK)
	{
		display_temp();//refresh temperature
		hum_comps.dis_oper_mark&=~MD_OPER_REFRESH_TEMP_MARK;
	}
	
	
}
static void debug_mode_display(unsigned char opt)
{
	if(hum_comps.dis_oper_mark&MD_OPER_REFRESH_FREQ_MARK)
	{
		unsigned long num=sensor_comps.signal_freq;   
		if(sensor_comps.signal_freq<7500)//high low level T=2*0x1ffff*(1/24)=10.923ms    f=1/T=100hz
		{
			num=(unsigned long)sensor_comps.signal_ferq_from_timer_ch0*100;
		}
		if(num<100000)//1000hz
		{
			hum_comps.dig0_0=num%10;
			hum_comps.dig0_1=num/10%10;
			hum_comps.dig0_2=num/100%10;
			hum_comps.dig0_3=num/1000%10;
			hum_comps.dig0_4=num/10000%10;
			hum_comps.dot0_pos=2;// 0.0
		}
		else
		{
			
			hum_comps.dig0_0=num/10%10;
			hum_comps.dig0_1=num/100%10;
			hum_comps.dig0_2=num/1000%10;
			hum_comps.dig0_3=num/10000%10;
			hum_comps.dig0_4=num/100000%10;
			hum_comps.dot0_pos=1;// 0.0
		}
		display_line0_data();  
		HID_S1_S2_S3; 
		hum_comps.dis_oper_mark&=~MD_OPER_REFRESH_FREQ_MARK;
	}
	if(hum_comps.dis_oper_mark&MD_OPER_REFRESH_TEMP_MARK)
	{
		display_temp();//refresh temperature
		hum_comps.dis_oper_mark&=~MD_OPER_REFRESH_TEMP_MARK;
	}
	
}
static void pwd_mode_display(unsigned char opt)
{
	if(hum_comps.dis_oper_mark&MD_OPER_CUR_MARK)
	{
		hum_comps.cursor_count++;
		if(hum_comps.cursor_count>100)
		{
		        hum_comps.cursor_count=0;
			if(hum_comps.dis_oper_mark&MD_OPER_DIS_MARK)
			{
				hum_comps.dis_oper_mark&=~MD_OPER_DIS_MARK;
			}
			else
			{
				hum_comps.dis_oper_mark|=MD_OPER_DIS_MARK;
			}
			display_line0_data();
		}
	}
}
static void query_mode_display(unsigned char opt)
{
	if(hum_comps.dis_oper_mark&MD_OPER_REFRESH_PARAM_MARK)
	{
		unsigned long num=sensor_comps.param_tab[mode_comps[hum_comps.current_mode].dis_option];
		hum_comps.dig0_0=num%10;
		hum_comps.dig0_1=num/10%10;
		hum_comps.dig0_2=num/100%10;
		hum_comps.dig0_3=num/1000%10;
		hum_comps.dig0_4=num/10000%10;
		hum_comps.dot0_pos=4;// 0.0
		if(mode_comps[hum_comps.current_mode].dis_option==5)//4-20ma current dir
		{
		    hide_zero(1);
		    hum_comps.dot0_pos=0;// 0.0
		}
		display_line0_data(); 
		if(mode_comps[hum_comps.current_mode].dis_option<5)
		{
			DIS_S1_G_CM_3;
		}
		else
		{
			HID_S1_S2_S3; 
		}
		
		hum_comps.dis_oper_mark&=~MD_OPER_REFRESH_PARAM_MARK;
	}
	if(hum_comps.dis_oper_mark&MD_OPER_REFRESH_OPTION_MARK)
	{
		unsigned long num=mode_comps[hum_comps.current_mode].dis_option;
		hum_comps.dig1_0=num%10;
		hum_comps.dig1_1=num/10%10;
		hum_comps.dig1_2=num/100%10;
	        hum_comps.dot1_pos=0;
		display_line1_data(); 
		HID_S4_C;// 
		hum_comps.dis_oper_mark&=~MD_OPER_REFRESH_OPTION_MARK;
	}
}
static void modify_mode_display(unsigned char opt)
{
	if(hum_comps.dis_oper_mark&MD_OPER_CUR_MARK)
	{
		hum_comps.cursor_count++;
		if(hum_comps.cursor_count>100)
		{
		        hum_comps.cursor_count=0;
			if(hum_comps.dis_oper_mark&MD_OPER_DIS_MARK)
			{
				hum_comps.dis_oper_mark&=~MD_OPER_DIS_MARK;
			}
			else
			{
				hum_comps.dis_oper_mark|=MD_OPER_DIS_MARK;
			}
			display_line0_data();
			if(sensor_comps.sw&MD_DENSITY_STB)
			{
				 DIS_S5_STB; 
			}
			else
			{
				FLASH_S5_STB;
			}
		}
	}
	
}

static void self_test_mode_display()
{
	if(hum_comps.dis_oper_mark& MD_OPER_REFRESH_ERR_CODE_MARK)
	{
		hum_comps.dig0_0=0;
		hum_comps.dig0_1=0;
		hum_comps.dig0_2=0;
		hum_comps.dig0_3=0;
		if(sensor_comps.sw&MD_E2PROM_DRIVER_ERR)
		{
			hum_comps.dig0_0=1;
		}
		if(sensor_comps.sw&MD_AD54X0_DRIVER_ERR)
		{
			hum_comps.dig0_1=1;
		}
		if(sensor_comps.sw&MD_DS18B20_DRIVER_ERR)
		{
			hum_comps.dig0_2=1;
		}
		hum_comps.dig0_4=0x0e;
		hum_comps.dot0_pos=0;// 
		display_line0_data();
		
		hum_comps.dig1_0= MD_HIDE_DISP ;
		hum_comps.dig1_1= MD_HIDE_DISP ;
		hum_comps.dig1_2= MD_HIDE_DISP ;
		hum_comps.dot1_pos=0;// 
		display_line1_data();
		hum_comps.dis_oper_mark&=~MD_OPER_REFRESH_ERR_CODE_MARK;
	}
}
/*********************************END DISPLAY FNCTION************************************************************/


static void ht1621x_test(hum_comps_t *const this)
{
	if(this->count==0)
	{
		MD_DISPLAY_WRITE_D0_0(this->dig0_0);// D00 H  L 
		
	}
        if(this->count==100)
	{
		MD_DISPLAY_WRITE_D0_1(this->dig0_1);
	}
	if(this->count==200)
	{
		MD_DISPLAY_WRITE_D0_2(this->dig0_2);
	}
	if(this->count==300)
	{
		MD_DISPLAY_WRITE_D0_3(this->dig0_3);
	}
	if(this->count==400)
	{
		MD_DISPLAY_WRITE_D0_4(this->dig0_4);// D04 H  L 
	}
	if(this->count==500)
	{
		MD_DISPLAY_WRITE_D1_0(this->dig1_0);// D10 H  L  
	}
	if(this->count==600)
	{
		MD_DISPLAY_WRITE_D1_1(this->dig1_1);// D11 H  L  
	}
	if(this->count==700)
	{
		MD_DISPLAY_WRITE_D1_2(this->dig1_2);// D12 H  L  
	}
}



static void handle_key(hum_comps_t *this)
{
	this->get_key(this);
	if(this->key_type==EM_NO_KEY)
	{
		return;
	}
	else if(this->key_type==EM_SHORT_KEY)
	{
		switch(this->key)
		{
			case MD_S_KEY:
					on_s_key();
			 		break;
			case MD_M_KEY:
					on_m_key();
	 				break;
			case MD_J_KEY:
					on_j_key();
	 				break;
			default:
					break;
		}
	}
	else if(this->key_type==EM_LONG_KEY)
	{
		switch(this->key)
		{
			case MD_S_KEY:
					on_long_s_key();
					break;
			case MD_M_KEY:
					on_long_m_key();
					break;
			case MD_S_KEY&MD_J_KEY:
					on_long_s_and_j_key();
					break;	
			deauflt:
					break;
		}
	}
	
}

static void handle_lcd_refresh()
{
	
	mode_comps[hum_comps.current_mode].display(mode_comps[hum_comps.current_mode].dis_option);
	
}

static void hum_comps_init(hum_comps_t *const this)
{
	int i=0;
	ht1621x_comps.write_cmd(this->BIAS);
	ht1621x_comps.write_cmd(this->SYSEN);
	ht1621x_comps.write_cmd(this->LCDON);
	for(i=0;i<18;i++)
	{
		this->dis_buf[i]=0;
		#define  LCD_SEG(i) (i<<2)
	        ht1621x_comps.write(LCD_SEG(i),this->dis_buf[i]);//seg0-seg17 set 0
		#undef LCD_SEG(i)
	}
}


static void hum_comps_task_handle()////Execution interval is 5 ms
{
	
	hum_comps_t *const this=hum_comps.this;
	if(this->do_init==1)
	{
		this->init(this);
		this->do_init=0;
	}
	if(!(this->count%100)&&(this->count<800))
	{
		 ht1621x_test(this);//interval time 500ms
		
	}
	if(this->count==800)
	{
		handle_key(this);
		handle_lcd_refresh();
	}
    else
    {
        this->count++;
    }
}

static void nop()
{
	NOP();
}
mode_comps_t  mode_comps[]=//Handling of keys in different modes
{	//mode_comps_t  mode_comps[5];
	{"normal mode"    ,mode_comps , EM_NORMAL_MODE   ,normal_mode_on_s_key,normal_mode_on_m_key,normal_mode_on_j_key,nop                      ,normal_mode_on_long_m_key,normal_mode_on_long_s_and_j_key,normal_mode_display   ,0},
	{"debug mode"     ,mode_comps+1,EM_DEBUG_MODE    ,debug_mode_on_s_key ,debug_mode_on_m_key ,debug_mode_on_j_key ,nop                      ,debug_mode_on_long_m_key ,debug_mode_on_long_s_and_j_key ,debug_mode_display    ,0},
	{"password mode"  ,mode_comps+2,EM_PWD_MODE      ,pwd_mode_on_s_key   ,pwd_mode_on_m_key   ,pwd_mode_on_j_key   ,nop		          ,pwd_mode_on_long_m_key   ,pwd_mode_on_long_s_and_j_key   ,pwd_mode_display      ,0},
	{"query mode"     ,mode_comps+3,EM_QUERY_MODE    ,query_mode_on_s_key ,query_mode_on_m_key ,query_mode_on_j_key ,nop		          ,query_mode_on_long_m_key ,query_mode_on_long_s_and_j_key ,query_mode_display    ,0},
	{"modify mode"    ,mode_comps+4,EM_MODIFY_MODE   ,modify_mode_on_s_key,modify_mode_on_m_key,modify_mode_on_j_key,modify_mode_on_long_s_key,modify_mode_on_long_m_key,modify_mode_on_long_s_and_j_key,modify_mode_display   ,0},
	{"self_test_mode" ,mode_comps+5,EM_SELF_TEST_MODE,nop                 ,nop                 ,nop          	,nop			  ,nop               	    ,nop                            ,self_test_mode_display,0},
};
	
	
hum_comps_t hum_comps=
{
	"Human machine interface",//char desc[32];
	&hum_comps,//struct _HUM_COMPONENTS *this;
	1,//int do_init;//Whether to initialize,1:init 0,no init
	hum_comps_init,//void (*const init)(struct _HUM_COMPONENTS *);
	0,//unsigned long count;   //Called counter
	
	/****************************** START HT1621******************************/
	//0    1    2    3     4,  5,   6,   7,   8,   9,   a    b    c    d    e    f    P    U    -  NULL
	{0x5f,0x50,0x3d,0x79,0x72,0x6b,0x6f,0x51,0x7f,0x7b,0x77,0x6e,0x0f,0x7c,0x2f,0x27,0x37,0x5e,0x20,0 },//{unsigned char const SEG_TAB[20]
	0x52,//const unsigned char BIAS;
	0x02,//const unsigned char SYSEN;
	0x06,//const unsigned char LCDON;
	8,//unsigned char dig0_0;
	8,//unsigned char dig0_1;
	8,//unsigned char dig0_2;
	8,//unsigned char dig0_3;
	8,//unsigned char dig0_4;
	0,//unsigned char dot0_pos;
	8,//unsigned char dig1_0;
	8,//unsigned char dig1_1;
	8,//unsigned char dig1_2;
	0,//unsigned char dot1_pos;
	{0},//char dis_buf[20];                  //SEG0-SEG17 disbuf  ht1621x memory map
	0 ,//unsigned char dis_oper_mark;
	0,//unsigned char cursor;
	0,//unsigned char cursor_count;
	/*******************END HT1621******************************/ 
	
	EM_SELF_TEST_MODE,//mode_type_t current_mode;
	0,//Different passwords to access different data(Sensor parameter);6,accessible data 0-5,other,accessible data 6,
	
	EM_NO_KEY,//key_type_t key_type;//[EM_NO_KEY, EM_SHORT_KEY,EM_LONG_KEY]
	MD_KEY_MASK,//int key;
	get_key,//int  (*const get_key)(struct _HUM_COMPONENTS *);
	
	
	hum_comps_task_handle//void (*const task_handle)(void);
};
















