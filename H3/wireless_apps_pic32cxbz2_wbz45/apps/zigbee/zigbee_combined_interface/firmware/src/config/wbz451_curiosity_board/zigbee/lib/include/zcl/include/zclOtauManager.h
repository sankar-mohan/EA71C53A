/*******************************************************************************
  Zigbee Cluster Library OTAU Manager Header File

  Company:
    Microchip Technology Inc.

  File Name:
    zclOtauManager.h

  Summary:
    Declaration of the OTAU manager interface.

  Description:
    Declaration of the OTAU manager interface.
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

#if APP_USE_OTAU == 1

#ifndef _ZCLOTAUMANAGER_H
#define _ZCLOTAUMANAGER_H
/******************************************************************************
                        Includes section
******************************************************************************/
#include <zcl/include/zclOTAUCluster.h>
#include <ofdMemoryDriver.h>
#include <private/zclDbg.h>
#include <zdo/include/zdo.h>
/******************************************************************************
                        Defines section
******************************************************************************/
#define KEY_LENGTH            (16U)
#define IV_LENGTH             (16U)

#define OTAU_MAX_MISSING_BLOCKS           6
#define OTAU_MISSING_BLOCK_EMPTY_VALUE    0xff

#if APP_DEVICE_OTAU_LOGGING == 1 && APP_USE_OTAU == 1
  #define appOtauPrintf(...) appSnprintf(__VA_ARGS__)
#else
  #define appOtauPrintf(...) do{}while(0)
#endif

#define OTAU_CONTEXT_BREAK_QUICK_TIMEOUT     20

/******************************************************************************
                           Types section
******************************************************************************/
typedef enum
{
  OTAU_STOPPED_STATE,
  OTAU_INIT_OFD_STATE,
  OTAU_WAIT_TO_DISCOVER_STATE,
  OTAU_GET_MATCH_DESC_BROADCAST_STATE,
  OTAU_GET_MATCH_DESC_UNICAST_STATE,
  OTAU_GET_IEEE_ADDR_STATE,
  OTAU_GET_NWK_ADDR_STATE,
  OTAU_GET_LINK_KEY_STATE,
  OTAU_WAIT_TO_QUERY_STATE,
  OTAU_QUERY_FOR_IMAGE_STATE,
  OTAU_START_DOWNLOAD_STATE,
  OTAU_GET_IMAGE_BLOCKS_STATE,
  OTAU_GET_IMAGE_PAGES_STATE,
  OTAU_GET_MISSED_BLOCKS_STATE,
  OTAU_WAIT_TO_UPGRADE_STATE
} ZclOtauClientStateMachine_t;

typedef union
{
  ZDO_ZdpReq_t          zdpOtauReq;
#if (defined _LINK_SECURITY_) && (!defined _LIGHT_LINK_PROFILE_)
  APS_RequestKeyReq_t   apsKeyReq;
#endif // (defined _LINK_SECURITY_) && (!defined _LIGHT_LINK_PROFILE_)
  ZCL_Request_t         zclCommandReq;
} OtauReqMemory_t;

typedef union
{
  ZCL_OtauQueryNextImageReq_t   uQueryNextImageReq;
  ZCL_OtauImageBlockReq_t       uImageBlockReq;
  ZCL_OtauImagePageReq_t        uImagePageReq;
  ZCL_OtauUpgradeEndReq_t       uUpgradeEndReq;
} OtauZclReqMemory_t;

/* This structure always should consist actual values to
 * continue broken uploading. */
#if OTAU_BITCLOUD_LEGACY_SUPPORT == 1

typedef struct
{
/* Size of internal subimage within main image
 * (for example size of eeprom area) */
  uint32_t  imageInternalLength;
/* Amount bytes were received from internal information structute
 * about the internal subimage.  Should be 8 bytes. 4 byte mcu start
 * address of the subimage. 4 byte size of the subimage. */
  uint8_t   internalAddressStatus;
/* Requested offset from the current file. */
  uint32_t  currentFileOffset;
/* Size of requested data. */
  uint8_t   currentDataSize;
/* Reminder of the not received data. */
  uint32_t  imageRemainder;
/* Address of the page begin. Address form main image. */
  uint32_t  imagePageOffset;
/* Page size of the last Image Page request. */
  uint16_t lastPageSize;
} OtauImageAuxVar_t;

typedef struct
{
  uint8_t         imageBlockData[OFD_BLOCK_SIZE];
  uint32_t        imageSize;
  uint8_t        *imagePageData;
} ZclOtauClientImageBuffer_t;
#else

typedef struct
{
  /* Total Image Size to be downloaded/Currently Getting Downloaded */
  uint32_t  totalImageLength;
  /* Request(ed) File Offset. */
  uint32_t  requestFileOffset;

#if APP_SUPPORT_OTAU_PAGE_REQUEST == 1
  /* Offset address of the missed Page being received*/
  uint32_t  currentPageOffset;
  uint8_t  totalReceivedBytesInPage;
  /*Contains all missed block offsests within that page*/
  uint8_t   missedBlockOffset[OTAU_MAX_MISSING_BLOCKS];
  /* Current offset within the page */
  uint8_t  imageBlockOffsetInPage;
  bool      isMissedBlock;
#endif
/* Size of block request(ed) */
  uint8_t   requestBlockSize;
} OtauImageAuxVar_t;

