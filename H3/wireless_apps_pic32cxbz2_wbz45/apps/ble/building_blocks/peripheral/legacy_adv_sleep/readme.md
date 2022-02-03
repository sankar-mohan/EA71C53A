---
grand_parent: Examples applications
parent: BLE Building Blocks
title: Legacy Advertisement
has_children: true
has_toc: false
nav_order: 62

family: PIC32CX-BZ
market:
  - wireless
---
[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)
#  BLE Sleep Mode Legacy Advertisements

[Getting Started](../../readme.md)

[Getting Started with Peripheral Building Blocks](../../peripheral/readme.md)

[**BLE Sleep Mode Legacy Advertisements**](readme.md)

## Introduction

This document will help users to enable "sleep/standby" mode with
BLE Advertisements on WBZ451 Curiosity board using MPLAB Code Configurator(MCC)
In this basic application example the advertisement interval will be set to 1 sec.
The advertisement interval will dictate the application sleep time.

Users of this document can choose to just run the precompiled
Application Example hex file on the WBZ451 Curiosity Board and
experience the demo or can go through the steps involved in developing
this Application from scratch

## Recommended Reads

1.  [FreeRtos BLE App Initialize](../freertos_ble_stack_init_peripheral.md)

2.  [Low Power Notes](../../../../docs/lowpower.md)

