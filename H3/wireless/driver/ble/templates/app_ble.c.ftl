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

<#if APP_TRCBP_SERVER == true>
#include "app_trcbps_handler.h"
</#if>

<#if APP_TRP_SERVER == true>
#include "app_trsps_handler.h"
</#if>

<#if APP_TRP_CLIENT == true>
#include "app_trspc_handler.h"
</#if>


// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Global Variables
// *****************************************************************************
// *****************************************************************************
<#if BLE_BOOL_GATT_CLIENT == true>
BLE_DD_Config_T         ddConfig;
</#if>

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

    <#if BLE_BOOL_GATT_CLIENT == true>
    BLE_DD_BleEventHandler(&ddConfig, p_stackEvt);
        <#if BOOL_GCM_SCM == true>
    BLE_SCM_BleEventHandler(p_stackEvt);
        </#if>
    </#if>

    //Direct event to BLE profiles
    <#if APP_TRP_SERVER == true>
    BLE_TRSPS_BleEventHandler(p_stackEvt);
    </#if>

    <#if APP_TRP_CLIENT == true>
    BLE_TRSPC_BleEventHandler(p_stackEvt);
    </#if>
    
    <#if APP_TRCBP_SERVER == true>
    BLE_TRCBPS_BleEventHandler(p_stackEvt);
    </#if>

    OSAL_Free(p_stackEvt->p_event);
}

<#if BLE_BOOL_GATT_CLIENT == true>
void APP_DdEvtHandler(BLE_DD_Event_T *p_event)
{
    <#if BOOL_GCM_SCM == true>
    BLE_SCM_BleDdEventHandler(p_event);
    </#if>

    <#if APP_TRP_CLIENT == true>
    BLE_TRSPC_BleDdEventHandler(p_event);
    </#if>
}
    <#if BOOL_GCM_SCM == true>
void APP_ScmEvtHandler(BLE_SCM_Event_T *p_event)
{
    if (p_event->eventId == BLE_SCM_EVT_SVC_CHANGE)
    {
        BLE_DD_RestartServicesDiscovery(p_event->eventField.evtServiceChange.connHandle);
    }

    /* TODO: implement your application state machine.*/
}
    </#if>
</#if>

