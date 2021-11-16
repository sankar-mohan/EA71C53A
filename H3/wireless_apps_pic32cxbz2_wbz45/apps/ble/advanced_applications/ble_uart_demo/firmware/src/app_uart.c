/*******************************************************************************
  Application UART Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_uart.c

  Summary:
    This file contains the Application UART functions for this project.

  Description:
    This file contains the Application UART functions for this project.
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


#include "ble_util/byte_stream.h"
#include "app_uart.h"
#include <string.h>
#include "app_trps.h"
#include "app_cmd_evt_proc.h"
#include "app_error_defs.h"

#ifdef APP_UART_RX_BUF_EN
#include "app_timer.h"
#endif
#ifdef APP_TRPC_ENABLE
#include "app_trpc.h"
#endif
#ifdef APP_BLE_MULTI_LINK_ENABLE
#include "app_ble_handler.h"
#endif

#include "app.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define UART_RX_STATE_IDLE               0x00    /**< UART RX State: Idle. */
#define UART_RX_STATE_LENGTH_H           0x01    /**< UART RX State: High byte of Length . */
#define UART_RX_STATE_LENGTH_L           0x02    /**< UART RX State: Low byte of Length . */
#define UART_RX_STATE_CMD_ID             0x03    /**< UART RX State: Command ID. */
#define UART_RX_STATE_PAYLOAD            0x04    /**< UART RX State: Data Payload. */
#define UART_RX_STATE_CHECKSUM           0x05    /**< UART RX State: Checksum. */

#define ADP_MSG_RCV_UART_COMMAND_OK      0x00
#define ADP_MSG_RCV_UART_COMMAND_ERROR   0x01


// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

typedef struct APP_UartCmdMsg_T{
    uint8_t     status;
    uint16_t    len;
    uint8_t     payload[128];
}APP_UartCmdMsg_T;

typedef struct APP_UartList_T
{
    uint8_t  rxState;
    uint8_t  checksum;
    uint16_t rdIndex;
}APP_UartList_T;


// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************
static APP_UartCmdMsg_T     s_adpCmdMsg;
static APP_UartList_T       s_appUartList;
static bool                 s_appIsTxReady;

#ifndef APP_DISABLE_UART_POLL
#ifdef APP_UART_RX_BUF_EN
static APP_TIMER_TmrElem_T  s_appUartRxTimer;
#endif
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************

void APP_UartRx(uint8_t data)
{
    switch (s_appUartList.rxState)
    {
        case UART_RX_STATE_IDLE:
        {
            if (data==UART_START_BYTE)
            {
                s_appUartList.rxState = UART_RX_STATE_LENGTH_H;
            }
        }
        break;

        case UART_RX_STATE_LENGTH_H:
        {
            s_appUartList.checksum = data;
            s_adpCmdMsg.len = (data << 8);
            s_appUartList.rxState = UART_RX_STATE_LENGTH_L;
        }
        break;

        case UART_RX_STATE_LENGTH_L:
        {
            s_adpCmdMsg.len |= data;
            s_appUartList.checksum  += data;
            s_appUartList.rxState = UART_RX_STATE_CMD_ID;
            s_appUartList.rdIndex = 0;
        }
        break;

        case UART_RX_STATE_CMD_ID:
        {
            s_adpCmdMsg.payload[s_appUartList.rdIndex] = data;

            s_appUartList.rdIndex++;
            s_appUartList.checksum  += data;
            if ((s_adpCmdMsg.len - 1) > 0)
            {
                s_appUartList.rxState = UART_RX_STATE_PAYLOAD;
            }
            else
            {
                s_appUartList.rxState = UART_RX_STATE_CHECKSUM;
            }
        }
        break;

        case UART_RX_STATE_PAYLOAD:
        {
            s_adpCmdMsg.payload[s_appUartList.rdIndex] = data;

            s_appUartList.rdIndex++;
            s_appUartList.checksum  += data;
            if (s_appUartList.rdIndex > s_adpCmdMsg.len)
            {
                s_appUartList.rxState = UART_RX_STATE_IDLE;
            }
            else if (s_appUartList.rdIndex == s_adpCmdMsg.len)
            {
                s_appUartList.rxState = UART_RX_STATE_CHECKSUM;
            }
        }
        break;

        case UART_RX_STATE_CHECKSUM:
        {
            APP_Msg_T   appMsg;
            
            if (data == 0xff)
            {
                s_adpCmdMsg.status = ADP_MSG_RCV_UART_COMMAND_OK;
            }
            else
            {
                s_adpCmdMsg.status = ADP_MSG_RCV_UART_COMMAND_ERROR;
            }
            
            appMsg.msgId = APP_MSG_UART_CMD_READY;
            OSAL_QUEUE_SendISR(&appData.appQueue, &appMsg);
                            
            s_appUartList.rxState = UART_RX_STATE_IDLE;
        }
        break;

        default:
        {
            s_appUartList.rxState = UART_RX_STATE_IDLE;
        }
        break;
    }
}

