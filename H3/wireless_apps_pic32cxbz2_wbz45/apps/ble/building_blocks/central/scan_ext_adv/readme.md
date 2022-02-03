---
grand_parent: Examples applications
parent: BLE Building Blocks
title: BLE Extended Scan
has_children: true
has_toc: false
nav_order: 32

family: PIC32CX-BZ
market:
  - wireless
---
[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)
#  BLE Scanning Extended Advertisements

<a href="../../readme.md"> Getting Started </a>

<a href="../../central/readme.md"> Getting Started with Central Building Blocks </a>

<a href="../../chip_peripherals/uart/readme.md"> Adding UART </a> **-->** <a href="readme.md"> **Extended Advertisements** (you are here) </a>

## Introduction

This document will enable scanning of Extended Advertisements
(ADV_EXT_IND, ADV_AUX_IND) on WBZ451 Curiosity board. For a successful
scan of Extended Advertisement user needs to have a broadcaster
transmitting these Advertisements. In BLE a central or observer always
starts with scanning.

Usage of scan_ext_adv Application example in combination with ext_adv
example will enable users to test features like long range (Coded PHY)
and sending data (1M, 2M, Coded PHY) over extended advertisements

Users can run run the precompiled
Application Example hex file on the WBZ451 Curiosity Board and
go through the steps involved in developing this Application from scratch

## Recommended Reads

1.  <a href="../freertos_ble_stack_init_central.md">FreeRtos BLE App Initialize</a>

2.  <a href="../../chip_peripherals/uart/readme.md">Adding UART</a>

## Hardware Required

  |**Tool**                | **Qty**                        |
  |------------------------| -------------------------------|
  |WBZ451 Curiosity Board  | 2                              |
  |Micro USB cable         | 2                              |

## SDK Setup

1.  <a href="../../../../docs/pic32cx_bz2_wbz45x_sdk_setup.md"> SDK Setup </a>

## Software

1.  <a href="https://ttssh2.osdn.jp/index.html.en"> TeraTerm </a>

## Smartphone App
  None

## Programming the precompiled hex file or Application Example

### Programming the hex file using MPLABX IPE

1.  Precompiled Hex file is located <a href="hex/"> here </a>

