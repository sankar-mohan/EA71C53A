/*******************************************************************************
  Application UART command/Event Process Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_cmd_evt_proc.c

  Summary:
    This file contains the Application UART command and event process functions for this project.

  Description:
    This file contains the Application UART command and event process functions for this project.
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
#include "osal/osal_freertos_extend.h"
#include "app_uart.h"
#include "ble_util/byte_stream.h"
#include "ble_trsps/ble_trsps.h"
#include "ble_trspc/ble_trspc.h"
#include "app_ble.h"
#include "app_ble_handler.h"
#include "app_cmd_evt_proc.h"
#include "app_trps.h"
#include "app_log.h"
#include "app_error_defs.h"
#include "app_adv.h"
#include "peripheral/rcon/plib_rcon.h"
#include "app_ds_pair.h"
#include "app_trp_common.h"


// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************

/** @brief BLE GAP Events size. */
#define BLE_GAP_EVT_SIZE_CONNECTED                              0x18        /**< See @ref BLE_GAP_EvtConnect_T. */
#define BLE_GAP_EVT_SIZE_DISCONNECTED                           0x03        /**< See @ref BLE_GAP_EvtDisconnect_T. */
#define BLE_GAP_EVT_SIZE_CONN_PARA_UPDATE                       0x0F        /**< See @ref BLE_GAP_EvtConnParamUpdateParams_T. */
#define BLE_GAP_EVT_SIZE_ENCRYPT_STATUS                         0x03        /**< See @ref BLE_GAP_EvtEncryptStatus_T. */
#define BLE_GAP_EVT_SIZE_COMMAND_COMPLETE                       0x08        /**< See @ref BLE_GAP_EvtCommandComplete_T and @ref BLE_GAP_ReadLocalResolvableAddressPara_T. */
#define BLE_GAP_EVT_SIZE_COMMAND_STATUS                         0x02        /**< See @ref BLE_GAP_EvtCommandStatus_T. */
#define BLE_GAP_EVT_SIZE_ADVERTISING_REPORT                     0x29        /**< See @ref BLE_GAP_EvtAdvertisingReport_T. */
#define BLE_GAP_EVT_SIZE_LONG_TERM_KEY_REQUEST                  0x02        /**< See @ref BLE_GAP_EvtEncInfoRequest_T. */
#define BLE_GAP_EVT_SIZE_REMOTE_CONN_PARAM_REQUEST              0x0A        /**< See @ref BLE_GAP_EvtRemoteconnParammsRequest_T. */
#define BLE_GAP_EVT_SIZE_DATA_LENGTH_CHANGE                     0x0A        /**< See @ref BLE_GAP_EvtDataLengthChange_T. */
#define BLE_GAP_EVT_SIZE_EXT_ADVERTISING_REPORT                 0xFE        /**< See @ref BLE_GAP_EvtExtAdvertisingReport_T. */


/** @brief BT L2CAP Events size. */
#define BT_L2CAP_EVT_SIZE_OPEN_CHANNEL_IND                          0x03        /**< See @ref BT_L2CAP_EvtOpenChannelInd_T. */
#define BT_L2CAP_EVT_SIZE_OPEN_CHANNEL_COMPLETE                     0x0E        /**< See @ref BT_L2CAP_EvtOpenChannelComplete_T. */
#define BT_L2CAP_EVT_SIZE_DISCONNECT_IND                            0x01        /**< See @ref BT_L2CAP_EvtDisconnectInd_T. */
#define BT_L2CAP_EVT_SIZE_DISCONNECT_CFM                            0x01        /**< See @ref BT_L2CAP_EvtDisconnectCfm_T. */
#define BT_L2CAP_EVT_SIZE_RCV_DATA_IND                              0x03        /**< See @ref BT_L2CAP_EvtRcvDataInd_T. */
#define BT_L2CAP_EVT_SIZE_TRANSACTION_TIMEOUT                       0x01        /**< See @ref BT_L2CAP_EvtTransactionTimeout_T. */
#define BT_L2CAP_EVT_SIZE_INFORMATION_RSP                           0x04        /**< See @ref BT_L2CAP_EvtInformationRsp_T. */
#define BT_L2CAP_EVT_SIZE_ECHO_RSP                                  0x02        /**< See @ref BT_L2CAP_EvtEchoRsp_T. */

/** @brief BLE L2CAP Events size. */
#define BLE_L2CAP_EVT_SIZE_CPU_REQ                                  0x0A        /**< See @ref BLE_L2CAP_EvtConnParamUpdateReq_T. */
#define BLE_L2CAP_EVT_SIZE_CPU_RSP                                  0x04        /**< See @ref BLE_L2CAP_EvtConnParamUpdateRsp_T. */
#define BLE_L2CAP_EVT_SIZE_OPEN_CHANNEL_IND                         0x0A        /**< See @ref BLE_L2CAP_EvtCbChannelOpenReq_T. */
#define BLE_L2CAP_EVT_SIZE_OPEN_CHANNEL_CFM                         0x09        /**< See @ref BLE_L2CAP_EvtCbConnInd_T. */
#define BLE_L2CAP_EVT_SIZE_RCV_CREDITS                              0x03        /**< See @ref BLE_L2CAP_EvtCbAddCreditsInd_T. */
#define BLE_L2CAP_EVT_SIZE_DISCONNECT_IND                           0x01        /**< See @ref BLE_L2CAP_EvtCbDiscInd_T. */
#define BLE_L2CAP_EVT_SIZE_DISCONNECT_CFM                           0x01        /**< See @ref BLE_L2CAP_EvtCbDisconnectCfm_T. */
#define BLE_L2CAP_EVT_SIZE_RCV_SDU_IND                              0x03        /**< See @ref BLE_L2CAP_EvtCbSduInd_T. */
#define BLE_L2CAP_EVT_SIZE_CMD_REJECT_IND                           0x04        /**< See @ref BLE_L2CAP_EvtCmdRejectRsp_T. */
#define BLE_L2CAP_EVT_SIZE_ECB_CHANNEL_OPEN_REQ                     0x0b
#define BLE_L2CAP_EVT_SIZE_ECB_CHANNEL_OPEN_RSP                     0x0e
#define BLE_L2CAP_EVT_SIZE_ECB_RECONFIG_IND                         0x0A
#define BLE_L2CAP_EVT_SIZE_ECB_RECONFIG_CFM                         0x08

/** @brief BLE L2CAP CoC TRS Events size. */
#define BLE_TRCBPS_EVT_SIZE_CONN_STATUS                             0x04        /**< See @ref BLE_TRCBPS_EvtConnStatus_T. */
#define BLE_TRCBPS_EVT_SIZE_RCV_DATA                                0x01        /**< See @ref BLE_TRCBPS_EvtReceiveData_T. */

/** @brief GATT Events size. */
#define GATT_EVT_SIZE_CONFIRMATION                                  0x04        /**< See @ref GATT_EvtReceiveCfm_T. */
#define GATT_EVT_SIZE_UPDATEMTU                                     0x04        /**< See @ref GATT_EvtUpdateMtu_T. */
#define GATT_EVT_SIZE_ERRORRESP                                     0x06        /**< See @ref GATT_EvtError_T. */
#define GATT_EVT_SIZE_BTCONNECT                                     0x04        /**< See @ref GATT_EvtBtConnectCompleteCfm_T. */
#define GATT_EVT_SIZE_BTDISCONNECT                                  0x02        /**< See @ref GATT_EvtBtDisconnectCfm_T. */
#define GATT_EVT_SIZE_BLE_CHANN_OPEN                                0x07

#define APP_ERR_UNKNOWN_COMMAND                                      0x01    /**< Application Error Code: Unknown Command. */
#define APP_ERR_UNKNOWN_CONNECTION_IDENTIFIER                        0x02    /**< Application Error Code: Unknown Connection Identifier. */
#define APP_ERR_CONNECTION_TIMEOUT                                   0x08    /**< Application Error Code: Connection Timeout. */
#define APP_ERR_CONNECTION_LIMIT_EXCEEDED                            0x09    /**< Application Error Code: Connection Limit Exceeded */
#define APP_ERR_COMMAND_DISALLOWED                                   0x0C    /**< Application Error Code: Command Disallowed. */
#define APP_ERR_CONNECTION_REJECTED_LIMITED_RESOURCES                0x0D    /**< Application Error Code: Connection Rejected due to Limited Resources. */
#define APP_ERR_CONNECTION_REJECTED_SECURITY_REASONS                 0x0E    /**< Application Error Code: Connection Rejected Due To Security Reasons. */
#define APP_ERR_CONNECTION_REJECTED_UNACCEPTABLE_BD_ADDR             0x0F    /**< Application Error Code: Connection Rejected due to Unacceptable BD_ADDR. */
#define APP_ERR_CONNECTION_REJECTED_ACCEPT_TIMEOUT_EXCEEDED          0x10    /**< Application Error Code: Connection Accept Timeout Exceeded. */
#define APP_ERR_INVALID_PARAMETERS                                   0x12    /**< Application Error Code: Invalid Parameters. */
#define APP_ERR_REMOTE_USER_TERMINATED_CONNECTION                    0x13    /**< Application Error Code: Remote User Terminated Connection. */
#define APP_ERR_REMOTE_DEVICE_TERMINATED_CONNECTION_LOW_RESOURCES    0x14    /**< Application Error Code: Remote Device Terminated Connection due to Low Resources. */
#define APP_ERR_REMOTE_DEVICE_TERMINATED_CONNECTION_POWEROFF         0x15    /**< Application Error Code: Remote Device Terminated Connection due to Power Off. */
#define APP_ERR_CONNECTION_TERMINATED_BYLOCALHOST                    0x16    /**< Application Error Code: Connection Terminated By Local Host. */
#define APP_ERR_CONNECTION_REJECTED_NOSUITABLECHANNELFOUND           0x39    /**< Application Error Code: Connection Rejected due to No Suitable Channel Found. */
#define APP_ERR_CONTROLLER_BUSY                                      0x3A    /**< Application Error Code: Controller Busy. */
#define APP_ERR_UNACCEPTABLE_CONNECTION_INTERVAL                     0x3B    /**< Application Error Code: Unacceptable Connection Interval. */
#define APP_ERR_DIRECTED_ADVERTISING_TIMEOUT                         0x3C    /**< Application Error Code: Directed Advertising Timeout. */
#define APP_ERR_CONNECTION_TERMINATED_MIC_FAILURE                    0x3D    /**< Application Error Code: Connection Terminated due to MIC Failure. */
#define APP_ERR_CONNECTION_FAILED_ESTABLISHED                        0x3E    /**< Application Error Code: Connection Failed to be Established. */
#define APP_ERR_INSUFFICIENT_RESOURCES                               0x91    /**< Application Error Code: Insufficient Resources. */

#define BLE_GAP_EVT_COMMAND_COMPLETE                0x04
#define BLE_GAP_CMD_COMPLETE_SET_ADVERTISING_DATA   0x1
#define BLE_GAP_CMD_COMPLETE_SET_ADVERTISING_PARAMS 0x3
#define BLE_GAP_CMD_COMPLETE_SET_ADVERTISING_ENABLE 0x4
#define BLE_GAP_CMD_COMPLETE_CLEAR_WHITE_LIST       0x6
#define BLE_GAP_CMD_COMPLETE_SET_WHITE_LIST         0x7
#define BLE_GAP_CMD_COMPLETE_SET_SCAN_PARAMS        0xD
#define BLE_GAP_CMD_COMPLETE_SET_SCAN_ENABLE        0xE
#define BLE_GAP_CMD_COMPLETE_SET_SCAN_FILTER_ENABLE 0xF

/** @brief PTS test service event id */
#define APP_PTS_EVT_GATTS_WRITE             0x00    /**< Write Requests/Command Event from GATT. See @ref APP_PTS_Evt_GattsWrite_T for event details. */
#define APP_PTS_EVT_GATTS_CONFIRMATION      0x01    /**< Handle Value Confirmation Event from GATT. See @ref APP_PTS_Evt_GattsReceiveCfm_T for event details. */
#define APP_PTS_EVT_GATTS_UPDATEMTU         0x02    /**< MTU Exchange Event from GATT. See @ref APP_PTS_Evt_GattsUpdateMtu_T for event details. */
#define APP_PTS_EVT_GATTS_GETATTRVALUE      0x03    /**< Get Attribute value from GATT Server. See @ref APP_PTS_Evt_GattsGetAttrValue_T for event details. */

#define APP_PTS_EVTSIZE_GATTS_WRITE         10      /**< Event length of APP_PTS_EVT_GATTS_WRITE excluding length of write data. */
#define APP_PTS_EVTSIZE_GATTS_GETATTRVALUE   5      /**< Event length of APP_PTS_EVT_GATTS_GETATTRVALUE excluding the getting data. */

// *****************************************************************************
// *****************************************************************************
// Section: Global Variabless
// *****************************************************************************
// *****************************************************************************



// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************
static uint8_t s_l2capInitiator;


// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
#ifdef APP_PAIRING_ENABLE
static uint16_t APP_ProcEnterTdcMode(void)
{
    uint8_t *p_devIdBuf = NULL;
    uint8_t *p_privacyMode = NULL;
    uint8_t devCnt = 0;
    uint16_t result = APP_RES_SUCCESS;

    p_devIdBuf = OSAL_Malloc(BLE_DM_MAX_PAIRED_DEVICE_NUM);

    if (!p_devIdBuf)
    {
        result = APP_RES_OOM;
        return result;
    }
    memset(p_devIdBuf, 0, BLE_DM_MAX_PAIRED_DEVICE_NUM);

    BLE_DM_GetPairedDeviceList(p_devIdBuf, &devCnt);

    if (devCnt != 0)
    {
        APP_ADV_Stop();

        result = BLE_DM_SetFilterAcceptList(devCnt, p_devIdBuf);

        if (result == MBA_RES_SUCCESS)
        {
            p_privacyMode = OSAL_Malloc(devCnt);

            if (p_privacyMode)
            {
                memset(p_privacyMode, BLE_GAP_PRIVACY_MODE_NETWORK, devCnt);

                result = BLE_DM_SetResolvingList(devCnt, p_devIdBuf, p_privacyMode);

                OSAL_Free(p_privacyMode);
            }
            else
            {
                result = APP_RES_OOM;
            }
        }

        if (result == MBA_RES_SUCCESS)
        {
            APP_ADV_SetFilterPolicy(BLE_GAP_ADV_FILTER_SCAN_CONNECT);
        }

        APP_ADV_Start();
    }
    else
    {
        result = APP_RES_FAIL;
    }

    if (p_devIdBuf)
        OSAL_Free(p_devIdBuf);

    return result;
}
#endif

uint16_t APP_PackageUartEvent(uint16_t tx_len, uint8_t *p_payload)
{
    uint8_t   *p_event, *p_temp;
    uint16_t result = APP_RES_FAIL;

#ifdef UART_DOCKLIGNT_FORMAT
    uint16_t pktLen;

    pktLen = tx_len + 5;

    p_event = OSAL_Malloc(pktLen);
    if (p_event != NULL)
    {
        p_temp = p_event;

        *p_temp++ = UART_START_BYTE;
        *p_temp++ = (uint8_t)(tx_len >> 8);
        *p_temp++ = (uint8_t)tx_len;

        while (tx_len > 255)
        {
            memcpy(p_temp, p_payload, 255);
            p_temp += 255;
            p_payload += 255;
            tx_len -= 255;
        }
        memcpy(p_temp, p_payload, tx_len);
        p_temp += tx_len;

        *p_temp++ = 0xff;
        *p_temp = 0xff;

        result = APP_UartPut(pktLen, p_event);
        OSAL_Free(p_event);
    }
#else
    uint16_t   i;
    uint8_t    check_sum;
    p_event = OSAL_Malloc(tx_len + 4);
    if (p_event != NULL)
    {
        p_temp = p_event;
        check_sum = 0;
        *p_temp++ = UART_START_BYTE;
        *p_temp++ = (uint8_t)(tx_len >> 8);
        check_sum += (uint8_t)(tx_len >> 8);
        *p_temp++ = (uint8_t)tx_len;
        check_sum += (uint8_t)tx_len;

        for (i = 0; i < tx_len; i++)
        {
            *p_temp++ = *(p_payload + i);
            check_sum += *(p_payload + i);
        }

        check_sum = 0xff - check_sum + 1;
        *p_temp = check_sum;

        result = APP_UartPut(tx_len + 4, p_event);
        OSAL_Free(p_event);
    }
#endif
    return result;
}


