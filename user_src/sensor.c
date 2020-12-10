#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_userdefine.h"
#include "r_cg_serial.h"
#include "sensor.h"
#include "hum.h"
#include "24cxx.h"
#include "ad54x0.h"
#include "ds18b20.h"
#include "r_cg_adc.h"
#include "r_cg_timer.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"


#define  MD_SET_TIMER_RESULT_0X1FFFF     for(i=0;i<MD_TIMER_MAX_POS;i++) {this->timer_result[0][i]=0x0001ffff;this->timer_result[1][i]=0x0001ffff;}
			
/////////////////////////////////////////get temperature////////////////////////////////////
/////////////////////////////////////////get temperature////////////////////////////////////
#define  MD_DENSITY_POWER_ON      P0.1=1
#define  MD_DENSITY_POWER_OFF     P0.1=0

static const long temp_tab[] = 
{

	583542,  554647, 526968,  500480, 475159, 450974, 427897, 405892, 384927,  364967, //-55...-46
	345975,  327915, 310751,  294448, 278969, 264279, 250344, 237130, 224603,  212733, //-45...-36
	201487,  190836, 180750,  171201, 162163, 153610, 145516, 137858, 130614,  123761,//-35....-26

	117280,  111149, 105351,  99867 , 94681,   89776,  85137,  80750,  76600,   72676,  // -25 .........-16
	68963 ,  65451 ,  62129,  58986 , 56012,   53198,  50539,  48013,  45627,   43368,   // -15.......... -6
	41229 ,  39204 ,  37285,  35468 , 33747,   32116,  30570,  29105,  27716,   26399,   // -5............4
	25150 ,  23965 ,  22842,  21776 , 20764,   19783,  18892,  18026,  17204,   16423,   //  5.......... 14
	15681 ,  14976 ,  14306,  13669 , 13063,   12487,  11939,  11418,  10921,   10449,    //  15...........24
	10000 ,  9571  ,   9164,  8775  ,  8405,   8052 ,  7716,    7396,   7090,    6798,     // 25...........34
	6520  ,   6255 ,   6002,  5760  ,  5529,    5309,  5098,    4897,   4704,    4521,     //  35..........44
	4345  ,   4177 ,   4016,  3863  ,  3716,    3588,  3440,    3311,   3188,    3069,    // 45...........54
	2956  ,   2848 ,   2744,  2644  ,  2548,    2457,  2369,    2284,   2204,    2126,2051, //  55..........65
	1980  ,    1911,   1845,  1782  ,  1721,    1663,  1606,    1552,   1500, 1450,  //66-75
	1402  ,    1356,   1312,  1269  ,  1228,    1188,  1150,    1113,   1078, 1044,  //76-85
	1011  ,     979,    948,  919   ,   891,     863,   837,     811,    787,  763,   //86-95
	740   ,     718,    697,  676   ,   657,     637,   619,     601,    584,  567,   //96-105
	551   ,     535,    520,  505   ,   491,     477,   464,     451,    439,  427,   //106-115
	415   ,     404,    393,  383   ,   373,     363,   353,     344,    335,  326   //116-125

}; 

static const float Alcohol_density_table[] = 
{

0.99800 ,0.99528 ,0.99243 ,0.98973 ,0.98718 ,0.98476 ,0.98233 ,0.98009 ,0.97786 ,0.9757,
0.97350 ,0.97145 ,0.96925 ,0.96699 ,0.96465 ,0.96224 ,0.95972 ,0.95703 ,0.95419 ,0.9512,
0.94805 ,0.94477 ,0.94135 ,0.93776 ,0.93444 ,0.93017 ,0.92617 ,0.92209 ,0.91789 ,0.9135,
0.90915 ,0.90463 ,0.90001 ,0.89531 ,0.89650 ,0.88558 ,0.88056 ,0.87542 ,0.87019 ,0.8648,
0.85928 ,0.85364 ,0.84786 ,0.84188 ,0.83569 ,0.82925 ,0.82246 ,0.81526 ,0.80749 ,0.799 ,0.78934

}; 

static const int Alcohol_consicy_table[] = 
{
    0     , 159  ,318    ,478    ,640   ,802   ,964  ,1128  ,1292   ,1456 ,
    1621  ,1788  ,1955   ,2122   ,2291  ,2461  ,2632  ,2804  ,2978  ,3153 ,
    3330  ,3509  ,3689   ,3872   ,4056  ,4243  ,4431  ,4623  ,4816  ,5011 ,
    5209  ,5410  ,5613   ,5819   ,6028  ,6239  ,6454  ,6672  ,6894  ,7119 ,
    7349  ,7582  ,7820   ,8063   ,8312  ,8567  ,8829  ,9101  ,9381  ,9682 ,10000

};

