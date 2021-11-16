/*******************************************************************************
  Color Scene Controller Color Control cluster Source File

  Company:
    Microchip Technology Inc.

  File Name:
    cscColorControlCluster.c

  Summary:
    This file contains the Color Scene Controller device Color Control cluster interface.

  Description:
    This file contains the Color Scene Controller device Color Control cluster interface.
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


#if (APP_Z3_DEVICE_TYPE == APP_DEVICE_TYPE_COLOR_SCENE_CONTROLLER)

/******************************************************************************
                    Includes section
******************************************************************************/
#include <zigbee_device/clusters/include/haClusters.h>
#include <zigbee_device/common/include/appConsts.h>
#include <zcl/include/zclCommandManager.h>
#include <zigbee_device/clusters/include/colorControlCluster.h>
#include <zigbee_device/devicetypes/colorSceneController/include/cscColorControlCluster.h>
#include <app_zigbee/zigbee_console/console.h>
#include <app_zigbee/app_zigbee_handler.h>

/******************************************************************************
                    Global variables section
******************************************************************************/
ZCL_ColorControlClusterCommands_t cscColorControlClusterCommands =
{
<#if (COLORCONTROL_CLUSTER_ENABLE == true) && (COLORCONTROL_CLUSTER_CS != "CLIENT")  && (CCC_SERVERCOMMANDCOUNT != 0) >
  ZCL_COLOR_CONTROL_CLUSTER_COMMANDS_FOR_COLOR_SCENE_CONTROLLER()
<#else>
  NULL
</#if>
};

ZCL_ColorControlClusterClientAttributes_t cscColorControlClusterClientAttributes =
{
  ZCL_DEFINE_COLOR_CONTROL_CLIENT_CLUSTER_ATTRIBUTES()
};

/******************************************************************************
                    Prototypes section
******************************************************************************/
static void cscColorControlReportInd(ZCL_Addressing_t *addressing, uint8_t reportLength, uint8_t *reportPayload);

/******************************************************************************
                    Implementation section
******************************************************************************/
/**************************************************************************//**
\brief Initializes On/Off cluster
******************************************************************************/
void cscColorControlClusterInit(void)
{
  ZCL_Cluster_t *cluster = ZCL_GetCluster(APP_ENDPOINT_COLOR_SCENE_CONTROLLER, COLOR_CONTROL_CLUSTER_ID, ZCL_CLUSTER_SIDE_CLIENT);
  
  if (cluster)
  {
    cluster->ZCL_ReportInd = cscColorControlReportInd;
    cluster->ZCL_DefaultRespInd = ZCL_CommandZclDefaultResp;
  }
<#if (COLORCONTROL_CLUSTER_CS != "SERVER")  && (CCC_CLUSTERREVISION == true) >
  cscColorControlClusterClientAttributes.clusterVersion.value = CLUSTER_VERSION;
</#if>
}

/**************************************************************************//**
\brief Report attribute indication handler

\param[in] addressing - pointer to addressing information;
\param[in] reportLength - data payload length;
\param[in] reportPayload - data pointer
******************************************************************************/
static void cscColorControlReportInd(ZCL_Addressing_t *addressing, uint8_t reportLength, uint8_t *reportPayload)
{
  ZCL_Report_t *rep = (ZCL_Report_t *)reportPayload;
  uint16_t reportValue;

  APP_Zigbee_Event_t event;
  event.eventGroup = EVENT_CLUSTER;
  event.eventId = CMD_ZCL_REPORTING_COLOR_CONTROL;
  event.eventData.zclEventData.addressing = addressing;
  event.eventData.zclEventData.payloadLength = reportLength;
  event.eventData.zclEventData.payload = (uint8_t*)reportPayload;

  APP_Zigbee_Handler(event);
}

#endif // (APP_Z3_DEVICE_TYPE == APP_DEVICE_TYPE_COLOR_SCENE_CONTROLLER)

// eof cscColorControlCluster.c
