/*******************************************************************************
  BLE Battery Service Header File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_bas.h

  Summary:
    This file contains the BLE Battery Service functions for application user.

  Description:
    This file contains the BLE Battery Service functions for application user.
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
 * @addtogroup BLE_BAS BLE BAS
 * @{
 * @brief Header file for the BLE Battery Service.
 * @note Definitions and prototypes for the BLE Battery Service stack layer application programming interface.
 */
#ifndef BLE_BAS_H
#define BLE_BAS_H

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
/**@defgroup BLE_BAS_ASSIGN_HANDLE BLE_BAS_ASSIGN_HANDLE
 * @brief Assigned attribute handles of BLE Battery Service.
 * @{ */
#define BAS_START_HDL                               0x00E0                                      /**< The start attribute handle of transparent service. */
/** @} */

/**@brief Definition of BLE Battery Service attribute handle */
typedef enum BLE_BAS_AttributeHandle_T
{
    BAS_HDL_SVC = BAS_START_HDL,           /**< Handle of Primary Service of BLE Batteryn Service. */
    BAS_HDL_CHAR_BAT_LEVEL,                /**< Handle of Battery Level characteristic. */
    BAS_HDL_CHARVAL_BAT_LEVEL,             /**< Handle of Battery Level characteristic value. */
<#if BAS_NOTIFY_ENABLE == true>
    BAS_HDL_CCCD_BAT_LEVEL,                /**< Handle of Battery Level characteristic CCCD */
</#if>
}BLE_BAS_AttributeHandle_T;

/**@defgroup BLE_BAS_ASSIGN_HANDLE BLE_BAS_ASSIGN_HANDLE
 * @brief Assigned attribute handles of BLE Battery Service.
 * @{ */
<#if BAS_NOTIFY_ENABLE == true>
#define BAS_END_HDL                                 (BAS_HDL_CCCD_BAT_LEVEL)         /**< The end attribute handle of Battery service. */
</#if>
<#if BAS_NOTIFY_ENABLE == false>
#define BAS_END_HDL                                 (BAS_HDL_CHARVAL_BAT_LEVEL)         /**< The end attribute handle of Battery service. */
</#if>
/** @} */


// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
/**
 *@brief Initialize BLE Battery Service callback function.
 *
 *
 *@return MBA_RES_SUCCESS                    Successfully register BLE Battery service.
 *@return MBA_RES_NO_RESOURCE                Fail to register service.
 *
 */
uint16_t BLE_BAS_Add();


#endif

/**
  @}
 */
