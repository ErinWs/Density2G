#ifndef DS18B20_H
#define DS18B20_H
typedef int     MD_DS18B20_EXECUTION_STATUS;
typedef struct _DS18B20_COMPONENTS
{
	char desc[32];
	struct _DS18B20_COMPONENTS  *this;
	MD_DS18B20_EXECUTION_STATUS (*const reset)(void);
	MD_DS18B20_EXECUTION_STATUS (*const read_ram )(void * const  buf, unsigned int len);
	void                        (*const write_ram)(void const * buf, unsigned int len);
	void(*const start_temp_convert)(void);
	int (*const get_temp_valve) (void);
	
}ds18b20_comps_t;

extern ds18b20_comps_t  ds18b20_comps;
#endif