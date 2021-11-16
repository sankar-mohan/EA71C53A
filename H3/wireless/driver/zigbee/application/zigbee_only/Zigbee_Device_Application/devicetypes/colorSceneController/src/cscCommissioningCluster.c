/*******************************************************************************
  Color scene controller Commissioning cluster Source File

  Company:
    Microchip Technology Inc.

  File Name:
    cscCommissioningCluster.c

  Summary:
    This file contains the Color scene controller Commissioning cluster interface.

  Description:
    This file contains the Color scene controller Commissioning cluster interface.
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
#include <zigbee_device/devicetypes/colorSceneController/include/cscCommissioningCluster.h>
#include <zcl/include/zclCommandManager.h>
#include <zigbee_device/clusters/include/commissioningCluster.h>
#include <app_zigbee/zigbee_console/console.h>

/******************************************************************************
                    Prototypes section
******************************************************************************/
static ZCL_Status_t getGroupIdentifiersInd(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_GetGroupIdentifiers_t *payload);
static ZCL_Status_t getEndpointListInd(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_GetEndpointList_t *payload);
static ZCL_Status_t endpointInformationResponseInd(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_EndpointInformation_t *payload);
static ZCL_Status_t getGroupIdentifiersResponseInd(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_GetGroupIdentifiersResponse_t *payload);
static ZCL_Status_t getEndpointListResponseInd(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_GetEndpointListResponse_t *payload);

/******************************************************************************
                    Global variables
******************************************************************************/
ZCL_CommissioningClusterClientAttributes_t cscCommissioningClusterClientAttributes =
{
  ZCL_DEFINE_COMMISSIONING_CLUSTER_CLIENT_ATTRIBUTES()
};

ZCL_CommissioningClusterServerAttributes_t cscCommissioningClusterServerAttributes =
{
  ZCL_DEFINE_COMMISSIONING_CLUSTER_SERVER_ATTRIBUTES()
};

ZCL_CommissioningClusterCommands_t cscCommissioningClusterCommands =
{
  ZCL_DEFINE_COMMISSIONING_CLUSTER_COMMANDS(getGroupIdentifiersInd, getEndpointListInd,
      endpointInformationResponseInd, getGroupIdentifiersResponseInd, getEndpointListResponseInd)
};

/******************************************************************************
                    Implementations section
******************************************************************************/
/**************************************************************************//**
\brief Initialize Commissioning cluster server.
******************************************************************************/
void cscCommissioningServerClusterInit(void)
{
  /**
  * Commissioning Server Cluster Of Color Scene Controller
  */
  ZCL_Cluster_t *commissioningCluster = ZCL_GetCluster(APP_ENDPOINT_COLOR_SCENE_CONTROLLER,
                                                       ZLL_COMMISSIONING_CLUSTER_ID,
                                                       ZCL_CLUSTER_SIDE_CLIENT);
  if(commissioningCluster)
    commissioningCluster->ZCL_DefaultRespInd = ZCL_CommandZclDefaultResp;

  cscCommissioningClusterServerAttributes.clusterVersion.value = CLUSTER_VERSION;
}
/**************************************************************************//**
\brief Initialize Commissioning cluster client.
******************************************************************************/
void cscCommissioningClientClusterInit(void)
{
  /**
  * Commissioning Client Cluster Of Color Scene Controller
  */
  cscCommissioningClusterClientAttributes.clusterVersion.value = CLUSTER_VERSION;
}

/**************************************************************************//**
\brief Callback on receive of Get Group Identifiers command
******************************************************************************/
static ZCL_Status_t getGroupIdentifiersInd(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_GetGroupIdentifiers_t *payload)
{
  ZCL_Request_t *req;
  uint8_t size;

  appSnprintf("getGroupIdentifiersInd(), startIndex=%02d\r\n", payload->startIndex);

  if (!(req = getFreeCommand()))
  {
   return ZCL_INSUFFICIENT_SPACE_STATUS;
  }

  size = fillGetGroupIdentifiersResponsePayload((ZCL_GetGroupIdentifiersResponse_t *)req->requestPayload,  payload->startIndex);
  fillCommandRequest(req, ZCL_COMMISSIONING_CLUSTER_GET_GROUP_IDENTIFIERS_RESPONSE_COMMAND_ID, size, APP_ENDPOINT_COLOR_SCENE_CONTROLLER);
  fillDstAddressingViaSourceAddressing(&req->dstAddressing, addressing, ZCL_CLUSTER_SIDE_CLIENT);
  ZCL_CommandManagerSendCommand(req);

  (void) payloadLength;
  return ZCL_SUCCESS_STATUS;
}