2.  Follow the steps mentioned [here](https://microchipdeveloper.com/ipe:programming-device)

 **Caution:** Users should choose the correct Device and Tool information
### Programming the Application using MPLABX IDE

1.  Follow steps mentioned in section 4 of <a href="../../../../docs/pic32cx_bz2_wbz45x_sdk_setup.md"> SDK Setup </a> document

2.  Open and program the Application Example <a href="firmware"> scan_ext_adv </a>  in MPLABX IDE

## Demo Description

This Application Example enables users to do scanning of Extended
Advertisements (ADV_EXT_IND, ADV_AUX_IND PDU's). Scanning of CODED PHY
(125 knps) is enabled by default in the application. After programming
the Application Example, on reset "ExtAdv Scan Enable Success" will be
printed in the terminal window and if there is a broadcaster sending
extended advertisements the Green LED will toggle. If this broadcaster
is another WBZ45x module programmed with ext_adv example it will print
the Application Data sent in Auxiliary packet - ADV_AUX_IND on TeraTerm (115200, Data: 8-bit, Parity: none, stop bits: 1 bit, Flow control: none).
Green LED toggles denote the reception of Extended Advertisements.

## Testing
Users should use another WBZ451 Curiosity Board configured as <a href="../../peripheral/ext_adv/readme.md"> BLE Ext Adv </a>

This section assumes that a user has already programmed the ext_adv and scan_ext_adv
application on 2 WBZ451 Curiosity Boards.

Board1 = WBZ451 Curiosity Board with Ext Adv Programmed
Board2 = WBZ451 Curiosity Board with Scan Ext Adv Programmed

Board1: Open TeraTerm @ (Speed: 115200, Data: 8-bit, Parity: none, stop bits: 1
bit, Flow control: none). Reset the board. Upon reset, "Ext Adv Enable" message is displayed on the TeraTerm.

Board2: Open TeraTerm @ (Speed: 115200, Data: 8-bit, Parity: none, stop bits: 1
bit, Flow control: none). Reset the board. Upon reset, "ExtAdv Scan
Enable Success" message is displayed on the TeraTerm. "Microchip"
message will be displayed as soon the WBZ451 module performs an extended
advertisement scan.

Data from advertiser is sent every 2 secs, Green LED is toggled when the observer device receives these extended advertisements.

![](media/scan_ext_adv1.PNG)


## Developing this Application from scratch using MPLAB Code Configurator

This section explains the steps required by a user to develop this application example from scratch using MPLABx Code Configurator

**Tip:** New users of MPLAB Code Configurator are recommended to go through the [overview](https://onlinedocs.microchip.com/pr/GUID-1F7007B8-9A46-4D03-AEED-650357BA760D-en-US-6/index.html?GUID-B5D058F5-1D0B-4720-8649-ACE5C0EEE2C0) .

1.  Create a new MCC Harmony Project - <a href="../../../../docs/creating_new_mplabx_harmony_project.md">instructions.</a>

2.  Import component configuration -- This step helps users
    setup the basic components required to start their
    Application Development component configuration related to this Application is
    available [here](firmware/ble_scan_ext_adv_wbz451.X/ble_scan_ext_adv_wbz451.mc3)     
    Users should follow the instructions mentioned
    [here](https://microchipdeveloper.com/mcc:peripheralconfig) to import the component configuration.

    **Tip:** Import and Export functionality of component configuration will help users to start from a known working setup of configuration

3.  Accept Dependencies or satisfiers, select "Yes"

4.  Verify if the Project Graph window has all the expected configuration
    ![](media/ext_adv_1.PNG)

## Verify Scan Configuration
1.  Select **BLE_Stack** component in project graph

    ![](media/ext_adv_2.PNG)

## Generate Code <a href="../../../../docs/generate_code.md"> link </a> for instructions

## Files and Routines Automatically generated by the MCC
After generating the program source from MCC interface by clicking Generate Code, the BLE configuration can be found in the following project directories

![](media/ext_adv_4.png)


The <a href="http://ww1.microchip.com/downloads/en/DeviceDoc/MPLAB%20Harmony%20OSAL%20Libraries%20Help%20v2.06.pdf"> OSAL </a>, RF System, BLE System  initialization routine executed during program initialization can be found in the project files. This initialization routine is automatically generated by the MCC

![](media/ext_adv_5.png)

The BLE stack initialization routine excuted during Application Initialization can be found in project files. This intitialization routine is automatically generated by the MCC. This call initializes and configures the GAP, GATT, SMP, L2CAP and BLE middleware layers.
![](media/ext_adv_7.PNG)

Configuration for scanning extended advertisements is autogenerated


  |**Source Files**        | **Usage**  |
  |------------------------|------------|
  |app.c                   |Application State machine, includes calls for Initialization of all BLE stack (GAP,GATT, SMP, L2CAP) related componenet configurations            |
  |app_ble.c               |Source Code for the BLE stack related componenet configurations, code related to function calls from app.c                                        |
  |app_ble_handler.c       |All GAP, GATT, SMP and L2CAP Event handlers   
  |app_user_edits.c        |User Code Change instruction|
> **Tip:** app.c is autogenerated and has a state machine based
> Application code sample, users can use this template to develop their
> application                                                                                                    |
### Header Files

-   ble_gap.h- This header file contains BLE GAP functions and is
    automatically included in the app.c file

### Function Calls

MCC generates and adds the code to initialize the BLE Stack GAP,
GATT, L2CAP and SMP in *APP_BleStackInit()* function

-   APP_BleStackInit() is the API that will be called inside the
    Applications Initial State -- APP_STATE_INIT in app.c

## User Application Development

### Include

-   definitions.h in all the files where UART will be used to print debug information
>Tip: definitions.h is not specific to just UART peripheral, instead it should be included in all application source files where peripheral functionality will be exercised
-   user action is required as mentioned <a href="../../../../docs/user_action.md"> here </a>


### Enable Scanning of Extended Advertisement

-   BLE_GAP_SetExtScanningEnable(BLE_GAP_SCAN_MODE_OBSERVER, &extScan );

API's mentioned in this section are called in the Applications initial
state - APP_STATE_INIT in app.c

![](media/scan_ext_adv2.PNG)

### Configure LED
Add the following piece of code in GPIO_Initialize() of \firmware\src\config\default\peripheral\gpio\plib_gpio.c

```
    GPIOB_REGS->GPIO_LAT = 0x0; /* Initial Latch Value */
    GPIOB_REGS->GPIO_TRISCLR = 0x8; /* Direction Control */
    GPIOB_REGS->GPIO_ANSELCLR = 0x8; /* Digital Mode Enable */
```

### Scan Results  

-   BLE_GAP_EVT_EXT_ADV_REPORT event is generated upon finding Adverstisements on legacy channels

```
  // code snippet to print ext adv data
    // GPIO will toggle if it can scan any EXT ADV PDU near based on BLE_GAP_SCAN_PHY chosen
    GPIOB_REGS->GPIO_PORTINV = 0x08;
    // length value of 19 is chosen as a filter as ext_adv example sends 19 bytes of data
    // user can modify filter mechanism based on their requirements
    if (p_event->eventField.evtExtAdvReport.length == 19)
    {
        SERCOM0_USART_Write((uint8_t *)"\r\n", 2);
        SERCOM0_USART_Write(&p_event->eventField.evtExtAdvReport.advData[5], 9);
    }
```
![](media/scan_ext_adv3.PNG)

Users can exercise various other BLE Advertisement functionalities by
using <a href="../../docs/api/driver/ble/docs/html/modules.html"> BLE Stack API </a>

## Where to go from here

<a href="../../central/readme.md"> Getting Started with Central Building Blocks </a>
