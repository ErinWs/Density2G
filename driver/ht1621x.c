//#include "r_cg_macrodriver.h"
//#include "ht1621.h"
//#include "densi.h"
//#include "e2prom.h"
//#include "r_cg_adc.h"
//#include "r_cg_wdt.h"
////                           0    1    2    3     4,  5,   6,   7,   8,   9,   a    b    c    d    e    f    P    U 
//const unsigned char zimo[]={0x5f,0x50,0x3d,0x79,0x72,0x6b,0x6f,0x51,0x7f,0x7b,0x77,0x6e,0x0f,0x7c,0x2f,0x27,0x37,0x5e};
//unsigned char       Lcd_Data_Reg[18]={0};
//
//unsigned char  Current_Cursor=0;
//unsigned char Calibrat_Scan_Or_Rewrite='S';
//
//
//unsigned char Key_Down_Reg=0;
//unsigned char Calibrat_Key_Dis_Index;
//unsigned char Normal_Key_Dis_Index=0;
//unsigned char Work_Mode=NORMAL_MODE;
//unsigned long Long_M_Key_Count=0;
//unsigned long Long_Debug_Key_Count=0;
//unsigned long Loop_Count=0;
//unsigned char Calibrated_Flag=0;
//
//unsigned char PassWord=0;
//unsigned long PassWord_Num=0;
//
//
//
//
//void SendBit_1621(unsigned char data,unsigned char cnt)		//data¦Ì???cnt??D¡ä¨¨?HT1621¡ê??????¨²?¡ã
//{
//	unsigned char i;
//	for(i =0; i <cnt; i ++)
//	{
//		if((data&0x80)==0)
//		dat=0;
//		else
//		dat=1;
//		wr=0;
//		delay_us(5);
//		delay_us(5);
//		delay_us(5);
//		delay_us(5);
//		
//		wr=1;
//		data<<=1;
//		delay_us(5);
//		delay_us(5);
//		
//		
//		
//	}
//}
//
//void SendDataBit_1621(unsigned char data,unsigned char cnt)	//data¦Ì?¦Ì¨ªcnt??D¡ä¨¨?HT1621¡ê?¦Ì¨ª???¨²?¡ã
//{
//	unsigned char i;
//	for(i =0; i <cnt; i ++)
//	{
//		if((data&0x01)==0) dat=0;
//		else dat=1;
//		wr=0;
//		delay_us(5);
//		delay_us(5);
//		delay_us(5);
//		delay_us(5);
//		
//		wr=1;
//		data>>=1;
//		delay_us(5);
//		delay_us(5);
//		
//		
//		
//	}
//}
//
//void SendCmd(unsigned char command)
//{
//	cs=0;
//	SendBit_1621(0x80,4);			//D¡ä¨¨?¡À¨º????"100"o¨ª9??command?¨¹¨¢?¡ê?¨®¨¦¨®¨²
//	SendBit_1621(command,8); 		//??¨®D¨º1¨®D¦Ì??¨¹??¨º¡À?¨®¨º?3?¦Ì¨¨?¨¹¨¢?¡ê??a¨¢?¡À¨¤3¨¬¡¤?¡À?
//	cs=1;						//?¡À?¨®??command¦Ì?¡Á?????D¡ä"0"
//}
//
//void Write_1621(unsigned char addr,unsigned char data)
//{
//	cs=0;
//	SendBit_1621(0xa0,3);			//D¡ä¨¨?¡À¨º????"101"
//	SendBit_1621(addr,6);			//D¡ä¨¨?addr¦Ì???6??
//	SendDataBit_1621(data,4);		//D¡ä¨¨?data¦Ì?¦Ì¨ª4??
//	cs=1;
//}
//
//void WriteAll_1621(unsigned char addr,unsigned char *p,unsigned char cnt)
//{
//	unsigned char i;
//	cs=0;
//	SendBit_1621(0xa0,3);			//D¡ä¨¨?¡À¨º????"101"
//	SendBit_1621(addr,6);			//D¡ä¨¨?addr¦Ì???6??
//	for(i =0; i <cnt; i ++,p++)		//¨¢?D?D¡ä¨¨?¨ºy?Y
//	{
//		SendDataBit_1621(*p,8);
//	}
//	cs=1;
//}
//
//void Init_1621(void)
//{
//	SendCmd(BIAS);		//????????
//	SendCmd(SYSEN);	//???????
//	SendCmd(LCDON);	//??LCD?????
//}
//
//
//void Clr_Lcd()
//{
//	Lcd_Data_Reg[17]=0;
//	Lcd_Data_Reg[16]=0; 
//	Lcd_Data_Reg[15]=0;
//	Lcd_Data_Reg[14]=0;
//	Lcd_Data_Reg[13]=0;
//	Lcd_Data_Reg[12]=0;
//	Lcd_Data_Reg[11]=0;
//	Lcd_Data_Reg[10]=0;
//	Lcd_Data_Reg[9]=0;
//	Lcd_Data_Reg[8]=0;
//	Lcd_Data_Reg[7]=0;
//	Lcd_Data_Reg[6]=0;
//	Lcd_Data_Reg[5]=0;
//	Lcd_Data_Reg[4]=0;
//	Lcd_Data_Reg[3]=0;
//	Lcd_Data_Reg[2]=0;
//	Lcd_Data_Reg[1]=0;
//	Lcd_Data_Reg[0]=0;
//	Write_1621(_22_PIN_LCD_ADD_17,Lcd_Data_Reg[17]);
//	Write_1621(_21_PIN_LCD_ADD_16,Lcd_Data_Reg[16]);
//	Write_1621(_20_PIN_LCD_ADD_15,Lcd_Data_Reg[15]);
//	Write_1621(_19_PIN_LCD_ADD_14,Lcd_Data_Reg[14]);
//	Write_1621(_18_PIN_LCD_ADD_13,Lcd_Data_Reg[13]);
//	Write_1621(_17_PIN_LCD_ADD_12,Lcd_Data_Reg[12]);
//	Write_1621(_16_PIN_LCD_ADD_11,Lcd_Data_Reg[11]);
//	Write_1621(_15_PIN_LCD_ADD_10,Lcd_Data_Reg[10]);
//	Write_1621(_14_PIN_LCD_ADD_09,Lcd_Data_Reg[9]);
//	Write_1621(_13_PIN_LCD_ADD_08,Lcd_Data_Reg[8]);
//	Write_1621(_12_PIN_LCD_ADD_07,Lcd_Data_Reg[7]);
//	Write_1621(_11_PIN_LCD_ADD_06,Lcd_Data_Reg[6]);
//	Write_1621(_10_PIN_LCD_ADD_05,Lcd_Data_Reg[5]);
//	Write_1621(_09_PIN_LCD_ADD_04,Lcd_Data_Reg[4]); 
//	Write_1621(_08_PIN_LCD_ADD_03,Lcd_Data_Reg[3]);
//	Write_1621(_07_PIN_LCD_ADD_02,Lcd_Data_Reg[2]); 
//	Write_1621(_06_PIN_LCD_ADD_01,Lcd_Data_Reg[1]);
//	Write_1621(_05_PIN_LCD_ADD_00,Lcd_Data_Reg[0]);
//	       
//	
//}
//
//void Delay_500ms()
//{
//	unsigned int i=0;
//	unsigned int j=0;
//	for(i=0;i<4000;i++)
//	{
//		for (j=0;j<200;j++)
//		{
//			delay_us(5);
//		}
//	}
//}
//	
//	
//
//void Lcd_Test()
//{
//	unsigned char i=0;
//	Clr_Lcd();
//	
//	Lcd_Data_Reg[17]|=(zimo[0]>>4);              //H  D00
//	Lcd_Data_Reg[16]|=(zimo[0]&0x0f);           //L 
//	Write_1621(_22_PIN_LCD_ADD_17,Lcd_Data_Reg[17]);
//	Write_1621(_21_PIN_LCD_ADD_16,Lcd_Data_Reg[16]);
//	R_WDT_Restart();
//	Delay_500ms();
//	
//	Lcd_Data_Reg[15]|=(zimo[0]>>4);            // D01
//	Lcd_Data_Reg[14]|=(zimo[0]&0x0f);
//	Write_1621(_20_PIN_LCD_ADD_15,Lcd_Data_Reg[15]);
//	Write_1621(_19_PIN_LCD_ADD_14,Lcd_Data_Reg[14]);
//	R_WDT_Restart();
//	Delay_500ms();
//	
//	Lcd_Data_Reg[13]|=(zimo[0]>>4);              //D02
//	Lcd_Data_Reg[12]|=(zimo[0]&0x0f);
//	Write_1621(_18_PIN_LCD_ADD_13,Lcd_Data_Reg[13]);
//	Write_1621(_17_PIN_LCD_ADD_12,Lcd_Data_Reg[12]);
//	R_WDT_Restart();
//	Delay_500ms();
//	
//	Lcd_Data_Reg[11]|=(zimo[0]>>4);             //D03
//	Lcd_Data_Reg[10]|=(zimo[0]&0x0f);
//	Write_1621(_16_PIN_LCD_ADD_11,Lcd_Data_Reg[11]);
//	Write_1621(_15_PIN_LCD_ADD_10,Lcd_Data_Reg[10]);
//	R_WDT_Restart();
//	Delay_500ms();
//	
//	Lcd_Data_Reg[9]|=(zimo[0]>>4);            //D04
//	Lcd_Data_Reg[8]|=(zimo[0]&0x0f);
//	Write_1621(_14_PIN_LCD_ADD_09,Lcd_Data_Reg[9]);
//	Write_1621(_13_PIN_LCD_ADD_08,Lcd_Data_Reg[8]);
//	R_WDT_Restart();
//	Delay_500ms();
//	
//	Lcd_Data_Reg[5]=(zimo[0]>>4);              //H  D10
//	Lcd_Data_Reg[4]=(zimo[0]&0x0f);           //L  
//	Write_1621(_10_PIN_LCD_ADD_05,Lcd_Data_Reg[5]);
//	Write_1621(_09_PIN_LCD_ADD_04,Lcd_Data_Reg[4]); 
//	R_WDT_Restart();
//	Delay_500ms();
//	
//	Lcd_Data_Reg[3]=(zimo[0]>>4);              //H  D11
//	Lcd_Data_Reg[2]=(zimo[0]&0x0f);           //L  
//	Write_1621(_08_PIN_LCD_ADD_03,Lcd_Data_Reg[3]);
//	Write_1621(_07_PIN_LCD_ADD_02,Lcd_Data_Reg[2]); 
//	R_WDT_Restart();
//	Delay_500ms();
//	
//	Lcd_Data_Reg[1]=(zimo[0]>>4);              //H  D12
//	Lcd_Data_Reg[0]=(zimo[0]&0x0f);           //L  
//	Write_1621(_06_PIN_LCD_ADD_01,Lcd_Data_Reg[1]);
//	Write_1621(_05_PIN_LCD_ADD_00,Lcd_Data_Reg[0]);
//	Delay_500ms();
////	for (i=1;i<10;i++)
////	{
////		Display(i*(unsigned long)11111,G_CM_3,i*(unsigned long)111,0,NORMAL_MODE,0,'S');
////		R_WDT_Restart();
////		Delay_500ms();
////	}
////	DIS_P1;  
////	R_WDT_Restart();
////	Delay_500ms();
////	DIS_P2;    	
////	DIS_M; 
////	R_WDT_Restart();
////	Delay_500ms();
////	DIS_P4;     	
////	DIS_P5;     	
////	DIS_P6;  
////	R_WDT_Restart();
////	Delay_500ms();
////	DIS_P7;     	
////	DIS_S1_G_CM_3; 
////	DIS_S2_KG_M_3; 
////	R_WDT_Restart();
////	Delay_500ms();
////	DIS_S3;    	
////	DIS_S4_C;  
////	R_WDT_Restart();
////	Delay_500ms();
////	DIS_S5;     	
////	DIS_S6;  
////	R_WDT_Restart();
////	Delay_500ms();
//}
//void Display(unsigned long Param_1,unsigned char Unit_1,unsigned long Param_2,
//             unsigned char Unit_2,unsigned char Dis_Mode,unsigned char Cursor,unsigned char Scan_Or_Rewrite)
//{
//	unsigned char dx;
//	unsigned char i=0;
//	Clr_Lcd();
//	if(Dis_Mode==DEBUG_MODE)
//	{
//		i=0;	
//		dx=Param_1%10;
//		Lcd_Data_Reg[17-(i*2+0)]|=(zimo[dx]>>4);             //H  D00
//		Lcd_Data_Reg[17-(i*2+1)]|=(zimo[dx]&0x0f);           //L 
//		Write_1621(((17-(i*2+0))<<2),Lcd_Data_Reg[17-(i*2+0)]);
//		Write_1621(((17-(i*2+1))<<2),Lcd_Data_Reg[17-(i*2+1)]);
//		Param_1/=10;
//		for (i=1;i<5;i++)
//		{
//			dx=Param_1%10;
//			Lcd_Data_Reg[17-(i*2+0)]|=(zimo[dx]>>4);             //H    D01
//			Lcd_Data_Reg[17-(i*2+1)]|=(zimo[dx]&0x0f);           //L 
//			Write_1621(((17-(i*2+0))<<2),Lcd_Data_Reg[17-(i*2+0)]);
//			Write_1621(((17-(i*2+1))<<2),Lcd_Data_Reg[17-(i*2+1)]);
//			Param_1/=10;
//			if(Param_1==0)
//			{
//				break;
//			}
//		}
//		DIS_P7;
//		
//		for (i=0;i<3;i++)
//		{
//			dx=Param_2%10;
//			Lcd_Data_Reg[5-(i*2+0)]|=(zimo[dx]>>4);              //H  D10
//			Lcd_Data_Reg[5-(i*2+1)]|=(zimo[dx]&0x0f);           //L 
//			Write_1621(((5-(i*2+0))<<2),Lcd_Data_Reg[5-(i*2+0)]);
//			Write_1621(((5-(i*2+1))<<2),Lcd_Data_Reg[5-(i*2+1)]);
//			Param_2/=10;
//			if(Param_2==0)
//			{
//				break;
//			}
//		}
//		DIS_S4_C; 
//		#if defined DIS_LOG_YUNYI
//		DIS_S6;
//		#endif
//		
//	}
//	if(Dis_Mode==NORMAL_MODE)
//	{
//		if(Unit_1==G_CM_3)
//		{
//			dx=Param_1%10;
//			Lcd_Data_Reg[17]|=(zimo[dx]>>4);              //H  D00
//			Lcd_Data_Reg[16]|=(zimo[dx]&0x0f);           //L 
//			Write_1621(_22_PIN_LCD_ADD_17,Lcd_Data_Reg[17]);
//			Write_1621(_21_PIN_LCD_ADD_16,Lcd_Data_Reg[16]);
//			Param_1/=10;
//			
//			dx=Param_1%10;
//			Lcd_Data_Reg[15]|=(zimo[dx]>>4);            // D01
//			Lcd_Data_Reg[14]|=(zimo[dx]&0x0f);
//			Write_1621(_20_PIN_LCD_ADD_15,Lcd_Data_Reg[15]);
//			Write_1621(_19_PIN_LCD_ADD_14,Lcd_Data_Reg[14]);
//			Param_1/=10;
//			
//			dx=Param_1%10;
//			Lcd_Data_Reg[13]|=(zimo[dx]>>4);              //D02
//			Lcd_Data_Reg[12]|=(zimo[dx]&0x0f);
//			Write_1621(_18_PIN_LCD_ADD_13,Lcd_Data_Reg[13]);
//			Write_1621(_17_PIN_LCD_ADD_12,Lcd_Data_Reg[12]);
//			Param_1/=10;
//			
//			dx=Param_1%10;
//			Lcd_Data_Reg[11]|=(zimo[dx]>>4);             //D03
//			Lcd_Data_Reg[10]|=(zimo[dx]&0x0f);
//			Write_1621(_16_PIN_LCD_ADD_11,Lcd_Data_Reg[11]);
//			Write_1621(_15_PIN_LCD_ADD_10,Lcd_Data_Reg[10]);
//			Param_1/=10;
//			
//			dx=Param_1%10; 
//			Lcd_Data_Reg[9]|=(zimo[dx]>>4);            //D04
//			Lcd_Data_Reg[8]|=(zimo[dx]&0x0f);
//			Write_1621(_14_PIN_LCD_ADD_09,Lcd_Data_Reg[9]);
//			Write_1621(_13_PIN_LCD_ADD_08,Lcd_Data_Reg[8]);
//			DIS_P4;
//			DIS_S1_G_CM_3;
//			
//			
//		}
//		if(Unit_1==KG_M_3)
//		{  
//			i=0;	
//			dx=Param_1%10;
//			Lcd_Data_Reg[17-(i*2+0)]|=(zimo[dx]>>4);             //H  D00
//			Lcd_Data_Reg[17-(i*2+1)]|=(zimo[dx]&0x0f);           //L 
//			Write_1621(((17-(i*2+0))<<2),Lcd_Data_Reg[17-(i*2+0)]);
//			Write_1621(((17-(i*2+1))<<2),Lcd_Data_Reg[17-(i*2+1)]);
//			Param_1/=10;
//			for (i=1;i<5;i++)
//			{
//				dx=Param_1%10;
//				Lcd_Data_Reg[17-(i*2+0)]|=(zimo[dx]>>4);             //H    D01
//				Lcd_Data_Reg[17-(i*2+1)]|=(zimo[dx]&0x0f);           //L 
//				Write_1621(((17-(i*2+0))<<2),Lcd_Data_Reg[17-(i*2+0)]);
//				Write_1621(((17-(i*2+1))<<2),Lcd_Data_Reg[17-(i*2+1)]);
//				Param_1/=10;
//				if(Param_1==0)
//				{
//					break;
//				}
//			}
//			DIS_P7;
//			DIS_S2_KG_M_3;
//		}
//		
//		////////////////////	
//
////		dx=Param_2%10;
////		Lcd_Data_Reg[5]=(zimo[dx]>>4);              //H  D10
////		Lcd_Data_Reg[4]=(zimo[dx]&0x0f);           //L  
////		Write_1621(_10_PIN_LCD_ADD_05,Lcd_Data_Reg[5]);
////		Write_1621(_09_PIN_LCD_ADD_04,Lcd_Data_Reg[4]);      
////		Param_2/=10;
////		
////		dx=Param_2%10;
////		Lcd_Data_Reg[3]=(zimo[dx]>>4);              //H  D11
////		Lcd_Data_Reg[2]=(zimo[dx]&0x0f);           //L  
////		Write_1621(_08_PIN_LCD_ADD_03,Lcd_Data_Reg[3]);
////		Write_1621(_07_PIN_LCD_ADD_02,Lcd_Data_Reg[2]);      
////		Param_2/=10;
////		
////		dx=Param_2%10;
////		Lcd_Data_Reg[1]=(zimo[dx]>>4);              //H  D12
////		Lcd_Data_Reg[0]=(zimo[dx]&0x0f);           //L  
////		Write_1621(_06_PIN_LCD_ADD_01,Lcd_Data_Reg[1]);
////		Write_1621(_05_PIN_LCD_ADD_00,Lcd_Data_Reg[0]);
//            
//		for (i=0;i<3;i++)
//		{
//			dx=Param_2%10;
//			Lcd_Data_Reg[5-(i*2+0)]|=(zimo[dx]>>4);              //H  D10
//			Lcd_Data_Reg[5-(i*2+1)]|=(zimo[dx]&0x0f);           //L 
//			Write_1621(((5-(i*2+0))<<2),Lcd_Data_Reg[5-(i*2+0)]);
//			Write_1621(((5-(i*2+1))<<2),Lcd_Data_Reg[5-(i*2+1)]);
//			Param_2/=10;
//			if(Param_2==0)
//			{
//				break;
//			}
//		}
//		DIS_S4_C; 
//		DIS_S4_C; 
//		#if defined DIS_LOG_YUNYI
//		DIS_S6;
//		#endif
//		
//	}
//	if(Dis_Mode==CALIBRAT_MODE)
//	{
//		static unsigned char Ev_Od=0;
//		Ev_Od^=0x01;
//		
//		dx=Param_1%10;
//		Lcd_Data_Reg[17]=(zimo[dx]>>4);              //H  D00
//		Lcd_Data_Reg[16]=(zimo[dx]&0x0f);           //L 
//		Param_1/=10;
//		
//		dx=Param_1%10;
//		Lcd_Data_Reg[15]=(zimo[dx]>>4);            // D01
//		Lcd_Data_Reg[14]=(zimo[dx]&0x0f);
//		Param_1/=10;
//		
//		dx=Param_1%10;
//		Lcd_Data_Reg[13]=(zimo[dx]>>4);              //D02
//		Lcd_Data_Reg[12]=(zimo[dx]&0x0f);
//		Param_1/=10;
//		
//		dx=Param_1%10;
//		Lcd_Data_Reg[11]=(zimo[dx]>>4);             //D03
//		Lcd_Data_Reg[10]=(zimo[dx]&0x0f);
//		Param_1/=10;
//		
//		dx=Param_1%10;  
//		Lcd_Data_Reg[9]=(zimo[dx]>>4);            //D04
//		Lcd_Data_Reg[8]=(zimo[dx]&0x0f);
//			
//		////////////////////	
//
//		
//		
//		dx=Unit_1%10;
//		Lcd_Data_Reg[1]=(zimo[dx]>>4);              //H  D12
//		Lcd_Data_Reg[0]=(zimo[dx]&0x0f);           //L  
//		
//		Write_1621(_22_PIN_LCD_ADD_17,Lcd_Data_Reg[17]);
//		Write_1621(_21_PIN_LCD_ADD_16,Lcd_Data_Reg[16]);//D00
//		
//		Write_1621(_20_PIN_LCD_ADD_15,Lcd_Data_Reg[15]);
//		Write_1621(_19_PIN_LCD_ADD_14,Lcd_Data_Reg[14]);//D01
//		
//		Write_1621(_18_PIN_LCD_ADD_13,Lcd_Data_Reg[13]);
//		Write_1621(_17_PIN_LCD_ADD_12,Lcd_Data_Reg[12]);//D02
//		
//		Write_1621(_16_PIN_LCD_ADD_11,Lcd_Data_Reg[11]);
//		Write_1621(_15_PIN_LCD_ADD_10,Lcd_Data_Reg[10]);//D03
//		
//		Write_1621(_14_PIN_LCD_ADD_09,Lcd_Data_Reg[9]);
//		Write_1621(_13_PIN_LCD_ADD_08,Lcd_Data_Reg[8]);//D04
//				
//		
//		if((PassWord==1)||(PassWord==2))
//		{
//			Write_1621(_06_PIN_LCD_ADD_01,Lcd_Data_Reg[1]);
//			Write_1621(_05_PIN_LCD_ADD_00,Lcd_Data_Reg[0]);//D12
//		}
//		if(Scan_Or_Rewrite=='R')
//		{
//			if(Cursor==0)
//			{
//				if(Ev_Od==0)
//				{
//					Lcd_Data_Reg[9]&=~0x07;
//					Lcd_Data_Reg[8]&=~0x0f;
//					Write_1621(_14_PIN_LCD_ADD_09,Lcd_Data_Reg[9]);
//					Write_1621(_13_PIN_LCD_ADD_08,Lcd_Data_Reg[8]);//D04
//				}
//			}
//			if(Cursor==1)
//			{
//				if(Ev_Od==0)
//				{
//					Lcd_Data_Reg[11]&=~0x07;
//					Lcd_Data_Reg[10]&=~0x0f;
//					Write_1621(_16_PIN_LCD_ADD_11,Lcd_Data_Reg[11]);
//					Write_1621(_15_PIN_LCD_ADD_10,Lcd_Data_Reg[10]);//D03
//				}
//			}
//			if(Cursor==2)
//			{
//				if(Ev_Od==0)
//				{
//					Lcd_Data_Reg[13]&=~0x07;
//					Lcd_Data_Reg[12]&=~0x0f;
//					Write_1621(_18_PIN_LCD_ADD_13,Lcd_Data_Reg[13]);
//					Write_1621(_17_PIN_LCD_ADD_12,Lcd_Data_Reg[12]);//D02
//				}
//			}
//			if(Cursor==3)
//			{
//				if(Ev_Od==0)
//				{
//					Lcd_Data_Reg[15]&=~0x07;
//					Lcd_Data_Reg[14]&=~0x0f;
//					Write_1621(_20_PIN_LCD_ADD_15,Lcd_Data_Reg[15]);
//					Write_1621(_19_PIN_LCD_ADD_14,Lcd_Data_Reg[14]);//D01
//				}
//			}
//			if(Cursor==4)
//			{
//				if(Ev_Od==0)
//				{
//					Lcd_Data_Reg[17]&=~0x07;
//					Lcd_Data_Reg[16]&=~0x0f;
//					Write_1621(_22_PIN_LCD_ADD_17,Lcd_Data_Reg[17]);
//					Write_1621(_21_PIN_LCD_ADD_16,Lcd_Data_Reg[16]);//D00
//				}
//			}
//		
//		}
//		if((PassWord==1)||(PassWord==2))
//		{
//			if(Unit_1<4)
//			{
//				DIS_S1_G_CM_3;
//				DIS_P4;
//				if(Rec_Pulse_Reg&REC_COUNT_STABLE)
//				{
//					DIS_S5;//????
//				}
//				
//			}
//			if(Unit_1==4)
//			{
//				 DIS_P6; 
//				 DIS_S4_C; 
//			}
//			if((Unit_1==5)||(Unit_1==6))
//			{
//				 DIS_P5; //DIANLIU_XIUZHENGXISHU 1.000  midu_xishu
//			}
//		}
//		
//		#if defined DIS_LOG_YUNYI
//		DIS_S6;
//		#endif
//		
//	}
//}
//
//void Proc_Key()
//{
//	
//	if(Key_Down_Reg&M_KEY)
//	{
//		if(Work_Mode==NORMAL_MODE)
//		{
//			Normal_Key_Dis_Index++;
//			Normal_Key_Dis_Index%=2;
//			Current_Cursor=0;
//			Display(Current_Density,Normal_Key_Dis_Index,Current_Temp,0,Work_Mode,Current_Cursor,Calibrat_Scan_Or_Rewrite);
//		}
//		else if(Work_Mode==CALIBRAT_MODE)
//		{
//			if(PassWord==1)
//			{
//			         if(Calibrat_Scan_Or_Rewrite=='R')
//				 {
//					if(Calibrat_Key_Dis_Index<3)
//					{
//					 	if((Rec_Pulse_Cycle>MIN_CYCLE)&&(Rec_Pulse_Cycle<MAX_CYCLE)&&(Rec_Pulse_Reg&REC_COUNT_STABLE))
//						{
//							Calibrat_Scan_Or_Rewrite='S';
//							Save_Param();
//							Calibrated_Flag=0;
//						}
//					}
//					else
//					{
//						Calibrat_Scan_Or_Rewrite='S';
//						Save_Param();
//					}
//				 }
//				 else
//				 {
//					 Calibrat_Key_Dis_Index++;
//					 Calibrat_Key_Dis_Index%=(MAX_PARAM_LENGTH-1);
//					 Current_Cursor=0;
//				 	 Display(Calibrat_Density_y[Calibrat_Key_Dis_Index],Calibrat_Key_Dis_Index,0,0,Work_Mode,Current_Cursor,Calibrat_Scan_Or_Rewrite);
//				 }
//			}
//			else if(PassWord==2)
//			{
//				 if(Calibrat_Scan_Or_Rewrite=='R')
//				 {
//					if(Calibrat_Key_Dis_Index==6)
//					{
//					 	if((Calibrat_Density_y[Calibrat_Key_Dis_Index]>=MIN_MIDU_XISHU )&&(Calibrat_Density_y[Calibrat_Key_Dis_Index]<=MAX_MIDU_XISHU))
//						{
//							Calibrat_Scan_Or_Rewrite='S';
//							Save_Param();
//							Midu_XiuZheng_Xishu=Calibrat_Density_y[6];
//							Calibrated_Flag=0;
//						}
//					}
//					
//				 }
//			}
//			else
//			{
//				if(PassWord_Num==57481)
//				{
//					PassWord=1;
//					Calibrat_Key_Dis_Index=0;
//					Calibrat_Scan_Or_Rewrite='S';
//					Current_Cursor=0;
//					Display(Calibrat_Density_y[Calibrat_Key_Dis_Index],Calibrat_Key_Dis_Index,0,0,Work_Mode,Current_Cursor,Calibrat_Scan_Or_Rewrite);
//				}
//				if(PassWord_Num==11111)
//				{
//					PassWord=2;
//					Calibrat_Key_Dis_Index=6;
//					Calibrat_Scan_Or_Rewrite='S';
//					Current_Cursor=0;
//					Display(Calibrat_Density_y[Calibrat_Key_Dis_Index],Calibrat_Key_Dis_Index,0,0,Work_Mode,Current_Cursor,Calibrat_Scan_Or_Rewrite);
//				}
//				
//				
//			}
//		}
//		Key_Down_Reg&=~M_KEY;
//	}
//	if(Key_Down_Reg&S_KEY)
//	{
//		if(Work_Mode==CALIBRAT_MODE)
//		{
//			if((PassWord==1)||(PassWord==2))
//			{
//				if(Calibrat_Scan_Or_Rewrite=='S')
//				{
//					Calibrat_Scan_Or_Rewrite='R';
//				}
//				else
//				{
//					Current_Cursor++;
//					Current_Cursor%=5;
//				}
//			}
//			else
//			{
//				Current_Cursor++;
//				Current_Cursor%=5;
//			}
//		}
//		Key_Down_Reg&=~S_KEY;
//	}
//	if(Key_Down_Reg&J_KEY)
//	{
//		if(Work_Mode==CALIBRAT_MODE)
//		{
//			if((PassWord==1)||(PassWord==2))
//			{
//				if(Calibrat_Scan_Or_Rewrite=='R')
//				{
//					unsigned char Dig[7]={0};
//					Calibrated_Flag=1;
//					Dig[0]=Calibrat_Density_y[Calibrat_Key_Dis_Index]/10000%10;//W
//					Dig[1]=Calibrat_Density_y[Calibrat_Key_Dis_Index]/1000%10;//K
//					Dig[2]=Calibrat_Density_y[Calibrat_Key_Dis_Index]/100%10;//H
//					Dig[3]=Calibrat_Density_y[Calibrat_Key_Dis_Index]/10%10;//T
//					Dig[4]=Calibrat_Density_y[Calibrat_Key_Dis_Index]%10;//a
//					Dig[Current_Cursor]++;
//					Dig[Current_Cursor]%=10;
//					Calibrat_Density_y[Calibrat_Key_Dis_Index]=Dig[0]*(unsigned long)10000+Dig[1]*(unsigned long)1000
//										  +Dig[2]*(unsigned long)100+Dig[3]*10+Dig[4];
//
//				}
//				
//			}
//			else
//			{
//				unsigned char Dig[7]={0};
//				Dig[0]=PassWord_Num/10000%10;//W
//				Dig[1]=PassWord_Num/1000%10;//K
//				Dig[2]=PassWord_Num/100%10;//H
//				Dig[3]=PassWord_Num/10%10;//T
//				Dig[4]=PassWord_Num%10;//a
//				Dig[Current_Cursor]++;
//				Dig[Current_Cursor]%=10;
//				PassWord_Num=Dig[0]*(unsigned long)10000+Dig[1]*(unsigned long)1000
//				+Dig[2]*(unsigned long)100+Dig[3]*10+Dig[4];
//			}
//		}
//		Key_Down_Reg&=~J_KEY;
//	}
//	if(Key_Down_Reg&LONG_M_KEY)
//	{
//		if(Work_Mode==NORMAL_MODE)
//		{
//			PassWord=0;
//			PassWord_Num=0;
//			Work_Mode=CALIBRAT_MODE;
//			Calibrated_Flag=0;
//			Calibrat_Key_Dis_Index=0;
//			Current_Cursor=0;
//			Calibrat_Scan_Or_Rewrite='R';
//			
//		}
//		else if(Work_Mode==CALIBRAT_MODE)
//		{
//			PassWord=0;
//			PassWord_Num=0;
//			Calibrated_Flag=0;
//			Calibrat_Scan_Or_Rewrite='S';
//			Work_Mode=NORMAL_MODE;
//		}
//		Key_Down_Reg&=~LONG_M_KEY;
//	}
//	if(Key_Down_Reg&LONG_DEBUG_KEY)
//	{
//		if(Work_Mode==NORMAL_MODE)
//		{
//			Work_Mode=DEBUG_MODE;	
//		}
//		else if(Work_Mode==DEBUG_MODE)
//		{
//			Work_Mode=NORMAL_MODE;
//		}
//		Key_Down_Reg&=~LONG_DEBUG_KEY;
//	}
//	
//}