void APP_ProcBleGapUartEvt(BLE_GAP_Event_T *p_event)
{
    uint8_t *p_buf;
    uint8_t *p_packet;
    uint8_t evt_len;

    p_packet = OSAL_Malloc(sizeof(BLE_GAP_Event_T) + 2);
    p_buf = p_packet;
    memset(p_packet, 0, (sizeof(BLE_GAP_Event_T)+2));

    if (p_packet==NULL)
    {
        return;
    }
    /* eventId */
    evt_len = 1;
    U8_TO_STREAM(&p_buf, UART_EVENT_TEST);
    U8_TO_STREAM(&p_buf, UART_GROUP_BLE_GAP);
    U8_TO_STREAM(&p_buf, p_event->eventId);

    switch (p_event->eventId)
    {
        case BLE_GAP_EVT_CONNECTED:
        {
            U16_TO_STREAM_BE(&p_buf, p_event->eventField.evtConnect.connHandle);
            U8_TO_STREAM(&p_buf, p_event->eventField.evtConnect.role);

            U8_TO_STREAM(&p_buf, p_event->eventField.evtConnect.status);
            U8_TO_STREAM(&p_buf, p_event->eventField.evtConnect.remoteAddr.addrType);
            VARIABLE_COPY_TO_STREAM(&p_buf, p_event->eventField.evtConnect.remoteAddr.addr, GAP_MAX_BD_ADDRESS_LEN);
            U8_TO_STREAM(&p_buf, p_event->eventField.evtConnect.localAddr.addrType);
            VARIABLE_COPY_TO_STREAM(&p_buf, p_event->eventField.evtConnect.localAddr.addr, GAP_MAX_BD_ADDRESS_LEN);
            U16_TO_STREAM_BE(&p_buf, p_event->eventField.evtConnect.interval);
            U16_TO_STREAM_BE(&p_buf, p_event->eventField.evtConnect.latency);
            U16_TO_STREAM_BE(&p_buf, p_event->eventField.evtConnect.supervisionTimeout);
            evt_len += BLE_GAP_EVT_SIZE_CONNECTED;
        }
        break;
        case BLE_GAP_EVT_DISCONNECTED:
        {
            U16_TO_STREAM_BE(&p_buf, p_event->eventField.evtDisconnect.connHandle);
            U8_TO_STREAM(&p_buf, p_event->eventField.evtDisconnect.reason);
            evt_len += BLE_GAP_EVT_SIZE_DISCONNECTED;
        }
        break;
        case BLE_GAP_EVT_CONN_PARAM_UPDATE:
        {
            U16_TO_STREAM_BE(&p_buf, p_event->eventField.evtConnParamUpdate.connHandle);
            U8_TO_STREAM(&p_buf, p_event->eventField.evtConnParamUpdate.status);
            U16_TO_STREAM_BE(&p_buf, p_event->eventField.evtConnParamUpdate.connParam.intervalMin);
            U16_TO_STREAM_BE(&p_buf, p_event->eventField.evtConnParamUpdate.connParam.intervalMax);
            U16_TO_STREAM_BE(&p_buf, p_event->eventField.evtConnParamUpdate.connParam.latency);
            U16_TO_STREAM_BE(&p_buf, p_event->eventField.evtConnParamUpdate.connParam.supervisionTimeout);
            U16_TO_STREAM_BE(&p_buf, 0);
            U16_TO_STREAM_BE(&p_buf, 0);
            U16_TO_STREAM_BE(&p_buf, 0);
            U16_TO_STREAM_BE(&p_buf, 0);
            evt_len += BLE_GAP_EVT_SIZE_CONN_PARA_UPDATE;
        }
        break;
        case BLE_GAP_EVT_ENCRYPT_STATUS:
        {
            U16_TO_STREAM_BE(&p_buf, p_event->eventField.evtEncryptStatus.connHandle);
            U8_TO_STREAM(&p_buf, p_event->eventField.evtEncryptStatus.status);
            evt_len += BLE_GAP_EVT_SIZE_ENCRYPT_STATUS;
        }
        break;

        case BLE_GAP_EVT_ADV_REPORT:
        {
            U8_TO_STREAM(&p_buf, p_event->eventField.evtAdvReport.eventType);
            U8_TO_STREAM(&p_buf, p_event->eventField.evtAdvReport.addr.addrType);
            VARIABLE_COPY_TO_STREAM(&p_buf, p_event->eventField.evtAdvReport.addr.addr, GAP_MAX_BD_ADDRESS_LEN);
            U8_TO_STREAM(&p_buf, p_event->eventField.evtAdvReport.length);
            VARIABLE_COPY_TO_STREAM(&p_buf, p_event->eventField.evtAdvReport.advData, BLE_GAP_ADV_MAX_LENGTH);
            U8_TO_STREAM(&p_buf, p_event->eventField.evtAdvReport.rssi);
            evt_len += BLE_GAP_EVT_SIZE_ADVERTISING_REPORT;
        }
        break;
        case BLE_GAP_EVT_REMOTE_CONN_PARAM_REQUEST:
        {
            U16_TO_STREAM_BE(&p_buf, p_event->eventField.evtRemoteConnParamReq.connHandle);
            U16_TO_STREAM_BE(&p_buf, p_event->eventField.evtRemoteConnParamReq.intervalMin);
            U16_TO_STREAM_BE(&p_buf, p_event->eventField.evtRemoteConnParamReq.intervalMax);
            U16_TO_STREAM_BE(&p_buf, p_event->eventField.evtRemoteConnParamReq.latency);
            U16_TO_STREAM_BE(&p_buf, p_event->eventField.evtRemoteConnParamReq.timeout);
            evt_len += BLE_GAP_EVT_SIZE_REMOTE_CONN_PARAM_REQUEST;
        }
        break;
        case BLE_GAP_EVT_EXT_ADV_REPORT:
        {
            U8_TO_STREAM(&p_buf, p_event->eventField.evtExtAdvReport.eventType);
            U8_TO_STREAM(&p_buf, p_event->eventField.evtExtAdvReport.dataStatus);
            U8_TO_STREAM(&p_buf, p_event->eventField.evtExtAdvReport.addr.addrType);
            VARIABLE_COPY_TO_STREAM(&p_buf, p_event->eventField.evtExtAdvReport.addr.addr, GAP_MAX_BD_ADDRESS_LEN);
            U8_TO_STREAM(&p_buf, p_event->eventField.evtExtAdvReport.priPhy);
            U8_TO_STREAM(&p_buf, p_event->eventField.evtExtAdvReport.secPhy);
            U8_TO_STREAM(&p_buf, p_event->eventField.evtExtAdvReport.sid);
            U8_TO_STREAM(&p_buf, p_event->eventField.evtExtAdvReport.txPower);
            U8_TO_STREAM(&p_buf, p_event->eventField.evtExtAdvReport.rssi);
            U16_TO_STREAM_BE(&p_buf, p_event->eventField.evtExtAdvReport.periodAdvInterval);
            U8_TO_STREAM(&p_buf, p_event->eventField.evtExtAdvReport.directAddr.addrType);
            VARIABLE_COPY_TO_STREAM(&p_buf, p_event->eventField.evtExtAdvReport.directAddr.addr, GAP_MAX_BD_ADDRESS_LEN);
            U8_TO_STREAM(&p_buf, p_event->eventField.evtExtAdvReport.length);
            VARIABLE_COPY_TO_STREAM(&p_buf, p_event->eventField.evtExtAdvReport.advData, BLE_GAP_EXT_ADV_MAX_LENGTH);
            evt_len += BLE_GAP_EVT_SIZE_EXT_ADVERTISING_REPORT;
        }
        break;

        case BLE_GAP_EVT_DEVICE_NAME_CHANGED:
        {
            evt_len = 3 + p_event->eventField.evtDevNameChanged.length;
            VARIABLE_COPY_TO_STREAM(&p_buf, p_event->eventField.evtDevNameChanged.devName, p_event->eventField.evtDevNameChanged.length);
        }
        break;
        default:
        {
            evt_len = 0;
        }
        break;
    }

    if(evt_len)
    {
        APP_PackageUartEvent(evt_len + 2, p_packet);
    }
    OSAL_Free(p_packet);
}

void APP_ProcBleL2capUartEvt(BLE_L2CAP_Event_T *p_event)
{
    uint8_t *p_buf;
    uint16_t evt_len;
    uint8_t *p_buftemp;

    switch (p_event->eventId)
    {
        case BLE_L2CAP_EVT_CONN_PARA_UPDATE_REQ:
        {
            evt_len = 2 + 1 + BLE_L2CAP_EVT_SIZE_CPU_REQ;

            p_buf = OSAL_Malloc(evt_len);
            if (p_buf == NULL)
                return;
            p_buftemp = p_buf;

            U8_TO_STREAM(&p_buftemp, UART_EVENT_TEST);
            U8_TO_STREAM(&p_buftemp, UART_GROUP_BLE_L2CAP);
            U8_TO_STREAM(&p_buftemp, p_event->eventId);
            U16_TO_STREAM_BE(&p_buftemp, p_event->eventField.evtConnParamUpdateReq.connHandle);
            U16_TO_STREAM_BE(&p_buftemp, p_event->eventField.evtConnParamUpdateReq.intervalMin);
            U16_TO_STREAM_BE(&p_buftemp, p_event->eventField.evtConnParamUpdateReq.intervalMax);
            U16_TO_STREAM_BE(&p_buftemp, p_event->eventField.evtConnParamUpdateReq.latency);
            U16_TO_STREAM_BE(&p_buftemp, p_event->eventField.evtConnParamUpdateReq.timeout);
        }
        break;

        case BLE_L2CAP_EVT_CONN_PARA_UPDATE_RSP:
        {
            evt_len = 2 + 1 + BLE_L2CAP_EVT_SIZE_CPU_RSP;

            p_buf = OSAL_Malloc(evt_len);
            if (p_buf == NULL)
                return;
            p_buftemp = p_buf;

            U8_TO_STREAM(&p_buftemp, UART_EVENT_TEST);
            U8_TO_STREAM(&p_buftemp, UART_GROUP_BLE_L2CAP);
            U8_TO_STREAM(&p_buftemp, p_event->eventId);
            U16_TO_STREAM_BE(&p_buftemp, p_event->eventField.evtConnParamUpdateRsp.connHandle);
            U16_TO_STREAM_BE(&p_buftemp, p_event->eventField.evtConnParamUpdateRsp.result);
        }
        break;

        case BLE_L2CAP_EVT_CB_CONN_IND:
        {
            if (s_l2capInitiator == 0)
            {
                evt_len = 2 + 1 + BLE_L2CAP_EVT_SIZE_OPEN_CHANNEL_IND;

                p_buf = OSAL_Malloc(evt_len);
                if (p_buf == NULL)
                    return;
                p_buftemp = p_buf;

                U8_TO_STREAM(&p_buftemp, UART_EVENT_TEST);
                U8_TO_STREAM(&p_buftemp, UART_GROUP_BLE_L2CAP);
                U8_TO_STREAM(&p_buftemp, p_event->eventId);
                U16_TO_STREAM_BE(&p_buftemp, p_event->eventField.evtCbConnInd.connHandle);
                U16_TO_STREAM_BE(&p_buftemp, p_event->eventField.evtCbConnInd.remoteMtu);
                U16_TO_STREAM_BE(&p_buftemp, p_event->eventField.evtCbConnInd.remoteMps);
                U16_TO_STREAM_BE(&p_buftemp, p_event->eventField.evtCbConnInd.initialCredits);
                U16_TO_STREAM_BE(&p_buftemp, p_event->eventField.evtCbConnInd.spsm);
            }
            else
            {
                evt_len = 2 + 1 + BLE_L2CAP_EVT_SIZE_OPEN_CHANNEL_CFM;

                p_buf = OSAL_Malloc(evt_len);
                if (p_buf == NULL)
                    return;

                p_buftemp = p_buf;

                U8_TO_STREAM(&p_buftemp, UART_EVENT_TEST);
                U8_TO_STREAM(&p_buftemp, UART_GROUP_BLE_L2CAP);
                U8_TO_STREAM(&p_buftemp, p_event->eventId);
                U8_TO_STREAM(&p_buftemp, p_event->eventField.evtCbConnInd.leL2capId);
                U16_TO_STREAM_BE(&p_buftemp, p_event->eventField.evtCbConnInd.remoteMtu);
                U16_TO_STREAM_BE(&p_buftemp, p_event->eventField.evtCbConnInd.remoteMps);
                U16_TO_STREAM_BE(&p_buftemp, p_event->eventField.evtCbConnInd.initialCredits);
                U16_TO_STREAM_BE(&p_buftemp, 0);
            }
            s_l2capInitiator = 0;
        }
        break;

        case BLE_L2CAP_EVT_CB_CONN_FAIL_IND:
        {
            if(p_event->eventField.evtCbConnFailInd.result == BLE_L2CAP_RES_CONN_REJECT)
            {
                evt_len = 2 + 1 + BLE_L2CAP_EVT_SIZE_CMD_REJECT_IND;

                p_buf = OSAL_Malloc(evt_len);
                if (p_buf == NULL)
                    return;
                p_buftemp = p_buf;

                U8_TO_STREAM(&p_buftemp, UART_EVENT_TEST);
                U8_TO_STREAM(&p_buftemp, UART_GROUP_BLE_L2CAP);
                U8_TO_STREAM(&p_buftemp, p_event->eventId);
                U16_TO_STREAM_BE(&p_buftemp, p_event->eventField.evtCbConnFailInd.connHandle);
                U16_TO_STREAM_BE(&p_buftemp, p_event->eventField.evtCbConnFailInd.result);
            }
            else
            {
                evt_len = 2 + 1 + BLE_L2CAP_EVT_SIZE_OPEN_CHANNEL_CFM;

                p_buf = OSAL_Malloc(evt_len);
                if (p_buf == NULL)
                    return;

                p_buftemp = p_buf;

                U8_TO_STREAM(&p_buftemp, UART_EVENT_TEST);
                U8_TO_STREAM(&p_buftemp, UART_GROUP_BLE_L2CAP);
                U8_TO_STREAM(&p_buftemp, p_event->eventId);
                U8_TO_STREAM(&p_buftemp, BLE_L2CAP_MAX_CB_NUM);
                U16_TO_STREAM_BE(&p_buftemp, 0);
                U16_TO_STREAM_BE(&p_buftemp, 0);
                U16_TO_STREAM_BE(&p_buftemp, 0);
                U16_TO_STREAM_BE(&p_buftemp, p_event->eventField.evtCbConnFailInd.result);
            }
            s_l2capInitiator = 0;
        }
        break;

        case BLE_L2CAP_EVT_CB_SDU_IND:
        {
            evt_len = 2 + 1 + BLE_L2CAP_EVT_SIZE_RCV_SDU_IND + p_event->eventField.evtCbSduInd.length;

            p_buf = OSAL_Malloc(evt_len);
            if (p_buf == NULL)
                return;

            p_buftemp = p_buf;

            U8_TO_STREAM(&p_buftemp, UART_EVENT_TEST);
            U8_TO_STREAM(&p_buftemp, UART_GROUP_BLE_L2CAP);
            U8_TO_STREAM(&p_buftemp, p_event->eventId);
            U8_TO_STREAM(&p_buftemp, p_event->eventField.evtCbSduInd.leL2capId);
            U16_TO_STREAM_BE(&p_buftemp, p_event->eventField.evtCbSduInd.length);
            memcpy(p_buftemp, p_event->eventField.evtCbSduInd.payload, p_event->eventField.evtCbSduInd.length);
        }
        break;

        case BLE_L2CAP_EVT_CB_ADD_CREDITS_IND:
        {
            evt_len = 2 + 1 + BLE_L2CAP_EVT_SIZE_RCV_CREDITS;

            p_buf = OSAL_Malloc(evt_len);
            if (p_buf == NULL)
                return;
            p_buftemp = p_buf;

            U8_TO_STREAM(&p_buftemp, UART_EVENT_TEST);
            U8_TO_STREAM(&p_buftemp, UART_GROUP_BLE_L2CAP);
            U8_TO_STREAM(&p_buftemp, p_event->eventId);
            U8_TO_STREAM(&p_buftemp, p_event->eventField.evtCbAddCreditsInd.leL2capId);
            U16_TO_STREAM_BE(&p_buftemp, p_event->eventField.evtCbAddCreditsInd.credits);
        }
        break;

        case BLE_L2CAP_EVT_CB_DISC_IND:
        {
            evt_len = 2 + 1 +  BLE_L2CAP_EVT_SIZE_DISCONNECT_IND;

            p_buf = OSAL_Malloc(evt_len);
            if (p_buf == NULL)
                return;
            p_buftemp = p_buf;

            U8_TO_STREAM(&p_buftemp, UART_EVENT_TEST);
            U8_TO_STREAM(&p_buftemp, UART_GROUP_BLE_L2CAP);
            U8_TO_STREAM(&p_buftemp, p_event->eventId);
            U8_TO_STREAM(&p_buftemp, p_event->eventField.evtCbDiscInd.leL2capId);
        }
        break;

        default:
        {
            evt_len = 3;

            p_buf = OSAL_Malloc(evt_len);
            if (p_buf == NULL)
                return;

            p_buftemp = p_buf;

            U8_TO_STREAM(&p_buftemp, UART_EVENT_TEST);
            U8_TO_STREAM(&p_buftemp, UART_GROUP_BLE_L2CAP);
            U8_TO_STREAM(&p_buftemp, p_event->eventId);
        }
        break;
    }

    APP_PackageUartEvent(evt_len, p_buf);
    OSAL_Free(p_buf);
}

