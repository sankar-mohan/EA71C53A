/*******************************************************************************
  Zigbee Cluster Library OTAU Client Cluster Header File

  Company:
    Microchip Technology Inc.

  File Name:
    zclOtauClient.h

  Summary:
    Declaration of the OTAU client interface.

  Description:
    Declaration of the OTAU client interface.
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

#if APP_USE_OTAU == 1 && defined(_ZCL_SUPPORT_) && defined(OTAU_CLIENT)

#ifndef _ZCLOTAUCLIENT_H
#define _ZCLOTAUCLIENT_H

/******************************************************************************
                   Includes section
******************************************************************************/
#include <zcl/include/zcl.h>
#include <zcl/include/clusters.h>
#include <zcl/include/zclOTAUCluster.h>
#include <zcl/include/zclOtauManager.h>
#include <private/zclDbg.h>
#include <zdo/include/zdo.h>
#include <ofdExtMemory.h>
#include <configserver/include/configserver.h>
#include <systemenvironment/include/sysAssert.h>
#if APP_SUPPORT_OTAU_PAGE_REQUEST == 1
  #include <nwk/include/nwkAttributes.h>
#endif // APP_SUPPORT_OTAU_PAGE_REQUEST == 1
#include <eeprom.h>
#include <halFlash.h>
#if defined(_ENABLE_PERSISTENT_SERVER_)
#include <pdsDataServer.h>
#endif // APP_SUPPORT_OTAU_RECOVERY == 1 && defined(_ENABLE_PERSISTENT_SERVER_)

/******************************************************************************
                   External variables section
******************************************************************************/
#ifdef __GNUC__
extern uint32_t __d_nv_mem_start;
extern uint32_t __d_nv_mem_end;
#endif

/******************************************************************************
                   Define(s) section
******************************************************************************/
#define OTAU_APP_IMAGE_NAME               "Z3Device"

#define OTAU_BLOCK_REQUEST_USAGE         1
#define OTAU_PAGE_REQUEST_USAGE          0
#define OTAU_GET_MISSED_BYTES            1
#define OTAU_NOT_GET_MISSED_BYTES        0
#define OTAU_FIRST_CLIENT_START          1
#define OTAU_CONTINUE_CLIENT_WORK        0
#define OTAU_BLOCK_REQUEST_DELAY_OFF     0
#define OTAU_BLOCK_REQUEST_DELAY_ON      1

#ifdef  __IAR_SYSTEMS_ICC__
  #pragma segment="D_NV_MEMORY"
  #define OTAU_WRITESKIP_MEMORY_START_ADDR  ((uint32_t)__sfb("D_NV_MEMORY"))
  #define OTAU_WRITESKIP_MEMORY_END_ADDR  ((uint32_t)__sfe("D_NV_MEMORY") - 1U)
  #define OTAU_WRITESKIP_MEMORY_SIZE (OTAU_WRITESKIP_MEMORY_END_ADDR - OTAU_WRITESKIP_MEMORY_START_ADDR )
#elif __GNUC__
  #define OTAU_WRITESKIP_MEMORY_START_ADDR (uint32_t)&__d_nv_mem_start
  #define OTAU_WRITESKIP_MEMORY_END_ADDR (((uint32_t)&__d_nv_mem_end) - 1U)
  #define OTAU_WRITESKIP_MEMORY_SIZE (OTAU_WRITESKIP_MEMORY_END_ADDR - OTAU_WRITESKIP_MEMORY_START_ADDR )
#else
  #error "Unsupported compiler"
#endif

#define OTAU_READ_EEPROM_OK         (0)
#define OTAU_READ_EEPROM_FAILED     (-1)
#define OTAU_EEPROM_BUSY            (-2)

#define BOOT_IMAGETYPE_INTERNAL   0
#define BOOT_IMAGETYPE_EXTERNAL   1

#define BOOT_IMAGE_INACTIVE       0
#define BOOT_IMAGE_ACTIVE         1

