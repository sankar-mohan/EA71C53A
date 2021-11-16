/*******************************************************************************
  Application Advertising Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_adv.c

  Summary:
    This file contains the Application advertising functions for this project.

  Description:
    This file contains the Application advertising functions for this project.
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
#include "app_ble_handler.h"
#include "app_adv.h"
#include "app_error_defs.h"
#include "app_led.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************




// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************
static APP_BLE_AdvParams_T                  s_bleAdvParam;
static BLE_GAP_AdvDataParams_T              s_bleAdvData;
static BLE_GAP_AdvDataParams_T              s_bleScanRspData;

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
static void APP_ADV_InitAdvParams(void)
{
    s_bleAdvParam.intervalMin = APP_ADV_DEFAULT_INTERVAL;
    s_bleAdvParam.intervalMax = APP_ADV_DEFAULT_INTERVAL;
    s_bleAdvParam.advType = BLE_GAP_ADV_TYPE_ADV_IND;
    s_bleAdvParam.filterPolicy = BLE_GAP_ADV_FILTER_DEFAULT;
}

static uint8_t APP_ADV_CalculateDataLength(uint8_t *advData)
{
    /* Caculate Total Length of Adv Data /Scan Response Data Elements  */
    uint8_t len = 0, i = 0;

    while (1)
    {
        if (advData[i] != 0x00)   // Check the length is Zero or not
        {
            len++;               // Add Length field size
            len += advData[i];   // Add this Element Data Size

            if (len >= BLE_GAP_ADV_MAX_LENGTH)
            {
                len = BLE_GAP_ADV_MAX_LENGTH;
                break;
            }
            else
            {
                i = len;
            }
        }
        else
        {
            break;
        }
    }
    return len;
}

#ifdef APP_EXT_ADV_ENABLE
static uint16_t APP_ADV_StartExtAdv(void)
{
    BLE_GAP_ExtAdvEnableParams_T advEnableParam[APP_ADV_SET_NUM];
    uint8_t i;
    uint16_t ret;

    for (i = 0; i < APP_ADV_SET_NUM; i++)
    {
        memset(&advEnableParam[i].advHandle, 0, sizeof(BLE_GAP_ExtAdvEnableParams_T));
    }

    //Set 1: Ext. Adv
    advEnableParam[0].advHandle = APP_ADV_HANDLE1;
    advEnableParam[0].duration = 0x0000;
    advEnableParam[0].maxExtAdvEvts = 0x00;

    //Set 2: Legacy Adv
    advEnableParam[1].advHandle = APP_ADV_HANDLE2;
    advEnableParam[1].duration = 0x0000;
    advEnableParam[1].maxExtAdvEvts = 0x00;

    #ifdef APP_BLE_CODED_PHY_ENABLE
    ret = BLE_GAP_SetExtAdvEnable(true,  1, advEnableParam);
    #else
    ret = BLE_GAP_SetExtAdvEnable(true,  APP_ADV_SET_NUM, advEnableParam);
    #endif
    

    if (ret != MBA_RES_SUCCESS)
        return APP_RES_FAIL;
    else
        return APP_RES_SUCCESS;
}

static uint16_t APP_ADV_StopExtAdv(void)
{
    BLE_GAP_ExtAdvEnableParams_T advEnableParam[APP_ADV_SET_NUM];
    uint8_t i;
    uint16_t ret;
    
    for (i = 0; i < APP_ADV_SET_NUM; i++)
    {
        memset(&advEnableParam[i].advHandle, 0, sizeof(BLE_GAP_ExtAdvEnableParams_T));
    }

    ret = BLE_GAP_SetExtAdvEnable(false, 0, advEnableParam);

    if (ret != MBA_RES_SUCCESS)
        return APP_RES_FAIL;
    else
        return APP_RES_SUCCESS;
}

