#include "r_cg_wdt.h"
#include "sensor.h"
#include "hum.h"
#include "ht1621x.h"
#include "stdlib.h"
#include "irc.h"

void (*const wdt_reset)(void)=R_WDT_Restart;
typedef struct _TASK_COMPONENTS
{
	char desc[32];
        struct _TASK_COMPONENTS *this;
	int run_flag;
	volatile int timer;//*1ms timer
	int interval_time;
	void (* *const task_hook)(void);
	//void * *const task_hook;
}task_comps_t;
 

static task_comps_t task_comps[]=    //interval time 5ms
{
	
	{"",&task_comps[0],0,10    ,1,   &hum_comps.task_handle   },
	{"",&task_comps[1],0,1000  ,4,   &sensor_comps.task_handle},//20ms
	{"",&task_comps[2],0,10    ,200, &wdt_reset               },
	
	//...TODO......
};

void callback_task_remarks(void)//millisecond interval timer interrupt callback si used to start task
{
	int i=0;
	for(i=0;i<sizeof(task_comps)/sizeof(task_comps_t);i++)
	{
		if(task_comps[i].timer>0)
		{
			task_comps[i].timer--;
			if(task_comps[i].timer==0)
			{
				task_comps[i].timer=task_comps[i].interval_time;
				task_comps[i].run_flag=1;
			}
		}
	}
}

void task_process(void)//Main loop call is used to execution tasks
{
	int i=0;
	for(i=0;i<sizeof(task_comps)/sizeof(task_comps_t);i++)
	{
		if(task_comps[i].run_flag)
		{
			(*task_comps[i].task_hook)();//Priority combination .>()>*
			task_comps[i].run_flag=0;
		}
	}
	ircComps.task_handle();
}