#define OTAU_UPGRADE_PENDING_VALUE 0xA5
/*
  From bootloader version > 3, Application boot information is located in last 256 bytes
  of internal flash(as also EEPROM) which will be used by bootloader for image upgrade
  OtauBootInfo_t structure will occupy 136 Bytes
  Additional 192 - 136 = 56 bytes are reseverd for future enhancements of OtauBootInfo_t
  Next two bytes are allocated for ImageType
  Next one byte is allocated for download complete flag,
  in case if any power cycle happens during integrity check
*/
#define APP_BOOTINFO_TOTAL_SIZE               (256)  // Bytes
/*Last ROW (256 Bytes) of EEPROM/FLASH */
#define APP_BOOTINFO_EEPROM_OFFSET            (FLASH_TOTAL_SIZE - APP_BOOTINFO_TOTAL_SIZE - FLASH_EEPROM_BASE_ADDRESS)
#define APP_BOOTINFO_MAX_SIZE                 (192) // (64Bytes in page * 3) 
#define APP_BOOTINFO_RESERVERD_EEPROM_SIZE    (APP_BOOTINFO_MAX_SIZE - sizeof(OtauBootInfo_t))  // 160 - 136ytes are reserved
/*last page of Last ROW*/
#define APP_IMAGE_TYPE_EEPROM_OFFSET          (APP_BOOTINFO_EEPROM_OFFSET + APP_BOOTINFO_MAX_SIZE)
#define APP_DOWNLOAD_COMPLETE_EEPROM_OFFSET   (APP_IMAGE_TYPE_EEPROM_OFFSET + sizeof(ZCL_OtauImageType_t))

/*For bootloader version < 3*/
#define OTAU_IMAGE_KEY_OFFSET                 (4)
#define EEPROM_IMGTYPE_OFFSET                 (37U)
#define EEPROM_IMGTYPE_LENGTH                 (2U)
#define OTAU_FLASH_WRITE_SKIP_EEPROM_OFFSET   (EEPROM_IMGTYPE_OFFSET + EEPROM_IMGTYPE_LENGTH)
#define OTAU_FLASH_WRITE_SKIP_EEPROM_LENGTH   (4U * 2)  // 4bytes each for start  address and end address
#define OTAU_FLASH_WRITE_SKIP_WORDS_LENGTH    (2U)
#define OTAU_EEPROM_ACTION_FLAG_OFFSET        (OTAU_FLASH_WRITE_SKIP_EEPROM_OFFSET + OTAU_FLASH_WRITE_SKIP_EEPROM_LENGTH)
#define OTAU_EEEPROM_ACTION_FLAG_LENGTH       (1U)

#define OTAU_UPGRADE_END_RESP_MAN_ID_WILDCARD_VAL   0xFFFF
#define OTAU_UPGRADE_END_RESP_IMG_TYPE_WILDCARD_VAL 0xFFFF
#define OTAU_UPGRADE_END_RESP_FW_VER_WILDCARD_VAL   0xFFFFFFFF

#define AUXILIARY_STRUCTURE_IS_FULL          (sizeof(uint32_t) + sizeof(uint32_t)) //  offset + internalLength
#define IMAGE_CRC_SIZE                       1

/*
OTAU header feilds length zcl spec rev 6 - 11.4.2 OTA Header Format
*/
#define OTAUHDR_FILEID                    (0x0BEEF11E)
#define OTAUHDR_MAJOR_VERSION             (0x01)
#define OTAUHDR_MAJOR_VERSION_SHIFT       (8)
#define OTAUHDR_FILEID_OCTETLEN           (4U)  // OTA upgrade file identifier
#define OTAUHDR_HDRVERSION_OCTELEN        (2U)  // OTA Header version
#define OTAUHDR_HDRLEN_OCTETLEN           (2U)  // OTA Header length
#define OTAUHDR_HDR_FIELD_CTRL_OCTETLEN   (2U)  // OTA Header Field control
#define OTAUHDR_MFG_CODE_OCTETLEN         (2U)  // Manufacturer code
#define OTAUHDR_IMGTYPE_OCTETLEN          (2U)  // Image type
#define OTAUHDR_FILE_VER_OCTETLEN         (4U)  // File version
#define OTAUHDR_ZSTACK_VER_OCTETLEN       (2U)  // ZigBee Stack version
#define OTAUHDR_HDR_STRING_OCTETLEN       (32U) // OTA Header string
#define OTAUHDR_TOTAL_IMAGE_SIZE_OCTETLEN (4U)  // Total Image size (including header)

