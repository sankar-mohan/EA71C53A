/*******************************************************************************
  Color Scene Controller Source File

  Company:
    Microchip Technology Inc.

  File Name:
    colorSceneController.c

  Summary:
    This file contains the Color Scene Controller device implementation.

  Description:
    This file contains the Color Scene Controller device implementation.
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
#include <zigbee_device/common/include/z3Device.h>

#ifdef BOARD_MEGARF
  #include <buttonsExt.h>
  #include <lcdExt.h>
#endif

#if BSP_SUPPORT == BOARD_SAMR21_ZLLEK
  #include <lcdExt.h>
  #include <joyStick.h>
#endif

#if defined(BOARD_MEGARF) || defined(BOARD_SAMR21)
  #include <ledsExt.h>
#endif

#include <stdlib.h>
#include <pds/include/wlPdsMemIds.h>
#include <zigbee_device/common/include/otauService.h>
#include <hal/include/sleep.h>
#include <hal/cortexm4/pic32cx/include/halSleep.h>
#include <zcl/include/zclCommandManager.h>
#include <zigbee_device/devicetypes/colorSceneController/include/cscClusters.h>
#include <zigbee_device/devicetypes/colorSceneController/include/cscBasicCluster.h>
#include <zigbee_device/devicetypes/colorSceneController/include/cscOnOffCluster.h>
#include <zigbee_device/devicetypes/colorSceneController/include/cscGroupsCluster.h>
#include <zigbee_device/devicetypes/colorSceneController/include/cscIdentifyCluster.h>
#include <zigbee_device/devicetypes/colorSceneController/include/cscScenesCluster.h>
#include <zigbee_device/devicetypes/colorSceneController/include/cscLevelControlCluster.h>
#include <zigbee_device/devicetypes/colorSceneController/include/cscColorControlCluster.h>
#include <zigbee_device/devicetypes/colorSceneController/include/cscCommissioningCluster.h>
#include <zigbee_device/devicetypes/colorSceneController/include/buttonHandlers.h>
#include <zigbee_device/clusters/include/scenesCluster.h>
#include <zigbee_device/clusters/include/basicCluster.h>
#include <zcl/clusters/include/identifyCluster.h>
#include <zcl/clusters/include/groupsCluster.h>
#include <systemenvironment/include/sysIdleHandler.h>
#if APP_ENABLE_CONSOLE == 1
  #include <app_zigbee/zigbee_console/console.h>
#endif // APP_ENABLE_CONSOLE == 1

#if ((QTOUCH_SUPPORT == 1) && (BSP_SUPPORT == BOARD_SAMR21_ZLLEK))
#include <qTouch.h>
#include <zigbee_device/clusters/include/levelControlCluster.h>
#include <halSleepTimerClock.h>
#endif

/******************************************************************************
                    Definitions section
******************************************************************************/
#if BSP_SUPPORT == BOARD_SAMR21_ZLLEK
  #define COLOR_X_RED             40000
  #define COLOR_Y_RED             20000
  #define COLOR_X_GREEN           10000
  #define COLOR_Y_GREEN           40000
  #define COLOR_X_YELLOW          30000
  #define COLOR_Y_YELLOW          30000
  #define COLOR_X_BLUE            10000
  #define COLOR_Y_BLUE            10000
#endif

/******************************************************************************
                    Prototypes section
******************************************************************************/
/*******************************************************************************
                    Static functions section
*******************************************************************************/
#if BSP_SUPPORT == BOARD_RCB_KEY_REMOTE
  static void appButtonsInd(BSP_ButtonEvent_t event, BSP_ButtonId_t button, uint8_t data);
#elif BSP_SUPPORT == BOARD_SAMR21_ZLLEK
  static void appJoyStickInd(BSP_JoystickState_t state);
  #if (QTOUCH_SUPPORT == 1)
    static void appTouchInd( BSP_TouchEvent_t event, BSP_ButtonId_t button, uint8_t data);
  #endif
#endif

#if defined (_SLEEP_WHEN_IDLE_)
static void sleepEventHandler(SYS_EventId_t eventId, SYS_EventData_t data);
#endif 