static void APP_ReadUsingUuidReviseEndian(uint8_t *p_src, uint16_t readType, uint8_t lengthOfValue)
{
    if (readType == UUID_CHARACTERISTIC)
    {
        uint16_t charValueHandle;
        uint8_t charUuid[ATT_UUID_LENGTH_16];

        // Handle-Value pair: Value - attribute value handle
        BUF_LE_TO_U16(&charValueHandle, (p_src+1));
        U16_TO_BUF_BE((p_src+1), charValueHandle);
        // Handle-Value pair: Value - characteristic uuid
        VARIABLE_TO_BUF(charUuid, (p_src+3), (lengthOfValue-3));
        BUF_COPY_TO_VARIABLE((p_src+3), charUuid, (lengthOfValue-3));
    }
    else if ((readType == UUID_PRIMARY_SERVICE) || (readType == UUID_SECONDARY_SERVICE))
    {
        uint8_t svcUuid[ATT_UUID_LENGTH_16];

        // Handle-Value pair: Value - service uuid
        VARIABLE_TO_BUF(svcUuid, (p_src), lengthOfValue);
        BUF_COPY_TO_VARIABLE((p_src), svcUuid, lengthOfValue);
    }
    else if (readType == UUID_INCLUDE)
    {
        uint16_t svcHandle, endGroupHandle, includeSvcUuid;

        // Handle-Value pair: Value - include service handle
        BUF_LE_TO_U16(&svcHandle, p_src);
        U16_TO_BUF_BE(p_src, svcHandle);
        // Handle-Value pair: Value - include end group handle
        BUF_LE_TO_U16(&endGroupHandle, (p_src+2));
        U16_TO_BUF_BE((p_src+2), endGroupHandle);

        if ((lengthOfValue-4) == ATT_UUID_LENGTH_2)
        {
            /* There will be service uuid in response only if the length of included service uuid is 16-bit. */
            // Handle-Value pair: Value - include service uuid
            BUF_LE_TO_U16(&includeSvcUuid, (p_src+4));
            U16_TO_BUF_BE((p_src+4), includeSvcUuid);
        }
    }
    else if ((readType == UUID_DESCRIPTOR_CHAR_EXT_PROP) || (readType == UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG) || (readType == UUID_DESCRIPTOR_SERVER_CHAR_CONFIG))
    {
        uint16_t descBitField;

        // Handle-Value pair: Value - bitField
        BUF_LE_TO_U16(&descBitField, p_src);
        U16_TO_BUF_BE(p_src, descBitField);
    }
    else if (readType == UUID_DESCRIPTOR_CHAR_PRE_FORMAT)
    {
        uint16_t descUnit, descDesc;

        // Handle-Value pair: Value - unit
        BUF_LE_TO_U16(&descUnit, (p_src+2));
        U16_TO_BUF_BE((p_src+2), descUnit);
        // Handle-Value pair: Value - description
        BUF_LE_TO_U16(&descDesc, (p_src+5));
        U16_TO_BUF_BE((p_src+5), descDesc);
    }
    else if (readType == UUID_DESCRIPTOR_CHAR_AGGRE_FROMAT)
    {
        uint16_t attrHandle;
        uint8_t numOfHandles = (lengthOfValue/2);
        uint8_t j;

        for (j=0; j<numOfHandles; j++)
        {
            // Handle-Value pair: Value - handle
            BUF_LE_TO_U16(&attrHandle, (p_src+(j*2)));
            U16_TO_BUF_BE((p_src+(j*2)), attrHandle);
        }
    }
}

void APP_ProcGattUartEvt(GATT_Event_T *p_event)
{
    uint8_t *p_buf;
    uint8_t *p_packet;
    uint8_t evt_len;

    switch (p_event->eventId)
    {
        case ATT_EVT_TIMEOUT:
        {
            evt_len = 3 ;
            p_buf = OSAL_Malloc(evt_len);
            if (p_buf)
            {
                p_buf[0] = UART_EVENT_TEST;
                p_buf[1] = UART_GROUP_GATT;
                p_buf[2] = p_event->eventId;
                APP_PackageUartEvent(evt_len, p_buf);
                OSAL_Free(p_buf);
            }
        }
        break;

        case ATT_EVT_UPDATE_MTU:
        {
            evt_len = 3 + GATT_EVT_SIZE_UPDATEMTU;
            p_packet = OSAL_Malloc(evt_len);
            p_buf = p_packet;

            if (p_packet)
            {
                U8_TO_STREAM(&p_buf, UART_EVENT_TEST);
                U8_TO_STREAM(&p_buf, UART_GROUP_GATT);
                U8_TO_STREAM(&p_buf, p_event->eventId);
                U16_TO_STREAM_BE(&p_buf, p_event->eventField.onUpdateMTU.connHandle);
                U16_TO_STREAM_BE(&p_buf, p_event->eventField.onUpdateMTU.exchangedMTU);
                APP_PackageUartEvent(evt_len, p_packet);
                OSAL_Free(p_packet);
            }
        }
        break;
        
        case GATTS_EVT_WRITE:
        {
            #if 0
            uint8_t evtLen;
            uint8_t *p_buf;

            /* Only process attributes in range */
            if ((p_event->eventField.onWrite.attrHandle>=PTS_START_HDL) && (p_event->eventField.onWrite.attrHandle<=PTS_END_HDL))
            {
                /* Build UART event. @ref APP_PTS_Evt_GattsWrite_T */
                evtLen = (3+APP_PTS_EVTSIZE_GATTS_WRITE+p_event->eventField.onWrite.writeDataLength);
                p_buf = OSAL_Malloc(evtLen);

                if (p_buf != NULL)
                {
                    p_buf[0] = UART_EVENT_TEST;
                    p_buf[1] = UART_GROUP_PTS_TEST;
                    p_buf[2] = APP_PTS_EVT_GATTS_WRITE;
                    U16_TO_BUF_BE((p_buf+3), p_event->eventField.onWrite.connHandle);
                    U16_TO_BUF_BE((p_buf+5), p_event->eventField.onWrite.attrHandle);
                    p_buf[7] = p_event->eventField.onWrite.writeType;
                    p_buf[8] = 0;
                    U16_TO_BUF_BE((p_buf+9), p_event->eventField.onWrite.valueOffset);
                    U16_TO_BUF_BE((p_buf+11), p_event->eventField.onWrite.writeDataLength);
                    if (p_event->eventField.onWrite.writeDataLength)
                    {
                        memcpy((p_buf+13), p_event->eventField.onWrite.writeValue, p_event->eventField.onWrite.writeDataLength);
                    }
                    APP_PackageUartEvent(evtLen, p_buf);
                    OSAL_Free(p_buf);
                }
            }
            #endif
        }
        break;

        case GATTS_EVT_HV_CONFIRM:
        {
            uint8_t *p_buf;
            uint8_t *p_packet;
            uint8_t evtLen;

            p_packet = OSAL_Malloc(GATT_EVT_SIZE_CONFIRMATION + 2);
            p_buf = p_packet;

            if (p_packet)
            {
                U8_TO_STREAM(&p_buf, UART_EVENT_TEST);
                U8_TO_STREAM(&p_buf, UART_GROUP_PTS_TEST);
                U8_TO_STREAM(&p_buf, APP_PTS_EVT_GATTS_CONFIRMATION);
                U16_TO_STREAM_BE(&p_buf, p_event->eventField.onConfirmation.connHandle);
                U16_TO_STREAM_BE(&p_buf, p_event->eventField.onConfirmation.attrHandle);
                evtLen = 3 + GATT_EVT_SIZE_CONFIRMATION;
                APP_PackageUartEvent(evtLen, p_packet);
                OSAL_Free(p_packet);
            }
        }
        break;
        
        case GATTC_EVT_ERROR_RESP:
        {
            evt_len = 3 + GATT_EVT_SIZE_ERRORRESP;
            p_packet = OSAL_Malloc(evt_len);
            p_buf = p_packet;

            if (p_packet)
            {
                U8_TO_STREAM(&p_buf, UART_EVENT_TEST);
                U8_TO_STREAM(&p_buf, UART_GROUP_GATT);
                U8_TO_STREAM(&p_buf, p_event->eventId);
                U16_TO_STREAM_BE(&p_buf, p_event->eventField.onError.connHandle);
                U8_TO_STREAM(&p_buf, p_event->eventField.onError.reqOpcode);
                U16_TO_STREAM_BE(&p_buf, p_event->eventField.onError.attrHandle);
                U8_TO_STREAM(&p_buf, p_event->eventField.onError.errCode);
                APP_PackageUartEvent(evt_len, p_packet);
                OSAL_Free(p_packet);
            }
        }
        break;

        case GATTC_EVT_DISC_PRIM_SERV_RESP:
        {
            uint8_t i=0;
            uint16_t svcHanlde, endGroupHandle;
            uint8_t svcUuid[ATT_UUID_LENGTH_16];

            evt_len = 3 + p_event->eventField.onDiscPrimServResp.attrDataLength + 6;
            p_packet = OSAL_Malloc(evt_len);
            p_buf = p_packet;

            if (p_packet)
            {
                U8_TO_STREAM(&p_buf, UART_EVENT_TEST);
                U8_TO_STREAM(&p_buf, UART_GROUP_GATT);
                U8_TO_STREAM(&p_buf, p_event->eventId);
                U16_TO_STREAM_BE(&p_buf, p_event->eventField.onDiscPrimServResp.connHandle);
                U8_TO_STREAM(&p_buf, p_event->eventField.onDiscPrimServResp.attrPairLength);
                U16_TO_STREAM_BE(&p_buf, p_event->eventField.onDiscPrimServResp.attrDataLength);
                U8_TO_STREAM(&p_buf, p_event->eventField.onDiscPrimServResp.procedureStatus);

                /* Handling raw data in GATT response. */
                while (i < p_event->eventField.onDiscPrimServResp.attrDataLength)
                {
                    /* Retrieve each attribute data (service handle, end group handle, service uuid) from data list. */
                    BUF_LE_TO_U16(&svcHanlde, (p_event->eventField.onDiscPrimServResp.attrData+i));
                    BUF_LE_TO_U16(&endGroupHandle, (p_event->eventField.onDiscPrimServResp.attrData+i+2));
                    BUF_TO_VARIABLE(svcUuid, (p_event->eventField.onDiscPrimServResp.attrData+i+4), (p_event->eventField.onDiscPrimServResp.attrPairLength-4));

                    /* Put attribute data into UART buffer in big endian format. */
                    U16_TO_STREAM_BE(&p_buf, svcHanlde);
                    U16_TO_STREAM_BE(&p_buf, endGroupHandle);
                    VARIABLE_COPY_TO_STREAM(&p_buf, svcUuid, (p_event->eventField.onDiscPrimServResp.attrPairLength-4));
                    i += p_event->eventField.onDiscPrimServResp.attrPairLength;
                }
                APP_PackageUartEvent(evt_len, p_packet);
                OSAL_Free(p_packet);
            }
        }
        break;

        case GATTC_EVT_DISC_PRIM_SERV_BY_UUID_RESP:
        {
            uint8_t i=0;
            uint16_t svcHanlde, endGroupHandle;

            evt_len = 3 + p_event->eventField.onDiscPrimServByUuidResp.handleInfoLength + 5;
            p_packet = OSAL_Malloc(evt_len);
            p_buf = p_packet;

            if (p_packet)
            {
                U8_TO_STREAM(&p_buf, UART_EVENT_TEST);
                U8_TO_STREAM(&p_buf, UART_GROUP_GATT);
                U8_TO_STREAM(&p_buf, p_event->eventId);
                U16_TO_STREAM_BE(&p_buf, p_event->eventField.onDiscPrimServByUuidResp.connHandle);
                U16_TO_STREAM_BE(&p_buf, p_event->eventField.onDiscPrimServByUuidResp.handleInfoLength);
                U8_TO_STREAM(&p_buf, p_event->eventField.onDiscPrimServByUuidResp.procedureStatus);

                /* Handling raw data in GATT response. */
                while (i < p_event->eventField.onDiscPrimServByUuidResp.handleInfoLength)
                {
                    /* Retrieve each handles information (service handle, end group handle) from handle information list. */
                    BUF_LE_TO_U16(&svcHanlde, (p_event->eventField.onDiscPrimServByUuidResp.handleInfo+i));
                    BUF_LE_TO_U16(&endGroupHandle, (p_event->eventField.onDiscPrimServByUuidResp.handleInfo+i+2));

                    /* Put handles information into UART buffer in big endian format. */
                    U16_TO_STREAM_BE(&p_buf, svcHanlde);
                    U16_TO_STREAM_BE(&p_buf, endGroupHandle);
                    i += 4;
                }

                APP_PackageUartEvent(evt_len, p_packet);
                OSAL_Free(p_packet);
            }
        }
        break;

        case GATTC_EVT_DISC_CHAR_RESP:
        {
            uint8_t i=0, charProperty;
            uint16_t attrHandle, charValueHandle;
            uint8_t charUuid[ATT_UUID_LENGTH_16];

            evt_len = 3 + p_event->eventField.onDiscCharResp.attrDataLength + 6;
            p_packet = OSAL_Malloc(evt_len);
            p_buf = p_packet;

            if (p_packet)
            {
                U8_TO_STREAM(&p_buf, UART_EVENT_TEST);
                U8_TO_STREAM(&p_buf, UART_GROUP_GATT);
                U8_TO_STREAM(&p_buf, p_event->eventId);
                U16_TO_STREAM_BE(&p_buf, p_event->eventField.onDiscCharResp.connHandle);
                U8_TO_STREAM(&p_buf, p_event->eventField.onDiscCharResp.attrPairLength);
                U16_TO_STREAM_BE(&p_buf, p_event->eventField.onDiscCharResp.attrDataLength);
                U8_TO_STREAM(&p_buf, p_event->eventField.onDiscCharResp.procedureStatus);

                while (i < p_event->eventField.onDiscCharResp.attrDataLength)
                {
                    /* Retrieve attribute handle from handle-value pair. */
                    BUF_LE_TO_U16(&attrHandle, (p_event->eventField.onDiscCharResp.attrData+i));
                    /* Parsing attribute value from handle-value pair: property/value handle/uuid. */
                    charProperty = *(p_event->eventField.onDiscCharResp.attrData+i+2);
                    BUF_LE_TO_U16(&charValueHandle, (p_event->eventField.onDiscCharResp.attrData+i+3));
                    VARIABLE_TO_BUF(charUuid, (p_event->eventField.onDiscCharResp.attrData+i+5), (p_event->eventField.onDiscCharResp.attrPairLength-5));

                    /* Put attribute handle into UART buffer*/
                    U16_TO_STREAM_BE(&p_buf, attrHandle);
                    /* Put attribute value into UART buffer*/
                    U8_TO_STREAM(&p_buf, charProperty);
                    U16_TO_STREAM_BE(&p_buf, charValueHandle);
                    VARIABLE_COPY_TO_STREAM(&p_buf, charUuid, (p_event->eventField.onDiscCharResp.attrPairLength-5));

                    i += p_event->eventField.onDiscCharResp.attrPairLength;
                }

                APP_PackageUartEvent(evt_len, p_packet);
                OSAL_Free(p_packet);
            }
        }
        break;

        case GATTC_EVT_DISC_CHAR_BY_UUID_RESP:
        {
            uint8_t i=0, charProperty;
            uint16_t attrHandle, charValueHandle;
            uint8_t charUuid[ATT_UUID_LENGTH_16];

            evt_len = 3 + p_event->eventField.onDiscCharByUuid.attrDataLength + 6;
            p_packet = OSAL_Malloc(evt_len);
            p_buf = p_packet;

            if (p_packet)
            {
                U8_TO_STREAM(&p_buf, UART_EVENT_TEST);
                U8_TO_STREAM(&p_buf, UART_GROUP_GATT);
                U8_TO_STREAM(&p_buf, p_event->eventId);
                U16_TO_STREAM_BE(&p_buf, p_event->eventField.onDiscCharByUuid.connHandle);
                U8_TO_STREAM(&p_buf, p_event->eventField.onDiscCharByUuid.attrPairLength);
                U16_TO_STREAM_BE(&p_buf, p_event->eventField.onDiscCharByUuid.attrDataLength);
                U8_TO_STREAM(&p_buf, p_event->eventField.onDiscCharByUuid.procedureStatus);

                while (i < p_event->eventField.onDiscCharByUuid.attrDataLength)
                {
                    /* Retrieve attribute handle from handle-value pair. */
                    BUF_LE_TO_U16(&attrHandle, (p_event->eventField.onDiscCharByUuid.attrData+i));
                    /* Parsing attribute value from handle-value pair: property/value handle/uuid. */
                    charProperty = *(p_event->eventField.onDiscCharByUuid.attrData+i+2);
                    BUF_LE_TO_U16(&charValueHandle, (p_event->eventField.onDiscCharByUuid.attrData+i+3));
                    VARIABLE_TO_BUF(charUuid, (p_event->eventField.onDiscCharByUuid.attrData+i+5), (p_event->eventField.onDiscCharByUuid.attrPairLength-5));

                    /* Put attribute handle into UART buffer*/
                    U16_TO_STREAM_BE(&p_buf, attrHandle);
                    /* Put attribute value into UART buffer*/
                    U8_TO_STREAM(&p_buf, charProperty);
                    U16_TO_STREAM_BE(&p_buf, charValueHandle);
                    VARIABLE_COPY_TO_STREAM(&p_buf, charUuid, (p_event->eventField.onDiscCharByUuid.attrPairLength-5));

                    i += p_event->eventField.onDiscCharByUuid.attrPairLength;
                }
                APP_PackageUartEvent(evt_len, p_packet);
                OSAL_Free(p_packet);
            }
        }
        break;

        case GATTC_EVT_DISC_DESC_RESP:
        {
            uint8_t i=0;
            uint16_t descHandle, descUuid;

            evt_len = 3 + p_event->eventField.onDiscDescResp.infoDataLength + 6;
            p_packet = OSAL_Malloc(evt_len);
            p_buf = p_packet;

            if (p_packet)
            {
                U8_TO_STREAM(&p_buf, UART_EVENT_TEST);
                U8_TO_STREAM(&p_buf, UART_GROUP_GATT);
                U8_TO_STREAM(&p_buf, p_event->eventId);
                U16_TO_STREAM_BE(&p_buf, p_event->eventField.onDiscDescResp.connHandle);
                U8_TO_STREAM(&p_buf, p_event->eventField.onDiscDescResp.infoDataFormat);
                U16_TO_STREAM_BE(&p_buf, p_event->eventField.onDiscDescResp.infoDataLength);
                U8_TO_STREAM(&p_buf, p_event->eventField.onDiscDescResp.procedureStatus);

                /* Handling raw data in GATT response. */
                while (i < p_event->eventField.onDiscDescResp.infoDataLength)
                {
                    /* Retrieve each information data (descriptor handle, descriptor uuid) from information data list. */
                    /* In discover descriptor response, only information data with 16-bit UUID format will be returned. */
                    BUF_LE_TO_U16(&descHandle, (p_event->eventField.onDiscDescResp.infoData+i));
                    BUF_LE_TO_U16(&descUuid, (p_event->eventField.onDiscDescResp.infoData+i+2));

                    /* Put descriptor handle/uuid into UART buffer in big endian format. */
                    U16_TO_STREAM_BE(&p_buf, descHandle);
                    U16_TO_STREAM_BE(&p_buf, descUuid);
                    i += 0x04;
                }

                APP_PackageUartEvent(evt_len, p_packet);
                OSAL_Free(p_packet);
            }
        }
        break;

        case GATTC_EVT_READ_USING_UUID_RESP:
        {
            uint8_t i=0;
            uint16_t attrHandle;

            evt_len = 3 + p_event->eventField.onReadUsingUuidResp.attrDataLength + 5;
            p_packet = OSAL_Malloc(evt_len);
            p_buf = p_packet;

            if (p_packet)
            {
                U8_TO_STREAM(&p_buf, UART_EVENT_TEST);
                U8_TO_STREAM(&p_buf, UART_GROUP_GATT);
                U8_TO_STREAM(&p_buf, p_event->eventId);
                U16_TO_STREAM_BE(&p_buf, p_event->eventField.onReadUsingUuidResp.connHandle);
                U8_TO_STREAM(&p_buf, p_event->eventField.onReadUsingUuidResp.attrPairLength);
                U16_TO_STREAM_BE(&p_buf, p_event->eventField.onReadUsingUuidResp.attrDataLength);

                while (i < p_event->eventField.onReadUsingUuidResp.attrDataLength)
                {
                    /* Retrieve attribute handle from handle-value pair. */
                    BUF_LE_TO_U16(&attrHandle, p_event->eventField.onReadUsingUuidResp.attrData+i);

                    /* Put attribute handle into UART buffer*/
                    U16_TO_STREAM_BE(&p_buf, attrHandle);
                    if (g_appDb.gattcReadUuid != 0)
                    {
                        APP_ReadUsingUuidReviseEndian((p_event->eventField.onReadUsingUuidResp.attrData+i+2), g_appDb.gattcReadUuid, (p_event->eventField.onReadUsingUuidResp.attrPairLength-2));
                    }
                    VARIABLE_COPY_TO_STREAM(&p_buf, (p_event->eventField.onReadUsingUuidResp.attrData+i+2), (p_event->eventField.onReadUsingUuidResp.attrPairLength-2));

                    i += p_event->eventField.onReadUsingUuidResp.attrPairLength;
                }
                g_appDb.gattcReadUuid = 0;
                APP_PackageUartEvent(evt_len, p_packet);
                OSAL_Free(p_packet);
            }
        }
        break;

        case GATTC_EVT_READ_RESP:
        {
            evt_len = 3 + p_event->eventField.onReadResp.attrDataLength + 5;
            p_packet = OSAL_Malloc(evt_len);
            p_buf = p_packet;

            if (p_packet)
            {
                U8_TO_STREAM(&p_buf, UART_EVENT_TEST);
                U8_TO_STREAM(&p_buf, UART_GROUP_GATT);
                U8_TO_STREAM(&p_buf, p_event->eventId);
                U16_TO_STREAM_BE(&p_buf, p_event->eventField.onReadResp.connHandle);
                U8_TO_STREAM(&p_buf, p_event->eventField.onReadResp.responseType);
                U16_TO_STREAM_BE(&p_buf, p_event->eventField.onReadResp.attrDataLength);
                VARIABLE_COPY_TO_STREAM(&p_buf, p_event->eventField.onReadResp.readValue, p_event->eventField.onReadResp.attrDataLength);
                APP_PackageUartEvent(evt_len, p_packet);
                OSAL_Free(p_packet);
            }
        }
        break;

        case GATTC_EVT_WRITE_RESP:
        {
            evt_len = 3 + p_event->eventField.onWriteResp.writeLength + 9;
            p_packet = OSAL_Malloc(evt_len);
            p_buf = p_packet;

            if (p_packet)
            {
                U8_TO_STREAM(&p_buf, UART_EVENT_TEST);
                U8_TO_STREAM(&p_buf, UART_GROUP_GATT);
                U8_TO_STREAM(&p_buf, p_event->eventId);
                U16_TO_STREAM_BE(&p_buf, p_event->eventField.onWriteResp.connHandle);
                U8_TO_STREAM(&p_buf, p_event->eventField.onWriteResp.responseType);
                U16_TO_STREAM_BE(&p_buf, p_event->eventField.onWriteResp.charHandle);
                U16_TO_STREAM_BE(&p_buf, p_event->eventField.onWriteResp.valueOffset);
                U16_TO_STREAM_BE(&p_buf, p_event->eventField.onWriteResp.writeLength);
                VARIABLE_COPY_TO_STREAM(&p_buf, p_event->eventField.onWriteResp.writeValue, p_event->eventField.onWriteResp.writeLength);
                APP_PackageUartEvent(evt_len, p_packet);
                OSAL_Free(p_packet);
            }
        }
        break;

        case GATTC_EVT_HV_NOTIFY:
        {
            evt_len = 3 + p_event->eventField.onNotification.receivedLength + 6;
            p_packet = OSAL_Malloc(evt_len);
            p_buf = p_packet;

            if (p_packet)
            {
                U8_TO_STREAM(&p_buf, UART_EVENT_TEST);
                U8_TO_STREAM(&p_buf, UART_GROUP_GATT);
                U8_TO_STREAM(&p_buf, p_event->eventId);
                U16_TO_STREAM_BE(&p_buf, p_event->eventField.onNotification.connHandle);
                U16_TO_STREAM_BE(&p_buf, p_event->eventField.onNotification.charHandle);
                U16_TO_STREAM_BE(&p_buf, p_event->eventField.onNotification.receivedLength);
                VARIABLE_COPY_TO_STREAM(&p_buf, p_event->eventField.onNotification.receivedValue, p_event->eventField.onNotification.receivedLength);
                APP_PackageUartEvent(evt_len, p_packet);
                OSAL_Free(p_packet);
            }
        }
        break;

        case GATTC_EVT_HV_INDICATE:
        {
            evt_len = 3 + p_event->eventField.onIndication.receivedLength + 6;
            p_packet = OSAL_Malloc(evt_len);
            p_buf = p_packet;

            if (p_packet)
            {
                U8_TO_STREAM(&p_buf, UART_EVENT_TEST);
                U8_TO_STREAM(&p_buf, UART_GROUP_GATT);
                U8_TO_STREAM(&p_buf, p_event->eventId);
                U16_TO_STREAM_BE(&p_buf, p_event->eventField.onIndication.connHandle);
                U16_TO_STREAM_BE(&p_buf, p_event->eventField.onIndication.charHandle);
                U16_TO_STREAM_BE(&p_buf, p_event->eventField.onIndication.receivedLength);
                VARIABLE_COPY_TO_STREAM(&p_buf, p_event->eventField.onIndication.receivedValue, p_event->eventField.onIndication.receivedLength);
                APP_PackageUartEvent(evt_len, p_packet);
                OSAL_Free(p_packet);
            }
        }
        break;

        case GATTC_EVT_PROTOCOL_AVAILABLE:
        {
            evt_len = 3 + 2;
            p_packet = OSAL_Malloc(evt_len);
            p_buf = p_packet;

            if (p_packet)
            {
                U8_TO_STREAM(&p_buf, UART_EVENT_TEST);
                U8_TO_STREAM(&p_buf, UART_GROUP_GATT);
                U8_TO_STREAM(&p_buf, p_event->eventId);
                U16_TO_STREAM_BE(&p_buf, p_event->eventField.onClientProtocolAvailable.connHandle);
#if 0//AutoPTS_V2.1.2 doesn't support the event.
                APP_PackageUartEvent(evt_len, p_packet);
#endif
                OSAL_Free(p_packet);
            }
        }
        break;

        default:
            break;
    }
}

