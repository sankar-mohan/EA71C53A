/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app_ble_handler.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

#ifndef _APP_BLE_HANDLER_H
#define _APP_BLE_HANDLER_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <string.h>
#include "gap_defs.h"
#include "ble_gap.h"
#include "ble_l2cap.h"
#include "ble_smp.h"
#include "gatt.h"
#include "ble_dtm.h"
#include "ble_dm/ble_dm.h"
#include "ble_gcm/ble_dd.h"
#include "ble_gcm/ble_scm.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
/**@defgroup APP_BLE_MAX_LINK_NUMBER APP_BLE_MAX_LINK_NUMBER
 * @brief The definition of maximum BLE links that can exist
 * @{ */
#define APP_BLE_MAX_LINK_NUMBER                     0x01
/** @} */
/**@brief Enumeration type of BLE state. */
typedef enum APP_BLE_LinkState_T
{
    APP_BLE_STATE_STANDBY,                                                /**< Standby state. i.e. not advertising */
    APP_BLE_STATE_ADVERTISING,                                            /**< BLE is advertising */
    APP_BLE_STATE_SCANNING,                                               /**< BLE is scanning (only for BLE_GAP_ROLE_CENTRAL) */
    APP_BLE_STATE_CONNECTED,                                              /**< BLE is connected */
    APP_BLE_STATE_TOTAL
} APP_BLE_LinkState_T;


// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

/**@brief The structure contains the BLE Connection parameters. */
typedef struct APP_BLE_ConnData_T
{
    uint8_t                role;                                           /**< GAP role, see @ref BLE_GAP_ROLE. */
    uint16_t               handle;                                         /**< Connection handle associated with this connection. */
    BLE_GAP_Addr_T         remoteAddr;                                     /**< See @ref BLE_GAP_Addr_T. */
    uint16_t               connInterval;                                   /**< Connection interval used on this connection. Range should be @ref BLE_GAP_CP_RANGE. */
    uint16_t               connLatency;                                    /**< Slave latency for the connection in terms of number of connection events, see @ref BLE_GAP_CP_RANGE. */
    uint16_t               supervisionTimeout;                             /**< Supervision timeout for the LE Link, see @ref BLE_GAP_CP_RANGE. */
} APP_BLE_ConnData_T;

/**@brief The structure contains the BLE link related information maintained by the application Layer */
typedef struct APP_BLE_ConnList_T
{
    APP_BLE_LinkState_T         linkState;                                              /**< BLE link state. see @ref APP_BLE_LinkState_T */
    APP_BLE_ConnData_T          connData;                                               /**< BLE connection information. See @ref APP_BLE_ConnData_T */
} APP_BLE_ConnList_T;

// *****************************************************************************
// *****************************************************************************
// Section: Global Variables
// *****************************************************************************
// *****************************************************************************

// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_BleGapEvtHandler( BLE_GAP_Event_T *p_event )

  Summary:
     BLE Stack application GAP event handler.

  Description:

  Precondition:

  Parameters:
    None.

  Returns:
    None.

*/
void APP_BleGapEvtHandler(BLE_GAP_Event_T *p_event);

/*******************************************************************************
  Function:
    void APP_BleL2capEvtHandler( BLE_L2CAP_Event_T *p_event )

  Summary:
     BLE application L2cap event handler.

  Description:

  Precondition:

  Parameters:
    None.

  Returns:
    None.

*/
void APP_BleL2capEvtHandler(BLE_L2CAP_Event_T *p_event);

/*******************************************************************************
  Function:
    void APP_GattEvtHandler( GATT_Event_T * p_event )

  Summary:
     BLE application GATT event handler.

  Description:

  Precondition:

  Parameters:
    None.

  Returns:
    None.

*/
void APP_GattEvtHandler(GATT_Event_T * p_event);

/*******************************************************************************
  Function:
    void APP_BleSmpEvtHandler( BLE_SMP_Event_T *p_event )

  Summary:
     BLE application SMP event handler.

  Description:

  Precondition:

  Parameters:
    None.

  Returns:
    None.

*/
void APP_BleSmpEvtHandler(BLE_SMP_Event_T *p_event);

/*******************************************************************************
  Function:
    void APP_DmEvtHandler( BLE_DM_Event_T *p_event )

  Summary:
     BLE application DM event handler.

  Description:

  Precondition:

  Parameters:
    None.

  Returns:
    None.

*/
void APP_DmEvtHandler(BLE_DM_Event_T *p_event);




void APP_InitConnList(void);
void APP_UpdateLocalName(uint8_t devNameLen, uint8_t *p_devName);
APP_BLE_LinkState_T APP_GetBleState(void);
void APP_SetBleState(APP_BLE_LinkState_T state);

#endif /* _APP_BLE_HANDLER_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

