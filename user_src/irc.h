#ifndef  IRC_H
#define  IRC_H

#define  MD_SET_RS_485_T_R    P2.2=1
#define	 MD_RESET_RS_485_T_R  P2.2=0
typedef struct 
{
 
	unsigned char *const recv_base_pt;//Passed to uart recv  point
	unsigned char *const recv_pos_pt;
	unsigned short       op_window_time; 
    union 
    {
    	unsigned char All;
    	struct{
                unsigned char runing		        :1;
                unsigned char busy		            :1;
               }_bit;
    }sw;
    void (*write)(unsigned char * const , unsigned int );
    void (*const task_handle)(void);
}ircComps_t;

extern ircComps_t ircComps;
extern  unsigned char Check_Sum_5A(unsigned char const *Data,unsigned char Len);
extern  long formatData4fixDot(long temp);

#endif