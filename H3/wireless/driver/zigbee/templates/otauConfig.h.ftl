/*******************************************************************************
  OTAU Configuration File

  Company:
    Microchip Technology Inc.

  File Name:
    otauConfig.h

  Summary:
    This file contains Zigbee OTAU Configuration.

  Description:
    This file contains Zigbee OTAU Configuration.
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

#ifndef _OTAU_CONFIG_H_
#define _OTAU_CONFIG_H_

#include <appConfig.h>

//-----------------------------------------------
// Includes OTAU specific Configurations.
//-----------------------------------------------

// Enables or disables support for OTA Upgrade.

<#if OTAU_ENABLE == true>
    #define APP_USE_OTAU 1
<#else>
    #define APP_USE_OTAU 1
</#if>

//-----------------------------------------------
//APP_USE_OTAU == 1
//-----------------------------------------------
#if (APP_USE_OTAU == 1)
  // Use fake OFD driver instead of real one. This option is useful for evaluation of
  // OTA Upgrade feature on boards without external flash.
  //#define APP_USE_FAKE_OFD_DRIVER 0
  //#define APP_USE_FAKE_OFD_DRIVER 1

#define  OTAU_${OTAU_ROLE}

<#if OTAU_USEMEM_INTEXT == "INTERNAL">
#define APP_USE_FAKE_OFD_DRIVER 1
<#else>
#define APP_USE_FAKE_OFD_DRIVER 0
</#if>


  #if (APP_Z3_DEVICE_TYPE ==  APP_DEVICE_TYPE_COMBINED_INTERFACE)

    //#define OTAU_SERVER     // OTAU device role

    // Enable or disable console tunneling support in image store driver. Application
    // can send date through tunnel in parallel to ISD driver
    #define APP_USE_ISD_CONSOLE_TUNNELING     1
    //#define APP_USE_ISD_CONSOLE_TUNNELING   0
  #else

    #define IMAGE_KEY {0x01, 0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78, 0x89, 0x90, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44}
    #define IMAGE_IV {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}

    // OTAU device role
    //#define OTAU_CLIENT
    
    // Enable to support legacy OTAU Devices, Disable for Generic / Certified Devices
    #define OTAU_BITCLOUD_LEGACY_SUPPORT 1
    //#define OTAU_BITCLOUD_LEGACY_SUPPORT 0

  #endif
  
  //OTAU TestHarness
  //#define OTAU_TESTHARNESS
  
  // Support OTAU image page request features
  #define APP_SUPPORT_OTAU_PAGE_REQUEST 1
  //#define APP_SUPPORT_OTAU_PAGE_REQUEST 0
  
  // Enable OTAU related logs on console
  #define APP_DEVICE_OTAU_LOGGING 1
  //#define APP_DEVICE_OTAU_LOGGING 0
  
  //-----------------------------------------------
  //APP_USE_FAKE_OFD_DRIVER == 0
  //-----------------------------------------------
  #if (APP_USE_FAKE_OFD_DRIVER == 0)
    // Defines type of used external flash memory chip.
    #define EXTERNAL_MEMORY AT45DB041
    //#define EXTERNAL_MEMORY AT25DF041A
    //#define EXTERNAL_MEMORY M25P40VMN6PB
    //#define EXTERNAL_MEMORY AT45DB041E
    //#define EXTERNAL_MEMORY MX25L2006E
    //#define EXTERNAL_MEMORY MX25V4006E
    //#define EXTERNAL_MEMORY AT25F2048
    
    #if defined (ATSAMR21E19A)
      #undef EXTERNAL_MEMORY
      #define EXTERNAL_MEMORY MX25V4006E
    #endif
    
    #if (BSP_SUPPORT == BOARD_SAMR21_ZLLEK)
      #undef EXTERNAL_MEMORY
      #define EXTERNAL_MEMORY M25P40VMN6PB
    #endif
  #endif
  
  // Support interrupted OTAU recovery
  #define APP_SUPPORT_OTAU_RECOVERY 0
  //#define APP_SUPPORT_OTAU_RECOVERY 1
  
  // The amount of servers the OTAU client can listen to during upgrade server
  // discovery
  // 
  //  The OTAU client can store information about several discovered OTAU servers.
  // However, the client device tries to connect to discovered servers one by one
  // until the first successful attempt and then communicates and uploads the whole
  // image from the first suitable server.
  // 
  //  The parameter is valid for OTAU clients only.
  // 
  //  Value range: at least 1
  //  C-type: uint8_t
  //  Can be set: at compile time only
  //  Persistent: No
  #define CS_ZCL_OTAU_DISCOVERED_SERVER_AMOUNT 1
  
  // The interval in milliseconds between two attempts to find an upgrade server
  // 
  //  The parameter is valid for OTAU clients only.
  // 
  //  Value range: any 32-bit value
  //  C-type: uint32_t
  //  Can be set: at any time before an OTAU start
  //  Persistent: No
  #define CS_ZCL_OTAU_SERVER_DISCOVERY_PERIOD 60000
  
  // The interval in milliseconds between two successful attempts to query the server
  // 
  //  The parameter is valid only for OTAU clients.
  // 
  //  Value range: any 32-bit value
  //  C-type: uint32_t
  //  Can be set: at any time before an OTAU start
  //  Persistent: No
  #define CS_ZCL_OTAU_QUERY_INTERVAL 10000
  
  // This is the default minimum delay that the server wants the client to wait
  // between subsequent block requests in milliseconds.
  // 
  //  The parameter is valid only for OTAU clients.
  // 
  //  Value range: any 16-bit value
  //  C-type: uint16_t
  //  Can be set: By Server
  //  Persistent: Yes
  #define ZCL_OTAU_DEFAULT_BLOCK_REQ_PERIOD 1000
  
  // Max retry count for commands (OTAU cluster, ZDO and APS) used for OTAU
  // 
  //  The parameter is valid only for OTAU clients.
  // 
  //  Value range: any 8-bit value
  //  C-type: uint8_t
  //  Can be set: at any time before an OTAU start
  //  Persistent: No
  #define CS_ZCL_OTAU_MAX_RETRY_COUNT 3
  
  // The file version indicates the image version that the client is required to
  // install.
  // 
  //  The file version represents the release and build number of the image
  // application and stack.The application release and build numbers are manufacturer
  // specific.
  // 
  //  The parameter is valid only for OTAU clients.
  // 
  //  Value range: any 32-bit value
  //  C-type: uint32_t
  //  Can be set: at any time before an OTAU start
  //  Persistent: No
  #define CS_ZCL_OTAU_FILE_VERSION CS_STACK_VERSION_VALUE
  
  // The default address of an upgrade server
  // 
  //  The parameter indicates how the OTAU client will search for OTAU servers in the
  // network. If one of broadcast addresses is specified, the client will attempt to
  // find all devices supporting the OTAU server cluster and will request new images
  // from the first server that will respond. Otherwise, the client will try to
  // connect to a particular device with the specified extended address.
  // 
  //  The parameter is valid for OTAU clients only.
  // 
  //  Value range: any 64-bit value:
  //  0x0000000000000000ull, 0xFFFFFFFFFFFFFFFFull - a server discovery request is
  // broadcast
  //  otherwise, the client tries to connect to a particular node
  // 
  // 
  //  C-type: ExtAddr_t
  //  Can be set: at any time before an OTAU start
  //  Persistent: No
  #define CS_ZCL_OTAU_DEFAULT_UPGRADE_SERVER_IEEE_ADDRESS 0xFFFFFFFFFFFFFFFFull
  
  // The default Encryption method to use for Image upgrade
  // 
  //  The parameter indicates
  // 
  //  The parameter is valid for OTAU clients only.
  // 
  //  Value range:
  //  OTAU_IMAGE_TYPE_UNSECURED = 0x0000,
  //  OTAU_IMAGE_TYPE_MANU_SPEC_ENCRYPT_IMAGE_NO_MIC = 0x8000,
  //  OTAU_IMAGE_TYPE_MANU_SPEC_ENCRYPT_IMAGE_PLAIN_CRC = 0xC000,
  //  OTAU_IMAGE_TYPE_MANU_SPEC_ENCRYPT_IMAGE_PLAIN_MAC = 0xD000,
  //  OTAU_IMAGE_TYPE_MANU_SPEC_ENCRYPT_IMAGE_ENCRYPT_CRC = 0xE000,
  //  OTAU_IMAGE_TYPE_MANU_SPEC_ENCRYPT_IMAGE_ENCRYPT_MAC = 0xF000,
  //  OTAU_IMAGE_TYPE_CLIENT_SECURITY_CREDENTIAL = 0xFFC0,
  //  OTAU_IMAGE_TYPE_CLIENT_CONFIGURATION = 0xFFC1,
  //  OTAU_IMAGE_TYPE_SERVER_LOG = 0xFFC2,
  //  OTAU_IMAGE_TYPE_PICTURE = 0xFFC3,
  //  OTAU_IMAGE_TYPE_WILD_CARD = 0xFFFF
  // 
  // 
  //  C-type: ZCL_OtauImageType_t
  //  Can be set: at any time before an OTAU start
  //  Persistent: Yes
  //#define CS_ZCL_OTAU_DEFAULT_IMAGE_TYPE 0xF000
  
<#if OTAU_DEFAULT_IMAGE_TYPE == "UNSECURED">
#define CS_ZCL_OTAU_DEFAULT_IMAGE_TYPE 0x0000
<#elseif OTAU_DEFAULT_IMAGE_TYPE == "MANU_SPEC_ENCRYPT_IMAGE_NO_MIC">
#define CS_ZCL_OTAU_DEFAULT_IMAGE_TYPE 0x8000
<#elseif OTAU_DEFAULT_IMAGE_TYPE == "MANU_SPEC_ENCRYPT_IMAGE_PLAIN_CRC">
#define CS_ZCL_OTAU_DEFAULT_IMAGE_TYPE 0xC000
<#elseif OTAU_DEFAULT_IMAGE_TYPE == "MANU_SPEC_ENCRYPT_IMAGE_PLAIN_MAC">
#define CS_ZCL_OTAU_DEFAULT_IMAGE_TYPE 0xD000
<#elseif OTAU_DEFAULT_IMAGE_TYPE == "MANU_SPEC_ENCRYPT_IMAGE_ENCRYPT_CRC">
#define CS_ZCL_OTAU_DEFAULT_IMAGE_TYPE 0xE000
<#elseif OTAU_DEFAULT_IMAGE_TYPE == "MANU_SPEC_ENCRYPT_IMAGE_ENCRYPT_MAC">
#define CS_ZCL_OTAU_DEFAULT_IMAGE_TYPE 0xF000
<#elseif OTAU_DEFAULT_IMAGE_TYPE == "CLIENT_SECURITY_CREDENTIAL">
#define CS_ZCL_OTAU_DEFAULT_IMAGE_TYPE 0xFFC0
<#elseif OTAU_DEFAULT_IMAGE_TYPE == "CLIENT_CONFIGURATION">
#define CS_ZCL_OTAU_DEFAULT_IMAGE_TYPE 0xFFC1
<#elseif OTAU_DEFAULT_IMAGE_TYPE == "SERVER_LOG">
#define CS_ZCL_OTAU_DEFAULT_IMAGE_TYPE 0xFFC2
<#elseif OTAU_DEFAULT_IMAGE_TYPE == "PICTURE">
#define CS_ZCL_OTAU_DEFAULT_IMAGE_TYPE 0xFFC3
<#elseif OTAU_DEFAULT_IMAGE_TYPE == "WILD_CARD">
#define CS_ZCL_OTAU_DEFAULT_IMAGE_TYPE 0xFFFF
</#if>

  // Indicates that image page request are used to load an image
  // 
  //  If the parameter is set to 1 the OTAU client will use image page requests
  // (rather than image block requests) to load an application image from the OTAU
  // server. In this case the server will send a whole page (consisting of multiple
  // blocks) without requesting an APS acknowledgement for each block. The client
  // marks not-delivered blocks and sends an image block request for each of missed
  // blocks after the transmission on the page finishes.
  // 
  //  If the parameter is set to 0 image block requests will be used. The APS
  // acknowledgement is sent for each of received blocks. If any of the blocks is
  // missing loading of the image is restarted.
  // 
  //  The parameter is valid only for OTAU clients.
  // 
  //  Value range: 1 or 0
  //  C-type: uint8_t
  //  Can be set: at compile time only
  //  Persistent: No
  #define CS_ZCL_OTAU_IMAGE_PAGE_REQUEST_ENABLE 1
  //#define CS_ZCL_OTAU_IMAGE_PAGE_REQUEST_ENABLE 0
  
  #if (CS_ZCL_OTAU_IMAGE_PAGE_REQUEST_ENABLE == 1)
    /*Period in milliseconds between two image block responses sent from the OTAU server
    The parameter indicates how fast the OTAU server shall send data (via the image block response command)
    to the OTAU client. The value is set on the client. The server receives the value from the
    client. The server shall wait for, at minimum, the period specified in the parameter before sending
    another block to the client.

    The parameter is valid only for OTAU clients.

    Value range: not less than 200ms (according to the OTAU specification) 
    C-type: uint16_t
    Can be set: at compile time only 
    Persistent: No*/
      #define CS_ZCL_OTAU_IMAGE_PAGE_REQUEST_RESPONSE_SPACING 200

    /*The page size - the number of bytes sent for a single image page request
    The parameter sets the number of bytes to be sent by the server for an image page
    request sent by the OTAU client. A page is transmitted in several data frames, one
    data frame for each block of data. The client, upon receiving individual blocks,
    does not send an APS acknowledgement until all blocks for the current page are not
    sent. Blocks that have not been delivered are requested by the client via image
    block requests after loading of the current page finishes.

    The page size should be greater than or equal to 64 bytes.

    The parameter is valid only for OTAU clients.

    Value range: equal to or greater than 64
    C-type: uint16_t
    Can be set: at compile time only
    Persistent: No*/
      #define CS_ZCL_OTAU_IMAGE_PAGE_REQUEST_PAGE_SIZE 256
    #endif
#endif //


#endif // _OTAU_CONFIG_H_