/**************************************************************************//**
\brief Callback on receive of Get Endpoint List command
******************************************************************************/
static ZCL_Status_t getEndpointListInd(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_GetEndpointList_t *payload)
{
  ZCL_Request_t *req;
  uint8_t size;

  appSnprintf("getEndpointListInd(), startIndex=%02d\r\n", payload->startIndex);

  if (!(req = getFreeCommand()))
  {
   return ZCL_INSUFFICIENT_SPACE_STATUS;
  }

  size = fillGetEndpointListResponsePayload((ZCL_GetEndpointListResponse_t *)req->requestPayload,  payload->startIndex);
  fillCommandRequest(req, ZCL_COMMISSIONING_CLUSTER_GET_ENDPOINT_LIST_RESPONSE_COMMAND_ID, size, APP_ENDPOINT_COLOR_SCENE_CONTROLLER);
  fillDstAddressingViaSourceAddressing(&req->dstAddressing, addressing, ZCL_CLUSTER_SIDE_CLIENT);
  ZCL_CommandManagerSendCommand(req);

  (void)payloadLength;
  return ZCL_SUCCESS_STATUS;
}

/**************************************************************************//**
\brief Callback on receive of Endpoint Information command
******************************************************************************/
static ZCL_Status_t endpointInformationResponseInd(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_EndpointInformation_t *payload)
{
  uint64_t tmpAddr;
  uint8_t *tmp;

  COPY_EXT_ADDR(tmpAddr, payload->ieeeAddress);
  tmp = (uint8_t *)&tmpAddr;

  appSnprintf("endpointInformationInd()\r\n");

  appSnprintf("ieeeAddress=");

  for (uint8_t i = 0; i < 8; i++)
  {
    appSnprintf("%02x", tmp[i]);
  }

  appSnprintf(", networkAddress=0x%04x, endpointId=0x%02x, profileId=0x%04x, deviceId=0x%04x, version=0x%02x", payload->networkAddress, payload->endpointId, payload->profileId, payload->deviceId, payload->version);

  (void)addressing;
  (void)payloadLength;
  (void)payload;
  (void)tmp;
  return ZCL_SUCCESS_STATUS;
}

/**************************************************************************//**
\brief Callback on receive of Get Group Identifiers Response command
******************************************************************************/
static ZCL_Status_t getGroupIdentifiersResponseInd(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_GetGroupIdentifiersResponse_t *payload)
{
  uint8_t i;
  ZCL_GroupInformationRecord_t record;

  appSnprintf("getGroupIdentifiersResponseInd()\r\n");

  appSnprintf("total=%d, startIndex=%d, count=%d\r\n", payload->total, payload->startIndex, payload->count);


  for (i = 0; i < payload->count; i++)
  {
    record = payload->recordList[i];
    appSnprintf("group id=0x%04x, type=0x%02x\r\n", record.groupId, record.groupType);
  }

  (void)addressing;
  (void)payloadLength;
  (void)payload;
  (void)record;
  return ZCL_SUCCESS_STATUS;
}

/**************************************************************************//**
\brief Callback on receive of Get Endpoint List Response command
******************************************************************************/
static ZCL_Status_t getEndpointListResponseInd(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_GetEndpointListResponse_t *payload)
{
  uint8_t i;
  ZCL_EndpointInformationRecord_t record;

  appSnprintf( "getEndpointListResponseInd()\r\n");

  appSnprintf("total=%d, startIndex=%d, count=%d\r\n", payload->total, payload->startIndex, payload->count);


  for (i = 0; i < payload->count; i++)
  {
    record = payload->recordList[i];
    appSnprintf("endpoint networkAddress=0x%04x, endpointId=0x%02x, profileId=0x%04x, deviceId=0x%04x, version=0x%02x\r\n", record.networkAddress, record.endpointId, record.profileId, record.deviceId, record.version);
  }

  (void)addressing;
  (void)payloadLength;
  (void)payload;
  (void)record;
  return ZCL_SUCCESS_STATUS;
}

#endif // APP_DEVICE_TYPE_COLOR_SCENE_CONTROLLER

// eof cscCommissioningCluster.c