static uint16_t APP_ADV_SetExtAdvParams(void)
{
    BLE_GAP_ExtAdvParams_T             extAdvParamSet1;
    BLE_GAP_ExtAdvParams_T             extAdvParamSet2;
    int8_t selectedTxPower1, selectedTxPower2;
    uint16_t ret;

    memset(&extAdvParamSet1, 0, sizeof(BLE_GAP_ExtAdvParams_T));
    memset(&extAdvParamSet2, 0, sizeof(BLE_GAP_ExtAdvParams_T));

    //Set 1: Ext. Adv
    extAdvParamSet1.advHandle = APP_ADV_HANDLE1;
    extAdvParamSet1.evtProperies = BLE_GAP_EXT_ADV_EVT_PROP_CONNECTABLE_ADV;   //If extended advertising PDU types are being used then the advertisement shall not be both connectable and scannable.
    extAdvParamSet1.priIntervalMax = s_bleAdvParam.intervalMax;
    extAdvParamSet1.priIntervalMin = s_bleAdvParam.intervalMin;
    extAdvParamSet1.priChannelMap = BLE_GAP_ADV_CHANNEL_ALL;
    extAdvParamSet1.filterPolicy = s_bleAdvParam.filterPolicy;
    extAdvParamSet1.txPower = BLE_GAP_EXT_ADV_TX_POWER_LEVEL_NO_PERFERENCE;
    #ifdef APP_BLE_CODED_PHY_ENABLE
    extAdvParamSet1.priPhy = BLE_GAP_PHY_TYPE_LE_CODED;
    extAdvParamSet1.secMaxSkip = 0x00;
    extAdvParamSet1.secPhy = BLE_GAP_PHY_TYPE_LE_CODED;
    #else
    extAdvParamSet1.priPhy = BLE_GAP_PHY_TYPE_LE_1M;
    extAdvParamSet1.secMaxSkip = 0x00;
    extAdvParamSet1.secPhy = BLE_GAP_PHY_TYPE_LE_2M;
    #endif
    extAdvParamSet1.sid = APP_ADV_SID1;
    extAdvParamSet1.scanReqNotifiEnable = 0x00;

    ret = BLE_GAP_SetExtAdvParams(&extAdvParamSet1, &selectedTxPower1);

    //Set 2: Legacy Adv
    if (ret != MBA_RES_SUCCESS)
        return APP_RES_FAIL;

    extAdvParamSet2.advHandle = APP_ADV_HANDLE2;
    extAdvParamSet2.evtProperies = BLE_GAP_EXT_ADV_EVT_PROP_CONNECTABLE_ADV | BLE_GAP_EXT_ADV_EVT_PROP_SCANNABLE_ADV | BLE_GAP_EXT_ADV_EVT_PROP_LEGACY_ADV;
    extAdvParamSet2.priIntervalMax = s_bleAdvParam.intervalMax;
    extAdvParamSet2.priIntervalMin = s_bleAdvParam.intervalMin;
    extAdvParamSet2.priChannelMap = BLE_GAP_ADV_CHANNEL_ALL;
    extAdvParamSet2.filterPolicy = s_bleAdvParam.filterPolicy;
    extAdvParamSet2.txPower = BLE_GAP_EXT_ADV_TX_POWER_LEVEL_NO_PERFERENCE;
    extAdvParamSet2.priPhy = BLE_GAP_PHY_TYPE_LE_1M;
    extAdvParamSet2.secMaxSkip = 0x00;
    extAdvParamSet2.secPhy = BLE_GAP_PHY_TYPE_LE_1M;
    extAdvParamSet2.sid = APP_ADV_SID2;
    extAdvParamSet2.scanReqNotifiEnable = 0x00;
    
    ret = BLE_GAP_SetExtAdvParams(&extAdvParamSet2, &selectedTxPower2);

    if (ret != MBA_RES_SUCCESS)
        return APP_RES_FAIL;
    else
        return APP_RES_SUCCESS;
}