void APP_ProcBleSmpUartEvt(BLE_SMP_Event_T *p_event)
{
    uint8_t *p_packet;
    uint8_t *p_buf;
    uint8_t evt_len;

    p_packet = OSAL_Malloc(sizeof(BLE_SMP_Event_T) + 2);

    if (p_packet==NULL)
    {
        return;
    }
    p_buf=p_packet;
    evt_len = 3;

    U8_TO_STREAM(&p_buf, UART_EVENT_TEST);
    U8_TO_STREAM(&p_buf, UART_GROUP_BLE_SMP);
    U8_TO_STREAM(&p_buf, p_event->eventId);

    switch (p_event->eventId)
    {
        case BLE_SMP_EVT_PAIRING_COMPLETE:
        {
            evt_len += 20;

            U16_TO_STREAM_BE(&p_buf, p_event->eventField.evtPairingComplete.connHandle);
            U8_TO_STREAM(&p_buf, p_event->eventField.evtPairingComplete.status);
            U8_TO_STREAM(&p_buf, p_event->eventField.evtPairingComplete.failReason);
            VARIABLE_COPY_TO_STREAM(&p_buf, p_event->eventField.evtPairingComplete.encryptKey, 16);
        }
        break;

        case BLE_SMP_EVT_SECURITY_REQUEST:
        {
            evt_len += 3;

            U16_TO_STREAM_BE(&p_buf, p_event->eventField.evtSecurityReq.connHandle);
            U8_TO_STREAM(&p_buf, p_event->eventField.evtSecurityReq.authReq);
        }
        break;

        case BLE_SMP_EVT_NUMERIC_COMPARISON_CONFIRM_REQUEST:
        {
            evt_len += 8;

            U16_TO_STREAM_BE(&p_buf, p_event->eventField.evtDisplayCompareValueReq.connHandle);
            VARIABLE_COPY_TO_STREAM(&p_buf, p_event->eventField.evtDisplayCompareValueReq.value, 6);
        }
        break;

        case BLE_SMP_EVT_INPUT_PASSKEY_REQUEST:
        {
            evt_len += 2;

            U16_TO_STREAM_BE(&p_buf, p_event->eventField.evtInputPasskeyReq.connHandle);
        }
        break;

        case BLE_SMP_EVT_DISPLAY_PASSKEY_REQUEST:
        {
            evt_len += 2;

            U16_TO_STREAM_BE(&p_buf, p_event->eventField.evtDisplayPasskeyReq.connHandle);
            APP_PackageUartEvent(evt_len, p_packet);

            #ifdef APP_PAIRING_ENABLE
            uint8_t app_passkey[6];
            BLE_SMP_GeneratePasskey(app_passkey);

            APP_SendCmdCompleteEvt(6, UART_GROUP_BLE_SMP, UART_BLE_SMP_CMD_GEN_PASSKEY, 0, app_passkey);

            if ((g_appDb.ioCapability == BLE_SMP_IO_DISPLAYONLY) || (g_appDb.ioCapability == BLE_SMP_IO_KEYBOARDDISPLAY))
            {
                uint16_t result;

                result = BLE_SMP_PasskeyReply(p_event->eventField.evtDisplayPasskeyReq.connHandle, app_passkey);

                APP_SendCmdCompleteEvt(0, UART_GROUP_BLE_SMP, UART_BLE_SMP_CMD_PASSKEY_REPLY, result, NULL);
            }
            #endif
        }
        break;

        case BLE_SMP_EVT_PAIRING_REQUEST:
        {
            evt_len += 2;
            U16_TO_STREAM_BE(&p_buf, p_event->eventField.evtPairingReq.connHandle);
        }
        break;

        case BLE_SMP_EVT_INPUT_OOB_DATA_REQUEST:
        {
            evt_len += 2;

            U16_TO_STREAM_BE(&p_buf, p_event->eventField.evtInputOobData.connHandle);
        }
        break;

        default:
            evt_len=0;
        break;
    }

    if(evt_len && (p_event->eventId != BLE_SMP_EVT_DISPLAY_PASSKEY_REQUEST))
        APP_PackageUartEvent(evt_len, p_packet);
    OSAL_Free(p_packet);
}

void APP_ProcTrcbpsUartEvt(BLE_TRCBPS_Event_T *p_event)
{
    uint8_t *p_buf;
    uint8_t evtLen;
    uint8_t *p_buftemp;

    switch(p_event->eventId)
    {
        case BLE_TRCBPS_EVT_CONNECTION_STATUS:
        {
            evtLen = 2 + 1 + BLE_TRCBPS_EVT_SIZE_CONN_STATUS;
            
            p_buf = OSAL_Malloc(evtLen);
            if (p_buf == NULL)
                return;
            p_buftemp = p_buf;

            U8_TO_STREAM(&p_buftemp, UART_EVENT_TEST);
            U8_TO_STREAM(&p_buftemp, UART_GROUP_BLE_L2CAP);
            U8_TO_STREAM(&p_buftemp, (p_event->eventId + 0x50));
            U16_TO_STREAM_BE(&p_buftemp, p_event->eventField.connStatus.connHandle);
            U8_TO_STREAM(&p_buftemp, p_event->eventField.connStatus.leL2capId);
            U8_TO_STREAM(&p_buftemp, p_event->eventField.connStatus.status);

            if (p_buf != NULL)
            {
                APP_PackageUartEvent(evtLen, p_buf);
                OSAL_Free(p_buf);
            }
        }
        break;

        case BLE_TRCBPS_EVT_RECEIVE_DATA:
        {
#if 0
            uint16_t dataLen;
            uint8_t *p_data = NULL;

            BLE_TRCBPS_GetDataLength(p_event->eventField.onReceiveData.leL2capId, &dataLen);

#if 0
            evtLen = 2 + 1 + BLE_TRCBPS_EVT_SIZE_RCV_DATA + sizeof(dataLen);  // +2 means the length
            
            p_buf = OSAL_Malloc(evtLen);
            if (p_buf == NULL)
                return;
            p_buftemp = p_buf;

            U8_TO_STREAM(&p_buftemp, UART_EVENT_TEST);
            U8_TO_STREAM(&p_buftemp, UART_GROUP_BLE_L2CAP);
            U8_TO_STREAM(&p_buftemp, (p_event->eventId + 0x50));
            U8_TO_STREAM(&p_buftemp, p_event->eventField.onReceiveData.leL2capId);
            U16_TO_STREAM_BE(&p_buftemp, dataLen);

            APP_UART_PackageEvent(evtLen, p_buf);
            OSAL_Free(p_buf);
            p_buf = NULL;

            //++ Get data
            p_data = OSAL_Malloc(dataLen);

            if (p_data != NULL)
            {
                BLE_TRCBPS_GetData(p_event->eventField.onReceiveData.leL2capId, p_data);

                evtLen = 2 + 1 + 3 + dataLen;  //+4 means leL2capId + length

                p_buf = OSAL_Malloc(evtLen);
                if (p_buf == NULL)
                    return;

                p_buftemp = p_buf;

                U8_TO_STREAM(&p_buftemp, UART_EVENT_TEST);
                U8_TO_STREAM(&p_buftemp, UART_GROUP_BLE_L2CAP);
                U8_TO_STREAM(&p_buftemp, BLE_L2CAP_EVT_CB_SDU_IND + 0x50);
                U8_TO_STREAM(&p_buftemp, p_event->eventField.onReceiveData.leL2capId);
                U16_TO_STREAM_BE(&p_buftemp, dataLen);
                memcpy(p_buftemp, p_data, dataLen);

                APP_UART_PackageEvent(evtLen, p_buf);
                OSAL_Free(p_buf);
                OSAL_Free(p_data);
                p_buf = NULL;
            }
            // -- Get data
#else
            #if 0
            evtLen = 2 + 1 + BLE_TRCBPS_EVT_SIZE_RCV_DATA + sizeof(dataLen);  // +2 means the length
            
            p_buf = OSAL_Malloc(evtLen);
            if (p_buf == NULL)
                return;
            p_buftemp = p_buf;
            
            U8_TO_STREAM(&p_buftemp, UART_EVENT_TEST);
            U8_TO_STREAM(&p_buftemp, UART_GROUP_BLE_L2CAP);
            U8_TO_STREAM(&p_buftemp, (p_event->eventId + 0x50));
            U8_TO_STREAM(&p_buftemp, p_event->eventField.onReceiveData.leL2capId);
            U16_TO_STREAM_BE(&p_buftemp, dataLen);
            
            APP_UART_PackageEvent(evtLen, p_buf);
            OSAL_Free(p_buf);
            p_buf = NULL;
            #endif
            
            //++ Get data
            p_data = OSAL_Malloc(dataLen);
            
            if (p_data != NULL)
            {
                BLE_TRCBPS_GetData(p_event->eventField.onReceiveData.leL2capId, p_data);

                OSAL_Free(p_data);
            }
            //-- Get data

#endif
#endif
        }
        break;

        default:
            break;
    }
}

