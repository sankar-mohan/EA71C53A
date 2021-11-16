---
grand_parent: Examples applications
parent: BLE Building Blocks
title: Central Transparent UART
has_children: true
has_toc: false
nav_order: 35

family: PIC32CX-BZ
market:
  - wireless
---

[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)
#  BLE Transparent UART(Central)

[Getting Started](../../../readme.md)

[Getting Started with Central Building Blocks](../../../central/readme.md)

[BLE Connection](../../connection/readme.md) **-->** [Profiles and Services](../../profiles_services/readme.md) **-->** [BLE Transparent UART](readme.md) **-->** [BLE Multilink Transparent UART](../ml_trp_uart/readme.md)

## Introduction

This document will help users create a central device and send/receive characters between 2 connected BLE devices over Microchip proprietary Transparent UART Profile. The central and peripheral devices in this tutorial are 2 WBZ45x devices.
The instructions mentioned below are applicable for a BLE Central device.

Users of this document can choose to just run the precompiled Application Example hex file on the WBZ451 Curiosity board and experience the demo or can go through the steps involved in developing this Application from scratch. 

## Recommended Reads

1. [BLE Connection(central)](../../connection/readme.md) 

## Hardware Required

  |**Tool**                | **Qty**  |
  |------------------------| ---------|
  |WBZ451 Curiosity Board  | 2        |
  |Micro USB cable         | 2        |

## SDK Setup
1.  [SDK Setup](../../../../../docs/pic32cx_bz2_wbz45x_sdk_setup.md)

## Software

