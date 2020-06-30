/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011, 2014 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_serial_user.c
* Version      : CodeGenerator for RL78/G14 V2.04.00.03 [07 Aug 2014]
* Device(s)    : R5F104BG
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for Serial module.
* Creation Date: 2019/10/10 ÐÇÆÚËÄ
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt INTST0 r_uart0_interrupt_send
#pragma interrupt INTSR0 r_uart0_interrupt_receive
#pragma interrupt INTSRE0 r_uart0_interrupt_error
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_serial.h"
/* Start user code for include. Do not edit comment generated here */
#include "irc.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern volatile uint8_t * gp_uart0_tx_address;         /* uart0 send buffer address */
extern volatile uint16_t  g_uart0_tx_count;            /* uart0 send data number */
extern volatile uint8_t * gp_uart0_rx_address;         /* uart0 receive buffer address */
extern volatile uint16_t  g_uart0_rx_count;            /* uart0 receive data number */
extern volatile uint16_t  g_uart0_rx_length;           /* uart0 receive data length */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_uart0_interrupt_receive
* Description  : This function is INTSR0 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_uart0_interrupt_receive(void)
{
    uint8_t rx_data;
    rx_data = RXD0;
    ircComps.recv_base_pt[*ircComps.recv_pos_pt]=rx_data;
    *ircComps.recv_pos_pt+=1;
    *ircComps.recv_pos_pt&=0x3f;
}

/***********************************************************************************************************************
* Function Name: r_uart0_interrupt_error
* Description  : This function is INTSRE0 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_uart0_interrupt_error(void)
{
    uint8_t err_type;

    *gp_uart0_rx_address = RXD0;
    err_type = (uint8_t)(SSR01 & 0x0007U);
    SIR01 = (uint16_t)err_type;
    r_uart0_callback_error(err_type);
}

/***********************************************************************************************************************
* Function Name: r_uart0_interrupt_send
* Description  : This function is INTST0 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_uart0_interrupt_send(void)
{
    if (g_uart0_tx_count > 0U)
    {
        TXD0 = *gp_uart0_tx_address;
        gp_uart0_tx_address++;
        g_uart0_tx_count--;
    }
    else
    {
        r_uart0_callback_sendend();
    }
}

/***********************************************************************************************************************
* Function Name: r_uart0_callback_receiveend
* Description  : This function is a callback function when UART0 finishes reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_uart0_callback_softwareoverrun
* Description  : This function is a callback function when UART0 receives an overflow data.
* Arguments    : rx_data -
*                    receive data
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_softwareoverrun(uint16_t rx_data)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_uart0_callback_sendend
* Description  : This function is a callback function when UART0 finishes transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_sendend(void)
{
    /* Start user code. Do not edit comment generated here */
    //R_TAU0_Channel0_Stop();
    disable_irc_send() ;   
    enable_irc_receive();
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_uart0_callback_error
* Description  : This function is a callback function when UART0 reception error occurs.
* Arguments    : err_type -
*                    error type value
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_error(uint8_t err_type)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
void disable_irc_send(void)
{
    NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
    MD_RESET_RS_485_T_R;
  
    STMK0 = 1U;    /* disable INTST0 interrupt */
    ST0 |= _0001_SAU_CH0_STOP_TRG_ON;
    SOE0 &= ~_0001_SAU_CH0_OUTPUT_ENABLE;    /* disable UART0 output */
    STIF0 = 0U;
    SO0 |= _0001_SAU_CH0_DATA_OUTPUT_1;    /* output level normal */
    ircComps.sw._bit.busy=0;
    
	//R_TAU0_Channel0_Stop();
	//TO0 &= ~_0010_TAU_CH4_OUTPUT_VALUE_1;

}

void enable_irc_send(void)
{
    MD_SET_RS_485_T_R;
     NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
    SO0 |= _0001_SAU_CH0_DATA_OUTPUT_1;    /* output level normal */
    SOE0 |= _0001_SAU_CH0_OUTPUT_ENABLE;    /* enable UART0 output */
    SS0 |= _0001_SAU_CH0_START_TRG_ON;    /* enable UART0 receive and transmit */
    STIF0 = 0U;    /* clear INTST0 interrupt flag */
    STMK0 = 0U;    /* enable INTST0 interrupt */
 }
 
void disable_irc_receive(void)
{
    MD_SET_RS_485_T_R;
    
    SRMK0 = 1U;    /* disable INTSR0 interrupt */
    ST0 |= _0002_SAU_CH1_STOP_TRG_ON ;    /* disable UART0 receive and transmit */
    SRIF0 = 0U;    /* clear INTSR0 interrupt flag */
}

void enable_irc_receive(void)
{
    MD_RESET_RS_485_T_R;
    
    SS0 |= _0002_SAU_CH1_START_TRG_ON ;    /* enable UART0 receive and transmit */
    SRIF0 = 0U;    /* clear INTSR0 interrupt flag */
    SRMK0 = 0U;    /* enable INTSR0 interrupt */
}
/*
    uint8_t rx_data;
    rx_data = RXD0;
    ircComps.recv_base_pt[*ircComps.recv_pos_pt]=rx_data;
    *ircComps.recv_pos_pt+=1;
    *ircComps.recv_pos_pt&=0x3f;
 */
/* End user code. Do not edit comment generated here */
