#ifndef  HT1621X_H
#define  HT1621X_H

typedef struct _HT1621X_COMPONENTS
{
	char desc[32];
	struct _HT1621X_COMPONENTS *this;
	
	void (*const write_cmd)(unsigned char cmd);
	void (*const write_all)(unsigned char addr,void const *p,unsigned char cnt);
	void (*const write)(unsigned char addr,unsigned char data);
	
}ht1621x_comps_t;

extern ht1621x_comps_t ht1621x_comps;

#define  _05_PIN_LCD_SEG_00    (0x00<<2)
#define  _06_PIN_LCD_SEG_01    (0x01<<2)
#define  _07_PIN_LCD_SEG_02    (0x02<<2)
#define  _08_PIN_LCD_SEG_03    (0x03<<2)
#define  _09_PIN_LCD_SEG_04    (0x04<<2)
#define  _10_PIN_LCD_SEG_05    (0x05<<2)
#define  _11_PIN_LCD_SEG_06    (0x06<<2)
#define  _12_PIN_LCD_SEG_07    (0x07<<2)
#define  _13_PIN_LCD_SEG_08    (0x08<<2)
#define  _14_PIN_LCD_SEG_09    (0x09<<2)
#define  _15_PIN_LCD_SEG_10    (0x0a<<2)
#define  _16_PIN_LCD_SEG_11    (0x0b<<2)
#define  _17_PIN_LCD_SEG_12    (0x0c<<2)
#define  _18_PIN_LCD_SEG_13    (0x0d<<2)
#define  _19_PIN_LCD_SEG_14    (0x0e<<2)
#define  _20_PIN_LCD_SEG_15    (0x0f<<2)
#define  _21_PIN_LCD_SEG_16    (0x10<<2)
#define  _22_PIN_LCD_SEG_17    (0x11<<2)


#define  DIS_P1         	hum_comps.dis_buf[0x01]|=0x08,ht1621x_comps.write( _06_PIN_LCD_SEG_01, hum_comps.dis_buf[0x01])
#define  DIS_P2    		hum_comps.dis_buf[0x03]|=0x08,ht1621x_comps.write( _08_PIN_LCD_SEG_03, hum_comps.dis_buf[0x03])
#define  DIS_M      		hum_comps.dis_buf[0x05]|=0x08,ht1621x_comps.write( _10_PIN_LCD_SEG_05, hum_comps.dis_buf[0x05])
#define  DIS_P4     		hum_comps.dis_buf[0x09]|=0x08,ht1621x_comps.write( _14_PIN_LCD_SEG_09, hum_comps.dis_buf[0x09])
#define  DIS_P5     		hum_comps.dis_buf[0x0b]|=0x08,ht1621x_comps.write( _16_PIN_LCD_SEG_11, hum_comps.dis_buf[0x0b])
#define  DIS_P6     		hum_comps.dis_buf[0x0d]|=0x08,ht1621x_comps.write( _18_PIN_LCD_SEG_13, hum_comps.dis_buf[0x0d])
#define  DIS_P7     		hum_comps.dis_buf[0x0f]|=0x08,ht1621x_comps.write( _20_PIN_LCD_SEG_15, hum_comps.dis_buf[0x0f])
#define  DIS_S1_G_CM_3  	hum_comps.dis_buf[0x06]&=~0x06,hum_comps.dis_buf[0x06]|=0x01,ht1621x_comps.write( _11_PIN_LCD_SEG_06, hum_comps.dis_buf[0x06])
#define  DIS_S2_KG_M_3  	hum_comps.dis_buf[0x06]&=~0x05,hum_comps.dis_buf[0x06]|=0x02,ht1621x_comps.write( _11_PIN_LCD_SEG_06, hum_comps.dis_buf[0x06])
#define  DIS_S3_PPM   		hum_comps.dis_buf[0x06]&=~0x03,hum_comps.dis_buf[0x06]|=0x04,ht1621x_comps.write( _11_PIN_LCD_SEG_06, hum_comps.dis_buf[0x06])
#define  HID_S1_S2_S3   	hum_comps.dis_buf[0x06]&=~0x07,ht1621x_comps.write( _11_PIN_LCD_SEG_06, hum_comps.dis_buf[0x06])
#define  HID_S1_S2_S3_S4   	hum_comps.dis_buf[0x06]&=~0x0f,ht1621x_comps.write( _11_PIN_LCD_SEG_06, hum_comps.dis_buf[0x06])
#define  DIS_S4_C     		hum_comps.dis_buf[0x06]|=0x08,ht1621x_comps.write( _11_PIN_LCD_SEG_06, hum_comps.dis_buf[0x06])
#define  HID_S4_C               hum_comps.dis_buf[0x06]&=~0x08,ht1621x_comps.write( _11_PIN_LCD_SEG_06, hum_comps.dis_buf[0x06])
#define  DIS_S5_STB     	hum_comps.dis_buf[0x07]|=0x01,ht1621x_comps.write( _12_PIN_LCD_SEG_07, hum_comps.dis_buf[0x07])
#define  HID_S5_STB     	hum_comps.dis_buf[0x07]&=~0x01,ht1621x_comps.write( _12_PIN_LCD_SEG_07, hum_comps.dis_buf[0x07])
#define  FLASH_S5_STB           hum_comps.dis_buf[0x07]^=0x01,ht1621x_comps.write( _12_PIN_LCD_SEG_07, hum_comps.dis_buf[0x07])
#define  DIS_S6_LOGO    	hum_comps.dis_buf[0x07]|=0x02,ht1621x_comps.write( _12_PIN_LCD_SEG_07, hum_comps.dis_buf[0x07])

#endif