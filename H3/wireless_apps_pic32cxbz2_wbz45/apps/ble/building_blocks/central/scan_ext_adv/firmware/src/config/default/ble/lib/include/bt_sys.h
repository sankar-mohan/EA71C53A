/*******************************************************************************
  Bluetooth System Header File

  Company:
    Microchip Technology Inc.

  File Name:
    bt_sys.h

  Summary:
    This file contains the BLE System functions for the project.

  Description:
    This file contains the BLE System functions for the project. The "BT_SYS_Init" 
    function shall be called in the "SYS_Initialize" function to initialize the 
    this modules in the system. And the "BM_Task"  shall be called in the "SYS_Tasks".
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

#ifndef BT_SYS_H
#define BT_SYS_H

#include <stdbool.h>
#include <stdint.h>

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define BT_SYS_DEV_ADDR_LEN             7

// *****************************************************************************
// *****************************************************************************
// SSection: Data Types
// *****************************************************************************

/**@brief Bluetooth system configuration. */
typedef struct BT_SYS_Cfg_T
{
    uint8_t     addrValid:1;                        /**< Set TRUE if devAddr field is valid. */
    uint8_t     rssiOffsetValid:1;                  /**< Set TRUE if rssiOffset field is valid. */
    uint8_t     antennaGainValid:1;                 /**< Set TRUE if antennaGain field is valid. */

    uint8_t     devAddr[BT_SYS_DEV_ADDR_LEN];       /**< Device address. */
    int8_t      rssiOffset;                         /**< RSSI offset. */
    int8_t      antennaGain;                        /**< Antenna gain. */
} BT_SYS_Cfg_T;

/**@brief BT SYS Asssert Callback
 *
 * @param[in] line                                  The line which assert occurred.
 * @param[in] p_file                                The file which assert occurred.
 *
 * @retval None
 */
typedef void (*BT_SYS_ErrCb)(uint32_t line, const char *p_file);


typedef void (*BT_SYS_LogCb)(uint16_t logType, uint16_t logId, uint16_t paramsLength, uint16_t payloadLength, uint8_t *p_logPayload, uint8_t *p_returnParams);

/**@brief Bluetooth system initialization. */
typedef struct BT_SYS_Option_T
{
    uint32_t    cmnMemSize;                         /**< Common memory size. */
    uint32_t    *p_sramVecorTable;                  /**< Vector table. */
    BT_SYS_ErrCb btErrCb;                           /**< BT system error call back. */
    BT_SYS_LogCb btLogCb;                           /**< BT system log call back. */
} BT_SYS_Option_T;
// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************

/**@brief BT Init Entry
 *
 * @param[in] p_reqQueueHandle                  OS handle for Bluetooth internal task.
 * @param[in] p_osalApiList                     OSAL api list.
 * @param[in] p_option                          Initialization option.
 * @param[in] p_cfg                             System configuration.
 *
 * @retval None
 */
/*************************************************************************************************/
void BT_SYS_Init(void *p_reqQueueHandle, void *p_osalApiList, BT_SYS_Option_T *p_option, BT_SYS_Cfg_T *p_cfg);


/**@brief The API is used to retrieve BT version.
 *
 * @param[out] p_version                        Point to version buffer.
 *
 * @retval None
 */
void BT_SYS_ReadVersion(uint32_t *p_version);

/**@brief The API is used to query if BT is ready for idle.
 *
 * @param[out] None
 *
 * @retval A boolean value, True means ready for idle
 */
bool BT_SYS_Ready4Idle();

/**@brief The API is used to query if BT is slept
 *
 * @param[out] None
 *
 * @retval A boolean value, True means BT is slept 
 */
bool BT_SYS_GetSleepMode();

/**@brief The API is used to request BT enter sleep
 *
 * @param[in] ulRtcFrequency             RTC frequency
 * @param[in] ulRtcCurrentCnt            RTC current count
 *
 * @retval A boolean value, True means BT is ready for idle and enter sleep
 *                          False means BT is already slept or BT is not ready for idle
 */
bool BT_SYS_EnterSleepMode(uint32_t ulRtcFrequency, uint32_t ulRtcCurrentCnt);

/**@brief The API is used to query if BT allow System enter sleep
 *
 * @param[in] ulRtcFrequency             RTC frequency
 * @param[in] ulRtcCurrentCnt            RTC current count
 *
 * @retval A boolean value, True means BT is slept and BT is ready for System enter sleep
 *                          False means BT is not slept or BT is not ready for System enter sleep
 */
bool BT_SYS_AllowSystemSleep(uint32_t ulRtcFrequency, uint32_t ulRtcCurrentCnt);

/**@brief The API is used to request if BT can suspend RF
 *
 * @param[out] None
 *
 * @retval A boolean value, True means BT can suspend RF
 */
bool BT_SYS_RfSuspendReq(uint8_t enable);



/**@brief BM_Task
 *
 * @param[in] p_param Pointer that will be used as the parameter for the task
 *
 * @retval None
 */
/*************************************************************************************************/
void BM_Task(void *p_param);

#endif//BT_SYS_H
