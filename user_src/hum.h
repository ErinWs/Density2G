#ifndef HUM_H

#define   MD_OPER_CUR_MARK                0x01  //cursor_mask
#define   MD_OPER_DIS_MARK                0x02  //display mask  1
#define   MD_OPER_REFRESH_FREQ_MARK       0x04 //refresh freq 
#define   MD_OPER_REFRESH_TEMP_MARK       0x08 //refresh temperature 
#define   MD_OPER_REFRESH_DATA_MARK       0x10 //refresh data (density unit switch etc)
#define   MD_OPER_REFRESH_PARAM_MARK      0x20 //refresh param 
#define   MD_OPER_REFRESH_OPTION_MARK     0x40 //refresh option
#define   MD_OPER_REFRESH_ERR_CODE_MARK   0x80//refresh err code
typedef enum  
{
	EM_NORMAL_MODE=0,
	EM_DEBUG_MODE,
	EM_PWD_MODE,
	EM_QUERY_MODE,// param query mode
	EM_MODIFY_MODE,//maram modify mode
	EM_SELF_TEST_MODE//test mode used to check ad54x0,e2prom,ds18b20
	//....TODO.....
	
}mode_type_t;

typedef struct _MODE_COMPONENTS//Handling of keys in different modes
{
	char desc[16];
	struct _MODE_COMPONENTS *this;
	mode_type_t type;
	void (*on_s_key)(void);
	void (*on_m_key)(void);
	void (*on_j_key)(void);
	void (*on_long_s_key)(void);
	void (*on_long_m_key)(void);
	void (*on_long_s_and_j_key)(void);
	void (*display)(unsigned char opt);
	unsigned char dis_option;
	
}mode_comps_t;

typedef enum
{
	EM_NO_KEY,               //em:enum 
	EM_SHORT_KEY,
	EM_LONG_KEY
} key_type_t;

typedef struct _HUM_COMPONENTS
{
	char desc[32];
	struct _HUM_COMPONENTS *this;
	int   do_init;//Whether to initialize,1:init 0,no init
	void (*const init)(struct _HUM_COMPONENTS *);
	unsigned long count;            //Called counter
	
	
	/******************************START HT1621 USE******************************/
	unsigned char const SEG_TAB[20] ;
	const unsigned char BIAS;
	const unsigned char SYSEN;
	const unsigned char LCDON;
	unsigned char dig0_0;
	unsigned char dig0_1;
	unsigned char dig0_2;
	unsigned char dig0_3;
	unsigned char dig0_4;
	unsigned char dot0_pos;
	unsigned char dig1_0;
	unsigned char dig1_1;
	unsigned char dig1_2;
	unsigned char dot1_pos;
	char dis_buf[20];                  //SEG0-SEG17 disbuf  ht1621x memory map
	unsigned char dis_oper_mark;
	unsigned char cursor;
	unsigned char cursor_count;
	/*******************END HT1621 USE**************************/ 
	mode_type_t   current_mode;
	unsigned char accessible_data;//Different passwords to access different data(Sensor parameter);6,accessible 0-5,other,accessible 6,
	
        key_type_t key_type;//[EM_NO_KEY, EM_SHORT_KEY,EM_LONG_KEY]
	int key;
	int  (*const get_key)(struct _HUM_COMPONENTS *);
	void (*const task_handle)(void);
	
	//TODO
	
}hum_comps_t;
extern mode_comps_t  mode_comps[];
extern hum_comps_t hum_comps;
#endif