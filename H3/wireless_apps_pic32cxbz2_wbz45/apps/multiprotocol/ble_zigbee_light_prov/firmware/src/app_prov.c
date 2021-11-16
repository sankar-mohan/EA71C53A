/*******************************************************************************
  Application Zigbee Provisioning Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_prov.c

  Summary:
    This file contains the Application Transparent Server Role functions for this project.

  Description:
    This file contains the Application Transparent Server Role functions for this project.
    The implementation of demo mode is included in this file.
 *******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2020 Microchip Technology Inc. and its subsidiaries.
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
//DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>
#include "app_trps.h"
#include "app_timer.h"
#include "app.h"
#include "app_ble_handler.h"
#include "app_prov.h"
#include "app_error_defs.h"
#include <configserver/include/configserver.h>
#include <bdb/include/bdb.h>
#include <zigbee_device/api/include/bdb_api.h>
#include <app_zigbee/zigbee_console/console.h>


// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************
char commission_device_name[PDS_APP_DEVICE_NAME_ITEM_SIZE];

PDS_DECLARE_ITEM(PDS_APP_DEVICE_NAME_ID, PDS_APP_DEVICE_NAME_ITEM_SIZE, commission_device_name, NULL, SIZE_MODIFICATION_ALLOWED);

// *****************************************************************************
// *****************************************************************************
// Section: Global Variables
// *****************************************************************************
// *****************************************************************************
APP_TRPS_ProvData_T provData;
bool startComm = false;

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
 static uint8_t APP_TRPS_Prov_Get_Cmd(uint8_t *p_cmd);
 static uint8_t APP_TRPS_Prov_Set_Cmd(uint8_t *p_cmd);


APP_TRPS_CmdResp_T appTrpsProvCmdResp[] = 
{
    PROV_DEFINE_CTRL_CMD_RESP()
};

APP_TRPS_NotifyData_T appTrpsProvNotify[] = 
{
    PROV_DEFINE_CTRL_NOTIFY()
};


/* Prov Get command handling */
 uint8_t APP_TRPS_Prov_Get_Cmd(uint8_t *p_cmd)
{
    switch(p_cmd[2])
    {
        case DEVICE_TYPE_GET_CMD:
        {
            provData.deviceType[0] = (APP_Z3_DEVICE_TYPE>>8)&0xFF;
            provData.deviceType[1] = APP_Z3_DEVICE_TYPE&0xFF;            
        }
        break;
            
        case SW_VERSION_GET_CMD:
        {
            memcpy(provData.swVersion,COMBO_APP_VERSION,sizeof(COMBO_APP_VERSION));
        }            
        break;
            
        case DEVICE_NAME_GET_CMD:
        {
            memset(provData.deviceName, 0, DEVICE_NAME_GET_RSP_LEN);            
            memcpy(provData.deviceName, commission_device_name, strlen(commission_device_name));
        }            
        break;
        
        case CHANNEL_MAP_GET_CMD:
        {
            CS_ReadParameter(CS_BDB_PRIMARY_CHANNELS_MASK_ID, &provData.channelMap);
        }            
        break;
        
        case DEVICE_STATE_GET_CMD:
        {
            provData.deviceState = BDB_GetBdbNodeIsOnANetwork();
        }            
        break;
        
        case MAC_ID_GET_CMD:
        {
            CS_ReadParameter(CS_UID_ID, &provData.macAddr);
        }            
        break;
        
        case CURRENT_CHANNEL_GET_CMD:
        {
            CS_ReadParameter(CS_RF_CURRENT_CHANNEL_ID, &provData.currChannel);
        }            
        break;         
            
    }

    return SUCCESS;
}

/* Prov Set command handling */
 uint8_t APP_TRPS_Prov_Set_Cmd(uint8_t *p_cmd)
{
    uint8_t status = SUCCESS;
    uint32_t channel_mask;
    switch(p_cmd[2])
    {
        case DEVICE_NAME_SET_CMD:
        {
            if( p_cmd[1] <= 17 )
            {
                memset(commission_device_name, 0, PDS_APP_DEVICE_NAME_ITEM_SIZE);
                memcpy(commission_device_name, &p_cmd[3], (p_cmd[1]-1));
                PDS_Store(PDS_APP_DEVICE_NAME_ID);
                
                BLE_GAP_SetDeviceName(strlen(commission_device_name), commission_device_name);
            }
            else
            {
                status = INVALID_PARAMETER;
            }           
        }
        break;
            
        case CHANNEL_MAP_SET_CMD:
        {
            if( p_cmd[1] == 5 )
            {
                memcpy(&channel_mask,&p_cmd[3],4);
                BDB_SetChannelMask(PRIMARY_CHANNEL_MASK, channel_mask); 
            }
            else
            {
                status = INVALID_PARAMETER;
            }
        }            
        break;
            
        case START_COMMISSION_SET_CMD:
        {
            if(!isDeviceCommissioned)
            {
                ZB_SysTaskInit();
                startComm = true;
                provStatus = 0;
            }    
            else
                status = OPERATION_FAILED;
        }            
        break;
        
        case FACTORY_NEW_SET_CMD:
        {
            ZB_BDB_ResetToFactoryNew(true);
        }            
        break;    
    }

    return status;
}

/* Init Prov Specific */
void APP_TRPS_Prov_Init(void)
{
    uint8_t localNameLen;
    /* Init TRPS profile with Prov specific command structure*/
    APP_TRPS_Init(APP_TRP_VENDOR_OPCODE_PROV,appTrpsProvCmdResp,appTrpsProvNotify,PROV_CMD_RESP_LST_SIZE,PROV_NOTIFY_LST_SIZE);
   
    // Configure device name
    if (PDS_IsAbleToRestore(PDS_APP_DEVICE_NAME_ID))
    {
      PDS_Restore(PDS_APP_DEVICE_NAME_ID);
      BLE_GAP_SetDeviceName(strlen(commission_device_name), commission_device_name);
    }
    else
    {
      APP_UpdateLocalName(0, NULL);   
      BLE_GAP_GetDeviceName(&localNameLen, &commission_device_name[0]);
    } 
}

/* Do the BLE Prov specific on disconnection  */
void APP_TRPS_Prov_DiscEvtProc(void)
{
  
}
