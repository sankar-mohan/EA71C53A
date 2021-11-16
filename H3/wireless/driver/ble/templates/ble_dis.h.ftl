/*******************************************************************************
  BLE Device Information Service Header File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_dis.h

  Summary:
    This file contains the BLE Device Information Service functions for application user.

  Description:
    This file contains the BLE Device Information Service functions for application user.
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
 * @addtogroup BLE_DIS BLE DIS
 * @{
 * @brief Header file for the BLE Device Information Service.
 * @note Definitions and prototypes for the BLE Transparent Service stack layer application programming interface.
 */
#ifndef BLE_DIS_H
#define BLE_DIS_H

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************

#define UUID_DEVICE_INFO_SERVICE                       0x180A  /**< Device Information Service. */
/**@defgroup BLE_DIS_ASSIGN_HANDLE BLE_DIS_ASSIGN_HANDLE
 * @brief Assigned attribute handles of BLE Device Information Service.
 * @{ */
#define DIS_START_HDL                               0x0080                                      /**< The start attribute handle of transparent service. */
/** @} */

/**@brief Definition of BLE Device Information Service attribute handle */
typedef enum BLE_DIS_AttributeHandle_T
{
    DIS_HDL_SVC = DIS_START_HDL,           /**< Handle of Primary Service of BLE Device Information Service. */
<#if DIS_MANU_NAME_ENABLE == true>
    DIS_HDL_CHAR_MANU_NAME,                /**< Handle of Manufacture Name characteristic. */
    DIS_HDL_CHARVAL_MANU_NAME,             /**< Handle of Manufacture Name characteristic value. */
</#if>
<#if DIS_MODEL_NUM_ENABLE == true>
    DIS_HDL_CHAR_MODEL_NUM,                /**< Handle of Model Number characteristic. */
    DIS_HDL_CHARVAL_MODEL_NUM,             /**< Handle of Model Number characteristic value. */
</#if>
<#if DIS_SERIAL_NUM_ENABLE == true>
    DIS_HDL_CHAR_SERIAL_NUM,               /**< Handle of Serial Number characteristic. */
    DIS_HDL_CHARVAL_SERIAL_NUM,            /**< Handle of Serial Number characteristic value. */
</#if>
<#if DIS_HW_REV_ENABLE == true>
    DIS_HDL_CHAR_HW_REV,                   /**< Handle of Hardware Revision characteristic. */
    DIS_HDL_CHARVAL_HW_REV,                /**< Handle of Hardware Revision characteristic value. */
</#if>
<#if DIS_FW_REV_ENABLE == true>
    DIS_HDL_CHAR_FW_REV,                   /**< Handle of Firmware Revision characteristic. */
    DIS_HDL_CHARVAL_FW_REV,                /**< Handle of Firmware Revision characteristic value. */
</#if>
<#if DIS_SW_REV_ENABLE == true>
    DIS_HDL_CHAR_SW_REV,                   /**< Handle of Software Revision characteristic. */
    DIS_HDL_CHARVAL_SW_REV,                /**< Handle of Software Revision characteristic value. */
</#if>
<#if DIS_SYSTEM_ID_ENABLE == true>
    DIS_HDL_CHAR_SYSTEM_ID,                /**< Handle of System ID characteristic. */
    DIS_HDL_CHARVAL_SYSTEM_ID,             /**< Handle of System ID characteristic value. */
</#if>
<#if DIS_IEEE_ENABLE == true>
    DIS_HDL_CHAR_IEEE_RCDL,                /**< Handle of IEEE 11073-20601 Regulatory Certification Data List characteristic. */
    DIS_HDL_CHARVAL_IEEE_RCDL,             /**< Handle of IEEE 11073-20601 Regulatory Certification Data List characteristic value. */
</#if>
<#if DIS_PNP_ID_ENABLE == true>
    DIS_HDL_CHAR_PNP_ID,                   /**< Handle of PnP ID characteristic. */
    DIS_HDL_CHARVAL_PNP_ID,                /**< Handle of PnP ID characteristic value. */
</#if>
    DIS_HDL_END
}BLE_DIS_AttributeHandle_T;

/**@defgroup BLE_DIS_ASSIGN_HANDLE BLE_DIS_ASSIGN_HANDLE
 * @brief Assigned attribute handles of BLE Device Information Service.
 * @{ */

#define DIS_END_HDL                                 (DIS_HDL_END-1)         /**< The end attribute handle of transparent service. */
/** @} */

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************

/**
 *@brief Initialize BLE Device Information Service callback function.
 *
 *
 *@return MBA_RES_SUCCESS                    Successfully register BLE Device Information service.
 *@return MBA_RES_NO_RESOURCE                Fail to register service.
 *
 */
uint16_t BLE_DIS_Add();


#endif

/**
  @}
 */
