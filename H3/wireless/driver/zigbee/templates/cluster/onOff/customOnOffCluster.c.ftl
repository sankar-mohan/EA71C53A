/*******************************************************************************
  Custom OnOff Cluster Source file

  Company:
    Microchip Technology Inc.

  File Name:
    customOnOffCluster.c

  Summary:
    This file contains Custom OnOff Cluster Implementation.

  Description:
    This file contains Custom OnOff Cluster Implementation.
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

#if (APP_Z3_DEVICE_TYPE == APP_DEVICE_TYPE_CUSTOM_DEVICE)

/******************************************************************************
                    Includes section
******************************************************************************/
#if defined(BOARD_MEGARF) || defined(BOARD_SAMR21)
#include <ledsExt.h>
#include <lcdExt.h>
#endif
#include <zigbee_device/clusters/include/haClusters.h>
#include <zigbee_device/devicetypes/custom/include/customLevelControlCluster.h>
#include <zigbee_device/devicetypes/custom/include/customScenesCluster.h>
#include <zigbee_device/common/include/z3Device.h>
#include <pds/include/wlPdsMemIds.h>
#include <zigbee_device/clusters/include/scenesCluster.h>
#include <zigbee_device/devicetypes/custom/include/customOnOffCluster.h>
#include <zigbee_device/devicetypes/custom/include/custom.h>
#include <zcl/include/zclCommandManager.h>
#include <app_zigbee/zigbee_console/console.h>
#include <app_zigbee/app_zigbee_handler.h>

<#if (ONOFF_CLUSTER_CS != "CLIENT")>
/******************************************************************************
                    Definitions section
******************************************************************************/
#define UPDATE_TIMER_INTERVAL               100 // 1/10th of a second as per specification
#define FADE_TIMER_INTERVAL                 UPDATE_TIMER_INTERVAL
#define MAX_BRIGHTNESS_LEVEL                255
#define MIN_BRIGHTNESS_LEVEL                (0)
                    
/******************************************************************************
                    Types section
******************************************************************************/
typedef struct
{

  uint8_t        firstInterval;
  uint8_t        secondInterval;
  uint8_t        currentLevel;
  uint8_t        transitionTime;
  int16_t        delta;
  HAL_AppTimer_t timer;
} FadeMem_t;

/******************************************************************************
                    Prototypes section
******************************************************************************/
static ZCL_Status_t onInd(ZCL_Addressing_t *addressing, uint8_t payloadLength, uint8_t *payload);
static ZCL_Status_t offInd(ZCL_Addressing_t *addressing, uint8_t payloadLength, uint8_t *payload);
static ZCL_Status_t toggleInd(ZCL_Addressing_t *addressing, uint8_t payloadLength, uint8_t *payload);
static ZCL_Status_t offWithEffectInd(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_OffWithEffect_t *payload);
static ZCL_Status_t onWithRecallGlobalSceneInd(ZCL_Addressing_t *addressing, uint8_t payloadLength, uint8_t *payload);
static ZCL_Status_t onWithTimedOffInd(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_OnWithTimedOff_t *payload);

static void startDeviceUpdate(void);
static void stopDeviceUpdate(void);
static void displayStatus(void);
static void updateOnOffState(void);
static void fadeTimerFired(void);
static void processOffEffect(uint8_t effectId, uint8_t effectVariant);

static void ZCL_OnOffAttributeEventInd(ZCL_Addressing_t *addressing, ZCL_AttributeId_t attributeId, ZCL_AttributeEvent_t event);
</#if>
<#if (ONOFF_CLUSTER_CS != "SERVER")>
static void customOnOffReportInd(ZCL_Addressing_t *addressing, uint8_t reportLength, uint8_t *reportPayload);
</#if>
/******************************************************************************
                    Global variables
******************************************************************************/
<#if (ONOFF_CLUSTER_CS != "CLIENT")>
PROGMEM_DECLARE(ZCL_OnOffClusterCommands_t customOnOffCommands) =
{
  ZCL_DEFINE_ONOFF_CLUSTER_COMMANDS(onInd, offInd, toggleInd, offWithEffectInd, onWithRecallGlobalSceneInd, onWithTimedOffInd)
};

