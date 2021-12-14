/*******************************************************************************
  BLE Log Middleware Source File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_log.c

  Summary:
    This file contains the BLE Log functions for this project.

  Description:
    This file contains the BLE Log functions for this project.
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
#include "osal/osal_freertos_extend.h"
#include "ble_util/byte_stream.h"
#include "ble_log/ble_log.h"
#include "host_hci_defs.h"

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

typedef void (*_BLE_LOG_FunTbl_T)(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLentgh);

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
static void ble_log_PackDisconnect(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackReadAuthenticatedPayloadTimeout(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackWriteAuthenticatedPayloadTimeout(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackReadBdAddr(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackReadRssi(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackSetRandomAddr(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackSetAdvParams(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackSetAdvData(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackSetScanRspData(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackSetAdvEnable(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackSetScanParams(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackSetScanEnable(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackCreateConn(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackCreateConnCancel(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackClearFilterAcceptList(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackAddDevToFilterAcceptList(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackConnectionUpdate(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackSetHostChannelClassification(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackReadChannelMap(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackReadRemoteFeature(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackEnableEncryption(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackLtkReqReply(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackLtkReqNegReply(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackEndTest(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackRemoteConnParamReqReply(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackRemoteConnParamReqNegReply(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackSetDataLength(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackAddDevToResolvingList(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackClearResolvingList(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackSetAddrResolutionEnable(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_packSetRpaTimeout(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackReadPhy(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackSetDefaultPhy(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackSetPhy(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackReceiverTestV2(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackSetAdvSetRandomAddr(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackSetExtAdvParam(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackSetExtAdvData(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackSetExtScanRspData(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackSetExtAdvEnable(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackRemoveAdvSet(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackClearAdvSet(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackSetPeriodicAdvParam(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackSetPeriodicAdvData(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackSetPeriodicAdvEnable(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackSetExtScanParam(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackSetExtScanEnable(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackExtCreateConn(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackPeriodicAdvCreateSync(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackPeriodicAdvCreateSyncCancel(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackPeriodicAdvTerminateSync(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackAddDevToPeriodicAdvList(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackClearPeriodicAdvList(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackReadTransmitPower(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackSetPrivacyMode(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackSetPeriodicAdvReceiveEnable(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackEnhancedReadTransmitPowerLevel(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackReadRemoteTransmitPowerLevel(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackSetTransmitPowerReportingEnable(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackTransmitterTestV4(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);

static void ble_log_PackDiscComplete(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackNumberOfCompletedPacket(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackConnectComplete(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackAdvReport(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackConnectionUpdateComplete(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackReadRemoteFeaturesComplete(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackLtkRequest(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackRemoteConnectionUpdateRequest(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackDataLengthChange(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackDirectAdvReport(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackPhyUpdateComplete(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackExtAdvReport(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackPeriodicAdvSyncEstablished(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackPeriodicAdvReport(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackPeriodicAdvSyncLost(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackScanTimeout(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackAdvSetTerminated(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);
static void ble_log_PackScanRequestReceived(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength);

// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************
BLE_LOG_PrintCb                 g_logPrintCb = NULL;

static const _BLE_LOG_FunTbl_T  s_hciCmdFun[] =
{
    NULL,   // (0x00)
    NULL,   // (0x01)
    NULL,   // (0x02)
    NULL,   // (0x03)
    NULL,   // (0x04)
    ble_log_PackSetRandomAddr,                          // HCI_LE_SET_RANDOM_ADDRESS (0x05)
    ble_log_PackSetAdvParams,                           // HCI_LE_SET_ADVERTISING_PARAMS (0x06)
    NULL,   // (0x07)
    ble_log_PackSetAdvData,                             // HCI_LE_SET_ADVERTISING_DATA (0x08)
    ble_log_PackSetScanRspData,                         // HCI_LE_SET_SCAN_RESPONSE_DATA (0x09)
    ble_log_PackSetAdvEnable,                           // HCI_LE_SET_ADVERTISING_ENABLE (0x0A)
    ble_log_PackSetScanParams,                          // HCI_LE_SET_SCAN_PARAMS (0x0B)
    ble_log_PackSetScanEnable,                          // HCI_LE_SET_SCAN_ENABLE (0x0C)
    ble_log_PackCreateConn,                             // HCI_LE_CREATE_CONNECTION (0x0D)
    ble_log_PackCreateConnCancel,                       // HCI_LE_CREATE_CONNECTION_CANCEL (0x0E)
    NULL,   // (0x0F)
    ble_log_PackClearFilterAcceptList,                  // HCI_LE_CLEAR_FILTER_ACCEPT_LIST (0x10)
    ble_log_PackAddDevToFilterAcceptList,               // HCI_LE_ADD_DEVICE_TO_FILTER_ACCEPT_LIST (0x11)
    NULL,   // (0x12)
    ble_log_PackConnectionUpdate,                       // HCI_LE_CONNECTION_UPDATE (0x13)
    ble_log_PackSetHostChannelClassification,           // HCI_LE_SET_HOST_CHANNEL_CLASSIFICATION (0x14)
    ble_log_PackReadChannelMap,                         // HCI_LE_READ_CHANNEL_MAP (0x15)
    ble_log_PackReadRemoteFeature,                      // HCI_LE_READ_REMOTE_FEATURES (0X16)
    NULL,   // (0x17)
    NULL,   // (0x18)
    ble_log_PackEnableEncryption,                       // HCI_LE_ENABLE_ENCRYPTION (0x19)
    ble_log_PackLtkReqReply,                            // HCI_LE_LONG_TERM_KEY_REQ_REPLY (0x1A)
    ble_log_PackLtkReqNegReply,                         // HCI_LE_LONG_TERM_KEY_REQ_NEGATIVE_REPLY (0x1B)
    0x00,   // (0x1C)
    NULL,   // (0x1D)
    NULL,   // (0x1E)
    ble_log_PackEndTest,                                // HCI_LE_TEST_END (0x1F)
    ble_log_PackRemoteConnParamReqReply,                // HCI_LE_REMOTE_CONN_PARAM_REQ_REPLY (0x20)
    ble_log_PackRemoteConnParamReqNegReply,             // HCI_LE_REMOTE_CONN_PARAM_REQ_NEGATIVE_REPLY (0x21)
    ble_log_PackSetDataLength,                          // HCI_LE_SET_DATA_LENGTH (0x22)
    NULL,   // (0x23)
    NULL,   // (0x24)
    NULL,   // (0x25)
    NULL,   // (0x26)
    ble_log_PackAddDevToResolvingList,                  // HCI_LE_ADD_DEVICE_TO_RESOLVING_LIST (0x27)
    NULL,   // (0x28)
    ble_log_PackClearResolvingList,                     // HCI_LE_CLEAR_RESOLVING_LIST (0x29)
    NULL,   // (0x2A)
    NULL,   // (0x2B)
    NULL,   // (0x2C)
    ble_log_PackSetAddrResolutionEnable,                // HCI_LE_SET_ADDR_RESOLUTION_ENABLE (0x2D)
    ble_log_packSetRpaTimeout,                          // HCI_LE_SET_RPA_TIMEOUT (0x2E)
    NULL,   // (0x2F)
    ble_log_PackReadPhy,                                // HCI_LE_READ_PHY (0x30)
    ble_log_PackSetDefaultPhy,                          // HCI_LE_SET_DEFAULT_PHY (0x31)
    ble_log_PackSetPhy,                                 // HCI_LE_SET_PHY (0x32)
    ble_log_PackReceiverTestV2,                         // HCI_LE_RECEIVER_TEST_V1 (0x33)
    NULL,   // (0x34)
    ble_log_PackSetAdvSetRandomAddr,                    // HCI_LE_SET_ADVERTISING_SET_RANDOM_ADDR (0x35)
    ble_log_PackSetExtAdvParam,                         // HCI_LE_SET_EXTENDED_ADVERTISING_PARAM (0x36)
    ble_log_PackSetExtAdvData,                          // HCI_LE_SET_EXTENDED_ADVERTISING_DATA (0x37)
    ble_log_PackSetExtScanRspData,                      // HCI_LE_SET_EXTENDED_SCAN_RSP_DATA (0x38)
    ble_log_PackSetExtAdvEnable,                        // HCI_LE_SET_EXTENDED_ADVERTISING_ENABLE (0x39)
    NULL,   // (0x3A)
    NULL,   // (0x3B)
    ble_log_PackRemoveAdvSet,                           // HCI_LE_REMOVE_ADVERTISING_SET (0x3C)
    ble_log_PackClearAdvSet,                            // HCI_LE_CLEAR_ADVERTISING_SETS (0x3D)
    ble_log_PackSetPeriodicAdvParam,                    // HCI_LE_SET_PERIODIC_ADVERTISING_PARAM (0x3E)
    ble_log_PackSetPeriodicAdvData,                     // HCI_LE_SET_PERIODIC_ADVERTISING_DATA (0x3F)
    ble_log_PackSetPeriodicAdvEnable,                   // HCI_LE_SET_PERIODIC_ADVERTISING_ENABLE (0x40)
    ble_log_PackSetExtScanParam,                        // HCI_LE_SET_EXTENDED_SCAN_PARAM (0x41)
    ble_log_PackSetExtScanEnable,                       // HCI_LE_SET_EXTENDED_SCAN_ENABLE (0x42)
    ble_log_PackExtCreateConn,                          // HCI_LE_EXTENDED_CREATE_CONNECTION (0x43)
    ble_log_PackPeriodicAdvCreateSync,                  // HCI_LE_PERIODIC_ADVERTISING_CREATE_SYNC (0x44)
    ble_log_PackPeriodicAdvCreateSyncCancel,            // HCI_LE_PERIODIC_ADVERTISING_CREATE_SYNC_CANCEL (0x45)
    ble_log_PackPeriodicAdvTerminateSync,               // HCI_LE_PERIODIC_ADVERTISING_TERMINATE_SYNC (0x46)
    ble_log_PackAddDevToPeriodicAdvList,                // HCI_LE_ADD_DEV_TO_PERIODIC_ADVERTISER_LIST (0x47)
    NULL,   // (0x48)
    ble_log_PackClearPeriodicAdvList,                   // HCI_LE_CLEAR_PERIODIC_ADVERTISER_LIST (0x49)
    NULL,   // (0x4A)
    ble_log_PackReadTransmitPower,                      // HCI_LE_READ_TRANSMIT_POWER (0x4B)
    NULL,   // (0x4C)
    NULL,   // (0x4D)
    ble_log_PackSetPrivacyMode,                         // HCI_LE_SET_PRIVACY_MODE (0x4E)
    NULL,   // (0x4F)
    NULL,   // (0x50)
    NULL,   // (0x51)
    NULL,   // (0x52)
    NULL,   // (0x53)
    NULL,   // (0x54)
    NULL,   // (0x55)
    NULL,   // (0x56)
    NULL,   // (0x57)
    NULL,   // (0x58)
    ble_log_PackSetPeriodicAdvReceiveEnable,            // HCI_LE_SET_PERIODIC_ADVERTISING_RECEIVE_ENABLE (0x59)
    NULL,   // (0x5A)
    NULL,   // (0x5B)
    NULL,   // (0x5C)
    NULL,   // (0x5D)
    NULL,   // (0x5E)
    NULL,   // (0x5F)
    NULL,   // (0x60)
    NULL,   // (0x61)
    NULL,   // (0x62)
    NULL,   // (0x63)
    NULL,   // (0x64)
    NULL,   // (0x65)
    NULL,   // (0x66)
    NULL,   // (0x67)
    NULL,   // (0x68)
    NULL,   // (0x69)
    NULL,   // (0x6A)
    NULL,   // (0x6B)
    NULL,   // (0x6C)
    NULL,   // (0x6D)
    NULL,   // (0x6E)
    NULL,   // (0x6F)
    NULL,   // (0x70)
    NULL,   // (0x71)
    NULL,   // (0x72)
    NULL,   // (0x73)
    NULL,   // (0x74)
    NULL,   // (0x75)
    ble_log_PackEnhancedReadTransmitPowerLevel,         // HCI_LE_ENHANCED_READ_TRANSMIT_POWER_LEVEL (0x76)
    ble_log_PackReadRemoteTransmitPowerLevel,           // HCI_LE_READ_REMOTE_TRANSMIT_POWER_LEVEL (0x77)
    NULL,   // (0x78)
    NULL,   // (0x79)
    ble_log_PackSetTransmitPowerReportingEnable,        // HCI_LE_SET_TRANSMIT_POWER_REPORTING_ENABLE (0x7A)
    ble_log_PackTransmitterTestV4                       // HCI_LE_TRANSMITTER_TEST_V4 (0x7B)
};

static const _BLE_LOG_FunTbl_T  s_hciLeMetaEvtFun[] =
{
    NULL,   // Reserved (0x00)
    ble_log_PackConnectComplete,                        // HCI_LE_CONNECT_COMPLETE (0x01)
    ble_log_PackAdvReport,                              // HCI_LE_ADVERTISING_REPORT (0x02)
    ble_log_PackConnectionUpdateComplete,               // HCI_LE_CONNECTION_UPDATE_COMPLETE (0x03)
    ble_log_PackReadRemoteFeaturesComplete,             // HCI_LE_READ_REMOTE_FEATURES_COMPLETE (0x04)
    ble_log_PackLtkRequest,                             // HCI_LE_LONG_TERM_KEY_REQUEST (0x05)
    ble_log_PackRemoteConnectionUpdateRequest,          // HCI_LE_REMOTE_CONNECTION_PARAMETER_REQUEST (0x06)
    ble_log_PackDataLengthChange,                       // HCI_LE_DATA_LENGTH_CHANGE (0x07)
    NULL,   // (0x08)
    NULL,   // (0x09)
    NULL,   // (0x0A)
    ble_log_PackDirectAdvReport,                        // HCI_LE_DIRECT_ADVERTISING_REPORT (0x0B)
    ble_log_PackPhyUpdateComplete,                      // HCI_LE_PHY_UPDATE_COMPLETE (0x0C)
    ble_log_PackExtAdvReport,                           // HCI_LE_EXTENDED_ADVERTISING_REPORT (0x0D)
    ble_log_PackPeriodicAdvSyncEstablished,             // HCI_LE_PERIODIC_ADVERTISING_SYNC_ESTABLISHED (0x0E)
    ble_log_PackPeriodicAdvReport,                      // HCI_LE_PERIODIC_ADVERTISING_REPORT (0x0F)
    ble_log_PackPeriodicAdvSyncLost,                    // HCI_LE_PERIODIC_ADVERTISING_SYNC_LOST (0x10)
    ble_log_PackScanTimeout,                            // HCI_LE_SCAN_TIMEOUT (0x11)
    ble_log_PackAdvSetTerminated,                       // HCI_LE_ADVERTISING_SET_TERMINATED (0x12)
    ble_log_PackScanRequestReceived                     // HCI_LE_SCAN_REQUEST_RECEIVED (0x13)
};

uint8_t s_hciCmdPktSize[] =
{
    0x00,   // (0x00)
    0x00,   // (0x01)
    0x00,   // (0x02)
    0x00,   // (0x03)
    HCI_CMD_SIZE_LE_SET_RANDOM_ADDR,                    // HCI_LE_SET_RANDOM_ADDRESS (0x05)
    HCI_CMD_SIZE_LE_SET_ADV_PARAMS,                     // HCI_LE_SET_ADVERTISING_PARAMS (0x06)
    0x00,   // (0x07)
    HCI_CMD_SIZE_LE_SET_ADV_DATA,                       // HCI_LE_SET_ADVERTISING_DATA (0x08)
    HCI_CMD_SIZE_LE_SET_SCAN_RSP_DATA,                  // HCI_LE_SET_SCAN_RESPONSE_DATA (0x09)
    HCI_CMD_SIZE_LE_SET_ADV_ENABLE,                     // HCI_LE_SET_ADVERTISING_ENABLE (0x0A)
    HCI_CMD_SIZE_LE_SET_SCAN_PARAMS,                    // HCI_LE_SET_SCAN_PARAMS (0x0B)
    HCI_CMD_SIZE_LE_SET_SCAN_ENABLE,                    // HCI_LE_SET_SCAN_ENABLE (0x0C)
    HCI_CMD_SIZE_LE_CREATE_CONN,                        // HCI_LE_CREATE_CONNECTION (0x0D)
    HCI_CMD_SIZE_LE_CREATE_CONN_CANCEL,                 // HCI_LE_CREATE_CONNECTION_CANCEL (0x0E)
    0x00, // (0x0F)
    HCI_CMD_SIZE_LE_CLEAR_FILTER_ACCEPT_LIST,           // HCI_LE_CLEAR_FILTER_ACCEPT_LIST (0x10)
    HCI_CMD_SIZE_LE_ADD_DEV_TO_FILTER_ACCEPT_LIST,      // HCI_LE_ADD_DEVICE_TO_FILTER_ACCEPT_LIST (0x11)
    0x00, // (0x12)
    HCI_CMD_SIZE_LE_CONN_UPDATE,                        // HCI_LE_CONNECTION_UPDATE (0x13)
    HCI_CMD_SIZE_LE_SET_HOST_CHANNEL_CLASSIFY,          // HCI_LE_SET_HOST_CHANNEL_CLASSIFICATION (0x14)
    HCI_CMD_SIZE_LE_READ_CHANNEL_MAP,                   // HCI_LE_READ_CHANNEL_MAP (0x15)
    HCI_CMD_SIZE_LE_READ_REMOTE_FEATURES,               // HCI_LE_READ_REMOTE_FEATURES (0X16)
    0x00, // (0x17)
    0x00, // (0x18)
    HCI_CMD_SIZE_LE_ENABLE_ENCRYPTION,                  // HCI_LE_ENABLE_ENCRYPTION (0x19)
    HCI_CMD_SIZE_LE_LTK_REQ_REPLY,                      // HCI_LE_LONG_TERM_KEY_REQ_REPLY (0x1A)
    HCI_CMD_SIZE_LE_LTK_REQ_NEG_REPLY,                  // HCI_LE_LONG_TERM_KEY_REQ_NEGATIVE_REPLY (0x1B)
    0x00, // (0x1C)
    HCI_CMD_SIZE_LE_RECEIVER_TEST_V1,                   // HCI_LE_RECEIVER_TEST_V1 (0x1D)
    HCI_CMD_SIZE_LE_TRANSMITTER_TEST_V1,                // HCI_LE_TRANSMITTER_TEST_V1 (0x1E)
    HCI_CMD_SIZE_LE_TEST_END,                           // HCI_LE_TEST_END (0x1F)
    HCI_CMD_SIZE_LE_REMOTE_CONN_PARAMS_REQ_REPLY,       // HCI_LE_REMOTE_CONN_PARAM_REQ_REPLY (0x20)
    HCI_CMD_SIZE_LE_REMOTE_CONN_PARAMS_REQ_NEG_REPLY,   // HCI_LE_REMOTE_CONN_PARAM_REQ_NEGATIVE_REPLY (0x21)
    HCI_CMD_SIZE_LE_SET_DATA_LENGTH,                    // HCI_LE_SET_DATA_LENGTH (0x22)
    0x00, // (0x23)
    0x00, // (0x24)
    0x00, // (0x25)
    0x00, // (0x26)
    HCI_CMD_SIZE_LE_ADD_DEV_TO_RES_LIST,                // HCI_LE_ADD_DEVICE_TO_RESOLVING_LIST (0x27)
    0x00, // (0x28)
    HCI_CMD_SIZE_LE_CLEAR_RES_LIST,                     // HCI_LE_CLEAR_RESOLVING_LIST (0x29)
    0x00, // (0x2A)
    0x00, // (0x2B)
    0x00, // (0x2C)
    HCI_CMD_SIZE_LE_SET_ADDR_RES_ENABLE,                // HCI_LE_SET_ADDR_RESOLUTION_ENABLE (0x2D)
    HCI_CMD_SIZE_LE_SET_RPA_TIMEOUT,                    // HCI_LE_SET_RPA_TIMEOUT (0x2E)
    0x00, // (0x2F)
    HCI_CMD_SIZE_LE_READ_PHY,                           // HCI_LE_READ_PHY (0x30)
    HCI_CMD_SIZE_LE_SET_DEFAULT_PHY,                    // HCI_LE_SET_DEFAULT_PHY (0x31)
    HCI_CMD_SIZE_LE_SET_PHY,                            // HCI_LE_SET_PHY (0x32)
    HCI_CMD_SIZE_LE_RECEIVER_TEST_V2,                   // HCI_LE_RECEIVER_TEST_V1 (0x33)
    0x00, // (0x34)
    HCI_CMD_SIZE_LE_SET_ADV_SET_RANDOM_ADDR,            // HCI_LE_SET_ADVERTISING_SET_RANDOM_ADDR (0x35)
    HCI_CMD_SIZE_LE_SET_EXT_ADV_PARAMS,                 // HCI_LE_SET_EXTENDED_ADVERTISING_PARAM (0x36)
    HCI_CMD_SIZE_LE_SET_EXT_ADV_DATA,                   // HCI_LE_SET_EXTENDED_ADVERTISING_DATA (0x37)
    HCI_CMD_SIZE_LE_SET_EXT_SCAN_RSP_DATA,              // HCI_LE_SET_EXTENDED_SCAN_RSP_DATA (0x38)
    HCI_CMD_SIZE_LE_SET_EXT_ADV_ENABLE,                 // HCI_LE_SET_EXTENDED_ADVERTISING_ENABLE (0x39)
    0x00, // (0x3A)
    0x00, // (0x3B)
    HCI_CMD_SIZE_LE_REMOVE_ADV_SET,                     // HCI_LE_REMOVE_ADVERTISING_SET (0x3C)
    HCI_CMD_SIZE_LE_CLEAR_ADV_SET,                      // HCI_LE_CLEAR_ADVERTISING_SETS (0x3D)
    HCI_CMD_SIZE_LE_SET_PERIODIC_ADV_PARAMS,            // HCI_LE_SET_PERIODIC_ADVERTISING_PARAM (0x3E)
    HCI_CMD_SIZE_LE_SET_PERIODIC_ADV_DATA,              // HCI_LE_SET_PERIODIC_ADVERTISING_DATA (0x3F)
    HCI_CMD_SIZE_LE_SET_PERIODIC_ADV_ENABLE,            // HCI_LE_SET_PERIODIC_ADVERTISING_ENABLE (0x40)
    HCI_CMD_SIZE_LE_SET_EXT_SCAN_PARAMS,                // HCI_LE_SET_EXTENDED_SCAN_PARAM (0x41)
    HCI_CMD_SIZE_LE_SET_EXT_SCAN_ENABLE,                // HCI_LE_SET_EXTENDED_SCAN_ENABLE (0x42)
    HCI_CMD_SIZE_LE_EXT_CREATE_CONN,                    // HCI_LE_EXTENDED_CREATE_CONNECTION (0x43)
    HCI_CMD_SIZE_LE_PERIODIC_ADV_CREATE_SYNC,           // HCI_LE_PERIODIC_ADVERTISING_CREATE_SYNC (0x44)
    HCI_CMD_SIZE_LE_PERIODIC_ADV_CREATE_SYNC_CANCEL,    // HCI_LE_PERIODIC_ADVERTISING_CREATE_SYNC_CANCEL (0x45)
    HCI_CMD_SIZE_LE_PERIODIC_ADV_TERMINATE_SYNC,        // HCI_LE_PERIODIC_ADVERTISING_TERMINATE_SYNC (0x46)
    HCI_CMD_SIZE_LE_ADD_DEV_TO_PERIODIC_ADV_LIST,       // HCI_LE_ADD_DEV_TO_PERIODIC_ADVERTISER_LIST (0x47)
    0x00, // (0x48)
    HCI_CMD_SIZE_LE_CLEAR_PERIODIC_ADV_LIST,            // HCI_LE_CLEAR_PERIODIC_ADVERTISER_LIST (0x49)
    0x00, // (0x4A)
    HCI_CMD_SIZE_LE_READ_TRANSMIT_POWER,                // HCI_LE_READ_TRANSMIT_POWER (0x4B)
    0x00, // (0x4C)
    0x00, // (0x4D)
    HCI_CMD_SIZE_LE_SET_PRIVACY_MODE,                   // HCI_LE_SET_PRIVACY_MODE (0x4E)
    0x00, // (0x4F)
    0x00, // (0x50)
    0x00, // (0x51)
    0x00, // (0x52)
    0x00, // (0x53)
    0x00, // (0x54)
    0x00, // (0x55)
    0x00, // (0x56)
    0x00, // (0x57)
    0x00, // (0x58)
    HCI_CMD_SIZE_LE_SET_PERIODIC_ADV_RX_ENABLE,         // HCI_LE_SET_PERIODIC_ADVERTISING_RECEIVE_ENABLE (0x59)
    0x00, // (0x5A)
    0x00, // (0x5B)
    0x00, // (0x5C)
    0x00, // (0x5D)
    0x00, // (0x5E)
    0x00, // (0x5F)
    0x00, // (0x60)
    0x00, // (0x61)
    0x00, // (0x62)
    0x00, // (0x63)
    0x00, // (0x64)
    0x00, // (0x65)
    0x00, // (0x66)
    0x00, // (0x67)
    0x00, // (0x68)
    0x00, // (0x69)
    0x00, // (0x6A)
    0x00, // (0x6B)
    0x00, // (0x6C)
    0x00, // (0x6D)
    0x00, // (0x6E)
    0x00, // (0x6F)
    0x00, // (0x70)
    0x00, // (0x71)
    0x00, // (0x72)
    0x00, // (0x73)
    0x00, // (0x74)
    0x00, // (0x75)
    HCI_CMD_SIZE_LE_ENHANCED_READ_TRANSMIT_POWER_LEVEL, // HCI_LE_ENHANCED_READ_TRANSMIT_POWER_LEVEL (0x76)
    HCI_CMD_SIZE_LE_READ_REMOTE_TRANSMIT_POWER_LEVEL,   // HCI_LE_READ_REMOTE_TRANSMIT_POWER_LEVEL (0x77)
    0x00, // (0x78)
    0x00, // (0x79)
    HCI_CMD_SIZE_LE_SET_TRANSMIT_POWER_REPORTING_ENABLE,// HCI_LE_SET_TRANSMIT_POWER_REPORTING_ENABLE (0x7A)
    HCI_CMD_SIZE_LE_TRANSMITTER_TEST_V4                 // HCI_LE_TRANSMITTER_TEST_V4 (0x7B)
};

uint8_t s_hciLeMetaEvtSize[] =
{
    0x00,   // Reserved (0x00)
    HCI_EVT_SIZE_LE_CONNECT_COMPLETE,                   // HCI_LE_CONNECT_COMPLETE (0x01)
    HCI_EVT_SIZE_LE_ADV_REPORT,                         // HCI_LE_ADVERTISING_REPORT (0x02)
    HCI_EVT_SIZE_LE_CONNECTION_UPDATE_COMPLETE,         // HCI_LE_CONNECTION_UPDATE_COMPLETE (0x03)
    HCI_EVT_SIZE_LE_READ_REMOTE_FEATURES_COMPLETE,      // HCI_LE_READ_REMOTE_FEATURES_COMPLETE (0x04)
    HCI_EVT_SIZE_LE_LONG_TERM_KEY_REQUEST,              // HCI_LE_LONG_TERM_KEY_REQUEST (0x05)
    HCI_EVT_SIZE_LE_REMOTE_CONNECTION_UPDATE_REQUEST,   // HCI_LE_REMOTE_CONNECTION_PARAMETER_REQUEST (0x06)
    HCI_EVT_SIZE_LE_DATA_LENGTH_CHANGE,                 // HCI_LE_DATA_LENGTH_CHANGE (0x07)
    0x00,   // (0x08)
    0x00,   // (0x09)
    0x00,   // (0x0A)
    HCI_EVT_SIZE_LE_DIRECT_ADV_REPORT,                  // HCI_LE_DIRECT_ADVERTISING_REPORT (0x0B)
    HCI_EVT_SIZE_LE_PHY_UPDATE_COMPLETE,                // HCI_LE_PHY_UPDATE_COMPLETE (0x0C)
    HCI_EVT_SIZE_LE_EXTENDED_ADV_REPORT,                // HCI_LE_EXTENDED_ADVERTISING_REPORT (0x0D)
    HCI_EVT_SIZE_LE_PERIODIC_ADV_SYNC_ESTABLISHED,      // HCI_LE_PERIODIC_ADVERTISING_SYNC_ESTABLISHED (0x0E)
    HCI_EVT_SIZE_LE_PERIODIC_ADV_REPORT,                // HCI_LE_PERIODIC_ADVERTISING_REPORT (0x0F)
    HCI_EVT_SIZE_LE_PERIODIC_ADV_SYNC_LOST,             // HCI_LE_PERIODIC_ADVERTISING_SYNC_LOST (0x10)
    HCI_EVT_SIZE_LE_SCAN_TIMEOUT,                       // HCI_LE_SCAN_TIMEOUT (0x11)
    HCI_EVT_SIZE_LE_ADV_SET_TERMINATED,                 // HCI_LE_ADVERTISING_SET_TERMINATED (0x12)
    HCI_EVT_SIZE_LE_SCAN_REQUEST_RECEIVED               // HCI_LE_SCAN_REQUEST_RECEIVED (0x13)
};

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************

static void ble_log_Print(uint8_t opcode, uint16_t logLength, uint8_t *p_logPacket)
{
    if (g_logPrintCb)
    {
        g_logPrintCb(opcode, logLength, p_logPacket);
    }
}

static void ble_log_GenerateHciCommandComplete(uint16_t logId, uint16_t result, uint16_t paramsLength, uint8_t *p_returnParams)
{
    uint8_t *p_logPacket;
    uint8_t *p_buf;
    uint8_t eventCode = HCI_COMMAND_COMPLETE;
    uint8_t numberOfHciPacket = 1;
    uint8_t ogf;
    uint16_t logLength;
    uint16_t packetLength;
    uint16_t ocf;
    uint16_t logType = BT_LOG_TYPE_HCI_EVENT;

    ogf = (logId >> 10);
    ocf = (logId & 0x03FF);
    logLength = (LOG_PACKET_TYPE_SIZE+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+
                HCI_EVENT_CC_PARAMETER_NUMBER_SIZE+HCI_EVENT_CC_PARAMETER_OPCODE_SIZE+
                paramsLength);
    packetLength = LOG_PACKET_LENGTH_SIZE+logLength+LOG_PACKET_END_SIZE;
    p_logPacket = OSAL_Malloc(packetLength);

    if (p_logPacket == NULL)
    {
        return;
    }

    p_buf = p_logPacket;
    U16_TO_STREAM_LE(&p_buf, logLength);
    U16_TO_STREAM_LE(&p_buf, logType);
    U8_TO_STREAM(&p_buf, eventCode);
    U8_TO_STREAM(&p_buf, (uint8_t)(logLength-LOG_PACKET_TYPE_SIZE-HCI_EVENT_CODE_SIZE-HCI_EVENT_PARAMETER_LENGTH_SIZE));
    U8_TO_STREAM(&p_buf, numberOfHciPacket);
    U16_TO_STREAM_LE(&p_buf, logId);

    /* To do: mapping result code. */
    U8_TO_STREAM(&p_buf, (uint8_t)result);

    if (ogf == HCI_LE_CTRL)
    {
        if (ocf == HCI_LE_READ_CHANNEL_MAP)
        {
            VARIABLE_COPY_TO_STREAM(&p_buf, (p_returnParams+HCI_EVENT_CC_PARAMETER_STATUS_SIZE), (HCI_CC_EVT_SIZE_READ_CHANNEL_MAP-HCI_EVENT_CC_PARAMETER_STATUS_SIZE));
        }
        else if (ocf == HCI_LE_LONG_TERM_KEY_REQ_REPLY)
        {
            VARIABLE_COPY_TO_STREAM(&p_buf, (p_returnParams+HCI_EVENT_CC_PARAMETER_STATUS_SIZE), (HCI_CC_EVT_SIZE_LTK_REQUEST_REPLY-HCI_EVENT_CC_PARAMETER_STATUS_SIZE));
        }
        else if (ocf == HCI_LE_LONG_TERM_KEY_REQ_NEGATIVE_REPLY)
        {
            VARIABLE_COPY_TO_STREAM(&p_buf, (p_returnParams+HCI_EVENT_CC_PARAMETER_STATUS_SIZE), (HCI_CC_EVT_SIZE_LTK_REQUEST_NEGATIVE_REPLY-HCI_EVENT_CC_PARAMETER_STATUS_SIZE));
        }
        else if (ocf == HCI_LE_TEST_END)
        {
            VARIABLE_COPY_TO_STREAM(&p_buf, (p_returnParams+HCI_EVENT_CC_PARAMETER_STATUS_SIZE), (HCI_CC_EVT_SIZE_TEST_END-HCI_EVENT_CC_PARAMETER_STATUS_SIZE));
        }
        else if (ocf == HCI_LE_REMOTE_CONN_PARAM_REQ_REPLY)
        {
            VARIABLE_COPY_TO_STREAM(&p_buf, (p_returnParams+HCI_EVENT_CC_PARAMETER_STATUS_SIZE), (HCI_CC_EVT_SIZE_REMOTE_CONN_PARAM_REQ_REPLY-HCI_EVENT_CC_PARAMETER_STATUS_SIZE));
        }
        else if (ocf == HCI_LE_REMOTE_CONN_PARAM_REQ_NEGATIVE_REPLY)
        {
            VARIABLE_COPY_TO_STREAM(&p_buf, (p_returnParams+HCI_EVENT_CC_PARAMETER_STATUS_SIZE), (HCI_CC_EVT_SIZE_REMOTE_CONN_PARAM_REQ_NEG_REPLY-HCI_EVENT_CC_PARAMETER_STATUS_SIZE));
        }
        else if (ocf == HCI_LE_SET_DATA_LENGTH)
        {
            VARIABLE_COPY_TO_STREAM(&p_buf, (p_returnParams+HCI_EVENT_CC_PARAMETER_STATUS_SIZE), (HCI_CC_EVT_SIZE_SET_DATA_LENGTH-HCI_EVENT_CC_PARAMETER_STATUS_SIZE));
        }
        else if (ocf == HCI_LE_READ_PHY)
        {
            VARIABLE_COPY_TO_STREAM(&p_buf, (p_returnParams+HCI_EVENT_CC_PARAMETER_STATUS_SIZE), (HCI_CC_EVT_SIZE_READ_PHY-HCI_EVENT_CC_PARAMETER_STATUS_SIZE));
        }
        else if (ocf == HCI_LE_SET_EXTENDED_ADVERTISING_PARAM)
        {
            VARIABLE_COPY_TO_STREAM(&p_buf, (p_returnParams+HCI_EVENT_CC_PARAMETER_STATUS_SIZE), (HCI_CC_EVT_SIZE_SET_EXT_ADV_PARAMS-HCI_EVENT_CC_PARAMETER_STATUS_SIZE));
        }
        else if (ocf == HCI_LE_READ_TRANSMIT_POWER)
        {
            VARIABLE_COPY_TO_STREAM(&p_buf, (p_returnParams+HCI_EVENT_CC_PARAMETER_STATUS_SIZE), (HCI_CC_EVT_SIZE_READ_TRANSMIT_POWER-HCI_EVENT_CC_PARAMETER_STATUS_SIZE));
        }
        else if (ocf == HCI_LE_ENHANCED_READ_TRANSMIT_POWER_LEVEL)
        {
            VARIABLE_COPY_TO_STREAM(&p_buf, (p_returnParams+HCI_EVENT_CC_PARAMETER_STATUS_SIZE), (HCI_CC_EVT_SIZE_ENHANCED_READ_TRANSMIT_POWER-HCI_EVENT_CC_PARAMETER_STATUS_SIZE));
        }
        else if (ocf == HCI_LE_SET_TRANSMIT_POWER_REPORTING_ENABLE)
        {
            VARIABLE_COPY_TO_STREAM(&p_buf, (p_returnParams+HCI_EVENT_CC_PARAMETER_STATUS_SIZE), (HCI_CC_EVT_SIZE_SET_TRANSMIT_POWER_REPORTING_ENABLE-HCI_EVENT_CC_PARAMETER_STATUS_SIZE));
        }
    }
    else if (ogf == HCI_HC)
    {
        if (ocf == HCI_READ_AUTHENTICATED_PAYLOAD_TIMEOUT)
        {
            VARIABLE_COPY_TO_STREAM(&p_buf, (p_returnParams+HCI_EVENT_CC_PARAMETER_STATUS_SIZE), (HCI_CC_EVT_SIZE_READ_AUTH_PAYLOAD_TO-HCI_EVENT_CC_PARAMETER_STATUS_SIZE));
        }
        else if (ocf == HCI_WRITE_AUTHENTICATED_PAYLOAD_TIMEOUT)
        {
            VARIABLE_COPY_TO_STREAM(&p_buf, (p_returnParams+HCI_EVENT_CC_PARAMETER_STATUS_SIZE), (HCI_CC_EVT_SIZE_WRITE_AUTH_PAYLOAD_TO-HCI_EVENT_CC_PARAMETER_STATUS_SIZE));
        }
    }
    else if ((ogf == HCI_IP) && (ocf == HCI_READ_BD_ADDR))
    {
        VARIABLE_COPY_TO_STREAM(&p_buf, (p_returnParams+HCI_EVENT_CC_PARAMETER_STATUS_SIZE), (HCI_CC_EVT_SIZE_READ_BD_ADDR-HCI_EVENT_CC_PARAMETER_STATUS_SIZE));
    }
    else if ((ogf == HCI_SP) && (ocf == HCI_READ_RSSI))
    {
        VARIABLE_COPY_TO_STREAM(&p_buf, (p_returnParams+HCI_EVENT_CC_PARAMETER_STATUS_SIZE), (HCI_CC_EVT_SIZE_READ_RSSI-HCI_EVENT_CC_PARAMETER_STATUS_SIZE));
    }

    U8_TO_STREAM(&p_buf, LOG_END_CHECK_BYTE_1);
    U8_TO_STREAM(&p_buf, LOG_END_CHECK_BYTE_2);
    ble_log_Print(BM70_OPCODE_HCI_EVT, packetLength, p_logPacket);
    OSAL_Free(p_logPacket);
}

