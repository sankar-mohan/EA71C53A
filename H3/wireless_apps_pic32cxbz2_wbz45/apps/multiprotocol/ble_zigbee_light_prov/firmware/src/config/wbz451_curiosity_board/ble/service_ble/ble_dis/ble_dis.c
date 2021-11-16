/*******************************************************************************
  BLE Device Information Service Source File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_dis.c

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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <string.h>
#include <stdint.h>
#include "mba_error_defs.h"
#include "gatt.h"
#include "ble_util/byte_stream.h"
#include "ble_dis/ble_dis.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define UUID_MANUFACTURER_NAME                         0x2A29  /**< Manufacturer Name. */
#define UUID_MODEL_NUMBER                              0x2A24  /**< Model Numbr. */
#define UUID_SERIAL_NUMBER                             0x2A25  /**< Serial Number. */
#define UUID_HARDWARE_REVISION                         0x2A27  /**< Hardware Revision. */
#define UUID_FIRMWARE_REVISION                         0x2A26  /**< Firmware Revision. */
#define UUID_SOFTWARE_REVISION                         0x2A28  /**< Software Revision. */
#define UUID_SYSTEM_ID                                 0x2A23  /**< Peripheral Preferred Connection Parameters. */
#define UUID_IEEE_11073_20601_RCDL                     0x2A2A  /**< Peripheral Preferred Connection Parameters. */
#define UUID_PNP_ID                                    0x2A50  /**< Peripheral Preferred Connection Parameters. */

#define DIS_MANU_NAME                                  "MCHP"
#define DIS_MODEL_NUM                                  "Coming Soon"
#define DIS_SERIAL_NUM                                 "Serial Number"
#define DIS_HW_REVISION                                "Hardware Revision"
#define DIS_FW_REVISION                                "0.2.0.0"
#define DIS_SW_REVISION                                "Software"
#define DIS_SYSTEM_ID                                  "System Id"
#define DIS_IEEE_11073_20601                           "NULL"
#define DIS_PNP_ID                                     "NULL"


// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************
const uint8_t g_svcUuidDis[ATT_UUID_LENGTH_2] =                 {UINT16_TO_BYTES(UUID_DEVICE_INFO_SERVICE)};

const uint8_t g_chUuidManuName[ATT_UUID_LENGTH_2] =             {UINT16_TO_BYTES(UUID_MANUFACTURER_NAME)};

const uint8_t g_chUuidModelNum[ATT_UUID_LENGTH_2] =             {UINT16_TO_BYTES(UUID_MODEL_NUMBER)};



const uint8_t g_chUuidFwRev[ATT_UUID_LENGTH_2] =                {UINT16_TO_BYTES(UUID_FIRMWARE_REVISION)};





/* Device Information Service Declaration */
static const uint16_t s_svcUuidDisLen = sizeof(g_svcUuidDis);

/* Manufacture Name Characteristic */
static const uint8_t s_charManuName[] = {(ATT_PROP_READ), UINT16_TO_BYTES(DIS_HDL_CHARVAL_MANU_NAME), UINT16_TO_BYTES(UUID_MANUFACTURER_NAME)};
static const uint16_t s_charManuNameLen = sizeof(s_charManuName);

/* Manufacture Name Value */
static uint8_t s_manuNameVal[]={DIS_MANU_NAME};
static uint16_t s_manuNameValLen = sizeof(s_manuNameVal);

/* Model Number Characteristic */
static const uint8_t s_charModelNum[] = {(ATT_PROP_READ), UINT16_TO_BYTES(DIS_HDL_CHARVAL_MODEL_NUM), UINT16_TO_BYTES(UUID_MODEL_NUMBER)};
static const uint16_t s_charModelNumLen = sizeof(s_charModelNum);

/* Model Number Value */
static uint8_t s_modelNumVal[] = {DIS_MODEL_NUM};
static uint16_t s_modelNumValLen = sizeof(s_modelNumVal);



/* Firmware Revision Characteristic */
static const uint8_t s_charFwRev[] = {(ATT_PROP_READ), UINT16_TO_BYTES(DIS_HDL_CHARVAL_FW_REV), UINT16_TO_BYTES(UUID_FIRMWARE_REVISION)};
static const uint16_t s_charFwRevLen = sizeof(s_charFwRev);

/* Firmware Revision Value */
static uint8_t s_fwRevVal[] = {DIS_FW_REVISION};
static uint16_t s_fwRevValLen = sizeof(s_fwRevVal);






/* Attribute list for Generic Access service */
static GATTS_Attribute_T s_disList[DIS_END_HDL-DIS_START_HDL+1];

/* GAP Service structure */
static GATTS_Service_T s_svcDis;
static uint8_t s_disAttrIndex;

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************