ZCL_OnOffClusterServerAttributes_t customOnOffClusterServerAttributes =
{
  ZCL_DEFINE_ONOFF_CLUSTER_SERVER_ATTRIBUTES(ONOFF_VAL_MIN_REPORT_PERIOD, ONOFF_VAL_MAX_REPORT_PERIOD)
};
<#else>
PROGMEM_DECLARE(ZCL_OnOffClusterCommands_t customOnOffCommands) =
{
  ZCL_DEFINE_ONOFF_CLUSTER_COMMANDS(NULL, NULL, NULL, NULL, NULL, NULL)
};
</#if>

<#if (ONOFF_CLUSTER_CS != "SERVER")>
ZCL_OnOffClusterClientAttributes_t customOnOffClusterClientAttributes =
{
  ZCL_DEFINE_ONOFF_CLUSTER_CLIENT_ATTRIBUTES()
};
</#if>
/******************************************************************************
                    Local variables
******************************************************************************/
<#if (ONOFF_CLUSTER_CS != "CLIENT")>
static HAL_AppTimer_t updateTimer;
static FadeMem_t fadeMem;
</#if>
/******************************************************************************
                    Implementations section
******************************************************************************/

/**************************************************************************//**
\brief Initialize On/Off cluster.
******************************************************************************/
void customOnOffClusterInit(void)
{
<#if (ONOFF_CLUSTER_CS != "CLIENT")>
  ZCL_Cluster_t *cluster = ZCL_GetCluster(APP_ENDPOINT_CUSTOM, ONOFF_CLUSTER_ID, ZCL_CLUSTER_SIDE_SERVER);
</#if>
<#if (ONOFF_CLUSTER_CS != "SERVER")>
  ZCL_Cluster_t *clusterClient = ZCL_GetCluster(APP_ENDPOINT_CUSTOM, ONOFF_CLUSTER_ID, ZCL_CLUSTER_SIDE_CLIENT);
</#if>

<#if (ONOFF_CLUSTER_CS != "CLIENT")>
  if (cluster)
    cluster->ZCL_AttributeEventInd = ZCL_OnOffAttributeEventInd;

  if (!PDS_IsAbleToRestore(APP_CUSTOM_ONOFF_MEM_ID))
  {
<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_ONOFF == true) >
    customOnOffClusterServerAttributes.onOff.value = false;
</#if>
<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_GLOBALSCENECONTROL == true) >
    customOnOffClusterServerAttributes.globalSceneControl.value = true;
</#if>
<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_ONTIME == true) >
    customOnOffClusterServerAttributes.onTime.value = 0;
</#if>
<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_OFFWAITTIME == true) >
    customOnOffClusterServerAttributes.offWaitTime.value = 0;
</#if>
<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_CLUSTERREVISION == true) >
    customOnOffClusterServerAttributes.clusterVersion.value = CLUSTER_VERSION;
</#if>
#if ZLO_CLUSTER_ENHANCEMENTS == 1
<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_STARTUPONOFF == true) >
    customOnOffClusterServerAttributes.startUpOnOff.value = ZCL_ON_OFF_CLUSTER_STARTUP_PREVIOUS;
</#if>
#endif
  }
  else
  {
    PDS_Restore(APP_CUSTOM_ONOFF_MEM_ID);
  }

  updateTimer.mode = TIMER_REPEAT_MODE;
  updateTimer.interval = UPDATE_TIMER_INTERVAL;
  updateTimer.callback = updateOnOffState;

  fadeMem.timer.mode     = TIMER_REPEAT_MODE;
  fadeMem.timer.interval = FADE_TIMER_INTERVAL;
  fadeMem.timer.callback = fadeTimerFired;

<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_STARTUPONOFF == true) >
#if ZLO_CLUSTER_ENHANCEMENTS == 1
  customUpdateStartUpOnOffState();
#endif  
</#if>
</#if>

<#if (ONOFF_CLUSTER_CS != "SERVER")  && (OCC_CLUSTERREVISION == true) >
  customOnOffClusterClientAttributes.clusterVersion.value = CLUSTER_VERSION;
</#if>
<#if (ONOFF_CLUSTER_CS != "SERVER")>
  if (clusterClient)
  {
    clusterClient->ZCL_DefaultRespInd = ZCL_CommandZclDefaultResp;
    clusterClient->ZCL_ReportInd = customOnOffReportInd;
  }
</#if>
<#if (ONOFF_CLUSTER_CS != "CLIENT")>
  displayStatus();
</#if>
}