static void ble_log_GenerateHciCommandStatus(uint16_t logId, uint16_t result)
{
    uint8_t *p_logPacket;
    uint8_t *p_buf;
    uint8_t eventCode = HCI_COMMAND_STATUS;
    uint8_t numberOfHciPacket = 1;
    uint16_t logLength;
    uint16_t packetLength;
    uint16_t logType = BT_LOG_TYPE_HCI_EVENT;

    logLength = (LOG_PACKET_TYPE_SIZE+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+
                HCI_EVENT_CS_PARAMETER_STATUS_SIZE+HCI_EVENT_CS_PARAMETER_NUMBER_SIZE+HCI_EVENT_CS_PARAMETER_OPCODE_SIZE);
    packetLength = LOG_PACKET_LENGTH_SIZE+logLength+LOG_PACKET_END_SIZE;
    p_logPacket = OSAL_Malloc(packetLength);

    if (p_logPacket == NULL)
    {
        return;
    }

    p_buf = p_logPacket;
    U16_TO_STREAM_LE(&p_buf, logLength);
    U16_TO_STREAM_LE(&p_buf, logType);
    U8_TO_STREAM(&p_buf, eventCode);
    U8_TO_STREAM(&p_buf, (logLength-LOG_PACKET_TYPE_SIZE-HCI_EVENT_CODE_SIZE-HCI_EVENT_PARAMETER_LENGTH_SIZE));

    /* To do: mapping result code. */
    U8_TO_STREAM(&p_buf, (uint8_t)result);

    U8_TO_STREAM(&p_buf, numberOfHciPacket);
    U16_TO_STREAM_LE(&p_buf, logId);
    U8_TO_STREAM(&p_buf, LOG_END_CHECK_BYTE_1);
    U8_TO_STREAM(&p_buf, LOG_END_CHECK_BYTE_2);
    ble_log_Print(BM70_OPCODE_HCI_EVT, packetLength, p_logPacket);
    OSAL_Free(p_logPacket);
}

