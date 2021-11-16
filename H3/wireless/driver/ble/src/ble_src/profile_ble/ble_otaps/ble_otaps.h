/*******************************************************************************
  BLE OTA Profile Server Header File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_otaps.h

  Summary:
    This file contains the BLE OTA Profile Server functions for application user.

  Description:
    This file contains the BLE OTA Profile Server functions for application user.
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
 * @addtogroup BLE_OTAPS
 * @{
 * @brief Header file for the BLE OTA Profile Server library.
 * @note Definitions and prototypes for the BLE OTA profile server stack layer application programming interface.
 */
#ifndef BLE_OTAPS_H
#define BLE_OTAPS_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "stack_mgr.h"


// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
/**@addtogroup BLE_OTAPS_DEFINES Defines
 * @{ */

/**@defgroup BLE_OTAS_SUP_IMG_TYPE BLE_OTAS_SUP_IMG_TYPE
 * @brief Supported image types of BLE OTA Service.
 * @{ */
#define BLE_OTAPS_SUP_IMG_TYPE_MCU                (1 << 0)                               /**< MCU image is supported. */
/** @} */


/**@defgroup BLE_OTAPS_FEATURE_DEF BLE_OTAPS_FEATURE_DEF
 * @brief The definition of features.
 * @{ */
#define BLE_OTAPS_FEATURE_SUP_IMG_TYPE            (BLE_OTAPS_SUP_IMG_TYPE_MCU)           /**< The supported image type of BLE OTA profile. */
/** @} */


/**@defgroup BLE_OTAPS_IMG_TYPE_DEF BLE_OTAPS_IMG_TYPE_DEF
 * @brief The definition of each image type.
 * @{ */
#define BLE_OTAPS_IMG_TYPE_MCU                    0x01                                   /**< MCU firmaware */
/** @} */

/**@defgroup BLE_OTAPS_COMP_OP_DEF BLE_OTAPS_COMP_OP_DEF
 * @brief The definition of complete event operation.
 * @{ */
#define BLE_OTAPS_COMP_OP_CONFIRM                 0x00                                   /**< Client requests to confirm the update is successful or failed. Application must call @ref BLE_OTAPS_ConfirmResponse to response. */
#define BLE_OTAPS_COMP_OP_STOP                    0x01                                   /**< Client requests to stop the update procedure. */
/** @} */


/**@} */ //BLE_OTAPS_DEFINES


/**@addtogroup BLE_OTAPS_ENUMS Enumerations
 * @{ */

/**@brief Enumeration type of BLE OTA profile callback events. */

typedef enum BLE_OTAPS_EventId_T
{
    BLE_OTAPS_EVT_UPDATE_REQ,                               /**< Request to start update procedure. See @ref BLE_OTAPS_EvtUpdateReq_T for the event details. */
    BLE_OTAPS_EVT_UPDATE_START,                             /**< Start to update one category image. See @ref BLE_OTAPS_EvtUpdateStart_T for the event details. */
    BLE_OTAPS_EVT_UPDATE,                                   /**< Update image fragments. See @ref BLE_OTAPS_EvtUpdate_T for the event details. */
    BLE_OTAPS_EVT_COMPLETE,                                 /**< Procedure is finished. See @ref BLE_OTAPS_EvtComplete_T for the event details. */
    BLE_OTAPS_EVT_RESET_REQ                                 /**< Update is complete, request to reset system. */
}BLE_OTAPS_EventId_T;

/**@} */ //BLE_OTAPS_ENUMS

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
/**@addtogroup BLE_OTAPS_STRUCTS Structures
 * @{ */

/**@brief The structure contains information about BLE OTA profile firmware update request event parameters. \n
* It should be used to get details when event @ref BLE_OTAPS_EVT_UPDATE_REQ is generated */
typedef struct BLE_OTAPS_EvtUpdateReq_T
{
    uint16_t connHandle;                                    /**< The connection handle to request firmware update */
    uint32_t mcuImageSize;                                  /**< The size of MCU firmware to update */
    uint32_t verMcu;                                        /**< MCU firmware version information */    
} BLE_OTAPS_EvtUpdateReq_T;

/**@brief The structure contains information about BLE OTA profile firmware update start event parameters. \n
* It should be used to get details when event @ref BLE_OTAPS_EVT_UPDATE_START is generated */
typedef struct BLE_OTAPS_EvtUpdateStart_T
{
    uint8_t  imageType;                                     /**< The type of image to update, see @ref BLE_OTAPS_IMG_TYPE_DEF. */
} BLE_OTAPS_EvtUpdateStart_T;

/**@brief The structure contains information about BLE OTA profile update event parameters. \n
* It should be used to get details when event @ref BLE_OTAPS_EVT_UPDATE is generated */
typedef struct BLE_OTAPS_EvtUpdate_T
{
    uint16_t  length;                                       /**< The length of image fragment be updated. */
} BLE_OTAPS_EvtUpdate_T;

/**@brief The structure contains information about BLE OTA profile end event parameters. \n
* It should be used to get details when event @ref BLE_OTAPS_EVT_COMPLETE is generated */
typedef struct BLE_OTAPS_EvtComplete_T
{
    uint8_t  operation;                                     /**< The operation of the finish process. 0x00: Success, 0x01: Failure*/
} BLE_OTAPS_EvtComplete_T;