static uint16_t APP_ADV_SetExtAdvData(void)
{
    uint16_t ret;
    BLE_GAP_ExtAdvDataParams_T advDataParam;
    BLE_GAP_ExtAdvDataParams_T scanDataParam;

    memset((uint8_t *)&advDataParam, 0, sizeof(BLE_GAP_ExtAdvDataParams_T));
    memset((uint8_t *)&scanDataParam, 0, sizeof(BLE_GAP_ExtAdvDataParams_T));

    //Set 1: Ext. Adv
    advDataParam.advHandle = APP_ADV_HANDLE1;
    advDataParam.operation = BLE_GAP_EXT_ADV_DATA_OP_COMPLETE;              //For single packet
    advDataParam.fragPreference = BLE_GAP_EXT_ADV_DATA_FRAG_PREF_MIN;
    advDataParam.advLen = (s_bleAdvData.advLen + s_bleScanRspData.advLen);  //Include device name
    advDataParam.p_advData = OSAL_Malloc(advDataParam.advLen);

    if (advDataParam.p_advData)
    {
        memcpy(advDataParam.p_advData, s_bleAdvData.advData, s_bleAdvData.advLen);
        memcpy((advDataParam.p_advData + s_bleAdvData.advLen), s_bleScanRspData.advData, s_bleScanRspData.advLen);

        ret = BLE_GAP_SetExtAdvData(&advDataParam);

        OSAL_Free(advDataParam.p_advData);
    }
    else
    {
        ret = APP_RES_OOM;
    }

    if (ret != MBA_RES_SUCCESS)
        return APP_RES_FAIL;

    memset((uint8_t *)&advDataParam, 0, sizeof(BLE_GAP_ExtAdvDataParams_T));

    //Set 2: Legacy Adv
    //Set Adv data
    advDataParam.advHandle = APP_ADV_HANDLE2;
    advDataParam.operation = BLE_GAP_EXT_ADV_DATA_OP_COMPLETE;              //For single packet
    advDataParam.fragPreference = BLE_GAP_EXT_ADV_DATA_FRAG_PREF_MIN;
    advDataParam.advLen = s_bleAdvData.advLen;
    advDataParam.p_advData = s_bleAdvData.advData;

    ret = BLE_GAP_SetExtAdvData(&advDataParam);

    if (ret != MBA_RES_SUCCESS)
        return APP_RES_FAIL;

    //Set Scan Rep Data
    scanDataParam.advHandle = APP_ADV_HANDLE2;
    scanDataParam.operation = BLE_GAP_EXT_ADV_DATA_OP_COMPLETE;              //For single packet
    scanDataParam.fragPreference = BLE_GAP_EXT_ADV_DATA_FRAG_PREF_MIN;
    scanDataParam.advLen = s_bleScanRspData.advLen;
    scanDataParam.p_advData = s_bleScanRspData.advData;

    ret = BLE_GAP_SetExtScanRspData(&scanDataParam);

    if (ret != MBA_RES_SUCCESS)
        return APP_RES_FAIL;
    else
        return APP_RES_SUCCESS;

}
#endif

uint8_t APP_ADV_GetAdvertisingType(void)
{
    return s_bleAdvParam.advType;
}

uint16_t APP_ADV_SetAdvertisingType(uint8_t advType)
{
    if (advType > BLE_GAP_ADV_TYPE_ADV_DIRECT_IND_LOW)
    {
        return APP_RES_INVALID_PARA;
    }

    s_bleAdvParam.advType = advType;

    return APP_RES_SUCCESS;
}

uint16_t APP_ADV_SetAdvertisingParams(void)
{
#ifndef APP_EXT_ADV_ENABLE
    BLE_GAP_AdvParams_T advParams;

    memset((uint8_t *)&advParams, 0, sizeof(BLE_GAP_AdvParams_T));

    advParams.intervalMin = s_bleAdvParam.intervalMin;
    advParams.intervalMax = s_bleAdvParam.intervalMax;
    advParams.type = s_bleAdvParam.advType;
    advParams.peerAddr.addrType = BLE_GAP_ADDR_TYPE_PUBLIC;

    //advParams.peerAddr.addr = ;   //Peer address (For directed adverising used)

    advParams.advChannelMap = BLE_GAP_ADV_CHANNEL_ALL;
    advParams.filterPolicy = s_bleAdvParam.filterPolicy;

    return BLE_GAP_SetAdvParams(&advParams);
#else
    return APP_ADV_SetExtAdvParams();
#endif
}

void APP_ADV_UpdateAdvData(void)
{
    uint8_t idx = 0;

    memset(&s_bleAdvData.advData[0], 0x00, BLE_GAP_ADV_MAX_LENGTH);

    //Flags
    s_bleAdvData.advData[idx++] = (APP_ADV_TYPE_LEN + 1); //length
    s_bleAdvData.advData[idx++] = APP_ADV_TYPE_FLAGS;     //AD Type: flags
    s_bleAdvData.advData[idx++] = APP_ADV_FLAG_LE_GEN_DISCOV | APP_ADV_FLAG_BREDR_NOT_SUPPORTED;

    //Service Data
    s_bleAdvData.advData[idx++] = (APP_ADV_TYPE_LEN + APP_ADV_SRV_DATA_LEN); //length
    s_bleAdvData.advData[idx++] = APP_ADV_TYPE_SRV_DATA_16BIT_UUID;              //AD Type: Service Data
    s_bleAdvData.advData[idx++] = (uint8_t)APP_ADV_SERVICE_UUID_MCHP;
    s_bleAdvData.advData[idx++] = (uint8_t)(APP_ADV_SERVICE_UUID_MCHP >> 8);
    s_bleAdvData.advData[idx++] = APP_ADV_ADD_DATA_CLASS_BYTE;
    s_bleAdvData.advData[idx++] = APP_ADV_PROD_TYPE_BLE_UART;

    s_bleAdvData.advLen = APP_ADV_CalculateDataLength(&s_bleAdvData.advData[0]);
}