static const float BoMeiDu_density_table[] = 
{
    1.00,	1.01,	1.02,	1.03,	1.04,	1.05,	1.06,	1.07,	1.08,	1.09,	1.1,	1.11,	1.12,	1.13,	1.14,	1.15,	1.16,	1.17,	1.18,	1.19,	1.2,	1.21,	1.22,	1.23,	1.24,	1.25,	1.26,
    1.27,	1.28,	1.29,	1.3,	1.31,	1.32,	1.33,	1.34,	1.35,	1.36,	1.37,	1.38,	1.39,	1.4,	1.41,	1.42,	1.43,	1.44,	1.45,	1.46,	1.47,	1.48,	1.49,	1.5,	1.51,	1.52,	1.53,
    1.54,	1.55,	1.56,	1.57,	1.58,	1.59,	1.6,	1.61,	1.62,	1.63,	1.64,	1.65,	1.66,	1.67,	1.68,	1.69,	1.7,	1.71,	1.72,	1.73,	1.74,	1.75,	1.76,	1.77,	1.78,	1.79,	1.8 

}; 
    
static const int BoMeiDu_consicy_table[] = //2fixdot 
{
   
0   	,144	,284	,422	,558	,691	,821	,949	,1074	,1197	,1318	,1437	,1554	,1668	,1781	,1891	,2000	,2107	,2212	,2315	,2417	,2516	,2615	,2711	,2806	,2900	,2992,
3083	,3172	,326	,3346	,3431	,3515	,3598	,3679	,3759	,3838	,3916	,3993	,4068	,4143	,4216	,4289	,436	,4431	,4500	,4568	,4636	,4703	,4768	,4833	,4897	,496	,5023,
5084	,5145	,5205	,5264	,5323	,538	,5438	,5494	,5549	,5604	,5658	,5712	,5765	,5817	,5869	,592	,5971	,602	,607	,6118	,6167	,6214	,6261	,6308	,6354	,6399	,6444
}; 


static  long calc_adc_average(sensor_comps_t *const this)
{
    int   i=0;
    const int count=this->adc_pos;
    unsigned long average=0;
    for(i=0;i<count;i++)
    {
		average+=this-> adc_result[1][i];
    }
    average=average*10/count;	
    //free (period);
    return average;
}

static long calc_ntc_valve(sensor_comps_t *const this)
{
	long  r_std=10000;
	long  ad3_average=calc_adc_average(this);
	long  ntc_valve=(r_std*ad3_average)/(10231-ad3_average);
	return ntc_valve;
}

/*
static float calc_pt1000(sensor_comps_t *const this)
{
	int   i=0;
	int   j=0;
	float temp_var=0;
	float sum=0;
	float pt1000_valve=1000;
	const int count=this->adc_pos;
	float ratio[32];//=malloc(count*sizeof(float));
//	if(NULL==ratio)
//	{
//		NOP();//TODO reset system
//	}
	for(i=0;i<count;i++)
	{
		if(this->adc_result[1][i]==0)//If the divisor is 0, let the divisor be 1
		{
			ratio[i]=this->adc_result[2][i];
		}
		else
		{
              		ratio[i]=(float)this->adc_result[2][i]/this->adc_result[1][i];
		}
	}
        for(i=0;i<count-1;i++)
	{
		for(j=0;j<count-1-i;j++)
		{
			if(ratio[j]>ratio[j+1])
			{
				temp_var=ratio[j+1];
				ratio[j+1]=ratio[j];
				ratio[j]=temp_var;
			}
		}
	}
	for(i=2;i<count-2;i++)
	{
		sum+=ratio[i];
	}	
	pt1000_valve=(sum/(count-4)-1)*MD_STD_RES;//pt1000=(A2/A1-1)*Rstd
	this->pt1000_valve=pt1000_valve;
	//free(ratio);
	return pt1000_valve;

}
*/
static int calc_temp(sensor_comps_t  *const this)
{
    long Tem;
    long LowRValue;
    long HighRValue;        
    long   Tem0;
    int i;
    int  cBottom, cTop;
    int limit=sizeof(temp_tab)/sizeof(temp_tab[0])-1;
    

    if (this->ntc_valve >temp_tab[0])                // 电阻值小于表格最值低于量程下限。
    {
         return -550;
    }
    if (this->ntc_valve < temp_tab[limit])        // 电阻值大于表格最大值，超出量程上限 。
    {
        return 1250;
    }
    cBottom = 0; 
    cTop    = limit;
     for (i=limit/2; (cTop-cBottom)!=1; )        // 2分法查表。
    {

       if (this->ntc_valve > temp_tab[i])
        {
            cTop = i;
            i = (cTop + cBottom) / 2;
        }
        else if (this->ntc_valve < temp_tab[i])
        {
            cBottom = i;
            i = (cTop + cBottom) / 2;
        }
        else
        {
            Tem0 = i * 10 - 550;
            Tem = Tem0;
            goto ret;
        }
    }
    Tem0 = i * 10 - 550;
    LowRValue  =temp_tab[cBottom];
    HighRValue = temp_tab[cTop];
    Tem = ( ((this->ntc_valve - LowRValue)*10) / (HighRValue - LowRValue) ) + Tem0;        // 表格是以5度为一步的。
ret:
    Tem=Tem/10000.* MD_TEMP_CORRECT_FACTOR;
    return Tem;
}