static void ble_log_PackDisconnect(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_Disconnect_T *)p_log->p_logPayload)->connHandle);
    U8_TO_STREAM(&p_buf, ((HCI_Disconnect_T *)p_log->p_logPayload)->reason);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandStatus(p_log->logId, p_log->result);
}

static void ble_log_PackReadAuthenticatedPayloadTimeout(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_ReadAuthenticatedPayloadTimeout_T *)p_log->p_logPayload)->connHandle);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackWriteAuthenticatedPayloadTimeout(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_WriteAuthenticatedPayloadTimeout_T *)p_log->p_logPayload)->connHandle);
    U16_TO_STREAM_LE(&p_buf, ((HCI_WriteAuthenticatedPayloadTimeout_T *)p_log->p_logPayload)->authenticatedPayloadTimeout);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackReadBdAddr(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackReadRssi(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_ReadRssi_T *)p_log->p_logPayload)->connHandle);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackSetRandomAddr(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_SetRandomAddress_T *)p_log->p_logPayload)->randomAddr, HCI_BD_ADDRESS_LEN);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackSetAdvParams(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetAdvertisingParameters_T *)p_log->p_logPayload)->intervalMin);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetAdvertisingParameters_T *)p_log->p_logPayload)->intervalMax);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetAdvertisingParameters_T *)p_log->p_logPayload)->type);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetAdvertisingParameters_T *)p_log->p_logPayload)->ownAddrType);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetAdvertisingParameters_T *)p_log->p_logPayload)->peerAddrType);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_SetAdvertisingParameters_T *)p_log->p_logPayload)->peerAddr, HCI_BD_ADDRESS_LEN);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetAdvertisingParameters_T *)p_log->p_logPayload)->advChannelMap);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetAdvertisingParameters_T *)p_log->p_logPayload)->filterPolicy);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackSetAdvData(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_LE_SetAdvertisingData_T *)p_log->p_logPayload)->advLen);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_SetAdvertisingData_T *)p_log->p_logPayload)->advData, HCI_MAX_ADV_LENGTH);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackSetScanRspData(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_LE_SetScanResponseData_T *)p_log->p_logPayload)->advScanRspLen);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_SetScanResponseData_T *)p_log->p_logPayload)->advScanRspData, HCI_MAX_ADV_LENGTH);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackSetAdvEnable(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, *p_log->p_logPayload);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackSetScanParams(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_LE_SetScanParameters_T *)p_log->p_logPayload)->scanType);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetScanParameters_T *)p_log->p_logPayload)->scanInterval);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetScanParameters_T *)p_log->p_logPayload)->scanWindow);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetScanParameters_T *)p_log->p_logPayload)->ownAddrType);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetScanParameters_T *)p_log->p_logPayload)->filterPolicy);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackSetScanEnable(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_LE_SetScanEnable_T *)p_log->p_logPayload)->enable);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetScanEnable_T *)p_log->p_logPayload)->filterDuplicates);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackCreateConn(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_CreateConnection_T *)p_log->p_logPayload)->scanInterval);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_CreateConnection_T *)p_log->p_logPayload)->scanWindow);
    U8_TO_STREAM(&p_buf, ((HCI_LE_CreateConnection_T *)p_log->p_logPayload)->filterPolicy);
    U8_TO_STREAM(&p_buf, ((HCI_LE_CreateConnection_T *)p_log->p_logPayload)->peerAddrType);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_CreateConnection_T *)p_log->p_logPayload)->peerAddr, HCI_BD_ADDRESS_LEN);
    U8_TO_STREAM(&p_buf, ((HCI_LE_CreateConnection_T *)p_log->p_logPayload)->ownAddrType);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_CreateConnection_T *)p_log->p_logPayload)->intervalMin);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_CreateConnection_T *)p_log->p_logPayload)->intervalMax);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_CreateConnection_T *)p_log->p_logPayload)->latency);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_CreateConnection_T *)p_log->p_logPayload)->supervisionTimeout);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_CreateConnection_T *)p_log->p_logPayload)->minCeLength);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_CreateConnection_T *)p_log->p_logPayload)->maxCeLength);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandStatus(p_log->logId, p_log->result);
}