typedef struct
{
  uint32_t        receivedImgBlockSize;
#if APP_SUPPORT_OTAU_PAGE_REQUEST == 1
  uint8_t*        receivedBlockPayload;
#else
  uint8_t         receivedImgBlock[OTAU_MAX_REQ_BLOCK_SIZE];
#endif
} ZclOtauClientImageBuffer_t;

#endif

typedef struct
{ /* memory for storage of server discovery result */
  struct
  {
    void *next;
  } service;

  bool        busy;
  Endpoint_t  serverEndpoint;
  ShortAddr_t serverShortAddress;
  ExtAddr_t   serverExtAddress;
} ZclOtauDiscoveryResult_t;

typedef struct
{
  struct
  {
    void *next;
  } service;

  bool                           busy;
  uint8_t                        id;
  uint16_t                       pageReminderSize;
  ZCL_Addressing_t               addressing;
  ZCL_Request_t                  zclCommandReq;

  union
  {
    ZCL_OtauQueryNextImageReq_t  queryNextImageReq;
    ZCL_OtauImageBlockReq_t      imageBlockReq;
    ZCL_OtauUpgradeEndReq_t      upgradeEndReq;
  };
  union
  {
    ZCL_OtauQueryNextImageResp_t queryNextImageResp;
    ZCL_OtauImageBlockResp_t     imageBlockResp;
    ZCL_OtauUpgradeEndResp_t     upgradeEndResp;
  };
#if APP_SUPPORT_OTAU_PAGE_REQUEST == 1
  ZCL_OtauImagePageReq_t         imagePageReq;
#endif
  
} ZclOtauServerTransac_t;

#if OTAU_BITCLOUD_LEGACY_SUPPORT == 1
typedef struct
{
  // reqMem contains the memory structure for ZDO, APS and ZCL commands
  OtauReqMemory_t              reqMem;

  // zclReqMem contains the memory structure for request commands supported by the client side OTAU cluster
  OtauZclReqMemory_t           zclReqMem;

  /// pageRequestTimer is used for timing the first block resp for the page request
  // also i plan to use this for timing the image page req completion turnaround
  HAL_AppTimer_t               pageRequestTimer;

  // otauParam holds the image blocks/page and size received over the air
  ZclOtauClientImageBuffer_t   otauParam;

  // imageAuxParam holds the offset tracking, image breakage and current data size
  OtauImageAuxVar_t            imageAuxParam;

  // memParam holds the memory structure to facilitate operation of OTAU flash drive
  OFD_MemoryAccessParam_t      memParam;

  // newFirmwareVersion holds the version information of currently downloading image
  ZCL_OtauFirmwareVersion_t    newFirmwareVersion;

  // numbers of servers that responded during the discovery procedure
  uint8_t                      discoveredServerAmount;

  // OTAU server extended address
  ExtAddr_t                    otauServer;

  // information related to the discovered servers
  ZclOtauDiscoveryResult_t    *discoveredServerMem;

  // bit mask pointer to uint8_t to track the missed bytes in a page download
  uint8_t                     *missedBytesMask;
  // Otau Transcation retry count
  uint8_t                       retryCount;
  // miscellaneous help information
  struct
  {
    uint8_t                    blockRequest : 1;
    uint8_t                    missedBytesGetting : 1;
    uint8_t                    firstStart : 1;
    uint8_t                    blockRequestDelayOn : 1;
  };
  // imgtype field populated/read in the header/eeprom
  ZCL_OtauImageType_t          eepromImgType;
/* Sequence number used for OTAU request */
  uint8_t otauReqSeqNum;
} ZCL_OtauClientMem_t;

#else
typedef struct
{
  // reqMem contains the memory structure for ZDO, APS and ZCL commands
  OtauReqMemory_t              reqMem;

  // zclReqMem contains the memory structure for request commands supported by the client side OTAU cluster
  OtauZclReqMemory_t           zclReqMem;
  /// blockResponseWaitTimer is used for timing the first block resp for the page request
  // also i plan to use this for timing the image page req completion turnaround
  HAL_AppTimer_t                blockResponseWaitTimer;

  // otauParam holds the image blocks/page and size received over the air
  ZclOtauClientImageBuffer_t   otauParam;

  // imageAuxParam holds the offset tracking, image breakage and current data size
  OtauImageAuxVar_t            imageAuxParam;

  // ofdParam holds the memory structure to facilitate operation of OTAU flash drive
  OFD_MemoryAccessParam_t      ofdParam;

  // newFirmwareVersion holds the version information of currently downloading image
  ZCL_OtauFirmwareVersion_t    newFirmwareVersion;

  // information related to the discovered servers
  ZclOtauDiscoveryResult_t    *discoveredServerMem;

  // imgtype field populated/read in the header/eeprom
  ZCL_OtauImageType_t          eepromImgType;

  ZCL_OtauImageType_t          downloadingImgType;

  // numbers of servers that responded during the discovery procedure
  uint8_t                      discoveredServerAmount;
  
  // Otau Transcation retry count
  uint8_t                       retryCount;

  // Upgrade End Status
  ZCL_Status_t otauUpgradeEndStatus;
  // Sequence number used for OTAU request
  uint8_t otauReqSeqNum;
} ZCL_OtauClientMem_t;