#define OTAUHDR_SEC_CREDENTIAL_VER_LEN    (1U)  //Security credential version
#define OTAUHDR_UPGRADE_DEST_ADDR_LEN     (8U)  // Upgrade file destination IEEE Address
#define OTAUHDR_MIN_HW_VER_LEN            (2U)  // Minimum hardware version
#define OTAUHDR_MAX_HW_VER_LEN            (2U)  // Maximum hardware version

/*
OTAU header feilds offset  zcl spec rev 6 - 11.4.2 OTA Header Format
*/

#define OTAUHDR_FILEID_OFFSET               (0)  // OTA upgrade file identifier
#define OTAUHDR_HDRVERSION_OFFSET           (OTAUHDR_FILEID_OFFSET          + OTAUHDR_FILEID_OCTETLEN)        // OTA Header version
#define OTAUHDR_HDRLEN_OFFSET               (OTAUHDR_HDRVERSION_OFFSET      + OTAUHDR_HDRVERSION_OCTELEN)     // OTA Header length
#define OTAUHDR_HDR_FIELD_CTRL_OFFSET       (OTAUHDR_HDRLEN_OFFSET          + OTAUHDR_HDRLEN_OCTETLEN)        // OTA Header Field control
#define OTAUHDR_MFG_CODE_OFFSET             (OTAUHDR_HDR_FIELD_CTRL_OFFSET  + OTAUHDR_HDR_FIELD_CTRL_OCTETLEN)  // Manufacturer code
#define OTAUHDR_IMGTYPE_OFFSET              (OTAUHDR_MFG_CODE_OFFSET        + OTAUHDR_MFG_CODE_OCTETLEN)      // Image type
#define OTAUHDR_FILE_VER_OFFSET             (OTAUHDR_IMGTYPE_OFFSET         + OTAUHDR_IMGTYPE_OCTETLEN)       // File version
#define OTAUHDR_ZSTACK_VER_OFFSET           (OTAUHDR_FILE_VER_OFFSET        + OTAUHDR_FILE_VER_OCTETLEN)      // ZigBee Stack version
#define OTAUHDR_HDR_STRING_OFFSET           (OTAUHDR_ZSTACK_VER_OFFSET      + OTAUHDR_ZSTACK_VER_OCTETLEN)    // OTA Header string
#define OTAUHDR_TOTAL_IMAGE_SIZE_OFFSET     (OTAUHDR_HDR_STRING_OFFSET      + OTAUHDR_HDR_STRING_OCTETLEN)    // Total Image size (including header)

#define OTAUHDR_IMGTYPE_IMGENC_MASK                 (1 << 15)   //0x8000  
#define OTAUHDR_IMGTYPE_INTCHKPRESENT_MASK          (1 << 14)   //0x4000
#define OTAUHDR_IMGTYPE_ISINTCHKENC_MASK            (1 << 13)   //0x2000
#define OTAUHDR_IMGTYPE_ISINTCHKOTHERTHANCRC_MASK   (1 << 12)   //0x1000
#define OTAUHDR_IMGTYPE_MASK                        (0xf << 12) //0xF000

#define OTAUHDR_IMGTYPE_ISIMGSEC_ENABLED(x)     (x & OTAUHDR_IMGTYPE_IMGENC_MASK)
#define OTAUHDR_IMGTYPE_ISINTCHKPRESENT(x)      (x & OTAUHDR_IMGTYPE_INTCHKPRESENT_MASK)
#define OTAUHDR_IMGTYPE_ISINTCHKENCRYPTED(x)    (x & OTAUHDR_IMGTYPE_ISINTCHKENC_MASK)
#define OTAUHDR_IMGTYPE_ISINTCHKOTHERTHANCRC(x) (x & OTAUHDR_IMGTYPE_ISINTCHKOTHERTHANCRC_MASK)