static void ble_log_PackCreateConnCancel(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackClearFilterAcceptList(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackAddDevToFilterAcceptList(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_LE_AddDeviceToFilterAcceptList_T *)p_log->p_logPayload)->addrType);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_AddDeviceToFilterAcceptList_T *)p_log->p_logPayload)->addr, HCI_BD_ADDRESS_LEN);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackConnectionUpdate(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ConnectionUpdate_T *)p_log->p_logPayload)->connHandle);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ConnectionUpdate_T *)p_log->p_logPayload)->intervalMin);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ConnectionUpdate_T *)p_log->p_logPayload)->intervalMax);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ConnectionUpdate_T *)p_log->p_logPayload)->latency);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ConnectionUpdate_T *)p_log->p_logPayload)->supervisionTimeout);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ConnectionUpdate_T *)p_log->p_logPayload)->minCeLength);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ConnectionUpdate_T *)p_log->p_logPayload)->maxCeLength);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandStatus(p_log->logId, p_log->result);
}

static void ble_log_PackSetHostChannelClassification(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    VARIABLE_COPY_TO_STREAM(&p_buf, (uint8_t *)p_log->p_logPayload, HCI_CMD_SIZE_LE_SET_HOST_CHANNEL_CLASSIFY);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackReadChannelMap(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ReadChannelMap_T *)p_log->p_logPayload)->connHandle);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackReadRemoteFeature(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ReadRemoteFeatures_T *)p_log->p_logPayload)->connHandle);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandStatus(p_log->logId, p_log->result);
}