<#if (ONOFF_CLUSTER_CS != "CLIENT")>
/**************************************************************************//**
\brief Set Extension Field Set
******************************************************************************/
void onOffClusterSetExtensionField(bool onOff)
{
  if (customOnOffClusterServerAttributes.onOff.value != onOff)
  {
    if (!onOff)
    {
<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_ONTIME == true) >
      customOnOffClusterServerAttributes.onTime.value = 0;
</#if>
    }
    else
    {
<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_OFFWAITTIME == true) >
      customOnOffClusterServerAttributes.offWaitTime.value = 0;
</#if>
      stopDeviceUpdate();
    }
<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_ONOFF == true) >
    customOnOffClusterServerAttributes.onOff.value = onOff;
</#if>
    displayStatus();
#ifdef _ZCL_REPORTING_SUPPORT_
    ZCL_ReportOnChangeIfNeeded(&customOnOffClusterServerAttributes.onOff);
#endif
    PDS_Store(APP_CUSTOM_ONOFF_MEM_ID);
  }
}

<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_GLOBALSCENECONTROL == true) >
/**************************************************************************//**
\brief Set Global Scene Control attribute to 'true'
******************************************************************************/
void onOffClusterSetGlobalSceneControl(void)
{
  customOnOffClusterServerAttributes.globalSceneControl.value = true;
}
</#if>

/**************************************************************************//**
\brief Checks, if device is in On state

\return True, if device is in On state; false - otherwise.
******************************************************************************/
bool deviceIsOn(void)
{
  return customOnOffClusterServerAttributes.onOff.value;
}

/**************************************************************************//**
\brief Start device update procedure
******************************************************************************/
static void startDeviceUpdate(void)
{
  HAL_StopAppTimer(&updateTimer);
  HAL_StartAppTimer(&updateTimer);
}

/**************************************************************************//**
\brief Stop device update procedure
******************************************************************************/
static void stopDeviceUpdate(void)
{
  HAL_StopAppTimer(&updateTimer);
}

/**************************************************************************//**
\brief Set Extension Field Set
******************************************************************************/
void setOnOff(bool onOff)
{
  customOnOffClusterServerAttributes.onOff.value = onOff;
<#if (SCENES_CLUSTER_ENABLE == true) && (SCENES_CLUSTER_CS != "CLIENT")>
  customScenesInvalidate();
</#if>
  displayStatus();

  if (onOff)
  {
<#if (LEVELCONTROL_CLUSTER_ENABLE == true) && (LEVELCONTROL_CLUSTER_ENABLE == true) && (LEVELCONTROL_CLUSTER_CS != "CLIENT")>
    LEDS_SET_BRIGHTNESS(customLevelControlClusterServerAttributes.currentLevel.value);
</#if>
#if APP_Z3_DEVICE_TYPE == APP_DEVICE_TYPE_DIMMABLE_LIGHT
    LEDS_SET_COLOR_HS(DIMMABLE_LIGHT_HUE_LEVEL,DIMMABLE_LIGHT_SATURATION_LEVEL);
#endif
  }
  else
  {
    LEDS_SET_BRIGHTNESS(MIN_BRIGHTNESS_LEVEL);
  }
#ifdef _ZCL_REPORTING_SUPPORT_
  ZCL_ReportOnChangeIfNeeded(&customOnOffClusterServerAttributes.onOff);
#endif
  PDS_Store(APP_CUSTOM_ONOFF_MEM_ID);
}

/**************************************************************************//**
\brief Display current On/Off status.
******************************************************************************/
static void displayStatus(void)
{
  LCD_PRINT(0, 0, customOnOffClusterServerAttributes.onOff.value ? "I" : "O");
}

/**************************************************************************//**
\brief On/Off cluster update timer callback
******************************************************************************/
static void updateOnOffState(void)
{
  if (customOnOffClusterServerAttributes.onOff.value)
  {
<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_ONTIME == true) >
    if (customOnOffClusterServerAttributes.onTime.value)
    {
      customOnOffClusterServerAttributes.onTime.value--;
    }
    else
    {
      customOnOffClusterServerAttributes.offWaitTime.value = 0;
      setOnOff(false);
    }
</#if>
  }
  else
  {
<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_OFFWAITTIME == true) >
    if (customOnOffClusterServerAttributes.offWaitTime.value)
    {
      customOnOffClusterServerAttributes.offWaitTime.value--;
    }
    else
    {
      stopDeviceUpdate();
    }