#if defined (_SLEEP_WHEN_IDLE_) && (APP_ENABLE_CONSOLE == 1)
  static void sleepModeHandler(SYS_EventId_t eventId, SYS_EventData_t data);
#endif
static void isBusyOrPollCheck(SYS_EventId_t eventId, SYS_EventData_t data);
#if BSP_SUPPORT == BOARD_SAMR21_ZLLEK
  static void performTouchlink (void);
  static BDB_InvokeCommissioningReq_t AppbdbCommissioningreq;
  static void touchlinkCallback(BDB_InvokeCommissioningConf_t *conf);
  static BDB_InitiatorReq_t AppinitiatorReq;
#endif
#ifdef OTAU_CLIENT
static void cscAddOTAUClientCluster(void);
#endif

/******************************************************************************
                    Local variables
******************************************************************************/

#if BSP_SUPPORT == BOARD_RCB_KEY_REMOTE
  static uint8_t altFunc = 0;
#endif // BSP_SUPPORT == BOARD_RCB_KEY_REMOTE

/******************************************************************************
                    Local variables section
******************************************************************************/
static ZCL_DeviceEndpoint_t cscEndpoint =
{
  .simpleDescriptor =
  {
    .endpoint            = APP_ENDPOINT_COLOR_SCENE_CONTROLLER,
    .AppProfileId        = APP_PROFILE_ID,
    .AppDeviceId         = APP_Z3DEVICE_ID,
    .AppDeviceVersion    = APP_VERSION,
    .AppInClustersCount  = ARRAY_SIZE(cscServerClusterIds),
    .AppInClustersList   = cscServerClusterIds,
    .AppOutClustersCount = ARRAY_SIZE(cscClientClusterIds),
    .AppOutClustersList  = cscClientClusterIds,
  },
  .serverCluster = cscServerClusters,
  .clientCluster = cscClientClusters,
};
static ClusterId_t cscClientClusterToBindIds[] =
{
  IDENTIFY_CLUSTER_ID,
  SCENES_CLUSTER_ID,
  ONOFF_CLUSTER_ID,
  LEVEL_CONTROL_CLUSTER_ID,
  COLOR_CONTROL_CLUSTER_ID,
  GROUPS_CLUSTER_ID,
//#ifdef OTAU_CLIENT
//  OTAU_CLUSTER_ID, // Always should be on last position in list of clusters ID.
//#endif // OTAU_CLIENT
  ZLL_COMMISSIONING_CLUSTER_ID,
};

static ClusterId_t cscServerClusterToBindIds[] =
{
  BASIC_CLUSTER_ID,
  IDENTIFY_CLUSTER_ID,
  ZLL_COMMISSIONING_CLUSTER_ID,
};

static AppBindReq_t cscBindReq =
{
  .srcEndpoint       = APP_ENDPOINT_COLOR_SCENE_CONTROLLER,
  .remoteServersCnt  = ARRAY_SIZE(cscClientClusterToBindIds),
  .remoteClientsCnt  = ARRAY_SIZE(cscServerClusterToBindIds),
  .groupId           = 0xffff,
  .remoteServers     = cscClientClusterToBindIds,
  .remoteClients     = cscServerClusterToBindIds,
  .callback          = NULL,
  .startIdentifyingFn = cscIdetifyStartIdentifyingCb
};

#if defined (_SLEEP_WHEN_IDLE_)
static void ZDO_WakeUpConf(ZDO_WakeUpConf_t *conf);
static ZDO_WakeUpReq_t zdoWakeUpReq;
static SYS_EventReceiver_t sleepEventListener = {.func = sleepEventHandler};
#endif
#if defined (_SLEEP_WHEN_IDLE_) && (APP_ENABLE_CONSOLE == 1)
static SYS_EventReceiver_t sleepModeListener = {.func = sleepModeHandler};
#endif
static SYS_EventReceiver_t zdoBusyPollCheck = { .func = isBusyOrPollCheck};
#ifdef OTAU_CLIENT
static void configureImageKeyDone(void);
#endif

/******************************************************************************
                    Prototypes section
******************************************************************************/

/******************************************************************************
                    Implementations section
******************************************************************************/

