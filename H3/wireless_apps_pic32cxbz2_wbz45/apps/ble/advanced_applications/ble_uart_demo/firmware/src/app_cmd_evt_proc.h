/*******************************************************************************
  Application UART Command/Event Process Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app_cmd_evt_proc.h

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

#ifndef APP_CMD_EVT_PROC_H
#define APP_CMD_EVT_PROC_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "gatt.h"
#include "ble_gap.h"
#include "ble_smp.h"
#include "ble_l2cap.h"
#include "mba_error_defs.h"
#include "ble_trcbps/ble_trcbps.h"
#include "app.h"
#include "app_error_defs.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define UART_DOCKLIGNT_FORMAT

#define UART_START_BYTE                                     0xAA       /**< UART Start Pattern. */

#define UART_CMD_TEST                                       0x40       /**< Test Command. */
#define UART_EVENT_TEST                                     0x50       /**< Test Event. */

#define UART_BLE_TRS_CMD_SEND_TRANSPARENT_DATA              0x03


enum UART_GROUP_ID
{
    UART_GROUP_BT_GAP = 0x00,
    UART_GROUP_GATT = 0x01,
    UART_GROUP_BLE_GAP = 0x02,
    UART_GROUP_BLE_SMP = 0x03,
    UART_GROUP_BLE_TRS = 0x04,
    UART_GROUP_BT_HFP = 0x05,
    UART_GROUP_RTU = 0x06,
    UART_GROUP_DFU = 0x07,
    UART_GROUP_BT_AVDTP = 0x08,
    UART_GROUP_BT_AVCTP = 0x09,
    UART_GROUP_BT_AVRCP = 0x0A,
    UART_GROUP_BT_L2CAP = 0x0B,
    UART_GROUP_BT_A2DP = 0x0C,
    UART_GROUP_OTA = 0x0D,
    UART_GROUP_BLE_L2CAP = 0x0E,
    UART_GROUP_PTS_TEST = 0x0F,

    UART_GROUP_PERIPHERAL_LED = 0x10,
    UART_GROUP_PERIPHERAL_ADC,
    UART_GROUP_PERIPHERAL_SPI,
    UART_GROUP_PERIPHERAL_PWM,
    UART_GROUP_PERIPHERAL_SQIFLASH,
    UART_GROUP_PERIPHERAL_GPIO,
    UART_GROUP_PERIPHERAL_I2C,
    UART_GROUP_PERIPHERAL_CHG,
    UART_GROUP_PERIPHERAL_IAP2,
    UART_GROUP_PERIPHERAL_USB_BC,

    UART_GROUP_BT_SDP = 0x20,
    UART_GROUP_BT_RFCOMM,
    UART_GROUP_BT_SPP,
    UART_GROUP_BT_PBAP,
    UART_GROUP_BLE_GFPS,

    UART_GROUP_UTILITY = 0x30,

    UART_GROUP_MW_ADV  = 0x40,
    UART_GROUP_MW_CONN,
    UART_GROUP_MW_DM,
    UART_GROUP_MW_DD,
    UART_GROUP_MW_SCAN,

    UART_GROUP_EVENT_COMMAND_COMPLETE = 0x80,
    UART_GROUP_END
};

enum UART_UTILITY_CMD_ID
{
    UART_UTILITY_CMD_SHUTDOWN = 0x00,
    UART_UTILITY_CMD_CURR_CONN_HANDLE,
    UART_UTILITY_CMD_INIT_DONE                  /**< Only used to notify app initial complete not real command */
};

enum UART_GAP_CMD_ID
{
    UART_GAP_CMD_RESET = 0x12,
    UART_GAP_CMD_END
};

