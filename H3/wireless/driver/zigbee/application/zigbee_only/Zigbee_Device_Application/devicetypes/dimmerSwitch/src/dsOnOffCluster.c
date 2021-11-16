/*******************************************************************************
  Dimmer Switch On/Off cluster Source File

  Company:
    Microchip Technology Inc.

  File Name:
   dsOnOffCluster.c

  Summary:
    This file contains the Dimmer Switch On/Off cluster interface.

  Description:
    This file contains the Dimmer Switch On/Off cluster interface.
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

#if (APP_Z3_DEVICE_TYPE == APP_DEVICE_TYPE_DIMMER_SWITCH)

/******************************************************************************
                    Includes section
******************************************************************************/
#include <haClusters.h>
#include <dsOnOffCluster.h>
#include <console.h>
#include <zclCommandManager.h>
#include <onOffCluster.h>

/******************************************************************************
                    Global variables section
******************************************************************************/
ZCL_OnOffClusterClientAttributes_t dsOnOffClusterClientAttributes =
{
  ZCL_DEFINE_ONOFF_CLUSTER_CLIENT_ATTRIBUTES()
};

/******************************************************************************
                    Prototypes section
******************************************************************************/
static void dsOnOffReportInd(ZCL_Addressing_t *addressing, uint8_t reportLength, uint8_t *reportPayload);

/******************************************************************************
                    Implementation section
******************************************************************************/
/**************************************************************************//**
\brief Initializes On/Off cluster
******************************************************************************/
void dsOnOffClusterInit(void)
{
  ZCL_Cluster_t *cluster = ZCL_GetCluster(APP_ENDPOINT_DIMMER_SWITCH, ONOFF_CLUSTER_ID, ZCL_CLUSTER_SIDE_CLIENT);
  cluster->ZCL_DefaultRespInd = ZCL_CommandZclDefaultResp;
  if (cluster)
    cluster->ZCL_ReportInd = dsOnOffReportInd;

  dsOnOffClusterClientAttributes.clusterVersion.value = CLUSTER_VERSION;
}

/**************************************************************************//**
\brief Report attribute indication handler

\param[in] addressing - pointer to addressing information;
\param[in] reportLength - data payload length;
\param[in] reportPayload - data pointer
******************************************************************************/
static void dsOnOffReportInd(ZCL_Addressing_t *addressing, uint8_t reportLength, uint8_t *reportPayload)
{
  ZCL_Report_t *rep = (ZCL_Report_t *)reportPayload;

  appSnprintf( "<-On/Off Attr Report: t = %d\r\n", (int)rep->value[0]);

  (void)addressing, (void)reportLength, (void)rep;
}

#endif //(APP_Z3_DEVICE_TYPE == APP_DEVICE_TYPE_DIMMER_SWITCH)

// eof dsOnOffCluster.c