1.  [TeraTerm](https://ttssh2.osdn.jp/index.html.en) 

## Smartphone App
  None

## Programming the precompiled hex file or Application Example

### Programming the hex file using MPLABX IPE

1.  Precompiled Hex file is located <a href="hex/"> here </a>

2.  Follow the steps mentioned [here](https://microchipdeveloper.com/ipe:programming-device)

 **Caution:** Users should choose the correct Device and Tool information
### Programming the Application using MPLABX IDE

1.  Follow steps mentioned in section 4 of [SDK Setup](../../../../../docs/pic32cx_bz2_wbz45x_sdk_setup.md) document

2.  Open and program the Application Example <a href="firmware"> central_trp_uart </a>  in MPLABX IDE 

## Demo Description

Upon programming the demo application, central device (WBZ45x) will start scanning for near by peripheral devices to connect. After a connection has been made data can be sent back and forth over UART between the two devices that are connected. 

Demo will print start of the scanning "Scanning",connected "Connected!" and disconnected
"Disconnected" state on a terminal emulator like TeraTerm@ (Speed:
115200, Data: 8-bit, Parity: none, stop bits: 1 bit, Flow control: none) Application Data to be sent to the connected peripheral device should be entered in the terminal emulator. 

## Testing
Users should use another WBZ451 Curiosity Board configured as [BLE Transparent UART(peripheral)](../../../peripheral/profiles_services/trp_uart/readme.md) 

**Demo Experience when using 2 WBZ45x Curiosity boards configured as Peripheral and  Central device**
This section assumes that a user has already programmed the peripheral_trp_uart and central_trp_uart application on 2 WBZ451 Curiosity Boards.

Board1 = WBZ451 Curiosity Board with peripheral_trp_uart application Programmed
Board2 = WBZ451 Curiosity Board with central_trp_uart applicaton Programmed

Board1: Open TeraTerm @ (Speed: 115200, Data: 8-bit, Parity: none, stop bits: 1
bit, Flow control: none). Reset the board. Upon reset, "Advertising" message is displayed on the TeraTerm.

Board2: Open TeraTerm @ (Speed: 115200, Data: 8-bit, Parity: none, stop bits: 1
bit, Flow control: none). Reset the board. Upon reset, "Scanning" message is displayed on the TeraTerm. Upon finding peripheral device with public address {0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6} message "Found Peer Node" will be displayed and a connection request will be initiated "Initiating connection".

 **Note:** Scanner is configured to scan only for 100 secs, user should ensure the peer device is advertising

After connection establishment, both the peripheral device (Board1) and central device(Board2) will display "Connected!" message on respective terminal windows. 

Users can now start sending data back and forth between the central and peripheral device using the terminal emulator. Characters entered on either terminals will immediately be sent to the peer devices

![](media/trp_uart_13.PNG)

## Developing this Application from scratch using Harmony 3

This section explains the steps required by a user to develop this
application example from scratch using MPLABx Harmony 3 Framework

**Tip:** New Harmony 3 Users are recommended to go through the [overview](https://microchip-mplab-harmony.github.io/mhc/doc/readme.html) of Harmony 3. Users can add/remove different components like peripheral support and other wireless functionality by following steps mentioned [here](https://microchip-mplab-harmony.github.io/mhc/doc/readme_mhc_configuration.html). 

1.  Create a new MPLAB Harmony 3 Project -- [link](../../../../../docs/creating_new_mplabx_harmony_project.md) for
    instructions

2.  Import Harmony 3 component configuration -- This step helps users
    setup the basic Harmony 3 components required to start their
    Application Development
    Harmony 3 component configuration related to this Application is
    available [here](export.mhc)
    Users should follow the instructions mentioned
    [here](https://microchip-mplab-harmony.github.io/mhc/doc/readme_mhc_import_export.html) to import the Harmony 3 component configuration.

    **Tip:** Import and Export functionality of Harmony 3 component
    configuration will help users to start from a known working setup of Harmony 3
    configuration

3.  Accept Dependencies or satisfiers, select "Yes"

4.  Verify if the Project Graph window has all the expected Harmony 3
    configuration
    ![](media/trp_uart_1.PNG)

## Verify Scan,Connection and Transparent UART Profile Configuration
1. Select **BLE_Stack** component in project graph
    ![](media/trp_uart_2.PNG)

1. Select **Transparent Profile** component in project graph
    ![](media/trp_uart_3.PNG)


## Generate Code [link](../../../../../docs/generate_code.md) for instructions

## Files and Routines Automatically generated by the MHC
After generating the program source from MHC interface by clicking Generate Code, the BLE configuration can be found in the following project directories 

![](media/trp_uart_4.PNG)


The [OSAL](http://ww1.microchip.com/downloads/en/DeviceDoc/MPLAB%20Harmony%20OSAL%20Libraries%20Help%20v2.06.pdf), RF System, BLE System  initialization routine executed during program initialization can be found in the project files. This initialization routine is automatically generated by the MHC

![](media/trp_uart_5.png)

The BLE stack initialization routine excuted during Application Initialization can be found in project files. This intitialization routine is automatically generated by the MHC. This call initializes and configures the GAP, GATT, SMP, L2CAP and BLE middleware layers.

![](media/trp_uart_7.PNG)


  |**Source Files**        | **Usage**  |
  |------------------------|------------|
  |app.c                   |Application State machine, includes calls for Initialization of all BLE stack (GAP,GATT, SMP, L2CAP) related componenet configurations            |
  |app_ble.c               |Source Code for the BLE stack related componenet configurations, code related to function calls from app.c                                        |
  |app_ble_handler.c       |All GAP, GATT, SMP and L2CAP Event handlers    |
  |app_trspc_handler.c     |All Transparent UART Client related Event handlers    |
  |ble_trspc.c             |All Transparent Client Functions for user application    |

> **Tip:** app.c is autogenerated and has a state machine based
> Application code sample, users can use this template to develop their
> application                                                                                                   
### Header Files 

-   ble_gap.h- This header file contains BLE GAP functions and is
    automatically included in the app.c file

- ble_trspc.h is the Header File associated with API’s and structures related to BLE Transparent Client functions for Application User

### Function Calls

Harmony 3 generates and adds the code to initialize the BLE Stack GAP,
GATT, L2CAP and SMP in *APP_BleStackInit()* function

-   APP_BleStackInit() is the API that will be called inside the
    Applications Initial State -- APP_STATE_INIT in app.c

## User Application Development

### Include

-   "ble_trspc.h" in app.c, BLE Transparent UART Server related API's are available here
-   "osal/osal_freertos_extend.h" in app_trsps_handler.c, OSAL related API's are available here
-   definitions.h in all the files where UART will be used to print debug information 
Tip: definitions.h is not specific to just UART peripheral, instead it should be included in all application source files where peripheral functionality will be exercised 
-   user action is required as mentioned <a href="../../../../../docs/user_action.md"> here </a> 


### Start Scanning  

```
BLE_GAP_SetScanningEnable(true, BLE_GAP_SCAN_FD_ENABLE, BLE_GAP_SCAN_MODE_OBSERVER, 1000);
```
![](media/trp_uart_6.png)

This API is called in the Applications initialstate - APP_STATE_INIT in app.c. Scan duration is 100 secs

### Scan Results and initiating a BLE connection  

-   BLE_GAP_EVT_ADV_REPORT event is generated upon finding Adverstisements on legacy channels
- BLE connection can be initiated by using the API BLE_GAP_CreateConnection(&createConnParam_t);

```
  // code snippet to filter scan results and initiate connection
  if (p_event->eventField.evtAdvReport.addr.addr[0] == 0xA1 && p_event->eventField.evtAdvReport.addr.addr[1] == 0xA2)
  {
      SERCOM0_USART_Write((uint8_t *)"Found Peer Node\r\n", 17);
      BLE_GAP_CreateConnParams_T createConnParam_t;
      createConnParam_t.scanInterval = 0x3C; // 37.5 ms 
      createConnParam_t.scanWindow = 0x1E; // 18.75 ms
      createConnParam_t.filterPolicy = BLE_GAP_SCAN_FP_ACCEPT_ALL;
      createConnParam_t.peerAddr.addrType = p_event->eventField.evtAdvReport.addr.addrType;
      memcpy(createConnParam_t.peerAddr.addr, p_event->eventField.evtAdvReport.addr.addr, GAP_MAX_BD_ADDRESS_LEN);
      createConnParam_t.connParams.intervalMin = 0x10; // 20ms
      createConnParam_t.connParams.intervalMax = 0x10; // 20ms
      createConnParam_t.connParams.latency = 0;
      createConnParam_t.connParams.supervisionTimeout = 0x48; // 720ms
      SERCOM0_USART_Write((uint8_t *)"Initiating Connection\r\n", 23);
      BLE_GAP_CreateConnection(&createConnParam_t);
  }

```
![](media/trp_uart_14.PNG)

### Connected & Disconnected Events

-   In app_ble_handler.c BLE_GAP_EVT_CONNECTED event will be generated when a BLE connection is completed

### Connection Handler
-   Connection handle associated with the peer peripheral device needs to be saved for data exchange after a BLE connection
-   p_event->eventField.evtConnect.connHandle has this information
    
    ![](media/trp_uart_8.PNG)

### Transmit Data
-   BLE_TRSPC_SendData(conn_hdl , 1, &data);  is the API to be used for sending data towards the central device
**Note:** The precompiled application example uses a UART callback to initiate the data transmission upon receiving a character on UART

Example Implementation for Transmitting the received data over UART using the BLE_TRSPC_SendData API 
```
  void uart_cb(SERCOM_USART_EVENT event, uintptr_t context)
  {
    // If RX data from UART reached threshold (previously set to 1)
    if( event == SERCOM_USART_EVENT_READ_THRESHOLD_REACHED )
    {
      uint8_t data;
      // Read 1 byte data from UART
      SERCOM0_USART_Read(&data, 1);
      // Send the data from UART to connected device through Transparent service
      BLE_TRSPC_SendData(conn_hdl, 1, &data);
    }
  }
  uint16_t conn_hdl; // connection handle info captured @BLE_GAP_EVT_CONNECTED event
  // Register call back when data is available on UART for Peripheral Device to send
  // Enable UART Read
  SERCOM0_USART_ReadNotificationEnable(true, true);
  // Set UART RX notification threshold to be 1
  SERCOM0_USART_ReadThresholdSet(1);
  // Register the UART RX callback function
  SERCOM0_USART_ReadCallbackRegister(uart_cb, (uintptr_t)NULL);
```
![](media/trp_uart_9.PNG)

### Receive Data
- BLE_TRSPC_EVT_RECEIVE_DATA is the event generated when data is sent from central device
- Users need to use the BLE_TRSPC_GetDataLength(&data_len) API to extract the length of application data received 
- BLE_TRSPC_GetData(&conn_hdl, data); API is used to  retrieve the data, conn_hdl is the value obtained from Connection Handler section

**Tip:** BLE_TRSPC_Event_T p_event structure stores the information about BLE transparent UART callback functions

Example Implementation for printing the received data from peripheral device over UART
```
  /* TODO: implement your application code.*/
    uint16_t data_len;
    uint8_t *data;
    // Retrieve received data length
    BLE_TRSPC_GetDataLength(&data_len);
    // Allocate memory according to data length
    data = OSAL_Malloc(data_len);
    if(data == NULL)
    break;
    // Retrieve received data
    BLE_TRSPC_GetData(&p_event->eventField.onReceiveData.connHandle, data);
    // Output received data to UART
    SERCOM0_USART_Write(data, data_len);
    // Free memory
    OSAL_Free(data);
```
![](media/trp_uart_10.PNG)


Users can exercise various other BLE functionalities by
using [BLE Stack API](../../../docs/api/driver/ble/docs/html/modules.html)

## Where to go from here

-   [BLE Profiles and Services](../readme.md)

-   [BLE Multilink Transparent UART](../ml_trp_uart/readme.md)
