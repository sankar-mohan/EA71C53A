/*******************************************************************************
  Combined Interface Basic cluster Source File

  Company:
    Microchip Technology Inc.

  File Name:
   ciBasicCluster.c

  Summary:
    This file contains the Combined Interface Basic cluster interface.

  Description:
    This file contains the Combined Interface Basic cluster interface.
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

#if (APP_Z3_DEVICE_TYPE == APP_DEVICE_TYPE_COMBINED_INTERFACE)

/******************************************************************************
                    Includes section
******************************************************************************/
#include <zigbee_device/clusters/include/haClusters.h>
#include <zigbee_device/devicetypes/combinedInterface/include/ciClusters.h>
#include <zcl/include/zclCommandManager.h>
#include <zigbee_device/devicetypes/combinedInterface/include/ciBasicCluster.h>
#include <zigbee_device/devicetypes/combinedInterface/include/ciIdentifyCluster.h>
#include <zigbee_device/devicetypes/combinedInterface/include/ciOnOffCluster.h>
#include <zigbee_device/devicetypes/combinedInterface/include/ciLevelControlCluster.h>
#include <zigbee_device/devicetypes/combinedInterface/include/ciGroupsCluster.h>
#include <zigbee_device/devicetypes/combinedInterface/include/ciScenesCluster.h>
#include <zigbee_device/devicetypes/combinedInterface/include/ciOccupancySensingCluster.h>
#include <zigbee_device/devicetypes/combinedInterface/include/ciCommissioningCluster.h>
#include <zigbee_device/devicetypes/combinedInterface/include/ciIlluminanceMeasurementCluster.h>
#include <zigbee_device/devicetypes/combinedInterface/include/ciHumidityMeasurementCluster.h>
#include <zigbee_device/devicetypes/combinedInterface/include/ciTemperatureMeasurementCluster.h>
#include <zigbee_device/devicetypes/combinedInterface/include/ciColorControlCluster.h>
#include <zigbee_device/devicetypes/combinedInterface/include/ciThermostatCluster.h>
#include <zigbee_device/devicetypes/combinedInterface/include/ciFanControlCluster.h>
#include <zigbee_device/devicetypes/combinedInterface/include/ciTimeCluster.h>
#include <zigbee_device/devicetypes/combinedInterface/include/ciAlarmsCluster.h>
#include <zigbee_device/devicetypes/combinedInterface/include/ciThermostatUiConfCluster.h>
#include <zigbee_device/devicetypes/combinedInterface/include/ciIasZoneCluster.h>
#include <zigbee_device/devicetypes/combinedInterface/include/ciIasACECluster.h>
#include <app_zigbee/app_zigbee_handler.h>

/******************************************************************************
                    Prototypes section
******************************************************************************/
<#if (BASIC_CLUSTER_CS != "CLIENT")  &&  (BS_RESETTOFNCMD == true) >
static ZCL_Status_t resetToFactoryDefaultsInd(ZCL_Addressing_t *addressing, uint8_t payloadLength, uint8_t *payload);
</#if>
/******************************************************************************
                    Global variables section
******************************************************************************/
ZCL_BasicClusterServerAttributes_t ciBasicClusterServerAttributes =
{
  ZCL_DEFINE_BASIC_CLUSTER_SERVER_ATTRIBUTES()
};

ZCL_BasicClusterClientAttributes_t ciBasicClusterClientAttributes =
{
  ZCL_DEFINE_BASIC_CLUSTER_CLIENT_ATTRIBUTES()
};

PROGMEM_DECLARE (ZCL_BasicClusterServerCommands_t ciBasicClusterServerCommands) =
{
<#if (BASIC_CLUSTER_CS != "CLIENT")  &&  (BS_RESETTOFNCMD == true) >
  ZCL_DEFINE_BASIC_CLUSTER_COMMANDS(resetToFactoryDefaultsInd)
</#if>
};

