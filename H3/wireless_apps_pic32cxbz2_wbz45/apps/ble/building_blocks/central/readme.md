---
grand_parent: Examples applications
parent: BLE Building Blocks
title: Central Building Blocks
has_children: true
has_toc: false
nav_order: 32

family: PIC32CX-BZ
market:
  - wireless
---
[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)
## Central Building Blocks
BLE Central devices are devices that discover and listen to BLE devices that are advertising. These devices are capable of establishing connection to [peripherals](../peripheral/readme.md). Central devices are also capable of establishing multiple connections(multilink) at the same time.
There are certain use cases where a connection between the devices (Peripheral & Central) is not essential and scanning (Receive only, no transmit) feature alone is sufficient. These type of devices are called observers.

**Tip:** New MPLAB Code Configurator Users are recommended to go through the [overview](https://onlinedocs.microchip.com/pr/GUID-1F7007B8-9A46-4D03-AEED-650357BA760D-en-US-6/index.html?GUID-B5D058F5-1D0B-4720-8649-ACE5C0EEE2C0). Users can add/remove different components like peripheral support and other wireless functionality by following steps mentioned [here](https://microchipdeveloper.com/mplabx:mcc).

![](media/image10.png)

### SDK Setup
-	[SDK Setup](../../../docs/pic32cx_bz2_wbz45x_sdk_setup.md)
**Note:** SDK Setup is mandatory step before users start developing app examples based of Building Blocks

### Blocks
-   [FreeRTOS and BLE Stack Setup](freertos_ble_stack_init_central.md)
	-	Create a new MCC Harmony project
	-	configure Harmony components like FreeRTOS and BLE Stack
	-	Init BLE Stack
-   [Adding UART](../chip_peripherals/uart/readme.md)
	-	print Hello World
-   [Legacy Scan](legacy_scan/readme.md)
	-	Receive Data Payload using BLE Legacy Scan(no BLE connection)
-	[Scan Extended Advertisment](scan_ext_adv/readme.md)
	-	Receive Data Payload using BLE Extended Scan(no BLE connection)
-   [Connection](connection/readme.md)
	-	Configure stack for Central Functionality
	-	Connection between Peripheral and Central Device  
-   [Profiles and services](profiles_services/readme.md)
    -   [Microchip propreitary - Transparent UART Central (Legacy Scan + Connection + Data Exchange)](profiles_services/trp_uart/readme.md)
    	- Data Exchange between 2 devices(peripheral and central) after a BLE connection using Microchip Transparent UART profile and service
        - [Multilink - Transparent UART Central (Legacy Scan + Multilink Connection + Data Exchange between Multiple Links)](profiles_services/ml_trp_uart/readme.md)