#define OTAU_SREC_DATA_LENGTH           16          // The conventional data field length in S-Record
#define OTAU_MIC_BLOCK_COUNT            1
#define OTAU_MIC_CRC_OFFSET             0
#define OTAUHDR_INTEGRITYCODE_OCTETLEN  (16U)

/* 
   Account for delay at server for ongoing transaction. Consider page processing
   at server as the longest transaction even if this client is in block mode.
   timeout ~= 3000 ms ~= (((pageSize/serverBlockRspPayloadSize)+1) * 
                           (respSpacing + ISDDelay))
   Considering, pageSize = 256, respSpacing = 200, serverBlockRspPayloadSize = 38,
   200 for server delay for read.
   Note : (1) This is just a guess at client side. If server is transacting page from 
          a client with much higher page size, this client's requests will have 
          frequent retry. (2) Assuming page parameters of server's ongoing transaction
          to be same as this client's page parameters is also not correct, so go for
          default value
*/
#define OTAU_DEFAULT_RESPONSE_WAIT_TIMEOUT   3000ul

#define OTAU_UPGRADE_END_RESP_MAN_ID_WILDCARD_VAL 0xFFFF
#define OTAU_UPGRADE_END_RESP_IMG_TYPE_WILDCARD_VAL 0xFFFF
#define OTAU_UPGRADE_END_RESP_FW_VER_WILDCARD_VAL 0xFFFFFFFF

/* frequency of write(in bytes) = total-file-size/OTAU_NUM_OF_PDS_WRITES */
#define OTAU_NUM_OF_PDS_WRITES               20 // (MAXIMUM 100)

#define REPOST_OFD_ACTION                    10ul
#define AMOUNT_MSEC_IN_SEC                   1000ul
#define WAIT_FOR_RUN_UPGRADE_REQ             0xFF
#define WAITING_UPGRADE_END_RESPONSE_TIMEOUT 1 * 60 * 60 * AMOUNT_MSEC_IN_SEC  // 1 hour (in msec)
#define UNLIMITED_WAITING_TIME               0xFFFFFFFFul
#define UPGRADE_END_REQ_QUICK_TIMEOUT        100

#define ZERO_SERVER_EXT_ADDRESS              0x0000000000000000ull
#define INVALID_SERVER_EXT_ADDRESS           0xFFFFFFFFFFFFFFFFull
#define OTAU_MAX_UNAUTHORIZED_SERVERS        3

#define OTAU_SET_STATE(machine, state)       ((machine) = (state))
#define OTAU_CHECK_STATE(machine, state)     ((machine) == (state))


#if OTAU_BITCLOUD_LEGACY_SUPPORT == 1

  #if APP_SUPPORT_OTAU_RECOVERY == 1
#define OTAU_IMGFILES_MEMORY_MEM_ID        OTAU_DIR1_MEM_ID
#define OTAU_AUXFILES_MEMORY_MEM_ID        OTAU_DIR2_MEM_ID
#define OTAU_OFFSETFILES_MEMORY_MEM_ID     OTAU_DIR3_MEM_ID
  #endif
#define OTAU_ATTRIBUTES_MEMORY_MEM_ID        OTAU_DIR4_MEM_ID

