/*******************************************************************************
  Application Transparent Common Function Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_key.c

  Summary:
    This file contains the Application Transparent common functions for this project.

  Description:
    This file contains the Application Transparent common functions for this project.
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
#include <string.h>
#include "app.h"
#include "ble_util/byte_stream.h"
#include "app_key.h"
#include "app_led.h"
#include "app_ble_handler.h"
#include "app_adv.h"
#include "app_trps.h"
#include "app_error_defs.h"

#ifdef APP_TRPC_ENABLE
#include "app_scan.h"
#include "app_trpc.h"
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
#define APP_KEY_PB4                 0x10
#define APP_KEY_LONG_PRESS_TIME     10      // 50 ms * 10 = 500 ms
#define APP_KEY_DOUBLE_CLICK_TIME   8       // 50 ms * 8 = 400 ms

// *****************************************************************************
// *****************************************************************************
// Section: Global Variables
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************
static APP_KEY_Elem_T       s_appKeyElement_t;
static APP_TIMER_TmrElem_T  s_scanTmr_t;        /**< Key scan timer of 50 ms unit. */
// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
#if (defined APP_TRPC_ENABLE) || (defined APP_BLE_MULTI_LINK_ENABLE) 
static void APP_KEY_NoBleLinkTrigger(uint8_t keyMesg)
{
    #ifdef APP_BLE_MULTI_LINK_ENABLE
    APP_BLE_ConnList_T *p_bleConnList_t = NULL;

    p_bleConnList_t = APP_GetCurrentBleLinkMtLink();
    if (keyMesg == APP_KEY_MSG_SHORT_PRESS)
    {
        if (APP_GetBleStateMtLink(p_bleConnList_t) == APP_BLE_STATE_ADVERTISING)
        {
            APP_ADV_Stop();
            APP_LED_AlwaysOn(APP_LED_TYPE_NULL, APP_LED_TYPE_NULL);
            #ifdef APP_TRPC_ENABLE
            APP_SCAN_Start();
            #endif
        }
        #ifdef APP_TRPC_ENABLE
        else if (APP_GetBleStateMtLink(p_bleConnList_t) == APP_BLE_STATE_SCANNING)
        {
            APP_SCAN_Stop();
            APP_LED_AlwaysOn(APP_LED_TYPE_NULL, APP_LED_TYPE_NULL);
            APP_ADV_Start();
        }
        #endif
        else
            APP_ADV_Start();
    }
    else if (keyMesg == APP_KEY_MSG_LONG_PRESS)
    {
        #ifdef APP_TRPC_ENABLE
            if ((APP_GetBleRoleMtLink(p_bleConnList_t) == BLE_GAP_ROLE_CENTRAL) 
                && (APP_GetBleStateMtLink(p_bleConnList_t) == APP_BLE_STATE_SCANNING))
            {
                APP_SCAN_Stop();
                APP_LED_AlwaysOn(APP_LED_TYPE_NULL, APP_LED_TYPE_NULL);
                p_bleConnList_t = APP_GetActiveBleLinkMtLink();
                if (p_bleConnList_t == NULL)
                    APP_LED_AlwaysOn(APP_LED_TYPE_NULL, APP_LED_TYPE_NULL);
                else
                {
                    if (APP_GetBleStateMtLink(p_bleConnList_t) == APP_BLE_STATE_CONNECTED)
                    {
                        APP_SetCurrentBleLinkMtLink(p_bleConnList_t);
                        APP_LED_AlwaysOn(APP_LED_TYPE_GREEN, APP_LED_TYPE_NULL);
                    }
                }
            }
            else
        #endif
            if (APP_GetBleStateMtLink(p_bleConnList_t) == APP_BLE_STATE_ADVERTISING)
            {
                APP_ADV_Stop();
                p_bleConnList_t = APP_GetActiveBleLinkMtLink();
                if (p_bleConnList_t == NULL)
                    APP_LED_AlwaysOn(APP_LED_TYPE_NULL, APP_LED_TYPE_NULL);
                else
                {
                    if (APP_GetBleStateMtLink(p_bleConnList_t) == APP_BLE_STATE_CONNECTED)
                    {
                        APP_SetCurrentBleLinkMtLink(p_bleConnList_t);
                        APP_LED_AlwaysOn(APP_LED_TYPE_GREEN, APP_LED_TYPE_NULL);
                    }
                }
            }
    }
    #else
    if (keyMesg == APP_KEY_MSG_SHORT_PRESS)
    {
        if (APP_GetBleState() == APP_BLE_STATE_ADVERTISING)
        {
            APP_ADV_Stop();
            APP_LED_AlwaysOn(APP_LED_TYPE_NULL, APP_LED_TYPE_NULL);
            #ifdef APP_TRPC_ENABLE
            APP_SCAN_Start();
            #endif
        }
        #ifdef APP_TRPC_ENABLE
        else if (APP_GetBleState() == APP_BLE_STATE_SCANNING)
        {
            APP_SCAN_Stop();
            APP_LED_AlwaysOn(APP_LED_TYPE_NULL, APP_LED_TYPE_NULL);
            APP_ADV_Start();
        }
        #endif
    }
    #endif
}
#endif

