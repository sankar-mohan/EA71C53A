/*******************************************************************************
  BLE Battery Service Source File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_bas.c

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
#include "ble_bas/ble_bas.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define UUID_BATTERY_SERVICE                           0x180F  /**< Battery Service. */
#define UUID_BATTERY_LEVEL                             0x2A19  /**< Battery Level. */


// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************

const uint8_t s_svcUuidBas[ATT_UUID_LENGTH_2] =                 {UINT16_TO_BYTES(UUID_BATTERY_SERVICE)};
const uint8_t s_chUuidBatLevel[ATT_UUID_LENGTH_2] =             {UINT16_TO_BYTES(UUID_BATTERY_LEVEL)};

/* Battery Service Declaration */
static uint16_t s_svcUuidBasLen = sizeof(s_svcUuidBas);

/* Battery Level Characteristic Declaration */
<#if BAS_NOTIFY_ENABLE == true>
static const uint8_t s_charBatLevel[] = {(ATT_PROP_READ | ATT_PROP_NOTIFY), UINT16_TO_BYTES(BAS_HDL_CHARVAL_BAT_LEVEL), UINT16_TO_BYTES(UUID_BATTERY_LEVEL)};
</#if>
<#if BAS_NOTIFY_ENABLE == false>
static const uint8_t s_charBatLevel[] = {(ATT_PROP_READ ), UINT16_TO_BYTES(BAS_HDL_CHARVAL_BAT_LEVEL), UINT16_TO_BYTES(UUID_BATTERY_LEVEL)};
</#if>
static const uint16_t s_charBatLevelLen = sizeof (s_charBatLevel);

/* Battery Level Characteristic Value */
static uint8_t s_batLevelVal[1] = {0};
static uint16_t s_batLevelValLen = 1;

<#if BAS_NOTIFY_ENABLE == true>
/* Battery Level Client Characteristic Configuration Descriptor */
static uint8_t s_descCccBatLevel[] = {UINT16_TO_BYTES(0x0000)};
static const uint16_t s_descCccBatLevelLen = sizeof (s_descCccBatLevel);
</#if>

/* Attribute list for Battery service */
static GATTS_Attribute_T s_basList[] = {
    /* Service Declaration */
    {
        (uint8_t *) g_gattUuidPrimSvc,
        (uint8_t *) s_svcUuidBas,
        (uint16_t *) & s_svcUuidBasLen,
        sizeof (s_svcUuidBas),
        0,
        PERMISSION_READ
    },
    /* Characteristic Declaration */
    {
        (uint8_t *) g_gattUuidChar,
        (uint8_t *) s_charBatLevel,
        (uint16_t *) & s_charBatLevelLen,
        sizeof (s_charBatLevel),
        0,
        PERMISSION_READ
    },
    /* Characteristic Value */
    {
        (uint8_t *) s_chUuidBatLevel,
        (uint8_t *) s_batLevelVal,
        (uint16_t *) & s_batLevelValLen,
        sizeof(s_batLevelVal),
        0,
        PERMISSION_READ
    },
<#if BAS_NOTIFY_ENABLE == true>    
    /* Client Characteristic Configuration Descriptor */
    {
        (uint8_t *) g_descUuidCcc,
        (uint8_t *) s_descCccBatLevel,
        (uint16_t *) & s_descCccBatLevelLen,
        sizeof (s_descCccBatLevel),
        (SETTING_CCCD),
        (PERMISSION_READ | PERMISSION_WRITE)
    },
</#if>    
};

<#if BAS_NOTIFY_ENABLE == true>
static const GATTS_CccdSetting_T s_basCccdSetting[] = 
{
    {BAS_HDL_CCCD_BAT_LEVEL, (NOTIFICATION)}
};
</#if>
/* Battery Service structure */
static GATTS_Service_T s_svcBas = 
{
    NULL,
    (GATTS_Attribute_T *) s_basList,
    <#if BAS_NOTIFY_ENABLE == true>
    (GATTS_CccdSetting_T const *)s_basCccdSetting,
    </#if>
    <#if BAS_NOTIFY_ENABLE == false>
    NULL,
    </#if>	
    BAS_START_HDL,
    BAS_END_HDL,
    <#if BAS_NOTIFY_ENABLE == true>
    1
    </#if>
    <#if BAS_NOTIFY_ENABLE == false>
    0
    </#if>
};

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************

uint16_t BLE_BAS_Add()
{
    return GATTS_AddService(&s_svcBas, (BAS_END_HDL - BAS_START_HDL + 1));
}