</#if>
  }
}

/**************************************************************************//**
\brief Fade timer has fired
******************************************************************************/
static void fadeTimerFired(void)
{
  if (fadeMem.firstInterval)
  {
    fadeMem.firstInterval--;

    int32_t remainingTime = fadeMem.firstInterval;
    int16_t targetLevel = MIN((uint16_t)fadeMem.currentLevel + fadeMem.delta, UINT8_MAX);
    int16_t newLevel    = targetLevel - (fadeMem.delta * remainingTime) / (int32_t)fadeMem.transitionTime;

    LEDS_SET_BRIGHTNESS(newLevel);
    (void)newLevel;
  }
  else if(fadeMem.secondInterval)
  {
    fadeMem.firstInterval  = fadeMem.secondInterval;
    fadeMem.transitionTime = fadeMem.secondInterval;
    fadeMem.secondInterval = 0;
    fadeMem.currentLevel   = MIN((uint16_t)fadeMem.currentLevel + fadeMem.delta, UINT8_MAX);
    fadeMem.delta          = -fadeMem.currentLevel;
    fadeTimerFired();
  }
  else
    HAL_StopAppTimer(&fadeMem.timer);
}

<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_OFFWITHEFFECT == true) >
/**************************************************************************//**
\brief Processes efefect of OffWithEffect command

\param[in] effectId      - effect identifier;
\param[in] effectVariant - effect variant
******************************************************************************/
static void processOffEffect(uint8_t effectId, uint8_t effectVariant)
{
<#if (LEVELCONTROL_CLUSTER_ENABLE == true) && (LEVELCONTROL_CLUSTER_CS != "CLIENT")>
  fadeMem.currentLevel = customLevelControlClusterServerAttributes.currentLevel.value;
<#else>
  fadeMem.currentLevel = 0;
</#if>
  if (0 == effectId)
  {
    if (0 == effectVariant)
    {
      fadeMem.firstInterval  = 8;
      fadeMem.secondInterval = 0;
      fadeMem.delta          = -fadeMem.currentLevel;
    }
    if (1 == effectVariant)
    {
      LEDS_SET_BRIGHTNESS(0);
      return;
    }
    if (2 == effectVariant)
    {
      fadeMem.firstInterval  = 8;
      fadeMem.secondInterval = 120;
      fadeMem.delta          = -fadeMem.currentLevel / 2;
    }
  }
  if (1 == effectId)
    if (0 == effectVariant)
    {
      fadeMem.firstInterval  = 5;
      fadeMem.secondInterval = 10;
      fadeMem.delta          = ((uint16_t)fadeMem.currentLevel * 2) / 10;
    }

  if ((signed)UINT8_MAX < fadeMem.delta + fadeMem.currentLevel)
    fadeMem.delta = UINT8_MAX - fadeMem.currentLevel;
  fadeMem.transitionTime = fadeMem.firstInterval;
  HAL_StopAppTimer(&fadeMem.timer);
  HAL_StartAppTimer(&fadeMem.timer);
}
</#if>

<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_ON == true) >
/**************************************************************************//**
\brief Callback on receive of On command
******************************************************************************/
static ZCL_Status_t onInd(ZCL_Addressing_t *addressing, uint8_t payloadLength, uint8_t *payload)
{
  APP_Zigbee_Event_t event;
  event.eventGroup = EVENT_CLUSTER;
  event.eventId = CMD_ZCL_ON;
  event.eventData.zclEventData.addressing = addressing;
  event.eventData.zclEventData.payloadLength = payloadLength;
  event.eventData.zclEventData.payload = (uint8_t *)payload;

<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_ONTIME == true) >
  if (0 == customOnOffClusterServerAttributes.onTime.value)
  {
<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_OFFWAITTIME == true) >
    customOnOffClusterServerAttributes.offWaitTime.value = 0;
</#if>
    stopDeviceUpdate();
  }
</#if>
<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_GLOBALSCENECONTROL == true) >
  onOffClusterSetGlobalSceneControl();
</#if>
<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_ONOFF == true) >
  setOnOff(true);