#define OTAU_SERVER_EXT_ADDR_MEM_ID          OTAU_PARAM1_MEM_ID
#define OTAU_IMAGE_VERSION_MEM_ID            OTAU_PARAM2_MEM_ID
#define OTAU_IMAGE_SIZE_MEM_ID               OTAU_PARAM3_MEM_ID
#define OTAU_NEXT_OFFSET_MEM_ID              OTAU_PARAM4_MEM_ID
#define OTAU_RUNNING_CHECKSUM_MEM_ID         OTAU_PARAM5_MEM_ID
#define OTAU_INTERNAL_LENGTH_MEM_ID          OTAU_PARAM6_MEM_ID
#define OTAU_IMAGE_REMAINDER_MEM_ID          OTAU_PARAM7_MEM_ID
#define OTAU_ADDRESS_STATUS_MEM_ID           OTAU_PARAM8_MEM_ID
#define OTAU_IMAGE_WRITE_OFFSET_ID           OTAU_PARAM9_MEM_ID

#else

#define OTAU_ATTRIBUTES_MEM_DIR_ID                  OTAU_DIR1_MEM_ID
// UpgradeServerID 0x0000
#define OTAU_ATTR_UPGRADE_SERVER_ID_ATTR_MEM_ID   OTAU_PARAM1_MEM_ID
// 0x0006 ImageUpgradeStatu 
#define OTAU_UPGRADE_STATUS_ATTR_MEM_ID           OTAU_PARAM2_MEM_ID
// 0x0009 MinimumBlockPeriod
#define OTAU_MIN_BLOCK_PERIOD_ATTR_MEM_ID         OTAU_PARAM3_MEM_ID

  #if APP_SUPPORT_OTAU_RECOVERY == 1
#define OTAU_RECOVERY_DIR_ID                        OTAU_DIR2_MEM_ID

#define OTAU_RECOVERY_IMAGE_DETAILS_MEM_ID        OTAU_PARAM4_MEM_ID
#define OTAU_RECOVERY_IMAGE_OFFSET_MEM_ID         OTAU_PARAM5_MEM_ID
  #endif

#endif

#define OTAU_MAX_QUERY_JITTER_VALUE         0xff
/******************************************************************************
                   Types section
******************************************************************************/

/******************************************************************************
                   Constants section
******************************************************************************/
typedef struct {
  ZclOtauDiscoveryResult_t addr;
  ZCL_OtauFirmwareVersion_t ver;
  uint8_t queryJitter;
} ZclOtauImageNotifyParams_t;


typedef struct {
  uint16_t  multiApps       :1;   //  BIT_0
  uint16_t  imgSecurity     :1;   //  BIT_1
  uint16_t  rfInterface     :1;   //  BIT_2
  uint16_t  usart0Interface :1;   //  BIT_3
  uint16_t  usart1Interface :1;   //  BIT_4
  uint16_t  spiInterface    :1;   //  BIT_5
  uint16_t  twiInterface    :1;   //  BIT_6
  uint16_t  usbInterface    :1;   //  BIT_7
  uint16_t  reserved        :8;   //  BIT_8 - BIT_16
} featureBits_t;

typedef union {
  uint16_t        mask;
  featureBits_t   bits;
} FeatureMask_t;

typedef struct 
{
  /*
    Whether the Existing image is in internal or external
      0 - Internal
      1 - External
  */
  uint16_t   memtype:1;
  /*
    Whether the Existing image is active or not
      0 - In Active
      1 - Active
  */
  uint16_t   active:1; 
  uint16_t   reserved2:6;         // Place-holder
}ExistingImageInfo_t;

typedef struct 
{
  /*
    updatePending  - 0 No image upgrade
                     1 Image upgrade pending
  */
  uint16_t   updatePending:1;
  /*
    Whether the downloaded image is in internal or external
      0 - Internal
      1 - External
  */
  uint16_t   srcMemType:1;
  /*
    Whether the destination memory is internal or external
      0 - Internal
      1 - External
  */
  uint16_t   destMemType:1;
  /*
    Type of the downloaded image:
    Img_type:   00- Application
                01- Secured application
                02- Non Application
  */
  uint16_t   imageType:2;
  uint16_t   reserved:3;  // Place-holder
}BootInfo_t;