void APP_ProcUtilityCmd(uint8_t *p_cmdPayload)
{
    uint8_t evt_len = 0;
    uint8_t *p_evt_buf = NULL;
    uint16_t result = APP_RES_SUCCESS;

    switch (*(p_cmdPayload))
    {
        case UART_UTILITY_CMD_CURR_CONN_HANDLE:
        {
            uint8_t handleValue[2];
            if (g_appConnList[0].state != APP_STATE_IDLE)
            {
                U16_TO_BUF_BE(handleValue, g_appConnList[0].connHandle);
            }
            else
            {
                result = APP_RES_BAD_STATE;
                U16_TO_BUF_BE(handleValue, 0);
            }

            p_evt_buf = handleValue;
            evt_len = 2;
        }
        break;
        default:
        {
            result = APP_RES_INVALID_PARA;
        }
        break;
    }
    APP_SendCmdCompleteEvt(evt_len, UART_GROUP_UTILITY, *(p_cmdPayload), result, p_evt_buf);
}

void APP_ProcGapCmd(uint8_t opcode, uint16_t data_len, uint8_t *p_cmdPayload)
{
    switch (opcode)
    {
        case UART_GAP_CMD_RESET:
        {
            RCON_SoftwareReset();
        }
        break;
    }
    APP_SendCmdCompleteEvt(0, UART_GROUP_BT_GAP, opcode, 0, NULL);
}


void APP_ProcGattCmd(uint8_t opcode, uint16_t data_len, uint8_t *p_cmdPayload)
{
    uint8_t *p_bufTmp;
    uint8_t status = APP_ERR_UNKNOWN_COMMAND;
    uint8_t *p_eventBuf = NULL;
    uint16_t eventLength = 0;

    p_bufTmp = p_cmdPayload;

    switch (opcode)
    {
        #ifdef APP_ATT_CLIENT_ENABLE
        case UART_GATT_CMD_READ:
        {
            uint16_t connHandle, attrHandle, readOffset;

            STREAM_BE_TO_U16(&connHandle, &p_bufTmp);
            STREAM_BE_TO_U16(&attrHandle, &p_bufTmp);
            STREAM_BE_TO_U16(&readOffset, &p_bufTmp);
            status = GATTC_Read(connHandle, attrHandle, readOffset);
        }
        break;
        #endif

        #ifdef APP_ATT_CLIENT_ENABLE
        case UART_GATT_CMD_WRITE:
        {
            uint16_t connHandle;
            GATTC_WriteParams_T *p_writeParams;

            p_writeParams = (GATTC_WriteParams_T *)OSAL_Malloc(sizeof(GATTC_WriteParams_T));
            if (p_writeParams != NULL)
            {
                STREAM_BE_TO_U16(&connHandle, &p_bufTmp);
                STREAM_BE_TO_U16(&p_writeParams->charHandle, &p_bufTmp);
                STREAM_BE_TO_U16(&p_writeParams->charLength, &p_bufTmp);
                STREAM_COPY_TO_VARIABLE(p_writeParams->charValue, &p_bufTmp, p_writeParams->charLength);
                STREAM_TO_U8(&p_writeParams->writeType, &p_bufTmp);
                STREAM_BE_TO_U16(&p_writeParams->valueOffset, &p_bufTmp);
                STREAM_TO_U8(&p_writeParams->flags, &p_bufTmp);
                status = GATTC_Write(connHandle, p_writeParams);
                OSAL_Free((uint8_t *)p_writeParams);
            }
            else
            {
                status = APP_RES_OOM;
            }
        }
        break;
        #endif

        #ifdef APP_ATT_CLIENT_ENABLE
        case UART_GATT_CMD_HANDLE_CONF:
        {
            uint16_t connHandle;

            STREAM_BE_TO_U16(&connHandle, &p_bufTmp);
            status = GATTC_HandleValueConfirm(connHandle);
        }
        break;
        #endif

        #ifdef APP_ATT_CLIENT_ENABLE
        case UART_GATT_CMD_EX_MTU:
        {
            uint16_t connHandle, mtuLength;

            STREAM_BE_TO_U16(&connHandle, &p_bufTmp);
            STREAM_BE_TO_U16(&mtuLength, &p_bufTmp);
            status = GATTC_ExchangeMTURequest(connHandle, mtuLength);
        }
        break;
        #endif

        #ifdef APP_ATT_CLIENT_ENABLE
        case UART_GATT_CMD_DIS_ALL_PSERV:
        {
            uint16_t connHandle;

            STREAM_BE_TO_U16(&connHandle, &p_bufTmp);
            status = GATTC_DiscoverAllPrimaryServices(connHandle);
        }
        break;
        #endif

        #ifdef APP_ATT_CLIENT_ENABLE
        case UART_GATT_CMD_DIS_PRIMARY_SERV_UUID:
        {
            uint16_t connHandle;
            GATTC_DiscoverPrimaryServiceByUuidParams_T *p_discParams;

            p_discParams = (GATTC_DiscoverPrimaryServiceByUuidParams_T *)OSAL_Malloc(sizeof(GATTC_DiscoverPrimaryServiceByUuidParams_T));
            if (p_discParams != NULL)
            {
                STREAM_BE_TO_U16(&connHandle, &p_bufTmp);
                STREAM_BE_TO_U16(&p_discParams->startHandle, &p_bufTmp);
                STREAM_BE_TO_U16(&p_discParams->endHandle, &p_bufTmp);
                STREAM_TO_U8(&p_discParams->valueLength, &p_bufTmp);
                /* Endian of docklight script is big endian. Endian of API is little endian. */
                STREAM_TO_VARIABLE(p_discParams->value, &p_bufTmp, p_discParams->valueLength);
                status = GATTC_DiscoverPrimaryServiceByUUID(connHandle, p_discParams);
                OSAL_Free((uint8_t *)p_discParams);
            }
            else
            {
                status = APP_RES_OOM;
            }
        }
        break;
        #endif

        #ifdef APP_ATT_CLIENT_ENABLE
        case UART_GATT_CMD_DIS_ALL_CHAR:
        {
            uint16_t connHandle, startHandle, endHandle;

            STREAM_BE_TO_U16(&connHandle, &p_bufTmp);
            STREAM_BE_TO_U16(&startHandle, &p_bufTmp);
            STREAM_BE_TO_U16(&endHandle, &p_bufTmp);
            status = GATTC_DiscoverAllCharacteristics(connHandle, startHandle, endHandle);
        }
        break;
        #endif

        #ifdef APP_ATT_CLIENT_ENABLE
        case UART_GATT_CMD_DIS_CHAR_USING_UUID:
        {
            uint16_t connHandle;
            GATTC_DiscoverCharacteristicByUuidParams_T *p_discParams;

            p_discParams = (GATTC_DiscoverCharacteristicByUuidParams_T *)OSAL_Malloc(sizeof(GATTC_DiscoverCharacteristicByUuidParams_T));
            if (p_discParams != NULL)
            {
                STREAM_BE_TO_U16(&connHandle, &p_bufTmp);
                STREAM_BE_TO_U16(&p_discParams->startHandle, &p_bufTmp);
                STREAM_BE_TO_U16(&p_discParams->endHandle, &p_bufTmp);
                p_discParams->uuidLength = (data_len-6);
                /* Endian of docklight script is big endian. Endian of API is little endian. */
                STREAM_TO_VARIABLE(p_discParams->uuid, &p_bufTmp, p_discParams->uuidLength);
                status = GATTC_DiscoverCharacteristicsByUUID(connHandle, p_discParams);
                OSAL_Free((uint8_t *)p_discParams);
            }
            else
            {
                status = APP_RES_OOM;
            }
        }
        break;
        #endif

        #ifdef APP_ATT_CLIENT_ENABLE
        case UART_GATT_CMD_DIS_ALL_DESC:
        {
            uint16_t connHandle, startHandle, endHandle;

            STREAM_BE_TO_U16(&connHandle, &p_bufTmp);
            STREAM_BE_TO_U16(&startHandle, &p_bufTmp);
            STREAM_BE_TO_U16(&endHandle, &p_bufTmp);
            status = GATTC_DiscoverAllDescriptors(connHandle, startHandle, endHandle);
        }
        break;
        #endif

        #ifdef APP_ATT_CLIENT_ENABLE
        case UART_GATT_CMD_READ_USING_UUID:
        {
            uint16_t connHandle;
            GATTC_ReadByTypeParams_T *p_readParams;

            p_readParams = (GATTC_ReadByTypeParams_T *)OSAL_Malloc(sizeof(GATTC_ReadByTypeParams_T));
            if (p_readParams != NULL)
            {
                STREAM_BE_TO_U16(&connHandle, &p_bufTmp);
                STREAM_BE_TO_U16(&p_readParams->startHandle, &p_bufTmp);
                STREAM_BE_TO_U16(&p_readParams->endHandle, &p_bufTmp);
                STREAM_TO_U8(&p_readParams->attrTypeLength, &p_bufTmp);
                /* Endian of docklight script is big endian. Endian of API is little endian. */
                STREAM_TO_VARIABLE(p_readParams->attrType, &p_bufTmp, p_readParams->attrTypeLength);
                status = GATTC_ReadUsingUUID(connHandle, p_readParams);

                if ((status == APP_RES_SUCCESS) && (p_readParams->attrTypeLength == ATT_UUID_LENGTH_2))
                {
                    /* Record UUID for event parsing. */
                    BUF_LE_TO_U16(&g_appDb.gattcReadUuid, p_readParams->attrType);
                }
                else
                {
                    g_appDb.gattcReadUuid = 0;
                }
                OSAL_Free((uint8_t *)p_readParams);
            }
            else
            {
                status = APP_RES_OOM;
            }
        }
        break;
        #endif

        #ifdef APP_ATT_SERVER_ENABLE
        case UART_GATT_CMD_SEND_HANDLE_VALUE:
        {
            uint16_t connHandle;
            GATTS_HandleValueParams_T *p_hvParams;

            p_hvParams = (GATTS_HandleValueParams_T *)OSAL_Malloc(sizeof(GATTS_HandleValueParams_T));
            if (p_hvParams != NULL)
            {
                STREAM_BE_TO_U16(&connHandle, &p_bufTmp);
                STREAM_BE_TO_U16(&p_hvParams->charHandle, &p_bufTmp);
                STREAM_BE_TO_U16(&p_hvParams->charLength, &p_bufTmp);
                STREAM_COPY_TO_VARIABLE(p_hvParams->charValue, &p_bufTmp, p_hvParams->charLength);
                STREAM_TO_U8(&p_hvParams->sendType, &p_bufTmp);
                status = GATTS_SendHandleValue(connHandle, p_hvParams);
                OSAL_Free((uint8_t *)p_hvParams);
            }
            else
            {
                status = APP_RES_OOM;
            }
        }
        break;
        #endif

        #ifdef APP_ATT_SERVER_ENABLE
        case UART_GATT_CMD_SEND_READ_RESP:
        {
            uint16_t connHandle;
            GATTS_SendReadRespParams_T *p_respParams;

            p_respParams = (GATTS_SendReadRespParams_T *)OSAL_Malloc(sizeof(GATTS_SendReadRespParams_T));
            if (p_respParams != NULL)
            {
                STREAM_BE_TO_U16(&connHandle, &p_bufTmp);
                STREAM_TO_U8(&p_respParams->responseType, &p_bufTmp);
                STREAM_BE_TO_U16(&p_respParams->attrLength, &p_bufTmp);
                STREAM_COPY_TO_VARIABLE(p_respParams->attrValue, &p_bufTmp, p_respParams->attrLength);
                status = GATTS_SendReadResponse(connHandle, p_respParams);
                OSAL_Free((uint8_t *)p_respParams);
            }
            else
            {
                status = APP_RES_OOM;
            }
        }
        break;
        #endif

        #ifdef APP_ATT_SERVER_ENABLE
        case UART_GATT_CMD_SEND_ERR_RESP:
        {
            uint16_t connHandle;
            GATTS_SendErrRespParams_T *p_respParams;

            p_respParams = (GATTS_SendErrRespParams_T *)OSAL_Malloc(sizeof(GATTS_SendErrRespParams_T));
            if (p_respParams != NULL)
            {
                STREAM_BE_TO_U16(&connHandle, &p_bufTmp);
                STREAM_TO_U8(&p_respParams->reqOpcode, &p_bufTmp);
                STREAM_BE_TO_U16(&p_respParams->attrHandle, &p_bufTmp);
                STREAM_TO_U8(&p_respParams->errorCode, &p_bufTmp);
                status = GATTS_SendErrorResponse(connHandle, p_respParams);
                OSAL_Free((uint8_t *)p_respParams);
            }
            else
            {
                status = APP_RES_OOM;
            }
        }
        break;
        #endif

        #ifdef APP_ATT_SERVER_ENABLE
        case UART_GATT_CMD_SEND_READ_BY_TYPE_RESP:
        {
            uint16_t connHandle;
            GATTS_SendReadByTypeRespParams_T *p_respParams;

            p_respParams = (GATTS_SendReadByTypeRespParams_T *)OSAL_Malloc(sizeof(GATTS_SendReadByTypeRespParams_T));
            if (p_respParams != NULL)
            {
                STREAM_BE_TO_U16(&connHandle, &p_bufTmp);
                STREAM_TO_U8(&p_respParams->eachPairLength, &p_bufTmp);
                STREAM_BE_TO_U16(&p_respParams->allPairsLength, &p_bufTmp);
                STREAM_COPY_TO_VARIABLE(p_respParams->attrDataList, &p_bufTmp, p_respParams->allPairsLength);
                status = GATTS_SendReadByTypeResponse(connHandle, p_respParams);
                OSAL_Free((uint8_t *)p_respParams);
            }
            else
            {
                status = APP_RES_OOM;
            }
        }
        break;
        #endif

        #ifdef APP_ATT_SERVER_ENABLE
        case UART_GATT_CMD_SEND_WRITE_RESP:
        {
            uint16_t connHandle;
            GATTS_SendWriteRespParams_T *p_respParams;

            p_respParams = (GATTS_SendWriteRespParams_T *)OSAL_Malloc(sizeof(GATTS_SendWriteRespParams_T));
            if (p_respParams != NULL)
            {
                STREAM_BE_TO_U16(&connHandle, &p_bufTmp);
                STREAM_TO_U8(&p_respParams->responseType, &p_bufTmp);
                STREAM_BE_TO_U16(&p_respParams->attrHandle, &p_bufTmp);
                STREAM_BE_TO_U16(&p_respParams->valueOffset, &p_bufTmp);
                STREAM_BE_TO_U16(&p_respParams->writeLength, &p_bufTmp);
                if (p_respParams->writeLength)
                {
                    STREAM_COPY_TO_VARIABLE(p_respParams->writeValue, &p_bufTmp, p_respParams->writeLength);
                }
                status = GATTS_SendWriteResponse(connHandle, p_respParams);
                OSAL_Free((uint8_t *)p_respParams);
            }
            else
            {
                status = APP_RES_OOM;
            }
        }
        break;
        #endif

        #ifdef APP_ATT_SERVER_ENABLE
        case UART_GATT_CMD_SERVICE_CHANGE:
        {
            status = GATTS_ServiceChanged();
        }
        break;
        #endif

        #ifdef APP_ATT_SERVER_ENABLE
        /* test command for service change */
        case UART_GATT_CMD_ADD_SERVICE:
        {
            /* Add PTS test 2 service for service change example */
            //status = APP_AddPtsSvc2();
        }
        break;
        #endif

        #ifdef APP_ATT_SERVER_ENABLE
        /* test command for service change */
        case UART_GATT_CMD_REMOVE_SERVICE:
        {
            /* Remove PTS test 2 service for service change example */
            //status = APP_RemovePtsSvc2();
        }
        break;
        #endif

        #ifdef APP_ATT_SERVER_ENABLE
        case UART_GATT_CMD_GET_ATTRIBUTE_VALUE:
        {
            uint16_t attrHandle, attrLength;
            uint8_t  attrValue[BLE_ATT_MAX_MTU_LEN];

            STREAM_BE_TO_U16(&attrHandle, &p_bufTmp);
            attrLength = BLE_ATT_MAX_MTU_LEN;
            status = GATTS_GetHandleValue(attrHandle, attrValue, &attrLength);
            if (status == APP_RES_SUCCESS)
            {
                eventLength = attrLength;
                p_eventBuf = attrValue;
            }
        }
        break;
        #endif

        #ifdef APP_EATT_TEST
        case UART_GATT_CMD_READ_MULTI_VARIABLE:
        {
            uint8_t attId, setOfHandle, handles[10];

            STREAM_TO_U8(&attId, &p_bufTmp);
            STREAM_TO_U8(&setOfHandle, &p_bufTmp);
            memcpy(handles, p_bufTmp, setOfHandle*2);
            status = GATTC_ReadMultipleVariable(attId, setOfHandle, handles);
        }
        break;

        case UART_GATT_CMD_MULTI_HANDLE_VALUE_NTF:
        {
            uint16_t attrHandle;
            uint8_t attId, valueLen, value[20];

            STREAM_TO_U8(&attId, &p_bufTmp);
            STREAM_BE_TO_U16(&attrHandle, &p_bufTmp);
            STREAM_TO_U8(&valueLen, &p_bufTmp);
            if(valueLen)
            {
                memcpy(value, p_bufTmp, valueLen);
            }
            status = GATTS_SendMultipleHandleValue(attId, attrHandle, valueLen, value);
        }
        break;
        #endif

        default:
        break;
    }
    APP_SendCmdCompleteEvt(eventLength, UART_GROUP_GATT, opcode, status, p_eventBuf);
}

