#ifndef  SENSOR_H
#define  SENSOR_H


#define   MD_DENSITY           0
#define   MD_CONSICY           1
#define   MD_DEVICE            MD_CONSICY


#define   MD_ALCOHOL            0
#define   MD_MUD                1
#define   MD_BOMEIDU            2
#define   MD_NIAOSU             3
#define   MD_LIUSUAN            4

#define   MD_CONSICY_TYPE       MD_LIUSUAN


#define  MD_ADC_CHANNEL_MAX_NUM         2
#define  MD_ADC_MAX_POS                 50
#define  MD_TIMER_CHANNEL_MAX_NUM       2
#define  MD_TIMER_MAX_POS               50

#define  MD_ADC_EOC_FLAG                0x0001  //adc end of convert flag
#define  MD_TIMER_CH1_MEASURE_OK_FLAG   0x0002 //use high leve time measure flag
#define  MD_TIMER_CH2_MEASURE_OK_FLAG   0x0002 //use low leve time measure flag

#define   MD_HUM_COMPS_PARAM_24CXX_ADDR_START           0x0020// 64bytes
#define   MD_HUM_COMPS_PERIOD_TAB_24XX_ADDR_START       0x0060// 64bytes
#define   MD_HUM_COMPS_TEMP_TAB_24XX_ADDR_START         0x00A0// 64bytes

#define   MD_E2PROM_DRIVER_ERR     (1)
#define   MD_AD54X0_DRIVER_ERR     (1<<1)
#define   MD_DS18B20_DRIVER_ERR    (1<<2)
#define   MD_DENSITY_STB           ((unsigned int)1<<8)


typedef struct _SENSOR_COMPONENTS
{
	char desc[32];
	struct _SENSOR_COMPONENTS  *const this;
	int   do_init;//Whether to initialize,1:init 0,no init
	unsigned char (*const init)(struct _SENSOR_COMPONENTS *);
	unsigned long count;            //Called counter   
	unsigned int  sw;//status word, XXXX,XXXX,XXXX,   X      X            X            X
	                 //                              stb    ds18b20    ad54x0_err  e2prom_err 
	unsigned int  adc_result[MD_ADC_CHANNEL_MAX_NUM][MD_ADC_MAX_POS];//adc ch0~ch3
	unsigned int  adc_pos;
	unsigned long timer_result[MD_TIMER_CHANNEL_MAX_NUM][MD_TIMER_MAX_POS];//timer ch1~ch2 ;ch1:high level measure,ch2:low level measure,24M timer counter
	unsigned int  timer_ch1_pos;
	unsigned int  timer_ch2_pos;
	unsigned int  signal_ferq_from_timer_ch0;//unit:Hz,timr ch0,use external event counter for freq measure,
	long  ntc_valve;
	int   current_temp;
	long ( * const calc_ntc_valve)(struct _SENSOR_COMPONENTS  *const);//point to function calc_pt1000( sensor_comps_t *const this) whith param adc_result[][] 
	int  ( * const calc_temp)  (struct _SENSOR_COMPONENTS  *const);//point to function calc_temp( sensor_comp_t *const this)with param pt1000_valve
	
	long  signal_hi_level_time;//unit: 0.1us,by call the function calc_signal_period(sensor_comp_t *const this)
	long  signal_lo_level_time;//unit: 0.1us,by call the function calc_signal_period(sensor_comp_t *const this)
	long  signal_freq;         //unit: 0.01 Hz,by call the function calc_signal_period(sensor_comp_t *const this)
	long  signal_period_n_9;//Last  period sample value Yn-9
	long  signal_period_n_8;//Last  period sample value Yn-8
	long  signal_period_n_7;//Last  period sample value Yn-7
	long  signal_period_n_6;//Last  period sample value Yn-6
	long  signal_period_n_5;//Last  period sample value Yn-5
	long  signal_period_n_4;//Last  period sample value Yn-4
	long  signal_period_n_3;//Last  period sample value Yn-3
	long  signal_period_n_2;//Last  period sample value Yn-2
	long  signal_period_n_1;//Last  period sample value Yn-1
        long  signal_period;//(Yn) this  period sample value for calc dennsity,24Mhz counter valve*10, by call the function calc_signal_period(sensor_comp_t *const this) with param [timer_result[][],
	
	long  current_density;
	long  current_consicy;//fix 2 dot
	
	////////////////////////////param 0-5,password 0 is accessible ,param 6,password 1  accessible
	long param_tab[11];//0-2,Density value for each calibration point(4 fix dot),3,full_range,4,Temperature Coe,5,Current coe,6,Density coe,7,check sum(cs)
	long period_tab[4];//0-2,Period value of each calibration point(1 fix dot),3,cs
	long temp_tab[4];//0-2,Temperature value of each calibration point(1 fix dot);3,cs
	void (*read_param)(void);
	void (*save_param)(void);
	
	long ( *const calc_signal_period)(struct _SENSOR_COMPONENTS *const ); //point to calc_signal_period(sensor_comp_t *const this)
    long ( *const calc_density)      (struct _SENSOR_COMPONENTS *const);//point to calc_density(sensor_comp_t *const this)
    long ( *const calc_consicy)      (struct _SENSOR_COMPONENTS *const);
	void  ( *const task_handle)(void);//point to sensor_comps_task_handle
	void  ( *const output_debug_info)(struct _SENSOR_COMPONENTS  const *const);//point to sensor_comps_output_debug_info(sensor_comps_t const *const this)
	char debug_info[128];
}sensor_comps_t; 
extern sensor_comps_t sensor_comps;

#define   MD_DENSITY_LOWER_RANGE_LIMIT      sensor_comps.param_tab[3]
#define   MD_DENSITY_UPPER_RANGE_LIMIT      sensor_comps.param_tab[4]
#define   MD_CURRENT_DIR_SETTING            sensor_comps.param_tab[5]
#define   MD_TEMP_CORRECT_FACTOR            sensor_comps.param_tab[6]
#define   MD_CURRENT_CORRECT_FACTOR         sensor_comps.param_tab[7]
#define   MD_DENSITY_CORRECT_FACTOR         sensor_comps.param_tab[8]
#define   MD_DEVICE_ADDR                    sensor_comps.param_tab[9]



#endif