</#if>

  APP_Zigbee_Handler(event);
  return ZCL_SUCCESS_STATUS;
}
</#if>
<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_OFF == true) >
/**************************************************************************//**
\brief Callback on receive of Off command
******************************************************************************/
static ZCL_Status_t offInd(ZCL_Addressing_t *addressing, uint8_t payloadLength, uint8_t *payload)
{
  APP_Zigbee_Event_t event;
  event.eventGroup = EVENT_CLUSTER;
  event.eventId = CMD_ZCL_OFF;
  event.eventData.zclEventData.addressing = addressing;
  event.eventData.zclEventData.payloadLength = payloadLength;
  event.eventData.zclEventData.payload = (uint8_t *)payload;
<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_ONTIME == true) >
  customOnOffClusterServerAttributes.onTime.value = 0;
</#if>
<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_ONOFF == true) >
  setOnOff(false);
</#if>
<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_ONTIME == true) >
  customOnOffClusterServerAttributes.onTime.value = 0;
</#if>

  APP_Zigbee_Handler(event);
  return ZCL_SUCCESS_STATUS;
}
</#if>
<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_TOGGLE == true) >
/**************************************************************************//**
\brief Callback on receive of Toggle command
******************************************************************************/
static ZCL_Status_t toggleInd(ZCL_Addressing_t *addressing, uint8_t payloadLength, uint8_t *payload)
{
  APP_Zigbee_Event_t event;
  event.eventGroup = EVENT_CLUSTER;
  event.eventId = CMD_ZCL_TOGGLE;
  event.eventData.zclEventData.addressing = addressing;
  event.eventData.zclEventData.payloadLength = payloadLength;
  event.eventData.zclEventData.payload = (uint8_t *)payload;

  if (false == customOnOffClusterServerAttributes.onOff.value)
  {
<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_ONTIME == true) >
    if (0 == customOnOffClusterServerAttributes.onTime.value)
<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_OFFWAITTIME == true) >
      customOnOffClusterServerAttributes.offWaitTime.value = 0;
</#if>
</#if>

    setOnOff(true);
<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_GLOBALSCENECONTROL == true) >
    onOffClusterSetGlobalSceneControl();
</#if>
  }
  else
  {
<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_ONTIME == true) >
    customOnOffClusterServerAttributes.onTime.value = 0;
</#if>
    setOnOff(false);
  }

  APP_Zigbee_Handler(event);
  return ZCL_SUCCESS_STATUS;
}
</#if>
<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_OFFWITHEFFECT == true) >
/**************************************************************************//**
\brief Callback on receive of Off With Effect command
******************************************************************************/
static ZCL_Status_t offWithEffectInd(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_OffWithEffect_t *payload)
{
  uint8_t previousOnOffState = customOnOffClusterServerAttributes.onOff.value;
  APP_Zigbee_Event_t event;
  event.eventGroup = EVENT_CLUSTER;
  event.eventId = CMD_ZCL_OFF_WITH_EFFECT;
  event.eventData.zclEventData.addressing = addressing;
  event.eventData.zclEventData.payloadLength = payloadLength;
  event.eventData.zclEventData.payload = (uint8_t *)payload;

<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_GLOBALSCENECONTROL == true) >
  if (customOnOffClusterServerAttributes.globalSceneControl.value)
  {
    customOnOffClusterServerAttributes.globalSceneControl.value = false;
<#if (SCENES_CLUSTER_ENABLE == true) && (SCENES_CLUSTER_CS != "CLIENT")>
    storeGlobalScene(APP_ENDPOINT_CUSTOM, customSceneTable, &customScenesClusterServerAttributes);
</#if>
  }
</#if>
  customOnOffClusterServerAttributes.onTime.value = 0;
  customOnOffClusterServerAttributes.onOff.value = false;
<#if (SCENES_CLUSTER_ENABLE == true) && (SCENES_CLUSTER_CS != "CLIENT")>
  customScenesInvalidate();
</#if>
  PDS_Store(Z3DEVICE_APP_MEMORY_MEM_ID);
  if (previousOnOffState)
  {/* Device was ON when receiving this command */
    displayStatus();
    processOffEffect(payload->effectIdentifier, payload->effectVariant);
  }
#ifdef _ZCL_REPORTING_SUPPORT_
  ZCL_ReportOnChangeIfNeeded(&customOnOffClusterServerAttributes.onOff);