enum UART_BLE_GAP_CMD_ID
{
    UART_BLE_GAP_CMD_SET_ADDR = 0x00,
    UART_BLE_GAP_CMD_GET_ADDR,
    UART_BLE_GAP_CMD_GET_DEV_NAME,
    UART_BLE_GAP_CMD_SET_DEV_NAME,
    UART_BLE_GAP_CMD_SET_ADV_DATA,
    UART_BLE_GAP_CMD_SET_SCAN_RESP_DATA,
    UART_BLE_GAP_CMD_SET_ADV_PARAM,
    UART_BLE_GAP_CMD_ADV_ENABLE,
    UART_BLE_GAP_CMD_TERMINATE_CONNECTION,
    UART_BLE_GAP_CMD_UPDATE_CONN_PARA,
    UART_BLE_GAP_CMD_GET_RSSI,
    UART_BLE_GAP_CMD_RESERVED_1,
    UART_BLE_GAP_CMD_CLEAR_WHITE_LIST,
    UART_BLE_GAP_CMD_SET_WHITE_LIST,
    UART_BLE_GAP_CMD_RESERVED_2,
    UART_BLE_GAP_CMD_SET_RESOLVING_LIST,
    UART_BLE_GAP_CMD_CLEAR_RESOLVING_LIST,
    UART_BLE_GAP_CMD_SET_PRIVACY,
    UART_BLE_GAP_CMD_GET_PRIVACY,
    UART_BLE_GAP_CMD_CREATE_CONNECTION,
    UART_BLE_GAP_CMD_CREATE_CONNECTION_CANCEL,
    UART_BLE_GAP_CMD_SET_SCANNING_PARAM,
    UART_BLE_GAP_CMD_SET_SCANNING_ENABLE,
    UART_BLE_GAP_CMD_SET_SCANNING_FILTER,
    UART_BLE_GAP_CMD_WRITE_BEACON_PARA,
    UART_BLE_GAP_CMD_WRITE_BEACON_DATA,
    UART_BLE_GAP_CMD_WRITE_BEACON_ENABLE,
    UART_BLE_GAP_CMD_CHANGE_TX_BUFFER_SIZE_MODE,
    UART_BLE_GAP_CMD_WRITE_BEACON_RANDOM_ADDRESS,
    UART_BLE_GAP_CMD_RECEIVER_TEST,
    UART_BLE_GAP_CMD_TRANSMITTER_TEST,
    UART_BLE_GAP_CMD_END_TEST,
    UART_BLE_GAP_CMD_RESERVED_3,
    UART_BLE_GAP_CMD_SET_CHANNEL_MAP,
    UART_BLE_GAP_CMD_START_ENCRYPTION,
    UART_BLE_GAP_CMD_SET_EXT_ADV_PARAM,
    UART_BLE_GAP_CMD_SET_EXT_ADV_DATA,
    UART_BLE_GAP_CMD_SET_EXT_SCAN_RESP_DATA,
    UART_BLE_GAP_CMD_SET_EXT_ADV_ENABLE,
    UART_BLE_GAP_CMD_SET_PERIODIC_ADV_PARAM,
    UART_BLE_GAP_CMD_SET_PERIODIC_ADV_DATA,
    UART_BLE_GAP_CMD_SET_PERIODIC_ADV_ENABLE,
    UART_BLE_GAP_CMD_SET_EXT_SCAN_PARAM,
    UART_BLE_GAP_CMD_SET_EXT_SCAN_ENABLE,

    UART_BLE_GAP_CMD_END
};

enum UART_BleGapScanFilterCmdId
{
    UART_BLE_SCAN_NAME_FILTER_CMD_SET =0,
    UART_BLE_SCAN_SERVICE_DATA_FILTER_CMD_SET,
    UART_BLE_SCAN_MSD_FILTER_CMD_SET,
    UART_BLE_SCAN_DISABLE_FILTER_CMD_SET
};

enum UART_BLE_SMP_CMD_ID
{
    UART_BLE_SMP_CMD_CONFIG = 0x00,
    UART_BLE_SMP_CMD_PASSKEY_REPLY,
    UART_BLE_SMP_CMD_PASSKEY_NEG_REPLY,
    UART_BLE_SMP_CMD_NUM_COMP_CONF_REPLY,
    UART_BLE_SMP_CMD_INIT_PAIR,
    UART_BLE_SMP_CMD_GEN_PASSKEY,
    UART_BLE_SMP_CMD_OOB_DATA_REPLY,
    UART_BLE_SMP_CMD_ENABLE_DEBUG_MODE,
    UART_BLE_SMP_CMD_GEN_OOB_DATA,
    UART_BLE_SMP_CMD_SET_LESC_OOB_DATA,
    UART_BLE_SMP_CMD_SET_TRIGGER_PAIR,
    UART_BLE_SMP_CMD_ENTER_EXIT_TDC_MODE,
    UART_BLE_SMP_CMD_KEYPRESS

};

enum UART_BLE_L2CAP_CMD
{
    UART_BLE_L2CAP_CMD_BLE_CPU,
    UART_BLE_L2CAP_CMD_CB_CONN_REQ,
    UART_BLE_L2CAP_CMD_CB_ADD_CREDIT,
    UART_BLE_L2CAP_CMD_CB_SEND_SDU,
    UART_BLE_L2CAP_CMD_CB_DISCONNECT,
    UART_BLE_L2CAP_CMD_CB_CONN_RSP,
    UART_BLE_L2CAP_CMD_SPSM_REGISTER,
    UART_BLE_L2CAP_CMD_ECB_CONN_REQ,
    UART_BLE_L2CAP_CMD_ECB_CONN_RSP,
    UART_BLE_L2CAP_CMD_ECB_RECONFIG_REQ,
    UART_BLE_L2CAP_CMD_END
};