/*
static long calc_temp(sensor_comps_t  *const this)
{
        const float fR=calc_pt1000(this);
	float fTemp;
 	float fLowRValue;
 	float fHighRValue;  
	int   i;
        int  Bottom=0;
        int  Top=sizeof(tab_pt1000[0])/sizeof(tab_pt1000[0][0])-1;
	if (fR<tab_pt1000[0][Bottom])               
        {
		fTemp =-50;
	}
	else if (fR>tab_pt1000[0][Top])       
        {
                fTemp =145;
	}
	else
	{
		i=Top/2;
		while((Top-Bottom)>1)
	 	{
	 		if (fR<tab_pt1000[0][i])
	                {
	                         Top = i;
	                         i = (Top + Bottom) / 2;
	                }
	                else if (fR >tab_pt1000[0][i])
	                {
	                        Bottom = i;
	                        i = (Top + Bottom) / 2;
	                }
	                else
	                {
	                        fTemp = tab_pt1000[1][i];
	                        return fTemp;
	                }
	        }
	 	fLowRValue  = tab_pt1000[0][Bottom];
	 	fHighRValue = tab_pt1000[0][Top];
		fTemp =(((fR-fLowRValue)*(tab_pt1000[1][Top]-tab_pt1000[1][Bottom]))/(fHighRValue - fLowRValue))+tab_pt1000[1][Bottom];
		
	}
	this->current_temp=fTemp*10;
	return this->current_temp;
}

*/
////////////////////////////////end get temperature////////////////////////////////////
////////////////////////////////end get temperature////////////////////////////////////




/////////////////////////////////////////get density////////////////////////////////////
/////////////////////////////////////////get density////////////////////////////////////

static int cmpfunc (const void * a, const void * b)
{
   long cmp=( *(long *)a - *(long*)b );
   if(cmp<0)
   {
	   return -1;
   }
   else
   {
	   return 1;
   }
}
static long calc_signal_period(sensor_comps_t *const this)
{
    int   i=0;
    int   j=0;
    long temp_var=0;
    long sum_period=0;
    long signal_period=0;
    long signal_freq=0;
    long sum_high_level=0;
    long sum_low_level=0;
    long period[100]={0};//=malloc(count*sizeof(float));
    int count=this->timer_ch1_pos;
    float a=1;
    long  cmp=0;
	if(count<MD_TIMER_MAX_POS/2)
	{
	        MD_SET_TIMER_RESULT_0X1FFFF;
	        count=MD_ADC_MAX_POS;
	}
	for(i=0;i<count;i++)
	{
		period[i]+=(this->timer_result[0][i]+this->timer_result[1][i]);
		//sum_high_level+=this->timer_result[0][i];
		//sum_low_level +=this->timer_result[1][i];
	}
	qsort(period,count,sizeof(period[0]),cmpfunc);
    for(i=1;i<count-1;i++)
	{
		sum_period+=period[i];
	}
	signal_period=(sum_period*10/(count-2));
	
	this->signal_period_n_9=this->signal_period_n_8;this->signal_period_n_8=this->signal_period_n_7;
	this->signal_period_n_7=this->signal_period_n_6;this->signal_period_n_6=this->signal_period_n_5;
	this->signal_period_n_5=this->signal_period_n_4;this->signal_period_n_4=this->signal_period_n_3;
	this->signal_period_n_3=this->signal_period_n_2;this->signal_period_n_2=this->signal_period_n_1;
	this->signal_period_n_1=this->signal_period;
	
	cmp=/*this->signal_period_n_9+this->signal_period_n_8+this->signal_period_n_7+this->signal_period_n_6+this->signal_period_n_5
	   +this->signal_period_n_4+this->signal_period_n_3+*/this->signal_period_n_2+this->signal_period_n_1+signal_period;
	cmp/=3;
	if((cmp==0)||(cmp==0x1ffff*2*10)||(cmp-signal_period>1000)||(cmp-signal_period<-1000))
	{
		sensor_comps.sw&=~MD_DENSITY_STB;
		this->signal_period=signal_period;
		signal_period;
	}
	else
	{
		sensor_comps.sw|=MD_DENSITY_STB;
		this->signal_period=cmp;
		signal_period=cmp;
	}
   

	
	/////////// start Temperature compensation 
	{
		//long cal_temp=(this->temp_tab[0]+this->temp_tab[1]+this->temp_tab[2])/3;
	   // this->signal_freq=signal_freq+(this->current_temp-cal_temp)*3.3;
	    //signal_period=20000./this->signal_freq *1000*1000;
        long cal_temp=250;//0.1 cel
        long delta_temp=(this->current_temp-cal_temp)/10;
        if(this->current_temp<-200)//<20 cel
        {
            signal_period-=0;
        }
        else if(this->current_temp<200)//<20 cel
        {
            signal_period-=delta_temp*30;
        }
        else if(this->current_temp<400)//<40 cel
        {
           signal_period-=delta_temp*31;
        }
        else if(this->current_temp<600)//<60 cel
        {
            signal_period-=delta_temp*32;;
        }
        else
        {
            signal_period-=delta_temp*33;
        }
   }
	
	////////// end Temperature compensation
	this->signal_freq=20000./signal_period *1000*1000*12;//24Mhz Counter timer ,unit:0.01,
	this->signal_hi_level_time=sum_high_level*10/(count*20);//24Mhz Counter timer ,unit:0.1us
	this->signal_lo_level_time=sum_low_level*10/(count*20); //24Mhz Counter timer ,unit:0.1us
	
	//the final param for calc dennsity,24Mhz counter valve *10

   
	
	//free (period);
	return signal_period;
}

