---
grand_parent: Examples applications
parent: BLE Building Blocks
title: Peripheral Building Blocks
has_children: true
has_toc: false
nav_order: 22

family: PIC32CX-BZ
market:
  - wireless
---
[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)
## Peripheral Building Blocks
BLE Peripheral devices send advertisement packets and accept connection requests from another [central device](../central/readme.md).  Connected BLE devices, enable bi-directional data transfer using the profiles and services. There are certain use cases where a connection between the devices (Peripheral & Central) is not essential and advertisement feature (transmit only, no receive) is sufficient

![](media/image10.PNG)

### SDK Setup
-	[SDK Setup](../../../docs/pic32cx_bz2_wbz45x_sdk_setup.md)

**Note:** SDK Setup is mandatory step before users start developing app examples based of Building Blocks

**Tip:** New users of MPLAB Code Configurator are recommended to go through the [overview](https://onlinedocs.microchip.com/pr/GUID-1F7007B8-9A46-4D03-AEED-650357BA760D-en-US-6/index.html?GUID-B5D058F5-1D0B-4720-8649-ACE5C0EEE2C0).
### Blocks
-   [Create FreeRTOS project and configure BLE stack using MPLAB X Harmony Code Configurator 3](freertos_ble_stack_init_peripheral.md)
	-	Create a new MCC Harmony project
	-	configure components like FreeRTOS and BLE Stack
	-	Init BLE Stack
-   [Adding UART](../chip_peripherals/uart/readme.md)
	-	print Hello World

-   [Legacy Advertisments](legacy_adv_sleep/readme.md)

	-	Transmit Data Payload using BLE Legacy Adv(no BLE connection)
-	[Extended Advertisment](ext_adv/readme.md)
	-	Transmit Data Payload using BLE Extended Adv(no BLE connection)
-   [Connection](connection/readme.md)
	-	Configure stack for Peripheral Functionality
	-	Connection between Peripheral and Central Device
-   [Profiles and services](profiles_services/readme.md)
    -   [Transparent UART Peripheral (Legacy Adv + Connection + Data Exchange)](profiles_services/trp_uart/readme.md)
    	- Data Exchange between 2 devices(peripheral and central) after a BLE connection using Microchip Transparent UART profile and service
    -   [Custom Service Peripheral (Legacy Adv + Connection + Data Exchange)](profiles_services/custom_service/readme.md)
    	- Data Exchange between 2 devices(peripheral and central) after a BLE connection using Custom Service
