/*******************************************************************************
  BLE Log Middleware Header File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_log.h

  Summary:
    This file contains the BLE Log functions for this project.

  Description:
    This file contains the BLE Log functions for this project.
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

/**
 * @addtogroup BLE_LOG
 * @{
 * @brief Header file for the BLE Log (ble_log) module.
 * @note Definitions and prototypes for the BLE Log application programming interface.
 */

#ifndef BLE_LOG_H
#define BLE_LOG_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>
#include <string.h>
#include "bt_sys_log.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************

/**@addtogroup BLE_LOG_DEFINES Defines
 * @{ */

/**@defgroup LOG_PROTOCOL Definitions of log protocol.
 * @brief The definitions of log protocol.
 * @{ */
#define LOG_END_CHECK_BYTE_1                0x55        /**< First byte of the end check of the protocol. */
#define LOG_END_CHECK_BYTE_2                0xAA        /**< Second byte of the end check of the protocol. */
/** @} */

/**@defgroup LOG_PARAMS_SIZE Size of parameters in the log packet.
 * @brief The size of parameters in the log packet.
 * @{ */
#define LOG_PACKET_LENGTH_SIZE              0x02                                            /**< Size of the length parameter in log protocol. */
#define LOG_PACKET_TYPE_SIZE                0x02                                            /**< Size of the log type parameter in log protocol. */
#define LOG_PACKET_HEADER_SIZE              (LOG_PACKET_LENGTH_SIZE+LOG_PACKET_TYPE_SIZE)   /**< Size of the parameters in header (length and type) in log protocol. */
#define LOG_PACKET_END_SIZE                 0x02                                            /**< Size of the end check parameter in log protocol. */
/** @} */

/**@defgroup LOG_PAYLOAD_PARAMS_SIZE Size of parameters in log payload of the log packet.
 * @brief The size of parameters in the log payload of log packets.
 * @{ */
#define HCI_COMMAND_OPCODE_SIZE             0x02                                            /**< Size of opcode parameter in HCI command packet. */
#define HCI_COMMAND_PARAMETER_LENGTH_SIZE   0x01                                            /**< Size of length parameter in HCI command packet. */

#define HCI_EVENT_CODE_SIZE                 0x01                                            /**< Size of event code parameter in HCI event packet. */
#define HCI_EVENT_PARAMETER_LENGTH_SIZE     0x01                                            /**< Size of length parameter in the HCI event packet. */
#define HCI_EVENT_CC_PARAMETER_NUMBER_SIZE  0x01                                            /**< Size of number of HCI commands parameter in HCI command complete event. */
#define HCI_EVENT_CC_PARAMETER_OPCODE_SIZE  0x02                                            /**< Size of opcode parameter in HCI command complete event. */
#define HCI_EVENT_CC_PARAMETER_STATUS_SIZE  0x01                                            /**< Size of status parameter in HCI command complete event. */
#define HCI_EVENT_CS_PARAMETER_NUMBER_SIZE  0x01                                            /**< Size of number of HCI commands parameter in HCI command status event. */
#define HCI_EVENT_CS_PARAMETER_OPCODE_SIZE  0x02                                            /**< Size of opcode parameter in HCI command status event. */
#define HCI_EVENT_CS_PARAMETER_STATUS_SIZE  0x01                                            /**< Size of status parameter in HCI command status event. */
#define HCI_EVENT_LE_PARAMETER_SUBCODE_SIZE 0x01                                            /**< Size of sub-event code parameter in HCI LE meta event. */

#define HCI_ACL_HEADER_SIZE                 0x02                                            /**< Size of the parameters in header (BC:2, PB: 2, Handle:10) in HCI ACL packet. */
#define HCI_ACL_PKT_PARAMETER_LENGTH_SIZE   0x02                                            /**< Size of length parameter in HCI ACL packet. */
/** @} */

/**@defgroup BM70_OPCODE BM70 log opcode definitions (for debugging)
 * @brief The definition of opcode in BM70 log packet
 * @{ */
#define BM70_OPCODE_HCI_CMD                 0x82    /**< HCI command. */
#define BM70_OPCODE_HCI_ACL_TX              0x83    /**< HCI ACL Tx. */
#define BM70_OPCODE_HCI_EVT                 0x85    /**< HCI event. */
#define BM70_OPCODE_HCI_ACL_RX              0x86    /**< HCI ACL Rx. */
/** @} */

/**@} */ //BLE_LOG_DEFINES

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************

/**@addtogroup BLE_LOG_STRUCTS Structures
 * @{ */

/**@brief Parameters of log from stack. */
typedef struct BLE_Log_T
{
    uint16_t    logType;                /**< Type of log. */
    uint16_t    logId;                  /**< ID of log in the specific log type. */
    uint16_t    paramsLength;           /**< Length of of the additional returned parameters. */
    uint16_t    payloadLength;          /**< Length of the payload. */
    uint8_t     *p_logPayload;          /**< Payload of log. */
    uint8_t     *p_returnParams;        /**< Additional returned parameters of the executed HCI command if logType is @ref BT_LOG_TYPE_HCI_COMMAND. */
    uint16_t    result;                 /**< Result of the executed HCI command if logType is @ref BT_LOG_TYPE_HCI_COMMAND. */
} BLE_Log_T;

/**@brief BLE_LOG print callback function prototype. */
typedef void (*BLE_LOG_PrintCb)(uint8_t opcode, uint16_t logLength, uint8_t *p_logPayload);

/**@} */ //BLE_LOG_STRUCTS

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************

/**@addtogroup BLE_LOG_FUNS Functions
 * @{ */

/**@brief Initialize BLE_LOG module.
 *
 * @param[in] printCb       Pointer to the @ref BLE_LOG_PrintCb print function.
*/
void BLE_LOG_Init(BLE_LOG_PrintCb printCb);

/**@brief Processing function of stack log.
 *
 * @param[in] p_log         Pointer to log buffer.
*/
void BLE_LOG_StackLogHandler(BLE_Log_T *p_log);

/**@} */ //BLE_LOG_FUNS
#endif

/**
  @}
*/