static long calc_density(sensor_comps_t *const this)
{
     long fT=calc_signal_period(this);
	 long fDensity;
 	 long fLowTValue;
 	 long fHighTValue;        
	int   i;
    int  Bottom=0;
    int  Top=3-1; //sensor_comps.period_tab[3]
    if (fT<sensor_comps.period_tab[Bottom])               
    {
             Top=Bottom+1;
	 goto insert_calc;//jmp 2 points insert_calc code
    }
    else if (fT>sensor_comps.period_tab[Top])       
    {
             Bottom=Top-1;
	 goto insert_calc;
    }
	i=Top/2;
	while(Top-Bottom>1)
	{
        if (fT<sensor_comps.period_tab[i])
        {
             Top = i;
             i = (Top + Bottom) / 2;
        }
        else if (fT >sensor_comps.period_tab[i])
        {
            Bottom = i;
            i = (Top + Bottom) / 2;
        }
        else
        {
            fDensity = sensor_comps.param_tab[i];
            goto  ret;
        }
    }
insert_calc:
    fLowTValue  = sensor_comps.period_tab[Bottom];
    fHighTValue = sensor_comps.period_tab[Top];
    if(fT-fLowTValue>0 && (fT-fLowTValue)*(sensor_comps.param_tab[Top]-sensor_comps.param_tab[Bottom])<0)
    {
	    fDensity=MD_DENSITY_UPPER_RANGE_LIMIT;
    }
    else
    {
	fDensity =(((fT-fLowTValue)*(sensor_comps.param_tab[Top]-sensor_comps.param_tab[Bottom]))/(fHighTValue - fLowTValue))+sensor_comps.param_tab[Bottom];    
    }
    
	
ret:	
    fDensity=fDensity/10000.* MD_DENSITY_CORRECT_FACTOR;
	if(fDensity<2000)//4 fix dot ,g/cm^3
	{
		this->current_density=fDensity=MD_DENSITY_LOWER_RANGE_LIMIT;
	}
	else if(fDensity<MD_DENSITY_UPPER_RANGE_LIMIT)
	{
		this->current_density=fDensity;
	}
	else
	{
             this->current_density=MD_DENSITY_UPPER_RANGE_LIMIT;
	}
	return this->current_density;
}
////////////////////////////////end get density////////////////////////////////////
////////////////////////////////end get density////////////////////////////////////
static long calc_Mud_consicy(sensor_comps_t *const this)
{
    long coe;	
    if(this->current_density<10000)
    {
        return 0;
    }
  // 
    coe=34150*(this->current_density/10000. -1);
    return coe;
}

static long calc_Alcohol_consicy(sensor_comps_t *const this)
{
    float delta_v= this->current_density/10000.;
	long consicy=0;

    float  LowTValue;
    float  HighTValue;        
    int   i;
    int  Bottom=0;
    int  Top=sizeof(Alcohol_density_table)/sizeof(Alcohol_density_table[0])-1; //
    if (delta_v>Alcohol_density_table[Bottom])               
    {
       // Top=Bottom+1;
       // goto insert_calc;//jmp 2 points insert_calc code
        return consicy=Alcohol_consicy_table[Bottom];
    }
    else if (delta_v<Alcohol_density_table[Top])       
    {
       // Bottom=Top-1;
        //goto insert_calc;
        return consicy=Alcohol_consicy_table[Top];
    }
	i=Top/2;
	while(Top-Bottom>1)
	{
        if (delta_v>Alcohol_density_table[i])
        {
            Top = i;
            i = (Top + Bottom) / 2;
        }
        else if (delta_v <Alcohol_density_table[i])
        {
            Bottom = i;
            i = (Top + Bottom) / 2;
        }
        else
        {
            consicy = Alcohol_consicy_table[i];
            goto  ret;
        }
    }
insert_calc:
{
    
    LowTValue  = Alcohol_density_table[Bottom];
    HighTValue = Alcohol_density_table[Top];
    consicy =(
    ((delta_v-LowTValue)*(Alcohol_consicy_table[Top]-Alcohol_consicy_table[Bottom]))
    /(HighTValue - LowTValue)
    )
    +Alcohol_consicy_table[Bottom];
}
ret:
    if(consicy<0)
    {
        consicy=0;
    }
    else if(consicy>10000)
    {
        consicy=10000;
    }
	return consicy;
}