#endif

  APP_Zigbee_Handler(event);
  return ZCL_SUCCESS_STATUS;
}
</#if>
<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_ONWITHRECALL == true) >
/**************************************************************************//**
\brief Callback on receive of On With Recall Global Scene command
******************************************************************************/
static ZCL_Status_t onWithRecallGlobalSceneInd(ZCL_Addressing_t *addressing, uint8_t payloadLength, uint8_t *payload)
{
  APP_Zigbee_Event_t event;
  event.eventGroup = EVENT_CLUSTER;
  event.eventId = CMD_ZCL_ON_WITH_RECALL_GLOBALSCENE;
  event.eventData.zclEventData.addressing = addressing;
  event.eventData.zclEventData.payloadLength = payloadLength;
  event.eventData.zclEventData.payload = (uint8_t *)payload;

  setOnOff(true);
<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_ONTIME == true) >
  customOnOffClusterServerAttributes.onTime.value = 0;
</#if>
<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_OFFWAITTIME == true) >
  customOnOffClusterServerAttributes.offWaitTime.value = 0;
</#if>
  stopDeviceUpdate();
<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_GLOBALSCENECONTROL == true) >
  if (false == customOnOffClusterServerAttributes.globalSceneControl.value)
  {
<#if (SCENES_CLUSTER_ENABLE == true) && (SCENES_CLUSTER_CS != "CLIENT")>
    recallGlobalScene(APP_ENDPOINT_CUSTOM, customSceneTable, &customScenesClusterServerAttributes);
</#if>
    customOnOffClusterServerAttributes.globalSceneControl.value = true;
<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_ONTIME == true) >
    if (0 == customOnOffClusterServerAttributes.onTime.value)
<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_OFFWAITTIME == true) >
      customOnOffClusterServerAttributes.offWaitTime.value = 0;
</#if>
</#if>
  }
</#if>
  APP_Zigbee_Handler(event);
  return ZCL_SUCCESS_STATUS;
}
</#if>
<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_ONWITHTIMEDOFF == true) >
/**************************************************************************//**
\brief Callback on receive of Toggle command
******************************************************************************/
static ZCL_Status_t onWithTimedOffInd(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_OnWithTimedOff_t *payload)
{
  APP_Zigbee_Event_t event;
  event.eventGroup = EVENT_CLUSTER;
  event.eventId = CMD_ZCL_ON_WITH_TIMED_OFF;
  event.eventData.zclEventData.addressing = addressing;
  event.eventData.zclEventData.payloadLength = payloadLength;
  event.eventData.zclEventData.payload = (uint8_t *)payload;

  if (payload->onOffControl & ZCL_ON_OFF_CLUSTER_ACCEPT_ONLY_WHEN_ON)
  {
    if (false == customOnOffClusterServerAttributes.onOff.value)
      return ZCL_SUCCESS_STATUS;
  }

<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_OFFWAITTIME == true) && (OC_ONTIME == true)>
  if (customOnOffClusterServerAttributes.offWaitTime.value > 0 &&
      false == customOnOffClusterServerAttributes.onOff.value)
  {
    customOnOffClusterServerAttributes.offWaitTime.value = MIN(payload->offWaitTime,
        customOnOffClusterServerAttributes.offWaitTime.value);
  }
  else
  {
    customOnOffClusterServerAttributes.onTime.value = MAX(payload->onTime,
        customOnOffClusterServerAttributes.onTime.value);

    customOnOffClusterServerAttributes.offWaitTime.value = payload->offWaitTime;

    setOnOff(true);
  }

  if (customOnOffClusterServerAttributes.onTime.value < 0xffff &&
      customOnOffClusterServerAttributes.offWaitTime.value < 0xffff)
  {
    startDeviceUpdate();
  }
</#if>

  APP_Zigbee_Handler(event);
  return ZCL_SUCCESS_STATUS;
}
</#if>
/**************************************************************************//**
\brief gets on/off state

\return on/off state
******************************************************************************/
bool onOffState(void)
{
  return customOnOffClusterServerAttributes.onOff.value;
}


