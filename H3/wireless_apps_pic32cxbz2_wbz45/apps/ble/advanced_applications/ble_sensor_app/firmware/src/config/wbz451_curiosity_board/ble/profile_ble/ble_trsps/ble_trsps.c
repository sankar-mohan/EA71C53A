/*******************************************************************************
  BLE Transparent Server Profile Source File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_trsps.c

  Summary:
    This file contains the BLE Transparent Server functions for application user.

  Description:
    This file contains the BLE Transparent Server functions for application user.
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
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "osal/osal_freertos.h"
#include "mba_error_defs.h"
#include "ble_gap.h"
#include "gatt.h"
#include "ble_util/mw_assert.h"
#include "ble_util/byte_stream.h"
#include "ble_trs/ble_trs.h"
#include "ble_trsps/ble_trsps.h"


// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************

/**@defgroup BLE_TRSPS_INIT_CREDIT BLE_TRSPS_INIT_CREDIT
 * @brief The definition of initial credit value.
 * @{ */
#define BLE_TRSPS_INIT_CREDIT                   0x10    /**< Definition of initial credit */
/** @} */

/**@defgroup BLE_TRSPS_MAX_BUF BLE_TRSPS_MAX_BUF
 * @brief The definition of maximum buffer list.
 * @{ */
#define BLE_TRSPS_MAX_BUF_IN                    (BLE_TRSPS_INIT_CREDIT*BLE_TRSPS_MAX_CONN_NBR)     /**< Maximum incoming queue number */
/** @} */

/**@defgroup BLE_TRSPS_MAX_RETURN_CREDIT BLE_TRSPS_MAX_RETURN_CREDIT
 * @brief The definition of maximum return credit number.
 * @{ */
#define BLE_TRSPS_MAX_RETURN_CREDIT              (13)   /**< Maximum return credit number */
/** @} */

/**@defgroup BLE_TRSPS_CBFC BLE_TRSPS_CBFC
 * @brief The definition of credit base flow control.
 * @{ */
#define BLE_TRSPS_CBFC_TX_ENABLED               1              /**< Definition of ble transparent service credit based transmit enable. */
#define BLE_TRSPS_CBFC_RX_ENABLED               (1<<1)         /**< Definition of ble transparent service credit based receive enable. */
/** @} */

/**@defgroup BLE_TRSPS_CCCD BLE_TRSPS_CCCD
 * @brief The definition of Client Characteristic Configuration Descriptor
 * @{ */
#define BLE_TRSPS_CCCD_DISABLE                  0x0000         /**< Definition of Client Characteristic Configuration Descriptor disable. */
#define BLE_TRSPS_CCCD_NOTIFY                   NOTIFICATION   /**< Definition of Client Characteristic Configuration Descriptor enable NOTIFY property. */
/** @} */


/**@defgroup BLE_TRSPS_CBFC_OPCODE BLE_TRSPS_CBFC_OPCODE
 * @brief The definition of BLE transparent credit based flow control
 * @{ */
#define BLE_TRSPS_CBFC_OPCODE_SERVER_ENABLED    0x14    /**< Definition of Op Code for Credit Based Flow Control Protocol, sending by Server. */
#define BLE_TRSPS_CBFC_OPCODE_GIVE_CREDIT       0x15    /**< Definition of Op Code for Credit Based Flow Control Protocol, giving credit. */
#define BLE_TRSPS_CBFC_OPCODE_SUCCESS           0       /**< Definition of response for successful operation. */
/** @} */

/**@defgroup BLE_TRSPS_VENDOR_OPCODE BLE_TRSPS_VENDOR_OPCODE
 * @brief The definition of BLE transparent vendor opcodes
 * @{ */
#define BLE_TRSPS_VENDOR_OPCODE_MIN             0x20    /**< Definition of Op Code range in TRS vendor commands. */
#define BLE_TRSPS_VENDOR_OPCODE_MAX             0xFF    /**< Definition of Op Code range in TRS vendor commands. */
/** @} */

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

/**@brief The structure contains information about BLE transparent profile packetIn. */
typedef struct BLE_TRSPS_PacketList_T
{
    uint8_t                    writeType;               /**< Write Type. @ref BLE_GATT_WRITE_TYPES*/
    uint16_t                   length;                  /**< Data length. */
    uint8_t                    *p_packet;               /**< Pointer to the TX/RX data buffer */
} BLE_TRSPS_PacketList_T;