// App-specific fields in APP_INFO
typedef struct app_table_tag {
  char applicationName[16];       // Description string for the app
  uint32_t  appStartAddr;         // Start address of the app image
  uint32_t  appSize;              // Size of the current app image
  uint32_t  srcAddress;           // Address where the image is downloaded
  uint32_t  dstAddress;           // Address where the image should be placed at the end of upgrade
  uint32_t  skipStartAddress;     // Start address of the location to be skipped 
  uint32_t  skipEndAddress;       // End address of the location to be skipped
  uint32_t  imageSize;            // Size of the image to be upgraded
  ExistingImageInfo_t existingImageInfo;  // 2 Bytes flag to indicate information abt existing image
  BootInfo_t bootImageInfo;       // 2 bytes flag  to indicate the new image information 
  uint8_t   imageSecureKey[OTAUHDR_INTEGRITYCODE_OCTETLEN];  // 16 byte security key to decrypt image after download  
} AppInfo_t;

// APP_INFO table 
typedef struct {
  uint8_t         crc;                   // CRC over the entire app-info (excluding this field)
  /*
  Specifies default application index for the bootloader to jump to, 
  if the Active fields in application-related fields are in inconsistent state 
  (e.g. all set as non-active).
  */
  uint8_t         defaultApp;
  
  /*Application-configurable bit-mask indicating enabled bootloader features and interfaces:
  uint16_t  multiApps       :1;   //  BIT_0
  uint16_t  imgSecurity     :1;   //  BIT_1
  uint16_t  rfInterface     :1;   //  BIT_2
  uint16_t  usart0Interface :1;   //  BIT_3
  uint16_t  usart1Interface :1;   //  BIT_4
  uint16_t  spiInterface    :1;   //  BIT_5
  uint16_t  twiInterface    :1;   //  BIT_6
  uint16_t  usbInterface    :1;   //  BIT_7
  uint16_t  reserved        :8;   //  BIT_8 to BIT_15
  */
  FeatureMask_t   features;         // Bit-mask for features requested by app (See boot_version.h for bit positions) 
  uint8_t         noOfApplications; // Number of valid applications in internal flash. Shall be at least 1.  
  AppInfo_t       appTable[2];      // App-specific sub-table for each app (upto 2)
} OtauBootInfo_t;

/* Bootloader information is available @200 location.
*/
typedef struct {
  uint8_t         crc;
  uint16_t        majorRev;     // Major revision of the bootloader release
  uint16_t        minorRev;     // Minor revision of the bootloader release
  uint16_t        reservedRev;  // Reserved for extending revision  
  FeatureMask_t   supported_features;  // Bit-mask indicating available features
  uint32_t        start_address;  // Start address of the bootloader in internal flash
  uint8_t         mcu_flash_size; // Flash size of the internal MCU (2^value KB, if non-zero) 
  uint8_t         ext_flash_size; // Flash size of the external mem (2^value KB, if non-zero)
  uint8_t         bprot_size;     // Size of the BOOT_PROT region (2^value KB, if non-zero)
  uint8_t         reserved;       // Future extension
} BootLoaderInfo_t;

// update the boot info for image upgrade
uint8_t otauUpdateBootInfo(void (*writeDone)());

/******************************************************************************
                          Prototypes section
******************************************************************************/
ZCL_Status_t otauQueryNextImageRespInd(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_OtauQueryNextImageResp_t *payload);
ZCL_Status_t otauImageNotifyInd(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_OtauImageNotify_t *payload);
ZCL_Status_t otauImageBlockRespInd(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_OtauImageBlockResp_t *payload);
ZCL_Status_t otauUpgradeEndRespInd(ZCL_Addressing_t *addressing, uint8_t payloadLength, ZCL_OtauUpgradeEndResp_t *payload);

bool isExtAddrValid(ExtAddr_t addr);

// initialization related functions
void otauOpenOfdCallback(OFD_Status_t status);

// discovery related functions
void otauEnqueueDiscoveryQueue(ShortAddr_t shortAddr, uint8_t endpoint);

void otauStartDiscovery(void);
void otauStartDiscoveryTimer(void);

