/*******************************************************************************
  Bluetooth Sys Log Header File

  Company:
    Microchip Technology Inc.

  File Name:
    bt_sys_log.h

  Summary:
    This file contains the BT system log definitions for the project.

  Description:
    This file contains the BT system log definitions for the project.
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

#ifndef BT_SYS_LOG_H
#define BT_SYS_LOG_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdbool.h>
#include <stdint.h>
#include "host_hci_defs.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************

/**@addtogroup BT_SYS_LOG_DEFINES Defines
 * @{ */

/**@defgroup BLE_SYS_LOG_GRP Group definitions of BT system log
 * @brief The definition of BT system log group
 * @{ */
#define BT_LOG_GROUP_SIGNAL         0x0000      /**< 0x0000 - 0x00FF. Group: Debug Log Control Signaling. */
#define BT_LOG_GROUP_HCI            0x0100      /**< 0x0100 - 0x01FF. Group: HCI. */
#define BT_LOG_GROUP_CTRL           0x0200      /**< 0x0200 - 0x03FF. Group: BT Controller. */
#define BT_LOG_GROUP_FUNC           0x0400      /**< 0x0400 - 0x05FF. Group: API Function. */
#define BT_LOG_GROUP_EVT            0x0600      /**< 0x0600 - 0x07FF. Group: API Event. */
/** @} */

/**@} */ //BT_SYS_LOG_DEFINES

/**@addtogroup BT_SYS_LOG_ENUMS Enumerations
 * @{ */

/**@brief The definition of id in signal packets */
typedef enum BT_LOG_SigId_T
{
    BT_LOG_TYPE_SIG_DEVICE_INFORMATION = (BT_LOG_GROUP_SIGNAL+0x0001),              /**< Device Information. */
    BT_LOG_TYPE_SIG_END
}BT_LOG_SigId_T;

/**@brief The definition of log type of HCI logs. */
typedef enum BT_LOG_HciType_T
{
    BT_LOG_TYPE_HCI_COMMAND = (BT_LOG_GROUP_HCI+0x0001),    /**< HCI Command. */
    BT_LOG_TYPE_HCI_ACL_TX,                                 /**< HCI ACL Tx Packet. */
    BT_LOG_TYPE_HCI_ACL_RX,                                 /**< HCI ACL Rx Packet. */
    BT_LOG_TYPE_HCI_EVENT,                                  /**< HCI Event. */
    BT_LOG_TYPE_HCI_END
}BT_LOG_HciType_T;

/**@brief The definition of log type of LLCP logs. */
typedef enum BT_LOG_LlcpType_T
{
    BT_LOG_TYPE_CTRL_LLCP_TX = BT_LOG_GROUP_CTRL,           /**< BLE LLCP Tx Packet. */
    BT_LOG_TYPE_CTRL_LLCP_RX,                               /**< BLE LLCP Rx Packet. */
    BT_LOG_TYPE_CTRL_LLCP_END
}BT_LOG_LlcpType_T;

/**@} */ //BT_SYS_LOG_ENUMS

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************

/**@brief The structure contains information about Bluetooth log event to application. */
typedef struct BT_LOG_Event_T
{
    uint16_t    evtLen;                     /**< Length of the event */
    uint8_t     *p_event;                   /**< Pointer to the event field */
} BT_LOG_Event_T;

#endif//BT_SYS_LOG_H
