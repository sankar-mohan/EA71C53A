/*******************************************************************************
  Application Data Storage Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app_ds_pair.h

  Summary:
    This file contains the Application Data Storage functions for storing paired device information.

  Description:
    This file contains the Application Data Storage functions for storing paired device information.
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


#ifndef APP_DS_PAIR_H
#define APP_DS_PAIR_H


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>
#include "ble_dm/ble_dm.h"


// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define APP_DS_PAIR_ID_OFFSET                                              0x50         /**< Paired device ID = Device ID + Offset. */

/**@defgroup APP_DS_PAIR_CONN_PRIORITY APP_DS_PAIR_CONN_PRIORITY
 * @brief The definition of the connection priority of paired device
 * @{ */
#define APP_DS_PAIR_HIGHEST_PRIORITY                                       0x01                            /**< The highest connection priority. */
#define APP_DS_PAIR_LOWEST_PRIORITY                                        BLE_DM_MAX_PAIRED_DEVICE_NUM    /**< The lowest connection priority. */
/** @} */




// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
/**@brief The structure contains information about paired device information. */
typedef struct APP_DS_PairedDevInfo_T
{
    uint8_t pairedId;                                   /**< Paired device ID. */
    uint8_t priority;                                   /**< Connection priority of paired device. See @ref APP_DS_PAIR_CONN_PRIORITY. */
} APP_DS_PairedDevInfo_T;




// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
uint16_t APP_DS_PairInit();
uint16_t APP_DS_PairGetStoredInfo(uint8_t devId, APP_DS_PairedDevInfo_T *p_pairedDevInfo);
uint8_t APP_DS_PairFindDevIdByPriority(uint8_t priority);
uint16_t APP_DS_PairUpdateInfo(uint8_t devId);
uint16_t APP_DS_PairDeleteStoredInfo(uint8_t devId);
uint16_t APP_DS_PairDeleteAllStoredInfo(void);

#endif
