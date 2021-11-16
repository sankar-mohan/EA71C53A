/**************************************************************************//**
  \file  rMain.c

  \brief Main

  \author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2015, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

  \internal
  History:

 ******************************************************************************/
/******************************************************************************
                   Includes section
 ******************************************************************************/
#include <rTypes.h>
#include <rParser.h>
#include <rMem.h>
#include <rEvents.h>
#include <rZcl.h>
#include <rSys.h>
//#include <leds.h>
#include <aps.h>
#include <console.h>
#include <bufferallocator.h>
#include <ZAppSiSerialManager.h>
#include <wlPdsMemIds.h>
#include <rInterpan.h>
#include <sysIdleHandler.h>
#include <sleep.h>
#include <sysAssert.h>
#include "compiler.h"
#include "system.h"
#if defined(_RUNNER_OSAL_)
#include <zgb_api.h>
#if (BDB_SUPPORT == 1)
#include <bdb_api.h>
#include <zcl_api.h>
#endif
#include <cs_api.h>
#include "osal_freertos.h"
#endif
#include <pdsDataServer.h>

#if (BDB_SUPPORT == 1)
#include <bdb.h>
#endif
/*****************************************************************************
                         Types section
******************************************************************************/
typedef void (*taskHandlers_t)(void);
/****************************************************************************
                         Static functions prototypes section
******************************************************************************/
static void rInit(void);
static void assertHandler(SYS_AssertParam_t *assertParam);
#if (BDB_SUPPORT == 1)
static void bdbInitCompleted(void);
#endif
/******************************************************************************
                         Prototypes section
 ******************************************************************************/
void *rGetMem(void);
/******************************************************************************
                         Static variables section
 ******************************************************************************/
//static uint8_t taskPosted = 0; //Array of bits indicates which task processor requested

/*****************************************************************************
                         Global variables section
******************************************************************************/
RState_t rState = R_INIT_STATE;
//Bit position number is equal to appropriate task ID
/*
static PROGMEM_DECLARE(taskHandlers_t taskHandlers[R_TASK_TABLE_SIZE]) = {rUsartTaskHandler,
  rParserTaskHandler};
  */
/******************************************************************************
                         Implementations section
 ******************************************************************************/

/**************************************************************************//**
\brief Handler to notify assertion (from down layers)
\param[in] SYS_AssertParam_t * type, holding information about the assertion
******************************************************************************/
static void assertHandler(SYS_AssertParam_t *assertParam)
{
#if (APP_ENABLE_SYSASSERT == 1)
  ZS_CommandBuffer_t *commandBuffer = bufferAllocator.allocate();
  uint8_t *ptr;
  
  if (NULL != assertParam)
  {
    switch (assertParam->level)
    {
      case FATAL_LEVEL:
      {
        /* fatal is considered irrecoverable.
           go for reset, after backing up needed stuff */
        while (1)
        {
          (void)assertParam->dbgCode;
        }
      }
      break;

      /* Add code here to take appropriate action for a level */
      case ERROR_LEVEL:
      {
        /* add error handling */
      }
      break;

      case WARN_LEVEL:
      default:
      break;
    }
    commandBuffer->commandFrame.commandId = R_SYS_ASSERT_INDICATION;

    ptr = &commandBuffer->commandFrame.payload[0];

    SYS_BYTE_MEMCPY(ptr, &assertParam->level, sizeof (uint16_t));
    ptr += sizeof (uint16_t);  

    SYS_BYTE_MEMCPY(ptr, &assertParam->dbgCode, sizeof (uint16_t));
    ptr += sizeof (uint16_t);  
  
    commandBuffer->commandFrame.length = R_COMMAND_ID_SIZE + (2*sizeof(uint16_t));
    incomingFrameHandler(commandBuffer);
  }
#else
  (void)assertParam;
#endif
}

/*****************************************************************************
  Peforms APS sublayer processing
  Parameters: None
  Returns: None
 *****************************************************************************/
void APL_TaskHandler(void)
{
  if (R_INIT_STATE == rState)
  {
    rInit();
#if (BDB_SUPPORT == 1)
  BDB_Init(bdbInitCompleted);
#endif /* BDB_SUPPORT */
  }
  else
  {
    rParserTaskHandler();
  }
}