#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "r_cg_serial.h"
#include "ht1621x.h"
#include "sensor.h"
#include "r_cg_adc.h"
#include "r_cg_timer.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"



#define BIAS     0x52
#define SYSEN    0x02
#define LCDON    0x06
#define LCDOFF   0x04
#define cs       P1.0
#define wr       P1.1
#define dat      P14.7

static void delay_4us()
{
	NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
	NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
	NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
	NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
}
static void SendBit_1621(unsigned char data,unsigned char cnt)
{
	unsigned char i;
	for(i =0; i <cnt; i ++)
	{
		if((data&0x80)==0)
		dat=0;
		else
		dat=1;
		wr=0;
		delay_4us();
		wr=1;
		data<<=1;
		delay_4us();
		
	}
}

static void SendDataBit_1621(unsigned char data,unsigned char cnt)	
{
	unsigned char i;
	for(i =0; i <cnt; i ++)
	{
		if((data&0x01)==0) dat=0;
		else dat=1;
		wr=0;
		delay_4us();
		wr=1;
		data>>=1;
		delay_4us();
	}
}

static void WriteCmd(unsigned char command)
{
	cs=0;
	SendBit_1621(0x80,4);			
	SendBit_1621(command,8); 		
	cs=1;						
}

static  void WriteAll_1621(unsigned char addr,void const *p,unsigned char cnt)
{
	unsigned char i;
	unsigned  char const *P=(unsigned char *)p;
	cs=0;
	SendBit_1621(0xa0,3);			
	SendBit_1621(addr,6);			
	for(i =0; i <cnt; i ++,P++)		
	{
		SendDataBit_1621(*P,8);
	}
	cs=1;
}
static void write_ht1621x(unsigned char addr,unsigned char data)
{
	cs=0;
	SendBit_1621(0xa0,3);			
	SendBit_1621(addr,6);			
	SendDataBit_1621(data,4);		
	cs=1;

}


//static void init_ht1621x()
//{
//	WriteCmd(BIAS);		
//	WriteCmd(SYSEN);	
//	WriteCmd(LCDON);
//}




ht1621x_comps_t ht1621x_comps=
{
	"ht1621x w driver",                              //char desc[64];
	&ht1621x_comps,                  //struct _HT1621X_COMPONENTS *this;
	//0    1    2    3     4,  5,   6,   7,   8,   9,   a    b    c    d    e    f    P    U 
//	{0x5f,0x50,0x3d,0x79,0x72,0x6b,0x6f,0x51,0x7f,0x7b,0x77,0x6e,0x0f,0x7c,0x2f,0x27,0x37,0x5e},//{unsigned char const SEG_TAB[20]
	WriteCmd,                         //void (*const send_cmd(unsigned char cmd);
	WriteAll_1621,
    write_ht1621x,                   //void (*const Write  )(unsigned char ,unsigned char);
};
