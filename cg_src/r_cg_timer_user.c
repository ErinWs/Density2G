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
* File Name    : r_cg_timer_user.c
* Version      : CodeGenerator for RL78/G14 V2.04.00.03 [07 Aug 2014]
* Device(s)    : R5F104BG
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for TAU module.
* Creation Date: 2019/10/10 ÐÇÆÚËÄ
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt INTTM00 r_tau0_channel0_interrupt
#pragma interrupt INTTM01 r_tau0_channel1_interrupt
#pragma interrupt INTTM02 r_tau0_channel2_interrupt
#pragma interrupt INTTM03 r_tau0_channel3_interrupt
/* Start user code for pragma. Do not edit comment generated here */

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_timer.h"
/* Start user code for include. Do not edit comment generated here */
#include "sensor.h"
#include "system.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* For TAU0_ch1 pulse measurement */
volatile uint32_t g_tau0_ch1_width = 0U;
/* For TAU0_ch2 pulse measurement */
volatile uint32_t g_tau0_ch2_width = 0U;
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_tau0_channel0_interrupt
* Description  : This function is INTTM00 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_tau0_channel0_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tau0_channel1_interrupt
* Description  : This function is INTTM01 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_tau0_channel1_interrupt(void)
{
    if ((TSR01 & _0001_TAU_OVERFLOW_OCCURS) == 1U)    /* overflow occurs */
    {            
        g_tau0_ch1_width = (uint32_t)(TDR01 + 1U) + 0x10000U;
    }
    else
    {
        g_tau0_ch1_width = (uint32_t)(TDR01 + 1U);
    }

    /* Start user code. Do not edit comment generated here */
    sensor_comps.timer_result[0][sensor_comps.timer_ch1_pos]=g_tau0_ch1_width;
    sensor_comps.timer_ch1_pos++;
    R_TAU0_Channel1_Stop();
//    if(sensor_comps.timer_ch1_pos>=MD_TIMER_MAX_POS)
//    {
//	   sensor_comps.sw|=MD_TIMER_CH1_MEASURE_OK_FLAG; 
//    }
    
    
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tau0_channel2_interrupt
* Description  : This function is INTTM02 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_tau0_channel2_interrupt(void)
{
    if ((TSR02 & _0001_TAU_OVERFLOW_OCCURS) == 1U)    /* overflow occurs */
    {            
        g_tau0_ch2_width = (uint32_t)(TDR02 + 1U) + 0x10000U;
    }
    else
    {
        g_tau0_ch2_width = (uint32_t)(TDR02 + 1U);
    }

    /* Start user code. Do not edit comment generated here */
    sensor_comps.timer_result[1][sensor_comps.timer_ch2_pos]=g_tau0_ch2_width;
    sensor_comps.timer_ch2_pos++;
    R_TAU0_Channel2_Stop();
//    if(sensor_comps.timer_ch2_pos>=MD_TIMER_MAX_POS)
//    {
//	   sensor_comps.sw|=MD_TIMER_CH2_MEASURE_OK_FLAG; 
//    }
    
    
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tau0_channel3_interrupt
* Description  : This function is INTTM03 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_tau0_channel3_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */

    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
