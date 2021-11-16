/*******************************************************************************
  Application Transparent Common Function Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_trp_common.c

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
#include "stack_mgr.h"
#include "app_uart.h"
#include "ble_trsps/ble_trsps.h"
#include "ble_trcbps/ble_trcbps.h"

#include "app_trp_common.h"
#include "app_timer.h"
#include "app_ble.h"
#include "drv_usart.h"
#include "app_trps.h"
#ifdef APP_TRPC_ENABLE
#include "ble_trspc/ble_trspc.h"
#include "app_trpc.h"
#endif
#ifdef APP_BLE_MULTI_LINK_ENABLE
#include "app_ble_handler.h"
#endif
#include "app_error_defs.h"
#include "app_log.h"

#ifdef APP_DISABLE_UART_POLL
#include "app_eic.h"
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
// Section: Global Variables
// *****************************************************************************
// *****************************************************************************
uint32_t trpLeRxCount, trpLeTxCount, trpUartRxCount, trpUartTxCount; // For debug purpose
#ifdef APP_BLE_MULTI_LINK_ENABLE
APP_TRP_TrafficPriority_T       g_trpcTrafficPriority_t, g_trpsTrafficPriority_t;
#endif
uint8_t g_trpUartQueueNum, g_trpClientLinkNum, g_trpServerLinkNum;

// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************
static APP_TRP_GenData_T        s_trpRxUartData_t;

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
void APP_TRP_COMMON_Init(void)
{
    memset((uint8_t *) &s_trpRxUartData_t, 0, sizeof(APP_TRP_GenData_T));
#ifdef APP_BLE_MULTI_LINK_ENABLE
    memset((uint8_t *) &g_trpcTrafficPriority_t, 0, sizeof(APP_TRP_TrafficPriority_T));
    memset((uint8_t *) &g_trpsTrafficPriority_t, 0, sizeof(APP_TRP_TrafficPriority_T));
#endif
    g_enableUartMode = 0;
    g_trpUartQueueNum = APP_UTILITY_MAX_QUEUE_NUM;
    g_trpClientLinkNum = 0;
    g_trpServerLinkNum = 0;
}

uint16_t APP_TRP_COMMON_SetTimer(uint16_t idInstance, void *p_tmrParam, 
    uint32_t timeout, bool isPeriodicTimer, APP_TIMER_TmrElem_T *p_tmrElem)
{
    uint8_t tmrId;
    uint16_t result;

    tmrId = (uint8_t)(idInstance >> 8);
    APP_TIMER_SetTimerElem(tmrId, (uint8_t)idInstance, (void *)p_tmrParam, p_tmrElem);
    result = APP_TIMER_SetTimer(p_tmrElem, timeout, isPeriodicTimer);
    
    if (result != APP_RES_SUCCESS)
    {
        switch(tmrId)
        {
            case APP_TIMER_UART_FETCH_00:
                APP_LOG_ERROR("APP_TIMER_UART_FETCH_00 error ! result=%d\n", result);
                break;

            case APP_TIMER_PROTOCOL_RSP_01:
                APP_LOG_ERROR("APP_TIMER_PROTOCOL_RSP_01 error ! result=%d\n", result);
                break;

            case APP_TIMER_FETCH_TRP_RX_DATA_02:
                APP_LOG_ERROR("APP_TIMER_FETCH_TRP_RX_DATA_02 error ! result=%d\n", result);
                break;

            case APP_TIMER_UART_SEND_03:
                APP_LOG_ERROR("APP_TIMER_UART_SEND_03 error ! result=%d\n", result);
                break;

            case APP_TIMER_LED_04:
                APP_LOG_ERROR("APP_TIMER_LED_04 error ! result=%d\n", result);
                break;

            case APP_TIMER_SCAN_ADV_05:
                APP_LOG_ERROR("APP_TIMER_SCAN_ADV_05 error ! result=%d\n", result);
                break;

            case APP_TIMER_KEY_SCAN_06:
                APP_LOG_ERROR("APP_TIMER_KEY_SCAN_06 error ! result=%d\n", result);
                break;

            default:
                APP_LOG_ERROR("APP_TIMER_RESERVED error ! result=%d\n", result);
                break;
        }
    }

    return result;
}

uint8_t APP_TRP_COMMON_GetValidLeCircQueueNum(APP_TRP_LeCircQueue_T *p_circQueue_t)
{
    uint8_t validNum = 0;
    
    if (p_circQueue_t != NULL)
    {
        if (p_circQueue_t->usedNum < APP_TRP_LE_MAX_QUEUE_NUM)
            validNum = APP_TRP_LE_MAX_QUEUE_NUM - p_circQueue_t->usedNum;
    }
    
    return validNum;
}

uint16_t APP_TRP_COMMON_InsertDataToLeCircQueue(uint16_t dataLeng, uint8_t *p_data, 
    APP_TRP_LeCircQueue_T *p_circQueue_t)
{
    uint16_t status = APP_RES_SUCCESS;

    if ((dataLeng > 0) && (p_data != NULL) && (p_circQueue_t != NULL))
    {
        if (APP_TRP_COMMON_GetValidLeCircQueueNum(p_circQueue_t) > 0)
        {
            p_circQueue_t->queueElem[p_circQueue_t->writeIdx].dataLeng = dataLeng;
            p_circQueue_t->queueElem[p_circQueue_t->writeIdx].p_data = p_data;
            p_circQueue_t->usedNum++;
            p_circQueue_t->writeIdx++;
            if (p_circQueue_t->writeIdx >= APP_TRP_LE_MAX_QUEUE_NUM)
                p_circQueue_t->writeIdx = 0;
        }
        else
            return APP_RES_NO_RESOURCE;
    }
    else
        return APP_RES_INVALID_PARA;
    
    return status;
}

APP_UTILITY_QueueElem_T * APP_TRP_COMMON_GetElemLeCircQueue(APP_TRP_LeCircQueue_T *p_circQueue_t)
{
    APP_UTILITY_QueueElem_T *p_queueElem_t;

    if (p_circQueue_t != NULL)
    {
        if (p_circQueue_t->usedNum > 0)
        {
            p_queueElem_t = (APP_UTILITY_QueueElem_T *) &(p_circQueue_t->queueElem[p_circQueue_t->readIdx].dataLeng);
        }
        else
            return NULL;
    }
    else
        return NULL;
    
    return p_queueElem_t;
}

void APP_TRP_COMMON_FreeElemLeCircQueue(APP_TRP_LeCircQueue_T *p_circQueue_t)
{
    if (p_circQueue_t != NULL)
    {
        p_circQueue_t->queueElem[p_circQueue_t->readIdx].dataLeng = 0;
        if (p_circQueue_t->queueElem[p_circQueue_t->readIdx].p_data != NULL)
            OSAL_Free(p_circQueue_t->queueElem[p_circQueue_t->readIdx].p_data);
        if (p_circQueue_t->usedNum > 0)
            p_circQueue_t->usedNum--;
        p_circQueue_t->readIdx++;
        if (p_circQueue_t->readIdx >= APP_TRP_LE_MAX_QUEUE_NUM)
            p_circQueue_t->readIdx = 0;
    }
}

uint16_t APP_TRP_COMMON_CheckCtrlChannel(APP_TRP_ConnList_T *p_connList_t)
{
    uint16_t result = APP_RES_FAIL;

    if ((p_connList_t->trpRole == APP_TRP_CLIENT_ROLE) || ((p_connList_t->trpRole == APP_TRP_SERVER_ROLE)
        && ((p_connList_t->channelEn & APP_TRP_CTRL_CHAN_ENABLE) || (p_connList_t->channelEn & APP_TRCBP_CTRL_CHAN_ENABLE))))
        result = APP_RES_SUCCESS;

    return result;
}

void APP_TRP_COMMON_SetCtrlRspFg(APP_TRP_ConnList_T *p_connList_t, uint16_t result, uint16_t flag)
{
    if (result == APP_RES_SUCCESS)
        p_connList_t->txCtrRspFg &= ~flag;
    else
        p_connList_t->txCtrRspFg |= flag;
}

uint16_t APP_TRP_COMMON_SendVendorCmd(APP_TRP_ConnList_T *p_connList_t, uint16_t length, uint8_t *p_payload)
{
    uint16_t result = APP_RES_FAIL;

    if (p_connList_t->trpRole == APP_TRP_SERVER_ROLE)
    {
        if ((p_connList_t->channelEn & APP_TRCBP_CTRL_CHAN_ENABLE) && (p_connList_t->type == APP_TRP_TYPE_TRCBP))      //TRCBP
        {
            result = BLE_TRCBPS_SendVendorCommand(p_connList_t->connHandle, APP_TRP_VENDOR_OPCODE_BLE_UART, 
                length, p_payload);
        }
        else if (p_connList_t->channelEn & APP_TRP_CTRL_CHAN_ENABLE)   //Legacy TRP
        {
            result = BLE_TRSPS_SendVendorCommand(p_connList_t->connHandle, APP_TRP_VENDOR_OPCODE_BLE_UART, 
                length, p_payload);
        }
    }
#ifdef APP_TRPC_ENABLE
    else if (p_connList_t->trpRole == APP_TRP_CLIENT_ROLE)
    {
        if (p_connList_t->type == APP_TRP_TYPE_LEGACY)  //Legacy TRP
        {
            result = BLE_TRSPC_SendVendorCommand(p_connList_t->connHandle, APP_TRP_VENDOR_OPCODE_BLE_UART, length, p_payload);
        }
    }
#endif

    return result;
}

uint16_t APP_TRP_COMMON_SendModeCommand(APP_TRP_ConnList_T *p_connList_t, uint8_t grpId, uint8_t commandId)
{
    uint8_t payload[APP_TRP_WMODE_TX_MODE_PL_NUM], idx;
    uint16_t result = APP_RES_FAIL, rspFlag = APP_TRP_SEND_GID_UART_FAIL;
    
    result = APP_TRP_COMMON_CheckCtrlChannel(p_connList_t);
    
    if (result == APP_RES_SUCCESS)
    {
        idx = 0;
        payload[idx++] = grpId;
        payload[idx] = commandId;

        result = APP_TRP_COMMON_SendVendorCmd(p_connList_t, APP_TRP_WMODE_TX_MODE_PL_NUM, payload);
        
        switch(grpId)
        {
            case TRP_GRPID_CHECK_SUM:
                rspFlag = APP_TRP_SEND_GID_CS_FAIL;
                break;

            case TRP_GRPID_FIX_PATTERN:
                rspFlag = APP_TRP_SEND_GID_FP_FAIL;
                break;

            case TRP_GRPID_LOOPBACK:
                rspFlag = APP_TRP_SEND_GID_LB_FAIL;
                break;

            case TRP_GRPID_UART:
                rspFlag = APP_TRP_SEND_GID_UART_FAIL;
                break;

            case TRP_GRPID_TRANSMIT:
                rspFlag = APP_TRP_SEND_GID_TX_FAIL;
                break;

            case TRP_GRPID_REV_LOOPBACK:
                rspFlag = APP_TRP_SEND_GID_REV_LB_FAIL;
                break;
            default:
                break;
        }
        APP_TRP_COMMON_SetCtrlRspFg(p_connList_t, result, rspFlag);
    }
    
    return result;
}

uint16_t APP_TRP_COMMON_SendLengthCommand(APP_TRP_ConnList_T *p_connList_t, 
    uint32_t length)
{
    uint8_t payload[APP_TRP_WMODE_TX_LENGTH_PL_NUM], idx;
    uint16_t result = APP_RES_FAIL;

    result = APP_TRP_COMMON_CheckCtrlChannel(p_connList_t);
    if (result == APP_RES_SUCCESS)
    {
        idx = 0;
        payload[idx++] = TRP_GRPID_TRANSMIT;
        payload[idx++] = APP_TRP_WMODE_TX_DATA_LENGTH;
        payload[idx++] = (uint8_t)(length >> 24);
        payload[idx++] = (uint8_t)(length >> 16);
        payload[idx++] = (uint8_t)(length >> 8);
        payload[idx] = (uint8_t)length;

        result = APP_TRP_COMMON_SendVendorCmd(p_connList_t, APP_TRP_WMODE_TX_LENGTH_PL_NUM, payload);
        APP_TRP_COMMON_SetCtrlRspFg(p_connList_t, result, APP_TRP_SEND_LENGTH_FAIL);
    }

    return result;
}

uint16_t APP_TRP_COMMON_SendTypeCommand(APP_TRP_ConnList_T *p_connList_t)
{
    uint8_t payload[APP_TRP_WMODE_TX_TYPE_PL_NUM], idx;
    uint16_t result = APP_RES_FAIL;

    result = APP_TRP_COMMON_CheckCtrlChannel(p_connList_t);
    
    if (result == APP_RES_SUCCESS)
    {
        idx = 0;
        payload[idx++] = TRP_GRPID_TRANSMIT;
        payload[idx++] = APP_TRP_WMODE_TX_TYPE;
        payload[idx] = p_connList_t->type;

        result = APP_TRP_COMMON_SendVendorCmd(p_connList_t, APP_TRP_WMODE_TX_TYPE_PL_NUM, payload);
        APP_TRP_COMMON_SetCtrlRspFg(p_connList_t, result, APP_TRP_SEND_TYPE_FAIL);
    }

    return result;
}

uint16_t APP_TRP_COMMON_SendCheckSumCommand(APP_TRP_ConnList_T *p_connList_t)
{
    uint8_t payload[APP_TRP_WMODE_CHECK_SUM_PL_NUM], idx;
    uint16_t result = APP_RES_FAIL;

    result = APP_TRP_COMMON_CheckCtrlChannel(p_connList_t);
    if (result == APP_RES_SUCCESS)
    {
        idx = 0;
        payload[idx++] = TRP_GRPID_CHECK_SUM;
        payload[idx++] = APP_TRP_WMODE_CHECK_SUM;
        payload[idx] = (uint8_t) p_connList_t->checkSum;

        result = APP_TRP_COMMON_SendVendorCmd(p_connList_t, APP_TRP_WMODE_CHECK_SUM_PL_NUM, payload);
        APP_TRP_COMMON_SetCtrlRspFg(p_connList_t, result, APP_TRP_SEND_CHECK_SUM_FAIL);
    }

    return result;
}

uint16_t APP_TRP_COMMON_SendLastNumber(APP_TRP_ConnList_T *p_connList_t)
{
    uint8_t payload[APP_TRP_WMODE_FIX_PATTERN_PL_NUM], idx;
    uint16_t result = APP_RES_FAIL, lastNumber = 0;

    result = APP_TRP_COMMON_CheckCtrlChannel(p_connList_t);
    if (result == APP_RES_SUCCESS)
    {
        idx = 0;
        if (p_connList_t->trpRole == APP_TRP_SERVER_ROLE)
            lastNumber = p_connList_t->lastNumber - 1;
        else
            lastNumber = p_connList_t->lastNumber;
        payload[idx++] = TRP_GRPID_FIX_PATTERN;
        payload[idx++] = APP_TRP_WMODE_TX_LAST_NUMBER;
        payload[idx++] = (uint8_t)(lastNumber >> 8);
        payload[idx] = (uint8_t)lastNumber;

        result = APP_TRP_COMMON_SendVendorCmd(p_connList_t, APP_TRP_WMODE_FIX_PATTERN_PL_NUM, payload);
        APP_TRP_COMMON_SetCtrlRspFg(p_connList_t, result, APP_TRP_SEND_LAST_NUMBER_FAIL);
    }
    
    return result;
}

uint16_t APP_TRP_COMMON_SendErrorRsp(APP_TRP_ConnList_T *p_connList_t, 
    uint8_t grpId)
{
    uint8_t payload[APP_TRP_WMODE_ERROR_PL_NUM], idx;
    uint16_t result = APP_RES_FAIL;

    result = APP_TRP_COMMON_CheckCtrlChannel(p_connList_t);
    if (result == APP_RES_SUCCESS)
    {
        idx = 0;
        payload[idx++] = grpId;
        payload[idx] = APP_TRP_WMODE_ERROR_RSP;

        result = APP_TRP_COMMON_SendVendorCmd(p_connList_t, APP_TRP_WMODE_ERROR_PL_NUM, payload);
        APP_TRP_COMMON_SetCtrlRspFg(p_connList_t, result, APP_TRP_SEND_ERROR_RSP_FAIL);
    }
    
    return result;
}

uint16_t APP_TRP_COMMON_SendUpConnParaStatus(APP_TRP_ConnList_T *p_connList_t, uint8_t grpId, uint8_t commandId, uint8_t upParaStatus)
{
    uint8_t payload[APP_TRP_WMODE_NOTIFY_PL_NUM], idx;
    uint16_t result = APP_RES_FAIL;

    result = APP_TRP_COMMON_CheckCtrlChannel(p_connList_t);
    
    if (result == APP_RES_SUCCESS)
    {
        idx = 0;
        payload[idx++] = grpId;
        payload[idx++] = commandId;
        payload[idx] = upParaStatus;

        result = APP_TRP_COMMON_SendVendorCmd(p_connList_t, APP_TRP_WMODE_NOTIFY_PL_NUM, payload);
        APP_TRP_COMMON_SetCtrlRspFg(p_connList_t, result, APP_TRP_SEND_STATUS_FLAG);
    }
    
    return result;
}
    
uint16_t APP_TRP_COMMON_GetTrpDataLength(APP_TRP_ConnList_T *p_connList_t, uint16_t *p_dataLeng)
{
    uint16_t status = APP_RES_INVALID_PARA;
    
    if (p_connList_t->trpRole == APP_TRP_SERVER_ROLE)
    {
        if (p_connList_t->type == APP_TRP_TYPE_LEGACY)
        {
            BLE_TRSPS_GetDataLength(p_connList_t->connHandle, p_dataLeng);
            status = APP_RES_SUCCESS;
        }
        else
        {
            status = BLE_TRCBPS_GetDataLength(p_connList_t->connHandle, p_dataLeng);
        }
    }
    #ifdef APP_TRPC_ENABLE
    else if(p_connList_t->trpRole == APP_TRP_CLIENT_ROLE)
    {
        if (p_connList_t->type == APP_TRP_TYPE_LEGACY)
        {
            BLE_TRSPC_GetDataLength(p_connList_t->connHandle, p_dataLeng);
            status = APP_RES_SUCCESS;
        }
    }
    #endif

    return status;
}

uint16_t APP_TRP_COMMON_GetTrpData(APP_TRP_ConnList_T *p_connList_t, uint8_t *p_data)
{
    uint16_t status = APP_RES_FAIL;

    if (p_data == NULL)
        return status;
    
    if (p_connList_t->trpRole == APP_TRP_SERVER_ROLE)
    {
        if (p_connList_t->type == APP_TRP_TYPE_LEGACY)
        {
            status = BLE_TRSPS_GetData(p_connList_t->connHandle, p_data);
        }
        else
        {
            status = BLE_TRCBPS_GetData(p_connList_t->connHandle, p_data);
        }
    }
#ifdef APP_TRPC_ENABLE
    else if(p_connList_t->trpRole == APP_TRP_CLIENT_ROLE)
    {
        if (p_connList_t->type == APP_TRP_TYPE_LEGACY)
        {
            status = BLE_TRSPC_GetData(p_connList_t->connHandle, p_data);
        }
    }
#endif
    
    return status;
}

uint16_t APP_TRP_COMMON_FreeLeData(APP_TRP_ConnList_T *p_connList_t)
{
    uint16_t dataLeng = 0, status = APP_RES_SUCCESS;
    uint8_t *p_data;

    status = APP_TRP_COMMON_GetTrpDataLength(p_connList_t, &dataLeng);
    if (status != APP_RES_SUCCESS)
        return status;
    if (dataLeng > 0)
    {
        p_data = OSAL_Malloc(dataLeng);
        if (p_data != NULL)
        {
            status = APP_TRP_COMMON_GetTrpData(p_connList_t, p_data);
            OSAL_Free(p_data);
        }
        else
        {
            status = APP_RES_OOM;
        }
    }

    return status;
}

void APP_TRP_COMMON_DelAllCircData(APP_UTILITY_CircQueue_T *p_circQueue_t)
{
    uint8_t i;

    if (p_circQueue_t->usedNum > 0)
    {
        for (i = 0; i < p_circQueue_t->usedNum; i++)
            APP_UTILITY_FreeElemCircQueue(p_circQueue_t);
    }
}

void APP_TRP_COMMON_DelAllLeCircData(APP_TRP_LeCircQueue_T *p_circQueue_t)
{
    uint8_t i;

    if (p_circQueue_t->usedNum > 0)
    {
        for (i = 0; i < p_circQueue_t->usedNum; i++)
            APP_TRP_COMMON_FreeElemLeCircQueue(p_circQueue_t);
    }
}

uint32_t APP_TRP_COMMON_CalculateCheckSum(uint32_t checkSum, uint32_t *dataLeng,
    APP_TRP_ConnList_T *p_connList_t)
{
    uint16_t tmpLeng, status;
    uint32_t i;
    uint8_t *p_data = NULL;

    if (((*dataLeng) == 0) || (p_connList_t == NULL))
        return checkSum;

    status = APP_TRP_COMMON_GetTrpDataLength(p_connList_t, &tmpLeng);
    if (status != APP_RES_SUCCESS)
        return checkSum;

    while (tmpLeng > 0)
    {
        p_data = OSAL_Malloc(tmpLeng);
        if (p_data != NULL)
        {
            status = APP_TRP_COMMON_GetTrpData(p_connList_t, p_data);
            if (status == APP_RES_SUCCESS)
            {
                for (i = 0; i < tmpLeng; i++)
                    checkSum += p_data[i];
                if ((*dataLeng) > tmpLeng)
                    *dataLeng -= tmpLeng;
                else
                    *dataLeng = 0;
            }
            OSAL_Free(p_data);
            if ((*dataLeng) == 0)
                break;
            status = APP_TRP_COMMON_GetTrpDataLength(p_connList_t, &tmpLeng);
            if (status != APP_RES_SUCCESS)
                return checkSum;
        }
        else
            return checkSum;
    }

    //Start a timer then reset the timer every time the device receives the data
    //If timeout occurs, send out current checksum directly.
    if (*dataLeng != 0)
    {
        APP_TRP_COMMON_SetTimer(APP_TRP_TMR_ID_INST_MERGE(APP_TIMER_PROTOCOL_RSP_01, 0),
            (void *)p_connList_t, APP_TIMER_3S, false, &(p_connList_t->protocolRspTmr));
    }

    return checkSum;
}

uint8_t * APP_TRP_COMMON_GenFixPattern(uint16_t *startSeqNum, uint16_t *patternLeng,
    uint32_t *pattMaxSize, uint32_t *checkSum)
{
    uint8_t *p_data, *p_buf;
    uint16_t i;
    
    if ((*patternLeng == 0) || (*patternLeng == 1) || (*pattMaxSize == 0))
        return NULL;
    
    *patternLeng = *patternLeng & ~0x01;  // even length
    p_data = OSAL_Malloc(*patternLeng);
    
    if (p_data == NULL)
        return p_data;
    
    p_buf = p_data;
    i = 0;
    
    while (i < *patternLeng)
    {
        U16_TO_STREAM_BE(&p_buf, *startSeqNum);
        (*startSeqNum)++;
        i += 2;
    }
    
    if (*pattMaxSize > *patternLeng)
        *pattMaxSize -= *patternLeng;
    else
        *pattMaxSize = 0;
    
    for (i = 0; i < *patternLeng; i++)
        *checkSum += p_data[i];

    return p_data;
}

uint16_t APP_TRP_COMMON_UpdateFixPatternLen(APP_TRP_ConnList_T *p_connList_t)
{
    uint16_t patternLeng = 0;

    if (p_connList_t->type == APP_TRP_TYPE_LEGACY)   //Legacy TRPS
    {
        if (p_connList_t->fixPattMaxSize > p_connList_t->txMTU)
        {
            patternLeng = p_connList_t->txMTU;
        }
        else
        {
            patternLeng = p_connList_t->fixPattMaxSize;
        }
    }
    else if (p_connList_t->channelEn & APP_TRCBP_DATA_CHAN_ENABLE)       //TRCBPS
    {
        if (p_connList_t->fixPattMaxSize > p_connList_t->fixPattTrcbpMtu)
        {
            patternLeng = p_connList_t->fixPattTrcbpMtu;
        }
        else
        {
            patternLeng = p_connList_t->fixPattMaxSize;
        }
    }

    return patternLeng;
}

uint16_t APP_TRP_COMMON_SendLeData(APP_TRP_ConnList_T *p_connList_t, uint16_t len, uint8_t *p_data)
{
    uint16_t status = APP_RES_FAIL;

    if (p_connList_t->trpRole == APP_TRP_SERVER_ROLE)
    {
        if ((p_connList_t->channelEn & APP_TRP_DATA_CHAN_ENABLE) || (p_connList_t->channelEn & APP_TRCBP_DATA_CHAN_ENABLE))
        {
            if (p_connList_t->type == APP_TRP_TYPE_LEGACY)
            {
                status = BLE_TRSPS_SendData(p_connList_t->connHandle, len, p_data);
            }
            else
            {
                status = BLE_TRCBPS_SendData(p_connList_t->connHandle, len, p_data);
            }
        }
    }
#ifdef APP_TRPC_ENABLE
    else if (p_connList_t->trpRole == APP_TRP_CLIENT_ROLE)
    {
        if (p_connList_t->type == APP_TRP_TYPE_LEGACY)  //Legacy TRP
        {
            status = BLE_TRSPC_SendData(p_connList_t->connHandle, len, p_data);
        }
    }
#endif

    return status;
}

void APP_TRP_COMMON_InitFixPatternParam(APP_TRP_ConnList_T *p_connList_t)
{
    p_connList_t->fixPattMaxSize = APP_TRP_WMODE_TX_MAX_SIZE;
    p_connList_t->lastNumber = 0;
    p_connList_t->rxLastNunber = 0;
    p_connList_t->checkSum = 0;
}

uint16_t APP_TRP_COMMON_SendFixPatternFirstPkt(APP_TRP_ConnList_T *p_connList_t)
{
    uint8_t *p_TrsBuf = NULL;
    uint16_t status = APP_RES_SUCCESS, dataLength = 0;
                    
    if (p_connList_t->type == APP_TRP_TYPE_LEGACY)
    {
        p_TrsBuf = APP_TRP_COMMON_GenFixPattern(&(p_connList_t->lastNumber), &(p_connList_t->txMTU), 
            &(p_connList_t->fixPattMaxSize), &(p_connList_t->checkSum));
    }
    else if (p_connList_t->channelEn & APP_TRCBP_DATA_CHAN_ENABLE)
    {
        p_TrsBuf = APP_TRP_COMMON_GenFixPattern(&(p_connList_t->lastNumber), &(p_connList_t->fixPattTrcbpMtu), 
            &(p_connList_t->fixPattMaxSize), &(p_connList_t->checkSum));
    }

    if (p_TrsBuf == NULL)
        return APP_RES_OOM;

    if (p_connList_t->type == APP_TRP_TYPE_LEGACY)
    {
        dataLength = p_connList_t->txMTU;
    }
    else if (p_connList_t->channelEn & APP_TRCBP_DATA_CHAN_ENABLE)
    {
        dataLength = p_connList_t->fixPattTrcbpMtu;
    }
    
    status = APP_TRP_COMMON_SendLeData(p_connList_t, dataLength, p_TrsBuf);
    
    if (status != APP_RES_SUCCESS)
    {
        p_connList_t->lastNumber = 0;
        p_connList_t->fixPattMaxSize = APP_TRP_WMODE_TX_MAX_SIZE;
    }

    OSAL_Free(p_TrsBuf);

    return status;
}

uint16_t APP_TRP_COMMON_SendFixPattern(APP_TRP_ConnList_T *p_connList_t)
{
    uint8_t *p_data, validNum = APP_TRP_MAX_TRANSMIT_NUM;
    uint16_t status = APP_RES_FAIL, patternLeng;
    uint16_t lastNum;
    uint32_t leftSize, lastCheckSum;

    patternLeng = APP_TRP_COMMON_UpdateFixPatternLen(p_connList_t);

    lastNum = p_connList_t->lastNumber;
    leftSize = p_connList_t->fixPattMaxSize;
    lastCheckSum = p_connList_t->checkSum;

    p_data = APP_TRP_COMMON_GenFixPattern(&(p_connList_t->lastNumber), &(patternLeng), &(p_connList_t->fixPattMaxSize),
        &(p_connList_t->checkSum));

    if (p_data == NULL)
        return APP_RES_OOM;

    while (p_data != NULL)
    {
        status = APP_TRP_COMMON_SendLeData(p_connList_t, patternLeng, p_data);

        OSAL_Free(p_data);

        if (status == APP_RES_SUCCESS)
        {
            // trpLeTxCount += patternLeng;
            validNum--;
            if (p_connList_t->fixPattMaxSize == 0)
            {
                status |= APP_RES_COMPLETE;
                return status;
            }
            else
            {
                if (validNum > 0) // Limit transmit number
                {
                    patternLeng = APP_TRP_COMMON_UpdateFixPatternLen(p_connList_t);

                    lastNum = p_connList_t->lastNumber;
                    leftSize = p_connList_t->fixPattMaxSize;
                    lastCheckSum = p_connList_t->checkSum;

                    p_data = APP_TRP_COMMON_GenFixPattern(&(p_connList_t->lastNumber), &(patternLeng), 
                        &(p_connList_t->fixPattMaxSize), &(p_connList_t->checkSum));

                    if (p_data == NULL)
                        return APP_RES_OOM;
                }
                else
                {
                    status = APP_RES_SUCCESS;
                    break;
                }
            }
        }
        else
        {
            p_connList_t->lastNumber = lastNum;
            p_connList_t->fixPattMaxSize = leftSize;
            p_connList_t->checkSum = lastCheckSum;

            break;
        }
    }
    
    return status;
}

#ifdef APP_BLE_MULTI_LINK_ENABLE
uint16_t APP_TRP_COMMON_SendMultiLinkFixPattern(APP_TRP_TrafficPriority_T *p_connToken, APP_TRP_ConnList_T *p_connList_t)
{
    uint8_t *p_data;
    uint16_t status = APP_RES_FAIL, patternLeng;
    uint16_t lastNum;
    uint32_t leftSize, lastCheckSum;
    
    if ((p_connToken == NULL) || (p_connList_t == NULL))
        return APP_RES_INVALID_PARA;
    
    patternLeng = APP_TRP_COMMON_UpdateFixPatternLen(p_connList_t);
    lastNum = p_connList_t->lastNumber;
    leftSize = p_connList_t->fixPattMaxSize;
    lastCheckSum = p_connList_t->checkSum;

    p_data = APP_TRP_COMMON_GenFixPattern(&(p_connList_t->lastNumber), &(patternLeng), &(p_connList_t->fixPattMaxSize), &(p_connList_t->checkSum));

    if (p_data != NULL)
    {
        status = APP_TRP_COMMON_SendLeData(p_connList_t, patternLeng, p_data);

        OSAL_Free(p_data);

        if (status == APP_RES_SUCCESS)
        {
            p_connToken->validNumber--;
            p_connList_t->maxAvailTxNumber--;
            
            if (p_connList_t->fixPattMaxSize == 0)
            {
                status |= APP_RES_COMPLETE;
                return status;
            }
        }
        else
        {
            p_connList_t->lastNumber = lastNum;
            p_connList_t->fixPattMaxSize = leftSize;
            p_connList_t->checkSum = lastCheckSum;
        }
    }
    else
        return APP_RES_OOM;
    
    return status;
}
#endif

uint16_t APP_TRP_COMMON_CheckFixPatternData(APP_TRP_ConnList_T *p_connList_t)
{
    uint16_t tmpLeng, status = APP_RES_SUCCESS, i, fixPatternData;
    uint8_t *p_data = NULL;

    if (p_connList_t == NULL)
        return APP_RES_INVALID_PARA;

    status = APP_TRP_COMMON_GetTrpDataLength(p_connList_t, &tmpLeng);
    
    if (status != APP_RES_SUCCESS)
        return status;

    while (tmpLeng > 0)
    {
        p_data = OSAL_Malloc(tmpLeng);
        
        if (p_data != NULL)
        {
            status = APP_TRP_COMMON_GetTrpData(p_connList_t, p_data);
            
            if (status == APP_RES_SUCCESS)
            {
                for (i = 0; i < tmpLeng; i += 2)
                {
                    fixPatternData = p_data[i];
                    fixPatternData = (fixPatternData << 8) | p_data[i+1];
                    
                    if (p_connList_t->rxLastNunber != fixPatternData)
                    {
                        status = APP_RES_FAIL;
                        break;
                    }
                    else
                        (p_connList_t->rxLastNunber)++;
                }
            }
            
            OSAL_Free(p_data);

            if (status != APP_RES_SUCCESS)
                break;
            
            status = APP_TRP_COMMON_GetTrpDataLength(p_connList_t, &tmpLeng);
            
            if (status != APP_RES_SUCCESS)
                return status;
        }
        else
            return APP_RES_OOM;
    }

    return status;
}

uint16_t APP_TRP_COMMON_CheckMultiLinkFixPatternData(APP_TRP_TrafficPriority_T *p_connToken, APP_TRP_ConnList_T *p_connList_t)
{
    uint16_t tmpLeng = 0, status = APP_RES_SUCCESS, i, fixPatternData;
    uint8_t *p_data = NULL;
    

    if (p_connList_t == NULL)
    {
        return APP_RES_INVALID_PARA;
    }
    
    status = APP_TRP_COMMON_GetTrpDataLength(p_connList_t, &tmpLeng);
    
    if (status != APP_RES_SUCCESS)
    {
        return status;
    }

    while (tmpLeng > 0)
    {
        p_data = OSAL_Malloc(tmpLeng);
        
        if (p_data != NULL)
        {
            status = APP_TRP_COMMON_GetTrpData(p_connList_t, p_data);
            
            if (status == APP_RES_SUCCESS)
            {
                for (i = 0; i < tmpLeng; i += 2)
                {
                    fixPatternData = p_data[i];
                    fixPatternData = (fixPatternData << 8) | p_data[i+1];
                    
                    if (p_connList_t->rxLastNunber != fixPatternData)
                    {
                        status = APP_RES_FAIL;
                        break;
                    }
                    else
                    {
                        (p_connList_t->rxLastNunber)++;
                    }
                }
            }
            
            OSAL_Free(p_data);

            if (status != APP_RES_SUCCESS)
            {
                break;
            }
            
            status = APP_TRP_COMMON_GetTrpDataLength(p_connList_t, &tmpLeng);
            
            if (status != APP_RES_SUCCESS)
            {
                return status;
            }
        }
        else
        {
            return APP_RES_OOM;
        }
    }

    return status;
}

uint16_t APP_TRP_COMMON_SendLeDataToUart(uint16_t dataLeng, uint8_t *p_rxBuf)
{
    uint16_t status = APP_RES_INVALID_PARA;

    if ((dataLeng > 0) && (p_rxBuf != NULL))
    {
        status = APP_UartPut(dataLeng, p_rxBuf);
    }

    return status;
}

void APP_TRP_COMMON_SendTrpProfileDataToUART(APP_TRP_ConnList_T *p_connList_t)
{
    APP_TRP_LeCircQueue_T *p_leCircQueue_t = &(p_connList_t->leCircQueue_t);
    APP_UTILITY_QueueElem_T *p_queueElem_t = NULL;
    uint16_t status = APP_RES_SUCCESS, dataLeng = 0;
    uint8_t validNum = APP_TRP_MAX_TRANSMIT_NUM, *p_data = NULL;

    if (p_connList_t == NULL)
        return;
        
    while(validNum > 0)
    {
        p_queueElem_t = APP_TRP_COMMON_GetElemLeCircQueue(p_leCircQueue_t);
        
        if (p_queueElem_t != NULL)
        {
            if ((p_queueElem_t->dataLeng != 0) && (p_queueElem_t->p_data != NULL))
            {
                status = APP_TRP_COMMON_SendLeDataToUart(p_queueElem_t->dataLeng, p_queueElem_t->p_data);
                if (status == APP_RES_SUCCESS)
                {
                    //trpUartTxCount += p_queueElem_t->dataLeng;
                    //APP_LOG_DEBUG("trpUartTxCount=%ld\n", trpUartTxCount);
                    //APP_LOG_DEBUG("trpUartTx data = %x,%x,%x,%x,%x,%x,%x,%x,%x,%x\n",
                    //    p_queueElem_t->p_data[0], p_queueElem_t->p_data[1], p_queueElem_t->p_data[2],
                    //    p_queueElem_t->p_data[3], p_queueElem_t->p_data[4], p_queueElem_t->p_data[5],
                    //    p_queueElem_t->p_data[6], p_queueElem_t->p_data[7], p_queueElem_t->p_data[8],
                    //    p_queueElem_t->p_data[9]);
                    APP_TRP_COMMON_FreeElemLeCircQueue(p_leCircQueue_t);
                    validNum--;
                }
                else
                {
                    // Set a timer to retransmit it.
                    APP_TRP_COMMON_SetTimer(APP_TRP_TMR_ID_INST_MERGE(APP_TIMER_UART_SEND_03, 0), 
                        (void *)p_connList_t, APP_TIMER_18MS, false, &(p_connList_t->uartTxTmr));

                    return;
                }
            }
            else
            {
                APP_TRP_COMMON_FreeElemLeCircQueue(p_leCircQueue_t);
                validNum--;
            }
        }
        else
        {
            status = APP_TRP_COMMON_GetTrpDataLength(p_connList_t, &dataLeng);
            
            if (status == APP_RES_SUCCESS)
            {
                if (dataLeng)
                {
                    p_data = OSAL_Malloc(dataLeng);
                    
                    if (p_data != NULL)
                    {
                        status = APP_TRP_COMMON_GetTrpData(p_connList_t, p_data);
                        
                        if (status == APP_RES_SUCCESS)
                        {
                            status = APP_TRP_COMMON_SendLeDataToUart(dataLeng, p_data);

                            if (status == APP_RES_SUCCESS)
                            {
                                //trpUartTxCount += p_queueElem_t->dataLeng;
                                //APP_LOG_DEBUG("trpUartTxCount=%ld\n", trpUartTxCount);
                                //APP_LOG_DEBUG("trpUartTx data = %x,%x,%x,%x,%x,%x,%x,%x,%x,%x\n",
                                //    p_queueElem_t->p_data[0], p_queueElem_t->p_data[1], p_queueElem_t->p_data[2],
                                //    p_queueElem_t->p_data[3], p_queueElem_t->p_data[4], p_queueElem_t->p_data[5],
                                //    p_queueElem_t->p_data[6], p_queueElem_t->p_data[7], p_queueElem_t->p_data[8],
                                //    p_queueElem_t->p_data[9]);
                                // APP_UTILITY_FreeElemCircQueue(&(p_connList_t->leCircQueue_t));
                                OSAL_Free(p_data);
                                validNum--;
                            }
                            else
                            {
                                if (status == APP_RES_FAIL)
                                {
                                    if (APP_TRP_COMMON_GetValidLeCircQueueNum(p_leCircQueue_t) > 0)
                                    {
                                        APP_TRP_COMMON_InsertDataToLeCircQueue(dataLeng, p_data, p_leCircQueue_t);
                                        status = APP_TRP_COMMON_GetTrpDataLength(p_connList_t, &dataLeng);
                                        if (status == APP_RES_SUCCESS)
                                        {
                                            if (dataLeng)
                                            {
                                                // Set a timer to retransmit.
                                                APP_TRP_COMMON_SetTimer(APP_TRP_TMR_ID_INST_MERGE(APP_TIMER_UART_SEND_03, 0), 
                                                    (void *)p_connList_t, APP_TIMER_18MS, false, &(p_connList_t->uartTxTmr));
                                            }
                                            else
                                            {
                                                APP_TRP_COMMON_SetTimer(APP_TRP_TMR_ID_INST_MERGE(APP_TIMER_FETCH_TRP_RX_DATA_02, 0), 
                                                    (void *)p_connList_t, APP_TIMER_18MS, false, &(p_connList_t->leRxTmr));
                                            }
                                        }
                                    }
                                    else
                                    {
                                        OSAL_Free(p_data);
                                    }
                                }
                                else
                                {
                                    OSAL_Free(p_data);
                                }
                                return;
                            }
                        }
                        else
                        {
                            OSAL_Free(p_data);
                            validNum--;
                        }
                    }
                    else
                        return;
                }
                else
                    return;
            }
            else
                return;
        }
    }
    status = APP_TRP_COMMON_GetTrpDataLength(p_connList_t, &dataLeng);
    if ((APP_TRP_COMMON_GetValidLeCircQueueNum(p_leCircQueue_t) > 0) || ((status == APP_RES_SUCCESS) 
        && (dataLeng > 0)))
    {
        // Set a timer to retransmit.
        APP_TRP_COMMON_SetTimer(APP_TRP_TMR_ID_INST_MERGE(APP_TIMER_UART_SEND_03, 0), 
            (void *)p_connList_t, APP_TIMER_18MS, false, &(p_connList_t->uartTxTmr));
    }
}

uint16_t APP_TRP_COMMON_InsertUartDataToCircQueue(APP_TRP_ConnList_T *p_connList_t, 
    APP_TRP_GenData_T *p_rxData)
{
    uint16_t status = APP_RES_SUCCESS;

    if (p_connList_t->uartCircQueue_t.usedNum >= g_trpUartQueueNum)
    {
        p_rxData->noNewElemFg = true;
        return APP_RES_NO_RESOURCE;
    }
    status = APP_UTILITY_InsertDataToCircQueue(p_rxData->srcOffset, p_rxData->p_srcData,
        &(p_connList_t->uartCircQueue_t));
    if (status == APP_RES_NO_RESOURCE)
    {
        p_rxData->noNewElemFg = true;
    }
    else
    {
        if ((status == APP_RES_INVALID_PARA) && (p_rxData->p_srcData != NULL))
            OSAL_Free(p_rxData->p_srcData);
        p_rxData->p_srcData = NULL;
        p_rxData->srcOffset = 0;
    }

    return status;
}

uint16_t APP_TRP_COMMON_CopyUartRxData(APP_TRP_ConnList_T *p_connList_t, APP_TRP_GenData_T *p_rxData)
{ 
    uint16_t status = APP_RES_SUCCESS, readLeng, dataLeng;
    uint8_t insertDataFlag;

    if ((p_rxData->srcOffset + p_rxData->rxLeng) >= p_connList_t->lePktLeng)
    {
        insertDataFlag = true;
        dataLeng = p_connList_t->lePktLeng - p_rxData->srcOffset;
    }
    else
    {
        insertDataFlag = false;
        dataLeng = p_rxData->rxLeng;
    }
    readLeng = DRV_UART_ReadRxBuf(SERCOM_UART0, p_rxData->p_srcData + p_rxData->srcOffset, dataLeng);
    p_rxData->rxLeng -= readLeng;
    p_rxData->srcOffset += readLeng;
    // trpUartRxCount += readLeng;
    // APP_LOG_DEBUG("trpUartRxCount=%ld \n", trpUartRxCount);
    if (insertDataFlag)
    {
        status = APP_TRP_COMMON_InsertUartDataToCircQueue(p_connList_t, p_rxData);
        // if (status == APP_RES_SUCCESS)
        // {
        //     APP_LOG_DEBUG("UartRxData:%x,%x,%x,%x,%x,%x,%x,%x,%x,%x \n", p_rxData->p_srcData[0],
        //            p_rxData->p_srcData[1], p_rxData->p_srcData[2], p_rxData->p_srcData[3], 
        //            p_rxData->p_srcData[4], p_rxData->p_srcData[5], p_rxData->p_srcData[6],
        //            p_rxData->p_srcData[7], p_rxData->p_srcData[8], p_rxData->p_srcData[9]);
        // }
    }

    return status;
}

uint16_t APP_TRP_COMMON_SendLeDataUartCircQueue(APP_TRP_ConnList_T *p_connList_t)
{
    APP_UTILITY_CircQueue_T *p_circQueue_t;
    APP_UTILITY_QueueElem_T *p_queueElem_t;
    uint16_t status = APP_RES_SUCCESS;
    uint8_t validNum = APP_TRP_MAX_TRANSMIT_NUM;

    p_circQueue_t = &(p_connList_t->uartCircQueue_t);
    
    p_queueElem_t = APP_UTILITY_GetElemCircQueue(p_circQueue_t);
    while (p_queueElem_t != NULL)
    {
        status = APP_TRP_COMMON_SendLeData(p_connList_t, p_queueElem_t->dataLeng, 
            p_queueElem_t->p_data);

        if (status == APP_RES_SUCCESS)
        {
            validNum--;
            //trpLeTxCount += p_queueElem_t->dataLeng;
            //APP_LOG_DEBUG("trpLeTxCount = %ld\n", trpLeTxCount);
            // APP_LOG_DEBUG("trpLeTx data = %x,%x,%x,%x,%x,%x,%x,%x,%x,%x \n", 
            //            p_queueElem_t->p_data[0], p_queueElem_t->p_data[1], p_queueElem_t->p_data[2],
            //            p_queueElem_t->p_data[3], p_queueElem_t->p_data[4], p_queueElem_t->p_data[5],
            //            p_queueElem_t->p_data[6], p_queueElem_t->p_data[7], p_queueElem_t->p_data[8],
            //            p_queueElem_t->p_data[9]);
            APP_UTILITY_FreeElemCircQueue(p_circQueue_t);
            if (validNum > 0)   // limit transmit number.
                p_queueElem_t = APP_UTILITY_GetElemCircQueue(p_circQueue_t);
            else
            {
                status = APP_RES_SUCCESS;
                break;
            }
        }
        else
        {
            if ((status == APP_RES_NO_RESOURCE) || (status == APP_RES_OOM))
            {
                APP_LOG_ERROR("LE Tx error: status=%d\n", status);
                break;
            }
            else
            {
                APP_UTILITY_FreeElemCircQueue(p_circQueue_t);
                validNum--;
                if (validNum > 0)   // limit transmit number.
                    p_queueElem_t = APP_UTILITY_GetElemCircQueue(p_circQueue_t);
                else
                {
                    status = APP_RES_SUCCESS;
                    break;
                }
            }
        }
    }
            
    return status;
}

#ifdef APP_BLE_MULTI_LINK_ENABLE
uint16_t APP_TRP_COMMON_SendMultiLinkLeDataTrpProfile(APP_TRP_TrafficPriority_T *p_connToken, APP_TRP_ConnList_T *p_connList_t)
{
    APP_TRP_LeCircQueue_T *p_leCircQueue_t;
    APP_UTILITY_QueueElem_T *p_queueElem_t;
    uint16_t status = APP_RES_SUCCESS, dataLeng = 0;
    uint8_t *p_data = NULL;


    if ((p_connToken == NULL) || (p_connList_t == NULL))
        return APP_RES_INVALID_PARA;
    
    p_leCircQueue_t = &(p_connList_t->leCircQueue_t);
    p_queueElem_t = APP_TRP_COMMON_GetElemLeCircQueue(p_leCircQueue_t);
    
    if (p_queueElem_t != NULL)
    {
        status = APP_TRP_COMMON_SendLeData(p_connList_t, p_queueElem_t->dataLeng, p_queueElem_t->p_data);

        if (status == APP_RES_SUCCESS)
        {
            //trpLeTxCount += p_queueElem_t->dataLeng;
            //APP_LOG_DEBUG("trpLeTxCount = %ld\n", trpLeTxCount);
            //APP_LOG_DEBUG("trpLeTx data = %x,%x,%x,%x,%x,%x,%x,%x,%x,%x\n", 
            //            p_queueElem_t->p_data[0], p_queueElem_t->p_data[1], p_queueElem_t->p_data[2],
            //            p_queueElem_t->p_data[3], p_queueElem_t->p_data[4], p_queueElem_t->p_data[5],
            //            p_queueElem_t->p_data[6], p_queueElem_t->p_data[7], p_queueElem_t->p_data[8],
            //            p_queueElem_t->p_data[9]);
            
            p_connToken->validNumber--;
            p_connList_t->maxAvailTxNumber--;
            APP_TRP_COMMON_FreeElemLeCircQueue(p_leCircQueue_t);
        }
        else
        {
            if ((status == APP_RES_NO_RESOURCE) || (status == APP_RES_OOM))
            {
                APP_LOG_ERROR("LE Tx error: status=0x%x\n", status);
            }
            else
            {
                p_connToken->validNumber--;
                p_connList_t->maxAvailTxNumber--;
                APP_TRP_COMMON_FreeElemLeCircQueue(p_leCircQueue_t);
            }
        }
    }
    else
    {
        status = APP_TRP_COMMON_GetTrpDataLength(p_connList_t, &dataLeng);
        
        if (dataLeng)
        {
            p_data = OSAL_Malloc(dataLeng);
            
            if (p_data != NULL)
            {
                status = APP_TRP_COMMON_GetTrpData(p_connList_t, p_data);
                
                if (status == APP_RES_SUCCESS)
                {
                    status = APP_TRP_COMMON_SendLeData(p_connList_t, dataLeng, p_data);

                    if (status == APP_RES_SUCCESS)
                    {
                        //trpLeTxCount += p_queueElem_t->dataLeng;
                        //APP_LOG_DEBUG("trpLeTxCount = %ld\n", trpLeTxCount);
                        //APP_LOG_DEBUG("trpLeTx data = %x,%x,%x,%x,%x,%x,%x,%x,%x,%x \n", 
                        //p_data[0], p_data[1], p_data[2], p_data[3], p_data[4], p_data[5], 
                        //p_data[6], p_data[7], p_data[8], p_data[9]);
                        
                        p_connToken->validNumber--;
                        p_connList_t->maxAvailTxNumber--;
                        OSAL_Free(p_data);
                    }
                    else
                    {            
                        if ((status == APP_RES_NO_RESOURCE) || (status == APP_RES_OOM))
                        { 
                            if (APP_TRP_COMMON_GetValidLeCircQueueNum(p_leCircQueue_t) > 0)
                            {
                                APP_TRP_COMMON_InsertDataToLeCircQueue(dataLeng, p_data, p_leCircQueue_t);
                            }
                            else
                            {
                                p_connToken->validNumber--;
                                p_connList_t->maxAvailTxNumber--;
                                OSAL_Free(p_data);
                            }
                        }
                        else
                        {
                            p_connToken->validNumber--;
                            p_connList_t->maxAvailTxNumber--;
                            OSAL_Free(p_data);
                        }
                        APP_LOG_ERROR("LE Tx error: status=0x%x\n", status);
                    }
                }
                else
                {
                    //can't find current link or there is no data in the queue.
                    OSAL_Free(p_data);
                    p_connList_t->maxAvailTxNumber = 0; //change link
                }
            }
            else
            {
                //can't allocate memory.
                return APP_RES_OOM;
            }
        }
        else
        {
            //get data length = 0.
            p_connList_t->maxAvailTxNumber = 0; //change link
        }
    }
    
    return status;
}
#else
uint16_t APP_TRPC_SendLeDataTrpProfile(APP_TRP_ConnList_T *p_connList_t)
{
    APP_TRP_LeCircQueue_T *p_leCircQueue_t;
    APP_UTILITY_QueueElem_T *p_queueElem_t;
    uint16_t status = APP_RES_SUCCESS, dataLeng = 0;
    uint8_t *p_data = NULL;


    if (p_connList_t == NULL)
        return APP_RES_INVALID_PARA;

    p_connList_t->maxAvailTxNumber = APP_TRP_MAX_TX_AVAILABLE_TIMES;
    p_leCircQueue_t = &(p_connList_t->leCircQueue_t);
    
    do
    {
        p_queueElem_t = APP_TRP_COMMON_GetElemLeCircQueue(p_leCircQueue_t);
        
        if (p_queueElem_t != NULL)
        {
            status = APP_TRP_COMMON_SendLeData(p_connList_t, p_queueElem_t->dataLeng, p_queueElem_t->p_data);

            if (status == APP_RES_SUCCESS)
            {
                p_connList_t->maxAvailTxNumber--;
                APP_TRP_COMMON_FreeElemLeCircQueue(p_leCircQueue_t);
            }
            else
            {
                if ((status == APP_RES_NO_RESOURCE) || (status == APP_RES_OOM))
                {
                    APP_LOG_ERROR("LE Tx error: status=0x%x\n", status);
                    return status;
                }
                else
                {
                    p_connList_t->maxAvailTxNumber--;
                    APP_TRP_COMMON_FreeElemLeCircQueue(p_leCircQueue_t);
                }
            }
        }
        else
        {
            status = APP_TRP_COMMON_GetTrpDataLength(p_connList_t, &dataLeng);
            
            if (dataLeng)
            {
                p_data = OSAL_Malloc(dataLeng);
                
                if (p_data != NULL)
                {
                    status = APP_TRP_COMMON_GetTrpData(p_connList_t, p_data);
                    
                    if (status == APP_RES_SUCCESS)
                    {
                        status = APP_TRP_COMMON_SendLeData(p_connList_t, dataLeng, p_data);

                        if (status == APP_RES_SUCCESS)
                        {
                            p_connList_t->maxAvailTxNumber--;
                            OSAL_Free(p_data);
                        }
                        else
                        {            
                            if ((status == APP_RES_NO_RESOURCE) || (status == APP_RES_OOM))
                            { 
                                if (APP_TRP_COMMON_GetValidLeCircQueueNum(p_leCircQueue_t) > 0)
                                {
                                    APP_TRP_COMMON_InsertDataToLeCircQueue(dataLeng, p_data, p_leCircQueue_t);
                                    p_connList_t->maxAvailTxNumber = 0;
                                }
                                else
                                {
                                    p_connList_t->maxAvailTxNumber--;
                                    OSAL_Free(p_data);
                                }
                            }
                            else
                            {
                                p_connList_t->maxAvailTxNumber--;
                                OSAL_Free(p_data);
                            }
                            APP_LOG_ERROR("LE Tx error: status=0x%x\n", status);
                        }
                    }
                    else
                    {
                        //can't get data or can't find the current link.
                        OSAL_Free(p_data);
                        p_connList_t->maxAvailTxNumber = 0;
                    }
                }
                else
                {
                    //can't allocate memory.
                    return APP_RES_OOM;
                }
            }
            else
            {
                //get data length = 0.
                p_connList_t->maxAvailTxNumber = 0;
            }
        }
    }
    while(p_connList_t->maxAvailTxNumber > 0);
    
    return status;
}
#endif  //#ifdef APP_BLE_MULTI_LINK_ENABLE

uint16_t APP_TRP_COMMON_UartRxData(APP_TRP_GenData_T *p_rxData, APP_TRP_ConnList_T *p_connList_t)
{
    uint16_t status = APP_RES_SUCCESS;
    uint8_t validNum = APP_TRP_MAX_TRANSMIT_NUM;

    // Send data to transparent profile
    if (p_connList_t->uartCircQueue_t.usedNum > 0)
    {
        APP_TRP_COMMON_SendLeDataUartCircQueue(p_connList_t);
    }
    
    if (p_rxData->noNewElemFg == true)
    {
        status = APP_TRP_COMMON_InsertUartDataToCircQueue(p_connList_t, p_rxData);
        if (status == APP_RES_NO_RESOURCE)
        {
            return status;
        }
        p_rxData->noNewElemFg = false;
    }
    
    if (p_connList_t->lePktLeng == 0)
    {
        if (p_connList_t->txMTU > 0)
            p_connList_t->lePktLeng = p_connList_t->txMTU;
        else
            p_connList_t->lePktLeng = BLE_ATT_MAX_MTU_LEN - ATT_HANDLE_VALUE_HEADER_SIZE;
    }
    
    while (p_rxData->rxLeng > 0)
    {
        if (p_rxData->p_srcData != NULL)
        {
            status = APP_TRP_COMMON_CopyUartRxData(p_connList_t, p_rxData);
            if (status != APP_RES_SUCCESS)
            {
                break;
            }
        }
        else
        {
            validNum--;
            if (validNum > 0)   // Limit transmission number
            {
                // Get Rx buffer.
                p_rxData->p_srcData = OSAL_Malloc(p_connList_t->lePktLeng);
                if (p_rxData->p_srcData == NULL)
                {
                    status = APP_RES_OOM;
                    break;
                } 
                p_rxData->srcOffset = 0;
                status = APP_TRP_COMMON_CopyUartRxData(p_connList_t, p_rxData);
                if (status != APP_RES_SUCCESS)
                {
                    break;
                }
            }
            else
            {
                 status = APP_RES_SUCCESS;
                 break;
            }
        }
    }
    
    // Send data to transparent profile
    if (p_connList_t->uartCircQueue_t.usedNum > 0)
    {
        APP_TRP_COMMON_SendLeDataUartCircQueue(p_connList_t);
    }

    return status;
}

static APP_TRP_ConnList_T *APP_TRP_COMMON_GetConnListByUartMode(bool trpcUartMode, bool trpsUartMode)
{
    APP_TRP_ConnList_T *p_connList_t = NULL;

    if (trpsUartMode)
    {
        p_connList_t = APP_TRPS_GetConnListByUartMode();
        if (p_connList_t == NULL)
        {
            APP_LOG_ERROR("TRPS UART: No connection list !\n");

            return NULL;
        }
    }
    #ifdef APP_TRPC_ENABLE
    else if (trpcUartMode)
    {
        p_connList_t = APP_TRPC_GetConnListByUartMode();
        if (p_connList_t == NULL)
        {
            APP_LOG_ERROR("TRPC UART: No connection list !\n");

            return NULL;
        }
    }
    #endif

    return p_connList_t;
}

void APP_TRP_COMMON_UartFetchData(bool trpcUartMode, bool trpsUartMode, uint16_t dataLeng)
{
    APP_TRP_ConnList_T *p_connList_t = NULL;
    uint16_t status;
    
    p_connList_t = APP_TRP_COMMON_GetConnListByUartMode(trpcUartMode, trpsUartMode);
    
    if (p_connList_t == NULL)
        return;
    
    s_trpRxUartData_t.rxLeng = dataLeng;
    p_connList_t->noUartRxCnt = 0;
    status = APP_TRP_COMMON_UartRxData(&s_trpRxUartData_t, p_connList_t);
    
    if (status != APP_RES_SUCCESS)
    {
        APP_LOG_ERROR("UART Rx error! status = %x. \n", status);
    }
}

static void APP_TRP_COMMON_UartRxEnd(APP_TRP_GenData_T *p_rxData, APP_TRP_ConnList_T *p_connList_t)
{
    uint16_t status = APP_RES_SUCCESS;

    if (p_rxData->p_srcData != NULL)
    {
        status = APP_UTILITY_InsertDataToCircQueue(p_rxData->srcOffset, p_rxData->p_srcData, &(p_connList_t->uartCircQueue_t));
        
        if (status == APP_RES_SUCCESS)
        {
            p_rxData->srcOffset = 0;
            p_rxData->p_srcData = NULL;
            status = APP_TRP_COMMON_SendLeDataUartCircQueue(p_connList_t);
        }
    }
    else
    {
        status = APP_TRP_COMMON_SendLeDataUartCircQueue(p_connList_t);
    }
}

void APP_TRP_COMMON_CheckUartRxEnd(bool trpcUartMode, bool trpsUartMode)
{
    APP_TRP_ConnList_T *p_connList_t = NULL;

    p_connList_t = APP_TRP_COMMON_GetConnListByUartMode(trpcUartMode, trpsUartMode);
    
    if (p_connList_t == NULL)
        return;
    
    (p_connList_t->noUartRxCnt)++;

#ifdef APP_DISABLE_UART_POLL
    if ((p_connList_t->noUartRxCnt > 3) || (APP_EIC_GetWakeUpPinStatus()))
#else
    if (p_connList_t->noUartRxCnt > 3)
#endif
    {
        #ifdef APP_DISABLE_UART_POLL
        if ((APP_EIC_GetWakeUpPinStatus()))
        {
            //Stop timer
            APP_EIC_StopUartRxTimer();
        }
        #endif

        p_connList_t->noUartRxCnt = 0;
        APP_TRP_COMMON_UartRxEnd(&s_trpRxUartData_t, p_connList_t);
    }
}


