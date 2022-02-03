---
grand_parent: Examples applications
parent: BLE Building Blocks
title: BLE Connection(Central)
has_children: true
has_toc: false
nav_order: 33

family: PIC32CX-BZ
market:
  - wireless
---
[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)
#  BLE Connection(Central)

[Getting Started](../../readme.md)

[Getting Started with Central Building Blocks](../../central/readme.md)

[Legacy Scan](../legacy_scan/readme.md) **-->** [BLE Connection](../connection/readme.md) **-->** [Profiles and Services](../profiles_services/readme.md)

## Introduction
This document will help users to enable scan and connect functionality
on the BLE Central Device (WBZ45x). For a successful BLE connection,
ADVERTISER must start sending advertisement packets on the three primary
advertisement channels (or a subset of these channels). This allows the
devices scanning for advertisers to find then and read their
advertisement data, the scanner can initiate a connection if advertiser
allows it. To demonstrate a BLE Connection on WBZ45x we need 2 device
types

1.  Advertiser (Transmitting Connectable Adv)

2.  Scanner

Users of this document can choose to just run the precompiled
Application Example hex file on the WBZ451 Curiosity Board and
experience the demo or can go through the steps involved in developing
this Application from scratch

## Recommended Reads

1.  [FreeRtos BLE App Initialize](../freertos_ble_stack_init_central.md)

2.  [BLE Legacy Scan](../../central/legacy_scan/readme.md)

## Hardware Required

  |**Tool**                | **Qty**  |
  |------------------------| ---------|
  |WBZ451 Curiosity Board  | 2        |
  |Micro USB cable         | 2        |

## SDK Setup

1.  [SDK Setup](../../../../docs/pic32cx_bz2_wbz45x_sdk_setup.md)

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

1.  Follow steps mentioned in section 4 of [SDK Setup](../../../../docs/pic32cx_bz2_wbz45x_sdk_setup.md) document

2.  Open and program the Application Example <a href="firmware"> central_connection </a>  in MPLABX IDE

## Demo Description

To experience this demo users are required to use [central_connection](../../central/legacy_scan/firmware) and [peripheral_connection](../../peripheral/legacy_adv_sleep/firmware) application examples.
The 2 Application Example mentioned above will demonstrate a BLE
Connection Establishment Process. An advertiser (Peripheral) will start
connectable, scannable, undirected BLE advertisement and the scanner
(Central) will receive the advertisements and initiate a BLE connection.
enables users to transmit Connectable and Scannable Undirected BLE
Advertisements. Demo will print start of the scanning "Scanning", connected "Connected!" and disconnected "Disconnected"
"Disconnected" state on a terminal emulator like TeraTerm@ (Speed:
115200, Data: 8-bit, Parity: none, stop bits: 1 bit, Flow control: none)

## Testing

Users should use another WBZ451 Curiosity Board configured as [peripheral connection](../../peripheral/connection/readme.md)

This section assumes that a user has already programmed the peripheral_conn and central_conn application on 2 WBZ451 Curiosity Boards.

Board1 = WBZ451 Curiosity Board with peripheral_conn application Programmed
Board2 = WBZ451 Curiosity Board with central_conn applicaton Programmed

Board1: Open TeraTerm @ (Speed: 115200, Data: 8-bit, Parity: none, stop bits: 1
bit, Flow control: none). Reset the board. Upon reset, "Advertising" message is displayed on the TeraTerm.

Board2: Open TeraTerm @ (Speed: 115200, Data: 8-bit, Parity: none, stop bits: 1
bit, Flow control: none). Reset the board. Upon reset, "Scanning" message is displayed on the TeraTerm. Upon finding peripheral device with public address {0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6} message "Found Peer Node" will be displayed and a connection request will be initiated "Initiating connection".

 **Note:** Scanner is configured to scan only for 100 secs, user should ensure the peer device is advertising

After connection establishment, both the peripheral device (Board1) and central device(Board2) will display "Connected!" message on respective terminal windows

![](media/central_conn_1.PNG)

## Developing this Application from scratch using MPLAB Code Configurator

This section explains the steps required by a user to develop this application example from scratch using MPLABx Code Configurator