/**@brief Union containing details regarding a BLE OTA profile event received by the application. */
typedef union
{
    BLE_OTAPS_EvtUpdateReq_T            evtUpdateReq;       /**< See @ref BLE_OTAPS_EvtUpdateReq_T */
    BLE_OTAPS_EvtUpdateStart_T          evtUpdateStart;     /**< See @ref BLE_OTAPS_EvtUpdateStart_T */    
    BLE_OTAPS_EvtUpdate_T               evtUpdate;          /**< See @ref BLE_OTAPS_EvtUpdate_T */        
    BLE_OTAPS_EvtComplete_T             evtComplete;        /**< See @ref BLE_OTAPS_EvtComplete_T */        
} BLE_OTAPS_EventField_T;


/**@brief The structure contains information about type of BLE OTA profile event received and associated details.\n
* It is used in BLE OTA profile callback function to get the details of the received event.*/
typedef struct  BLE_OTAPS_Event_T
{
    uint8_t                             eventId;            /**< Event ID. See @ref BLE_OTAPS_EventId_T.  */
    BLE_OTAPS_EventField_T              eventField;         /**< See @ref BLE_OTAPS_EventField_T */
} BLE_OTAPS_Event_T;

/**@} */ //BLE_OTAPS_STRUCTS

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
/**@addtogroup BLE_OTAPS_FUNS Functions
 * @{ */



/**@brief BLE OTA profile callback function prototype. */
typedef void(*BLE_OTAPS_ProcessRoutine)(BLE_OTAPS_Event_T *p_event);

/**
 *@brief Initialize BLE OTA Profile.
 *
 * @return MBA_RES_SUCCESS          Success to add a service to the service table. 
 * @return MBA_RES_FAIL             Fail to add a service to the service table.
 * @return MBA_RES_OOM              No available memory. 
 *
 */
uint16_t BLE_OTAPS_Init();

/**
 *@brief This API is use to initialize BLE OTA profile callback function and service.\n
 *       Application must call this API before starting BLE OTA procedure. 
 *
 *
 *@param[in] bleOtapsRoutine        Application/profile callback function pointer.
 *
 */
void BLE_OTAPS_EventRegistration(BLE_OTAPS_ProcessRoutine bleOtapsRoutine);

/**
 *@brief This API is use to enable BLE OTA profile.\n
 *       Application must call this API before starting BLE OTA procedure. 
 *
 * @return MBA_RES_SUCCESS          Successfully enable BLE OTA profile.
 */
uint16_t BLE_OTAPS_Enable();

/**
 *@brief This API is use to disable BLE OTA profile.
 *       After applicaton allowed OTA procedure by @ref BLE_OTAPS_UpdateReqResponse, the BLE OTA procedue is in progress.
 *       If application needs to stop BLE OTA procedure, it must call this API.
 *
 *
 * @return MBA_RES_SUCCESS          Successfully disable BLE OTA profile.
 */
uint16_t BLE_OTAPS_Disable();


/**
 *@brief This API is use to set IV value and Key used for image encryption.\n
 *       Application must call this API before starting BLE OTA procedure. 
 *
 *@param[in] p_iv                   The IV value.
 *@param[in] p_key                  The encryption key.
 *
 * @return MBA_RES_SUCCESS          Successfully set IV value and Key.
 * @return MBA_RES_BAD_STATE        Failure because the BLE OTA is in progress.
 */
uint16_t BLE_OTAPS_SetEncrytionInfo(uint8_t * p_iv, uint8_t * p_key);

/**
 *@brief This API is use to response @ref BLE_OTAPS_EVT_UPDATE_REQ event. Application must determinate if allow to start BLE OTA update procedure.\n
 *
 *@param[in] connHandle             Handle of the connection to send response. 
 *@param[in] isAllow                false: not allow to start update. true: allow to start update.
 *
 *@return MBA_RES_SUCCESS           Successfully send response.
 *@return MBA_RES_OOM               Memory not available to send response.
 *@return MBA_RES_INVALID_PARA      Invalid parameters. Connection handle is not valid
 *
 */
uint16_t BLE_OTAPS_UpdateReqResponse(uint16_t connHandle, bool isAllow);

/**
 *@brief This API is use to response @ref BLE_OTAPS_EVT_COMPLETE event if operation is @ref BLE_OTAPS_COMP_OP_CONFIRM.
 *       Application must determinate if the update is successful.
 *
 *@param[in] success                false: update is failed. true: update is successful.
 *
 *@return MBA_RES_SUCCESS           Successfully send response.
 *@return MBA_RES_OOM               Memory not available to send response.
 *
 */
uint16_t BLE_OTAPS_ConfirmResponse(bool success);


/**@brief Function for handling BLE events.
 *       This API should be called in the application while caching BLE events
 *
 * @param[in] p_stackEvent          Pointer to BLE events buffer.
 *
*/
void BLE_OTAPS_BleEventHandler(STACK_Event_T *p_stackEvent);


/**@} */ //BLE_TRPS_FUNS


#endif

/**
  @}
*/


