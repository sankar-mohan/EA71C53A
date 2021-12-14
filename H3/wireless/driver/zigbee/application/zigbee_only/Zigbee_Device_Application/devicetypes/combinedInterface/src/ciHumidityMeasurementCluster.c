/*******************************************************************************
 Combined Interface Humidity Measurement cluster Source File

  Company:
    Microchip Technology Inc.

  File Name:
   ciHumidityMeasurementCluster.c

  Summary:
    This file contains the Combined Interface Humidity Measurement cluster interface.

  Description:
    This file contains the Combined Interface Humidity Measurement cluster interface.
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
#include <zigbee_device/devicetypes/combinedInterface/include/ciHumidityMeasurementCluster.h>
#include <zcl/include/zclCommandManager.h>
#include <zigbee_device/clusters/include/haClusters.h>

#include <zcl/include/zclZllOnOffCluster.h>
#include <zcl/include/zclZllLevelControlCluster.h>
#include <app_zigbee/zigbee_console/console.h>

/******************************************************************************
//                      Global variables section
*****************************************************************************/
/******************************************************************************
                    Prototypes section
******************************************************************************/
static void ciHumidityMeasurementReportInd(ZCL_Addressing_t *addressing, uint8_t reportLength, uint8_t *reportPayload);

ZCL_HumidityMeasurementClusterClientAttributes_t ciHumidityMeasurementClusterClientAttributes =
{
  ZCL_DEFINE_HUMIDITY_MEASUREMENT_CLUSTER_CLIENT_ATTRIBUTES()
};

/******************************************************************************
                    Implementation section
******************************************************************************/
/**************************************************************************//**
\brief Initializes Humidity Measurement cluster
******************************************************************************/
void ciHumidityMeasurementClusterInit(void)
{
  ZCL_Cluster_t *cluster = ZCL_GetCluster(APP_ENDPOINT_COMBINED_INTERFACE, HUMIDITY_MEASUREMENT_CLUSTER_ID, ZCL_CLUSTER_SIDE_CLIENT);
  ciHumidityMeasurementClusterClientAttributes.clusterVersion.value = CLUSTER_VERSION;
  if (cluster)
    cluster->ZCL_ReportInd = ciHumidityMeasurementReportInd;
  cluster->ZCL_DefaultRespInd = ZCL_CommandZclDefaultResp;
}
/**************************************************************************//**
\brief Report attribute indication handler

\param[in] addressing - pointer to addressing information;
\param[in] reportLength - data payload length;
\param[in] reportPayload - data pointer
******************************************************************************/
static void ciHumidityMeasurementReportInd(ZCL_Addressing_t *addressing, uint8_t reportLength, uint8_t *reportPayload)
{
  ZCL_Report_t *rep = (ZCL_Report_t *)reportPayload;
  uint16_t reportValue;

  memcpy(&reportValue, &rep->value[0], sizeof(uint16_t));
  appSnprintf("<-Humidity Measurement Attr Report: t = %u\r\n", reportValue);

  (void)addressing, (void)reportLength, (void)rep;
}

#endif // APP_DEVICE_TYPE_COMBINED_INTERFACE

// eof ciHumidityMeasurementCluster.c