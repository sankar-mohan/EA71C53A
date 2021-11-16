/*******************************************************************************
  ZLL Address Map Header File

  Company:
    Microchip Technology Inc.

  File Name:
    N_AddressMap.h

  Summary:
    This file contains the ZLL Address Map functions.

  Description:
    This file contains the ZLL Address Map functions.
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


#ifndef N_ADDRESSMAP_H
#define N_ADDRESSMAP_H

/***************************************************************************************************
* INCLUDE FILES
***************************************************************************************************/

#include "N_Address.h"
#include "N_Types.h"

/***************************************************************************************************
* C++ DECLARATION WRAPPER
***************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************************************
* EXPORTED TYPES
***************************************************************************************************/


/***************************************************************************************************
* EXPORTED MACROS AND CONSTANTS
***************************************************************************************************/


/***************************************************************************************************
* EXPORTED FUNCTIONS
***************************************************************************************************/
/** Look up the index and extended address of a device by network address in the address map
    \param networkAddress [in] The network address of that device
    \param pExtendedAddress [out] The extended address of that device
    \returns TRUE if the entry exists, otherwise FALSE

    \note The out parameters of this function should not be used if the return value is FALSE
*/
bool N_AddressMap_LookupByNetworkAddress(uint16_t networkAddress, N_Address_Extended_t* pExtendedAddress);

/** Look up the index and network address of a device by the extended address in the address map
    \param pExtendedAddress [in] The extended address of that device
    \param pNetworkAddress [out] The network address of that device
    \returns TRUE if the entry exists, otherwise FALSE

    \note The out parameters of this function should not be used if the return value is FALSE
*/
bool N_AddressMap_LookupByExtendedAddress(N_Address_Extended_t* pExtendedAddress, uint16_t* pNetworkAddress);

/** Update the address map with these values or create a new entry if it does not exist
    \param networkAddress [in] The network address of that device
    \param pExtendedAddress [in] The extended address of that device
    \returns TRUE if the entry exists, otherwise FALSE

    \note This function will automatically add a reference
    \note The out parameters of this function should not be used if the return value is FALSE
*/
bool N_AddressMap_UpdateNetworkAddress(uint16_t networkAddress, N_Address_Extended_t* pExtendedAddress);

/***************************************************************************************************
* END OF C++ DECLARATION WRAPPER
***************************************************************************************************/

#ifdef __cplusplus
}
#endif

#endif // N_ADDRESSMAP_H
