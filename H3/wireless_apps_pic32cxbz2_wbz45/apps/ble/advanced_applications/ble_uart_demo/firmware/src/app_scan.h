/*******************************************************************************
  Application Timer Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app_scan.h

  Summary:
    This file contains the Application scan functions for this project.

  Description:
    This file contains the Application scan functions for this project.
    Including the Set/Stop/Reset timer and timer expired handler.
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


#ifndef APP_SCAN_H
#define APP_SCAN_H


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>



// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define APP_SCAN_COMPANY_ID_MCHP            0x00CD
#define APP_SCAN_SERVICE_UUID_MCHP          0xFEDA
#define APP_SCAN_ADD_DATA_CLASS_BYTE        0xFF

/**@defgroup APP_ADV_PROD_TYPE APP_ADV_PROD_TYPE
* @brief The definition of the product type in the advertising data
* @{ */
#define APP_SCAN_PROD_TYPE_BLE_UART         0x01           /**< Product Type: BLE UART */
/** @} */

#define APP_SCAN_MIN_RSSI_60CM       ((int8_t)0 - 40)

// *****************************************************************************
// *****************************************************************************
// Section: Global Variables
// *****************************************************************************
// *****************************************************************************
extern BLE_GAP_EvtAdvReport_T g_appScanAdvReport_t;
#ifdef APP_EXT_SCAN_ENABLE
extern BLE_GAP_EvtExtAdvReport_T g_appScanExtAdvReport_t;
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
void APP_SCAN_Init(void);
void APP_SCAN_Start(void);
void APP_SCAN_Stop(void);
void APP_SCAN_ConnectDevice(void);


#endif