/******************************************************************************
                    Implementation section
******************************************************************************/
/**************************************************************************//**
\brief Initializes Basic cluster
******************************************************************************/
void ciBasicClusterInit(void)
{
  ZCL_Cluster_t *cluster = ZCL_GetCluster(APP_ENDPOINT_COMBINED_INTERFACE, BASIC_CLUSTER_ID, ZCL_CLUSTER_SIDE_CLIENT);

<#if (BASIC_CLUSTER_CS != "CLIENT")  && (BC_CLUSTERREVISION == true) >
  ciBasicClusterServerAttributes.clusterVersion.value      = CLUSTER_VERSION;
</#if>
<#if (BASIC_CLUSTER_CS != "CLIENT")  && (BC_ZCLVERSION == true) >
  ciBasicClusterServerAttributes.zclVersion.value          = ZCL_VERSION;
</#if>
<#if (BASIC_CLUSTER_CS != "CLIENT")  && (BC_POWERSOURCE == true) >
  ciBasicClusterServerAttributes.powerSource.value         = POWER_SOURCE;
</#if>
<#if (BASIC_CLUSTER_CS != "CLIENT")  && (BC_PHYENV == true) >
  ciBasicClusterServerAttributes.physicalEnvironment.value = UNKNOWN_ENVIRONMENT;
</#if>
<#if (BASIC_CLUSTER_CS != "CLIENT")  && (BC_APPLCIATIONVERSION == true) >
  ciBasicClusterServerAttributes.applicationVersion.value  = APP_VERSION;
</#if>
<#if (BASIC_CLUSTER_CS != "CLIENT")  && (BC_STACKVERSION == true) >
  ciBasicClusterServerAttributes.stackVersion.value        = STACK_VERSION;
</#if>
<#if (BASIC_CLUSTER_CS != "CLIENT")  && (BC_HWVERSION == true) >
  ciBasicClusterServerAttributes.hwVersion.value           = HW_VERSION;
</#if>
<#if (BASIC_CLUSTER_CS != "CLIENT")  && (BC_MANUNAME == true) >
  memcpy(ciBasicClusterServerAttributes.manufacturerName.value, "\x1f Microchip ", 12);
</#if>
<#if (BASIC_CLUSTER_CS != "CLIENT")  && (BC_MODELID == true) >
  memcpy(ciBasicClusterServerAttributes.modelIdentifier.value, "\x1f CombinedInterface", 19);
</#if>
<#if (BASIC_CLUSTER_CS != "CLIENT")  && (BC_DATECODE == true) >
  memcpy(ciBasicClusterServerAttributes.dateCode.value, "\xf 20150901", 10);
</#if>
<#if (BASIC_CLUSTER_CS != "CLIENT")  && (BC_SWBUILDID == true) >
  memcpy(ciBasicClusterServerAttributes.swBuildId.value, "\xf abcdefgh", 10);
</#if>
  /* Client side attributes */
<#if (BASIC_CLUSTER_CS != "SERVER")  && (BS_CLUSTERREVISION == true) >
  ciBasicClusterClientAttributes.clusterVersion.value      = CLUSTER_VERSION;
</#if>
  cluster->ZCL_DefaultRespInd = ZCL_CommandZclDefaultResp;
}

<#if (BASIC_CLUSTER_CS != "CLIENT")  &&  (BS_RESETTOFNCMD == true) >
/**************************************************************************//**
\brief Callback on receive of resetToFactoryDefaults command
******************************************************************************/
static ZCL_Status_t resetToFactoryDefaultsInd(ZCL_Addressing_t *addressing, uint8_t payloadLength, uint8_t *payload)
{
  APP_Zigbee_Event_t event;
  event.eventGroup = EVENT_CLUSTER;
  event.eventId = CMD_ZCL_RESET_TO_FACTORY_DEFAULTS;
  event.eventData.zclEventData.addressing = addressing;
  event.eventData.zclEventData.payloadLength = payloadLength;
  event.eventData.zclEventData.payload = payload;

  for (uint8_t i=0; i<CI_SERVER_CLUSTER_INIT_COUNT; i++)
  {
    if (ciServerClusterInitFunctions[i])
      (ciServerClusterInitFunctions[i])();
  }

  // needs to enable after app directory is created
  //PDS_Store(Z3DEVICE_APP_MEMORY_MEM_ID);

  APP_Zigbee_Handler(event);
  return ZCL_SUCCESS_STATUS;
}
</#if>

<#if (BASIC_CLUSTER_CS != "SERVER")  &&  (BC_RESETTOFNCMD == true) >
/**************************************************************************//**
\brief Sends resetToFactoryDefaults command unicastly

\param[in] mode - address mode;
\param[in] addr - short address of destination node;
\param[in] ep   - destination endpoint;
******************************************************************************/
void basicResetToFactoryDefaultsCommand(APS_AddrMode_t mode,ShortAddr_t addr, Endpoint_t ep, Endpoint_t srcEp)
{
  ZCL_Request_t *req;

  if (!(req = getFreeCommand()))
    return;

  fillCommandRequest(req, ZCL_BASIC_CLUSTER_SERVER_RESET_TO_FACTORY_DEFAULTS_COMMAND_ID, 0, srcEp);
  fillDstAddressing(&req->dstAddressing, mode, addr, ep, BASIC_CLUSTER_ID);

  ZCL_CommandManagerSendCommand(req);
}
</#if>
#endif // APP_DEVICE_TYPE_COMBINED_INTERFACE

// eof ciBasicCluster.c