static void APP_PrepareGapCmdCompl(uint8_t cmdId)
{
    uint8_t *p_buf;
    uint8_t *p_packet;
    uint8_t evt_len = BLE_GAP_EVT_SIZE_COMMAND_COMPLETE+3;

    p_packet = OSAL_Malloc(evt_len);
    if(p_packet==NULL)
        return;
    p_buf = p_packet;
    memset(p_packet, 0, evt_len);

    U8_TO_STREAM(&p_buf, UART_EVENT_TEST);
    U8_TO_STREAM(&p_buf, UART_GROUP_BLE_GAP);
    U8_TO_STREAM(&p_buf, BLE_GAP_EVT_COMMAND_COMPLETE);
    switch(cmdId) 
    {
        case UART_BLE_GAP_CMD_SET_ADV_DATA://0x4
        {
            U8_TO_STREAM(&p_buf, BLE_GAP_CMD_COMPLETE_SET_ADVERTISING_DATA);
        }
        break;
        case UART_BLE_GAP_CMD_SET_ADV_PARAM://0x6
        {
            U8_TO_STREAM(&p_buf, BLE_GAP_CMD_COMPLETE_SET_ADVERTISING_PARAMS);
        }
        break;
        case UART_BLE_GAP_CMD_ADV_ENABLE://0x07
        {
            U8_TO_STREAM(&p_buf, BLE_GAP_CMD_COMPLETE_SET_ADVERTISING_ENABLE);
        }
        break;
        case UART_BLE_GAP_CMD_CLEAR_WHITE_LIST://0x0C
        {
            U8_TO_STREAM(&p_buf, BLE_GAP_CMD_COMPLETE_CLEAR_WHITE_LIST);
        }
        break;
        case UART_BLE_GAP_CMD_SET_WHITE_LIST://0x0D
        {
            U8_TO_STREAM(&p_buf, BLE_GAP_CMD_COMPLETE_SET_WHITE_LIST);
        }
        break;
        case UART_BLE_GAP_CMD_SET_SCANNING_PARAM://0x15
        {
            U8_TO_STREAM(&p_buf, BLE_GAP_CMD_COMPLETE_SET_SCAN_PARAMS);
        }
        break;
        case UART_BLE_GAP_CMD_SET_SCANNING_ENABLE://0x16
        {
            U8_TO_STREAM(&p_buf, BLE_GAP_CMD_COMPLETE_SET_SCAN_ENABLE);
        }
        break;
        
        default:
            evt_len=0;
        break;
    }
    
    if(evt_len)
        APP_PackageUartEvent(evt_len, p_packet);
    OSAL_Free(p_packet);
}