static void ble_log_PackEnableEncryption(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_EnableEncryption_T *)p_log->p_logPayload)->connHandle);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_EnableEncryption_T *)p_log->p_logPayload)->randomNum, HCI_LTK_RANDOM_SIZE);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_EnableEncryption_T *)p_log->p_logPayload)->encryptedDiversifier);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_EnableEncryption_T *)p_log->p_logPayload)->longTermKey, HCI_MAX_KEY_LENGTH);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandStatus(p_log->logId, p_log->result);
}

static void ble_log_PackLtkReqReply(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_LongTermKeyRequestReply_T *)p_log->p_logPayload)->connHandle);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_LongTermKeyRequestReply_T *)p_log->p_logPayload)->longTermKey, HCI_MAX_KEY_LENGTH);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackLtkReqNegReply(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_LongTermKeyRequestNegativeReply_T *)p_log->p_logPayload)->connHandle);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackEndTest(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackRemoteConnParamReqReply(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_RemoteConnParamReqReply_T *)p_log->p_logPayload)->connHandle);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_RemoteConnParamReqReply_T *)p_log->p_logPayload)->intervalMin);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_RemoteConnParamReqReply_T *)p_log->p_logPayload)->intervalMax);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_RemoteConnParamReqReply_T *)p_log->p_logPayload)->latency);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_RemoteConnParamReqReply_T *)p_log->p_logPayload)->supervisionTimeout);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_RemoteConnParamReqReply_T *)p_log->p_logPayload)->minCeLength);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_RemoteConnParamReqReply_T *)p_log->p_logPayload)->maxCeLength);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackRemoteConnParamReqNegReply(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_RemoteConnParamReqNegReply_T *)p_log->p_logPayload)->connHandle);
    U8_TO_STREAM(&p_buf, ((HCI_LE_RemoteConnParamReqNegReply_T *)p_log->p_logPayload)->reason);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackSetDataLength(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetDataLength_T *)p_log->p_logPayload)->connHandle);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetDataLength_T *)p_log->p_logPayload)->txOctets);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetDataLength_T *)p_log->p_logPayload)->txTime);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackAddDevToResolvingList(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_LE_AddDeviceToResolvingList_T *)p_log->p_logPayload)->peerIdAddrType);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_AddDeviceToResolvingList_T *)p_log->p_logPayload)->peerIdAddr, HCI_BD_ADDRESS_LEN);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_AddDeviceToResolvingList_T *)p_log->p_logPayload)->peerIrk, HCI_MAX_KEY_LENGTH);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_AddDeviceToResolvingList_T *)p_log->p_logPayload)->localIrk, HCI_MAX_KEY_LENGTH);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackClearResolvingList(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackSetAddrResolutionEnable(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_LE_SetAddressResolutionEnable_T *)p_log->p_logPayload)->addressResolutionEnable);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_packSetRpaTimeout(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetRpaTimeout_T *)p_log->p_logPayload)->rpaTimeout);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackReadPhy(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ReadPhy_T *)p_log->p_logPayload)->connHandle);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackSetDefaultPhy(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_LE_SetDefaultPhy_T *)p_log->p_logPayload)->allPhys);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetDefaultPhy_T *)p_log->p_logPayload)->txPhys);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetDefaultPhy_T *)p_log->p_logPayload)->rxPhys);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackSetPhy(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetPhy_T *)p_log->p_logPayload)->connHandle);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetPhy_T *)p_log->p_logPayload)->allPhys);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetPhy_T *)p_log->p_logPayload)->txPhys);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetPhy_T *)p_log->p_logPayload)->rxPhys);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetPhy_T *)p_log->p_logPayload)->phyOptions);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandStatus(p_log->logId, p_log->result);
}

static void ble_log_PackReceiverTestV2(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_LE_ReceiverTestV2_T *)p_log->p_logPayload)->rxChannel);
    U8_TO_STREAM(&p_buf, ((HCI_LE_ReceiverTestV2_T *)p_log->p_logPayload)->phy);
    U8_TO_STREAM(&p_buf, ((HCI_LE_ReceiverTestV2_T *)p_log->p_logPayload)->modulationIndex);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackSetAdvSetRandomAddr(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_LE_SetAdvertisingSetRandomAddr_T *)p_log->p_logPayload)->advHandle);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_SetAdvertisingSetRandomAddr_T *)p_log->p_logPayload)->advRandomAddr, HCI_BD_ADDRESS_LEN);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackSetExtAdvParam(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingParameters_T *)p_log->p_logPayload)->advhandle);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetExtAdvertisingParameters_T *)p_log->p_logPayload)->evtProperies);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingParameters_T *)p_log->p_logPayload)->priIntervalMin, 3);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingParameters_T *)p_log->p_logPayload)->priIntervalMax, 3);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingParameters_T *)p_log->p_logPayload)->priChannelMap);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingParameters_T *)p_log->p_logPayload)->ownAddrType);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingParameters_T *)p_log->p_logPayload)->peerAddrType);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingParameters_T *)p_log->p_logPayload)->peerAddr, HCI_BD_ADDRESS_LEN);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingParameters_T *)p_log->p_logPayload)->filterPolicy);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingParameters_T *)p_log->p_logPayload)->txPower);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingParameters_T *)p_log->p_logPayload)->priPhy);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingParameters_T *)p_log->p_logPayload)->secMaxSkip);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingParameters_T *)p_log->p_logPayload)->secPhy);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingParameters_T *)p_log->p_logPayload)->advSid);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingParameters_T *)p_log->p_logPayload)->scanReqNotifiEnable);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackSetExtAdvData(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);
    uint8_t cmdParamLength;
    uint16_t logLength;

    /* Correct actual length */
    cmdParamLength = 1+1+1+1+((HCI_LE_SetExtAdvertisingData_T *)p_log->p_logPayload)->advLen;
    logLength = LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE+cmdParamLength;
    packetLength = LOG_PACKET_LENGTH_SIZE+logLength+LOG_PACKET_END_SIZE;
    *(p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE) = cmdParamLength;
    U16_TO_BUF_LE(p_logPacket, logLength);

    /* Pack packet */
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingData_T *)p_log->p_logPayload)->advHandle);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingData_T *)p_log->p_logPayload)->operation);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingData_T *)p_log->p_logPayload)->fragPreference);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingData_T *)p_log->p_logPayload)->advLen);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingData_T *)p_log->p_logPayload)->advData, ((HCI_LE_SetExtAdvertisingData_T *)p_log->p_logPayload)->advLen);
    U8_TO_STREAM(&p_buf, LOG_END_CHECK_BYTE_1);
    U8_TO_STREAM(&p_buf, LOG_END_CHECK_BYTE_2);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackSetExtScanRspData(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);
    uint8_t cmdParamLength;
    uint16_t logLength;

    /* Correct actual length */
    cmdParamLength = 1+1+1+1+((HCI_LE_SetExtScanRespData_T *)p_log->p_logPayload)->scanRspLen;
    logLength = LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE+cmdParamLength;
    packetLength = LOG_PACKET_LENGTH_SIZE+logLength+LOG_PACKET_END_SIZE;
    *(p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE) = cmdParamLength;
    U16_TO_BUF_LE(p_logPacket, logLength);

    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtScanRespData_T *)p_log->p_logPayload)->advHandle);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtScanRespData_T *)p_log->p_logPayload)->operation);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtScanRespData_T *)p_log->p_logPayload)->fragPreference);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtScanRespData_T *)p_log->p_logPayload)->scanRspLen);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_SetExtScanRespData_T *)p_log->p_logPayload)->scanRspData, ((HCI_LE_SetExtScanRespData_T *)p_log->p_logPayload)->scanRspLen);
    U8_TO_STREAM(&p_buf, LOG_END_CHECK_BYTE_1);
    U8_TO_STREAM(&p_buf, LOG_END_CHECK_BYTE_2);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackSetExtAdvEnable(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);
    uint8_t cmdParamLength, i;
    uint16_t logLength;

    /* Correct actual length */
    cmdParamLength = 1+1+4*((HCI_LE_SetExtAdvertisingEnable_T *)p_log->p_logPayload)->numOfSets;
    logLength = LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE+cmdParamLength;
    packetLength = LOG_PACKET_LENGTH_SIZE+logLength+LOG_PACKET_END_SIZE;
    *(p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE) = cmdParamLength;
    U16_TO_BUF_LE(p_logPacket, logLength);

    /* Pack packet */
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingEnable_T *)p_log->p_logPayload)->enable);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingEnable_T *)p_log->p_logPayload)->numOfSets);
    for (i=0; i< ((HCI_LE_SetExtAdvertisingEnable_T *)p_log->p_logPayload)->numOfSets; i++)
    {
        U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingEnable_T *)p_log->p_logPayload)->advSet[i].advHandle);
        U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetExtAdvertisingEnable_T *)p_log->p_logPayload)->advSet[i].duration);
        U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingEnable_T *)p_log->p_logPayload)->advSet[i].maxExtAdvEvts);
    }
    U8_TO_STREAM(&p_buf, LOG_END_CHECK_BYTE_1);
    U8_TO_STREAM(&p_buf, LOG_END_CHECK_BYTE_2);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackRemoveAdvSet(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_LE_RemoveAdvertisingSet_T *)p_log->p_logPayload)->advHandle);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackClearAdvSet(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackSetPeriodicAdvParam(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_LE_SetPeriodicAdvertisingParams_T *)p_log->p_logPayload)->advHandle);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetPeriodicAdvertisingParams_T *)p_log->p_logPayload)->intervalMin);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetPeriodicAdvertisingParams_T *)p_log->p_logPayload)->intervalMax);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetPeriodicAdvertisingParams_T *)p_log->p_logPayload)->properties);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackSetPeriodicAdvData(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);
    uint8_t cmdParamLength;
    uint16_t logLength;

    /* Correct actual length */
    cmdParamLength = 1+1+1+((HCI_LE_SetPeriodicAdvertisingData_T *)p_log->p_logPayload)->advLen;
    logLength = LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE+cmdParamLength;
    packetLength = LOG_PACKET_LENGTH_SIZE+logLength+LOG_PACKET_END_SIZE;
    *(p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE) = cmdParamLength;
    U16_TO_BUF_LE(p_logPacket, logLength);

    /* Pack packet */
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetPeriodicAdvertisingData_T *)p_log->p_logPayload)->advHandle);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetPeriodicAdvertisingData_T *)p_log->p_logPayload)->operation);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetPeriodicAdvertisingData_T *)p_log->p_logPayload)->advLen);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_SetPeriodicAdvertisingData_T *)p_log->p_logPayload)->advData, ((HCI_LE_SetPeriodicAdvertisingData_T *)p_log->p_logPayload)->advLen);
    U8_TO_STREAM(&p_buf, LOG_END_CHECK_BYTE_1);
    U8_TO_STREAM(&p_buf, LOG_END_CHECK_BYTE_2);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackSetPeriodicAdvEnable(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_LE_SetPeriodicAdvertisingEnable_T *)p_log->p_logPayload)->enable);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetPeriodicAdvertisingEnable_T *)p_log->p_logPayload)->advHandle);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackSetExtScanParam(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);
    uint8_t cmdParamLength, usedPhys, i;
    uint16_t logLength;

    if (((HCI_LE_SetExtScanParams_T *)p_log->p_logPayload)->scanPhys == (HCI_PHY_OPTION_1M|HCI_PHY_OPTION_CODED))
    {
        usedPhys = 2;
    }
    else if ((((HCI_LE_SetExtScanParams_T *)p_log->p_logPayload)->scanPhys == HCI_PHY_OPTION_1M)|| (((HCI_LE_SetExtScanParams_T *)p_log->p_logPayload)->scanPhys == HCI_PHY_OPTION_CODED))
    {
        usedPhys = 1;
    }
    else
    {
        usedPhys = 0;
    }

    /* Correct actual length */
    cmdParamLength = 1+1+1+(usedPhys*5);
    logLength = LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE+cmdParamLength;
    packetLength = LOG_PACKET_LENGTH_SIZE+logLength+LOG_PACKET_END_SIZE;
    *(p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE) = cmdParamLength;
    U16_TO_BUF_LE(p_logPacket, logLength);

    /* Pack packet */
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtScanParams_T *)p_log->p_logPayload)->ownAddrType);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtScanParams_T *)p_log->p_logPayload)->scanFilterPolicy);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtScanParams_T *)p_log->p_logPayload)->scanPhys);

    for (i=0; i<usedPhys; i++)
    {
        U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtScanParams_T *)p_log->p_logPayload)->paramsByPhy[i].scanType);
        U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetExtScanParams_T *)p_log->p_logPayload)->paramsByPhy[i].scanInterval);
        U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetExtScanParams_T *)p_log->p_logPayload)->paramsByPhy[i].scanWindow);
    }

    U8_TO_STREAM(&p_buf, LOG_END_CHECK_BYTE_1);
    U8_TO_STREAM(&p_buf, LOG_END_CHECK_BYTE_2);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackSetExtScanEnable(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtScanEnable_T *)p_log->p_logPayload)->enable);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtScanEnable_T *)p_log->p_logPayload)->filterDuplicates);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetExtScanEnable_T *)p_log->p_logPayload)->duration);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetExtScanEnable_T *)p_log->p_logPayload)->period);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackExtCreateConn(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);
    uint8_t phys = ((HCI_LE_ExtCreateConnParam_T *)p_log->p_logPayload)->initPhys;
    uint8_t cmdParamLength, usedPhys, i;
    uint16_t logLength;

    if (phys == (HCI_PHY_OPTION_1M|HCI_PHY_OPTION_2M|HCI_PHY_OPTION_CODED))
    {
        usedPhys = 3;
    }
    else if ((phys == (HCI_PHY_OPTION_1M|HCI_PHY_OPTION_2M)) || (phys == (HCI_PHY_OPTION_1M|HCI_PHY_OPTION_CODED)) || (phys == (HCI_PHY_OPTION_2M|HCI_PHY_OPTION_CODED)))
    {
        usedPhys = 2;
    }
    else if ((phys == HCI_PHY_OPTION_1M) || (phys == HCI_PHY_OPTION_2M) || (phys == HCI_PHY_OPTION_CODED))
    {
        usedPhys = 1;
    }
    else
    {
        usedPhys = 0;
    }

    /* Correct actual length */
    cmdParamLength = 1+1+1+6+1+(usedPhys*16);
    logLength = LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE+cmdParamLength;
    packetLength = LOG_PACKET_LENGTH_SIZE+logLength+LOG_PACKET_END_SIZE;
    *(p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE) = cmdParamLength;
    U16_TO_BUF_LE(p_logPacket, logLength);

    /* Pack packet */
    U8_TO_STREAM(&p_buf, ((HCI_LE_ExtCreateConnParam_T *)p_log->p_logPayload)->initFilterPolicy);
    U8_TO_STREAM(&p_buf, ((HCI_LE_ExtCreateConnParam_T *)p_log->p_logPayload)->ownAddrType);
    U8_TO_STREAM(&p_buf, ((HCI_LE_ExtCreateConnParam_T *)p_log->p_logPayload)->peerAddrType);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_ExtCreateConnParam_T *)p_log->p_logPayload)->peerAddr, HCI_BD_ADDRESS_LEN);
    U8_TO_STREAM(&p_buf, ((HCI_LE_ExtCreateConnParam_T *)p_log->p_logPayload)->initPhys);

    for (i=0; i<usedPhys; i++)
    {
        U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ExtCreateConnParam_T *)p_log->p_logPayload)->paramsByPhy[i].scanInterval);
        U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ExtCreateConnParam_T *)p_log->p_logPayload)->paramsByPhy[i].scanWindow);
        U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ExtCreateConnParam_T *)p_log->p_logPayload)->paramsByPhy[i].intervalMin);
        U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ExtCreateConnParam_T *)p_log->p_logPayload)->paramsByPhy[i].intervalMax);
        U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ExtCreateConnParam_T *)p_log->p_logPayload)->paramsByPhy[i].latency);
        U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ExtCreateConnParam_T *)p_log->p_logPayload)->paramsByPhy[i].supervisionTimeout);
        U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ExtCreateConnParam_T *)p_log->p_logPayload)->paramsByPhy[i].minCeLength);
        U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ExtCreateConnParam_T *)p_log->p_logPayload)->paramsByPhy[i].maxCeLength);
    }

    U8_TO_STREAM(&p_buf, LOG_END_CHECK_BYTE_1);
    U8_TO_STREAM(&p_buf, LOG_END_CHECK_BYTE_2);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandStatus(p_log->logId, p_log->result);
}

