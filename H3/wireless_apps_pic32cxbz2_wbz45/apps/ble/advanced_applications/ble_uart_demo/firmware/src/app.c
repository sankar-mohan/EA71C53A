/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <string.h>
#include "app.h"
#include "definitions.h"
#include "app_ble.h"
#include "ble_util/byte_stream.h"
#include "timers.h"
#include "ble_dis/ble_dis.h"
#include "app_ble_handler.h"
#include "app_uart.h"
#include "app_cmd_evt_proc.h"
#include "app_timer.h"
#include "app_trps.h"
#include "app_adv.h"
#include "app_led.h"
#include "app_key.h"

#ifdef APP_TRPC_ENABLE
#include "app_trpc.h"
#include "app_scan.h"
#endif

#include "app_eic.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
#define APP_QUEUE_LENGTH    96



// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/

static void APP_ConfigBleBuiltInSrv(void)
{
    GATTS_GattServiceOptions_T gattServiceOptions;

    /* GATT Service option */
    gattServiceOptions.enable = 1;
    GATTS_ConfigureBuildInService(&gattServiceOptions);
}

static void APP_Init(void)
{
    APP_BleStackInit();
    APP_UartInit();

    /* Configure build-in GATT Services*/
    APP_ConfigBleBuiltInSrv();

    /* Add BLE Service */
    BLE_DIS_Add();

    APP_LED_Init();

    APP_KEY_Init();
    
    APP_UpdateLocalName(0, NULL);
    APP_InitConnList();
    APP_ADV_Init();

    APP_TRP_COMMON_Init();
    APP_TRPS_Init();

#ifdef APP_TRPC_ENABLE
    APP_SCAN_Init();
    APP_TRPC_Init();
#endif

    APP_EIC_Init();

#ifdef APP_UART_CMD_SET_ENABLE
    APP_SendCmdCompleteEvt(0, UART_GROUP_UTILITY, UART_UTILITY_CMD_INIT_DONE, 0x0000, NULL);
#endif
}


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;


    appData.appQueue = xQueueCreate( APP_QUEUE_LENGTH, sizeof(APP_Msg_T) );
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{
    APP_Msg_T    appMsg[1];
    APP_Msg_T   *p_appMsg;
    uint8_t     instance;
    APP_TRP_ConnList_T *p_connList;
    p_appMsg=appMsg;

    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            bool appInitialized = true;
            //appData.appQueue = xQueueCreate( APP_QUEUE_LENGTH, sizeof(APP_Msg_T) );

            APP_Init();

#ifdef ENABLE_SLEEP_MODE
            RTC_Timer32Start();
#endif

            if (appInitialized)
            {

                appData.state = APP_STATE_SERVICE_TASKS;
            }
            break;
        }

        case APP_STATE_SERVICE_TASKS:
        {
            if (OSAL_QUEUE_Receive(&appData.appQueue, &appMsg, OSAL_WAIT_FOREVER))
            {
                switch (p_appMsg->msgId)
                {
                    case APP_MSG_BLE_STACK_EVT:
                    {
                        // Pass BLE Stack Event Message to User Application for handling
                        APP_BleStackEvtHandler((STACK_Event_T *)p_appMsg->msgData);
                    }
                    break;

                    case APP_MSG_UART_CMD_READY:
                    {
                        APP_UartMsgHandler();
                    }
                    break;

                    case APP_MSG_UART_FETCH:
                    {
                        APP_UART_FetchData();
                    }
                    break;

                    case APP_MSG_PROTOCOL_RSP:
                    {
                        p_connList = ((APP_TIMER_TmrElem_T *)p_appMsg->msgData)->p_tmrParam;
                        
                        if ((p_connList != NULL) && (p_connList->trpRole == APP_TRP_SERVER_ROLE))
                            APP_TRP_COMMON_SendCheckSumCommand(p_connList);
                        #ifdef APP_TRPC_ENABLE
                        else if ((p_connList != NULL) && (p_connList->trpRole == APP_TRP_CLIENT_ROLE))
                            APP_TRPC_ProtocolErrRsp(p_connList);
                        #endif
                    }
                    break;

                    case APP_MSG_FETCH_TRP_RX_DATA:
                    {
                        p_connList = ((APP_TIMER_TmrElem_T *)p_appMsg->msgData)->p_tmrParam;
                        if ((p_connList != NULL) && (p_connList->trpRole == APP_TRP_SERVER_ROLE))
                            #ifdef APP_BLE_MULTI_LINK_ENABLE
                            APP_TRPS_LeRxMultiLinkProc(p_connList);
                            #else
                            APP_TRPS_LeRxProc(p_connList);
                            #endif
                        #ifdef APP_TRPC_ENABLE
                        else if ((p_connList != NULL) && (p_connList->trpRole == APP_TRP_CLIENT_ROLE))
                            APP_TRPC_LeRxProc(p_connList);
                        #endif
                    }
                    break;
                    
                    case APP_MSG_UART_SEND:
                    {
                        p_connList = ((APP_TIMER_TmrElem_T *)p_appMsg->msgData)->p_tmrParam;
                        if (p_connList != NULL)
                            APP_TRP_COMMON_SendTrpProfileDataToUART(p_connList);
                    }
                    break;

                    case APP_MSG_LED_TIMEOUT:
                    {
                        APP_LED_Elem_T *p_ledElem;

                        instance = ((APP_TIMER_TmrElem_T *)p_appMsg->msgData)->instance;
                        p_ledElem = (APP_LED_Elem_T *)((APP_TIMER_TmrElem_T *)p_appMsg->msgData)->p_tmrParam;
                        APP_LED_StateMachine(instance, p_ledElem);
                    }
                        break;

                    case APP_MSG_SCAN_ADV_TIMEOUT:
                    {
                    #ifdef APP_TRPC_ENABLE
                        APP_SCAN_ConnectDevice();
                    #endif
                    }
                        break;

                    case APP_MSG_KEY_SCAN:
                    {
                        APP_KEY_Scan();
                    }
                        break;

                    default:
                        break;
                }
            }
            break;
        }

        /* TODO: implement your application state machine.*/


        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}


/*******************************************************************************
 End of File
 */