<#if (ONOFF_CLUSTER_CS != "CLIENT")  && (OC_STARTUPONOFF == true) >
#if ZLO_CLUSTER_ENHANCEMENTS == 1
/**************************************************************************//**
\brief Update device ON state as per StartUpOnOff attribute value
******************************************************************************/
void customUpdateStartUpOnOffState(void)
{
  switch (customOnOffClusterServerAttributes.startUpOnOff.value)
  {
    case ZCL_ON_OFF_CLUSTER_STARTUP_OFF:
      customOnOffClusterServerAttributes.onOff.value = false;
      break;

    case ZCL_ON_OFF_CLUSTER_STARTUP_ON:
      customOnOffClusterServerAttributes.onOff.value = true;
      break;

    case ZCL_ON_OFF_CLUSTER_STARTUP_TOGGLE:
      customOnOffClusterServerAttributes.onOff.value = !customOnOffClusterServerAttributes.onOff.value;
      break;

    case ZCL_ON_OFF_CLUSTER_STARTUP_PREVIOUS:
      ; // customOnOffClusterServerAttributes.onOff.value restored from PDS is used
      break;

    default:
      break;
  }

  PDS_Store(APP_CUSTOM_ONOFF_MEM_ID);
}
#endif
</#if>


/**************************************************************************//**
\brief Attribute event (writing/reading) callback.

\param[in] addressing - incoming request addressing information.
\param[in] attributeId - attribute identifier.
\param[in] event - attribute event (read/write).
******************************************************************************/
static void ZCL_OnOffAttributeEventInd(ZCL_Addressing_t *addressing, ZCL_AttributeId_t attributeId, ZCL_AttributeEvent_t event)
{
  APP_Zigbee_Event_t eventItem;
  eventItem.eventGroup = EVENT_CLUSTER;
  eventItem.eventId = CMD_ZCL_ATTR_ONOFF;
  eventItem.eventData.zclAttributeData.addressing = addressing;
  eventItem.eventData.zclAttributeData.attributeId = attributeId;
  eventItem.eventData.zclAttributeData.event = event;
  APP_Zigbee_Handler(eventItem);

#if ZLO_CLUSTER_ENHANCEMENTS == 1
  if (((ZCL_WRITE_ATTRIBUTE_EVENT == event) && \
      ((ZCL_ONOFF_CLUSTER_STARTUP_ONOFF_SERVER_ATTRIBUTE_ID == attributeId) || \
       (ZCL_ONOFF_CLUSTER_ONOFF_SERVER_ATTRIBUTE_ID == attributeId))) || \
       (((ZCL_CONFIGURE_ATTRIBUTE_REPORTING_EVENT == event) || \
       (ZCL_CONFIGURE_DEFAULT_ATTRIBUTE_REPORTING_EVENT == event)) && \
       (ZCL_ONOFF_CLUSTER_ONOFF_SERVER_ATTRIBUTE_ID == attributeId))
      )
#else
  if (((ZCL_WRITE_ATTRIBUTE_EVENT == event) && \
       (ZCL_ONOFF_CLUSTER_ONOFF_SERVER_ATTRIBUTE_ID == attributeId)) || \
       (((ZCL_CONFIGURE_ATTRIBUTE_REPORTING_EVENT == event) || \
       (ZCL_CONFIGURE_DEFAULT_ATTRIBUTE_REPORTING_EVENT == event)) && \
       (ZCL_ONOFF_CLUSTER_ONOFF_SERVER_ATTRIBUTE_ID == attributeId))
      )	  
#endif	  
  {
    PDS_Store(APP_CUSTOM_ONOFF_MEM_ID);
  }

}
<#else>
bool deviceIsOn(void)
{
  return false;
}
</#if>
<#if (ONOFF_CLUSTER_CS != "SERVER")>
/**************************************************************************//**
\brief Report attribute indication handler

\param[in] addressing - pointer to addressing information;
\param[in] reportLength - data payload length;
\param[in] reportPayload - data pointer
******************************************************************************/
static void customOnOffReportInd(ZCL_Addressing_t *addressing, uint8_t reportLength, uint8_t *reportPayload)
{
  APP_Zigbee_Event_t eventItem;
  eventItem.eventGroup = EVENT_CLUSTER;
  eventItem.eventId = CMD_ZCL_REPORTING_ONOFF;
  eventItem.eventData.zclEventData.addressing = addressing;
  eventItem.eventData.zclEventData.payloadLength = reportLength;
  eventItem.eventData.zclEventData.payload = reportPayload;

  APP_Zigbee_Handler(eventItem);
}
</#if>
#endif // APP_Z3_DEVICE_TYPE == APP_DEVICE_TYPE_CUSTOM_DEVICE

// eof customOnOffCluster.c