## Hardware Required

  |**Tool**                    | **Qty**  |
  |----------------------------| ---------|
  |WBZ451 Curiosity Board      | 1        |
  |Micro USB cable             | 1        |
  |[Power Debugger](https://www.microchip.com/en-us/development-tool/ATPOWERDEBUGGER)/Multimeter | 1        |

## SDK Setup

1.  [SDK Setup](../../../../docs/pic32cx_bz2_wbz45x_sdk_setup.md)

## Software

1.  [TeraTerm](https://ttssh2.osdn.jp/index.html.en)

## Smartphone App

1.  Light Blue

## Programming the precompiled hex file or Application Example

### Programming the hex file using MPLABX IPE

1.  Precompiled Hex file is located <a href="hex/"> here </a>

2.  Follow the steps mentioned [here](https://microchipdeveloper.com/ipe:programming-device)

 **Caution:** Users should choose the correct Device and Tool information
### Programming the Application using MPLABX IDE

1.  Follow steps mentioned in section "How to open, build and program an existing application example" of [SDK Setup document](../../../../docs/pic32cx_bz2_wbz45x_sdk_setup.md)

2.  Open and program the Application Example <a href="firmware"> legacy_adv_sleep </a>  in MPLABX IDE

## Demo Description

This Application Example enables users to enable sleep mode while transmitting non Connectable, Undirected BLE Advertisements. On reset demo will print "Advertising"
on a terminal emulator like TeraTerm, this denotes start of advertisements, Device will enter sleep mode periodically based on the advertisement interval which is set to 1 sec for this example

## Testing

Connect the WBZ45x Curiosity board to PC, program the precompiled hex
file or application example as mentioned. Open TeraTerm @ (Speed:
115200, Data: 8-bit, Parity: none, stop bits: 1 bit, Flow control:
none). Reset the board. Upon reset, "Advertising" message is displayed
on the TeraTerm. User can open the LightBlue App on Smartphone to scan
for Advertisements. Device with device name "Microchip" will
appear.

![Light Blue App](media/sleep_legacy_adv_1.png)

users using a wireshark sniffer can examine the complete Application Payload sent

![Light Blue App](media/sleep_legacy_adv_6.png)

Users can use another WBZ451 Curiosity Board configured as [BLE scanner](../../central/legacy_scan/readme.md)

## Current Consumption Measurement

- Connect the Multimeter/Power Debugger to Power Measurement Header J6. Power on the Board. If using Power Debugger, users can use [Data Visualizer](https://www.microchip.com/en-us/development-tools-tools-and-software/embedded-software-center/atmel-data-visualizer) to measure the current consumption

Current measured in sleep/standby mode is 577.9 uA and avg current consumption is around 624.8 uA
Users of this Early adopter package should go through the [known issues](../../../../../../../Documentation/Known%20Issues.pdf) document and understand the limitations if any with the current low power mode implementation

![Light Blue App](media/sleep_legacy_adv_13.png)

## Developing this Application from scratch using MPLAB Code Configurator

This section explains the steps required by a user to develop this application example from scratch using MPLABx Code Configurator

**Tip:** New users of MPLAB Code Configurator are recommended to go through the [overview](https://onlinedocs.microchip.com/pr/GUID-1F7007B8-9A46-4D03-AEED-650357BA760D-en-US-6/index.html?GUID-B5D058F5-1D0B-4720-8649-ACE5C0EEE2C0).


1.  Create a new MCC Harmony Project -- [link](../../../../docs/creating_new_mplabx_harmony_project.md) for
    instructions

2.  Import component configuration -- This step helps users
    setup the basic components required to start their
    Application Development
    Users should follow the instructions mentioned
    [here](https://microchipdeveloper.com/mcc:peripheralconfig) to import the component configuration.

    **Tip:** Import and Export functionality of component configuration will help users to start from a known working setup of configuration

3.  Accept Dependencies or satisfiers, select "Yes"

4.  Verify if the Project Graph window has all the expected configuration

    ![](media/sleep_legacy_adv_2.png)

## Verify Advertisement, system sleep and RTC Clock Source Configuration
1.  Select **BLE_Stack** component in project graph

    ![](media/sleep_legacy_adv_3.png)
> **Tip:** advertisement payload can be configured by user here

2. Select **clock configuration**

    ![](media/sleep_legacy_adv_11.PNG)

3. Configure **RTC clock configuration**

    ![](media/sleep_legacy_adv_12.png)
> **Tip:** The configuration bits will be generated after user Generates code

## Generate Code [link](../../../../docs/generate_code.md) for instructions

## Files and Routines Automatically generated by the MCC
After generating the program source from MCC interface by clicking Generate Code, the BLE configuration can be found in the following project directories

![](media/legacy_adv_4.png)


The [OSAL](http://ww1.microchip.com/downloads/en/DeviceDoc/MPLAB%20Harmony%20OSAL%20Libraries%20Help%20v2.06.pdf), RF System, BLE System  initialization routine executed during program initialization can be found in the project files. This initialization routine is automatically generated by the MCC

![](media/legacy_adv_5.png)

The BLE stack initialization routine excuted during Application Initialization can be found in project files. This intitialization routine is automatically generated by the MCC. This call initializes and configures the GAP, GATT, SMP, L2CAP and BLE middleware layers.

During system sleep, clock (system PLL) will be disabled and syatem tick will be turned off. FreeRTOS timer needs to be componsated for the time spent in sleep. RTC timer which works in the sleep mode is used to accomplish this.  RTC timer will be initialized after BLE stack initialization.
![](media/sleep_legacy_adv_7.png)

Autogenerated, adverisement Data Format

![](media/legacy_adv_8.png)


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

MCC generates and adds the code to initialize the BLE Stack GAP,
GATT, L2CAP and SMP in *APP_BleStackInit()* function

-   APP_BleStackInit() is the API that will be called inside the
    Applications Initial State -- APP_STATE_INIT in app.c

## User Application Development

### Include

-   user action is required as mentioned [here](../../../../docs/user_action.md)
-   definitions.h in all the files where UART will be used to print debug information
>Tip: definitions.h is not specific to just UART peripheral, instead it should be included in all application source files where peripheral functionality will be exercised  


### Set PUBLIC Device Address

-   BLE_GAP_SetDeviceAddr(&devAddr);

This API can be called in APP_BleConfig() located in file app_ble.c

```
    BLE_GAP_Addr_T devAddr;
    devAddr.addrType = BLE_GAP_ADDR_TYPE_PUBLIC;
    devAddr.addr[0] = 0xA1;
    devAddr.addr[1] = 0xA2;
    devAddr.addr[2] = 0xA3;
    devAddr.addr[3] = 0xA4;
    devAddr.addr[4] = 0xA5;
    devAddr.addr[5] = 0xA6;

    // Configure device address
    BLE_GAP_SetDeviceAddr(&devAddr);
```
![](media/legacy_adv_10.PNG)

### Start Advertisement

-   BLE_GAP_SetAdvEnable(0x01, 0);

This API is called in the Applications initialstate - APP_STATE_INIT in app.c

![](media/legacy_adv_9.png)

Users can exercise various other BLE Advertisement functionalities by
using [BLE Stack API](../../docs/api/driver/ble/docs/html/modules.html)


## Where to go from here

-   [BLE Connection](../../peripheral/connection/readme.md)