void APP_KEY_Init(void)
{
    // GPIO register initialization for the user button.
    GPIOB_REGS->GPIO_ANSELCLR = APP_KEY_PB4;
    GPIOB_REGS->GPIO_TRISSET =  APP_KEY_PB4;

    memset((uint8_t *)&s_appKeyElement_t, 0, sizeof(APP_KEY_Elem_T));
    APP_TRP_COMMON_SetTimer(APP_TRP_TMR_ID_INST_MERGE(APP_TIMER_KEY_SCAN_06, 0), NULL, APP_TIMER_50MS,
        true, &s_scanTmr_t);
}

void APP_KEY_Scan(void)
{
    uint8_t keyState, keyMesg = APP_KEY_MSG_NULL;

    if ((GPIOB_REGS->GPIO_PORT & APP_KEY_PB4) == 0)
        keyState = APP_KEY_STATE_SHORT_PRESS;
    else
        keyState = APP_KEY_STATE_RELEASE;

    if (keyState == APP_KEY_STATE_SHORT_PRESS)
    {
        s_appKeyElement_t.counter++;
        if (s_appKeyElement_t.state == APP_KEY_STATE_RELEASE)
        {
            s_appKeyElement_t.state = APP_KEY_STATE_SHORT_PRESS;
        }
        else if(s_appKeyElement_t.state == APP_KEY_STATE_SHORT_PRESS)
        {
            if (s_appKeyElement_t.counter > APP_KEY_LONG_PRESS_TIME)
            {
                s_appKeyElement_t.state = APP_KEY_STATE_LONG_PRESS;
                keyMesg = APP_KEY_MSG_LONG_PRESS;
            }
        }
        else if (s_appKeyElement_t.state == APP_KEY_STATE_SHORT_PRESS_RELEASE)
        {
            s_appKeyElement_t.state = APP_KEY_STATE_DOUBLE_CLICK_PRESS;
            keyMesg = APP_KEY_MSG_DOUBLE_CLICK;
        }
    }
    else
    {
        if (s_appKeyElement_t.state != APP_KEY_STATE_RELEASE)
            s_appKeyElement_t.counter++;
        if ((s_appKeyElement_t.state == APP_KEY_STATE_SHORT_PRESS) && 
            (s_appKeyElement_t.counter <= APP_KEY_DOUBLE_CLICK_TIME))
            s_appKeyElement_t.state = APP_KEY_STATE_SHORT_PRESS_RELEASE;
        else if ((s_appKeyElement_t.state == APP_KEY_STATE_SHORT_PRESS) || ((s_appKeyElement_t.counter 
            > APP_KEY_DOUBLE_CLICK_TIME) && (s_appKeyElement_t.state == APP_KEY_STATE_SHORT_PRESS_RELEASE)))
            keyMesg = APP_KEY_MSG_SHORT_PRESS;
        if ((keyMesg != APP_KEY_MSG_NULL) || (s_appKeyElement_t.state == APP_KEY_STATE_LONG_PRESS)
            || (s_appKeyElement_t.state == APP_KEY_STATE_DOUBLE_CLICK_PRESS))
        {
            s_appKeyElement_t.state = APP_KEY_STATE_RELEASE;
            s_appKeyElement_t.counter = 0;
        }
    }
    
    if (keyMesg != APP_KEY_MSG_NULL)
    {
        #ifdef APP_BLE_MULTI_LINK_ENABLE
        APP_BLE_ConnList_T *p_bleConnList_t = NULL;

        p_bleConnList_t = APP_GetCurrentBleLinkMtLink();
        if (APP_GetBleStateMtLink(p_bleConnList_t) == APP_BLE_STATE_CONNECTED)
        {
        #ifdef APP_TRPC_ENABLE
            if (APP_GetBleRoleMtLink(p_bleConnList_t) == BLE_GAP_ROLE_CENTRAL)
                APP_TRPC_KeyTrigger(keyMesg);
            else
        #endif
                APP_TRPS_KeyTrigger(keyMesg);
        }
        else
            APP_KEY_NoBleLinkTrigger(keyMesg);
        #else
        if (APP_GetBleState() == APP_BLE_STATE_CONNECTED)
        {
        #ifdef APP_TRPC_ENABLE
            if (APP_GetBleRole() == BLE_GAP_ROLE_CENTRAL)
                APP_TRPC_KeyTrigger(keyMesg);
            else
        #endif
                APP_TRPS_KeyTrigger(keyMesg);
        }
        #ifdef APP_TRPC_ENABLE
        else
            APP_KEY_NoBleLinkTrigger(keyMesg);
        #endif
        #endif
    }
}
