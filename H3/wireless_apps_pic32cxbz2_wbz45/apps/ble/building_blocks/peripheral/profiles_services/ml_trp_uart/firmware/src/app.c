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
#include "ble_trsps.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

uint16_t conn_hdl[MAX_CONNECTED_DEVICE];        // connection handle info captured @BLE_GAP_EVT_CONNECTED event
uint8_t no_of_links = 0;                        // No of connected peripheral devices
uint8_t uart_rx_buffer[100];
uint8_t uart_tx_buffer[100];

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


    appData.appQueue = xQueueCreate( 64, sizeof(APP_Msg_T) );
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}

void uart_cb(SERCOM_USART_EVENT event, uintptr_t context)
{
  // If RX data from UART reached threshold (previously set to 1)
  if( event == SERCOM_USART_EVENT_READ_THRESHOLD_REACHED )
  {
      
      APP_Msg_T   appMsg;
      
      appMsg.msgId = APP_MSG_UART_RX_EVT;
      OSAL_QUEUE_SendISR(&appData.appQueue, &appMsg);
  }
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
    p_appMsg=appMsg;

    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            bool appInitialized = true;
            //appData.appQueue = xQueueCreate( 10, sizeof(APP_Msg_T) );
            // Enable UART Read
            SERCOM0_USART_ReadNotificationEnable(true, true);
            // Set UART RX notification threshold to be 1
            SERCOM0_USART_ReadThresholdSet(1);
            // Register the UART RX callback function
            SERCOM0_USART_ReadCallbackRegister(uart_cb, (uintptr_t)NULL);
            
            for(uint8_t i = 0; i<MAX_CONNECTED_DEVICE; i++ )
            {
                conn_hdl[i] = 0xFFFF;
            }
            APP_BleStackInit();
            // Start Advertisement
            BLE_GAP_SetAdvEnable(0x01, 0x00);
            SERCOM0_USART_Write((uint8_t *)"Advertising\r\n",13);

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
                if(p_appMsg->msgId==APP_MSG_BLE_STACK_EVT)
                {
                    // Pass BLE Stack Event Message to User Application for handling
                    APP_BleStackEvtHandler((STACK_Event_T *)p_appMsg->msgData);
                }
                if(p_appMsg->msgId==APP_MSG_UART_RX_EVT)
                {
                    uint8_t data;
                    // Read 1 byte data from UART
                    SERCOM0_USART_Read(&data, 1);
                    for(uint8_t i = 0; i<MAX_CONNECTED_DEVICE; i++ )
                    {
                        // Send the data from UART to connected devices through Transparent service
                        if(conn_hdl[i] != 0xFFFF )
                        {
                            BLE_TRSPS_SendData(conn_hdl[i], 1, &data);
                        }
                    }
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

// To suppress compiler warning
int _mon_getc(int canblock)
{
   int c = 0;
   bool success = false;
   (void)canblock;
   do
   {
       success = SERCOM0_USART_Read((uint8_t *)&c, 1);                
   }while( !success);
   return c;
}

void _mon_putc(char c)
{
   bool success = false;
   do
   {
       success = SERCOM0_USART_Write((uint8_t *)&c, 1);
   }while (!success);
}

/*******************************************************************************
 End of File
 */