void APP_BleConfig()
{
    uint8_t devName[]="${GAP_DEV_NAME}";
    BLE_SMP_Config_T                smpParam;
    int8_t                          connTxPower;

    <#if BOOL_GAP_PRIVACY == true>
    BLE_GAP_LocalPrivacyParams_T    localPriParam;
    </#if>
    
    <#if GAP_ADVERTISING == true>
        <#if BOOL_GAP_EXT_ADV == false>
    int8_t                          advTxPower;
    BLE_GAP_AdvParams_T             advParam;
            <#if GAP_ADV_DATA_LEN != 0>
    uint8_t advData[]={${GAP_ADV_DATA}};
    BLE_GAP_AdvDataParams_T         appAdvData;
            </#if>
            <#if GAP_SCAN_RSP_DATA_LEN != 0>
    uint8_t scanRspData[]={${GAP_SCAN_RSP_DATA}};
    BLE_GAP_AdvDataParams_T         appScanRspData;
            </#if>    
        <#else>
    int8_t selectedTxPower;
    BLE_GAP_ExtAdvParams_T advParams;
            <#if GAP_EXT_ADV_DATA_LEN != 0>
    uint8_t advData[]={${GAP_EXT_ADV_DATA}};
    BLE_GAP_ExtAdvDataParams_T appAdvData;
            </#if>
            <#if GAP_EXT_SCAN_RSP_DATA_LEN != 0>
    uint8_t scanRspData[]={${GAP_EXT_SCAN_RSP_DATA}};
    BLE_GAP_ExtAdvDataParams_T appScanRspData;
            </#if>
    
            <#if GAP_EXT_ADV_ADV_SET_2 == true>
    int8_t selectedTxPower2;
                <#if GAP_EXT_ADV_DATA_LEN_2 != 0>
    uint8_t advData2[]={${GAP_EXT_ADV_DATA_2}};
                </#if>
                <#if GAP_EXT_SCAN_RSP_DATA_LEN_2 != 0>
    uint8_t scanRspData2[]={${GAP_EXT_SCAN_RSP_DATA_2}};
                </#if>
    
            </#if>
        </#if>    
    </#if>
    
    
    <#if GAP_SCAN == true>
        <#if BOOL_GAP_EXT_SCAN == false>
    BLE_GAP_ScanningParams_T        scanParam;
        <#else>
    BLE_GAP_ExtScanningPhy_T        scanParams;
        </#if>
    </#if>
    BLE_DM_Config_T                 dmConfig;
    BLE_GAP_ServiceOption_T         gapServiceOptions;
    <#if BLE_BOOL_GATT_SVC_CHANGE == true>
    GATTS_GattServiceOptions_T      gattServiceOptions;
    </#if>

    // Configure device name
    BLE_GAP_SetDeviceName(sizeof(devName), devName);
    
    
    // GAP Service option
    gapServiceOptions.charDeviceName.enableWriteProperty = ${GAP_SVC_DEV_NAME_WRITE?c};
    gapServiceOptions.charAppearance.appearance = 0x${GAP_SVC_APPEARANCE};
    gapServiceOptions.charPeriPreferConnParam.enable = ${GAP_SVC_PERI_PRE_CP?c};
    <#if GAP_SVC_PERI_PRE_CP == true>
    gapServiceOptions.charPeriPreferConnParam.minConnInterval = ${GAP_SVC_CP_MIN_CONN_INTERVAL};
    gapServiceOptions.charPeriPreferConnParam.maxConnInterval = ${GAP_SVC_CP_MAX_CONN_INTERVAL};
    gapServiceOptions.charPeriPreferConnParam.peripheralLatency = ${GAP_SVC_CP_PERI_LATENCY};
    gapServiceOptions.charPeriPreferConnParam.connSupervisionTimeoutMulti = ${GAP_SVC_CP_PERI_SUPERVISION_TIMEOUT};
    </#if>
    
    BLE_GAP_ConfigureBuildInService(&gapServiceOptions);
    
    <#if BOOL_GAP_PRIVACY == true> 
    // Configure privacy
    localPriParam.enable=true;
    localPriParam.privacyAddrType = ${GAP_PRIV_ADDR_TYPE};
    localPriParam.addrTimeout = ${GAP_PRIV_ADDR_TO};
    memset(localPriParam.localIrk, 0, 16);
    BLE_GAP_SetLocalPrivacy(&localPriParam);
    </#if>

    <#if GAP_ADVERTISING == true>
    // Configure advertising parameters
        <#if BOOL_GAP_EXT_ADV == false>
    BLE_GAP_SetAdvTxPowerLevel(${GAP_ADV_TX_PWR},&advTxPower);    
    
    memset(&advParam, 0, sizeof(BLE_GAP_AdvParams_T));
    advParam.intervalMin = ${GAP_ADV_INTERVAL_MIN};
    advParam.intervalMax = ${GAP_ADV_INTERVAL_MAX};
    advParam.type = ${GAP_ADV_TYPE};
    advParam.advChannelMap = ${GAP_CHANNEL_MAP};
    advParam.filterPolicy = ${GAP_ADV_FILT_POLICY};
    BLE_GAP_SetAdvParams(&advParam);
    
            <#if GAP_ADV_DATA_LEN != 0>
    // Configure advertising data
    appAdvData.advLen=sizeof(advData);
    memcpy(appAdvData.advData, advData, appAdvData.advLen);
    BLE_GAP_SetAdvData(&appAdvData);
            </#if>

            <#if GAP_SCAN_RSP_DATA_LEN != 0>
    // Configure advertising scan response data
    appScanRspData.advLen=sizeof(scanRspData);
    memcpy(appScanRspData.advData, scanRspData, appScanRspData.advLen);
    BLE_GAP_SetScanRspData(&appScanRspData);
            </#if>

        <#else>
    advParams.advHandle = ${GAP_EXT_ADV_ADV_SET_HANDLE};
    advParams.evtProperies = ${GAP_EXT_ADV_EVT_PROPERTIES};
            <#if GAP_EXT_ADV_EVT_PROP_ANON == true>
    advParams.evtProperies |= BLE_GAP_EXT_ADV_EVT_PROP_OMIT_ADDRESS;
            </#if>
            <#if GAP_EXT_ADV_EVT_PROP_TX_POWER == true>
    advParams.evtProperies |= BLE_GAP_EXT_ADV_EVT_PROP_TX_POWER;
            </#if>
    advParams.priIntervalMin = ${GAP_EXT_ADV_PRI_INTERVAL_MIN};
    advParams.priIntervalMax = ${GAP_EXT_ADV_PRI_INTERVAL_MAX};
    advParams.priChannelMap = ${GAP_PRI_CHANNEL_MAP};
    memset(&advParams.peerAddr, 0x00, sizeof(advParams.peerAddr));
    advParams.filterPolicy = ${GAP_EXT_ADV_FILT_POLICY};
    advParams.txPower = ${GAP_EXT_ADV_TX_POWER};
    advParams.priPhy = ${GAP_PRI_ADV_PHY};
    advParams.secMaxSkip = ${GAP_EXT_ADV_MAX_SKIP};
    advParams.secPhy = ${GAP_SEC_ADV_PHY};
    advParams.sid = ${GAP_EXT_ADV_SID};
    advParams.scanReqNotifiEnable = ${GAP_EXT_ADV_SCAN_ENABLE?c};
    BLE_GAP_SetExtAdvParams(&advParams, &selectedTxPower);
    
            <#if GAP_EXT_ADV_DATA_LEN != 0>
    appAdvData.advHandle = ${GAP_EXT_ADV_ADV_SET_HANDLE};
    appAdvData.operation = BLE_GAP_EXT_ADV_DATA_OP_COMPLETE;
    appAdvData.fragPreference = BLE_GAP_EXT_ADV_DATA_FRAG_PREF_ALL;
    appAdvData.advLen = sizeof(advData);
    appAdvData.p_advData=OSAL_Malloc(appAdvData.advLen);
    if(appAdvData.p_advData)
    {
        memcpy(appAdvData.p_advData, advData, appAdvData.advLen);
    }
    BLE_GAP_SetExtAdvData(&appAdvData);
    OSAL_Free(appAdvData.p_advData);
            </#if>
    
            <#if GAP_EXT_SCAN_RSP_DATA_LEN != 0>
    appScanRspData.advHandle = ${GAP_EXT_ADV_ADV_SET_HANDLE};
    appScanRspData.operation = BLE_GAP_EXT_ADV_DATA_OP_COMPLETE;
    appScanRspData.fragPreference = BLE_GAP_EXT_ADV_DATA_FRAG_PREF_ALL;
    appScanRspData.advLen = sizeof(scanRspData);
    appScanRspData.p_advData=OSAL_Malloc(appScanRspData.advLen);
    if(appScanRspData.p_advData)
    {
        memcpy(appScanRspData.p_advData, scanRspData, appScanRspData.advLen);
    }
    BLE_GAP_SetExtScanRspData(&appScanRspData);
    OSAL_Free(appScanRspData.p_advData);
            </#if>

            <#if GAP_EXT_ADV_ADV_SET_2 == true>
    advParams.advHandle = ${GAP_EXT_ADV_ADV_SET_HANDLE_2};
    advParams.evtProperies = ${GAP_EXT_ADV_EVT_PROPERTIES_2};
                <#if GAP_EXT_ADV_EVT_PROP_ANON_2 == true>
    advParams.evtProperies |= BLE_GAP_EXT_ADV_EVT_PROP_OMIT_ADDRESS;
                </#if>
                <#if GAP_EXT_ADV_EVT_PROP_TX_POWER_2 == true>
    advParams.evtProperies |= BLE_GAP_EXT_ADV_EVT_PROP_TX_POWER;
                </#if>
    advParams.priIntervalMin = ${GAP_EXT_ADV_PRI_INTERVAL_MIN_2};
    advParams.priIntervalMax = ${GAP_EXT_ADV_PRI_INTERVAL_MAX_2};
    advParams.priChannelMap = ${GAP_PRI_CHANNEL_MAP_2};
    memset(&advParams.peerAddr, 0x00, sizeof(advParams.peerAddr));
    advParams.filterPolicy = ${GAP_EXT_ADV_FILT_POLICY_2};
    advParams.txPower = ${GAP_EXT_ADV_TX_POWER_2};
    advParams.priPhy = ${GAP_PRI_ADV_PHY_2};
    advParams.secMaxSkip = ${GAP_EXT_ADV_MAX_SKIP_2};
    advParams.secPhy = ${GAP_SEC_ADV_PHY_2};
    advParams.sid = ${GAP_EXT_ADV_SID_2};
    advParams.scanReqNotifiEnable = ${GAP_EXT_ADV_SCAN_ENABLE_2?c};
    BLE_GAP_SetExtAdvParams(&advParams, &selectedTxPower2);
    
                <#if GAP_EXT_ADV_DATA_LEN_2 != 0>
    appAdvData.advHandle = ${GAP_EXT_ADV_ADV_SET_HANDLE_2};
    appAdvData.operation = BLE_GAP_EXT_ADV_DATA_OP_COMPLETE;
    appAdvData.fragPreference = BLE_GAP_EXT_ADV_DATA_FRAG_PREF_ALL;
    appAdvData.advLen = sizeof(advData2);
    appAdvData.p_advData=OSAL_Malloc(appAdvData.advLen);
    if(appAdvData.p_advData)
    {
        memcpy(appAdvData.p_advData, advData2, appAdvData.advLen);
    }
    BLE_GAP_SetExtAdvData(&appAdvData);
    OSAL_Free(appAdvData.p_advData);
                </#if>
    
                <#if GAP_EXT_SCAN_RSP_DATA_LEN_2 != 0>
    appScanRspData.advHandle = ${GAP_EXT_ADV_ADV_SET_HANDLE_2};
    appScanRspData.operation = BLE_GAP_EXT_ADV_DATA_OP_COMPLETE;
    appScanRspData.fragPreference = BLE_GAP_EXT_ADV_DATA_FRAG_PREF_ALL;
    appScanRspData.advLen = sizeof(scanRspData2);
    appScanRspData.p_advData=OSAL_Malloc(appScanRspData.advLen);
    if(appScanRspData.p_advData)
    {
        memcpy(appScanRspData.p_advData, scanRspData2, appScanRspData.advLen);
    }
    BLE_GAP_SetExtScanRspData(&appScanRspData);
    OSAL_Free(appScanRspData.p_advData);
                </#if>
            </#if>
        </#if>
    </#if>
        
    
    <#if GAP_SCAN == true>
    // Configure scan parameters
        <#if BOOL_GAP_EXT_SCAN == false>
    scanParam.type = ${GAP_SCAN_TYPE};
    scanParam.interval = ${GAP_SCAN_INTERVAL};
    scanParam.window = ${GAP_SCAN_WINDOW};
    scanParam.filterPolicy = ${GAP_SCAN_FILT_POLICY};
    BLE_GAP_SetScanningParam(&scanParam);
        <#else>
            <#if (GAP_EXT_SCAN_PHY == 'BLE_GAP_SCAN_PHY_1M') || (GAP_EXT_SCAN_PHY == 'BLE_GAP_SCAN_PHY_1M|BLE_GAP_SCAN_PHY_CODED')>
    scanParams.le1mPhy.enable = true;
    scanParams.le1mPhy.type = ${GAP_EXT_SCAN_TYPE_1M};
    scanParams.le1mPhy.interval = ${GAP_EXT_SCAN_INTERVAL_1M};
    scanParams.le1mPhy.window = ${GAP_EXT_SCAN_WINDOW_1M};
            <#else>
    scanParams.le1mPhy.enable = false;
            </#if>
            <#if (GAP_EXT_SCAN_PHY == 'BLE_GAP_SCAN_PHY_CODED') || (GAP_EXT_SCAN_PHY == 'BLE_GAP_SCAN_PHY_1M|BLE_GAP_SCAN_PHY_CODED')>
    scanParams.leCodedPhy.enable = true;
    scanParams.leCodedPhy.type = ${GAP_EXT_SCAN_TYPE_CODED};
    scanParams.leCodedPhy.interval = ${GAP_EXT_SCAN_INTERVAL_CODED};
    scanParams.leCodedPhy.window = ${GAP_EXT_SCAN_WINDOW_CODED};
            <#else>
    scanParams.leCodedPhy.enable = false;
            </#if>
    BLE_GAP_SetExtScanningParams(${GAP_EXT_SCAN_FILT_POLICY}, &scanParams);
        </#if>
    </#if>
    
    BLE_GAP_SetConnTxPowerLevel(${GAP_CONN_TX_PWR}, &connTxPower);

    // Configure SMP parameters
    memset(&smpParam, 0, sizeof(BLE_SMP_Config_T));
    smpParam.ioCapability = ${SMP_IOCAP_TYPE};
    <#if SMP_AUTH_REQ_BOND == true>
    smpParam.authReqFlag |= BLE_SMP_OPTION_BONDING;
    </#if>
    <#if SMP_AUTH_REQ_MITM == true>
    smpParam.authReqFlag |= BLE_SMP_OPTION_MITM;
    </#if>
    <#if SMP_AUTH_REQ_SC == true>
    smpParam.authReqFlag |= BLE_SMP_OPTION_SECURE_CONNECTION;
    </#if>
    <#if SMP_OOB == true>
    oobDataFlag = BLE_SMP_OOB_PRESENT;
    </#if>
    <#if SMP_SC_ONLY == true>
    smpParam.secureConnectionsOnly = BLE_SMP_SC_ONLY_ENABLE;
    </#if>
    BLE_SMP_Config(&smpParam);

    // Configure BLE_DM middleware parameters
    <#if BOOL_DM_SEC_AUTO_ACCEPT == true>
    dmConfig.secAutoAccept = true;
    <#else>
    dmConfig.secAutoAccept = false;
    </#if>
    <#if BOOL_DM_CONN_AUTO_ACCEPT == false>
    dmConfig.connConfig.autoReplyUpdateRequest = false;
    <#else>
    dmConfig.connConfig.autoReplyUpdateRequest = true;
    dmConfig.connConfig.minAcceptConnInterval = ${DM_MIN_CONN_INTERVAL};
    dmConfig.connConfig.maxAcceptConnInterval = ${DM_MAX_CONN_INTERVAL};
    dmConfig.connConfig.minAcceptPeripheralLatency = ${DM_MIN_CONN_LATENCY};
    dmConfig.connConfig.maxAcceptPeripheralLatency = ${DM_MAX_CONN_LATENCY};
    </#if>
    BLE_DM_Config(&dmConfig);

    <#if BLE_BOOL_GATT_SVC_CHANGE == true>
    // GATT Service option
    gattServiceOptions.enable = true;
    GATTS_ConfigureBuildInService(&gattServiceOptions);
    </#if>

    <#if BLE_BOOL_GATT_CLIENT == true>
    // Configure BLE_DD middleware parameters
    ddConfig.waitForSecurity = false;
    ddConfig.initDiscInCentral = true;
    ddConfig.initDiscInPeripheral = false;
    </#if>
}