/**@brief The structure contains information about packet input queue format of BLE transparent profile. */
typedef struct BLE_TRSPS_QueueIn_T
{
    uint8_t                    usedNum;                    /**< The number of data list of packetIn buffer. */
    uint8_t                    writeIndex;                 /**< The Index of data, written in packet buffer. */
    uint8_t                    readIndex;                  /**< The Index of data, read in packet buffer. */
    BLE_TRSPS_PacketList_T     packetList[BLE_TRSPS_INIT_CREDIT];  /**< Written in packet buffer. @ref BLE_TRSPS_PacketBufferIn_T.*/
} BLE_TRSPS_QueueIn_T;

/**@brief The structure contains information about BLE transparent profile connection parameters for recording connection information. */
typedef struct BLE_TRSPS_ConnList_T
{
    uint8_t                    trsState;                /**< BLE transparent service current state. @ref BLE_TRSPS_STATUS.*/
    uint16_t                   connHandle;              /**< Connection handle associated with this connection. */
    uint16_t                   attMtu;                  /**< Record the current connection MTU size. */
    uint8_t                    cbfcEnable;              /**< Credit based flow enable. @ref BLE_TRSPS_CREDIT_BASED_FLOW_CONTROL. */
    uint8_t                    peerCredit;              /**< Credit number from Central to Peripheral. */
    uint8_t                    localCredit;             /**< Credit number from Peripheral to Central. */
    BLE_TRSPS_QueueIn_T        inputQueue;              /**< Input queue to store Rx packets. */
} BLE_TRSPS_ConnList_T;

/**@brief The structure contains information about the parameters of BLE transparent profile. */
typedef struct BLE_TRSPS_Params_T
{
    uint8_t                    *p_sentDataOffset;
    uint16_t                   sentDataLen;
} BLE_TRSPS_Params_T;

// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************

static BleTrspsProcessRoutine   bleTrspsProcess;
static BLE_TRSPS_ConnList_T     s_trsConnList[BLE_TRSPS_MAX_CONN_NBR];
static BLE_TRSPS_Params_T       s_trsParams;
static GATTS_SendWriteRespParams_T *sp_trsRespParams;
static GATTS_SendErrRespParams_T *sp_trsErrParams;
static uint16_t trsRespErrConnHandle;

MW_ASSERT((BLE_TRSPS_MAX_CONN_NBR*BLE_TRSPS_INIT_CREDIT)==BLE_TRSPS_MAX_BUF_IN);

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************

static void ble_trsps_InitConnList(BLE_TRSPS_ConnList_T *p_conn)
{
    memset((uint8_t *)p_conn, 0, sizeof(BLE_TRSPS_ConnList_T));
    p_conn->attMtu= BLE_ATT_DEFAULT_MTU_LEN;
}

static BLE_TRSPS_ConnList_T * ble_trsps_GetConnListByHandle(uint16_t connHandle)
{
    uint8_t i;

    for(i=0; i<BLE_TRSPS_MAX_CONN_NBR;i++)
    {
        if (s_trsConnList[i].connHandle == connHandle)
        {
            return &s_trsConnList[i];
        }
    }

    return NULL;
}

static BLE_TRSPS_ConnList_T *ble_trsps_GetFreeConnList()
{
    uint8_t i;

    for(i=0; i<BLE_TRSPS_MAX_CONN_NBR;i++)
    {
        if (s_trsConnList[i].connHandle == 0)
        {
            return &s_trsConnList[i];
        }
    }

    return NULL;
}

static uint16_t ble_trsps_ServerReturnCredit(BLE_TRSPS_ConnList_T *p_conn)
{
    GATTS_HandleValueParams_T hvParams;
    uint8_t *p_buf;

    if (p_conn->peerCredit == 0)
        return MBA_RES_SUCCESS;

    hvParams.sendType = ATT_HANDLE_VALUE_NTF;
    hvParams.charHandle = TRS_HDL_CHARVAL_CTRL;
    hvParams.charLength = 0x05;

    p_buf=hvParams.charValue;
    U8_TO_STREAM(&p_buf, BLE_TRSPS_CBFC_OPCODE_SUCCESS);
    U8_TO_STREAM(&p_buf, BLE_TRSPS_CBFC_OPCODE_SERVER_ENABLED);
    U16_TO_STREAM_BE(&p_buf, p_conn->attMtu)
    U8_TO_STREAM(&p_buf, p_conn->peerCredit)

    if (GATTS_SendHandleValue(p_conn->connHandle, &hvParams) == MBA_RES_SUCCESS)
    {
        p_conn ->peerCredit = 0;
        return MBA_RES_SUCCESS;
    }
    else
    {
        return MBA_RES_FAIL;
    }
}