static void APP_UartRxCb(SERCOM_USART_EVENT event, uintptr_t context)
{
    if (event == SERCOM_USART_EVENT_READ_THRESHOLD_REACHED)
    {
        SERCOM0_REGS->USART_INT.SERCOM_INTENCLR = SERCOM_USART_INT_INTENSET_RXC(1);
    }
}

static void APP_UartTx(void)
{
    s_appIsTxReady = true;
}

void APP_UartInit()
{
    _DRV_UART_PlibRegister_T  reg;

    memset(&reg, 0, sizeof(_DRV_UART_PlibRegister_T));
    reg.usartReadThreadByte                = APP_UART_MAX_RX_BUFFER_SIZE - 1;    //Rx Buffer Size -1
    reg.usartReadNotifyEnable              = true;
    reg.usartReadNotifyPersistent          = true;
    reg.usartReadCb                  = (SERCOM_USART_RING_BUFFER_CALLBACK)APP_UartRxCb;
    reg.usartWriteCb     = (SERCOM_USART_TX_DONE_CALLBACK)APP_UartTx;
    DRV_UART_Init(SERCOM_UART0, APP_UART_MAX_TX_BUFFER_SIZE, &reg);

    #if APP_LOG_ENABLE
    DRV_UART_Init(SERCOM_UART1, APP_UART_MAX_TX_BUFFER_SIZE, NULL);
    #endif

    s_appIsTxReady = true;
#ifndef APP_DISABLE_UART_POLL
    #ifdef APP_UART_RX_BUF_EN
    APP_TRP_COMMON_SetTimer(APP_TRP_TMR_ID_INST_MERGE(APP_TIMER_UART_FETCH_00, 0), NULL, APP_TIMER_20MS,
        true, &s_appUartRxTimer);
    #endif
#endif
}

uint16_t APP_UartPut(uint16_t length, uint8_t *p_buffer)
{
    uint32_t status;
    
//    while (!s_appIsTxReady) __ASM volatile ("nop");

    s_appIsTxReady = false;

    status = DRV_UART_PutBuf(SERCOM_UART0, length, p_buffer);

    if (status == 0)
        return APP_RES_SUCCESS;
    else
        return APP_RES_FAIL;
}

void APP_UART_FetchData(void)
{
    uint16_t dataLeng, i;
    uint8_t *p_data;
    bool trpcUartMode = false, trpsUartMode = false;
    #ifdef APP_BLE_MULTI_LINK_ENABLE
    uint8_t linkNum = APP_GetConnLinkNum();
    #endif

    dataLeng = DRV_UART_GetRxLength(SERCOM_UART0);
    #ifdef APP_TRPC_ENABLE
    trpcUartMode = APP_TRPC_IsUartMode();
    #endif
    trpsUartMode = APP_TRPS_IsUartMode();
    #ifdef APP_BLE_MULTI_LINK_ENABLE
    if ((trpcUartMode || trpsUartMode) && (linkNum < 2))
    #else
    if (trpcUartMode || trpsUartMode)
    #endif
    { 
        if (dataLeng > 0)
            APP_TRP_COMMON_UartFetchData(trpcUartMode, trpsUartMode, dataLeng);
        else
            APP_TRP_COMMON_CheckUartRxEnd(trpcUartMode, trpsUartMode);
    }
    #ifdef APP_BLE_MULTI_LINK_ENABLE
    else if ((trpcUartMode || trpsUartMode) && (linkNum > 1))
    {
        if (dataLeng > 0)
        {
            p_data = OSAL_Malloc(dataLeng);
            if (p_data == NULL)
                return;
            dataLeng = DRV_UART_ReadRxBuf(SERCOM_UART0, p_data, dataLeng);
            OSAL_Free(p_data);
        }
    }
    #endif
    else
    {
        if (dataLeng > 0)
        {
            p_data = OSAL_Malloc(dataLeng);
            if (p_data == NULL)
                return;
            dataLeng = DRV_UART_ReadRxBuf(SERCOM_UART0, p_data, dataLeng);
            for (i = 0; i < dataLeng; i++)
                APP_UartRx(p_data[i]);
            OSAL_Free(p_data);
        }
    }
}

void APP_UartMsgHandler()
{
#ifdef APP_UART_CMD_SET_ENABLE
    if (s_adpCmdMsg.status == ADP_MSG_RCV_UART_COMMAND_OK)
    {
        APP_ProcUartCmd(s_adpCmdMsg.len , s_adpCmdMsg.payload);
    }
    else
    {
        APP_SendCmdCompleteEvt(0, 0, 0, 0xff, NULL);
    }
#endif

}

bool APP_Uart_IsTxIdle(void)
{
    if (s_appIsTxReady && DRV_UART_Sercom1TxIsReady())
    {
        return true;
    }
    else
    {
        return false;
    }
}
