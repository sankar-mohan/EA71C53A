/*******************************************************************************
  ZLL Security Header File

  Company:
    Microchip Technology Inc.

  File Name:
    N_Security.h

  Summary:
    This file contains the ZLL Security functions.

  Description:
    This file contains the ZLL Security functions.
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


#ifndef N_SECURITY_H
#define N_SECURITY_H

/***************************************************************************************************
* INCLUDE FILES
***************************************************************************************************/

#include "zllplatform/infrastructure/N_Types/include/N_Types.h"

/***************************************************************************************************
* C++ DECLARATION WRAPPER
***************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************************************
* EXPORTED TYPES
***************************************************************************************************/

typedef uint8_t N_Security_Key_t[16];
typedef void (*N_Security_DoneCallback_t)(void);

/***************************************************************************************************
* EXPORTED MACROS AND CONSTANTS
***************************************************************************************************/

/***************************************************************************************************
* EXPORTED FUNCTIONS
***************************************************************************************************/

/** Fills a buffer with random data.
    \param[in] pBuffer Pointer to buffer to fill with random data
    \param[in] bufferLength Number of bytes to fill with random data
*/
void N_Security_GetRandomData(uint8_t* pBuffer, uint8_t bufferLength);

/** Gets a random 32 bits TransactionID.
    \returns The generated transaction ID.
*/
uint32_t N_Security_GetTransactionId(void);

/** Gets a random 32 bits ResponseID.
    \returns The generated response ID.
*/
uint32_t N_Security_GetResponseId(void);

/** Generates a random network key.
    \param[out] networkKey The new network key
*/
void N_Security_GenerateNewNetworkKey(N_Security_Key_t networkKey);

/** Encrypts a network key. The returned key is ready for transmission over Interpan.
    \param[in] keyIndex Key index
    \param[in] transactionId The Transaction ID.
    \param[in] responseId The Response ID.
    \param[in] unencKey The unencrypted network key
    \param[out] encKey Buffer to keep the encrypted NWK key
    \param[in] done Encrypt done callback
*/
void N_Security_EncryptNetworkKey(uint8_t keyIndex, uint32_t transactionId, uint32_t responseId,
                                  N_Security_Key_t unencKey, N_Security_Key_t encKey, N_Security_DoneCallback_t done);

/** Decrypts an encrypted network key received from Interpan. The returned key is ready to be used in the network.
    \param[in] keyIndex Key index
    \param[in] transactionId The Transaction ID.
    \param[in] responseId The Response ID.
    \param[in] encKey The encrypted network key
    \param[out] unencKey - Storage to put the decrypted network key into;
    \param[in] done Decrypt done callback
*/
void N_Security_DecryptNetworkKey(uint8_t keyIndex, uint32_t transactionId, uint32_t responseId,
                                  N_Security_Key_t encKey, N_Security_Key_t unencKey, N_Security_DoneCallback_t done);

/** Gets the Distributed TrustCenter Link Key (e.g. the ZLL LinkKey).
    \param[out] pKey Location to write the LinkKey to (16 bytes)
    When proper ZLL security material is available, the production key is returned, else the certification key is returned.
*/
void N_Security_GetDistributedTrustCenterLinkKey(uint8_t* pKey, N_Security_DoneCallback_t done);

  /** Gets the Centralized TrustCenter Link Key (e.g. the HA LinkKey).
    \param[out] pKey Location to write the LinkKey to (16 bytes)
    When proper ZLL security material is available, the production key is returned, else the certification key is returned.
*/
void N_Security_GetCentralizedTrustCenterLinkKey(uint8_t* pKey, N_Security_DoneCallback_t done);

/** Gets the mask of supported keys.
    \returns The key bit-mask of the device
*/
uint16_t N_Security_GetKeyBitMask(void);

/** Check if the given keymask is compatible with our keymask.
    \param othersBitMask Other key bit-mask
    \return TRUE if compatible
*/
bool N_Security_IsCompatibleWithKeyMask(uint16_t othersBitMask);

/** Find the shared key index between our keymask and another keymask.
    \param othersBitMask Other key bit-mask
    \return The shared key index
    \note Keys must be compatible, see \ref N_Security_IsCompatibleWithKeyMask .
*/
uint8_t N_Security_FindSharedKeyIndex(uint16_t othersBitMask);

/** En/decrypt 16-byte data using a secret and unknown key.
    \param pIn [in] 16-byte input data
    \param pOut [in] 16-byte output data
    \param done [in] Callback on operation completion
    \note pIn==pOut is allowed
*/
void N_Security_Crypt16(const uint8_t* pIn, uint8_t* pOut, N_Security_DoneCallback_t done);

/***************************************************************************************************
* END OF C++ DECLARATION WRAPPER
***************************************************************************************************/

#ifdef __cplusplus
}
#endif

#endif // N_SECURITY_H