void APP_ADV_UpdateScanRspData(void)
{
    uint8_t devNameLen;

    memset(&s_bleScanRspData.advData[0], 0x00, BLE_GAP_ADV_MAX_LENGTH);

    BLE_GAP_GetDeviceName(&devNameLen, &s_bleScanRspData.advData[2]);

    //Local Name
    s_bleScanRspData.advData[0] = (APP_ADV_TYPE_LEN + devNameLen); //length
    s_bleScanRspData.advData[1] = APP_ADV_TYPE_COMPLETE_LOCAL_NAME;

    s_bleScanRspData.advLen = APP_ADV_CalculateDataLength(&s_bleScanRspData.advData[0]);
}

void APP_ADV_SetFilterPolicy(uint8_t filterPolicy)
{
    s_bleAdvParam.filterPolicy = filterPolicy;
}

uint8_t APP_ADV_GetFilterPolicy(void)
{
    return s_bleAdvParam.filterPolicy;
}

uint16_t APP_ADV_Ctrl(uint8_t enable)
{
    uint16_t result = APP_RES_BAD_STATE;

#ifdef APP_BLE_MULTI_LINK_ENABLE
    APP_BLE_ConnList_T *p_bleConnList_t = NULL;

    if (enable)
    {
        p_bleConnList_t = APP_GetFreeConnList();
        
        if (p_bleConnList_t == NULL)
            return APP_RES_NO_RESOURCE;

        if (APP_GetBleStateMtLink(NULL) != APP_BLE_STATE_ADVERTISING)
        {
            #ifndef APP_EXT_ADV_ENABLE
            result = BLE_GAP_SetAdvEnable(true, 0);
            #else
            result = APP_ADV_StartExtAdv();
            #endif

            if (result == APP_RES_SUCCESS)
            {
                APP_SetBleStateMtLink(p_bleConnList_t, APP_BLE_STATE_ADVERTISING);
                APP_SetCurrentBleLinkMtLink(p_bleConnList_t);
                g_appLedGreenHandler = APP_LED_StartByMode(APP_LED_MODE_ADV);
            }
        }
    }
    else
    {
        p_bleConnList_t = APP_GetCurrentBleLinkMtLink();
        if ((APP_GetBleStateMtLink(p_bleConnList_t) == APP_BLE_STATE_ADVERTISING) || 
            (APP_GetBleStateMtLink(p_bleConnList_t) == APP_BLE_STATE_CONNECTED))
        {
            #ifndef APP_EXT_ADV_ENABLE
            result = BLE_GAP_SetAdvEnable(false, 0);
            #else
            result = APP_ADV_StopExtAdv();
            #endif

            if ((result == APP_RES_SUCCESS) && (APP_GetBleStateMtLink(p_bleConnList_t) == APP_BLE_STATE_ADVERTISING))
            {
                APP_SetBleStateMtLink(p_bleConnList_t, APP_BLE_STATE_STANDBY);
            }
        }
    }
#else
    if (enable)
    {
        if (APP_GetBleState() != APP_BLE_STATE_ADVERTISING)
        {
            #ifndef APP_EXT_ADV_ENABLE
            result = BLE_GAP_SetAdvEnable(true, 0);
            #else
            result = APP_ADV_StartExtAdv();
            #endif

            if (result == APP_RES_SUCCESS)
            {
                APP_SetBleState(APP_BLE_STATE_ADVERTISING);
                g_appLedGreenHandler = APP_LED_StartByMode(APP_LED_MODE_ADV);
            }
        }
    }
    else
    {
        if(APP_GetBleState() == APP_BLE_STATE_ADVERTISING)
        {
            #ifndef APP_EXT_ADV_ENABLE
            result = BLE_GAP_SetAdvEnable(false, 0);
            #else
            result = APP_ADV_StopExtAdv();
            #endif

            if(result == APP_RES_SUCCESS)
            {
                APP_SetBleState(APP_BLE_STATE_STANDBY);
            }
        }
    }
#endif  //#ifdef APP_BLE_MULTI_LINK_ENABLE

    return result;
}

void APP_ADV_Start(void)
{
    APP_ADV_SetAdvertisingParams();
    APP_ADV_UpdateAdvData();
    APP_ADV_UpdateScanRspData();

#ifndef APP_EXT_ADV_ENABLE
    BLE_GAP_SetAdvData(&s_bleAdvData);
    BLE_GAP_SetScanRspData(&s_bleScanRspData);
#else
    APP_ADV_SetExtAdvData();
#endif

    APP_ADV_Ctrl(true);
}

void APP_ADV_Stop(void)
{
    APP_ADV_Ctrl(false);
}

void APP_ADV_Init(void)
{
    APP_ADV_InitAdvParams();
    APP_ADV_Start();
}
