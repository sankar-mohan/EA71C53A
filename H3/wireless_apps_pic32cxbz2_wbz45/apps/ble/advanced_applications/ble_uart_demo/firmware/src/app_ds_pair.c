/*******************************************************************************
  Application Data Storage Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_ds_pair.c

  Summary:
    This file contains the Application Data Storage functions for storing paired device information.

  Description:
    This file contains the Application Data Storage functions for storing paired device information.
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
#include "app_ds.h"
#include "app_ds_pair.h"
#include "app_ble_handler.h"
#include "app_adv.h"
#include "app_error_defs.h"
#include "driver/pds/include/pds.h"



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
static APP_DS_PairedDevInfo_T s_pairedDevInfo[BLE_DM_MAX_PAIRED_DEVICE_NUM];
static bool s_writeCompleted;

PDS_DECLARE_FILE(APP_DS_PAIRED_ITEM_ID_1, (BLE_DM_MAX_PAIRED_DEVICE_NUM * sizeof(APP_DS_PairedDevInfo_T)), &s_pairedDevInfo[0], FILE_INTEGRITY_CONTROL_MARK);


// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************

static void APP_DS_PairWriteCompelteHandle(PDS_MemId_t id)
{
    if (id == APP_DS_PAIRED_ITEM_ID_1)
    {
        s_writeCompleted = true;
    }
}

static void APP_DS_PairInitPara(void)
{
    uint8_t i;

    PDS_InitItems(APP_DS_PAIRED_ITEM_ID_1, APP_DS_PAIRED_ITEM_ID_1);

    for (i = 0; i < BLE_DM_MAX_PAIRED_DEVICE_NUM; i++)
    {
        memset((uint8_t *)&s_pairedDevInfo[i], APP_DS_INIT_VALUE, sizeof(APP_DS_PairedDevInfo_T));
    }

}

uint16_t APP_DS_PairInit(void)
{
    s_writeCompleted = true;

    APP_DS_PairInitPara();

    PDS_RegisterWriteCompleteCallback(APP_DS_PairWriteCompelteHandle);

    return APP_RES_SUCCESS;
}

static uint8_t APP_DS_PairGetFreeIdx(APP_DS_PairedDevInfo_T *p_pairedDevInfo)
{
    uint8_t i;

    for (i = 0; i < BLE_DM_MAX_PAIRED_DEVICE_NUM; i++)
    {
        if (p_pairedDevInfo[i].pairedId == APP_DS_INIT_VALUE)
        {
            break;
        }
    }

    return i;
}

uint16_t APP_DS_PairGetStoredInfo(uint8_t devId, APP_DS_PairedDevInfo_T *p_pairedDevInfo)
{
    uint16_t ret = APP_RES_FAIL;

    if (devId >= BLE_DM_MAX_PAIRED_DEVICE_NUM)
    {
        return APP_RES_INVALID_PARA;
    }

    if (PDS_IsAbleToRestore(APP_DS_PAIRED_ITEM_ID_1))
    {
        if ((!s_writeCompleted) || (PDS_Restore(APP_DS_PAIRED_ITEM_ID_1)))
        {
            uint8_t i;
            for (i = 0; i < BLE_DM_MAX_PAIRED_DEVICE_NUM; i++)
              {
                  if ((s_pairedDevInfo[i].pairedId - APP_DS_PAIR_ID_OFFSET) == devId)
                  {
                      memcpy((uint8_t *)p_pairedDevInfo, (uint8_t *)&s_pairedDevInfo[i], sizeof(APP_DS_PairedDevInfo_T));
                      
                      ret = APP_RES_SUCCESS;

                      break;
                  }
              }
        }
    }

    return ret;
}

//Decrease the connected priority higher than assigned one
static void APP_DS_PairDegradePriority(uint8_t priority, APP_DS_PairedDevInfo_T *p_pairedDevInfo)
{
    uint8_t i;

    for (i = 0; i < BLE_DM_MAX_PAIRED_DEVICE_NUM; i++)
    {
        if ((p_pairedDevInfo[i].priority >= APP_DS_PAIR_HIGHEST_PRIORITY) && (p_pairedDevInfo[i].priority < priority))
        {
            p_pairedDevInfo[i].priority++;
        }
    }
}

uint8_t APP_DS_PairFindDevIdByPriority(uint8_t priority)
{
    uint8_t devId;
    APP_DS_PairedDevInfo_T pairedDevInfo;

    for (devId = 0; devId < BLE_DM_MAX_PAIRED_DEVICE_NUM; devId++)
    {
        memset((uint8_t *)&pairedDevInfo, 0, sizeof(APP_DS_PairedDevInfo_T));

        if (APP_DS_PairGetStoredInfo(devId, &pairedDevInfo) == APP_RES_SUCCESS)
        {
            if (pairedDevInfo.priority == priority)
            {
                break;
            }
        }
    }

    return devId;
}

uint16_t APP_DS_PairUpdateInfo(uint8_t devId)
{
    uint16_t ret = APP_RES_SUCCESS;
    bool needStore = false;
    bool hasBonded = false;
    uint8_t i;

    if (PDS_IsAbleToRestore(APP_DS_PAIRED_ITEM_ID_1))
    {
        if ((!s_writeCompleted) || (PDS_Restore(APP_DS_PAIRED_ITEM_ID_1)))
        {
            for (i = 0; i < BLE_DM_MAX_PAIRED_DEVICE_NUM; i++)
            {
                if (s_pairedDevInfo[i].pairedId == (devId + APP_DS_PAIR_ID_OFFSET))
                {
                    hasBonded = true;   //paired record exists already
                    break;
                }
            }

            if (hasBonded)
            {
                if (s_pairedDevInfo[i].priority != APP_DS_PAIR_HIGHEST_PRIORITY)
                {
                    APP_DS_PairDegradePriority(s_pairedDevInfo[i].priority, &s_pairedDevInfo[0]);   //Adjust priority 

                    s_pairedDevInfo[i].priority = APP_DS_PAIR_HIGHEST_PRIORITY;

                    needStore = true;
                }
            }
            else  //paired record is NOT existed
            {
                uint8_t idx;
                idx = APP_DS_PairGetFreeIdx(&s_pairedDevInfo[0]);
                if (idx == BLE_DM_MAX_PAIRED_DEVICE_NUM)   //Paired list is  full
                {
                    return APP_RES_FAIL;
                }

                APP_DS_PairDegradePriority(APP_DS_PAIR_LOWEST_PRIORITY, &s_pairedDevInfo[0]);   //Adjust priority before storing the new one

                s_pairedDevInfo[idx].pairedId = devId + APP_DS_PAIR_ID_OFFSET;
                s_pairedDevInfo[idx].priority = APP_DS_PAIR_HIGHEST_PRIORITY;

                needStore = true;
            }
        }
        else
        {
            return APP_RES_FAIL;
        }
    }
    else   //paired record is empty
    {
        APP_DS_PairInitPara();

        s_pairedDevInfo[0].pairedId = devId + APP_DS_PAIR_ID_OFFSET;
        s_pairedDevInfo[0].priority = APP_DS_PAIR_HIGHEST_PRIORITY;

        needStore = true;
    }

    if (needStore)
    {
        if (!PDS_Store(APP_DS_PAIRED_ITEM_ID_1))
        {
            ret = APP_RES_FAIL;
        }
        else
        {
            s_writeCompleted = false;
        }
    }

    return ret;
}

uint16_t APP_DS_PairDeleteStoredInfo(uint8_t devId)
{
    uint16_t ret = APP_RES_SUCCESS;

    if (devId >= BLE_DM_MAX_PAIRED_DEVICE_NUM)
        return APP_RES_INVALID_PARA;

    ret = BLE_DM_DeletePairedDevice(devId);
    if (ret != MBA_RES_SUCCESS)
    {
        return ret;
    }

    if (PDS_IsAbleToRestore(APP_DS_PAIRED_ITEM_ID_1))
    {
        if ((!s_writeCompleted) || (PDS_Restore(APP_DS_PAIRED_ITEM_ID_1)))
        {
            uint8_t i;

            for (i = 0; i < BLE_DM_MAX_PAIRED_DEVICE_NUM; i++)
            {
                if ((s_pairedDevInfo[i].pairedId - APP_DS_PAIR_ID_OFFSET) == devId)
                {
                    memset((uint8_t *)&s_pairedDevInfo[i], APP_DS_INIT_VALUE, sizeof(APP_DS_PairedDevInfo_T));
                    break;
                }
            }
        }
        else
        {
            return APP_RES_FAIL;
        }
    }
    else
    {
        return APP_RES_FAIL;
    }

    if (!PDS_Store(APP_DS_PAIRED_ITEM_ID_1))
    {
        return APP_RES_FAIL;
    }
    else
    {
        s_writeCompleted = false;
    }

    return ret;
}

uint16_t APP_DS_PairDeleteAllStoredInfo(void)
{
    uint16_t ret;
    bool needStopAdv = true;

    ret = BLE_DM_DeleteAllPairedDevice();
    if (ret != MBA_RES_SUCCESS)
    {
        return ret;
    }

    //Check if the local device is under Trusted Device Connectable Mode
    uint8_t filterPolicy;
    filterPolicy = APP_ADV_GetFilterPolicy();

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

    //Clear Accept list and Resolving list
    BLE_DM_SetFilterAcceptList(0, NULL);

    BLE_DM_SetResolvingList(0, NULL, NULL);


    if (filterPolicy == BLE_GAP_ADV_FILTER_SCAN_CONNECT)
    {
        APP_ADV_SetFilterPolicy(BLE_GAP_ADV_FILTER_DEFAULT);
    }

    if (needStopAdv)
    {
        APP_ADV_Start();
    }

    if (PDS_Delete(APP_DS_PAIRED_ITEM_ID_1) == PDS_SUCCESS)
    {
        return APP_RES_SUCCESS;
    }
    else
    {
        return APP_RES_FAIL;
    }
}