static void ble_trsps_RcvData(BLE_TRSPS_ConnList_T *p_conn, uint8_t writeType, uint16_t receivedLen, uint8_t *p_receivedValue)
{
    if (p_conn->inputQueue.usedNum < BLE_TRSPS_INIT_CREDIT)
    {
        BLE_TRSPS_Event_T evtPara;
        uint8_t *p_buffer = NULL;


        memset((uint8_t *) &evtPara, 0, sizeof(evtPara));
        p_buffer = OSAL_Malloc(receivedLen);
        
        if (p_buffer == NULL)
        {
            return;
        }

        memcpy(p_buffer, p_receivedValue, receivedLen);
        p_conn->inputQueue.packetList[p_conn->inputQueue.writeIndex].writeType = writeType;
        p_conn->inputQueue.packetList[p_conn->inputQueue.writeIndex].length = receivedLen;
        p_conn->inputQueue.packetList[p_conn->inputQueue.writeIndex].p_packet = p_buffer;
        p_conn->inputQueue.writeIndex++;
        if (p_conn->inputQueue.writeIndex >= BLE_TRSPS_INIT_CREDIT)
            p_conn->inputQueue.writeIndex = 0;

        p_conn->inputQueue.usedNum++;

        evtPara.eventId=BLE_TRSPS_EVT_RECEIVE_DATA;
        evtPara.eventField.onReceiveData.connHandle = p_conn->connHandle;
        if (bleTrspsProcess)
        {
            bleTrspsProcess(&evtPara);
        }
    }

}

static bool ble_trsps_CheckQueuedTask(void)
{
    uint8_t i;

    if (sp_trsRespParams != NULL)
    {
        return true;
    }
    if (sp_trsErrParams != NULL)
    {
        return true;
    }
    
    for(i=0; i < BLE_TRSPS_MAX_CONN_NBR; i++)
    {
        if ((s_trsConnList[i].connHandle != 0) && (s_trsConnList[i].peerCredit >= BLE_TRSPS_MAX_RETURN_CREDIT))
        {
            return true;
        }
    }

    return false;
}

static void ble_trsps_ProcessQueuedTask(void)
{
    uint8_t i;
    uint16_t status;
    
    if (sp_trsRespParams != NULL)
    {
        status = GATTS_SendWriteResponse(trsRespErrConnHandle, sp_trsRespParams);
        if (status == MBA_RES_SUCCESS)
        {
            OSAL_Free(sp_trsRespParams);
            sp_trsRespParams = NULL;
            trsRespErrConnHandle = 0;
        }
    }
    if (sp_trsErrParams != NULL)
    {
        status = GATTS_SendErrorResponse(trsRespErrConnHandle, sp_trsErrParams);
        if (status == MBA_RES_SUCCESS)
        {
            OSAL_Free(sp_trsErrParams);
            sp_trsErrParams = NULL;
            trsRespErrConnHandle = 0;
        }
    }
    
    for(i=0; i<BLE_TRSPS_MAX_CONN_NBR;i++)
    {
        if (s_trsConnList[i].connHandle != 0)
        {
            if (s_trsConnList[i].peerCredit >= BLE_TRSPS_MAX_RETURN_CREDIT)
                ble_trsps_ServerReturnCredit(&s_trsConnList[i]);
        }
    }
}

void BLE_TRSPS_EventRegistration(BleTrspsProcessRoutine bleTranServHandler)
{
    bleTrspsProcess = bleTranServHandler;
}

uint16_t BLE_TRSPS_Init()
{
    uint8_t i;

    memset((uint8_t *)&s_trsParams, 0, sizeof(BLE_TRSPS_Params_T));
    sp_trsRespParams = NULL;
    sp_trsErrParams = NULL;
    trsRespErrConnHandle = 0;
    for (i = 0; i < BLE_TRSPS_MAX_CONN_NBR; i++)
    {
        ble_trsps_InitConnList(&s_trsConnList[i]);
    }

    return BLE_TRS_Add();
}

