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


    //Direct event to BLE profiles

    

    OSAL_Free(p_stackEvt->p_event);
}


void APP_BleConfig()
{
    uint8_t devName[]="Microchip";
    BLE_SMP_Config_T                smpParam;
    int8_t                          connTxPower;

    
    int8_t selectedTxPower;
    BLE_GAP_ExtAdvParams_T advParams;
    uint8_t advData[]={0x02, 0x01, 0x04, 0x0A, 0x09, 0x4D, 0x69, 0x63, 0x72, 0x6F, 0x63, 0x68, 0x69, 0x70, 0x04, 0x16, 0xDA, 0xFE, 0x0F};
    BLE_GAP_ExtAdvDataParams_T appAdvData;
    uint8_t scanRspData[]={0x0B, 0x09, 0x70, 0x69, 0x63, 0x33, 0x32, 0x63, 0x78, 0x2D, 0x62, 0x7A};
    BLE_GAP_ExtAdvDataParams_T appScanRspData;
    
    
    
    BLE_DM_Config_T                 dmConfig;
    BLE_GAP_ServiceOption_T         gapServiceOptions;

    // Configure device name
    BLE_GAP_SetDeviceName(sizeof(devName), devName);
    
    
    // GAP Service option
    gapServiceOptions.charDeviceName.enableWriteProperty = false;
    gapServiceOptions.charAppearance.appearance = 0x0;
    gapServiceOptions.charPeriPreferConnParam.enable = false;
    
    BLE_GAP_ConfigureBuildInService(&gapServiceOptions);
    

    // Configure advertising parameters
    advParams.advHandle = 1;
    advParams.evtProperies = 0;
    advParams.priIntervalMin = 1600;
    advParams.priIntervalMax = 1600;
    advParams.priChannelMap = BLE_GAP_ADV_CHANNEL_ALL;
    memset(&advParams.peerAddr, 0x00, sizeof(advParams.peerAddr));
    advParams.filterPolicy = BLE_GAP_ADV_FILTER_DEFAULT;
    advParams.txPower = 12;
    advParams.priPhy = BLE_GAP_PHY_TYPE_LE_CODED;
    advParams.secMaxSkip = 10;
    advParams.secPhy = BLE_GAP_PHY_TYPE_LE_CODED;
    advParams.sid = 1;
    advParams.scanReqNotifiEnable = false;
    BLE_GAP_SetExtAdvParams(&advParams, &selectedTxPower);
    
    appAdvData.advHandle = 1;
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
    
    appScanRspData.advHandle = 1;
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


}

void APP_BleStackInit()
{
    uint16_t gattsInitParam=GATTS_CONFIG_NONE;
    

    STACK_EventRegister(APP_BleStackCb);
    
    //Initialize BLE Stack    
    BLE_GAP_Init();

    BLE_GAP_AdvInit();
    BLE_GAP_ExtAdvInit();


    BLE_GAP_ConnPeripheralInit();
    

    BLE_L2CAP_Init();
    
    GATTS_Init(gattsInitParam);
    

    BLE_SMP_Init();
    

    //Initialize BLE middleware
    BLE_DM_Init();
    BLE_DM_EventRegister(APP_DmEvtHandler);


    //Initialize BLE profiles



    APP_BleConfig();
}





