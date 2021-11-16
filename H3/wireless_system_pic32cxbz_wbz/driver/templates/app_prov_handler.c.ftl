/*******************************************************************************
  Application BLE - Zigbee Provisioning Handler Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_prov_handler.c

  Summary:
    This file contains the BLE Zigbee Provisioning Handler functions for this 
    project.

  Description:
    This file contains the Application Handler functions for commissioning a 
    zigbee device via BLE link in this project.
    The implementation of demo mode is included in this file.
 *******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2021 Microchip Technology Inc. and its subsidiaries.
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
#include <string.h>
#include <stdint.h>
#include "app_prov.h"
#include "app_trps.h"
#include "app_prov_handler.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Variables
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************

/* TRPS Event handler called from BLE Stack */
void APP_TRPS_EventHandler(BLE_TRSPS_Event_T *p_event)
{ 
    uint8_t idx=0, status = SUCCESS;
    bool cmdFound = false;
    APP_TRPS_CtrlCmd_T *p_Cmd = (APP_TRPS_CtrlCmd_T*)p_event->eventField.onVendorCmd.p_payLoad;
    APP_TRPS_Ctrl_T *p_trpsCtrl = NULL;
    
    switch(p_event->eventId)
    {
        case BLE_TRSPS_EVT_CTRL_STATUS:
        {
            s_trpsConnList_t.connHandle = p_event->eventField.onCtrlStatus.connHandle;
        }
        break;
        
        case BLE_TRSPS_EVT_VENDOR_CMD:
        {
            p_trpsCtrl = APP_GetCtrlListByOpcode(p_Cmd->Opcode);
            if ((s_trpsConnList_t.connHandle == p_event->eventField.onVendorCmd.connHandle)
                && (p_trpsCtrl != NULL))
            {
                s_trpsConnList_t.connHandle = p_event->eventField.onVendorCmd.connHandle;
                
                for(idx =0; idx<p_trpsCtrl->cmdRspSize; idx++)
                {
                    if(p_trpsCtrl->appTrpsCmdResp[idx].CmdId == p_Cmd->ctrlID)
                    {
                        cmdFound = true;
                        break;
                    }
                }
                
                if(cmdFound)
                {
                    if(p_trpsCtrl->appTrpsCmdResp[idx].fnPtr)
                        status = p_trpsCtrl->appTrpsCmdResp[idx].fnPtr(p_event->eventField.onVendorCmd.p_payLoad);
                    APP_TRPS_SendCmdRsp(&p_trpsCtrl->appTrpsCmdResp[idx],p_trpsCtrl->opcode,status);                    
                }
                else
                {
                    APP_TRPS_SendErrorRsp(p_trpsCtrl->opcode,p_Cmd->ctrlID,INVALID_PARAMETER);
                }

            }
            else
            {
                APP_TRPS_SendErrorRsp(p_Cmd->Opcode,0x00,OPCODE_NOT_SUPPORTED);
            }
        }
        break;

        default:
            break;
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
void BZ_Prov_BleGapEvtHandler(BLE_GAP_Event_T *p_event)
{
    switch(p_event->eventId)
    {
        case BLE_GAP_EVT_CONNECTED:
        {
            //GATTS_UpdateBondingInfo(p_event->eventField.evtConnect.connHandle, NULL, 0, NULL);
			isBleConnected = true;
			
        }
        break;

        case BLE_GAP_EVT_DISCONNECTED:
        {
            isBleConnected = false;
<#if (ZIGBEE_DEV_TYPE == "ZIGBEE_COLOR_SCENE_CONTROLLER") || (ZIGBEE_DEV_TYPE == "ZIGBEE_MULTI_SENSOR") || (ZIGBEE_DEV_TYPE == "ZIGBEE_IAS_ACE")>
            if(BDB_GetBdbNodeIsOnANetwork())
            {
                BLE_GAP_AdvParams_T             advParam;
                memset(&advParam, 0, sizeof(BLE_GAP_AdvParams_T));
                advParam.intervalMin = BLE_GAP_ADV_INTERVAL_MAX;
                advParam.intervalMax = BLE_GAP_ADV_INTERVAL_MAX;
                advParam.type = BLE_GAP_ADV_TYPE_ADV_NONCONN_IND;
                advParam.advChannelMap = BLE_GAP_ADV_CHANNEL_ALL;
                advParam.filterPolicy = BLE_GAP_ADV_FILTER_DEFAULT;
                BLE_GAP_SetAdvParams(&advParam);
                SYS_EnableSleepWhenIdle();
            }
			BLE_GAP_SetAdvEnable(0x01, 0);
<#else>
			BLE_GAP_SetAdvEnable(0x01, 0);
</#if>
        }
        break;
        case BLE_GAP_EVT_ENCRYPT_STATUS:
        {
            if (p_event->eventField.evtEncryptStatus.status != BLE_GAP_ENCRYPT_SUCCESS)
            {
                //BLE_GAP_Disconnect(p_event->eventField.evtEncryptStatus.connHandle, GAP_DISC_REASON_REMOTE_TERMINATE);
            }
        }
        break;

        default:
        break;
    }
}


/*******************************************************************************
  Function:
    void Zigbee_Event_Handler(APP_Zigbee_Event_t event)

  Remarks:
    See prototype in app_zigbee_handler.h.
******************************************************************************/

void BZ_Prov_Zigbee_Event_Handler(APP_Zigbee_Event_t event)
{
    // User to handle all zigbee stack events  
    switch(event.eventId)
    {
        case EVENT_COMMISSIONING_FINDBIND_COMPLETE:
	    {
            if (update_find_n_bind() == false)
            {
                event.eventGroup = EVENT_ZIGBEE;
                event.eventId = EVENT_COMMISSIONING_COMPLETE;
                event.eventData.value = BDB_COMMISSIONING_SUCCESS;
                APP_Zigbee_Handler(event);
            }
	    }
	    break;
        case EVENT_COMMISSIONING_FAILURE:
        {
			if (event.eventData.value ==BDB_COMMISSIONING_NO_NETWORK)
			{
				if(startComm)
				{
					if ( isBleConnected == true)
					{
						provStatus = 0;
                        APP_TRPS_SendNotification(APP_TRP_VENDOR_OPCODE_PROV,PROV_STATUS_NFY);
					}
				}				
			}
		}
		break;
        case EVENT_STARTED_CENTRALIZED_NETWORK:
        {
            if(startComm)
            {
                /* APP_TRPS_Sensor_BLE_light_Init(true);                 */
                if(!isDeviceCommissioned)
                {
                    isDeviceCommissioned = true;
                    PDS_Store(PDS_APP_COMMISION_ID);
                }        
				if ( isBleConnected == true)
                {
                    provStatus = (provStatus & 0xFB) | 0x04;
                    //APP_TRPS_SendNotification(APP_TRP_VENDOR_OPCODE_PROV,PROV_STATUS_NFY);
                }
            }
        }
        break;

        case EVENT_STARTED_DISTRIBUTED_NETWORK:
        {
            if(startComm)
            {
                /* APP_TRPS_Sensor_BLE_light_Init(true);                 */
                if(!isDeviceCommissioned)
                {
                    isDeviceCommissioned = true;
                    PDS_Store(PDS_APP_COMMISION_ID);
                }        
				if ( isBleConnected == true)
                {
                    provStatus = (provStatus & 0xFB) | 0x04;
                    //APP_TRPS_SendNotification(APP_TRP_VENDOR_OPCODE_PROV,PROV_STATUS_NFY);
                }
            }
        }
        break;

        case EVENT_NETWORK_ESTABLISHED:
        {
            if(startComm)
            {
                /* APP_TRPS_Sensor_BLE_light_Init(true);    */           
                if(!isDeviceCommissioned)
                {
                    isDeviceCommissioned = true;
                    PDS_Store(PDS_APP_COMMISION_ID);
                }
                if ( isBleConnected == true)
                {
                    provStatus = (provStatus & 0xFE) | 0x01; 
                    //APP_TRPS_SendNotification(APP_TRP_VENDOR_OPCODE_PROV,PROV_STATUS_NFY);
                }
            }
<#if (ZIGBEE_DEV_TYPE == "ZIGBEE_COLOR_SCENE_CONTROLLER") || (ZIGBEE_DEV_TYPE == "ZIGBEE_MULTI_SENSOR") || (ZIGBEE_DEV_TYPE == "ZIGBEE_IAS_ACE")>
            else
            {
                if ( isBleConnected == false)
                {
                    BLE_GAP_SetAdvEnable(0x00, 0);
                    BLE_GAP_AdvParams_T             advParam;
                    memset(&advParam, 0, sizeof(BLE_GAP_AdvParams_T));
                    advParam.intervalMin = BLE_GAP_ADV_INTERVAL_MAX;
                    advParam.intervalMax = BLE_GAP_ADV_INTERVAL_MAX;
                    advParam.type = BLE_GAP_ADV_TYPE_ADV_NONCONN_IND;
                    advParam.advChannelMap = BLE_GAP_ADV_CHANNEL_ALL;
                    advParam.filterPolicy = BLE_GAP_ADV_FILTER_DEFAULT;
                    BLE_GAP_SetAdvParams(&advParam);
                    BLE_GAP_SetAdvEnable(0x01, 0);
                    SYS_EnableSleepWhenIdle();
                }
            }
</#if>
        }
        break;
        case EVENT_JOINED_TO_AN_EXISTING_NETWORK:
        {
            if(startComm)
            {
                /* APP_TRPS_Sensor_BLE_light_Init(true);    */           
                if(!isDeviceCommissioned)
                {
                    isDeviceCommissioned = true;
                    PDS_Store(PDS_APP_COMMISION_ID);
                }
                if ( isBleConnected == true)
                {
                    provStatus = (provStatus & 0xFE) | 0x01; 
                    //APP_TRPS_SendNotification(APP_TRP_VENDOR_OPCODE_PROV,PROV_STATUS_NFY);
                }
            }
        }
        break;
        case EVENT_COMMISSIONING_COMPLETE:
        {
            provStatus = (provStatus & 0xFD);
            if(event.eventData.value == BDB_COMMISSIONING_SUCCESS)
            {
                provStatus = (provStatus & 0xFD) | 0x02;
            }

            if(startComm)
            {
                if ( isBleConnected == true)
                {
                    APP_TRPS_SendNotification(APP_TRP_VENDOR_OPCODE_PROV,PROV_STATUS_NFY);
                }
            }
        }
        break;      
        default:
        break;
    }
}