static void rInit(void)
{
  //BSP_OpenLeds();
//  BSP_OffLed(LED_RED);
//  BSP_OffLed(LED_YELLOW);
//  BSP_OnLed(LED_GREEN);

#ifdef _ENABLE_PERSISTENT_SERVER_
  if (PDS_IsAbleToRestore(BC_ALL_MEMORY_MEM_ID))
    PDS_Restore(BC_ALL_MEMORY_MEM_ID);
  else if (PDS_IsAbleToRestore(NWK_SECURITY_COUNTERS_ITEM_ID))
    PDS_Restore(NWK_SECURITY_COUNTERS_ITEM_ID);
#endif // _ENABLE_PERSISTENT_SERVER_

  rState = R_IDLE_STATE;

  openBufferAllocator();

#if (APP_INTERFACE == APP_INTERFACE_USART) || (APP_INTERFACE == APP_INTERFACE_VCP) || (APP_INTERFACE == APP_INTERFACE_UART) || (APP_INTERFACE == APP_INTERFACE_USBFIFO)
  openSerialManager(INTERFACE_ID_USART0, incomingFrameHandler);
#elif (APP_INTERFACE == APP_INTERFACE_SPI)
  openSerialManager(INTERFACE_ID_SPI, incomingFrameHandler);
#else
  #error "Unknown serial interface"
#endif

  rInitEvents();
#ifdef _INTERPAN_
 rInitInterpan();
#endif

#ifdef _ZCL_SUPPORT_
  zclReset();
#endif /* ZCL_SUPPORT == 1 */
  SYS_AssertSubscribe(assertHandler);
}
#if (BDB_SUPPORT == 1)
/**************************************************************************
\brief Callback on BDB init completetion
***************************************************************************/
static void bdbInitCompleted(void)
{
  // nothing to do
}
#endif // BDB_SUPPORT

#if defined(_RUNNER_OSAL_)
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName) {
    (void) pcTaskName;
    (void) pxTask;
 
    /* Run time task stack overflow checking is performed if
    configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
    called if a task stack overflow is detected. Note the system/interrupt
    stack is not checked. */
    taskDISABLE_INTERRUPTS();
    for (;;);
}
#endif

/***************************************************************************//**
\brief Static callback for adapter assert
\param condition[in] - condition for assert
\param code[in] - assert code
\return none
******************************************************************************/
void adapterAssert(bool condition, uint16_t code)
{
  (void)condition;
  (void)code;
  halAssert(condition, code);
}

void vApplicationIdleHook(void)
{
#ifdef _ENABLE_PERSISTENT_SERVER_
  PDS_StoreItemTaskHandler(); 
#endif
  //printf("[IDLE Task] FW sleep procedure...\r\n");
}

/**************************************************************************//**
\brief Network unsolicited update notification
\param notify - update notification parameters
******************************************************************************/
#ifndef ZAPPSI_BDB
void ZDO_MgmtNwkUnsolicitedUpdateNotf(ZDO_MgmtNwkUnsolicatedEnhancedUpdateNotf_t *notify)
{
  (void)notify;
}
#endif

#if defined(_RUNNER_OSAL_)
void process_UARTRunner_evt(char* cmdBuf)
{
  APL_TaskHandler();
  (void)cmdBuf;
}
#endif

#if !defined(_RUNNER_OSAL_)
/**************************************************************************//**
\brief ZDO_ResetNetwork Confirmation function

\param[out] conf - Confirmation structure with status
******************************************************************************/
void ZDO_ResetNetworkConf(ZDO_ResetNetworkConf_t *conf)
{
  sysAssert(ZDO_SUCCESS_STATUS == conf->status, ZDOINIT_ZDOINITRESETNETWORKCONF_0);
  SYS_PostTask(APL_TASK_ID);
}
#endif

/**********************************************************************//**
  \brief Main - C program main start function

  \param none
  \return none
**************************************************************************/
int main(void)
{
#ifdef CHIMERA_SOC
  SYS_Initialize(NULL);
#else
  SYS_SysInit();
#endif
    while (1)
    {
    #if defined(_RUNNER_OSAL_)
      SYS_Tasks();
    #else
      SYS_RunTask();
    #endif
      SYS_PostIdleTask();
    }
}

/* eof rMain.c */
