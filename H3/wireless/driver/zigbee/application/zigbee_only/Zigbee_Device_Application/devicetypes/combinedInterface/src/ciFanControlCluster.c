/*******************************************************************************
Combined Interface Fan Control Cluster Source File

  Company:
    Microchip Technology Inc.

  File Name:
   ciFanControlCluster.c

  Summary:
    This file contains the Combined Interface Fan Control Cluster Interface.

  Description:
    This file contains the Combined Interface Fan Control Cluster Interface.
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
#include <zigbee_device/common/include/appConsts.h>
#include <zigbee_device/devicetypes/combinedInterface/include/ciFanControlCluster.h>
#include <zcl/include/zclCommandManager.h>
#include <zigbee_device/clusters/include/haClusters.h>
#if APP_ENABLE_CONSOLE == 1
#include <app_zigbee/zigbee_console/console.h>
#endif

/******************************************************************************
                    Global variables
******************************************************************************/
ZCL_FanControlClusterClientAttributes_t ciFanControlClusterClientAttributes =
{
  ZCL_DEFINE_FAN_CONTROL_CLUSTER_CLIENT_ATTRIBUTES()
};

/******************************************************************************
                    Prototypes section
******************************************************************************/
static void ciFanControlAttrEventInd(ZCL_Addressing_t *addressing, ZCL_AttributeId_t attributeId, ZCL_AttributeEvent_t event);

/******************************************************************************
                    Implementation section
******************************************************************************/
/**************************************************************************//**
\brief Initializes fan control client cluster
******************************************************************************/
void ciFanControlClusterInit(void)
{
  ZCL_Cluster_t *cluster = ZCL_GetCluster(APP_ENDPOINT_COMBINED_INTERFACE, FAN_CONTROL_CLUSTER_ID, ZCL_CLUSTER_SIDE_CLIENT);
  ciFanControlClusterClientAttributes.clusterVersion.value = CLUSTER_VERSION;

  if (cluster)
  {
    cluster->ZCL_AttributeEventInd = ciFanControlAttrEventInd;
  }
  cluster->ZCL_DefaultRespInd = ZCL_CommandZclDefaultResp;
}
/**************************************************************************//**
\brief Attribute Event indication handler(to indicate when attr values have
        read or written)

\param[in] addressing - pointer to addressing information;
\param[in] reportLength - data payload length;
\param[in] reportPayload - data pointer
******************************************************************************/
static void ciFanControlAttrEventInd(ZCL_Addressing_t *addressing, ZCL_AttributeId_t attributeId, ZCL_AttributeEvent_t event)
{
#if APP_ENABLE_CONSOLE == 1
  LOG_STRING(AttrEventIndStr, "<-Attr ID 0x%x event 0x%x\r\n");
  appSnprintf(AttrEventIndStr, attributeId, event);
#else
  (void)attributeId;
  (void)event;
#endif
  (void)addressing;
}

#endif // APP_DEVICE_TYPE_COMBINED_INTERFACE

// eof ciFanControlCluster.c