uint16_t BLE_TRSPS_SendVendorCommand(uint16_t connHandle, uint8_t commandID, uint8_t commandLength, uint8_t *p_commandPayload)
{
    BLE_TRSPS_ConnList_T *p_conn;
    GATTS_HandleValueParams_T *p_hvParams;
    uint16_t result;

    p_conn = ble_trsps_GetConnListByHandle(connHandle);
    if (p_conn == NULL)
        return MBA_RES_FAIL;

    if (commandID < BLE_TRSPS_VENDOR_OPCODE_MIN)
    {
        return MBA_RES_INVALID_PARA;
    }

    if (commandLength > (p_conn->attMtu-ATT_NOTI_INDI_HEADER_SIZE-1))
    {
        return MBA_RES_INVALID_PARA;
    }

    p_hvParams = OSAL_Malloc(sizeof(GATTS_HandleValueParams_T));
    if (p_hvParams != NULL)
    {
        p_hvParams->charHandle = TRS_HDL_CHARVAL_CTRL;
        p_hvParams->charLength = (commandLength+1);
        p_hvParams->charValue[0] = commandID;
        memcpy(&p_hvParams->charValue[1], p_commandPayload, commandLength);
        p_hvParams->sendType = ATT_HANDLE_VALUE_NTF;
        result = GATTS_SendHandleValue(p_conn->connHandle, p_hvParams);
        OSAL_Free(p_hvParams);
        
        return result;
    }
    else
    {
        return MBA_RES_OOM;
    }
}

uint16_t BLE_TRSPS_SendData(uint16_t connHandle, uint16_t len, uint8_t *p_data)
{
    GATTS_HandleValueParams_T  hvParams;
    BLE_TRSPS_ConnList_T *p_conn;
    uint16_t result;

    p_conn = ble_trsps_GetConnListByHandle(connHandle);
    if (p_conn == NULL)
    {
        return MBA_RES_FAIL;
    }

    if (p_conn->trsState != BLE_TRSPS_STATUS_TX_OPENED)
    {
        return MBA_RES_BAD_STATE;
    }

    if ((p_conn->cbfcEnable&BLE_TRSPS_CBFC_TX_ENABLED) && (p_conn->localCredit == 0))
    {
        return MBA_RES_NO_RESOURCE;
    }

    if (ble_trsps_CheckQueuedTask())
    {
        return MBA_RES_NO_RESOURCE;
    }

    if (len > (p_conn->attMtu - ATT_HANDLE_VALUE_HEADER_SIZE))
    {
        return MBA_RES_FAIL;
    }

    hvParams.charHandle = TRS_HDL_CHARVAL_TX;
    hvParams.charLength = len;
    memcpy(hvParams.charValue, p_data, hvParams.charLength);
    hvParams.sendType = ATT_HANDLE_VALUE_NTF;

    result = GATTS_SendHandleValue(p_conn->connHandle, &hvParams);
    if (result == MBA_RES_SUCCESS)
    {
        if (p_conn->cbfcEnable&BLE_TRSPS_CBFC_TX_ENABLED)
            p_conn->localCredit--;
    }

    return result;
}

void BLE_TRSPS_GetDataLength(uint16_t connHandle, uint16_t *p_dataLength)
{
    BLE_TRSPS_ConnList_T *p_conn = NULL;

    p_conn = ble_trsps_GetConnListByHandle(connHandle);
    if (p_conn)
    {
        if ((p_conn->inputQueue.usedNum) > 0)
        {
            *p_dataLength = p_conn->inputQueue.packetList[p_conn->inputQueue.readIndex].length;
        }
        else
            *p_dataLength = 0;
    }
    else
        *p_dataLength = 0;
}