void APP_ProcBleGapCmd(uint8_t *p_cmdPayload, uint16_t len)
{
    uint16_t result = APP_RES_SUCCESS,  additionalLen = 0;;
    uint8_t *p_additional = NULL;

    switch (*(p_cmdPayload))
    {
        case UART_BLE_GAP_CMD_SET_ADDR:
        {
            result = BLE_GAP_SetDeviceAddr((BLE_GAP_Addr_T *)(p_cmdPayload + 1));
        }
        break;

        case UART_BLE_GAP_CMD_GET_ADDR:
        {
            BLE_GAP_Addr_T addr;
            uint8_t *p_buf;
            
            additionalLen=7;
            p_additional=OSAL_Malloc(additionalLen);
            if(p_additional==NULL)
            {
                result=APP_RES_OOM;
                break;
            }
            p_buf=p_additional;
            
            result = BLE_GAP_GetDeviceAddr(&addr);

            U8_TO_STREAM(&p_buf, addr.addrType);
            memcpy(p_buf, addr.addr, 6); 
        }
        break;

        case UART_BLE_GAP_CMD_GET_DEV_NAME:
        {
            uint8_t nameLen;
            p_additional=OSAL_Malloc(GAP_MAX_DEVICE_NAME_LEN);
            if(p_additional==NULL)
            {
                result=APP_RES_OOM;
                break;
            }

            BLE_GAP_GetDeviceName(&nameLen, p_additional);

            additionalLen=nameLen;
        }
        break;

        case UART_BLE_GAP_CMD_SET_DEV_NAME:
        {
            uint8_t len = *(p_cmdPayload + 1);
            uint8_t *p_deviceName = (p_cmdPayload + 2);
            result = BLE_GAP_SetDeviceName(len, p_deviceName);
        }
        break;

        #ifdef APP_GAP_ADV_ENABLE
        case UART_BLE_GAP_CMD_SET_ADV_DATA:
        {
            BLE_GAP_AdvDataParams_T advData;
            advData.advLen = len - 1;
            memcpy(advData.advData, p_cmdPayload + 1, advData.advLen);

            result = BLE_GAP_SetAdvData(&advData);
        }
        break;
        #endif

        #ifdef APP_GAP_ADV_ENABLE
        case UART_BLE_GAP_CMD_SET_SCAN_RESP_DATA:
        {
            BLE_GAP_AdvDataParams_T scanData;
            scanData.advLen = len - 1;
            memcpy(scanData.advData, p_cmdPayload + 1, scanData.advLen);

            result = BLE_GAP_SetScanRspData(&scanData);
        }
        break;
        #endif

        #ifdef APP_GAP_ADV_ENABLE
        case UART_BLE_GAP_CMD_SET_ADV_PARAM:
        {
            uint8_t *p_bufTmp = (p_cmdPayload + 1);
            BLE_GAP_AdvParams_T params;
            uint8_t ownAt;

            STREAM_BE_TO_U16(&params.intervalMin, &p_bufTmp);
            STREAM_BE_TO_U16(&params.intervalMax, &p_bufTmp);
            STREAM_TO_U8(&params.type, &p_bufTmp);
            STREAM_TO_U8(&ownAt, &p_bufTmp);
            STREAM_TO_U8(&params.peerAddr.addrType, &p_bufTmp);
            STREAM_COPY_TO_VARIABLE(params.peerAddr.addr, &p_bufTmp, GAP_MAX_BD_ADDRESS_LEN);
            STREAM_TO_U8(&params.advChannelMap, &p_bufTmp);
            STREAM_TO_U8(&params.filterPolicy, &p_bufTmp);

            result = BLE_GAP_SetAdvParams(&params);
        }
        break;
        #endif

        #ifdef APP_GAP_ADV_ENABLE
        case UART_BLE_GAP_CMD_ADV_ENABLE:
        {
            uint8_t enable = *(p_cmdPayload + 1);
            result = BLE_GAP_SetAdvEnable(enable, 0);
        }
        break;
        #endif

        #if(defined APP_GAP_CENTRAL_ENABLE || defined APP_GAP_PERIPHERAL_ENABLE)
        case UART_BLE_GAP_CMD_TERMINATE_CONNECTION:
        {
            uint8_t *p_bufTmp = (p_cmdPayload + 1);
            uint8_t reason;
            uint16_t connHandle;

            STREAM_BE_TO_U16(&connHandle, &p_bufTmp);
            STREAM_TO_U8(&reason, &p_bufTmp);

            BLE_GAP_Disconnect(connHandle, reason);
        }
        break;
        #endif

        #if(defined APP_GAP_CENTRAL_ENABLE || defined APP_GAP_PERIPHERAL_ENABLE)
        case UART_BLE_GAP_CMD_UPDATE_CONN_PARA:
        {
            uint8_t *p_bufTmp = (p_cmdPayload + 1);
            uint16_t connHandle;
            BLE_GAP_ConnParams_T params;

            STREAM_BE_TO_U16(&connHandle, &p_bufTmp);
            STREAM_BE_TO_U16(&params.intervalMin, &p_bufTmp);
            STREAM_BE_TO_U16(&params.intervalMax, &p_bufTmp);
            STREAM_BE_TO_U16(&params.latency, &p_bufTmp);
            STREAM_BE_TO_U16(&params.supervisionTimeout, &p_bufTmp);

            result = BLE_GAP_UpdateConnParam(connHandle, &params);
        }
        break;
        #endif

        #if(defined APP_GAP_CENTRAL_ENABLE || defined APP_GAP_PERIPHERAL_ENABLE)
        case UART_BLE_GAP_CMD_GET_RSSI:
        {
            uint8_t *p_bufTmp = (p_cmdPayload + 1);
            uint16_t connHandle;

            additionalLen=1;
            p_additional=OSAL_Malloc(additionalLen);
            if(p_additional==NULL)
            {
                result=APP_RES_OOM;
                break;
            }

            STREAM_BE_TO_U16(&connHandle, &p_bufTmp);
            result = BLE_GAP_GetRssi(connHandle, (int8_t *)p_additional);
        }
        break;
        #endif

        case UART_BLE_GAP_CMD_RESERVED_1:
        {

        }
        break;

        case UART_BLE_GAP_CMD_CLEAR_WHITE_LIST:
        {
            result = BLE_GAP_SetFilterAcceptList(0, NULL);
        }
        break;

        case UART_BLE_GAP_CMD_SET_WHITE_LIST:
        {
            uint8_t *p_bufTmp = (p_cmdPayload + 1);
            BLE_GAP_Addr_T params;

            STREAM_TO_U8(&params.addrType, &p_bufTmp);
            STREAM_COPY_TO_VARIABLE(&params.addr, &p_bufTmp, GAP_MAX_BD_ADDRESS_LEN);

            result = BLE_GAP_SetFilterAcceptList(1, &params);
        }
        break;

        case UART_BLE_GAP_CMD_RESERVED_2:
        {

        }
        break;

        case UART_BLE_GAP_CMD_SET_RESOLVING_LIST:
        {
            uint8_t *p_bufTmp = (p_cmdPayload + 1);
            BLE_GAP_ResolvingListParams_T params;

            STREAM_TO_U8(&params.peerIdAddr.addrType, &p_bufTmp);
            STREAM_COPY_TO_VARIABLE(&params.peerIdAddr.addr, &p_bufTmp, 6);
            STREAM_COPY_TO_VARIABLE(&params.peerIrk, &p_bufTmp, 16);
            STREAM_COPY_TO_VARIABLE(&params.localIrk, &p_bufTmp, 16);
            STREAM_TO_U8(&params.privacyMode, &p_bufTmp);
            result = BLE_GAP_SetResolvingList(1, &params);
        }
        break;

        case UART_BLE_GAP_CMD_CLEAR_RESOLVING_LIST:
        {
            result=BLE_GAP_SetResolvingList(0 ,NULL);
        }
        break;

        case UART_BLE_GAP_CMD_SET_PRIVACY:
        {
            uint8_t *p_bufTmp = (p_cmdPayload + 1);
            BLE_GAP_LocalPrivacyParams_T privacyParams;

            STREAM_TO_U8(&privacyParams.privacyAddrType, &p_bufTmp);
            STREAM_TO_U8(&privacyParams.enable, &p_bufTmp);
            STREAM_BE_TO_U16(&privacyParams.addrTimeout, &p_bufTmp);
            memcpy(privacyParams.localIrk, p_bufTmp, 16);

            APP_ADV_Stop();

            result = BLE_GAP_SetLocalPrivacy(&privacyParams);

            APP_ADV_Start();
        }
        break;

        case UART_BLE_GAP_CMD_GET_PRIVACY:
        {
            BLE_GAP_LocalPrivacyParams_T privacyParams;
            uint8_t *p_buf;

            additionalLen=20;
            p_additional=OSAL_Malloc(additionalLen);
            if(p_additional==NULL)
            {
                result=APP_RES_OOM;
                break;
            }

            p_buf=p_additional;

            result = BLE_GAP_GetLocalPrivacy(&privacyParams);

            U8_TO_STREAM(&p_buf, privacyParams.privacyAddrType);
            U8_TO_STREAM(&p_buf, privacyParams.enable);
            U16_TO_STREAM_BE(&p_buf, privacyParams.addrTimeout);
            memcpy(p_buf, privacyParams.localIrk, 16);
        }
        break;

        #ifdef APP_GAP_CENTRAL_ENABLE
        case UART_BLE_GAP_CMD_CREATE_CONNECTION:
        {
            uint8_t *p_bufTmp = (p_cmdPayload + 1);
            BLE_GAP_CreateConnParams_T createConnParams;

            createConnParams.filterPolicy = BLE_GAP_INIT_FP_FILTER_ACCEPT_LIST_NOT_USED;
            createConnParams.scanInterval = 0x0020;
            createConnParams.scanWindow = 0x0020;
            #if 0
            createConnParams.connParams.intervalMax = 0x0006;
            createConnParams.connParams.intervalMin = 0x0006;
            createConnParams.connParams.latency = 0x0000;
            createConnParams.connParams.supervisionTimeout = 0x0018;
            #else
            createConnParams.connParams.intervalMax = 0x000c;
            createConnParams.connParams.intervalMin = 0x000c;
            createConnParams.connParams.latency = 0x0000;
            createConnParams.connParams.supervisionTimeout = 0x0100;
            #endif

            STREAM_TO_U8(&createConnParams.peerAddr.addrType, &p_bufTmp);
            STREAM_COPY_TO_VARIABLE(createConnParams.peerAddr.addr, &p_bufTmp, GAP_MAX_BD_ADDRESS_LEN);

            result = BLE_GAP_CreateConnection(&createConnParams);
        }
        break;
        #endif

        #ifdef APP_GAP_CENTRAL_ENABLE
        case UART_BLE_GAP_CMD_CREATE_CONNECTION_CANCEL:
        {
            result = BLE_GAP_CreateConnectionCancel();
        }
        break;
        #endif

        #ifdef APP_GAP_SCAN_ENABLE
        case UART_BLE_GAP_CMD_SET_SCANNING_PARAM:
        {
            uint8_t *p_bufTmp = (p_cmdPayload + 1);
            BLE_GAP_ScanningParams_T params;
            uint8_t ownAddrType;

            STREAM_TO_U8(&params.type, &p_bufTmp);
            STREAM_BE_TO_U16(&params.interval, &p_bufTmp);
            STREAM_BE_TO_U16(&params.window, &p_bufTmp);
            STREAM_TO_U8(&ownAddrType, &p_bufTmp);
            STREAM_TO_U8(&params.filterPolicy, &p_bufTmp);

            result = BLE_GAP_SetScanningParam(&params);
        }
        break;
        #endif

        #ifdef APP_GAP_SCAN_ENABLE
        case UART_BLE_GAP_CMD_SET_SCANNING_ENABLE:
        {
            uint8_t *p_bufTmp = (p_cmdPayload + 1);
            uint8_t enable;
            uint8_t filterDuplicate;
            uint8_t mode;

            STREAM_TO_U8(&enable, &p_bufTmp);
            STREAM_TO_U8(&filterDuplicate, &p_bufTmp);
            STREAM_TO_U8(&mode, &p_bufTmp);

            result = BLE_GAP_SetScanningEnable(enable, filterDuplicate, mode, 0);
        }
        break;
        #endif

        #if (defined APP_GAP_SCAN_ENABLE) || (defined APP_GAP_EXT_SCAN_ENABLE)
        case UART_BLE_GAP_CMD_SET_SCANNING_FILTER:
        {
            BLE_GAP_ScanFilterConfig_T param;

            memset(&param, 0, sizeof(BLE_GAP_ScanFilterConfig_T));

            switch (*(p_cmdPayload +1))
            {
                case UART_BLE_SCAN_NAME_FILTER_CMD_SET:
                {
                    BLE_GAP_ScanFilterName_T  nameFilter;
                    char *name = "TWS";

                    nameFilter.nameLen = 3;
                    param.nameFilterEnable = 1;
                    memcpy((char *)nameFilter.targetName, name, 3);

                    memcpy(&param.nameFilter, &nameFilter, sizeof(BLE_GAP_ScanFilterName_T));
                }
                break;
                case UART_BLE_SCAN_SERVICE_DATA_FILTER_CMD_SET:
                {
                    BLE_GAP_ScanFilterServiceData_T serviceDataFilter;
                    char uuid[] = {0xDA ,0xFE, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17}; 
                    char data[] = {0x80, 0x81, 0x82, 0x83};

                    param.svcDataFilterEnable = 1;
                    serviceDataFilter.uuidLen = 4;
                    memcpy(serviceDataFilter.uuidData, uuid, serviceDataFilter.uuidLen);

                    serviceDataFilter.svcDataLen = sizeof(data);
                    memcpy(serviceDataFilter.svcData, data, serviceDataFilter.svcDataLen);

                    memcpy(&param.serviceDataFilter, &serviceDataFilter, sizeof(BLE_GAP_ScanFilterServiceData_T));
                }
                break;
                case UART_BLE_SCAN_MSD_FILTER_CMD_SET:
                {
                    BLE_GAP_ScanFilterMsd_T msdFilter;
                    char companyId[] = {0x12, 0x34};
                    char data[] = {0x80, 0x81, 0x82, 0x83};

                    param.msdFilterEnable = 1;
                    memcpy(msdFilter.companyId, companyId, 2);

                    msdFilter.msdLen = sizeof(data);
                    memcpy(msdFilter.msd, data, sizeof(data));

                    memcpy(&param.msdFilter, &msdFilter, sizeof(BLE_GAP_ScanFilterMsd_T));
                }
                break;
                case UART_BLE_SCAN_DISABLE_FILTER_CMD_SET:
                {
                    param.msdFilterEnable = 0;
                    param.svcDataFilterEnable = 0;
                    param.msdFilterEnable = 0;
                }
                break;
            }

            result = BLE_GAP_SetScanningFilter(&param);
            if (result == MBA_RES_SUCCESS)
            {
                BLE_GAP_ScanningParams_T    scanParam;

                scanParam.type = BLE_GAP_SCAN_TYPE_PASSIVE_SCAN;
                scanParam.interval = 160;
                scanParam.window   = 32;
                scanParam.filterPolicy = BLE_GAP_SCAN_FP_ACCEPT_ALL;
                BLE_GAP_SetScanningParam(&scanParam);

                result = BLE_GAP_SetScanningEnable(true, BLE_GAP_SCAN_FD_ENABLE, BLE_GAP_SCAN_MODE_OBSERVER, 0);
            }
        }
        break;
        #endif

        case UART_BLE_GAP_CMD_SET_CHANNEL_MAP:
        {
            uint8_t *p_map = (p_cmdPayload + 1);
            result = BLE_GAP_SetChannelMap(p_map);
        }
        break;

        #ifdef APP_GAP_TEST_ENABLE
        case UART_BLE_GAP_CMD_END_TEST:
        {
            uint16_t packetCount;
            result = BLE_GAP_EndTest(&packetCount);
        }
        break;
        #endif

        case UART_BLE_GAP_CMD_RESERVED_3:
        {

        }
        break;

        #ifdef APP_GAP_CENTRAL_ENABLE
        case UART_BLE_GAP_CMD_START_ENCRYPTION:
        {
            APP_PairingDevice_T *p_device;
            uint8_t devIndex;

            devIndex=APP_GetPairedDeviceIndex(&g_appConnList[0].remoteAddr);
            if(devIndex!=APP_MAX_PAIRED_DEVICE_NUM)
            {
                p_device = (APP_PairingDevice_T *)OSAL_Malloc(sizeof(APP_PairingDevice_T));
                if (p_device != NULL)
                {
                    APP_GetPairedDeviceInfo(devIndex, p_device);
                    result = BLE_GAP_EnableEncryption(g_appConnList[0].connHandle, p_device->rand, p_device->ediv, p_device->bleLinkKey);
                    OSAL_Free(p_device);
                }
            }
        }
        break;
        #endif

        #ifdef APP_GAP_EXT_ADV_ENABLE
        case UART_BLE_GAP_CMD_SET_EXT_ADV_PARAM:
        {
            uint8_t *p_bufTmp = (p_cmdPayload + 1);
            int8_t selectedTxPower;
            uint8_t ownAddrType;
            BLE_GAP_ExtAdvParams_T params;

            STREAM_TO_U8(&params.advHandle, &p_bufTmp);
            STREAM_BE_TO_U16(&params.evtProperies, &p_bufTmp);
            STREAM_COPY_TO_VARIABLE(&params.priIntervalMin, &p_bufTmp, 3);
            STREAM_COPY_TO_VARIABLE(&params.priIntervalMax, &p_bufTmp, 3);
            STREAM_TO_U8(&params.priChannelMap, &p_bufTmp);
            STREAM_TO_U8(&ownAddrType, &p_bufTmp);
            STREAM_TO_U8(&params.peerAddr.addrType, &p_bufTmp);
            STREAM_COPY_TO_VARIABLE(params.peerAddr.addr, &p_bufTmp, GAP_MAX_BD_ADDRESS_LEN);
            STREAM_TO_U8(&params.filterPolicy, &p_bufTmp);
            STREAM_TO_U8(&params.txPower, &p_bufTmp);
            STREAM_TO_U8(&params.priPhy, &p_bufTmp);
            STREAM_TO_U8(&params.secMaxSkip, &p_bufTmp);
            STREAM_TO_U8(&params.secPhy, &p_bufTmp);
            STREAM_TO_U8(&params.sid, &p_bufTmp);
            STREAM_TO_U8(&params.scanReqNotifiEnable, &p_bufTmp);

            result = BLE_GAP_SetExtAdvParams(&params, &selectedTxPower);
        }
        break;
        #endif

        #ifdef APP_GAP_EXT_ADV_ENABLE
        case UART_BLE_GAP_CMD_SET_EXT_ADV_DATA:
        {
            uint8_t *p_bufTmp = (p_cmdPayload + 3);
            BLE_GAP_ExtAdvDataParams_T params;

            STREAM_TO_U8(&params.advHandle, &p_bufTmp);
            STREAM_TO_U8(&params.operation, &p_bufTmp);
            STREAM_TO_U8(&params.fragPreference, &p_bufTmp);
            STREAM_TO_U8(&params.advLen, &p_bufTmp);
            if(params.advLen)
            {
                params.p_advData=OSAL_Malloc(params.advLen);
                if(params.p_advData)
                {
                    STREAM_COPY_TO_VARIABLE(params.p_advData, &p_bufTmp, params.advLen);
                }
            }

            result = BLE_GAP_SetExtAdvData(&params);
            OSAL_Free(params.p_advData);
        }
        break;
        #endif

        #ifdef APP_GAP_EXT_ADV_ENABLE
        case UART_BLE_GAP_CMD_SET_EXT_SCAN_RESP_DATA:
        {
            BLE_GAP_ExtAdvDataParams_T params;
            uint8_t *p_bufTmp = (p_cmdPayload + 3);

            STREAM_TO_U8(&params.advHandle, &p_bufTmp);
            STREAM_TO_U8(&params.operation, &p_bufTmp);
            STREAM_TO_U8(&params.fragPreference, &p_bufTmp);
            STREAM_TO_U8(&params.advLen, &p_bufTmp);
            if(params.advLen)
            {
                params.p_advData=OSAL_Malloc(params.advLen);
                if(params.p_advData)
                {
                    STREAM_COPY_TO_VARIABLE(params.p_advData, &p_bufTmp, params.advLen);
                }
            }
            
            result = BLE_GAP_SetExtScanRspData(&params);
            OSAL_Free(params.p_advData);
        }
        break;
        #endif

        #ifdef APP_GAP_EXT_ADV_ENABLE
        case UART_BLE_GAP_CMD_SET_EXT_ADV_ENABLE:
        {
            uint8_t *p_bufTmp = (p_cmdPayload + 3);
            BLE_GAP_ExtAdvEnableParams_T params;

            STREAM_TO_U8(&params.advHandle, &p_bufTmp);
            STREAM_BE_TO_U16(&params.duration, &p_bufTmp);
            STREAM_TO_U8(&params.maxExtAdvEvts, &p_bufTmp);

            result = BLE_GAP_SetExtAdvEnable(*(p_cmdPayload + 1), *(p_cmdPayload + 2), &params);
        }
        break;
        #endif
        
        #ifdef APP_GAP_EXT_ADV_ENABLE
        case UART_BLE_GAP_CMD_SET_PERIODIC_ADV_PARAM:
        {
            uint8_t *p_bufTmp = (p_cmdPayload + 1);
            BLE_GAP_PeriAdvParams_T params;

            STREAM_TO_U8(&params.advHandle, &p_bufTmp);
            STREAM_BE_TO_U16(&params.intervalMin, &p_bufTmp);
            STREAM_BE_TO_U16(&params.intervalMax, &p_bufTmp);
            STREAM_BE_TO_U16(&params.properties, &p_bufTmp);

            result = BLE_GAP_SetPeriAdvParams(&params);
        }
        break;
        #endif
        
        #ifdef APP_GAP_EXT_ADV_ENABLE
        case UART_BLE_GAP_CMD_SET_PERIODIC_ADV_DATA:
        {
            uint8_t *p_bufTmp = (p_cmdPayload + 3);
            BLE_GAP_PeriAdvDataParams_T params;

            STREAM_TO_U8(&params.advHandle, &p_bufTmp);
            STREAM_TO_U8(&params.operation, &p_bufTmp);
            STREAM_TO_U8(&params.advLen, &p_bufTmp);
            if(params.advLen)
            {
                params.p_advData=OSAL_Malloc(params.advLen);
                if(params.p_advData)
                {
                    STREAM_COPY_TO_VARIABLE(params.p_advData, &p_bufTmp, params.advLen);
                }
            }

            result = BLE_GAP_SetPeriAdvData(&params);
            OSAL_Free(params.p_advData);
        }
        break;
        #endif

        #ifdef APP_GAP_EXT_ADV_ENABLE
        case UART_BLE_GAP_CMD_SET_PERIODIC_ADV_ENABLE:
        {
            result = BLE_GAP_SetPeriAdvEnable(*(p_cmdPayload + 1), *(p_cmdPayload + 2));
        }
        break;
        #endif

        #ifdef APP_GAP_EXT_SCAN_ENABLE
        case UART_BLE_GAP_CMD_SET_EXT_SCAN_PARAM:
        {
            uint8_t *p_bufTmp = (p_cmdPayload + 1);
            uint8_t ownAddrType, filterPolicy, phy;
            BLE_GAP_ExtScanningPhy_T params;

            STREAM_TO_U8(&ownAddrType, &p_bufTmp);
            STREAM_TO_U8(&filterPolicy, &p_bufTmp);
            STREAM_TO_U8(&phy, &p_bufTmp);

            if (phy & 0x01)//1M PHY
            {
                params.le1mPhy.enable = true;
                params.leCodedPhy.enable = false;
                STREAM_TO_U8(&params.le1mPhy.type, &p_bufTmp);
                STREAM_BE_TO_U16(&params.le1mPhy.interval, &p_bufTmp);
                STREAM_BE_TO_U16(&params.le1mPhy.window, &p_bufTmp);
            }

            if (phy & 0x04)//Codec PHY
            {
                params.le1mPhy.enable = false;
                params.leCodedPhy.enable = true;
                STREAM_TO_U8(&params.leCodedPhy.type, &p_bufTmp);
                STREAM_BE_TO_U16(&params.leCodedPhy.interval, &p_bufTmp);
                STREAM_BE_TO_U16(&params.leCodedPhy.window, &p_bufTmp);
            }

            result = BLE_GAP_SetExtScanningParams(filterPolicy, &params);
        }
        break;
        #endif
        
        #ifdef APP_GAP_EXT_SCAN_ENABLE
        case UART_BLE_GAP_CMD_SET_EXT_SCAN_ENABLE:
        {
            uint8_t *p_bufTmp = (p_cmdPayload + 2);
            BLE_GAP_ExtScanningEnable_T params;

            STREAM_TO_U8(&params.enable, &p_bufTmp);
            STREAM_TO_U8(&params.filterDuplicates, &p_bufTmp);
            STREAM_BE_TO_U16(&params.duration, &p_bufTmp);
            STREAM_BE_TO_U16(&params.period, &p_bufTmp);

            result = BLE_GAP_SetExtScanningEnable(*(p_cmdPayload + 1), &params);
        }
        break;
        #endif

        default:
        {
            result=APP_RES_FAIL;
        }
        break;
    }
    APP_SendCmdCompleteEvt(additionalLen, UART_GROUP_BLE_GAP, *(p_cmdPayload), result, p_additional);
    if(p_additional!=NULL)
        OSAL_Free(p_additional);
    
    APP_PrepareGapCmdCompl(*(p_cmdPayload));
}