static void ble_log_PackPeriodicAdvCreateSync(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_LE_PeriodicAdvertisingCreateSync_T *)p_log->p_logPayload)->options);
    U8_TO_STREAM(&p_buf, ((HCI_LE_PeriodicAdvertisingCreateSync_T *)p_log->p_logPayload)->advSid);
    U8_TO_STREAM(&p_buf, ((HCI_LE_PeriodicAdvertisingCreateSync_T *)p_log->p_logPayload)->advAddrType);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_PeriodicAdvertisingCreateSync_T *)p_log->p_logPayload)->advAddr, HCI_BD_ADDRESS_LEN);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_PeriodicAdvertisingCreateSync_T *)p_log->p_logPayload)->skip);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_PeriodicAdvertisingCreateSync_T *)p_log->p_logPayload)->syncTimeout);
    U8_TO_STREAM(&p_buf, ((HCI_LE_PeriodicAdvertisingCreateSync_T *)p_log->p_logPayload)->syncCteType);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandStatus(p_log->logId, p_log->result);
}

static void ble_log_PackPeriodicAdvCreateSyncCancel(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackPeriodicAdvTerminateSync(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_PeriodicAdvertisingTerminateSync_T *)p_log->p_logPayload)->syncHandle);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackAddDevToPeriodicAdvList(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_LE_AddDevToPeriodicAdvList_T *)p_log->p_logPayload)->advAddrType);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_AddDevToPeriodicAdvList_T *)p_log->p_logPayload)->advAddr, HCI_BD_ADDRESS_LEN);
    U8_TO_STREAM(&p_buf, ((HCI_LE_AddDevToPeriodicAdvList_T *)p_log->p_logPayload)->advSid);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackClearPeriodicAdvList(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackReadTransmitPower(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackSetPrivacyMode(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_LE_SetPrivacyMode_T *)p_log->p_logPayload)->peerIdentifyAddrType);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_SetPrivacyMode_T *)p_log->p_logPayload)->peerIdentifyAddr, HCI_BD_ADDRESS_LEN);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetPrivacyMode_T *)p_log->p_logPayload)->privacyMode);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackSetPeriodicAdvReceiveEnable(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetPeriodicAdvReceiveEnable_T *)p_log->p_logPayload)->syncHandle);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetPeriodicAdvReceiveEnable_T *)p_log->p_logPayload)->enable);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackEnhancedReadTransmitPowerLevel(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_EnhancedReadTransmitPowerLevel_T *)p_log->p_logPayload)->connHandle);
    U8_TO_STREAM(&p_buf, ((HCI_LE_EnhancedReadTransmitPowerLevel_T *)p_log->p_logPayload)->phy);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackReadRemoteTransmitPowerLevel(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ReadRemoteTransmitPowerLevel_T *)p_log->p_logPayload)->connHandle);
    U8_TO_STREAM(&p_buf, ((HCI_LE_ReadRemoteTransmitPowerLevel_T *)p_log->p_logPayload)->phy);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandStatus(p_log->logId, p_log->result);
}

static void ble_log_PackSetTransmitPowerReportingEnable(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetTransmitPowerReportingEnable_T *)p_log->p_logPayload)->connHandle);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetTransmitPowerReportingEnable_T *)p_log->p_logPayload)->localEnable);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetTransmitPowerReportingEnable_T *)p_log->p_logPayload)->remoteEnable);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

static void ble_log_PackTransmitterTestV4(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t     *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);
    uint8_t     cmdParamLength;
    uint16_t    logLength;

    /* Correct actual length */
    cmdParamLength = 1+1+1+1+1+1+1+((HCI_LE_TransmitterTestV4_T *)p_log->p_logPayload)->switchingPatternLen+1;
    logLength = LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE+cmdParamLength;
    packetLength = LOG_PACKET_LENGTH_SIZE+logLength+LOG_PACKET_END_SIZE;
    *(p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE) = cmdParamLength;
    U16_TO_BUF_LE(p_logPacket, logLength);

    U8_TO_STREAM(&p_buf, ((HCI_LE_TransmitterTestV4_T *)p_log->p_logPayload)->txChannel);
    U8_TO_STREAM(&p_buf, ((HCI_LE_TransmitterTestV4_T *)p_log->p_logPayload)->testDataLen);
    U8_TO_STREAM(&p_buf, ((HCI_LE_TransmitterTestV4_T *)p_log->p_logPayload)->packetPayload);
    U8_TO_STREAM(&p_buf, ((HCI_LE_TransmitterTestV4_T *)p_log->p_logPayload)->phy);
    U8_TO_STREAM(&p_buf, ((HCI_LE_TransmitterTestV4_T *)p_log->p_logPayload)->cteLen);
    U8_TO_STREAM(&p_buf, ((HCI_LE_TransmitterTestV4_T *)p_log->p_logPayload)->cteType);
    U8_TO_STREAM(&p_buf, ((HCI_LE_TransmitterTestV4_T *)p_log->p_logPayload)->switchingPatternLen);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_TransmitterTestV4_T *)p_log->p_logPayload)->antennaIds, ((HCI_LE_TransmitterTestV4_T *)p_log->p_logPayload)->switchingPatternLen);
    U8_TO_STREAM(&p_buf, ((HCI_LE_TransmitterTestV4_T *)p_log->p_logPayload)->txPowerLevel);
    U8_TO_STREAM(&p_buf, LOG_END_CHECK_BYTE_1);
    U8_TO_STREAM(&p_buf, LOG_END_CHECK_BYTE_2);
    ble_log_Print(BM70_OPCODE_HCI_CMD, packetLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, p_log->result, p_log->paramsLength, p_log->p_returnParams);
}