uint16_t BLE_TRSPS_GetData(uint16_t connHandle, uint8_t *p_data)
{
    BLE_TRSPS_ConnList_T *p_conn = NULL;
    uint8_t writeType = 0;

    p_conn = ble_trsps_GetConnListByHandle(connHandle);
    if (p_conn)
    {
        if ((p_conn->inputQueue.usedNum) > 0)
        {
            if (p_conn->inputQueue.packetList[p_conn->inputQueue.readIndex].p_packet)
            {
                memcpy(p_data, p_conn->inputQueue.packetList[p_conn->inputQueue.readIndex].p_packet, 
                    p_conn->inputQueue.packetList[p_conn->inputQueue.readIndex].length);
                OSAL_Free(p_conn->inputQueue.packetList[p_conn->inputQueue.readIndex].p_packet);
                p_conn->inputQueue.packetList[p_conn->inputQueue.readIndex].p_packet = NULL;
            }
            
            writeType = p_conn->inputQueue.packetList[p_conn->inputQueue.readIndex].writeType;

            p_conn->inputQueue.readIndex++;
            if (p_conn->inputQueue.readIndex >= BLE_TRSPS_INIT_CREDIT)
                p_conn->inputQueue.readIndex = 0;

            p_conn->inputQueue.usedNum --;
            
            if ((p_conn->cbfcEnable&BLE_TRSPS_CBFC_RX_ENABLED)
            && (writeType == ATT_WRITE_CMD))
            {
                p_conn->peerCredit++;
                if (p_conn->peerCredit >= BLE_TRSPS_MAX_RETURN_CREDIT)
                    ble_trsps_ServerReturnCredit(p_conn);
            }

            return MBA_RES_SUCCESS;
        }
        else
            return MBA_RES_FAIL;
    }
    else
        return MBA_RES_FAIL;

}

static uint8_t ble_trsps_TxCccd(BLE_TRSPS_ConnList_T *p_conn, uint8_t *p_value)
{
    uint16_t cccd;
    BLE_TRSPS_Event_T evtPara;

    memset((uint8_t *) &evtPara, 0, sizeof(evtPara));
    BUF_LE_TO_U16(&cccd, p_value);

    if (cccd == BLE_TRSPS_CCCD_NOTIFY)
    {
        p_conn->trsState = BLE_TRSPS_STATUS_TX_OPENED;
    }
    else if(cccd == BLE_TRSPS_CCCD_DISABLE)
    {
        p_conn->trsState = BLE_TRSPS_STATUS_TX_DISABLED;
    }
    else
    {
        return ATT_ERRCODE_APPLICATION_ERROR;
    }

    evtPara.eventId=BLE_TRSPS_EVT_TX_STATUS;
    evtPara.eventField.onTxStatus.connHandle = p_conn->connHandle;
    evtPara.eventField.onTxStatus.status = p_conn->trsState;
    if (bleTrspsProcess)
    {
        bleTrspsProcess(&evtPara);
    }

    return 0;
}

static void ble_trsps_RxValue(BLE_TRSPS_ConnList_T *p_conn, uint8_t writeType, uint16_t length, uint8_t *p_value)
{
    ble_trsps_RcvData(p_conn, writeType, length, p_value);
}

static void ble_trsps_CtrlValue(BLE_TRSPS_ConnList_T *p_conn, uint16_t length, uint8_t *p_value)
{
    BLE_TRSPS_Event_T evtPara;

    memset((uint8_t *) &evtPara, 0, sizeof(evtPara));
    
    switch (p_value[0])
    {
        case BLE_TRSPS_CBFC_OPCODE_SERVER_ENABLED:
        {
            p_conn->cbfcEnable |= BLE_TRSPS_CBFC_RX_ENABLED;
            p_conn->peerCredit = BLE_TRSPS_INIT_CREDIT;
            ble_trsps_ServerReturnCredit(p_conn);
            
            if (bleTrspsProcess)
            {
                evtPara.eventId=BLE_TRSPS_EVT_CBFC_ENABLED;
                evtPara.eventField.onCbfcEnabled.connHandle = p_conn->connHandle;
                bleTrspsProcess(&evtPara);
            }
        }
            break;

        case BLE_TRSPS_CBFC_OPCODE_GIVE_CREDIT:
        {
            p_conn->cbfcEnable |= BLE_TRSPS_CBFC_TX_ENABLED;
            p_conn->localCredit += p_value[1];
            
            if (bleTrspsProcess)
            {
                evtPara.eventId = BLE_TRSPS_EVT_CBFC_CREDIT;
                evtPara.eventField.onCbfcEnabled.connHandle = p_conn->connHandle;
                bleTrspsProcess(&evtPara);
            }
        }
        break;
        
        default:
        {
            if ((p_value[0] >= BLE_TRSPS_VENDOR_OPCODE_MIN) && (p_value[0] <= BLE_TRSPS_VENDOR_OPCODE_MAX)
                && bleTrspsProcess)
            {
                evtPara.eventId = BLE_TRSPS_EVT_VENDOR_CMD;
                evtPara.eventField.onVendorCmd.connHandle = p_conn->connHandle;
                evtPara.eventField.onVendorCmd.length = length;
                evtPara.eventField.onVendorCmd.p_payLoad = p_value;
                
                bleTrspsProcess(&evtPara);
            }
        }
            break;
    }
}