static long calc_BoMeiDu_consicy(sensor_comps_t *const this)
{
    float delta_v= this->current_density/10000.;
    long consicy=0;

    float  LowTValue;
    float  HighTValue;        
    int   i;
    int  Bottom=0;
    int  Top=sizeof(BoMeiDu_density_table)/sizeof(BoMeiDu_density_table[0])-1; //
    if (delta_v<BoMeiDu_density_table[Bottom])               
    {
       // Top=Bottom+1;
       // goto insert_calc;//jmp 2 points insert_calc code
        return consicy=BoMeiDu_consicy_table[Bottom];
    }
    else if (delta_v>BoMeiDu_density_table[Top])       
    {
       // Bottom=Top-1;
        //goto insert_calc;
        return consicy=BoMeiDu_consicy_table[Top];
    }
    i=Top/2;
    while(Top-Bottom>1)
    {
        if (delta_v<BoMeiDu_density_table[i])
        {
            Top = i;
            i = (Top + Bottom) / 2;
        }
        else if (delta_v >BoMeiDu_density_table[i])
        {
            Bottom = i;
            i = (Top + Bottom) / 2;
        }
        else
        {
            consicy = BoMeiDu_consicy_table[i];
            goto  ret;
        }
    }
insert_calc:
{
    
    LowTValue  = BoMeiDu_density_table[Bottom];
    HighTValue = BoMeiDu_density_table[Top];
    consicy =(
    ((delta_v-LowTValue)*(BoMeiDu_consicy_table[Top]-BoMeiDu_consicy_table[Bottom]))
    /(HighTValue - LowTValue)
    )
    +BoMeiDu_consicy_table[Bottom];
}
ret:
    if(consicy<0)
    {
        consicy=0;
    }
    else if(consicy>10000)
    {
        consicy=10000;
    }
    return consicy;
}