uint16_t BLE_DIS_Add()
{
    uint16_t result;
    
    s_disAttrIndex=0;

    /* Initialize attribute list */
    s_disList[s_disAttrIndex].p_uuid = (uint8_t *) g_gattUuidPrimSvc;
    s_disList[s_disAttrIndex].p_value = (uint8_t *) g_svcUuidDis;
    s_disList[s_disAttrIndex].p_len = (uint16_t *) &s_svcUuidDisLen;
    s_disList[s_disAttrIndex].maxLen = sizeof(g_svcUuidDis);
    s_disList[s_disAttrIndex].settings = 0;
    s_disList[s_disAttrIndex].permissions = PERMISSION_READ;
    s_disAttrIndex++;

    /* Manufacturer Name */
    s_disList[s_disAttrIndex].p_uuid = (uint8_t *) g_gattUuidChar;
    s_disList[s_disAttrIndex].p_value = (uint8_t *) s_charManuName;
    s_disList[s_disAttrIndex].p_len = (uint16_t *) &s_charManuNameLen;
    s_disList[s_disAttrIndex].maxLen = sizeof(s_charManuName);
    s_disList[s_disAttrIndex].settings = 0;
    s_disList[s_disAttrIndex].permissions = PERMISSION_READ;
    s_disAttrIndex++;

    s_disList[s_disAttrIndex].p_uuid = (uint8_t *) g_chUuidManuName;
    s_disList[s_disAttrIndex].p_value = (uint8_t *) s_manuNameVal;
    s_disList[s_disAttrIndex].p_len = (uint16_t *) &s_manuNameValLen;
    s_disList[s_disAttrIndex].maxLen = sizeof(s_manuNameVal);
    s_disList[s_disAttrIndex].settings = 0;
    s_disList[s_disAttrIndex].permissions = PERMISSION_READ;
    s_disAttrIndex++;


    /* Model Number */
    s_disList[s_disAttrIndex].p_uuid = (uint8_t *) g_gattUuidChar;
    s_disList[s_disAttrIndex].p_value = (uint8_t *) s_charModelNum;
    s_disList[s_disAttrIndex].p_len = (uint16_t *) &s_charModelNumLen;
    s_disList[s_disAttrIndex].maxLen = sizeof(s_charModelNum);
    s_disList[s_disAttrIndex].settings = 0;
    s_disList[s_disAttrIndex].permissions = PERMISSION_READ;
    s_disAttrIndex++;

    s_disList[s_disAttrIndex].p_uuid = (uint8_t *) g_chUuidModelNum;
    s_disList[s_disAttrIndex].p_value = (uint8_t *) s_modelNumVal;
    s_disList[s_disAttrIndex].p_len = (uint16_t *) &s_modelNumValLen;
    s_disList[s_disAttrIndex].maxLen = sizeof(s_modelNumVal);
    s_disList[s_disAttrIndex].settings = 0;
    s_disList[s_disAttrIndex].permissions = PERMISSION_READ;
    s_disAttrIndex++;    




    /* Firmware Revision */   
    s_disList[s_disAttrIndex].p_uuid = (uint8_t *) g_gattUuidChar;
    s_disList[s_disAttrIndex].p_value = (uint8_t *) s_charFwRev;
    s_disList[s_disAttrIndex].p_len = (uint16_t *) &s_charFwRevLen;
    s_disList[s_disAttrIndex].maxLen = sizeof(s_charFwRev);
    s_disList[s_disAttrIndex].settings = 0;
    s_disList[s_disAttrIndex].permissions = PERMISSION_READ;
    s_disAttrIndex++;

    s_disList[s_disAttrIndex].p_uuid = (uint8_t *) g_chUuidFwRev;
    s_disList[s_disAttrIndex].p_value = (uint8_t *) s_fwRevVal;
    s_disList[s_disAttrIndex].p_len = (uint16_t *) &s_fwRevValLen;
    s_disList[s_disAttrIndex].maxLen = sizeof(s_fwRevVal);
    s_disList[s_disAttrIndex].settings = 0;
    s_disList[s_disAttrIndex].permissions = PERMISSION_READ;
    s_disAttrIndex++;




    
    /* Initialize Service */
    s_svcDis.p_next = NULL;
    s_svcDis.p_attrList = (GATTS_Attribute_T *) s_disList;
    s_svcDis.p_cccdSetting = NULL;
    s_svcDis.startHandle = DIS_START_HDL;
    s_svcDis.endHandle = DIS_START_HDL+s_disAttrIndex-1;
    s_svcDis.cccdNumber = 0;
    result=GATTS_AddService(&s_svcDis, s_disAttrIndex);
    return result;
}