void ble_log_ProcHciCommand(BLE_Log_T *p_log)
{
    uint8_t *p_logPacket;
    uint8_t *p_buf;
    uint8_t ogf;
    uint16_t evtParamsLength;
    uint16_t logLength;
    uint16_t packetLength;
    uint16_t ocf;

    ogf = (p_log->logId >> 10);
    ocf = (p_log->logId & 0x03FF);

    if (ogf == HCI_LE_CTRL)
    {
        evtParamsLength = s_hciCmdPktSize[ocf-1];
    }
    else
    {
        if ((ogf == HCI_LC) && (ocf == HCI_DISCONNECT))
        {
            evtParamsLength = HCI_CMD_SIZE_DISCONNECT;
        }
        else if (ogf == HCI_HC)
        {
            if (ocf == HCI_READ_AUTHENTICATED_PAYLOAD_TIMEOUT)
            {
                evtParamsLength = HCI_CMD_SIZE_READ_AUTH_PAYLOAD_TO;
            }
            else if (ocf == HCI_WRITE_AUTHENTICATED_PAYLOAD_TIMEOUT)
            {
                evtParamsLength = HCI_CMD_SIZE_WRITE_AUTH_PAYLOAD_TO;
            }
            else
            {
                return;
            }
        }
        else if ((ogf == HCI_IP) && (ocf == HCI_READ_BD_ADDR))
        {
            evtParamsLength = HCI_CMD_SIZE_READ_BD_ADDR;
        }
        else if ((ogf == HCI_SP) && (ocf == HCI_READ_RSSI))
        {
            evtParamsLength = HCI_CMD_SIZE_READ_RSSI;
        }
        else
        {
            return;
        }
    }

    logLength = LOG_PACKET_TYPE_SIZE+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE+evtParamsLength;
    packetLength = LOG_PACKET_LENGTH_SIZE+logLength+LOG_PACKET_END_SIZE;
    p_logPacket = OSAL_Malloc(packetLength);

    if (p_logPacket == NULL)
    {
        return;
    }

    p_buf = p_logPacket;
    U16_TO_STREAM_LE(&p_buf, logLength);
    U16_TO_STREAM_LE(&p_buf, p_log->logType);
    U16_TO_STREAM_LE(&p_buf, p_log->logId);
    U8_TO_STREAM(&p_buf, evtParamsLength);
    *(p_logPacket+packetLength-LOG_PACKET_END_SIZE) = LOG_END_CHECK_BYTE_1;
    *(p_logPacket+packetLength-LOG_PACKET_END_SIZE+1) = LOG_END_CHECK_BYTE_2;

    if (ogf == HCI_LE_CTRL)
    {
        if (s_hciCmdFun[ocf] != NULL)
        {
            s_hciCmdFun[ocf](p_log, p_logPacket, packetLength);
        }
    }
    else
    {
        if ((ogf == HCI_LC) && (ocf == HCI_DISCONNECT))
        {
            ble_log_PackDisconnect(p_log, p_logPacket, packetLength);
        }
        else if (ogf == HCI_HC)
        {
            if (ocf == HCI_READ_AUTHENTICATED_PAYLOAD_TIMEOUT)
            {
                ble_log_PackReadAuthenticatedPayloadTimeout(p_log, p_logPacket, packetLength);
            }
            else if (ocf == HCI_WRITE_AUTHENTICATED_PAYLOAD_TIMEOUT)
            {
                ble_log_PackWriteAuthenticatedPayloadTimeout(p_log, p_logPacket, packetLength);
            }
        }
        else if ((ogf == HCI_IP) && (ocf == HCI_READ_BD_ADDR))
        {
            ble_log_PackReadBdAddr(p_log, p_logPacket, packetLength);
        }
        else if ((ogf == HCI_SP) && (ocf == HCI_READ_RSSI))
        {
            ble_log_PackReadRssi(p_log, p_logPacket, packetLength);
        }
    }

    OSAL_Free(p_logPacket);
}

static void ble_log_PackDiscComplete(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_EvtDisconnect_T *)p_log->p_logPayload)->status);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtDisconnect_T *)p_log->p_logPayload)->connHandle);
    U8_TO_STREAM(&p_buf, ((HCI_EvtDisconnect_T *)p_log->p_logPayload)->reason);
    ble_log_Print(BM70_OPCODE_HCI_EVT, packetLength, p_logPacket);
}

static void ble_log_PackNumberOfCompletedPacket(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE);
    uint8_t i;

    U8_TO_STREAM(&p_buf, ((HCI_EvtNumOfCompletePacket_T *)p_log->p_logPayload)->numHandles);
    for (i=0; i<((HCI_EvtNumOfCompletePacket_T *)p_log->p_logPayload)->numHandles; i++)
    {
        U16_TO_STREAM_LE(&p_buf, ((HCI_EvtNumOfCompletePacket_T *)p_log->p_logPayload)->completePacket[i].connHandle);
        U16_TO_STREAM_LE(&p_buf, ((HCI_EvtNumOfCompletePacket_T *)p_log->p_logPayload)->completePacket[i].numPackets);
    }
    ble_log_Print(BM70_OPCODE_HCI_EVT, packetLength, p_logPacket);
}

static void ble_log_PackConnectComplete(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+HCI_EVENT_LE_PARAMETER_SUBCODE_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_EvtLeEnhanceConnect_T *)p_log->p_logPayload)->status);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeEnhanceConnect_T *)p_log->p_logPayload)->connHandle);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeEnhanceConnect_T *)p_log->p_logPayload)->role);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeEnhanceConnect_T *)p_log->p_logPayload)->peerAddrType);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_EvtLeEnhanceConnect_T *)p_log->p_logPayload)->peerAddr, HCI_BD_ADDRESS_LEN);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeEnhanceConnect_T *)p_log->p_logPayload)->connInterval);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeEnhanceConnect_T *)p_log->p_logPayload)->connLatency);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeEnhanceConnect_T *)p_log->p_logPayload)->supervisionTimeout);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeEnhanceConnect_T *)p_log->p_logPayload)->centralClkAcc);
    ble_log_Print(BM70_OPCODE_HCI_EVT, packetLength, p_logPacket);
}

static void ble_log_PackAdvReport(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+HCI_EVENT_LE_PARAMETER_SUBCODE_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_EvtLeAdvReport_T *)p_log->p_logPayload)->numReports);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeAdvReport_T *)p_log->p_logPayload)->reports[0].eventType);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeAdvReport_T *)p_log->p_logPayload)->reports[0].addrType);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_EvtLeAdvReport_T *)p_log->p_logPayload)->reports[0].addr, HCI_BD_ADDRESS_LEN);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeAdvReport_T *)p_log->p_logPayload)->reports[0].advLength);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_EvtLeAdvReport_T *)p_log->p_logPayload)->reports[0].advData, ((HCI_EvtLeAdvReport_T *)p_log->p_logPayload)->reports[0].advLength);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeAdvReport_T *)p_log->p_logPayload)->reports[0].rssi);
    ble_log_Print(BM70_OPCODE_HCI_EVT, packetLength, p_logPacket);
}

static void ble_log_PackConnectionUpdateComplete(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+HCI_EVENT_LE_PARAMETER_SUBCODE_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_EvtLeConnParamUpdate_T *)p_log->p_logPayload)->status);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeConnParamUpdate_T *)p_log->p_logPayload)->connHandle);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeConnParamUpdate_T *)p_log->p_logPayload)->connInterval);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeConnParamUpdate_T *)p_log->p_logPayload)->connLatency);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeConnParamUpdate_T *)p_log->p_logPayload)->supervisionTimeout);
    ble_log_Print(BM70_OPCODE_HCI_EVT, packetLength, p_logPacket);
}

static void ble_log_PackReadRemoteFeaturesComplete(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+HCI_EVENT_LE_PARAMETER_SUBCODE_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_EvtLeReadRemoteFeature_T *)p_log->p_logPayload)->status);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeReadRemoteFeature_T *)p_log->p_logPayload)->connHandle);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_EvtLeReadRemoteFeature_T *)p_log->p_logPayload)->leFeatures, HCI_FEATURES_BITMASK_LENGTH);
    ble_log_Print(BM70_OPCODE_HCI_EVT, packetLength, p_logPacket);
}

static void ble_log_PackLtkRequest(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+HCI_EVENT_LE_PARAMETER_SUBCODE_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeLongTermKeyReq_T *)p_log->p_logPayload)->connHandle);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_EvtLeLongTermKeyReq_T *)p_log->p_logPayload)->randomNum, HCI_LTK_RANDOM_SIZE);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeLongTermKeyReq_T *)p_log->p_logPayload)->encDiver);
    ble_log_Print(BM70_OPCODE_HCI_EVT, packetLength, p_logPacket);
}

static void ble_log_PackRemoteConnectionUpdateRequest(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+HCI_EVENT_LE_PARAMETER_SUBCODE_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeRemoteConnParamsReq_T *)p_log->p_logPayload)->connHandle);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeRemoteConnParamsReq_T *)p_log->p_logPayload)->intervalMin);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeRemoteConnParamsReq_T *)p_log->p_logPayload)->intervalMax);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeRemoteConnParamsReq_T *)p_log->p_logPayload)->latency);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeRemoteConnParamsReq_T *)p_log->p_logPayload)->supervisionTimeout);
    ble_log_Print(BM70_OPCODE_HCI_EVT, packetLength, p_logPacket);
}

static void ble_log_PackDataLengthChange(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+HCI_EVENT_LE_PARAMETER_SUBCODE_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeDataLengthChange_T *)p_log->p_logPayload)->connHandle);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeDataLengthChange_T *)p_log->p_logPayload)->maxTxOctects);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeDataLengthChange_T *)p_log->p_logPayload)->maxTxTime);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeDataLengthChange_T *)p_log->p_logPayload)->maxRxOctets);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeDataLengthChange_T *)p_log->p_logPayload)->maxRxTime);
    ble_log_Print(BM70_OPCODE_HCI_EVT, packetLength, p_logPacket);
}

static void ble_log_PackDirectAdvReport(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+HCI_EVENT_LE_PARAMETER_SUBCODE_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_EvtLeDirectAdvReport_T *)p_log->p_logPayload)->numReports);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeDirectAdvReport_T *)p_log->p_logPayload)->reports[0].eventType);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeDirectAdvReport_T *)p_log->p_logPayload)->reports[0].addrType);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_EvtLeDirectAdvReport_T *)p_log->p_logPayload)->reports[0].addr, HCI_BD_ADDRESS_LEN);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeDirectAdvReport_T *)p_log->p_logPayload)->reports[0].directAddrType);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_EvtLeDirectAdvReport_T *)p_log->p_logPayload)->reports[0].directAddr, HCI_BD_ADDRESS_LEN);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeDirectAdvReport_T *)p_log->p_logPayload)->reports[0].rssi);
    ble_log_Print(BM70_OPCODE_HCI_EVT, packetLength, p_logPacket);
}

static void ble_log_PackPhyUpdateComplete(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+HCI_EVENT_LE_PARAMETER_SUBCODE_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_EvtLePhyUpdate_T *)p_log->p_logPayload)->status);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLePhyUpdate_T *)p_log->p_logPayload)->connHandle);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLePhyUpdate_T *)p_log->p_logPayload)->txPhy);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLePhyUpdate_T *)p_log->p_logPayload)->rxPhy);
    ble_log_Print(BM70_OPCODE_HCI_EVT, packetLength, p_logPacket);
}