static void sensor_comps_output_debug_info(sensor_comps_t const *const this)
{
	static int line_num=0;
	int tx_num=0;
	memset(this->debug_info,0,sizeof(this->debug_info));
	if(line_num==0)
	{
		
		//start output attribute name(title)
		sprintf(this->debug_info+strlen(this->debug_info),"ht(us)\t\t");//signal_hi_level_time
		sprintf(this->debug_info+strlen(this->debug_info),"lt(us)\t\t");//signal_lo_level_time
		sprintf(this->debug_info+strlen(this->debug_info),"fc(hz)\t\t");//signal_ferq_from_timer_ch0 extern event counter 
		sprintf(this->debug_info+strlen(this->debug_info),"ft(hz)\t\t");//signal_freq
		sprintf(this->debug_info+strlen(this->debug_info),"ntc(om)\t\t");//pt1000_valve
		sprintf(this->debug_info+strlen(this->debug_info),"tp(c)\t\t"); //current_temp
		sprintf(this->debug_info+strlen(this->debug_info),"count\r\n"); //signal_period
		//end output attribute name(title)
	}
	else
	{
		sprintf(this->debug_info+strlen(this->debug_info),"%0.1f\t\t",this->signal_hi_level_time/10.);
		sprintf(this->debug_info+strlen(this->debug_info),"%0.1f\t\t",this->signal_lo_level_time/10.);
		sprintf(this->debug_info+strlen(this->debug_info),"%0.1f\t\t",(float)this->signal_ferq_from_timer_ch0);
		sprintf(this->debug_info+strlen(this->debug_info),"%0.2f\t\t",this->signal_freq/100.);
		sprintf(this->debug_info+strlen(this->debug_info),"%0.1f\t\t",(float)this->ntc_valve);
		sprintf(this->debug_info+strlen(this->debug_info),"%0.1f\t\t",this->current_temp/10.);
		sprintf(this->debug_info+strlen(this->debug_info),"%ld\r\n",this->signal_period);
	
	}
	line_num++;
	if(line_num>=10)//Output attribute name(title) every 50 lines
	{
		line_num=0; 
	}
	tx_num=strlen(this->debug_info);
	if(tx_num>=sizeof(this->debug_info))
	{
		memset(this->debug_info,0,sizeof(this->debug_info));
		sprintf(this->debug_info,"Write sensor debug output buffer overflow\r\n"); 
		tx_num=strlen(this->debug_info);
	}
	R_UART0_Send(this->debug_info,tx_num);
	
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
static void read_param(void)
{
	unsigned long check_sum=0;
	
	_24cxx_comps.read(MD_HUM_COMPS_PARAM_24CXX_ADDR_START,sensor_comps.param_tab,sizeof(sensor_comps.param_tab));//(void * const buf, unsigned int addr, unsigned int n);
	check_sum= calc_sum(sensor_comps.param_tab,sizeof(sensor_comps.param_tab)/sizeof(sensor_comps.param_tab[0])-1);
	if(sensor_comps.param_tab[sizeof(sensor_comps.param_tab)/sizeof(sensor_comps.param_tab[0])-1]!= check_sum)
	{
		sensor_comps.param_tab[0]=7889; //Density calibration point 0
		sensor_comps.param_tab[1]=9970; //Density calibration point 1
		sensor_comps.param_tab[2]=11400;//Density calibration point 2
		sensor_comps.param_tab[3]=0;//3,lower_range,
		#if(MD_DEVICE==MD_CONSICY)
		sensor_comps.param_tab[4]=10000;//4,upper_range
		#else
		sensor_comps.param_tab[4]=25000;//4,upper_range
		#endif
		sensor_comps.param_tab[5]=0;//5,Current dir,
		sensor_comps.param_tab[6]=10000;//6Temperature Coe,
		sensor_comps.param_tab[7]=10000;//5,Current Coe,
		sensor_comps.param_tab[8]=10000;//6,Density coe,
		sensor_comps.param_tab[9]=1;//6,device add
	
	}
	
	_24cxx_comps.read(MD_HUM_COMPS_PERIOD_TAB_24XX_ADDR_START,sensor_comps.period_tab,sizeof(sensor_comps.period_tab));
	check_sum= calc_sum(sensor_comps.period_tab,sizeof(sensor_comps.period_tab)/sizeof(sensor_comps.period_tab[0])-1);
	if(sensor_comps.period_tab[sizeof(sensor_comps.period_tab)/sizeof(sensor_comps.period_tab[0])-1]!= check_sum)
	{
		sensor_comps.period_tab[0]=(unsigned long)6396*20*8 ;//point 1
		sensor_comps.period_tab[1]=(unsigned long)6624*20*8;//point 2
		sensor_comps.period_tab[2]=(unsigned long)6852*20*8;//point 3 
		
	}
	
	_24cxx_comps.read(MD_HUM_COMPS_TEMP_TAB_24XX_ADDR_START,sensor_comps.temp_tab,sizeof(sensor_comps.temp_tab));
	check_sum= calc_sum(sensor_comps.temp_tab,sizeof(sensor_comps.temp_tab)/sizeof(sensor_comps.temp_tab[0])-1);
	if(sensor_comps.temp_tab[sizeof(sensor_comps.temp_tab)/sizeof(sensor_comps.temp_tab[0])-1]!= check_sum)
	{
		sensor_comps.temp_tab[0]=250 ;//temp 1
		sensor_comps.temp_tab[1]=250;//temp  2
		sensor_comps.temp_tab[2]=250;//temp  3 
	}
}

static void save_param(void)
{
	sensor_comps.param_tab[sizeof(sensor_comps.param_tab)/sizeof(sensor_comps.param_tab[0])-1]= calc_sum(sensor_comps.param_tab,sizeof(sensor_comps.param_tab)/sizeof(sensor_comps.param_tab[0])-1);
	_24cxx_comps.write(MD_HUM_COMPS_PARAM_24CXX_ADDR_START,sensor_comps.param_tab,sizeof(sensor_comps.param_tab));
	sensor_comps.period_tab[sizeof(sensor_comps.period_tab)/sizeof(sensor_comps.period_tab[0])-1]= calc_sum(sensor_comps.period_tab,sizeof(sensor_comps.period_tab)/sizeof(sensor_comps.period_tab[0])-1);
	_24cxx_comps.write(MD_HUM_COMPS_PERIOD_TAB_24XX_ADDR_START,sensor_comps.period_tab,sizeof(sensor_comps.period_tab));
	sensor_comps.temp_tab[sizeof(sensor_comps.temp_tab)/sizeof(sensor_comps.temp_tab[0])-1]= calc_sum(sensor_comps.temp_tab,sizeof(sensor_comps.temp_tab)/sizeof(sensor_comps.temp_tab[0])-1);
	_24cxx_comps.write(MD_HUM_COMPS_TEMP_TAB_24XX_ADDR_START  ,sensor_comps.temp_tab  ,sizeof(sensor_comps.temp_tab));
}

static unsigned char sensor_comps_init(sensor_comps_t *const this)
{
	/**************START E2PROM TEST********************/
	if(sensor_comps.sw&MD_E2PROM_DRIVER_ERR)	
	{
		char msg[32]="";
		_24cxx_comps.write(0,"this is a e2prom driver test",strlen("this is a e2prom driver test"));
		_24cxx_comps.read (0,msg,strlen("this is a e2prom driver test"));
		if(!strcmp(msg,"this is a e2prom driver test"))
		{
			sensor_comps.sw&=~MD_E2PROM_DRIVER_ERR;	
		}
		else
		{
			sensor_comps.sw|=MD_E2PROM_DRIVER_ERR;
		}
	}
	/**************END E2PROM TEST********************/
	

	/***********************AD54x0******************************/
	if(sensor_comps.sw&MD_AD54X0_DRIVER_ERR)
	{
		unsigned int valve=1;
		unsigned int valve_read_back;
		ad54x0_comps.write_reset_reg(&valve);
		/**************************************************************************************
		DB15 	DB14   DB13   DB12    DB11 DB10 DB9 DB8	  DB7 DB6 DB5  DB4   DB3   DB2  DB1  DB0
	       0     0     REXT   OUTEN         R(CLK)          SR(STEP)  SREN   DCEN   R2 	 R1   R0
		   0     0      0       1      1    1   1    1     1   1   1   1      0     1   0    1(4-20ma)
		***************************************************************************************/
		valve=0x1ff5;
		ad54x0_comps.write_control_reg(&valve);
		ad54x0_comps.read_control_reg(&valve_read_back);
		if(valve==valve_read_back)
		{
			sensor_comps.sw&=~MD_AD54X0_DRIVER_ERR;
		}
		else
		{
			sensor_comps.sw|=MD_AD54X0_DRIVER_ERR;
		}
	}
	/*********************END AD54x0******************************/
	
	
	
	/**************START ds18b20********************/
//	if(sensor_comps.sw&MD_DS18B20_DRIVER_ERR)
//	{
//		unsigned char buf[5];
//		if(!ds18b20_comps.read_ram(buf,sizeof(buf)))
//		{
//			sensor_comps.sw&=~MD_DS18B20_DRIVER_ERR;
//			if(buf[4]!=0xef)
//			{
//				unsigned char config_buf[3];
//				config_buf[0]=buf[2];
//				config_buf[1]=buf[3];
//				config_buf[2]=0xef;//
//				ds18b20_comps.write_ram(config_buf,sizeof(config_buf));
//			}
//		}
//		else
//		{
//			sensor_comps.sw|=MD_DS18B20_DRIVER_ERR;
//		}
//		
//	}
	/**************END ds18b20 ********************/
	
	if(!(sensor_comps.sw&0x00ff))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}


//i=16ma/(2^N)*code+4ma
static void calc_4_20ma_output( long density,long lower_range,long upper_range)
{
	/****START calc 4-20ma*******
	I   4000        20000	      1uA  Positive output 
	I   20000       4000          1uA  Reverse output
	D   lower       upper         g/cm^3 4fix dot	
	
	****************************/
	{
		long code=0;//code=(I-4)/16*65536
	    long output_i=0;
		unsigned int  data=0;
		if(!MD_CURRENT_DIR_SETTING)
		{
		    output_i=(20000-4000)*(density-lower_range)/(upper_range-lower_range)+4000;
		}
		else
		{
            		output_i=(4000-20000)*(density-lower_range)/(upper_range-lower_range)+20000;
		}
		output_i=output_i/10000.*MD_CURRENT_CORRECT_FACTOR;
		code=(output_i-4000)*4096/1000;
		if(code>65535)
		{
			code=65535;
		}
		data=code;
		ad54x0_comps.write_data_reg(&data);
	}
	
}

static void sensor_comps_task_handle()//Execution interval is 20 ms
{
	int i=0;
	sensor_comps_t *this=sensor_comps.this;
	if(this->do_init==1)
	{
		if(this->count<60)
		{
			if(!this->init(this))
			{
				this->do_init=0;
			}
			else
			{
				this->count++;
				hum_comps.dis_oper_mark|=MD_OPER_REFRESH_ERR_CODE_MARK;
			}
		}
		else
		{
			this->do_init=0;
		}
		if(this->do_init==0)
		{
			//MD_DENSITY_POWER_ON;
			if(!get_cpu_clk_st())
			{
				WDTE='R';
			}
			R_TAU0_Channel0_Start();
			hum_comps.current_mode=EM_NORMAL_MODE;
			this->read_param();
			this->count=0;
			//TODO
		}
	}
	
	if(this->do_init==0)
	{
		if(this->count==MD_TIMER_MAX_POS)//every 12*20ms calc temperature and density
		{
			this->count=0;
			//this->calc_pt1000(this);
			this->ntc_valve=this->calc_ntc_valve(this);
			this->current_temp=this->calc_temp(this);
			//this->current_temp=(long)ds18b20_comps.get_temp_valve()*10/16;
			hum_comps.dis_oper_mark|=MD_OPER_REFRESH_TEMP_MARK;
			//this->calc_signal_period(this);
			this->calc_density(this);
			#if(MD_CONSICY_TYPE==MD_ALCOHOL)
			this->current_consicy=calc_Alcohol_consicy(this);
			#elif(MD_CONSICY_TYPE==MD_MUD)
			this->current_consicy=calc_Mud_consicy(this);
			#elif(MD_CONSICY_TYPE==MD_BOMEIDU)
			this->current_consicy=calc_BoMeiDu_consicy(this);
			#endif
			
		    #if((MD_DEVICE==MD_CONSICY) && (MD_CONSICY_TYPE==MD_ALCOHOL||MD_CONSICY_TYPE==MD_BOMEIDU) )
			calc_4_20ma_output(this->current_consicy,MD_DENSITY_LOWER_RANGE_LIMIT,MD_DENSITY_UPPER_RANGE_LIMIT);
			#else
            calc_4_20ma_output(this->current_density,MD_DENSITY_LOWER_RANGE_LIMIT,MD_DENSITY_UPPER_RANGE_LIMIT);
			#endif
			hum_comps.dis_oper_mark|=MD_OPER_REFRESH_DATA_MARK;
			hum_comps.dis_oper_mark|=MD_OPER_REFRESH_FREQ_MARK;
			this->adc_pos=0;
			this->timer_ch1_pos=0;
			this->timer_ch2_pos=0;
			
		}
		R_ADC_Start();
		if(this->count==0)
		{
			
			//ds18b20_comps.start_temp_convert();
		}
		R_TAU0_Channel1_Start();//high level measure
		R_TAU0_Channel2_Start();//low  level measure
		this->count++;
	}
}



sensor_comps_t sensor_comps=
{
	"Sensor component description", //char desc[32];
	&sensor_comps,//struct _SENSOR_COMPONENTS  *const this;
	1,//Whether to initialize,1:init 0,no init
	sensor_comps_init,//unsigned char (*const init)(struct _SENSOR_COMPONENTS *);
	0,//unsigned long count;            //Called counter
	
	0+MD_AD54X0_DRIVER_ERR+MD_E2PROM_DRIVER_ERR,   //unsigned int  sw;//status word,
        {0},          //unsigned int  adc_result[MD_ADC_CHANNEL_MAX_NUM][MD_ADC_MAX_POS];//adc ch0~ch3
	0,            //unsigned int  adc_pos
	{0},          //unsigned long timer_result[MD_TIMER_CHANNEL_MAX_NUM][MD_TIMER_MAX_POS];//timer ch1~ch2 ;ch1:high level measure,ch2:low level measure,24M timer counter
	0,            //unsigned int  timer_ch1_pos;
	0,            //unsigned int  timer_ch2_pos;
	0,            //unsigned int  signal_ferq_from_timer_ch0;//unit:Hz,timr ch0,use external event counter for freq measure,
	10000,            //ntc_vlve;
	0,            //float current_temp;
	calc_ntc_valve,  //float (*calc_pt1000)(struct _SENSOR_COMPONENTS const *const);//point to function calc_pt1000(const sensor_comps_t *const this) whith param adc_result[][] 
	calc_temp,    //long (*calc_temp)  (struct _SENSOR_COMPONENTS const *const);//point to function calc_temp(const sensor_comp_t *const this)with param pt1000_valve
	
	0,            //unsigned long signal_hi_level_time;//unit: us,by call the function calc_signal_period(sensor_comp_t *const this)
	0,            //unsigned long signal_lo_level_time;//unit: us,by call the function calc_signal_period(sensor_comp_t *const this)
	0,            //unsigned long signal_freq;         //unit: Hz,by call the function calc_signal_period(sensor_comp_t *const this)

    0,0,0,0,0,0,0,0,0,0, //Yn-9-----------Yn-1 Yn  this  period sample value for calc dennsity,24Mhz counter valve, by call the function calc_signal_period(sensor_comp_t *const this) with param [timer_result[][],
    0,            // long current_density;
    0, //long current_consicy;
	
	////////////////////////////param 0-5,password 0 is accessible ,param 6,password 1  accessible
	//4 fixed-point decimals,g/cm^3  F_RANGE      T    C      D     cs
	{7789   ,9970   ,11400 ,         25000,     10000 ,10000, 10000, 0},//unsigned long param_tab[8];//0-2,Density value for each calibration point,3,full_range,4,Temperature Coe,5,Current coe,6,Density coe,7,check sum(cs)
	{0      ,0 	,0     ,          0},//unsigned long period_tab[4];1 fix dot//0-2,Period value of each calibration point,3,cs.//24Mhz freq=>T=(1/24)us,533us=1/1876Hz   
	{250    ,250    ,250   ,          0},//unsigned long temp_tab[4];//1 fix dot,0-2,Temperature value of each calibration point;3,cs
	read_param,                         //void (*read_param)(void);
	save_param,//void (*save_param)(void);
	
	
	calc_signal_period,//unsigned long (*calc_signal_period)(struct _SENSOR_COMPONENTS *const ); //point to calc_signal_period(sensor_comp_t *const this)
    calc_density, //unsigned long (*calc_density)(struct _SENSOR_COMPONENTS const *const);//point to calc_density(float signal_freq_ch0,float signal_freq ,float fT)
    calc_Alcohol_consicy, //long ( *const calc_consicy)      (struct _SENSOR_COMPONENTS *const);
	sensor_comps_task_handle,//void  (* const task_handle)(void);//point to sensor_comps_task_handle
	sensor_comps_output_debug_info,//void (* const output_debug_info)(struct _SENSOR_COMPONENTS const  *this); point to sensor_comps_output_debug_info(sensor_comps_t const *const this)
	""                             //char debug_info[100];
};









