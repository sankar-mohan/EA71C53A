---
parent: Release notes
title: Zigbee Combined Interface App
has_children: false
has_toc: false
nav_order: 300

---

[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)
# PIC32CX-BZ2/WBZ45 Zigbee Combined Interface App Firmware Release Notes
____
## Release ZB_v5.0.0_3.1_2.0E

### Enhancements
+ Updated the application to latest Zigbee stack base.
+ Added support for WBZ451 module

### Bug fixes
- None

### Known Issues
- None


## Release ZB_v5.0.0.0.2_1.2E

### Enhancements
+ Updated the application to latest Zigbee stack base.
+ The Combined Interface getting hang after few min of run is fixed (CHMRSW-661).

### Bug fixes
- CHMRSW-661

### Known Issues
- None

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
1. Button Control(SW2) support on pic32cx board to issue resetToFN for OOBE.

### Bug fixes
- None

### Known Issues
- None
____

## Release ZB_v5.0.0.0.1_1.1E

### New Features
1. First internal release of Zigbee ,Pre-EA (RC1)
2. Supports zigbee application device type on Harmony Environment.

### Bug fixes
- None

### Known Issues
- Recommend to use invokeCommissioning 2 0 instead of setPermitJoin command from Uart for any manual joining process

### Development Tools
- [MPLAB X v5.45](https://www.microchip.com/mplab/mplab-x-ide)
- [MPLAB® XC32 C/C++ Compiler v2.50](https://www.microchip.com/mplab/compilers)
- Device Pack: PIC32CX-BZ_DFP (1.0.55)