void otauIeeeAddrReq(ZclOtauDiscoveryResult_t *srv);
void otauIeeeAddrResp(ZDO_ZdpResp_t *ieeeAddrResp);

void otauNwkAddrReq(void);
void otauNwkAddrResp(ZDO_ZdpResp_t *nwkAddrResp);

void otauMatchDescReq(bool unicast, void (* zdoZdpResp)(ZDO_ZdpResp_t *zdpResp));
void otauBroadcastMatchDescResp(ZDO_ZdpResp_t *zdpResp);
void otauUnicastMatchDescResp(ZDO_ZdpResp_t *zdpResp);

#if (defined _LINK_SECURITY_) && (!defined _LIGHT_LINK_PROFILE_)
void otauRequestKeyReq(ExtAddr_t *tcExtAddr);
void otauRequestKeyResp(APS_RequestKeyConf_t *requestKeyConf);
#endif // (defined _LINK_SECURITY_) && (!defined _LIGHT_LINK_PROFILE_)

// query related functions
void otauStartQuery(void);
void otauStartQueryTimer(void);
void otauTransitionToQuery(void);

// downloading related functions
void otauStartDownload(void);
void otauGetMissedBlocks(void);
void otauImageBlockReq(void);
void otauImagePageReq(void);
void otauImageBlockIntervalElapseCB(void);
void otauCountActuallyDataSize(void);
bool otauCheckPageIntegrity(void);
void otauScheduleImageBlockReq(void);
void otauScheduleImagePageReq(void);
void otauFinalizeProcess(void);

// upgrading related functions
void otauUpgradeEndReq(void);

// helper functions
uint8_t otauCalcCrc(uint8_t crc, uint8_t *pcBlock, uint8_t length);
uint8_t otauCalculateRunningChecksum(uint8_t *data);
void otauContinueWritingImageToFlash(void);
void otauStartSwitch(void);
void otauStartFlush(void);
void otauStartWrite(void);
void otauStartErase(void);
void otauGetCrc(void);

void otauSwitchCallback(OFD_Status_t status);
void otauFlushCallback(OFD_Status_t status, OFD_ImageInfo_t *imageInfo);
void otauGetCrcCallback(OFD_Status_t status, OFD_ImageInfo_t *imageInfo);

void zclOtauFillOutgoingZclRequest(uint8_t id, uint8_t length, uint8_t *payload);
void otauSomeRequestConfirm(ZCL_Notify_t *resp);
void otauSomeDefaultResponse(ZCL_Request_t *req, ZCL_Addressing_t *addressing, uint8_t payloadLength, uint8_t *payload);
void otauAbortUpgradeProcess (void);
void otauSendQueryNextImageReq(void);
void otauSetImageUpgradeStatusAttr(ZCL_ImageUpdateStatus_t status);
ZCL_Status_t otauProcessSuccessfullImageBlockResponse(ZCL_OtauImageBlockResp_t *payload);
ZCL_Status_t otauFillAuxiliaryStructure(ZCL_OtauImageBlockResp_t *payload);
void otauBlockResponseImageDataStoring(ZCL_OtauImageBlockResp_t *payload);
void otauPollServerEndUpgrade(void);
void otauClearPdsParams(void);
void otauConfigureNextPdsWriteOffset(void);
void otauEraseCallback(OFD_Status_t status);

// security related functions
void otauStartEepromDelayedTransaction(void);
void otauReadImgTypeFromEeprom(void);
void readImgTypeEepromCallback(void);
void writeKeyToEeprom(void);

void writeSkipSectionToEepromCallback(void);
void writeEepromActionFlagCallback(void);
void otauWriteEepromAction(void);
void otauStartEepromPollTimer(void (*delayedTransaction)());
void otauWriteSkipSectionToEeprom(void);
int otauSetDownLoadCompleteEEPROM(uint8_t value, void (*writeDone)());
bool otauIsupgradePending(void);

#endif // (ZCL_SUPPORT == 1) && (APP_USE_OTAU == 1)

#endif // _ZCLOTAUCLIENT_H
//eof zclOtauClient.h