/**************************************************************************//**
\brief Initialize the application
******************************************************************************/
void appDeviceInit(void)
{
#if APP_ENABLE_CONSOLE == 1
  initConsole();
#endif

  /* Restore memory in case of power failure */
  if (PDS_IsAbleToRestore(Z3DEVICE_APP_MEMORY_MEM_ID))
    PDS_Restore(Z3DEVICE_APP_MEMORY_MEM_ID);

  /* Initialize the LCD screen */
  LCD_INIT();
  LCD_PRINT(0, 1, "ColorScene Controller");

  /* Enable buttons */
#if BSP_SUPPORT == BOARD_RCB_KEY_REMOTE
  BSP_InitButtons(appButtonsInd);
  BSP_SetButtonThreshold(BUTTON_PWR, 10);
  BSP_SetButtonThreshold(BUTTON_SEL, 30);
#elif BSP_SUPPORT == BOARD_SAMR21_ZLLEK
  BSP_OpenJoystick(appJoyStickInd);
  #if (QTOUCH_SUPPORT == 1)
    BSP_InitQTouch(appTouchInd);
  #endif // QTOUCH_SUPPORT
#endif // BSP_SUPPORT == BOARD_RCB_KEY_REMOTE

  if (!APP_RegisterEndpoint(&cscEndpoint, &cscBindReq))
    return;

  /* Subscribe the Commissioning update command for Target devices */
  ZCL_CommandManagerInit();
  cscBasicClusterInit();
  cscIdentifyClusterInit();
  cscGroupsClusterInit();
  cscScenesClusterInit();
  cscOnOffClusterInit();
  cscLevelControlClusterInit();
  cscColorControlClusterInit();
  cscCommissioningServerClusterInit();
  cscCommissioningClientClusterInit();
#ifdef OTAU_CLIENT
  cscAddOTAUClientCluster();
#endif //OTAU_CLIENT

#if defined (_SLEEP_WHEN_IDLE_)
#if (ZB_COMMISSIONING_ON_STARTUP == 1)
  SYS_EnableSleepWhenIdle();
#endif
#if (APP_ENABLE_CONSOLE == 1)
  SYS_SubscribeToEvent(HAL_EVENT_FALLING_ASLEEP, &sleepModeListener);
  SYS_SubscribeToEvent(HAL_EVENT_CONTINUING_SLEEP, &sleepModeListener);
#endif
  SYS_SubscribeToEvent(HAL_EVENT_WAKING_UP, &sleepEventListener);
#endif
  SYS_SubscribeToEvent(BC_EVENT_POLL_REQUEST, &zdoBusyPollCheck);
}

/**************************************************************************//**
\brief Callback about confguring image key on EEPROM
******************************************************************************/
#ifdef OTAU_CLIENT
static void configureImageKeyDone(void)
{
}
#endif
/**************************************************************************//**
\brief Device common task handler
******************************************************************************/
void appDeviceTaskHandler(void)
{
  switch (appDeviceState) // Actual device state when one joined network
  {
    case DEVICE_INITIAL_STATE:
      {
        appDeviceState = DEVICE_ACTIVE_IDLE_STATE;
#ifdef OTAU_CLIENT
          startOtauClient();
          ZCL_ConfigureOtauImageKey(configureImageKeyDone);
#endif
      }
      break;
    case DEVICE_ACTIVE_IDLE_STATE:
    default:
      break;
  }
}

/**************************************************************************//**
\brief Stops identifying on endpoints
******************************************************************************/
void appIdentifyStart(uint16_t identifyTime, bool colorEffect, uint16_t enhancedHue)
{
  cscIdentifyStart(identifyTime);
  (void)colorEffect,(void)enhancedHue;
}

/**************************************************************************//**
\brief Stops identifying on endpoints
******************************************************************************/
void appIdentifyStop(void)
{
  cscIdentifyStop();
}