void APP_ProcBleSmpCmd(uint8_t *p_cmdPayload)
{
    uint16_t result;
    uint16_t connHandle;
    uint8_t *p_evt_buf = NULL;
    uint8_t evt_len = 0;
    BLE_SMP_Config_T config;

    result = APP_RES_SUCCESS;

    connHandle = APP_GetCurrentConnHandle();

    if (connHandle == 0)
    {
        connHandle = ((*(p_cmdPayload + 1) << 8) + *(p_cmdPayload + 2));
    }

    switch (*(p_cmdPayload))
    {
        case UART_BLE_SMP_CMD_CONFIG:
        {
            #ifndef APP_PAIRING_ENABLE
            config.ioCapability = *(p_cmdPayload + 1);
            config.authReqFlag = *(p_cmdPayload + 2);
            /* for compatible current automation test */
            config.oobDataFlag = false;//*(p_cmdPayload + 3);
            config.scOnly = false;
            #else
            memset(&config, 0, sizeof(BLE_SMP_Config_T));
            config.ioCapability = *(p_cmdPayload + 1);

            if (config.ioCapability != BLE_SMP_IO_NOINPUTNOOUTPUT)
            {
                config.authReqFlag |= BLE_SMP_OPTION_MITM;
            }
            config.authReqFlag |=  BLE_SMP_OPTION_BONDING | BLE_SMP_OPTION_SECURE_CONNECTION;
            config.oobDataFlag = false;
            config.scOnly = false;
            #endif

            result = BLE_SMP_Config(&config);

            g_appDb.ioCapability=*(p_cmdPayload + 1);
        }
        break;
        case UART_BLE_SMP_CMD_PASSKEY_REPLY:
        {
            result = BLE_SMP_PasskeyReply(connHandle, p_cmdPayload + 3);
        }
        break;
        case UART_BLE_SMP_CMD_PASSKEY_NEG_REPLY:
        {
            result = BLE_SMP_PasskeyNegativeReply(connHandle);
        }
        break;
        case UART_BLE_SMP_CMD_NUM_COMP_CONF_REPLY:
        {
            result = BLE_SMP_NumericComparisonConfirmReply(connHandle, *(p_cmdPayload + 3));
        }
        break;
        case UART_BLE_SMP_CMD_INIT_PAIR:
        {
            result = BLE_SMP_InitiatePairing(connHandle);
        }
        break;
        case UART_BLE_SMP_CMD_GEN_PASSKEY:
        {
            p_evt_buf = OSAL_Malloc(6);

            if (p_evt_buf)
            {
                BLE_SMP_GeneratePasskey(p_evt_buf);
                evt_len = 6;
            }
            else
                result = APP_RES_OOM;
        }
        break;
        case UART_BLE_SMP_CMD_OOB_DATA_REPLY:
        {
            uint8_t oob[16];

            BUF_TO_VARIABLE(oob, p_cmdPayload + 3, 16);
            result = BLE_SMP_OobDataReply(connHandle, oob);
        }
        break;
        case UART_BLE_SMP_CMD_ENABLE_DEBUG_MODE:
        {
            BLE_SMP_EnableDebugMode();
        }
        break;

#if 0
        case UART_BLE_SMP_CMD_GEN_OOB_DATA:
        {
            uint8_t r[16], c[16];

            p_evt_buf = OSAL_Malloc(16 + 16);
            if (p_evt_buf)
            {
                result = BLE_SMP_GenerateScOobData(c, r);

                VARIABLE_TO_BUF(p_evt_buf, c, 16);
                VARIABLE_TO_BUF(p_evt_buf + 16, r, 16);

                evt_len = 16 + 16;
            }
            else
                result = APP_RES_OOM;
        }
        break;
        case UART_BLE_SMP_CMD_SET_LESC_OOB_DATA:
        {
            BUF_TO_VARIABLE(g_appDb.oobConfirm, p_cmdPayload + 1, 16);
            BUF_TO_VARIABLE(g_appDb.oobRandom, p_cmdPayload + 17, 16);
        }
        break;
#endif

#ifdef APP_PAIRING_ENABLE
        case UART_BLE_SMP_CMD_SET_TRIGGER_PAIR:
        {
            APP_SetPairTrigger(*(p_cmdPayload + 3));
        }
        break;

        case UART_BLE_SMP_CMD_ENTER_EXIT_TDC_MODE:
        {
            if (*(p_cmdPayload + 3))   //Enter Trusted Device Connectable mode
            {
                //Disallow to enter TDC mode if the connected device number reaches max.
                if ((g_trpClientLinkNum >= APP_TRPC_MAX_LINK_NUMBER) || (g_trpServerLinkNum >= APP_TRPS_MAX_LINK_NUMBER) ||
                    APP_TRP_IS_CONN_MAX_DEV)
                {
                    result = MBA_RES_FAIL;
                }
                else
                {
                    result = APP_ProcEnterTdcMode();
                }
            }
            else                       //Exit Trusted Device Connectable mode
            {
                bool needStopAdv = true;

#ifdef APP_BLE_MULTI_LINK_ENABLE
                if (APP_GetAdvConnList() == NULL)  //Check if adv. is enabled or not
                {
                    needStopAdv = false;
                }
#endif

                if (needStopAdv)
                {
                    APP_ADV_Stop();
                }

                APP_ADV_SetFilterPolicy(BLE_GAP_ADV_FILTER_DEFAULT);

                if (needStopAdv)
                {
                    APP_ADV_Start();
                }

            }
        }
        break;
#endif

        case UART_BLE_SMP_CMD_KEYPRESS:
        {
            result = BLE_SMP_Keypress(connHandle, *(p_cmdPayload + 3));
        }
        break;

        default:
        {
            result = APP_RES_INVALID_PARA;
        }
        break;
    }

    APP_SendCmdCompleteEvt(evt_len, UART_GROUP_BLE_SMP, *(p_cmdPayload), result, p_evt_buf);

    if (p_evt_buf)
        OSAL_Free(p_evt_buf);
}

void APP_ProcMwDmCmd(uint8_t *p_cmdPayload)
{
    uint8_t evt_len = 0;
    uint8_t *p_evt_buf = NULL;
    uint16_t result = MBA_RES_SUCCESS;

    switch (*(p_cmdPayload))
    {
        case UART_MW_DM_CMD_CONFIG:
        {
            BLE_DM_Config_T dmConfig;
            uint8_t *p_bufTmp = (p_cmdPayload + 1);

            STREAM_BE_TO_U16(&dmConfig.connConfig.minAcceptConnInterval, &p_bufTmp);
            STREAM_BE_TO_U16(&dmConfig.connConfig.maxAcceptConnInterval, &p_bufTmp);
            STREAM_BE_TO_U16(&dmConfig.connConfig.minAcceptPeripheralLatency, &p_bufTmp);
            STREAM_BE_TO_U16(&dmConfig.connConfig.maxAcceptPeripheralLatency, &p_bufTmp);
            STREAM_TO_U8(&dmConfig.connConfig.autoReplyUpdateRequest, &p_bufTmp);
            STREAM_TO_U8(&dmConfig.secAutoAccept, &p_bufTmp);
            result = BLE_DM_Config(&dmConfig);
        }
        break;
        case UART_MW_DM_CMD_SM_PAIRING:
            result = BLE_DM_ProceedSecurity(*(uint16_t *)(p_cmdPayload + 1), *(p_cmdPayload + 3));
            break;
        case UART_MW_DM_CMD_SET_ACCEPTLIST:
            result = BLE_DM_SetFilterAcceptList(*(p_cmdPayload + 1), p_cmdPayload + 2);
            break;
        case UART_MW_DM_CMD_DELETE_ALL_DEVICE:
        {
            //result = BLE_DM_DeleteAllPairedDevice();
            if (APP_GetConnLinkNum() == 0)
            {
                result = APP_DS_PairDeleteAllStoredInfo();
            }
            else
            {
                result = MBA_RES_FAIL;
            }
        }
        break;
        case UART_MW_DM_CMD_GET_PAIRED_LIST:
        {
            p_evt_buf = OSAL_Malloc(1 + BLE_DM_MAX_PAIRED_DEVICE_NUM);

            if (p_evt_buf)
            {
                memset(p_evt_buf, 0, (1 + BLE_DM_MAX_PAIRED_DEVICE_NUM));
                BLE_DM_GetPairedDeviceList(p_evt_buf + 1, p_evt_buf);
                evt_len = 1 + *p_evt_buf;

                result = MBA_RES_SUCCESS;
            }
            else
                result = MBA_RES_OOM;
        }
        break;
        case UART_MW_DM_CMD_GET_PAIRED_DEVICE:
        {
            p_evt_buf = OSAL_Malloc(sizeof(BLE_DM_PairedDevInfo_T));

            if (p_evt_buf == NULL)
            {
                result = MBA_RES_OOM;
            }
            else
            {
                memset(p_evt_buf, 0, sizeof(BLE_DM_PairedDevInfo_T));
                result = BLE_DM_GetPairedDevice(*(p_cmdPayload + 1), (BLE_DM_PairedDevInfo_T *)p_evt_buf);
                evt_len = sizeof(BLE_DM_PairedDevInfo_T);
            }
        }
        break;
        case UART_MW_DM_CMD_CONN_CPU:
        {
            BLE_DM_ConnParamUpdate_T connUpdate;
            uint16_t connHandle;
            uint8_t *p_bufTmp = (p_cmdPayload + 1);

            STREAM_BE_TO_U16(&connHandle, &p_bufTmp);
            STREAM_BE_TO_U16(&connUpdate.intervalMin, &p_bufTmp);
            STREAM_BE_TO_U16(&connUpdate.intervalMax, &p_bufTmp);
            STREAM_BE_TO_U16(&connUpdate.latency, &p_bufTmp);
            STREAM_BE_TO_U16(&connUpdate.timeout, &p_bufTmp);
            result = BLE_DM_ConnectionParameterUpdate(connHandle, &connUpdate);
        }
        break;
        default:
            result = MBA_RES_INVALID_PARA;
            break;
    }

    APP_SendCmdCompleteEvt(evt_len, UART_GROUP_MW_DM, *(p_cmdPayload), result, p_evt_buf);

    if (p_evt_buf)
        OSAL_Free(p_evt_buf);
}


void  APP_ProcBleTrpCms(uint8_t opcode, uint16_t data_len, uint8_t *p_cmdPayload)
{
    uint16_t status = APP_ERR_UNKNOWN_COMMAND;
    uint16_t evt_len = 0;
    uint8_t *p_evt_buf = NULL;

    switch (opcode)
    {
        case UART_BLE_TRS_CMD_SEND_TRANSPARENT_DATA:
        {
            uint16_t connection_handle;
            
            connection_handle = (*(p_cmdPayload + 0) << 8) | *(p_cmdPayload + 1);
            // status = BLE_TRS_SendTransparentData(connection_handle, (data_len - 2), (p_cmdPayload + 2));
            status = BLE_TRSPS_SendData(connection_handle, (data_len - 2), (p_cmdPayload + 2));
        }
        break;
    }

    APP_SendCmdCompleteEvt(evt_len, UART_GROUP_BLE_TRS, opcode, status, p_evt_buf);

    OSAL_Free(p_evt_buf);
}



void APP_ProcBleL2capCmd(uint8_t opcode, uint16_t data_len, uint8_t *p_cmdPayload)
{
    uint16_t len = data_len;
    uint16_t status = APP_RES_FAIL;

    switch (opcode)
    {
        case UART_BLE_L2CAP_CMD_BLE_CPU:
        {
            uint16_t connHandle;
            uint16_t intervalMin, intervalMax, latency, timeout;
            uint8_t *p_buf = p_cmdPayload;

            STREAM_BE_TO_U16(&connHandle, &p_buf);
            STREAM_BE_TO_U16(&intervalMin, &p_buf);
            STREAM_BE_TO_U16(&intervalMax, &p_buf);
            STREAM_BE_TO_U16(&latency, &p_buf);
            STREAM_BE_TO_U16(&timeout, &p_buf);

            status = BLE_L2CAP_ConnParamUpdateReq(connHandle, intervalMin, intervalMax, latency, timeout);
        }
        break;
        #ifdef APP_L2CAP_CB_ENABLE
        case UART_BLE_L2CAP_CMD_CB_CONN_REQ:
        {
            uint16_t connHandle, psm;
            uint8_t *p_buf = p_cmdPayload;

            STREAM_BE_TO_U16(&connHandle, &p_buf);
            STREAM_BE_TO_U16(&psm, &p_buf);
            s_l2capInitiator = 1;

            #ifdef APP_BLE_TRCBPS_ENABLE
            status = BLE_TRCBPS_ConnReq(connHandle);
            #else
            status = BLE_L2CAP_CbConnReq(connHandle, psm);
            #endif
        }
        break;
        #endif

        #ifdef APP_L2CAP_CB_ENABLE
        case UART_BLE_L2CAP_CMD_CB_ADD_CREDIT:
        {
            uint16_t credit;
            uint8_t l2capId;
            uint8_t *p_buf = p_cmdPayload;

            STREAM_TO_U8(&l2capId, &p_buf);
            STREAM_BE_TO_U16(&credit, &p_buf);

            status = BLE_L2CAP_CbAddCredits(l2capId, credit);
        }
        break;
        #endif

        #ifdef APP_L2CAP_CB_ENABLE
        case UART_BLE_L2CAP_CMD_CB_SEND_SDU:
        {
            uint8_t *p_buf = p_cmdPayload;
            uint8_t *p_option = OSAL_Malloc(len);


            #ifdef APP_BLE_TRCBPS_ENABLE
            uint16_t connHandle;

            STREAM_BE_TO_U16(&connHandle, &p_buf);

            status = BLE_TRCBPS_SendData(connHandle, len - 2, p_option);
            #else
            uint8_t l2capId;

            STREAM_TO_U8(&l2capId, &p_buf);
            STREAM_COPY_TO_VARIABLE(p_option, &p_buf, len);

            status = BLE_L2CAP_CbSendSdu(l2capId, len-1, p_option);
            #endif

            OSAL_Free(p_option);
            p_option = NULL;
        }
        break;
        #endif

        #ifdef APP_L2CAP_CB_ENABLE
        case UART_BLE_L2CAP_CMD_CB_DISCONNECT:
        {
            #ifdef APP_BLE_TRCBPS_ENABLE
            uint16_t connHandle;
            uint8_t *p_buf = p_cmdPayload;

            STREAM_BE_TO_U16(&connHandle, &p_buf);

            status = BLE_TRCBPS_DisconnectReq(connHandle);
            #else
            uint8_t l2capId = 0;
            uint8_t *p_buf = p_cmdPayload;

            STREAM_TO_U8(&l2capId, &p_buf);

            status = BLE_L2CAP_CbDiscReq(l2capId);
            #endif
        }
        break;
        #endif
        
        #ifdef APP_L2CAP_CB_ENABLE
        case UART_BLE_L2CAP_CMD_CB_CONN_RSP:
        {

        }
        break;
        #endif

        #ifdef APP_L2CAP_CB_ENABLE
        case UART_BLE_L2CAP_CMD_SPSM_REGISTER:
        {
            uint16_t spsm, mtu,mps, initCredit;
            uint8_t *p_buf = p_cmdPayload;
            uint8_t permission;

            STREAM_BE_TO_U16(&spsm, &p_buf);
            STREAM_BE_TO_U16(&mtu, &p_buf);
            STREAM_BE_TO_U16(&mps, &p_buf);
            STREAM_BE_TO_U16(&initCredit, &p_buf);
            STREAM_TO_U8(&permission, &p_buf);

            status = BLE_L2CAP_CbRegisterSpsm(spsm, mtu, mps, initCredit, permission);
        }
        break;
        #endif
        default:
        {
            status = APP_RES_INVALID_PARA;
        }
        break;
    }

    APP_SendCmdCompleteEvt(0, UART_GROUP_BLE_L2CAP, opcode, status, NULL);
}

void APP_ProcUartCmd(uint16_t len, uint8_t *p_cmdPayload)
{
    uint8_t RX_OPCode;

    RX_OPCode = *p_cmdPayload;

    switch (RX_OPCode)
    {
        case UART_CMD_TEST:
        {
            switch (*(p_cmdPayload + 1))
            {
                case UART_GROUP_BT_GAP:
                {
                    APP_ProcGapCmd(*(p_cmdPayload + 2), (len - 3), (p_cmdPayload + 3));
                }
                break;

                case UART_GROUP_GATT:
                {
                    APP_ProcGattCmd(*(p_cmdPayload + 2), (len - 3), (p_cmdPayload + 3));
                }
                break;

                case UART_GROUP_BLE_GAP:
                {
                    APP_ProcBleGapCmd(p_cmdPayload + 2, len - 2);
                }
                break;

                case UART_GROUP_BLE_SMP:
                {
                    APP_ProcBleSmpCmd(p_cmdPayload + 2);
                }
                break;

                case UART_GROUP_BLE_TRS:
                {
                    APP_ProcBleTrpCms(*(p_cmdPayload + 2), (len - 3), (p_cmdPayload + 3));
                }
                break;

                case UART_GROUP_BLE_L2CAP:
                {
                    APP_ProcBleL2capCmd(*(p_cmdPayload + 2), (len - 3), (p_cmdPayload + 3));
                }
                break;
                
                case UART_GROUP_UTILITY:
                {
                    APP_ProcUtilityCmd(p_cmdPayload + 2);
                }
                break;

                case UART_GROUP_MW_DM:
                {
                    APP_ProcMwDmCmd(p_cmdPayload + 2);
                }
                break;

                default:
                {
                    APP_SendCmdCompleteEvt(0, *(p_cmdPayload + 1), *(p_cmdPayload + 2), APP_ERR_UNKNOWN_COMMAND, 0);
                }
                break;
            }
        }
        break;
        default:
        {
            APP_SendCmdCompleteEvt(0, RX_OPCode, 0, APP_ERR_UNKNOWN_COMMAND, 0);
        }
        break;
    }
}

void APP_SendCmdCompleteEvt(uint8_t data_len, uint8_t grp, uint8_t cmd, uint16_t status, uint8_t *p_buffer)
{
    uint8_t *p_commandComplete;
    uint8_t total_len = 0;
    total_len = data_len + 6;
    p_commandComplete = OSAL_Malloc(total_len);
    if (p_commandComplete != NULL)
    {
        *(p_commandComplete + 0) = UART_EVENT_TEST;
        *(p_commandComplete + 1) = UART_GROUP_EVENT_COMMAND_COMPLETE;
        *(p_commandComplete + 2) = grp;
        *(p_commandComplete + 3) = cmd;
        *(p_commandComplete + 4) = status >> 8;
        *(p_commandComplete + 5) = status;
        if (data_len)
        {
            memcpy((uint8_t *)(p_commandComplete + 6), (uint8_t *)p_buffer, data_len);
        }
        APP_PackageUartEvent(total_len, p_commandComplete);
        OSAL_Free(p_commandComplete);
    }
}


void APP_SendTrpConnEvt(uint16_t connHandle, uint8_t attRole, uint8_t status)
{
    uint8_t *p_EvtBuf, *p_Buf;
    uint8_t total_len = 7;

    p_EvtBuf = OSAL_Malloc(total_len);
    p_Buf = p_EvtBuf;
    if (p_Buf != NULL)
    {
        U8_TO_STREAM(&p_Buf, UART_EVENT_TEST);
        U8_TO_STREAM(&p_Buf, UART_GROUP_BLE_TRS);
        U8_TO_STREAM(&p_Buf, EVT_TRP_CONNECTED);
        U16_TO_STREAM_BE(&p_Buf, connHandle);
        U8_TO_STREAM(&p_Buf, attRole);
        U8_TO_STREAM(&p_Buf, status);
        APP_PackageUartEvent(total_len, p_EvtBuf);
        OSAL_Free(p_EvtBuf);
    }
}