void APP_BleStackInit()
{
    <#if BLE_BOOL_GATT_CACHING == true>
    uint16_t gattsInitParam=GATTS_CONFIG_CACHING_SUPPORT;
    <#else>
    uint16_t gattsInitParam=GATTS_CONFIG_NONE;
    </#if>
    
    <#if BLE_BOOL_GATT_CLIENT == true>
    <#if BLE_BOOL_GATT_MANU_CONFIRM == true>
    uint16_t gattcInitParam=GATTC_CONFIG_MANUAL_CONFIRMATION;
    <#else>
    uint16_t gattcInitParam=GATTC_CONFIG_NONE;
    </#if>
    </#if>

    STACK_EventRegister(APP_BleStackCb);
    
    //Initialize BLE Stack    
    BLE_GAP_Init();

    <#if GAP_ADVERTISING == true>
    BLE_GAP_AdvInit();
    <#if BOOL_GAP_EXT_ADV == true>
    BLE_GAP_ExtAdvInit();
    </#if>
    </#if>

    <#if GAP_SCAN == true>
    BLE_GAP_ScanInit();
    <#if BOOL_GAP_EXT_SCAN == true>
    BLE_GAP_ExtScanInit(${GAP_EXT_SCAN_DATA_LEN}, ${GAP_EXT_SCAN_NUM_2_PKT});
    </#if>
    </#if>

    <#if GAP_PERIPHERAL == true>
    BLE_GAP_ConnPeripheralInit();
    </#if>
    
    <#if GAP_CENTRAL == true>
    BLE_GAP_ConnCentralInit();
    </#if>

    BLE_L2CAP_Init();
    <#if BOOL_L2CAP_CREDIT_FLOWCTRL == true>
    BLE_L2CAP_CbInit();
    </#if>
    
    GATTS_Init(gattsInitParam);
    
    <#if BLE_BOOL_GATT_CLIENT == true>
    GATTC_Init(gattcInitParam);
    </#if>

    BLE_SMP_Init();
    
    <#if GAP_DIRECT_TEST_MODE == true>
    BLE_DTM_Init();
    </#if>

    //Initialize BLE middleware
    BLE_DM_Init();
    BLE_DM_EventRegister(APP_DmEvtHandler);

    <#if BLE_BOOL_GATT_CLIENT == true>
    BLE_DD_Init();
    BLE_DD_EventRegister(APP_DdEvtHandler);
    
        <#if BOOL_GCM_SCM == true>
    BLE_SCM_Init();
    BLE_SCM_EventRegister(APP_ScmEvtHandler);
        </#if>
    </#if>

    //Initialize BLE profiles
    <#if APP_TRP_SERVER == true>
    BLE_TRSPS_Init();
    BLE_TRSPS_EventRegistration(APP_TrspsEvtHandler);
    </#if>

    <#if APP_TRP_CLIENT == true>
    BLE_TRSPC_Init();
    BLE_TRSPC_EventRegistration(APP_TrspcEvtHandler);
    </#if>

    <#if APP_TRCBP_SERVER == true>
    BLE_TRCBPS_Init();
    BLE_TRCBPS_EventRegistration(APP_TrcbpsEvtHandler);
    </#if>

    APP_BleConfig();
}





