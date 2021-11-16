---
title: Examples applications
has_children: true
has_toc: false
nav_order: 2

family: PIC32CX-BZ
market:
  - wireless
---
[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)
# Examples applications

The following applications are provided to demonstrate the typical use case of BLE, Zigbee and Multiprotocol(BLE + Zigbee) with PIC32CX_BZ2/WBZ45x devices.

## BLE

### BLE Building Blocks

* [Getting Started](ble/building_blocks/readme.md)
	* [Peripheral](ble/building_blocks/peripheral/readme.md)
		* [FreeRtos and BLE Stack Setup](ble/building_blocks/peripheral/freertos_ble_stack_init_peripheral.md)
		* [Adding UART](ble/building_blocks/chip_peripherals/uart/readme.md)
		* [Legacy Advertisement](ble/building_blocks/peripheral/legacy_adv_sleep/readme.md)
		* [Extended Advertisement](ble/building_blocks/peripheral/ext_adv/readme.md)
		* [Connection](ble/building_blocks/peripheral/connection/readme.md)
		* [Profiles and Services](ble/building_blocks/peripheral/profiles_services/readme.md)
		* [Transparent UART](ble/building_blocks/peripheral/profiles_services/trp_uart/readme.md)
	* [Central](ble/building_blocks/central/readme.md)
		* [FreeRtos and BLE Stack Setup](ble/building_blocks/central/freertos_ble_stack_init_central.md)
		* [Adding UART](ble/building_blocks/chip_peripherals/uart/readme.md)
		* [Legacy Scan](ble/building_blocks/central/legacy_scan/readme.md)
		* [Scan Extended Advertisement](ble/building_blocks/central/scan_ext_adv/readme.md)
		* [Connection](ble/building_blocks/central/connection/readme.md)
		* [Profiles and Services](ble/building_blocks/central/profiles_services/readme.md)
		* [Transparent UART](ble/building_blocks/central/profiles_services/trp_uart/readme.md)
		* [Transparent UART Multilink](ble/building_blocks/central/profiles_services/trp_uart/readme.md)

### BLE Advanced Applications
* [BLE Sensor App (Sleep)](ble/advanced_applications/ble_sensor_app/readme.md)
* [BLE UART Demo](ble/advanced_applications/ble_uart_demo/readme.md)

## Zigbee
* [Zigbee Combined Interface/Gateway](zigbee/readme.md)
* [Zigbee Lights](zigbee/readme.md)
* [Zigbee Multisensor(sleep)](zigbee/zigbee_multisensor/readme.md)

## Multiprotocol (BLE + Zigbee)
* [Multiprotocol BLE and Zigbee Basic ](multiprotocol/ble_zigbee_basic/readme.md)
* [Multiprotocol Zigbee Light Provisioning over BLE ](multiprotocol/ble_zigbee_light_prov/readme.md)
* [Multiprotocol Zigbee Multisensor Provisioning over BLE ](multiprotocol/ble_zigbee_multiSensor_prov/readme.md)