**Tip:** New users of MPLAB Code Configurator are recommended to go through the [overview](https://onlinedocs.microchip.com/pr/GUID-1F7007B8-9A46-4D03-AEED-650357BA760D-en-US-6/index.html?GUID-B5D058F5-1D0B-4720-8649-ACE5C0EEE2C0). 

1.  Create a new MCC Harmony Project --[link](../../../../docs/creating_new_mplabx_harmony_project.md) for
    instructions

2.  Import Harmony component configuration -- This step helps users
    setup the basic Harmony components required to start their
    Application Development
    Harmony component configuration related to this Application is
    available [here](firmware/ble_central_conn_wbz451.X/ble_central_conn_wbz451.mc3)    
    Users should follow the instructions mentioned
    [here](https://microchipdeveloper.com/mcc:peripheralconfig) to import the Harmony component configuration.

    **Tip:** Import and Export functionality of Harmony component
    configuration will help users to start from a known working setup of MCC
    configuration

3.  Accept Dependencies or satisfiers, select "Yes"

4.  Verify if the Project Graph window has all the expected
    configuration
    ![](media/central_conn_2.PNG)

## Verify Scan and Connection Configuration
1. Select **BLE_Stack** component in project graph
    ![](media/central_conn_3.PNG)

## Generate Code [link](../../../../docs/generate_code.md) for instructions

## Files and Routines Automatically generated by the MCC
After generating the program source from MCC interface by clicking Generate Code, the BLE configuration can be found in the following project directories

![](media/central_conn_4.png)


The [OSAL](http://ww1.microchip.com/downloads/en/DeviceDoc/MPLAB%20Harmony%20OSAL%20Libraries%20Help%20v2.06.pdf), RF System, BLE System  initialization routine executed during program initialization can be found in the project files. This initialization routine is automatically generated by the MCC

![](media/central_conn_5.png)

The BLE stack initialization routine excuted during Application Initialization can be found in project files. This intitialization routine is automatically generated by the MCC. This call initializes and configures the GAP, GATT, SMP, L2CAP and BLE middleware layers.
![](media/central_conn_6.png)


  |**Source Files**        | **Usage**  |
  |------------------------|------------|
  |app.c                   |Application State machine, includes calls for Initialization of all BLE stack (GAP,GATT, SMP, L2CAP) related componenet configurations            |
  |app_ble.c               |Source Code for the BLE stack related componenet configurations, code related to function calls from app.c                                        |
  |app_ble_handler.c       |All GAP, GATT, SMP and L2CAP Event handlers   

> **Tip:** app.c is autogenerated and has a state machine based
> Application code sample, users can use this template to develop their
> application                                                                                                    |
### Header Files

-   ble_gap.h- This header file contains BLE GAP functions and is
    automatically included in the app.c file

### Function Calls

MPLAB Code Configurator generates and adds the code to initialize the BLE Stack GAP,
GATT, L2CAP and SMP in *APP_BleStackInit()* function

-   APP_BleStackInit() is the API that will be called inside the
    Applications Initial State -- APP_STATE_INIT in app.c\

## User Application Development

### Include

-   user action is required as mentioned [here](../../../../docs/user_action.md)
-   definitions.h in all the files where UART will be used to print debug information
>Tip: definitions.h is not specific to just UART peripheral, instead it should be included in all application source files where peripheral functionality will be exercised  


### Start Scanning  

```
BLE_GAP_SetScanningEnable(true, BLE_GAP_SCAN_FD_ENABLE, BLE_GAP_SCAN_MODE_OBSERVER, 1000);
```

This API is called in the Applications initialstate - APP_STATE_INIT in app.c. Scan duration is 100 secs
![](media/central_conn_7.PNG)

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

![](media/central_conn_8.PNG)

### Connected & Disconnected Events

-   All the possible GAP, GATT, SMP and L2CAP Event handlers are available in file app_ble_handler.c, users can implement application code to denote Connection State here.
BLE_GAP_EVT_CONNECTED event is generated after a successful BLE Connection with peripheral device
    ![](media/central_conn_9.PNG)

### Scan Timeout Event   

-   In app_ble_handler.c BLE_GAP_EVT_SCAN_TIMEOUT event is generated when BLE Scan duration expires.

![](media/central_conn_10.PNG)   

Users can exercise various other BLE Advertisement functionalities by
using [BLE Stack API](../../docs/api/driver/ble/docs/html/modules.html)

## Where to go from here

-   [BLE Profiles and Services](../profiles_services/readme.md)