#if BSP_SUPPORT == BOARD_RCB_KEY_REMOTE
/**************************************************************************//**
\brief Process color scene remote's power button's press
******************************************************************************/
static void processPwrButtonPressedEvent(void)
{
  if (APP_WORKING != appState)
    return;

  switch (altFunc & 0x03)
  {
    case 0:
      performTouchlink();
      break;

    case 1:
      initiateStartingNetwork();
      break;

    case 2:
      BDB_ResetToFactoryNew(true);
      break;

    case 3:
      performResetDeviceToFn();
      break;

    default:
      break;
  }
}

/**************************************************************************//**
\brief Process color scene remote's power button's press
******************************************************************************/
static void processPwrButtonReleasedEvent(void)
{
  if (APP_WORKING != appState)
    return;

  stopTouchlink();
}

/**************************************************************************//**
\brief Buttons handler

\param[in] event - event
\param[in] button - index
\param[in] data - data (AKA hold time)
******************************************************************************/
void appButtonsInd(BSP_ButtonEvent_t event, BSP_ButtonId_t button, uint8_t data)
{
  appSnprintf("btn = %d, ev = %d, data = %d\r\n", button, event, data);

  if (BUTTON_RIGHT_M == button)
  {
    if (BUTTON_PRESSED_EVENT == event)
      altFunc |= 0x02;
    else if (BUTTON_RELEASED_EVENT == event)
      altFunc &= ~0x02;
  }

  if (BUTTON_RELEASED_EVENT == event)
  {
    if ((2 == altFunc)  && (BUTTON_LEFT_M == button))
    {
    }
  }

  if (BUTTON_PWR == button)
  {
    if (BUTTON_PRESSED_EVENT == event)
    {
      processPwrButtonPressedEvent();
    }
    else if (BUTTON_RELEASED_EVENT == event)
    {
      processPwrButtonReleasedEvent();
    }
  }

  if (BUTTON_RIGHT_P == button)
  {
    if (BUTTON_PRESSED_EVENT == event)
      altFunc |= 0x01;
    else if (BUTTON_RELEASED_EVENT == event)
      altFunc &= ~0x01;
  }

  if (BUTTON_RIGHT_M == button)
  {
    if (BUTTON_PRESSED_EVENT == event)
      altFunc |= 0x02;
    else if (BUTTON_RELEASED_EVENT == event)
      altFunc &= ~0x02;
  }

  if (BUTTON_RELEASED_EVENT == event)
  {
    if ((appState == APP_WORKING) && isAnyDeviceBound())
    {
      /* Try to reestablish network communication */
      restartActivity();
      buttonHandler(button, data, altFunc);
    }
  }
}

#elif BSP_SUPPORT == BOARD_SAMR21_ZLLEK
/**************************************************************************//**
\brief BDB invoke commissioning callback

\param[in] conf - pointer to confirmation structure
******************************************************************************/
static void touchlinkCallback(BDB_InvokeCommissioningConf_t *conf)
{
  appSnprintf("CommissioningStatus = %d\r\n", conf->status);
  if(BDB_COMMISSIONING_SUCCESS == conf->status)
  {
    // ADD Group to light
    groupsSendAddGroup(APS_NO_ADDRESS,0,0,APP_ENDPOINT_COLOR_SCENE_CONTROLLER,0x2);
  }
}

/**************************************************************************//**
\brief performTouchlink

\param[in] None
******************************************************************************/
static void performTouchlink (void)
{
  memset(&AppbdbCommissioningreq,0,sizeof(BDB_InvokeCommissioningReq_t));
  AppbdbCommissioningreq.mode.touchlink = 1; 
  AppbdbCommissioningreq.initiatorReq = NULL;
  AppBindReq_t *appBindReqLocal = deviceBindReqs[0];

  AppbdbCommissioningreq.initiatorReq = &AppinitiatorReq;

  AppbdbCommissioningreq.initiatorReq->groupId = 0xffffu;

  AppbdbCommissioningreq.initiatorReq->initiatorEndpoint = appBindReqLocal->srcEndpoint;
  AppbdbCommissioningreq.initiatorReq->clientClustersCount = appBindReqLocal->remoteServersCnt;
  AppbdbCommissioningreq.initiatorReq->serverClustersCount = appBindReqLocal->remoteClientsCnt;
  AppbdbCommissioningreq.initiatorReq->clientClustersList=appBindReqLocal->remoteServers;
  AppbdbCommissioningreq.initiatorReq->serverClustersList=appBindReqLocal->remoteClients;
  AppbdbCommissioningreq.initiatorReq->callback = appBindReqLocal->callback;

  AppbdbCommissioningreq.BDB_InvokeCommissioningConf = touchlinkCallback;
  BDB_InvokeCommissioning(&AppbdbCommissioningreq);
}