static uint8_t ble_trsps_CtrlCccd(BLE_TRSPS_ConnList_T *p_conn, uint8_t *p_value)
{
    uint16_t cccd;
    BLE_TRSPS_Event_T evtPara;


    memset((uint8_t *) &evtPara, 0, sizeof(evtPara));
    BUF_LE_TO_U16(&cccd, p_value);
    
    if ((cccd != 0) && (cccd != BLE_TRSPS_CCCD_NOTIFY))
    {
        return ATT_ERRCODE_APPLICATION_ERROR;
    }

    if (bleTrspsProcess)
    {
        evtPara.eventId = BLE_TRSPS_EVT_CTRL_STATUS;
        evtPara.eventField.onCtrlStatus.connHandle = p_conn->connHandle;
        if (cccd == BLE_TRSPS_CCCD_NOTIFY)
            evtPara.eventField.onCtrlStatus.status = BLE_TRSPS_STATUS_CTRL_OPENED;
        else
            evtPara.eventField.onCtrlStatus.status = BLE_TRSPS_STATUS_CTRL_DISABLED;
    
        bleTrspsProcess(&evtPara);
    }
    
    return 0;
}

static void ble_trsps_GattsWriteProcess(GATT_Event_T *p_event)
{
    uint8_t error = 0;
    BLE_TRSPS_ConnList_T *p_conn = NULL;
    uint16_t status;

    if ((p_event->eventField.onWrite.attrHandle <= TRS_START_HDL) ||
        (p_event->eventField.onWrite.attrHandle > TRS_END_HDL))
    {
        /* Not BLE Trs characteristic. */
        return;
    }

    p_conn = ble_trsps_GetConnListByHandle(p_event->eventField.onWrite.connHandle);
        
    if (p_conn == NULL)
    {
        error = ATT_ERRCODE_APPLICATION_ERROR;
    }

    if (p_event->eventField.onWrite.writeType == ATT_PREPARE_WRITE_REQ)
    {
        error = ATT_ERRCODE_APPLICATION_ERROR;
    }

    if (!error)
    {
        switch(p_event->eventField.onWrite.attrHandle)
        {
            case TRS_HDL_CCCD_TX:
            {
                error=ble_trsps_TxCccd(p_conn, p_event->eventField.onWrite.writeValue);
            }
            break;

            case TRS_HDL_CHARVAL_RX:
            {
                ble_trsps_RxValue(p_conn, p_event->eventField.onWrite.writeType,
                                  p_event->eventField.onWrite.writeDataLength,
                                  p_event->eventField.onWrite.writeValue);
            }
            break;

            case TRS_HDL_CHARVAL_CTRL:
            {
                ble_trsps_CtrlValue(p_conn, p_event->eventField.onWrite.writeDataLength, 
                    p_event->eventField.onWrite.writeValue);
            }
            break;

            case TRS_HDL_CCCD_CTRL:
            {
                error=ble_trsps_CtrlCccd(p_conn, p_event->eventField.onWrite.writeValue);
            }
            break;
        }
    }

    if ((p_event->eventField.onWrite.writeType == ATT_WRITE_REQ)
    || (p_event->eventField.onWrite.writeType == ATT_PREPARE_WRITE_REQ))
    {
        if (!error)
        {
            sp_trsRespParams = (GATTS_SendWriteRespParams_T *)OSAL_Malloc(sizeof(GATTS_SendWriteRespParams_T));
            if (sp_trsRespParams == NULL)
            {
                return;
            }
            trsRespErrConnHandle = p_event->eventField.onWrite.connHandle;
            sp_trsRespParams->responseType = ATT_WRITE_RSP;
            status = GATTS_SendWriteResponse(p_event->eventField.onWrite.connHandle, sp_trsRespParams);
            if (status == MBA_RES_SUCCESS)
            {
                OSAL_Free(sp_trsRespParams);
                sp_trsRespParams = NULL;
            }
        }
        else
        {
            sp_trsErrParams = (GATTS_SendErrRespParams_T *)OSAL_Malloc(sizeof(GATTS_SendErrRespParams_T));
            if (sp_trsErrParams == NULL)
            {
                return;
            }
            trsRespErrConnHandle = p_event->eventField.onWrite.connHandle;
            sp_trsErrParams->reqOpcode = p_event->eventField.onWrite.writeType;
            sp_trsErrParams->attrHandle = p_event->eventField.onWrite.attrHandle;
            sp_trsErrParams->errorCode = error;
            status = GATTS_SendErrorResponse(p_event->eventField.onWrite.connHandle, sp_trsErrParams);
            if (status == MBA_RES_SUCCESS)
            {
                OSAL_Free(sp_trsErrParams);
                sp_trsErrParams = NULL;
            }
        }
    }
}