#endif

typedef struct
{
  ZCL_Request_t            unsolicitedReq;
  ZCL_OtauUpgradeEndResp_t unsolicitedUpgradeEndResp;
  ZCL_OtauImageNotify_t    imageNotify;
  ZclOtauServerTransac_t  *serverTransac;
} ZCL_OtauServerMem_t;

typedef struct
{
  ZCL_OtauStatInd_t     otauInd;
  ZCL_OtauInitParams_t  initParam;
  HAL_AppTimer_t        genericTimer;
  bool                  isOtauStopTriggered;
#ifdef OTAU_CLIENT
    ZCL_OtauClientMem_t clientMem;
#else
    ZCL_OtauServerMem_t serverMem;
#endif
} ZclOtauMem_t;

typedef struct
{
  uint8_t keyUpdated;
  uint8_t key[KEY_LENGTH];
  uint8_t iv[IV_LENGTH];
} ZCL_OtauImageKey_t;

#if APP_SUPPORT_OTAU_RECOVERY == 1
typedef struct
{
  ZCL_OtauFirmwareVersion_t version;
  uint32_t totalSize;
}ZclOtauRecoveryImage_t;

typedef uint32_t ZCLOtauRecoveryReqFileOffset_t;

typedef struct
{
  ZclOtauRecoveryImage_t          image;
  ZCLOtauRecoveryReqFileOffset_t  fileOffset;
  uint32_t                        pageUpdateFreq;
}ZclOtauRecovery_t;
#endif
/******************************************************************************
                           External variables section
******************************************************************************/
/** zclOtauMem is defined in zclOtauManager.c */
extern ZclOtauMem_t zclOtauMem;
extern bool isOtauBusy;

/******************************************************************************
                           Inline functions section
******************************************************************************/
INLINE ZclOtauMem_t* zclGetOtauMem(void)
{
  return &zclOtauMem;
}
#ifdef OTAU_CLIENT
INLINE ZCL_OtauClientMem_t* zclGetOtauClientMem(void)
{
  return &zclOtauMem.clientMem;
}
#endif
#ifdef OTAU_SERVER
INLINE ZCL_OtauServerMem_t* zclGetOtauServerMem(void)
{
  return &zclOtauMem.serverMem;
}
#endif
INLINE bool isOtauStopped(void)
{
  return zclOtauMem.isOtauStopTriggered;
}
/******************************************************************************
                   Prototypes section
******************************************************************************/
/******`*********************************************************************//**
\brief Lift otau cluster action for customer.

\param[in] action - the OTAU action for a customer.
******************************************************************************/
void zclRaiseCustomMessage(const ZCL_OtauAction_t action);

/***************************************************************************//**
\brief Start otau client services
******************************************************************************/
void zclStartOtauClient(void);

/***************************************************************************//**
\brief Re start otau client services
******************************************************************************/
void zclRestartOtauClient(void);

/***************************************************************************//**
\brief Start otau server service
******************************************************************************/
void zclStartOtauServer(void);

/***************************************************************************//**
\brief Stop otau server service
******************************************************************************/
void zclStopOtauServer(void);

/***************************************************************************//**
\brief Stop otau client service
******************************************************************************/
void zclStopOtauClient(void);

/***************************************************************************//**
\brief Clear otau client memory.
******************************************************************************/
void zclClearOtauClientMemory(void);

/***************************************************************************//**
\brief Restart poll timer

\param[in] period - timer fired time
\param[in] cb     - pointer to fired method
******************************************************************************/
void otauStartGenericTimer(uint32_t period, void (* cb)(void));

/***************************************************************************//**
\brief Stop poll timer
******************************************************************************/
void otauStopGenericTimer(void);

#if APP_SUPPORT_OTAU_RECOVERY == 1
/***************************************************************************//**
\brief  Returns OTAU recovery parameters
******************************************************************************/
ZclOtauRecovery_t *zclOtauGetRecoveryParams(void);

/***************************************************************************//**
\brief  Resets OTAU recovery parameters to default
******************************************************************************/
void zclOtauResetRecoveryParams(void);
#endif
void zclOtauResetAttributesPDS(void);

#endif /* _ZCLOTAUMANAGER_H */
#endif // APP_USE_OTAU
//eof zclOtauManager.h
