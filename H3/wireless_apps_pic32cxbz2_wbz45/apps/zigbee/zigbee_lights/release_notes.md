---
parent: Release notes
title: Zigbee Combined Interface App
has_children: false
has_toc: false
nav_order: 460

---

[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)
# PIC32CX-BZ2/WBZ45 Zigbee Lights App Firmware Release Notes

____
## Release ZB_v5.0.0_3.1_2.0E (02/02/2022)

### Enhancements
+ Updated the application to latest Zigbee stack base.
+ Added support for WBZ451 module
+ Example converted to MCC based Project

### Bug fixes
- None

### Known Issues
- None

____

## Release ZB_v5.0.0.0.2_1.2E

### Enhancements
+ Updated the application to latest Zigbee stack base.

### Bug fixes
- None

### Known Issues
- In manual commissioing mode when the device retains its state as FN and when reset is strobbed then Restore ID (one of the ID) is not proper which causes system reset.
____

## Release ZB_v5.0.0.0.2_1.1E

### Enhancements
+ Implementation of new programming model to handle all the events from the stack and middleware at one file in the application.
+ Information Base Access Method for reading MACID.

### Bug fixes
- None

### Known Issues
- None
____

## Release ZB_v5.0.0.0.1_1.2E

### New Features
1. RGB Color Control Added for Light Device type.
2. Button Control(SW2) support on pic32cx board to issue resetToFN for OOBE.

### Bug fixes
- None

### Known Issues
- Recommend to use invokeCommissioning 2 0 instead of setPermitJoin command from Uart for any manual joining process

____

## Release ZB_v5.0.0.0.1_1.1E

### New Features
1. First internal release of Zigbee ,Pre-EA (RC1)
2. Supports zigbee application device type on Harmony Environment.

### Bug fixes
- None

### Known Issues
- None

### Development Tools
- [MPLAB X v5.45](https://www.microchip.com/mplab/mplab-x-ide)
- [MPLAB® XC32 C/C++ Compiler v3.01](https://www.microchip.com/mplab/compilers)
- Device Pack: PIC32CX-BZ_DFP (1.0.80)