/**************************************************************************//**
\brief JoyStick handler

\param[in] state - joystick state
******************************************************************************/
static void appJoyStickInd(BSP_JoystickState_t state)
{
  switch (state)
  {
    case JOYSTICK_PUSH:
      performTouchlink();
    break; 
    case JOYSTICK_LEFT:
     // Move to Red
      colorControlSendMoveToColorCommand(APS_NO_ADDRESS,0, 0, APP_ENDPOINT_COLOR_SCENE_CONTROLLER,
                                        /* Payload arguments */
                                        COLOR_X_RED,COLOR_Y_RED,5,0,0);
    break;
    case JOYSTICK_RIGHT:
     // Move to Green
      colorControlSendMoveToColorCommand(APS_NO_ADDRESS,0, 0, APP_ENDPOINT_COLOR_SCENE_CONTROLLER,
                                        /* Payload arguments */
                                        COLOR_X_GREEN,COLOR_Y_GREEN,5,0,0);
    break;
    case JOYSTICK_UP:
     // Move to Yellow
      colorControlSendMoveToColorCommand(APS_NO_ADDRESS,0, 0, APP_ENDPOINT_COLOR_SCENE_CONTROLLER,
                                        /* Payload arguments */
                                        COLOR_X_YELLOW,COLOR_Y_YELLOW,5,0,0);
    break;
    case JOYSTICK_DOWN:
     // Move to Blue
      colorControlSendMoveToColorCommand(APS_NO_ADDRESS,0, 0, APP_ENDPOINT_COLOR_SCENE_CONTROLLER,
                                        /* Payload arguments */
                                        COLOR_X_BLUE,COLOR_Y_BLUE,5,0,0);
    break;
    default:
      break;
  }
}
#endif // BSP_SUPPORT == BOARD_SAMR21_ZLLEK

#if defined (_SLEEP_WHEN_IDLE_)
/**************************************************************************//**
  \brief Processes HAL_EVENT_WAKING_UP event

  \param[in] eventId - id of raised event;
  \param[in] data    - event's data.
******************************************************************************/
static void sleepEventHandler(SYS_EventId_t eventId, SYS_EventData_t data)
{
  HAL_SleepControl_t *sleepControl = (HAL_SleepControl_t *)data;

  if (HAL_WAKEUP_SOURCE_EXT_IRQ == sleepControl->wakeupSource)
  {
    zdoWakeUpReq.ZDO_WakeUpConf = ZDO_WakeUpConf;
    ZDO_WakeUpReq(&zdoWakeUpReq);
  }
  (void)eventId; /* Do nothing, to avoid compiler warning */
}

/**************************************************************************//**
  \brief Wake up confirmation handler.

  \param[in] conf - confirmation parameters.
*****************************************************************************/
static void ZDO_WakeUpConf(ZDO_WakeUpConf_t *conf)
{
  (void)conf;
}

#if (APP_ENABLE_CONSOLE == 1)
/**************************************************************************//**
  \brief Processes HAL_EVENT_FALL_ASLEEP & HAL_EVENT_CONITUING_SLEEP events

  \param[in] eventId - id of raised event;
  \param[in] data    - event's data.
******************************************************************************/
static void sleepModeHandler(SYS_EventId_t eventId, SYS_EventData_t data)
{
  // when console is enabled, we go to idle mode to handle UART Rx interrupt
  // because UART Rx interrupt will not wake up the MCU from power save or power down modes
  *(HAL_SleepMode_t *)data = HAL_SLEEP_MODE_IDLE;
  (void)eventId;
}
#endif
#endif

