/*******************************************************************************
  Application BLE Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_ble.c

  Summary:
    This file contains the Application BLE implementation for this project.

  Description:
    This file contains the Application BLE implementation for this project.
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

#include "app.h"
#include "osal/osal_freertos_extend.h"
#include "app_ble.h"
#include "app_ble_handler.h"

#include "app_trcbps_handler.h"

#include "app_trsps_handler.h"

#include "app_trspc_handler.h"


// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#ifdef APP_EXT_SCAN_ENABLE
#define MAX_LEN_OF_EXT_ADV_PAYLOAD      1650
#define MAX_NUM_OF_EXT_ADV_PKT          16
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Global Variables
// *****************************************************************************
// *****************************************************************************
BLE_DD_Config_T         ddConfig;

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************

void APP_BleStackCb(STACK_Event_T *p_stack)
{
    STACK_Event_T stackEvent;
    APP_Msg_T   appMsg;
    APP_Msg_T   *p_appMsg;

    memcpy((uint8_t *)&stackEvent, (uint8_t *)p_stack, sizeof(STACK_Event_T));
    stackEvent.p_event=OSAL_Malloc(p_stack->evtLen);
    if(stackEvent.p_event==NULL)
    {
        return;
    }
    memcpy(stackEvent.p_event, p_stack->p_event, p_stack->evtLen);
    stackEvent.p_event=stackEvent.p_event;

    if (p_stack->groupId==STACK_GRP_GATT)
    {
        GATT_Event_T *p_evtGatt = (GATT_Event_T *)stackEvent.p_event;

        if (p_evtGatt->eventId == GATTS_EVT_CLIENT_CCCDLIST_CHANGE)
        {
            uint8_t *p_payload;

            p_payload = (uint8_t *)OSAL_Malloc((p_evtGatt->eventField.onClientCccdListChange.numOfCccd*4));
            if (p_payload != NULL)
            {
                memcpy(p_payload, (uint8_t *)p_evtGatt->eventField.onClientCccdListChange.p_cccdList, (p_evtGatt->eventField.onClientCccdListChange.numOfCccd*4));
                p_evtGatt->eventField.onClientCccdListChange.p_cccdList = (GATTS_CccdList_T *)p_payload;
            }
        }
    }

    appMsg.msgId=APP_MSG_BLE_STACK_EVT;

    ((STACK_Event_T *)appMsg.msgData)->groupId=p_stack->groupId;
    ((STACK_Event_T *)appMsg.msgData)->evtLen=p_stack->evtLen;
    ((STACK_Event_T *)appMsg.msgData)->p_event=stackEvent.p_event;

    p_appMsg = &appMsg;
    OSAL_QUEUE_Send(&appData.appQueue, p_appMsg, 0);
}

void APP_BleStackEvtHandler(STACK_Event_T *p_stackEvt)
{
    switch(p_stackEvt->groupId)
    {
        case STACK_GRP_BLE_GAP:
        {
            APP_BleGapEvtHandler((BLE_GAP_Event_T *)p_stackEvt->p_event);
        }
        break;
        
        case STACK_GRP_BLE_L2CAP:
        {
            APP_BleL2capEvtHandler((BLE_L2CAP_Event_T *)p_stackEvt->p_event);
         }
        break;
        case STACK_GRP_BLE_SMP:
        {
            APP_BleSmpEvtHandler((BLE_SMP_Event_T *)p_stackEvt->p_event);
         }
        break;

        case STACK_GRP_GATT:
        {
            APP_GattEvtHandler((GATT_Event_T *)p_stackEvt->p_event);
        }
        break;

    } 

    //Direct event to BLE middleware
    BLE_DM_BleEventHandler(p_stackEvt);

    BLE_DD_BleEventHandler(&ddConfig, p_stackEvt);

    //Direct event to BLE profiles
    BLE_TRSPS_BleEventHandler(p_stackEvt);

    BLE_TRSPC_BleEventHandler(p_stackEvt);
    
    BLE_TRCBPS_BleEventHandler(p_stackEvt);

    OSAL_Free(p_stackEvt->p_event);
}

void APP_DdEvtHandler(BLE_DD_Event_T *p_event)
{
#if (defined APP_MIDDLEWARE_ENABLE) && (defined APP_TRPC_ENABLE)
    BLE_TRSPC_BleDdEventHandler(p_event);
#endif
}

void APP_BleConfig()
{
    uint8_t devName[]="Microchip";
    BLE_SMP_Config_T                smpParam;
    int8_t                          connTxPower;

#ifdef APP_PRIVACY_ENABLE
    BLE_GAP_LocalPrivacyParams_T    localPriParam;
#endif

    int8_t                          advTxPower;
#ifndef APP_EXT_ADV_ENABLE
    BLE_GAP_AdvParams_T             advParam;
#endif

#ifdef APP_EXT_SCAN_ENABLE
    BLE_GAP_ExtScanningPhy_T        extScanPhy;
#else
    BLE_GAP_ScanningParams_T        scanParam;
#endif

    BLE_DM_Config_T                 dmConfig;
    BLE_GAP_ServiceOption_T         gapServiceOptions;

    // Configure device name
    BLE_GAP_SetDeviceName(sizeof(devName), devName);

    
    // GAP Service option
    gapServiceOptions.charDeviceName.enableWriteProperty = true;
    gapServiceOptions.charAppearance.appearance = 0x0;
    gapServiceOptions.charPeriPreferConnParam.enable = false;
    
    BLE_GAP_ConfigureBuildInService(&gapServiceOptions);
    
#ifdef APP_PRIVACY_ENABLE
    // Configure privacy
    localPriParam.enable=true;
    localPriParam.privacyAddrType = BLE_GAP_ADDR_TYPE_RANDOM_RESOLVABLE;
    localPriParam.addrTimeout = 900;
    memset(localPriParam.localIrk, 0, 16);
    BLE_GAP_SetLocalPrivacy(&localPriParam);
#endif

    // Configure advertising parameters
    BLE_GAP_SetAdvTxPowerLevel(11,&advTxPower);

#ifndef APP_EXT_ADV_ENABLE
    // Configure advertising parameters
    memset(&advParam, 0, sizeof(BLE_GAP_AdvParams_T));
    advParam.intervalMin = 512;
    advParam.intervalMax = 512;
    advParam.type = BLE_GAP_ADV_TYPE_ADV_IND;
    advParam.advChannelMap = BLE_GAP_ADV_CHANNEL_ALL;
    advParam.filterPolicy = BLE_GAP_ADV_FILTER_DEFAULT;
    BLE_GAP_SetAdvParams(&advParam);
#else
    BLE_GAP_ExtAdvParams_T             extAdvParamSet1;
    BLE_GAP_ExtAdvParams_T             extAdvParamSet2;
    int8_t selectedTxPower1, selectedTxPower2;

    memset(&extAdvParamSet1, 0, sizeof(BLE_GAP_ExtAdvParams_T));
    memset(&extAdvParamSet2, 0, sizeof(BLE_GAP_ExtAdvParams_T));

    //Set 1: Ext. Adv
    extAdvParamSet1.advHandle = 0x00;
    extAdvParamSet1.evtProperies = BLE_GAP_EXT_ADV_EVT_PROP_CONNECTABLE_ADV;   //If extended advertising PDU types are being used then the advertisement shall not be both connectable and scannable.
    extAdvParamSet1.priIntervalMax = 512;
    extAdvParamSet1.priIntervalMin = 512;
    extAdvParamSet1.priChannelMap = BLE_GAP_ADV_CHANNEL_ALL;
    extAdvParamSet1.filterPolicy = BLE_GAP_ADV_FILTER_DEFAULT;
    extAdvParamSet1.txPower = BLE_GAP_EXT_ADV_TX_POWER_LEVEL_NO_PERFERENCE;
    extAdvParamSet1.priPhy = BLE_GAP_PHY_TYPE_LE_1M;                          //Primary channel only supports 1M or Coded PHY
    extAdvParamSet1.secMaxSkip = 0x05;
    extAdvParamSet1.secPhy = BLE_GAP_PHY_TYPE_LE_2M;
    extAdvParamSet1.sid = 0x00;
    extAdvParamSet1.scanReqNotifiEnable = 0x00;

    BLE_GAP_SetExtAdvParams(&extAdvParamSet1, &selectedTxPower1);

    //Set 2: Legacy Adv
    extAdvParamSet2.advHandle = 0x01;
    extAdvParamSet2.evtProperies = BLE_GAP_EXT_ADV_EVT_PROP_CONNECTABLE_ADV | BLE_GAP_EXT_ADV_EVT_PROP_SCANNABLE_ADV | BLE_GAP_EXT_ADV_EVT_PROP_LEGACY_ADV;
    extAdvParamSet2.priIntervalMax = 512;
    extAdvParamSet2.priIntervalMin = 512;
    extAdvParamSet2.priChannelMap = BLE_GAP_ADV_CHANNEL_ALL;
    extAdvParamSet2.filterPolicy = BLE_GAP_ADV_FILTER_DEFAULT;
    extAdvParamSet2.txPower = BLE_GAP_EXT_ADV_TX_POWER_LEVEL_NO_PERFERENCE;
    extAdvParamSet2.priPhy = BLE_GAP_PHY_TYPE_LE_1M;
    extAdvParamSet2.secMaxSkip = 0x05;
    extAdvParamSet2.secPhy = BLE_GAP_PHY_TYPE_LE_1M;
    extAdvParamSet2.sid = 0x01;
    extAdvParamSet2.scanReqNotifiEnable = 0x00;

    BLE_GAP_SetExtAdvParams(&extAdvParamSet2, &selectedTxPower2);
#endif

    #ifdef APP_EXT_SCAN_ENABLE
    #ifdef APP_BLE_CODED_PHY_ENABLE
    extScanPhy.le1mPhy.enable = false;
    extScanPhy.leCodedPhy.enable = true;
    extScanPhy.leCodedPhy.type = BLE_GAP_SCAN_TYPE_ACTIVE_SCAN;
    extScanPhy.leCodedPhy.interval = 160;
    extScanPhy.leCodedPhy.window = 32;
    #else
    extScanPhy.le1mPhy.enable = true;
    extScanPhy.leCodedPhy.enable = false;
    extScanPhy.le1mPhy.type = BLE_GAP_SCAN_TYPE_ACTIVE_SCAN;
    extScanPhy.le1mPhy.interval = 160;
    extScanPhy.le1mPhy.window = 32;
    #endif
    BLE_GAP_SetExtScanningParams(BLE_GAP_SCAN_FP_ACCEPT_ALL, &extScanPhy);
    #else
    // Configure scan parameters
    scanParam.type = BLE_GAP_SCAN_TYPE_ACTIVE_SCAN;
    scanParam.interval = 160;
    scanParam.window = 32;
    scanParam.filterPolicy = BLE_GAP_SCAN_FP_ACCEPT_ALL;
    BLE_GAP_SetScanningParam(&scanParam);
    #endif
    
    BLE_GAP_SetConnTxPowerLevel(15, &connTxPower);

    // Configure SMP parameters
    memset(&smpParam, 0, sizeof(BLE_SMP_Config_T));
    smpParam.ioCapability = BLE_SMP_IO_NOINPUTNOOUTPUT;
    smpParam.authReqFlag |= BLE_SMP_OPTION_BONDING;
    smpParam.authReqFlag |= BLE_SMP_OPTION_SECURE_CONNECTION;
    BLE_SMP_Config(&smpParam);
    
    // Configure BLE_DM middleware parameters
    dmConfig.secAutoAccept = true;
    dmConfig.connConfig.autoReplyUpdateRequest = true;
    dmConfig.connConfig.minAcceptConnInterval = 6;
    dmConfig.connConfig.maxAcceptConnInterval = 3200;
    dmConfig.connConfig.minAcceptPeripheralLatency = 0;
    dmConfig.connConfig.maxAcceptPeripheralLatency = 499;
    BLE_DM_Config(&dmConfig);


    // Configure BLE_DD middleware parameters
    ddConfig.waitForSecurity = false;
    ddConfig.initDiscInCentral = true;
    ddConfig.initDiscInPeripheral = false;
    
}

void APP_BleStackInit()
{
    uint16_t gattsInitParam=GATTS_CONFIG_NONE;
    
    uint16_t gattcInitParam=GATTC_CONFIG_NONE;

    STACK_EventRegister(APP_BleStackCb);
    
    //Initialize BLE Stack    
    BLE_GAP_Init();

    BLE_GAP_AdvInit();

#ifdef APP_EXT_ADV_ENABLE
    BLE_GAP_ExtAdvInit();
#endif

    BLE_GAP_ScanInit();

#ifdef APP_EXT_SCAN_ENABLE
    BLE_GAP_ExtScanInit(MAX_LEN_OF_EXT_ADV_PAYLOAD, MAX_NUM_OF_EXT_ADV_PKT);
#endif

    BLE_GAP_ConnPeripheralInit();
    
    BLE_GAP_ConnCentralInit();
    
#ifdef APP_EXT_SCAN_ENABLE
    BLE_GAP_ExtConnCentralInit();
#endif

    BLE_L2CAP_Init();
    BLE_L2CAP_CbInit();
    
    GATTS_Init(gattsInitParam);

    GATTC_Init(gattcInitParam);

    BLE_SMP_Init();


    //Initialize BLE middleware
    BLE_DM_Init();
    BLE_DM_EventRegister(APP_DmEvtHandler);

    BLE_DD_Init();
    BLE_DD_EventRegister(APP_DdEvtHandler);


    //Initialize BLE profiles
    BLE_TRSPS_Init();
    BLE_TRSPS_EventRegistration(APP_TrspsEvtHandler);

    #ifdef APP_PAIRING_ENABLE
    BLE_TRS_PermissionConfig(TRS_HDL_CCCD_CTRL, PERMISSION_WRITE_ENC);
    BLE_TRS_PermissionConfig(TRS_HDL_CCCD_TX, PERMISSION_WRITE_ENC);
    #endif

    BLE_TRSPC_Init();
    BLE_TRSPC_EventRegistration(APP_TrspcEvtHandler);

    BLE_TRCBPS_Init();
    BLE_TRCBPS_EventRegistration(APP_TrcbpsEvtHandler);
    
    APP_BleConfig();
}