static void ble_log_PackExtAdvReport(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+HCI_EVENT_LE_PARAMETER_SUBCODE_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_EvtLeExtAdvReport_T *)p_log->p_logPayload)->numReports);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeExtAdvReport_T *)p_log->p_logPayload)->reports[0].eventType);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeExtAdvReport_T *)p_log->p_logPayload)->reports[0].addrType);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_EvtLeExtAdvReport_T *)p_log->p_logPayload)->reports[0].addr, HCI_BD_ADDRESS_LEN);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeExtAdvReport_T *)p_log->p_logPayload)->reports[0].priPhy);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeExtAdvReport_T *)p_log->p_logPayload)->reports[0].secPhy);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeExtAdvReport_T *)p_log->p_logPayload)->reports[0].sid);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeExtAdvReport_T *)p_log->p_logPayload)->reports[0].txPower);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeExtAdvReport_T *)p_log->p_logPayload)->reports[0].rssi);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeExtAdvReport_T *)p_log->p_logPayload)->reports[0].periodicAdvInterval);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeExtAdvReport_T *)p_log->p_logPayload)->reports[0].directAddrType);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_EvtLeExtAdvReport_T *)p_log->p_logPayload)->reports[0].directAddr, HCI_BD_ADDRESS_LEN);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeExtAdvReport_T *)p_log->p_logPayload)->reports[0].advLength);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_EvtLeExtAdvReport_T *)p_log->p_logPayload)->reports[0].advData, ((HCI_EvtLeExtAdvReport_T *)p_log->p_logPayload)->reports[0].advLength);
    ble_log_Print(BM70_OPCODE_HCI_EVT, packetLength, p_logPacket);
}

static void ble_log_PackPeriodicAdvSyncEstablished(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+HCI_EVENT_LE_PARAMETER_SUBCODE_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_EvtLePeriodicAdvSyncEst_T *)p_log->p_logPayload)->status);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLePeriodicAdvSyncEst_T *)p_log->p_logPayload)->syncHandle);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLePeriodicAdvSyncEst_T *)p_log->p_logPayload)->advSid);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLePeriodicAdvSyncEst_T *)p_log->p_logPayload)->advAddrType);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_EvtLePeriodicAdvSyncEst_T *)p_log->p_logPayload)->advAddr, HCI_BD_ADDRESS_LEN);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLePeriodicAdvSyncEst_T *)p_log->p_logPayload)->advPhy);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLePeriodicAdvSyncEst_T *)p_log->p_logPayload)->periodicAdvInterval);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLePeriodicAdvSyncEst_T *)p_log->p_logPayload)->advClkAccuracy);
    ble_log_Print(BM70_OPCODE_HCI_EVT, packetLength, p_logPacket);
}

static void ble_log_PackPeriodicAdvReport(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+HCI_EVENT_LE_PARAMETER_SUBCODE_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLePeriodicAdvReport_T *)p_log->p_logPayload)->syncHandle);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLePeriodicAdvReport_T *)p_log->p_logPayload)->txPower);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLePeriodicAdvReport_T *)p_log->p_logPayload)->rssi);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLePeriodicAdvReport_T *)p_log->p_logPayload)->cteType);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLePeriodicAdvReport_T *)p_log->p_logPayload)->dataStatus);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLePeriodicAdvReport_T *)p_log->p_logPayload)->advLength);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_EvtLePeriodicAdvReport_T *)p_log->p_logPayload)->advData, ((HCI_EvtLePeriodicAdvReport_T *)p_log->p_logPayload)->advLength);
    ble_log_Print(BM70_OPCODE_HCI_EVT, packetLength, p_logPacket);
}

static void ble_log_PackPeriodicAdvSyncLost(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+HCI_EVENT_LE_PARAMETER_SUBCODE_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLePeriodicAdvSyncLost_T *)p_log->p_logPayload)->syncHandle);
    ble_log_Print(BM70_OPCODE_HCI_EVT, packetLength, p_logPacket);
}

static void ble_log_PackScanTimeout(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    ble_log_Print(BM70_OPCODE_HCI_EVT, packetLength, p_logPacket);
}

static void ble_log_PackAdvSetTerminated(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+HCI_EVENT_LE_PARAMETER_SUBCODE_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_EvtLeAdvSetTerminated_T *)p_log->p_logPayload)->status);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeAdvSetTerminated_T *)p_log->p_logPayload)->advHandle);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeAdvSetTerminated_T *)p_log->p_logPayload)->connHandle);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeAdvSetTerminated_T *)p_log->p_logPayload)->numComplExtAdvEvents);
    ble_log_Print(BM70_OPCODE_HCI_EVT, packetLength, p_logPacket);
}

static void ble_log_PackScanRequestReceived(BLE_Log_T *p_log, uint8_t *p_logPacket, uint16_t packetLength)
{
    uint8_t *p_buf = (p_logPacket+LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+HCI_EVENT_LE_PARAMETER_SUBCODE_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_EvtLeScanReqReceived_T *)p_log->p_logPayload)->advHandle);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeScanReqReceived_T *)p_log->p_logPayload)->scannerAddrType);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_EvtLeScanReqReceived_T *)p_log->p_logPayload)->scannerAddr, HCI_BD_ADDRESS_LEN);
    ble_log_Print(BM70_OPCODE_HCI_EVT, packetLength, p_logPacket);
}

void ble_log_ProcHciEvent(BLE_Log_T *p_log)
{
    uint8_t *p_logPacket;
    uint8_t *p_buf;
    uint8_t evtParamSize;
    uint8_t evtCode, subEvtCode;
    uint16_t logLength;
    uint16_t packetLength;

    evtCode = (uint8_t)p_log->logId;

    if (evtCode == HCI_DISCONNECTION_COMPLETE)
    {
        evtParamSize = HCI_EVT_SIZE_DISCONNECTION;
    }
    else if (evtCode == HCI_ENCRYPTION_CHANGE)
    {
        evtParamSize = HCI_EVT_SIZE_ENCRYPTION_CHANGE;
    }
    else if (evtCode == HCI_NBR_OF_COMPLETED_PACKETS)
    {
        evtParamSize = 1 + (((HCI_EvtNumOfCompletePacket_T *)p_log->p_logPayload)->numHandles*sizeof(HCI_CompletePacket_T));
    }
    else if (evtCode == HCI_AUTHENTICATED_PAYLOAD_TIMEOUT_EXPIRED)
    {
        evtParamSize = HCI_EVT_SIZE_AUTH_PAYLOAD_TO_EXPIRED;
    }
    else
    {
        /* HCI_LE_META_EVENT */
        subEvtCode = (uint8_t)(p_log->logId>>8);
        evtParamSize = sizeof(subEvtCode) + s_hciLeMetaEvtSize[subEvtCode];

        /* Adjust event length of specific event */
        if (subEvtCode == HCI_LE_ADVERTISING_REPORT)
        {
            evtParamSize -= (HCI_MAX_ADV_LENGTH-((HCI_EvtLeAdvReport_T *)p_log->p_logPayload)->reports[0].advLength);
        }
        else if (subEvtCode == HCI_LE_EXTENDED_ADVERTISING_REPORT)
        {
            evtParamSize -= (HCI_EVT_MAX_EXTENDED_ADVERTISING_DATA_LENGTH-((HCI_EvtLeExtAdvReport_T *)p_log->p_logPayload)->reports[0].advLength);
        }
        else if (subEvtCode == HCI_LE_PERIODIC_ADVERTISING_REPORT)
        {
            evtParamSize -= (HCI_EVT_MAX_PERIODIC_ADVERTISING_DATA_LENGTH-((HCI_EvtLePeriodicAdvReport_T *)p_log->p_logPayload)->advLength);
        }
    }

    logLength = LOG_PACKET_TYPE_SIZE+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+evtParamSize;
    packetLength = LOG_PACKET_LENGTH_SIZE+logLength+LOG_PACKET_END_SIZE;
    p_logPacket = OSAL_Malloc(packetLength);

    if (p_logPacket == NULL)
    {
        return;
    }

    p_buf = p_logPacket;
    U16_TO_STREAM_LE(&p_buf, logLength);
    U16_TO_STREAM_LE(&p_buf, p_log->logType);
    U8_TO_STREAM(&p_buf, evtCode);
    U8_TO_STREAM(&p_buf, evtParamSize);
    *(p_logPacket+packetLength-LOG_PACKET_END_SIZE) = LOG_END_CHECK_BYTE_1;
    *(p_logPacket+packetLength-LOG_PACKET_END_SIZE+1) = LOG_END_CHECK_BYTE_2;

    if (evtCode == HCI_DISCONNECTION_COMPLETE)
    {
        ble_log_PackDiscComplete(p_log, p_logPacket, packetLength);
    }
    else if (evtCode == HCI_ENCRYPTION_CHANGE)
    {
        ;
    }
    else if (evtCode == HCI_NBR_OF_COMPLETED_PACKETS)
    {
        ble_log_PackNumberOfCompletedPacket(p_log, p_logPacket, packetLength);
    }
    else if (evtCode == HCI_AUTHENTICATED_PAYLOAD_TIMEOUT_EXPIRED)
    {
        ;
    }
    else
    {
        /* HCI_LE_META_EVENT */
        U8_TO_STREAM(&p_buf, subEvtCode);
        if (s_hciLeMetaEvtFun[subEvtCode] != NULL)
        {
            s_hciLeMetaEvtFun[subEvtCode](p_log, p_logPacket, packetLength);
        }
    }

    OSAL_Free(p_logPacket);
}

void ble_log_ProcHciAcl(BLE_Log_T *p_log)
{
    uint8_t *p_logPacket;
    uint8_t *p_buf;
    uint16_t logLength;
    uint16_t packetLength;

    logLength = LOG_PACKET_TYPE_SIZE+HCI_ACL_HEADER_SIZE+HCI_ACL_PKT_PARAMETER_LENGTH_SIZE+p_log->payloadLength;
    packetLength = LOG_PACKET_LENGTH_SIZE+logLength+LOG_PACKET_END_SIZE;
    p_logPacket = OSAL_Malloc(packetLength);

    if (p_logPacket == NULL)
    {
        return;
    }

    p_buf = p_logPacket;
    U16_TO_STREAM_LE(&p_buf, logLength);
    U16_TO_STREAM_LE(&p_buf, p_log->logType);
    U16_TO_STREAM_LE(&p_buf, p_log->logId); // Reuse ID as connection handle
    U16_TO_STREAM_LE(&p_buf, p_log->payloadLength);
    VARIABLE_COPY_TO_STREAM(&p_buf, p_log->p_logPayload, p_log->payloadLength);
    U8_TO_STREAM(&p_buf, LOG_END_CHECK_BYTE_1);
    U8_TO_STREAM(&p_buf, LOG_END_CHECK_BYTE_2);

    if (p_log->logType == BT_LOG_TYPE_HCI_ACL_TX)
        ble_log_Print(BM70_OPCODE_HCI_ACL_TX, packetLength, p_logPacket);
    else
        ble_log_Print(BM70_OPCODE_HCI_ACL_RX, packetLength, p_logPacket);

    OSAL_Free(p_logPacket);
}

void BLE_LOG_StackLogHandler(BLE_Log_T *p_log)
{
    if (p_log->logType == BT_LOG_TYPE_HCI_COMMAND)
    {
        ble_log_ProcHciCommand(p_log);
    }
    else if (p_log->logType == BT_LOG_TYPE_HCI_EVENT)
    {
        ble_log_ProcHciEvent(p_log);
    }
    else if (p_log->logType == BT_LOG_TYPE_HCI_ACL_TX)
    {
        ble_log_ProcHciAcl(p_log);
    }
    else if (p_log->logType == BT_LOG_TYPE_HCI_ACL_RX)
    {
        ble_log_ProcHciAcl(p_log);
    }

    if (p_log->p_logPayload)
    {
        OSAL_Free(p_log->p_logPayload);
    }

    if (p_log->p_returnParams)
    {
        OSAL_Free(p_log->p_returnParams);
    }

    OSAL_Free(p_log);
}

void BLE_LOG_Init(BLE_LOG_PrintCb printCb)
{
    g_logPrintCb = NULL;

    if (printCb)
    {
        g_logPrintCb = printCb;
    }
}