enum UART_RTU_CMD
{
    UART_RTU_CMD_READ_ALL_PAIRED_DEVICE = 0x00,
    UART_RTU_CMD_ERASE_ALL_PAIRED_DEVICE,
    UART_RTU_CMD_DEL_SPECIFIED_PAIRED_DEVICE,
    UART_RTU_CMD_LOAD,
    UART_RTU_CMD_SAVE,
    UART_RTU_CMD_DUMP_BLOCK,
    UART_RTU_CMD_DUMP_RTU,
    UART_RTU_CMD_ERASE_SECTOR,
    UART_RTU_CMD_READ_SPECIFIC_PAIRED_DEVICE
};

enum UART_GATT_CMD_ID
{
    UART_GATT_CMD_READ = 0x00,
    UART_GATT_CMD_WRITE,
    UART_GATT_CMD_HANDLE_CONF,
    UART_GATT_CMD_EX_MTU,
    UART_GATT_CMD_DIS_ALL_PSERV,
    UART_GATT_CMD_DIS_PRIMARY_SERV_UUID,
    UART_GATT_CMD_DIS_ALL_CHAR,
    UART_GATT_CMD_DIS_ALL_DESC,
    UART_GATT_CMD_READ_USING_UUID,
    UART_GATT_CMD_SEND_HANDLE_VALUE,
    UART_GATT_CMD_RESERVED1,
    UART_GATT_CMD_SEND_READ_RESP,
    UART_GATT_CMD_SEND_ERR_RESP,
    UART_GATT_CMD_SEND_READ_BY_TYPE_RESP,
    UART_GATT_CMD_SEND_WRITE_RESP,
    UART_GATT_CMD_BT_CONNECT,
    UART_GATT_CMD_BT_DISCONNECT,        //0x10
    UART_GATT_CMD_DIS_CHAR_USING_UUID,
    UART_GATT_CMD_SERVICE_CHANGE,
    UART_GATT_CMD_ADD_SERVICE,
    UART_GATT_CMD_REMOVE_SERVICE,
    UART_GATT_CMD_READ_MULTI_VARIABLE,
    UART_GATT_CMD_MULTI_HANDLE_VALUE_NTF,
    UART_GATT_CMD_GET_ATTRIBUTE_VALUE,
    UART_GATT_CMD_TEST
};

enum UART_MW_DM_CMD_ID
{
    UART_MW_DM_CMD_CONFIG = 0x00,
    UART_MW_DM_CMD_SM_PAIRING,
    UART_MW_DM_CMD_SET_ACCEPTLIST,
    UART_MW_DM_CMD_DELETE_ALL_DEVICE,
    UART_MW_DM_CMD_GET_PAIRED_LIST,
    UART_MW_DM_CMD_GET_PAIRED_DEVICE,
    UART_MW_DM_CMD_CONN_CPU
};

typedef enum UTILITY_EventID_T
{
    UTILITY_EVT_PAIRED_RECORD_FULL,                     /**< The maximum record number of paired device have been reached. See the @ref UTILITY_Evt_PairedDeviceRecordFull_T for the event content. */
    UTILITY_EVT_PAIRED_KEY_NOTIFY,                      /**< The link key of paired device have been updated. See the @ref UTILITY_Evt_PairedDeviceRecordFull_T for the event content. */        
    UTILITY_EVT_REMOTE_BOND_LOST,                       /**< The remote bond has been losted. See the @ref UTILITY_Evt_RemoteBondLost_T for the event content. */        
    UTILITY_EVT_END
} UTILITY_EventID_T;


enum APP_TRP_Event_T
{
    EVT_TRP_CONNECTED = 0x00,
    EVT_TRP_RX_DATA,
    
    EVT_END   
};

enum APP_STATE_ID
{
    APP_STATE_IDLE,
    APP_STATE_BLE_CONNECTED
};

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
void APP_ProcBleGapUartEvt(BLE_GAP_Event_T *p_event);
void APP_ProcBleL2capUartEvt(BLE_L2CAP_Event_T *p_event);
void APP_ProcGattUartEvt(GATT_Event_T *p_event);
void APP_ProcBleSmpUartEvt(BLE_SMP_Event_T *p_event);
void APP_ProcTrcbpsUartEvt(BLE_TRCBPS_Event_T *p_event);
void APP_ProcUartCmd(uint16_t len, uint8_t *p_cmdPayload);
void APP_SendCmdCompleteEvt(uint8_t data_len, uint8_t grp, uint8_t cmd, uint16_t status, uint8_t *p_buffer);
void APP_SendTrpConnEvt(uint16_t connHandle, uint8_t attRole, uint8_t status);

#endif
