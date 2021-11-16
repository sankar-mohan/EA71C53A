/*******************************************************************************
  Application Timer Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_scan.c

  Summary:
    This file contains the Application scan functions for this project.

  Description:
    This file contains the Application scan functions for this project.
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

#ifdef APP_TRPC_ENABLE
#include "app_ble_handler.h"
#include "app_scan.h"
#include "app_led.h"
#include "app_timer.h"
#include "app_trp_common.h"
#include "app_error_defs.h"
#include "app_log.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define APP_SCAN_INTERVAL               0x3C
#define APP_SCAN_WINDOW                 0x1E
#define APP_SCAN_MIN_RSSI               ((int8_t)0 - 127)
#define APP_SCAN_2M_CONN_INTERVAL       0x10    // 20 ms
#define APP_SCAN_2M_SUPERVISION_TO      0x48;   // 720 ms
#define APP_SCAN_CODED_CONN_INTERVAL    0x50    // 100 ms
#define APP_SCAN_CODED_SUPERVISION_TO   0x168;   // 3600 ms

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
BLE_GAP_EvtAdvReport_T g_appScanAdvReport_t;
#ifdef APP_EXT_SCAN_ENABLE
BLE_GAP_EvtExtAdvReport_T g_appScanExtAdvReport_t;
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************
static APP_TIMER_TmrElem_T  s_appScanAdvTimer;


// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
#ifdef APP_EXT_SCAN_ENABLE
static void APP_SCAN_FillExtScanParam(BLE_GAP_ExtScanningPhy_T  *extScanPhy)
{
    extScanPhy->le1mPhy.type = BLE_GAP_SCAN_TYPE_ACTIVE_SCAN;
    extScanPhy->le1mPhy.interval = APP_SCAN_INTERVAL;
    extScanPhy->le1mPhy.window = APP_SCAN_WINDOW;
    extScanPhy->leCodedPhy.type = BLE_GAP_SCAN_TYPE_ACTIVE_SCAN;
    extScanPhy->leCodedPhy.interval = APP_SCAN_INTERVAL;
    extScanPhy->leCodedPhy.window = APP_SCAN_WINDOW;
}

static void APP_SCAN_FillExtScanEnable(bool enable, BLE_GAP_ExtScanningEnable_T *p_extScanEn_t)
{
    memset((uint8_t *) p_extScanEn_t, 0, sizeof(BLE_GAP_ExtScanningEnable_T));
    p_extScanEn_t->enable = enable;
    p_extScanEn_t->filterDuplicates = BLE_GAP_SCAN_FD_DISABLE;
    p_extScanEn_t->duration = 0;
    p_extScanEn_t->period = 0;
}
#else
static void APP_SCAN_FillScanParam(BLE_GAP_ScanningParams_T *p_scanParam)
{
    p_scanParam->type = BLE_GAP_SCAN_TYPE_ACTIVE_SCAN;
    p_scanParam->interval = APP_SCAN_INTERVAL;
    p_scanParam->window = APP_SCAN_WINDOW;
    p_scanParam->filterPolicy = BLE_GAP_SCAN_FP_ACCEPT_ALL;
}
#endif

static void APP_SCAN_FillScanFilterParam(BLE_GAP_ScanFilterConfig_T *p_scanfilterParam)
{
    // uint8_t remoteName[]={"BLE_UART_53B7"};
    uint8_t i;

    // Fill scan name filter
    // p_scanfilterParam->nameFilterEnable = true;
    // p_scanfilterParam->nameFilter.nameLen = sizeof(remoteName);
    // memcpy(p_scanfilterParam->nameFilter.targetName, remoteName, p_scanfilterParam->nameFilter.nameLen);
    // Fill service data filter
    p_scanfilterParam->svcDataFilterEnable = true;
    i = 0;
    p_scanfilterParam->serviceDataFilter.uuidData[i++] = (uint8_t) APP_SCAN_SERVICE_UUID_MCHP;
    p_scanfilterParam->serviceDataFilter.uuidData[i++] = (uint8_t) (APP_SCAN_SERVICE_UUID_MCHP >> 8);
    p_scanfilterParam->serviceDataFilter.uuidLen = i;
    i = 0;
    p_scanfilterParam->serviceDataFilter.svcData[i++] = APP_SCAN_ADD_DATA_CLASS_BYTE;
    p_scanfilterParam->serviceDataFilter.svcData[i++] = APP_SCAN_PROD_TYPE_BLE_UART;
    p_scanfilterParam->serviceDataFilter.svcDataLen = i;
}

static void APP_SCAN_ClearAdvReport(void)
{
    #ifdef APP_EXT_SCAN_ENABLE
    memset((uint8_t *)&(g_appScanExtAdvReport_t), 0, sizeof(BLE_GAP_EvtExtAdvReport_T));
    g_appScanExtAdvReport_t.rssi = APP_SCAN_MIN_RSSI;
    #endif
    memset((uint8_t *)&(g_appScanAdvReport_t), 0, sizeof(BLE_GAP_EvtAdvReport_T));
    g_appScanAdvReport_t.rssi = APP_SCAN_MIN_RSSI;
}

uint16_t APP_SCAN_Ctrl(uint8_t enable)
{
    uint16_t result = APP_RES_BAD_STATE;
    #ifdef APP_EXT_SCAN_ENABLE
    BLE_GAP_ExtScanningEnable_T extScanEn_t = {0};
    #endif

#ifdef APP_BLE_MULTI_LINK_ENABLE
    APP_BLE_ConnList_T *p_bleConnList_t = NULL;

    if (enable)
    {
        p_bleConnList_t = APP_GetFreeConnList();
        
        if (p_bleConnList_t == NULL)
            return APP_RES_NO_RESOURCE;

        if (APP_GetBleStateMtLink(NULL) != APP_BLE_STATE_SCANNING)
        {
            #ifdef APP_EXT_SCAN_ENABLE
            APP_SCAN_FillExtScanEnable(true, &extScanEn_t);
            result = BLE_GAP_SetExtScanningEnable(BLE_GAP_SCAN_MODE_OBSERVER, &extScanEn_t);
            #else
            result = BLE_GAP_SetScanningEnable(true, BLE_GAP_SCAN_FD_DISABLE, BLE_GAP_SCAN_MODE_OBSERVER, 0);
            #endif
            
            if (result == APP_RES_SUCCESS)
            {
                APP_SCAN_ClearAdvReport();
                APP_SetBleStateMtLink(p_bleConnList_t, APP_BLE_STATE_SCANNING);
                APP_SetCurrentBleLinkMtLink(p_bleConnList_t);
                g_appLedRedHandler = APP_LED_StartByMode(APP_LED_MODE_SCAN);
                APP_TRP_COMMON_SetTimer(APP_TRP_TMR_ID_INST_MERGE(APP_TIMER_SCAN_ADV_05, 0), NULL, APP_TIMER_5S, false, &s_appScanAdvTimer);
                APP_LOG_INFO("start scan %d\n", result);
            }
        }
    }
    else
    {
        if (APP_GetBleState() == APP_BLE_STATE_SCANNING)
        {
            APP_TIMER_StopTimer(&(s_appScanAdvTimer.tmrHandle));
            #ifdef APP_EXT_SCAN_ENABLE
            APP_SCAN_FillExtScanEnable(false, &extScanEn_t);
            result = BLE_GAP_SetExtScanningEnable(BLE_GAP_SCAN_MODE_OBSERVER, &extScanEn_t);
            #else
            result = BLE_GAP_SetScanningEnable(false, BLE_GAP_SCAN_FD_DISABLE, BLE_GAP_SCAN_MODE_OBSERVER, 0);
            #endif
            if(result == APP_RES_SUCCESS)
            {
                APP_SetBleState(APP_BLE_STATE_STANDBY);
            }
        }
    }

#else
    if (enable)
    {
        if (APP_GetBleState() != APP_BLE_STATE_SCANNING)
        {
            result = BLE_GAP_SetScanningEnable(true, BLE_GAP_SCAN_FD_DISABLE, BLE_GAP_SCAN_MODE_OBSERVER, 0);
            if (result == APP_RES_SUCCESS)
            {
                APP_SCAN_ClearAdvReport();
                APP_SetBleState(APP_BLE_STATE_SCANNING);
                g_appLedRedHandler = APP_LED_StartByMode(APP_LED_MODE_SCAN);
                APP_TRP_COMMON_SetTimer(APP_TRP_TMR_ID_INST_MERGE(APP_TIMER_SCAN_ADV_05, 0), NULL, 
                    APP_TIMER_5S, false, &s_appScanAdvTimer);
                APP_LOG_INFO("start scan %d\n", result);
            }
        }
    }
    else
    {
        if (APP_GetBleState() == APP_BLE_STATE_SCANNING)
        {
            APP_TIMER_StopTimer(&(s_appScanAdvTimer.tmrHandle));
            result = BLE_GAP_SetScanningEnable(false, BLE_GAP_SCAN_FD_DISABLE, BLE_GAP_SCAN_MODE_OBSERVER, 0);
            if(result == APP_RES_SUCCESS)
            {
                APP_SetBleState(APP_BLE_STATE_STANDBY);
            }
        }
    }
#endif  //#ifdef APP_BLE_MULTI_LINK_ENABLE

    return result;
}

void APP_SCAN_Start(void)
{
    APP_SCAN_Ctrl(true);
}

void APP_SCAN_Stop(void)
{
     APP_SCAN_Ctrl(false);
}

void APP_SCAN_Init(void)
{
    #ifdef APP_EXT_SCAN_ENABLE
    BLE_GAP_ExtScanningPhy_T        extScanPhy;
    #else
    BLE_GAP_ScanningParams_T    scanParam;
    #endif
    BLE_GAP_ScanFilterConfig_T  scanFilterParam;
      
    memset((uint8_t *) &s_appScanAdvTimer, 0, sizeof(APP_TIMER_TmrElem_T));
    #ifdef APP_EXT_SCAN_ENABLE
    memset((uint8_t *) &extScanPhy, 0, sizeof(BLE_GAP_ExtScanningPhy_T));
    APP_SCAN_FillExtScanParam(&extScanPhy);
    #ifdef APP_BLE_CODED_PHY_ENABLE
    extScanPhy.le1mPhy.enable = false;
    extScanPhy.leCodedPhy.enable = true;
    #else
    extScanPhy.le1mPhy.enable = true;
    extScanPhy.leCodedPhy.enable = false;
    #endif
    BLE_GAP_SetExtScanningParams(BLE_GAP_SCAN_FP_ACCEPT_ALL, &extScanPhy);
    #else
    memset((uint8_t *) &scanParam, 0, sizeof(BLE_GAP_ScanningParams_T));
    APP_SCAN_FillScanParam(&scanParam);
    BLE_GAP_SetScanningParam(&scanParam);
    #endif
    memset((uint8_t *) &scanFilterParam, 0, sizeof(BLE_GAP_ScanFilterConfig_T));
    APP_SCAN_FillScanFilterParam(&scanFilterParam);
    BLE_GAP_SetScanningFilter(&scanFilterParam);

    // APP_SCAN_Start();
}

void APP_SCAN_ConnectDevice(void)
{
#ifdef APP_EXT_SCAN_ENABLE
    uint8_t filterPolicy;
    BLE_GAP_Addr_T peerAddr;
    BLE_GAP_ExtCreateConnPhy_T createConnPhy_t;

    #if 0
    uint8_t i;

    APP_LOG_DEBUG("Adv:Type=%d,RSSI=%x\n", g_appScanExtAdvReport_t.eventType,
        g_appScanExtAdvReport_t.rssi);
    APP_LOG_DEBUG("Adv_AddrType: %x,", g_appScanExtAdvReport_t.addr.addrType);
    for (i = 0; i < 6; i++)
        APP_LOG_DEBUG(" %x", g_appScanExtAdvReport_t.addr.addr[i]);
    APP_LOG_DEBUG("\nAdvReport: ");
    for (i = 0; i < g_appScanExtAdvReport_t.length; i++)
        APP_LOG_DEBUG("%x ", g_appScanExtAdvReport_t.advData[i]);
    APP_LOG_DEBUG("\n");
    #endif
    createConnPhy_t.le1mPhy.enable = false;
    createConnPhy_t.le2mPhy.enable = false;
    createConnPhy_t.leCodedPhy.enable = false;
    if (g_appScanExtAdvReport_t.rssi > APP_SCAN_MIN_RSSI)
    {
        APP_TIMER_StopTimer(&(s_appScanAdvTimer.tmrHandle));
        APP_SCAN_Stop();
        filterPolicy = BLE_GAP_SCAN_FP_ACCEPT_ALL;
        peerAddr.addrType = g_appScanExtAdvReport_t.addr.addrType;
        memcpy(peerAddr.addr, g_appScanExtAdvReport_t.addr.addr, GAP_MAX_BD_ADDRESS_LEN);
        #ifdef APP_BLE_CODED_PHY_ENABLE
        createConnPhy_t.leCodedPhy.enable = true;
        createConnPhy_t.leCodedPhy.scanInterval = APP_SCAN_INTERVAL;
        createConnPhy_t.leCodedPhy.scanWindow = APP_SCAN_WINDOW;
        createConnPhy_t.leCodedPhy.connParams.intervalMin = APP_SCAN_CODED_CONN_INTERVAL;
        createConnPhy_t.leCodedPhy.connParams.intervalMax = APP_SCAN_CODED_CONN_INTERVAL;
        createConnPhy_t.leCodedPhy.connParams.latency = 0;
        createConnPhy_t.leCodedPhy.connParams.supervisionTimeout = APP_SCAN_CODED_SUPERVISION_TO;
        #else
        createConnPhy_t.le1mPhy.enable = true;
        createConnPhy_t.le1mPhy.scanInterval = APP_SCAN_INTERVAL;
        createConnPhy_t.le1mPhy.scanWindow = APP_SCAN_WINDOW;
        createConnPhy_t.le1mPhy.connParams.intervalMin = APP_SCAN_2M_CONN_INTERVAL;
        createConnPhy_t.le1mPhy.connParams.intervalMax = APP_SCAN_2M_CONN_INTERVAL;
        createConnPhy_t.le1mPhy.connParams.latency = 0;
        createConnPhy_t.le1mPhy.connParams.supervisionTimeout = APP_SCAN_2M_SUPERVISION_TO;

        createConnPhy_t.le2mPhy.enable = true;
        createConnPhy_t.le2mPhy.scanInterval = APP_SCAN_INTERVAL;
        createConnPhy_t.le2mPhy.scanWindow = APP_SCAN_WINDOW;
        createConnPhy_t.le2mPhy.connParams.intervalMin = APP_SCAN_2M_CONN_INTERVAL;
        createConnPhy_t.le2mPhy.connParams.intervalMax = APP_SCAN_2M_CONN_INTERVAL;
        createConnPhy_t.le2mPhy.connParams.latency = 0;
        createConnPhy_t.le2mPhy.connParams.supervisionTimeout = APP_SCAN_2M_SUPERVISION_TO;
        #endif
        BLE_GAP_ExtCreateConnection(filterPolicy, &peerAddr, &createConnPhy_t);
    }
    else
        APP_TRP_COMMON_SetTimer(APP_TRP_TMR_ID_INST_MERGE(APP_TIMER_SCAN_ADV_05, 0), NULL, APP_TIMER_5S, 
            false, &s_appScanAdvTimer);
#else
    BLE_GAP_CreateConnParams_T createConnParam_t;

    #if 0
    uint8_t i;

    APP_LOG_DEBUG("Adv:Type=%d,RSSI=%x\n", g_appScanAdvReport_t.eventType,
        g_appScanAdvReport_t.rssi);
    APP_LOG_DEBUG("Adv_AddrType: %x,", g_appScanAdvReport_t.addr.addrType);
    for (i = 0; i < 6; i++)
        APP_LOG_DEBUG(" %x", g_appScanAdvReport_t.addr.addr[i]);
    APP_LOG_DEBUG("\nAdvReport: ");
    for (i = 0; i < g_appScanAdvReport_t.length; i++)
        APP_LOG_DEBUG("%x ", g_appScanAdvReport_t.advData[i]);
    APP_LOG_DEBUG("\n");
    #endif
    
    if (g_appScanAdvReport_t.rssi > APP_SCAN_MIN_RSSI)
    {
        APP_TIMER_StopTimer(&(s_appScanAdvTimer.tmrHandle));
        APP_SCAN_Stop();
        createConnParam_t.scanInterval = APP_SCAN_INTERVAL;
        createConnParam_t.scanWindow = APP_SCAN_WINDOW;
        createConnParam_t.filterPolicy = BLE_GAP_SCAN_FP_ACCEPT_ALL;
        createConnParam_t.peerAddr.addrType = g_appScanAdvReport_t.addr.addrType;
        memcpy(createConnParam_t.peerAddr.addr, g_appScanAdvReport_t.addr.addr, GAP_MAX_BD_ADDRESS_LEN);
        createConnParam_t.connParams.intervalMin = APP_SCAN_2M_CONN_INTERVAL;
        createConnParam_t.connParams.intervalMax = APP_SCAN_2M_CONN_INTERVAL;
        createConnParam_t.connParams.latency = 0;
        createConnParam_t.connParams.supervisionTimeout = APP_SCAN_2M_SUPERVISION_TO;
        BLE_GAP_CreateConnection(&createConnParam_t);
    }
    else
        APP_TRP_COMMON_SetTimer(APP_TRP_TMR_ID_INST_MERGE(APP_TIMER_SCAN_ADV_05, 0), NULL, APP_TIMER_5S, 
            false, &s_appScanAdvTimer);
#endif //#ifdef APP_EXT_SCAN_ENABLE
}

#endif //#ifdef APP_TRPC_ENABLE