void ble_trsps_GattEventProcess(GATT_Event_T *p_event)
{
    BLE_TRSPS_ConnList_T *p_conn = NULL;


    switch (p_event->eventId)
    {
        case ATT_EVT_TIMEOUT:
        {
        }
        break;

        case ATT_EVT_UPDATE_MTU:
        {
            p_conn = ble_trsps_GetConnListByHandle(p_event->eventField.onUpdateMTU.connHandle);
            if (p_conn == NULL)
                break;
            p_conn->attMtu = p_event->eventField.onUpdateMTU.exchangedMTU;
        }
        break;

        case GATTS_EVT_WRITE:
        {
            ble_trsps_GattsWriteProcess(p_event);
        }
        break;

        default:
            break;
    }
}

void ble_trsps_GapEventProcess(BLE_GAP_Event_T *p_event)
{
    switch(p_event->eventId)
    {
        case BLE_GAP_EVT_CONNECTED:
        {
            if ((p_event->eventField.evtConnect.status == GAP_STATUS_SUCCESS))
            {
                BLE_TRSPS_ConnList_T    *p_conn;
                
                p_conn=ble_trsps_GetFreeConnList();
                if(p_conn==NULL)
                    return;

                p_conn->connHandle=p_event->eventField.evtConnect.connHandle;
            }
        }
        break;

        case BLE_GAP_EVT_DISCONNECTED:
        {
            BLE_TRSPS_ConnList_T    *p_conn;

            p_conn=ble_trsps_GetConnListByHandle(p_event->eventField.evtDisconnect.connHandle);

            if (p_conn != NULL)
            {
                // Flush all queued data.
                while (p_conn->inputQueue.usedNum > 0)
                {
                    if (p_conn->inputQueue.packetList[p_conn->inputQueue.readIndex].p_packet)
                    {
                        OSAL_Free(p_conn->inputQueue.packetList[p_conn->inputQueue.readIndex].p_packet);
                        p_conn->inputQueue.packetList[p_conn->inputQueue.readIndex].p_packet = NULL;
                    }

                    p_conn->inputQueue.readIndex++;
                    if (p_conn->inputQueue.readIndex >= BLE_TRSPS_INIT_CREDIT)
                        p_conn->inputQueue.readIndex = 0;

                    p_conn->inputQueue.usedNum --;
                }
                ble_trsps_InitConnList(p_conn);
            }
        }
        break;

        case BLE_GAP_EVT_TX_BUF_AVAILABLE:
        {
            ble_trsps_ProcessQueuedTask();
        }
        break;
       
        default:
        break;
    }
}

void BLE_TRSPS_BleEventHandler(STACK_Event_T *p_stackEvent)
{
    switch (p_stackEvent->groupId)
    {
        case STACK_GRP_BLE_GAP:
        {
            ble_trsps_GapEventProcess((BLE_GAP_Event_T *)p_stackEvent->p_event);
        }
        break;

        case STACK_GRP_GATT:
        {
            ble_trsps_GattEventProcess((GATT_Event_T *)p_stackEvent->p_event);
        }
        break;

        default:
        break;
    }
}

