/*******************************************************************************
  Application External Interrupt Control(EIC) Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_eic.c

  Summary:
    This file contains the Application External Interrupt Control(EIC) functions for this project.

  Description:
    This file contains the Application EIC functions for this project.
    Including the EIC interrupt handler.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "definitions.h"
#include "app.h"
#include "app_eic.h"

#ifdef APP_DISABLE_UART_POLL
#include "app_timer.h"
#include "app_trp_common.h"
#endif


// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************



// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************



// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************
#ifdef APP_DISABLE_UART_POLL
static APP_TIMER_TmrElem_T s_appIdleUartRxTimer;
#endif


// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************

uint8_t APP_EIC_GetWakeUpPinStatus(void)
{
    if (GPIOB_REGS->GPIO_PORT & APP_EIC_PIN_RX_IND)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static void APP_EIC_Pin0Callback (uintptr_t context)
{
#if 0
#ifdef APP_DISABLE_UART_POLL
    if (APP_EIC_GetWakeUpPinStatus() == 0)
    {
        //PB4 is normal high, falling trigger to start timer
        //Start timer
        APP_TRP_COMMON_SetTimer(APP_TRP_TMR_ID_INST_MERGE(APP_TIMER_UART_FETCH_00, 0), NULL, APP_TIMER_20MS,
        true, &s_appIdleUartRxTimer);
    }
#endif
#endif
}

#ifdef APP_DISABLE_UART_POLL
uint16_t APP_EIC_StopUartRxTimer(void)
{
    return APP_TIMER_StopTimer(&(s_appIdleUartRxTimer.tmrHandle));
}
#endif

void APP_EIC_Init(void)
{
    EIC_CallbackRegister(EIC_PIN_0, APP_EIC_Pin0Callback, 0);
}