/**************************************************************************//**
  \brief Processes BC_EVENT_BUSY_REQUEST and BC_EVENT_POLL_REQUEST events

  \param[in] eventId - id of raised event;
  \param[in] data    - event's data.
******************************************************************************/
static void isBusyOrPollCheck(SYS_EventId_t eventId, SYS_EventData_t data)
{
#if defined (_SLEEP_WHEN_IDLE_)
  bool *check = (bool *)data;

  if (BC_EVENT_POLL_REQUEST == eventId)
    *check |= isCommssioiningInProgress();
#else
  (void)eventId, (void)data;
#endif
}

#if((QTOUCH_SUPPORT == 1) && (BSP_SUPPORT == BOARD_SAMR21_ZLLEK))
/**************************************************************************//**
\brief Touch handler

\param[in] event - event
\param[in] button - index
\param[in] data - touch data
******************************************************************************/
void appTouchInd(BSP_TouchEvent_t event, BSP_ButtonId_t button, uint8_t data)
{
  static bool ButtonOnOff = 0;
  static uint8_t sliderInitialState = 0;
  uint8_t sliderPosition = 0;
  uint8_t triggerPacket = 0;

  /*Since Sleep When Idle to be disabled for Touch operations for periodic operations of the Timer to monitor for touch events
    Waking up the MCU when event recognized*/
  HAL_WakeUpCPU();
  /* Left button pressed*/   
  if ((BUTTON_L == button) && (event == TOUCHKEY_PRESSED_EVENT))
  {
    if(!ButtonOnOff)
    {
      LCD_PRINT(0, 1, "Button ON Pressed");
      ButtonOnOff =1;
      onOffSendOnOffToggle(APS_NO_ADDRESS,
                            0, // address
                            0, // dstEndpoint
                            APP_ENDPOINT_COLOR_SCENE_CONTROLLER,
                            ZCL_ONOFF_CLUSTER_ON_COMMAND_ID);
    }
    else
    {
      LCD_PRINT(0, 1, "Button OFF Pressed");
      ButtonOnOff =0;
      onOffSendOnOffToggle(APS_NO_ADDRESS,
                            0, // address
                            0, // dstEndpoint
                            APP_ENDPOINT_COLOR_SCENE_CONTROLLER,
                            ZCL_ONOFF_CLUSTER_OFF_COMMAND_ID);
    }
  }

  /* Right button pressed*/
  else if ((BUTTON_R == button) && (event == TOUCHKEY_PRESSED_EVENT))
  {
    /* Display when touch is enabled/Used*/
    LCD_PRINT(0, 1, "Performing Touchlink");
    performTouchlink();
    LCD_PRINT(0u, 1,"                    ");
  }

  else if((SLIDER == button) && (event == TOUCHKEY_PRESSED_EVENT))
  {
    if(data > 32 )
      sliderPosition = (data >> 5);
    else
      sliderPosition = sliderInitialState;
    
    if(sliderPosition != sliderInitialState)
    {
      uint8_t stepMode;
      if(sliderPosition > sliderInitialState)
      {
        triggerPacket = sliderPosition - sliderInitialState;
        stepMode = ZLL_LEVEL_CONTROL_UP_DIRECTION;
      }
      else
      {
        triggerPacket = sliderInitialState - sliderPosition;
        stepMode = ZLL_LEVEL_CONTROL_DOWN_DIRECTION;
      }
      if(triggerPacket >= 1)
      {
        triggerPacket = 0;
        sliderInitialState = sliderPosition;
        levelControlSendStep(APS_NO_ADDRESS,
                            0, // address
                            0, // dstEndpoint
                            APP_ENDPOINT_COLOR_SCENE_CONTROLLER,
                            stepMode,40,5,1,0,0);
      }
    }
   }
}
#endif

#ifdef OTAU_CLIENT
/**************************************************************************//**
\brief Adds OTAU client cluster to list of clients clusters of color 
       scene controller device.
******************************************************************************/
static void cscAddOTAUClientCluster(void)
{
  cscClientClusters[CSC_CLIENT_CLUSTERS_COUNT - 1U] = ZCL_GetOtauClientCluster();
}
#endif // OTAU_CLIENT

#endif // #if (APP_Z3_DEVICE_TYPE == APP_DEVICE_TYPE_COLOR_SCENE_CONTROLLER)
// eof colorSceneController.c

